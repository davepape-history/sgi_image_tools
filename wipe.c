#include <stdio.h>
#include "dave.h"

void matte_img(UCHAR *,UCHAR *,UCHAR *,UCHAR *,int);

main(int argc,char **argv)
{
 UCHAR *red1,*green1,*blue1,*red2,*green2,*blue2,*outred,*outgreen,*outblue;
 UCHAR *wipe;
 int xdim,ydim,zdim,i,frame0,frame1,step,x2,y2,z2;
 char name[1024];
 if (argc<7) {
	fprintf(stderr,"Usage: %s inpattern1 inpattern2 wipepattern outpattern startframe endframe [step]\n",
		argv[0]);
	exit();
	}
 frame0 = atoi(argv[5]);
 frame1 = atoi(argv[6]);
 if (argc>7) step = atoi(argv[7]);
 else step=1;
 for (i=frame0; i<=frame1; i+=step) {
     sprintf(name,argv[1],i);
     if (!dpReadSgiImage(name,&red1,&green1,&blue1,&xdim,&ydim,&zdim)) exit();
     sprintf(name,argv[2],i);
     if (!dpReadSgiImage(name,&red2,&green2,&blue2,&x2,&y2,&z2)) exit();
     if ((x2!=xdim) || (y2!=ydim) || (z2!=zdim)) {
	fprintf(stderr,"Error: images must have same dimensions\n");
	exit();
	}
     sprintf(name,argv[3],i);
     if (!dpReadSgiImageByte(name,&wipe,&x2,&y2)) exit();
     if ((x2!=xdim) || (y2!=ydim)) {
	fprintf(stderr,"Error: images must have same dimensions\n");
	exit();
	}

     outred = (UCHAR *) sizeof(xdim*ydim*sizeof(UCHAR));
     if (zdim>1) outgreen = (UCHAR *) sizeof(xdim*ydim*sizeof(UCHAR));
     if (zdim>2) outblue = (UCHAR *) sizeof(xdim*ydim*sizeof(UCHAR));

     matte_img(red1,red2,wipe,outred,xdim*ydim);
     if (zdim>1) matte_img(green1,green2,wipe,outgreen,xdim*ydim);
     if (zdim>2) matte_img(blue1,blue2,wipe,outblue,xdim*ydim);

     sprintf(name,argv[4],i);
     dpWriteSgiImage(name,outred,outgreen,outblue,xdim,ydim,zdim);

     free(red1);  free(red2);  free(outred);
     free(wipe);
     if (zdim>1) { free(green1);  free(green2);  free(outgreen); }
     if (zdim>2) { free(blue1);  free(blue2);  free(outblue); }
     }
}

void matte_img(UCHAR *img1,UCHAR *img2,UCHAR *matte,UCHAR *out,int size)
{
 UCHAR *p1,*p2,*op,*mp;
 int i;
 float t;
 for (i=0, p1=img1, p2=img2, mp=matte, op=out; i<size;
	i++, p1++, p2++, mp++, op++) {
		t = (float)(*mp)/255.0;
		*op = (1.0-t)*(*p1) + t*(*p2);
		}
}
