/*
	fbd
	File:/test/Fbd.test.c
	Date:2021.07.28
	By MIT License.
	Copyright (c) 2021 Suote127.All rights reserved.
*/

#include<stdio.h>
#include<stdlib.h>

#include"fbd/fbd.h"

int main(void)
{
	Fbd_Device *fbdev = fbd_open("/dev/fb0");

	for (int y = 1;y <= 768;y++) {
		fbd_point(fbdev,2,y,0,255,0,0);
	}
	getchar();

	fbd_clear(fbdev);
	fbd_line(fbdev,1,1,fbd_width(fbdev),fbd_height(fbdev),0,0,255,0);
	getchar();

	fbd_clear(fbdev);
	fbd_box(fbdev,100,100,400,400,0,0,0,255);

	fbd_close(fbdev);

	return 0;
}
