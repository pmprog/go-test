
#include "includes.h"

odroid_battery_state battery_state;
odroid_gamepad_state pad_state;
int px = 0;
int py = 0;

void app_drawcol(uint16_t c);

void app_main(void)
{
	printf("app_main:nvs_flash_init()\n");
	nvs_flash_init();
	printf("app_main:odroid_system_init()\n");
	odroid_system_init();
	printf("app_main:video_init()\n");
	video_init();
	printf("app_main:odroid_input_gamepad_init()\n");
	odroid_input_gamepad_init();
	printf("app_main:odroid_input_battery_level_init()\n");
	odroid_input_battery_level_init();

	while (true)
	{
		printf("app_main:odroid_input_gamepad_read()\n");
		odroid_input_gamepad_read(&pad_state);

		uint16_t c = 0xFFFF;

		if( pad_state.values[ODROID_INPUT_UP] )
		{
			printf("app_main:UP pressed\n");
			c = 0xFF00;
		}
		if( pad_state.values[ODROID_INPUT_A] )
		{
			printf("app_main:A pressed\n");
			c = 0x0FF0;
		}
		if( pad_state.values[ODROID_INPUT_B] )
		{
			printf("app_main:B pressed\n");
			c = 0x00FF;
		}
		app_drawcol(c);
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
