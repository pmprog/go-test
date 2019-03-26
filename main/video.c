
#include "video.h"

void video_task(void* arg);

uint16_t* video_buffer;
uint16_t* video_pokevalue;

QueueHandle_t video_queue;
volatile bool video_running = false;

void video_init()
{
	printf("video_init:ili9341_init()\n");
	ili9341_init();

	printf("video_init:heap_caps_malloc()\n");
	video_buffer = (uint16_t*)heap_caps_malloc(320*240*2, MALLOC_CAP_SPIRAM);
	if(!video_buffer)
	{
		printf("video_init:video_buffer == %d", (int)video_buffer);
		ili9341_poweroff();
		return;
	}

	printf("video_init:clear video buffer\n");
	//memset((void*)video_buffer, 0, 320*240*2);
	for(int i = 0; i < 320*240*2; i++)
	{
		video_buffer[i] = 0x0000;
	}

	printf("video_init:queuecreate\n");
	video_queue = xQueueCreate(1, sizeof(uint16_t*));
	xTaskCreatePinnedToCore(&video_task, "videoTask", 1024, NULL, 5, NULL, 1);
}

void video_deinit()
{
	video_pokevalue = 1;
	xQueueSend(video_queue, &video_pokevalue, portMAX_DELAY);
	while( video_running )
	{
	}
	free(video_buffer);
}

IRAM_ATTR uint16_t video_rgbtoi16(uint8_t r, uint8_t g, uint8_t b)
{
	uint16_t rx = ((uint16_t)r & (uint16_t)0x00f8) << 8;
	uint16_t gx = ((uint16_t)g & (uint16_t)0x00fc) << 3;
	uint16_t bx = ((uint16_t)b & (uint16_t)0x00f8) >> 3;
	//return ((rx | gx | bx) << 8) | ((rx | gx | bx) >> 8);
	return (rx | gx | bx);
}

IRAM_ATTR void video_i16torgb(uint16_t colour, uint8_t* r, uint8_t* g, uint8_t* b)
{
	uint16_t swapped = colour; // (colour >> 8) | (colour << 8);
	*r = (swapped & 0xf800) >> 8;
	*g = (swapped & 0x07e0) >> 5;
	*b = (swapped & 0x001f) << 3;
}

IRAM_ATTR void video_getpixel(int x, int y, uint8_t* r, uint8_t* g, uint8_t* b)
{
	video_i16torgb( video_buffer[(y * 320) + x], r, g, b );
}

IRAM_ATTR void video_setpixel(int x, int y, uint8_t r, uint8_t g, uint8_t b)
{
	video_buffer[(y * 320) + x] = video_rgbtoi16(r, g, b);
}

IRAM_ATTR void video_task(void* arg)
{
	printf("video_task:task start\n");
	uint16_t* param = 0;
	bool abort = false;
	int rowrender = 0;
	int rendermode = 0;

	video_running = true;

	printf("video_task:loop start\n");
	while(!abort)
	{
		param = 0;
		//xQueuePeek(video_queue, &param, portMAX_DELAY);
		xQueuePeek(video_queue, &param, 1);
		if( param == 1 )
		{
			printf("video_task: poked(1)\n");
			abort = true;
		}
		if( param == 2 )
		{
			// rendermode = 1 - rendermode;
			printf("video_task: poked(2)\n");
			ili9341_write_frame_rectangleLE( 0, 0, 320, 240, video_buffer );
		}
		/*
		odroid_display_lock();
		ili9341_write_frame( video_buffer );
		odroid_display_unlock();
		*/
		
		xQueueReceive(video_queue, &param, 1);
	}

	vTaskDelete(NULL);
	video_running = false;
}

IRAM_ATTR void video_poke(uint16_t value)
{
	printf("video_poke:%d\n", (int)value);
	video_pokevalue = (uint16_t)value;
	xQueueSend(video_queue, &video_pokevalue, portMAX_DELAY);
}
