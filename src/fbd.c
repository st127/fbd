/*
	fbd
	File:/src/fbd.c
	Date:2021.10.02
	By MIT License.
	Copyright (c) 2021 Suote127.All rights reserved.
*/

#include<stdio.h>
#include<stdlib.h>
#include<stddef.h>
#include<stdint.h>
#include<string.h>

#include<unistd.h>
#include<fcntl.h>
#include<sys/mman.h>
#include<sys/ioctl.h>
#include<linux/fb.h>

#include"fbd/fbd.h"

#define fbd_abs(a) ((a) < 0 ? (-(a)) : (a))

Fbd_Device *fbd_open(char const *path)
{
	Fbd_Device *dev = (Fbd_Device*)malloc(sizeof(Fbd_Device));
	if (!dev)
		return NULL;
	
	dev->fd = open(path,O_RDWR);
	if (dev->fd < 0)
		goto err;
	
	if (ioctl(dev->fd,FBIOGET_FSCREENINFO,&(dev->finfo)) < 0)
		goto err;
	if (ioctl(dev->fd,FBIOGET_VSCREENINFO,&(dev->vinfo)) < 0)
		goto err;
	
	dev->screenSize	= dev->finfo.smem_len;
	dev->xRes	= dev->vinfo.xres;
	dev->yRes	= dev->vinfo.yres;
	dev->bpp	= dev->vinfo.bits_per_pixel;
	dev->lineLength = dev->finfo.line_length;

	/*	Support 32bit ARGB only	*/
	if (dev->bpp != 32)
		goto err;
	
	dev->map	= (uint8_t*)mmap(NULL,dev->screenSize,
					 PROT_READ | PROT_WRITE,
					 MAP_SHARED,dev->fd,0);
	if (!dev->map)
		goto err;
	/*	Disable auto blanking	*/
	int ttyFd = open("/dev/tty",O_WRONLY);
	if (ttyFd < 0)
		goto err;
	write(ttyFd,"\033[9;0]",6);
	close(ttyFd);
	puts("\033[?25l");
	
	fbd_clear(dev);


	return dev;
	
err:
	free(dev);
	return NULL;
}

void fbd_close(Fbd_Device *dev)
{
	close(dev->fd);
	free(dev);
	int ttyFd = open("/dev/tty",O_WRONLY);
	if (ttyFd < 0)
		return;
	write(ttyFd,"\033[9;10]",7);
	close(ttyFd);
	puts("\033[?25h");
	return;
}

void fbd_clear(Fbd_Device *dev)
{
	memset(dev->map,0,dev->screenSize);
	return;
}

static inline uint32_t color_convert(Fbd_Device *dev,uint8_t a,
				     uint8_t r,uint8_t g,uint8_t b)
{
	if (dev->bpp == 32) {
		return a << 24 | r << 16 | g << 8 | b;
	}
	return 0;
}

void fbd_point(Fbd_Device *dev,uint32_t x,uint32_t y,
	       uint8_t a,uint8_t r,uint8_t g,uint8_t b)
{
	uint32_t color = color_convert(dev,a,r,g,b);

	x += dev->vinfo.xoffset;
	y += dev->vinfo.yoffset;

	if (dev->bpp == 32) {
		uint32_t location		  = x + ((y * dev->lineLength) >> 2);
		((uint32_t*)(dev->map))[location] = color;
	}

	return;
}

void fbd_line(Fbd_Device *dev,uint32_t sx,uint32_t sy,
	      uint32_t ex,uint32_t ey,
	      uint8_t a,uint8_t r,uint8_t g,uint8_t b)
{
	if (sx == ex) {
		for (uint32_t y = sy;y <= ey;y++)
			fbd_point(dev,sx,y,a,r,g,b);
	}

	double k = fbd_abs((int64_t)ey - (int64_t)sy) /
		   (fbd_abs((int64_t)ex - (int64_t)sx) + 0.);

	double y = sy;
	for (uint32_t x = sx;
	     x <= ex;
	     x++) {
		fbd_point(dev,x,(uint32_t)y,a,r,g,b);
		y = y + k;
	}

	return;
}

void fbd_box(Fbd_Device *dev,uint32_t x1,uint32_t y1,
	     uint32_t x2,uint32_t y2,
	     uint8_t a,uint8_t r,uint8_t g,uint8_t b)
{
	fbd_line(dev,x1,y1,x2,y1,a,r,g,b);
	fbd_line(dev,x1,y1,x1,y2,a,r,g,b);
	fbd_line(dev,x2,y1,x2,y2,a,r,g,b);
	fbd_line(dev,x1,y2,x2,y2,a,r,g,b);

	return;
}
