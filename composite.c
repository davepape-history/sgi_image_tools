#include <stdio.h>
#include <fcntl.h>
#include "dave.h"
#include <gl/gl.h>
#include <gl/device.h>

typedef struct {
		ULONG *img;
		int x,y;
		} image_t;

void composite(image_t *bgimage,image_t *oimage);
void get_alpha(char *fname,image_t *image);
void open_window(char *winname,int winx,int winy);
int create_menu(void);
void load_image_and_composite(image_t *bgimage);
void save_image(void);
void draw_images(image_t *bgimage,image_t *oimage,int ox,int oy);

#define LOAD	 1
#define SAVE	 2
#define QUIT	99

char *over_image_rgb, *over_image_alpha, *out_image;

main(int argc,char **argv)
{
 int menu,dev,choice;
 short val;
 image_t bgimage;
 ULONG *p;
 int i;
 if (argc<5) {
	fprintf(stderr,"Usage: %s under-image over-image over-image.alpha out-image\n",argv[0]);
	exit(1);
	}
 over_image_rgb = strdup(argv[2]);
 over_image_alpha = strdup(argv[3]);
 out_image = strdup(argv[4]);
 if (!dpReadSgiImageLong(argv[1],&bgimage.img,&bgimage.x,&bgimage.y)) exit(1);
 open_window(argv[1],bgimage.x,bgimage.y);
 for (i=bgimage.x*bgimage.y, p=bgimage.img; i; i--, p++)
	*p &= 0x00ffffff;
 draw_images(&bgimage,NULL,0,0);
 menu=create_menu();
 while (1) {
	dev = qread(&val);
	if ((dev==RIGHTMOUSE) && (val)) {
		choice = dopup(menu);
		if (choice == LOAD) load_image_and_composite(&bgimage);
		else if (choice == SAVE) save_image();
		else if (choice==QUIT) break;
		}
	else if ((dev==ESCKEY) && (val)) break;
	else if (dev==REDRAW) draw_images(&bgimage,NULL,0,0);
	}
}

void open_window(char *winname,int winx,int winy)
{
 foreground();
 prefsize(winx,winy);
 winopen(winname);
 winconstraints();
 RGBmode();
 doublebuffer();
 gconfig();
 qdevice(MOUSEX);
 qdevice(MOUSEY);
 qdevice(LEFTMOUSE);
 qdevice(RIGHTMOUSE);
 qdevice(ESCKEY);
 qdevice(BACKSPACEKEY);
 readsource(SRC_FRONT);
 cpack(0); clear(); swapbuffers(); clear();
}

int create_menu()
{
 return(defpup("Composite %t| Load image %x1| Save %x2| Quit %x99"));
}

#if 0
char *get_filename(char *prompt)
{
 char *fname;
 static char pattern[256]="*.rgb",dir[256]=".";
 char defname[256];
 bzero(defname,256);
 fname = (char *)fl_show_fselector(prompt,dir,pattern,defname);
 strcpy(dir,fl_get_directory());
 strcpy(pattern,fl_get_pattern());
 if (fname[strlen(fname)-1]=='/') return(NULL);
 return(fname);
}
#endif

void load_image_and_composite(image_t *bgimage)
{
 image_t oimage;
 char *fname;
 lrectread(0,0,bgimage->x-1,bgimage->y-1,bgimage->img);
#if 0
 fname = get_filename("Load image");
#else
 fname = over_image_rgb;
#endif
 draw_images(bgimage,NULL,0,0);
 if ((!fname) || (!*fname)) return;
 if (!dpReadSgiImageLong(fname,&oimage.img,&oimage.x,&oimage.y)) return;
#if 0
 fname = get_filename("Load alpha");
#else
 fname = over_image_alpha;
#endif
 draw_images(bgimage,NULL,0,0);
 if ((fname) && (*fname)) get_alpha(fname,&oimage);
 composite(bgimage,&oimage);
 free(oimage.img);
 lrectread(0,0,bgimage->x-1,bgimage->y-1,bgimage->img);
}

