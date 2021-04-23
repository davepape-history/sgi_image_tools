#include <stdio.h>
#include "dave.h"

void scale_img(UCHAR *,UCHAR *,float,int);

main(int argc,char **argv)
{
 UCHAR *red1,*green1,*blue1,*outred,*outgreen,*outblue;
 int xdim,ydim,zdim,i,frames,fadein;
 float t;
 char name[1024],*p1,*p2;
 for (p1=p2=argv[0]; *p1; p1++)
	if (*p1=='/') p2=p1+1;
 fadein = !strcmp(p2,"fadein");
 if (argc<4) {
	fprintf(stderr,"Usage: %s inimage outimage-basename frames\n",
		argv[0]);
	exit();
	}
 if (!read_sgi_image(argv[1],&red1,&green1,&blue1,&xdim,&ydim,&zdim)) exit();
 frames = atoi(argv[3]);
 outred = arralloc(UCHAR,xdim*ydim);
 if (zdim>1) outgreen = arralloc(UCHAR,xdim*ydim);
 if (zdim>2) outblue = arralloc(UCHAR,xdim*ydim);
 for (i=0; i<frames; i++) {
	if (frames>1) t = (float)i/(frames-1);
	else t = 1.0;
	if (!fadein) t = 1.0-t;
	scale_img(red1,outred,t,xdim*ydim);
	if (zdim>1) scale_img(green1,outgreen,t,xdim*ydim);
	if (zdim>2) scale_img(blue1,outblue,t,xdim*ydim);
	sprintf(name,"%s%04d",argv[2],i);
	write_sgi_image(name,outred,outgreen,outblue,xdim,ydim,zdim);
	}
}

void scale_img(UCHAR *img1,UCHAR *out,float t,int size)
{
 UCHAR *p1,*op;
 int i;
 for (i=0, p1=img1, op=out; i<size; i++, p1++, op++) *op = t*(*p1);
}
