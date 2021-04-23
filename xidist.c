#include <stdio.h>
#include <X11/Xlib.h>
#include "dave.h"

#define HEIGHT 512

void draw_plot();
Window open_window();
GC get_gc();

main(argc,argv)
char **argv;
{
 int i,sum[256],x,y,z;
 unsigned char *r,*g,*b,*p;
 if (argc<2) { fprintf(stderr,"Usage: %s image\n",argv[0]); exit(); }
 if (!read_sgi_image(argv[1],&r,&g,&b,&x,&y,&z)) exit();
 for (i=0; i<256; i++) sum[i]=0;
 for (i=0, p=r; i<x*y; i++,p++) sum[*p]++;
 if (z>1) for (i=0, p=g; i<x*y; i++,p++) sum[*p]++;
 if (z>2) for (i=0, p=b; i<x*y; i++,p++) sum[*p]++;
 draw_plot(sum);
}

void draw_plot(sum)
int sum[256];
{
 int i,max;
 Display *display;
 Window win;
 GC gc;
 XEvent event;
 if ((display = XOpenDisplay(NULL)) == NULL) {
	fprintf(stderr,"cannot connect to X server %s\n",XDisplayName(NULL));
	return;
	}
 win = open_window(display);
 gc = get_gc(display,win);
 max=sum[0];
 for (i=1; i<256; i++) if (max<sum[i]) max=sum[i];
 while (1) {
	XNextEvent(display,&event);
	if (event.type == Expose) {
		XClearWindow(display,win);
		for (i=0; i<256; i++) XDrawLine(display,win,gc,i,HEIGHT-1,
						i,(HEIGHT-sum[i]*HEIGHT/max));
		}
	}
}

Window open_window(display)
Display *display;
{
 int screen;
 Window win;
 screen = DefaultScreen(display);
 win = XCreateSimpleWindow(display,RootWindow(display,screen),0,0,256,HEIGHT,
	2,BlackPixel(display,screen),WhitePixel(display,screen));
 XSelectInput(display,win,ExposureMask);
 XMapWindow(display,win);
 return(win);
}

GC get_gc(display,win)
Display *display;
Window win;
{
 unsigned long valuemask = GCForeground | GCBackground;
 XGCValues values;
 values.foreground=BlackPixel(display,DefaultScreen(display));
 values.background=WhitePixel(display,DefaultScreen(display));
 return(XCreateGC(display, win, valuemask, &values));
}

