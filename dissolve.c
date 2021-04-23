#include <stdio.h>
#include "dave.h"

void interp_img(UCHAR *,UCHAR *,UCHAR *,float,int);

main(int argc,char **argv)
{
 UCHAR *red1,*green1,*blue1,*red2,*green2,*blue2,*outred,*outgreen,*outblue;
 int xdim,ydim,zdim,i,frame0,frame1,step,x2,y2,z2;
 float t,frames;
 char name[1024];
 if (argc<6) {
	fprintf(stderr,"Usage: %s inpattern1 inpattern2 outpattern startframe endframe [step]\n",
		argv[0]);
	exit();
	}
 frame0 = atoi(argv[4]);
 frame1 = atoi(argv[5]);
 if (argc>6) step = atoi(argv[6]);
 else step=1;
 frames = frame1 - frame0;
 for (i=frame0; i<=frame1; i+=step) {
     sprintf(name,argv[1],i);
     if (!dpReadSgiImage(name,&red1,&green1,&blue1,&xdim,&ydim,&zdim)) exit();
     sprintf(name,argv[2],i);
     if (!dpReadSgiImage(name,&red2,&green2,&blue2,&x2,&y2,&z2)) exit();
     if ((x2!=xdim) || (y2!=ydim) || (z2!=zdim)) {
	fprintf(stderr,"Error: images must have same dimensions\n");
	exit();
	}
     outred = (UCHAR *) malloc(xdim*ydim*sizeof(UCHAR));
     if (zdim>1) outgreen = (UCHAR *) malloc(xdim*ydim*sizeof(UCHAR));
     if (zdim>2) outblue = (UCHAR *) malloc(xdim*ydim*sizeof(UCHAR));
     if (frames>0) t = (float)(i-frame0)/frames;
     else t = 1.0;
     interp_img(red1,red2,outred,t,xdim*ydim);
     if (zdim>1) interp_img(green1,green2,outgreen,t,xdim*ydim);
     if (zdim>2) interp_img(blue1,blue2,outblue,t,xdim*ydim);
     sprintf(name,argv[3],i);
     dpWriteSgiImage(name,outred,outgreen,outblue,xdim,ydim,zdim);
     free(red1);  free(red2);  free(outred);
     if (zdim>1) { free(green1);  free(green2);  free(outgreen); }
     if (zdim>2) { free(blue1);  free(blue2);  free(outblue); }
     }
}

void interp_img(UCHAR *img1,UCHAR *img2,UCHAR *out,float t,int size)
{
 UCHAR *p1,*p2,*op;
 int i;
 for (i=0, p1=img1, p2=img2, op=out; i<size; i++, p1++, p2++, op++)
	*op = (1.0-t)*(*p1) + t*(*p2);
}
