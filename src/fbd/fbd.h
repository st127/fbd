/*
	fbd
	File:/src/fbd/fbd.h
	Date:2021.07.28
	By MIT License.
	Copyright (c) 2021 Suote127.All rights reserved.
*/

#ifndef __FBDEV_H_INC__
#define __FBDEV_H_INC__

#include<stdint.h>

#include<unistd.h>
#include<linux/fb.h>

typedef struct {
	struct fb_var_screeninfo vinfo;
	struct fb_fix_screeninfo finfo;
	size_t xRes,yRes;
	int fd;
	int bpp;
	size_t lineLength;
	size_t screenSize;
	uint8_t *map;
}Fbd_Device;

typedef struct {
	uint32_t a;
	uint32_t r;
	uint32_t g;
	uint32_t b;
}Fdb_Color;

#define fbd_width(dev) ((dev)->xRes)
#define fbd_height(dev) ((dev)->yRes)

Fbd_Device *fbd_open(char const *path);
void fbd_close(Fbd_Device *dev);
void fbd_clear(Fbd_Device *dev);
void fbd_point(Fbd_Device *dev,uint32_t x,uint32_t y,
	       uint8_t a,uint8_t r,uint8_t g,uint8_t b);
void fbd_line(Fbd_Device *dev,uint32_t sx,uint32_t sy,
	      uint32_t ex,uint32_t ey,
	      uint8_t a,uint8_t r,uint8_t g,uint8_t b);
void fbd_box(Fbd_Device *dev,uint32_t x1,uint32_t y1,
	     uint32_t x2,uint32_t y2,
	     uint8_t a,uint8_t r,uint8_t g,uint8_t b);

#endif
