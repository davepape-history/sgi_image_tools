#include <stdio.h>
#include <math.h>
#include <gl.h>
#include "forms.h"

typedef struct {
		int x,y;
		unsigned long *image;
		} image_t;

void open_window(char *name,int x,int y,float zoomfac);
void draw_image(image_t *img);
void make_forms();
void do_forms();
void next_frame(int *curframe,int *delta,int mode);

image_t **frame;
int numframes;
int windowid;

main(int argc,char **argv)
{
 int i,n;
 float zoomfac = 1.0;
 numframes = argc-1;
 for (i=1; i<argc; i++)
   if (!strcmp(argv[i],"-z")) {
	zoomfac = atof(argv[++i]);
	numframes -= 2;
	}
 if (numframes<1) {
	fprintf(stderr,"Usage: %s [-z zoom] image-files...\n",argv[0]);
	exit();
	}
 frame = (image_t **) malloc(numframes*sizeof(image_t *));
 for (i=1, n=0; i<argc; i++)
   if (strcmp(argv[i],"-z")) {
	frame[n] = (image_t *) malloc(sizeof(image_t));
	if (!read_sgi_image_long(argv[i],&frame[n]->image,&frame[n]->x,
				&frame[n]->y)) exit();
	if (!n) open_window(argv[i],frame[0]->x,frame[0]->y,zoomfac);
	draw_image(frame[n]);
	n++;
	}
   else i++;
 make_forms();
 draw_image(frame[0]);
 do_forms();
}

void open_window(char *name,int x,int y,float zoomfac)
{
 prefsize(x*zoomfac,y*zoomfac);
 windowid = winopen(name);
 rectzoom(zoomfac,zoomfac);
 doublebuffer();
 RGBmode();
 gconfig();
 cpack(0); clear();
 swapbuffers();
}

void draw_image(image_t *img)
{
 winset(windowid);
 lrectwrite(0,0,img->x-1,img->y-1,img->image);
 swapbuffers();
}

FL_OBJECT *frameslider,*revbutton,*stopbutton,*forbutton,*swingbutton,
	*stepforbutton,*steprevbutton,*quitbutton,*framebox;

void make_forms()
{
 FL_FORM *form;
 form = fl_bgn_form(FL_UP_BOX,500.0,200.0);
   frameslider = fl_add_slider(FL_HOR_SLIDER,20.0,140.0,400.0,40.0,"Frame");
   fl_set_slider_bounds(frameslider,0.0,(float)numframes-0.5);
   fl_set_slider_value(frameslider,0.5);
   framebox = fl_add_box(FL_DOWN_BOX,440.0,140.0,40.0,40.0,"0");
   fl_bgn_group();
     revbutton = fl_add_lightbutton(FL_RADIO_BUTTON,20.0,80.0,70.0,40.0,"<-");
     stopbutton = fl_add_lightbutton(FL_RADIO_BUTTON,200.0,80.0,70.0,40.0,"Stop");
     forbutton = fl_add_lightbutton(FL_RADIO_BUTTON,380.0,80.0,70.0,40.0,"->");
     swingbutton = fl_add_lightbutton(FL_RADIO_BUTTON,170.0,20.0,110.0,40.0,"<- Swing ->");
     fl_set_button(stopbutton,1);
   fl_end_group();
   steprevbutton = fl_add_button(FL_NORMAL_BUTTON,110.0,80.0,70.0,40.0,"-1");
   stepforbutton = fl_add_button(FL_NORMAL_BUTTON,290.0,80.0,70.0,40.0,"+1");
   quitbutton = fl_add_button(FL_NORMAL_BUTTON,390.0,20.0,70.0,40.0,"Quit");
 fl_end_form();
 fl_show_form(form,FL_PLACE_SIZE,TRUE,"Movie Control");
}

#define FORWARD 1
#define REVERSE 2
#define STOPPED 0
#define SWING   3

void reset_buttons(int mode)
{
 if (mode==FORWARD) fl_set_button(forbutton,0);
 else if (mode==REVERSE) fl_set_button(revbutton,0);
 else if (mode==SWING) fl_set_button(swingbutton,0);
 fl_set_button(stopbutton,1);
}

void do_forms()
{
 int curframe=0,delta,mode=STOPPED;
 char str[80];
 FL_OBJECT *obj;
 while (1) {
    if (mode==STOPPED)
	obj = fl_do_forms();
    else while ((obj=fl_check_forms()) == NULL) {
	next_frame(&curframe,&delta,mode);
	draw_image(frame[curframe]);
	sprintf(str,"%d",curframe);
	fl_set_object_label(framebox,str);
	fl_set_slider_value(frameslider,(float)curframe+0.5);
	}
    if (obj==frameslider) {
		curframe = fl_get_slider_value(frameslider);
		draw_image(frame[curframe]);
		sprintf(str,"%d",curframe);
		fl_set_object_label(framebox,str);
		reset_buttons(mode);
		mode = STOPPED;
		}
    else if (obj==stepforbutton) {
		if (curframe < numframes-1) {
			curframe++;
			draw_image(frame[curframe]);
			sprintf(str,"%d",curframe);
			fl_set_object_label(framebox,str);
			fl_set_slider_value(frameslider,(float)curframe+0.5);
			}
		reset_buttons(mode);
		mode = STOPPED;
		}
    else if (obj==steprevbutton) {
		if (curframe) {
			curframe--;
			draw_image(frame[curframe]);
			sprintf(str,"%d",curframe);
			fl_set_object_label(framebox,str);
			fl_set_slider_value(frameslider,(float)curframe+0.5);
			}
		reset_buttons(mode);
		mode = STOPPED;
		}
    else if (obj==stopbutton) mode = STOPPED;
    else if (obj==forbutton) {
		mode = FORWARD;
		delta = 1;
		}
    else if (obj==revbutton) {
		mode = REVERSE;
		delta = -1;
		}
    else if (obj==swingbutton) {
		mode = SWING;
		delta = 1;
		}
    else if (obj==quitbutton) return;
    }
}

void next_frame(int *curframe,int *delta,int mode)
{
 *curframe += *delta;
 if (*curframe<0) {
	if (mode==SWING) {
		*curframe = 0;
		*delta = 1;
		}
	else *curframe = numframes-1;
	}
 else if (*curframe > numframes-1) {
	if (mode==SWING) {
		*curframe = numframes-1;
		*delta = -1;
		}
	else *curframe = 0;
	}
}
