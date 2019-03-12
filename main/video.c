
#include "includes.h"

void video_task(void* arg);

uint16_t* video_buffer;

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
	uint16_t* param = 1;
	xQueueSend(video_queue, &param, portMAX_DELAY);
	while( video_running )
	{
	}
	free(video_buffer);
}

int16_t video_getpixel(int x, int y)
{
	return video_buffer[(y * 320) + x];
}

void video_setpixel(int x, int y, int16_t c)
{
	printf("video_task:setpixel(%d,%d,%d)\n",x,y,c);
	video_buffer[(y * 320) + x] = c;
}

void video_task(void* arg)
{
	printf("video_task:task start\n");
	uint16_t* param = 0;
	bool abort = false;
	video_running = true;

	printf("video_task:loop start\n");
	while(!abort)
	{
		printf("video_task:queuepeek\n");
		xQueuePeek(video_queue, &param, 1);
		if( param == 1 )
		{
			printf("video_task:queueshutdown\n");
			abort = true;
			xQueueReceive(video_queue, &param, portMAX_DELAY);
		} else {
			printf("video_task:drawframe\n");
			odroid_display_lock();
			ili9341_write_frame_rectangleLE(0, 0, 320, 240, video_buffer);
			odroid_display_unlock();
		}
	}

	vTaskDelete(NULL);
	video_running = false;
}
