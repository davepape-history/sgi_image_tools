#include <stdio.h>
#include <gl.h>
#include <gl/device.h>
#include "dave.h"

void display_image(char *fname);
void handle_events(ULONG *img,int xdim,int ydim);

Boolean slideshow = FALSE;

main(int argc,char **argv)
{
 int i,start=1;
 char line[1024],fname[1024];
 if ((argc>1) && (!strcmp(argv[1],"-s"))) {
	slideshow = TRUE;
	start = 2;
	}
 if (argc>start) for (i=start; i<argc; i++) display_image(argv[i]);
 else while (gets(line)) {
	sscanf(line,"%s",fname);
	display_image(fname);
	}
 sginap(50);
}

void display_image(char *fname)
{
 unsigned long *img;
 int xdim,ydim,dev;
 static int window=0;
 short val;
 Boolean done;
 if (!dpReadSgiImageLong(fname,&img,&xdim,&ydim)) return;
 if (!window) {
	prefsize(xdim,ydim);
	foreground();
	window = winopen(fname);
	RGBmode();
	if (getgdesc(GD_BITS_NORM_DBL_RED)==8) doublebuffer();
	gconfig();
	qdevice(ESCKEY);
	if (slideshow) qdevice(LEFTMOUSE);
	if (slideshow) qdevice(RETKEY);
	}
 cpack(0); clear();
 wintitle(fname);
 lrectwrite(0,0,xdim-1,ydim-1,img);
 swapbuffers();
 handle_events(img,xdim,ydim);
 free(img);
}

void handle_events(ULONG *img,int xdim,int ydim)
{
 int dev;
 short val;
 if (slideshow)
     while (dev = qread(&val)) {
	if ((dev==ESCKEY) && (val)) exit();
	if (((dev==LEFTMOUSE) || (dev==RETKEY)) && (val)) break;
	else if (dev==REDRAW) lrectwrite(0,0,xdim-1,ydim-1,img), swapbuffers();
	}
 else
     while (qtest()) {
	dev = qread(&val);
	if ((dev==ESCKEY) && (val)) exit();
	if (dev==REDRAW) lrectwrite(0,0,xdim-1,ydim-1,img), swapbuffers();
	}
}
