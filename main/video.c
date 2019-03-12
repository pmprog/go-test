
#include "includes.h"

void video_task(void* arg);

uint16_t* video_buffer;

QueueHandle_t video_queue;
volatile bool video_running = false;

void video_init()
{
	ili9341_init();
	ili9341_clear(0xF000);

	video_buffer = (uint16_t*)heap_caps_malloc(320*240*2, MALLOC_CAP_SPIRAM);
	if(!video_buffer)
	{
		ili9341_poweroff();
		return;
	}

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
	video_buffer[(y * 320) + x] = c;
}

void video_task(void* arg)
{
	uint16_t* param = 0;
	bool abort = false;
	video_running = true;

	while(!abort)
	{
		xQueuePeek(video_queue, &param, portMAX_DELAY);
		if( param == 1 )
		{
			abort = true;
			xQueueReceive(video_queue, &param, portMAX_DELAY);
		} else {
			ili9341_write_frame_rectangleLE(0, 0, 320, 240, video_buffer);
		}
	}

	video_running = false;
}
