
#include "includes.h"

odroid_battery_state battery_state;
odroid_gamepad_state pad_state;
int px = 0;
int py = 0;

void app_drawcol(uint16_t c);

void app_main(void)
{
	nvs_flash_init();
	odroid_system_init();
	video_init();
	odroid_input_gamepad_init();
	odroid_input_battery_level_init();

	while (true)
	{
		odroid_input_gamepad_read(&pad_state);

		if( pad_state.values[ODROID_INPUT_UP] )
		{
			app_drawcol(0xFF00);
		}
		if( pad_state.values[ODROID_INPUT_A] )
		{
			app_drawcol(0x0FF0);
		}
		if( pad_state.values[ODROID_INPUT_B] )
		{
			app_drawcol(0x00FF);
		}

        vTaskDelay(10 / portTICK_PERIOD_MS);
		app_drawcol(0xFFFF);
	}

	video_deinit();
}

void app_drawcol(uint16_t c)
{
	video_setpixel(px, py, c);

	px = (px + 1) % 320;
	if( px == 0 )
	{
		py = (py + 1) % 240;
	}
}
