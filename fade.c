#include <stdio.h>
#include "dave.h"

void interp_img(UCHAR *img1,UCHAR *img2,UCHAR *out,int t,int max,int size);

main(int argc,char **argv)
{
 UCHAR *red1,*green1,*blue1,*red2,*green2,*blue2,*outred,*outgreen,*outblue;
 int xdim,ydim,zdim,i,frames,x2,y2,z2,max;
 char name[1024];
 if (argc<5) {
	fprintf(stderr,"Usage: %s inimage1 inimage2 outimage-basename frames\n",
		argv[0]);
	exit();
	}
 if (!dpReadSgiImage(argv[1],&red1,&green1,&blue1,&xdim,&ydim,&zdim)) exit();
 if (!dpReadSgiImage(argv[2],&red2,&green2,&blue2,&x2,&y2,&z2)) exit();
 if ((x2!=xdim) || (y2!=ydim) || (z2!=zdim)) {
	fprintf(stderr,"Error: images must have same dimensions\n");
	exit();
	}
 frames = atoi(argv[4]);
 outred = (UCHAR *) malloc(xdim*ydim*sizeof(UCHAR));
 if (zdim>1) outgreen = (UCHAR *) malloc(xdim*ydim*sizeof(UCHAR));
 if (zdim>2) outblue = (UCHAR *) malloc(xdim*ydim*sizeof(UCHAR));
 if (frames>1) max = frames-1;
 else max=1;
 for (i=0; i<frames; i++) {
	interp_img(red1,red2,outred,i,max,xdim*ydim);
	if (zdim>1) interp_img(green1,green2,outgreen,i,max,xdim*ydim);
	if (zdim>2) interp_img(blue1,blue2,outblue,i,max,xdim*ydim);
	sprintf(name,"%s%04d",argv[3],i);
	dpWriteSgiImage(name,outred,outgreen,outblue,xdim,ydim,zdim);
	}
}

void interp_img(UCHAR *img1,UCHAR *img2,UCHAR *out,int t,int max,int size)
{
 UCHAR *p1,*p2,*op;
 int i;
 for (i=0, p1=img1, p2=img2, op=out; i<size; i++, p1++, p2++, op++)
	*op = ((max-t)*(*p1) + t*(*p2))/max;
}
