#include <stdio.h>
#include <gl.h>
#include <gl/device.h>
#include <math.h>
#include "dave.h"

#define LINECOLOR 0xff
#define NPOINTS 16000

void open_window(char *name,int x,int y)
{
 foreground();
 prefsize(x,y);
 winopen(name);
 doublebuffer();
 RGBmode();
 gconfig();
 cpack(0); clear();
}

ULONG *image;
int xdim,ydim;
int xcoord[NPOINTS],ycoord[NPOINTS],count;
int zoom=1;
int numverts=0;

void redraw()
{
 int i;
 cpack(0); clear();
 lrectwrite(0,0,xdim-1,ydim-1,image);
 cpack(LINECOLOR);
 move2i(xcoord[0],ycoord[0]);
 for (i=1; i<count; i++)
	draw2i(xcoord[i],ycoord[i]);
 swapbuffers();
}

void dump_points()
{
 int i;
 if (!count) return;
 for (i=0; i<count; i++)
	printf("v %.4f %.4f 0\n",((float)xcoord[i])/zoom/xdim,
		((float)ycoord[i])/zoom/ydim);
 printf("f");
 for (i=0; i<count; i++)
	printf(" %d",i+numverts+1);
 printf("\n");
 numverts += count;
 count=0;
 redraw();
}

void get_point()
{
 short val;
 long origx,origy;
 getorigin(&origx,&origy);
 qread(&val);  xcoord[count] = val-origx;
 qread(&val);  ycoord[count] = val-origy;
 cpack(LINECOLOR);
 frontbuffer(TRUE);
 if (count) draw2i(xcoord[count],ycoord[count]);
 else move2i(xcoord[count],ycoord[count]);
 frontbuffer(FALSE);
 count++;
 if (count==NPOINTS) {
	fprintf(stderr,"Maximum number of points reached!\n");
	fprintf(stderr,"Dumping point list and restarting\n");
	dump_points();
	redraw();
	}
}

void delete_point()
{
 if (count) count--;
 redraw();
}
 
main(int argc,char **argv)
{
 int dev;
 short val;
 if (argc<2) {
	fprintf(stderr,"Usage: %s image [-z zoom]\n",argv[0]);
	exit();
	}
 if (!dpReadSgiImageLong(argv[1],&image,&xdim,&ydim)) exit();
 if ((argc>3) && (!strcmp(argv[2],"-z"))) zoom = atoi(argv[3]);
 open_window(argv[1],xdim*zoom,ydim*zoom);
 rectzoom((float)zoom,(float)zoom);
 redraw();
 qdevice(LEFTMOUSE);
 qdevice(BACKSPACEKEY);
 qdevice(RETKEY);
 qdevice(ESCKEY);
 tie(LEFTMOUSE,MOUSEX,MOUSEY);
 while (1) {
	dev = qread(&val);
	if (val) {
		if (dev==LEFTMOUSE) get_point();
		else if (dev==BACKSPACEKEY) delete_point();
		else if (dev==RETKEY) dump_points();
		else if (dev==REDRAW) redraw();
		else if (dev==ESCKEY) { dump_points(); exit(); }
		}
	}
}
