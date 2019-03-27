
#pragma once

#include "includes.h"

void video_init();
void video_deinit();

uint16_t video_rgbtoi16(uint8_t r, uint8_t g, uint8_t b);
void video_i16torgb(uint16_t colour, uint8_t* r, uint8_t* g, uint8_t* b);

void video_getpixel(int x, int y, uint8_t* r, uint8_t* g, uint8_t* b);
void video_setpixel(int x, int y, uint8_t r, uint8_t g, uint8_t b);

void video_poke(uint16_t value);
