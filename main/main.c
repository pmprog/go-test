
#include "includes.h"
#include "video.h"

#define min(a,b) (a > b ? b : a)
#define max(a,b) (a > b ? a : b)

odroid_battery_state battery_state;
odroid_gamepad_state pad_state;
odroid_gamepad_state pad_pstate;
int px = 0;
int py = 0;

IRAM_ATTR void app_drawcol(uint8_t r, uint8_t g, uint8_t b)
{
	video_setpixel(px, py, r, g, b);

	px = (px + 1) % 320;
	if( px == 0 )
	{
		//video_poke(py);
		py = (py + 1) % 240;
		if( py == 0 ) 
		{
			video_poke(888);
		}
	}
	
}

void app_main(void)
{
	uint8_t r = 128;
	uint8_t g = 128;
	uint8_t b = 128;

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

	odroid_input_gamepad_read(&pad_pstate);
	while (true)
	{
		if( px == 0 )
		{
			odroid_input_gamepad_read(&pad_state);

			if( pad_state.values[ODROID_INPUT_UP] ) // && !pad_pstate.values[ODROID_INPUT_UP] )
			{
				printf("app_main:UP pressed\n");
				r = min( r+1, 255 );
			}
			if( pad_state.values[ODROID_INPUT_DOWN] ) // && !pad_pstate.values[ODROID_INPUT_DOWN] )
			{
				printf("app_main:DOWN pressed\n");
				r = max( r-1, 0 );
			}
			if( pad_state.values[ODROID_INPUT_LEFT] ) // && !pad_pstate.values[ODROID_INPUT_LEFT] )
			{
				printf("app_main:LEFT pressed\n");
				g = max( g-1, 0 );
			}
			if( pad_state.values[ODROID_INPUT_RIGHT] ) // && !pad_pstate.values[ODROID_INPUT_RIGHT] )
			{
				printf("app_main:RIGHT pressed\n");
				g = min( g+1, 255 );
			}
			if( pad_state.values[ODROID_INPUT_A] ) // && !pad_pstate.values[ODROID_INPUT_A] )
			{
				printf("app_main:A pressed\n");
				b = min( b+1, 255 );
			}
			if( pad_state.values[ODROID_INPUT_B] ) // && !pad_pstate.values[ODROID_INPUT_B] )
			{
				printf("app_main:B pressed\n");
				b = max( b-1, 0 );
			}

			if( pad_state.values[ODROID_INPUT_MENU] ) // && !pad_pstate.values[ODROID_INPUT_MENU] )
			{
				printf("app_main:MENU pressed\n");
				video_poke( 2 );
			}
		}
		app_drawcol(r, g, b);

		pad_pstate = pad_state;	// Copy pad state
		//vTaskDelay(5 / portTICK_PERIOD_MS);
	}

	video_deinit();
}

