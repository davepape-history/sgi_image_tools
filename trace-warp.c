#include <stdio.h>
#include <gl.h>
#include <gl/device.h>
#include <math.h>
#include "../lib/dave.h"

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
int inxcoord[NPOINTS],inycoord[NPOINTS];
float texcoord[NPOINTS][2];
int zoom=1;
int disp_poly = 1;

void redraw()
{
 int i;
 float v[2];
 cpack(0); clear();
 lrectwrite(0,0,xdim-1,ydim-1,image);
 if (disp_poly) {
	cpack(0xffffffff);
	texbind(TX_TEXTURE_0,1);
	afunction(0,AF_NOTEQUAL);
	concave(TRUE);
	bgnpolygon();
	for (i=0; i<count; i++) {
	   v[0] = xcoord[i]*zoom;  v[1] = ycoord[i]*zoom;
	   t2f(texcoord[i]);
	   v2f(v);
	   }
	endpolygon();
	concave(FALSE);
	texbind(TX_TEXTURE_0,0);
	afunction(0,AF_ALWAYS);
	}
 cpack(0xff);
 move2i(xcoord[0]*zoom,ycoord[0]*zoom);
 for (i=1; i<count; i++) draw2i(xcoord[i]*zoom,ycoord[i]*zoom);
 draw2i(xcoord[0]*zoom,ycoord[0]*zoom);
 cpack(0xff00);
 for (i=0; i<count; i++)
	circi(xcoord[i]*zoom,ycoord[i]*zoom,2);
 swapbuffers();
}

void dump_points()
{
 int i;
 for (i=0; i<count; i++) printf("%d %d %d %d\n",xcoord[i],ycoord[i],inxcoord[i],inycoord[i]);
 redraw();
}

int prevmx,prevmy;

void move_point(int doit)
{
 long mx,my,dx,dy,origx,origy;
 mx = getvaluator(MOUSEX);
 my = getvaluator(MOUSEY);
 dx = mx-prevmx;
 dy = my-prevmy;
 if ((abs(dx)>2) || (abs(dy)>2) || (doit)) {
	getorigin(&origx,&origy);
	xcoord[current_point] = (mx-origx)/zoom;
	ycoord[current_point] = (my-origy)/zoom;
	prevmx = mx;  prevmy=my;
	redraw();
	}
}

void find_current_point()
{
 long x,y,origx,origy,i,dist,d;
 getorigin(&origx,&origy);
 x = getvaluator(MOUSEX) - origx;
 y = getvaluator(MOUSEY) - origy;
 x/=zoom;
 y/=zoom;
 current_point = 0;
 dist = (xcoord[0]-x)*(xcoord[0]-x)+(ycoord[0]-y)*(ycoord[0]-y);
 for (i=1; i<count; i++) {
	d = (xcoord[i]-x)*(xcoord[i]-x)+(ycoord[i]-y)*(ycoord[i]-y);
	if (d<dist) {
		dist = d;
		current_point = i;
		}
	}
}

void read_point_list(char *fname)
{
 FILE *fp;
 char line[256];
 if ((fp = fopen(fname,"r")) == NULL) { perror(fname); exit(); }
 count=0;
 while (fgets(line,256,fp))
	if (line[0] != '#') {
		sscanf(line,"%d%d",&inxcoord[count],&inycoord[count]);
		xcoord[count] = inxcoord[count];
		ycoord[count] = inycoord[count];
		texcoord[count][0] = (float)xcoord[count]/xdim;
		texcoord[count][1] = (float)ycoord[count]/ydim;
		count++;
		}
 fclose(fp);
 if (!count) {
	fprintf(stderr,"Error: no points read from \"%s\"\n",fname);
	exit();
	}
}

void init_texmap(char *fname)
{
 ULONG *teximg,*p;
 int texx,texy,i,j;
 static float texprops[5] = {
                        TX_MINFILTER, TX_POINT,
                        TX_MAGFILTER, TX_POINT,
                        TX_NULL };
 static float tevprops[1] = { TV_NULL };
 tevdef(1,0,tevprops);
 tevbind(TV_ENV0,1);
 if (!read_sgi_image_long(fname,&teximg,&texx,&texy)) exit();
 for (j=0, p=teximg; j<texy; j++)
   for (i=0; i<texx; i++, p++)
	*p = (*p & 0xffffff) | (*p & 0xff) << 24;
 texdef2d(1,4,texx,texy,teximg,0,texprops);
 free(teximg);
}

main(int argc,char **argv)
{
 int dev,moving;
 short val;
 if (argc<3) {
	fprintf(stderr,"Usage: %s warp-image bg-image point_list [-z zoom]\n",argv[0]);
	exit();
	}
 if (!read_sgi_image_long(argv[2],&image,&xdim,&ydim)) exit();
 read_point_list(argv[3]);
 if ((argc>5) && (!strcmp(argv[4],"-z"))) zoom = atoi(argv[5]);
 open_window(argv[1],xdim*zoom,ydim*zoom);
 init_texmap(argv[1]);
 rectzoom((float)zoom,(float)zoom);
 redraw();
 qdevice(LEFTMOUSE);
 qdevice(MOUSEX);
 qdevice(MOUSEY);
 qdevice(RETKEY);
 qdevice(ESCKEY);
 qdevice(DKEY);
 moving = 0;
 while (1) {
	dev = qread(&val);
	if (val) {
		if (dev==LEFTMOUSE) {
			find_current_point();
			moving = 1;
			move_point(1);
			}
		else if (((dev==MOUSEX) || (dev==MOUSEY)) && (moving))
				move_point(0);
		else if (dev==RETKEY) dump_points();
		else if (dev==REDRAW) redraw();
		else if (dev==ESCKEY) exit();
		else if (dev==DKEY) { disp_poly = !disp_poly; redraw(); }
		}
	else if (dev==LEFTMOUSE) moving = 0;
	}
}
