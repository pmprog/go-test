
#pragma once

#include "freertos/FreeRTOS.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_event_loop.h"
#include "nvs_flash.h"
#include "esp_adc_cal.h"
#include "esp_task_wdt.h"
#include "esp_spiffs.h"
#include "esp_partition.h"
#include "esp_ota_ops.h"

#include "driver/gpio.h"
#include "driver/spi_master.h"
#include "driver/ledc.h"
#include "driver/i2s.h"
#include "driver/adc.h"
#include "driver/rtc_io.h"

#include "../components/odroid/odroid_system.h"
#include "../components/odroid/odroid_display.h"
#include "../components/odroid/odroid_input.h"
#include "../components/odroid/odroid_sdcard.h"
#include "../components/odroid/odroid_settings.h"
#include "../components/odroid/odroid_audio.h"


void video_init();
void video_deinit();
int16_t video_getpixel(int x, int y);
void video_setpixel(int x, int y, int16_t c);