void save_image()
{
 char *fname,aname[256];
 ULONG *image,*ip;
 UCHAR *alpha,*ap;
 long xdim,ydim,i;
 getsize(&xdim,&ydim);
 image = ARRALLOC(ULONG,xdim*ydim);
 readsource(SRC_FRONT);
 lrectread(0,0,xdim-1,ydim-1,image);
#if 0
 fname = get_filename("Save image");
#else
 fname = out_image;
#endif
 if ((fname) && (*fname)) dpWriteSgiImageLong(fname,image,xdim,ydim);
 lrectwrite(0,0,xdim-1,ydim-1,image); swapbuffers();
 sprintf(aname,"%s.a",fname);
 alpha = ARRALLOC(UCHAR,xdim*ydim);
 for (i=xdim*ydim, ip=image, ap=alpha; i; i--, ip++, ap++)
	*ap = *ip >> 24;
 dpWriteSgiImageByte(aname,alpha,xdim,ydim);
 free(alpha);
 free(image);
}

void get_alpha(char *fname,image_t *image)
{
 UCHAR *alpha,*ap;
 ULONG *ip;
 int x,y,i;
 if (!dpReadSgiImageByte(fname,&alpha,&x,&y)) return;
 if ((x!=image->x) || (y!=image->y)) {
	fprintf(stderr,"Error: alpha must have same dimensions as image\n");
	return;
	}
 for (i=0, ap=alpha, ip=image->img; i<x*y; i++, ip++, ap++)
	*ip = (*ip & 0xffffff) | (*ap << 24);
 free(alpha);
}

void draw_images(image_t *bgimage,image_t *oimage,int ox,int oy)
{
 int urx,ury,llx,lly;
 blendfunction(BF_ONE,BF_ZERO);
 if (bgimage) lrectwrite(0,0,bgimage->x-1,bgimage->y-1,bgimage->img);
 else { cpack(0); clear(); }
 blendfunction(BF_SA,BF_MSA);
 if (oimage) {
	urx = ox + oimage->x - 1;
	ury = oy + oimage->y - 1;
	llx = ox;
	lly = oy;
	if (llx<0) llx = 0;
	if (lly<0) lly = 0;
	if (urx > bgimage->x-1) urx = bgimage->x-1;
	if (ury > bgimage->y-1) ury = bgimage->y-1;
	pixmode(PM_STRIDE,oimage->x);
	lrectwrite(llx,lly,urx,ury,oimage->img+(llx-ox)+(lly-oy)*oimage->x);
	pixmode(PM_STRIDE,0);
	}
 swapbuffers();
}

#define CLAMP(v,min,max) { if (v<min) v=min; else if (v>max) v=max; }

void composite(image_t *bgimage,image_t *oimage)
{
 int ox,oy,dev,redraw,dx,dy,prevx,prevy;
 short val;
 ox = oy = 0;
 draw_images(bgimage,oimage,ox,oy);
 prevx = getvaluator(MOUSEX);
 prevy = getvaluator(MOUSEY);
 while (1) {
	redraw = 0;
	dev = qread(&val);
	if ((dev == MOUSEX) || (dev == MOUSEY)) {
		dx = getvaluator(MOUSEX) - prevx;
		dy = getvaluator(MOUSEY) - prevy;
		if (abs(dx)+abs(dy) > 3) {
		   prevx += dx;
		   prevy += dy;
		   if (getbutton(LEFTSHIFTKEY) || getbutton(RIGHTSHIFTKEY)) {
			dx/=3;
			dy/=3;
			}
		   if (getbutton(LEFTMOUSE)) {
			ox += dx;
			oy += dy;
			CLAMP(ox,1-oimage->x,bgimage->x-1);
			CLAMP(oy,1-oimage->y,bgimage->y-1);
			redraw=1;
			}
		   }
		}
	else if (dev==REDRAW) redraw=1;
	else if ((dev==ESCKEY) && (val)) break;
	else if ((dev==RIGHTMOUSE) && (val)) break;
	else if ((dev==BACKSPACEKEY) && (val)) {
		draw_images(bgimage,NULL,0,0);
		break;
		}
	if (redraw) draw_images(bgimage,oimage,ox,oy);
	}
 qenter(dev,val);
}
