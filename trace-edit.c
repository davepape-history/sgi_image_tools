#include <stdio.h>
#include <gl.h>
#include <gl/device.h>
#include <math.h>
#include "/usr/people/dave/lib/dave.h"

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
int xcoord[NPOINTS],ycoord[NPOINTS],count,current_point=0;
int zoom=1;

void redraw()
{
 int i;
 cpack(0); clear();
 lrectwrite(0,0,xdim-1,ydim-1,image);
 cpack(LINECOLOR);
 move2i(xcoord[0]*zoom,ycoord[0]*zoom);
 for (i=1; i<count; i++)
	draw2i(xcoord[i]*zoom,ycoord[i]*zoom);
 cpack(0xff00);
 circi(xcoord[current_point]*zoom,ycoord[current_point]*zoom,2);
 swapbuffers();
}

void dump_points()
{
 int i;
 for (i=0; i<count; i++) printf("%d %d\n",xcoord[i],ycoord[i]);
 printf("#\n");
 redraw();
}

void get_point()
{
 short val;
 long origx,origy;
 getorigin(&origx,&origy);
 qread(&val);  xcoord[current_point] = (val-origx)/zoom;
 qread(&val);  ycoord[current_point] = (val-origy)/zoom;
 current_point=(current_point+1)%count;
 redraw();
}

void read_point_list(char *fname)
{
 FILE *fp;
 char line[256];
 if ((fp = fopen(fname,"r")) == NULL) { perror(fname); exit(); }
 count=0;
 while (fgets(line,256,fp))
	if (line[0] != '#') {
		sscanf(line,"%d%d",&xcoord[count],&ycoord[count]);
		count++;
		}
 fclose(fp);
 if (!count) {
	fprintf(stderr,"Error: no points read from \"%s\"\n",fname);
	exit();
	}
}

main(int argc,char **argv)
{
 int dev;
 short val;
 if (argc<3) {
	fprintf(stderr,"Usage: %s image point_list [-z zoom]\n",argv[0]);
	exit();
	}
 if (!read_sgi_image_long(argv[1],&image,&xdim,&ydim)) exit();
 read_point_list(argv[2]);
 if ((argc>4) && (!strcmp(argv[3],"-z"))) zoom = atoi(argv[4]);
 open_window(argv[1],xdim*zoom,ydim*zoom);
 rectzoom((float)zoom,(float)zoom);
 redraw();
 qdevice(LEFTMOUSE);
 qdevice(RETKEY);
 qdevice(ESCKEY);
 qdevice(LEFTARROWKEY);
 qdevice(RIGHTARROWKEY);
 tie(LEFTMOUSE,MOUSEX,MOUSEY);
 while (1) {
	dev = qread(&val);
	if (val) {
		if (dev==LEFTMOUSE) get_point();
		else if (dev==LEFTARROWKEY) {
		    current_point = (current_point+count-1)%count;
		    redraw();
		    }
		else if (dev==RIGHTARROWKEY) {
		    current_point = (current_point+1)%count;
		    redraw();
		    }
		else if (dev==RETKEY) dump_points();
		else if (dev==REDRAW) redraw();
		else if (dev==ESCKEY) exit();
		}
	}
}
