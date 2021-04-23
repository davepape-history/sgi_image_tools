#include "image.h"

#define USHORT unsigned short

void to_yiq(USHORT *rbuf,USHORT *gbuf,USHORT *bbuf,float *ybuf,float *ibuf,
		float *qbuf,int xsize)
{
 int x;
 for (x=0; x<xsize; x++) {
	ybuf[x] = 0.299*rbuf[x] + 0.587*gbuf[x] + 0.114*bbuf[x];
	ibuf[x] = 0.596*rbuf[x] - 0.275*gbuf[x] - 0.321*bbuf[x];
	qbuf[x] = 0.212*rbuf[x] - 0.523*gbuf[x] + 0.311*bbuf[x];
	}
}

void from_yiq(USHORT *rbuf,USHORT *gbuf,USHORT *bbuf,float *ybuf,float *ibuf,
		float *qbuf,int xsize)
{
 int x,r,g,b;
 for (x=0; x<xsize; x++) {
	r = ybuf[x] + 0.94*ibuf[x] + 0.62*qbuf[x];
	if (r<0) r=0; else if (r>255) r=255;
	g = ybuf[x] - 0.27*ibuf[x] - 0.65*qbuf[x];
	if (g<0) g=0; else if (g>255) g=255;
	b = ybuf[x] - 1.11*ibuf[x] + 1.70*qbuf[x];
	if (b<0) b=0; else if (b>255) b=255;
	rbuf[x] = r;
	gbuf[x] = g;
	bbuf[x] = b;
	}
}

void filter_y_row(float *ybuf,int xsize)
{
#define FYSIZE 9
 static float filter[FYSIZE] = { 0.0295, -0.0524, -0.118, 0.472, 1.0, 0.472,
				-0.118, -0.0524, 0.0295};
 float sum,fsum;
 float *outbuf;
 int x,n,x0,x1;
 outbuf = (float *) malloc(xsize*sizeof(float));
 for (n=0, fsum=0.0; n<FYSIZE; n++) fsum += filter[n];
 for (x=0; x<xsize; x++) {
	x0 = x-(FYSIZE-1)/2;
	if (x0<0) x0=0;
	x1 = x+(FYSIZE-1)/2;
	if (x1>xsize-1) x1=xsize-1;
	for (n=x0, sum=0.0; n<=x1; n++) sum += filter[n-x0]*ybuf[n];
	outbuf[x] = sum/fsum;
	}
 for (x=0; x<xsize; x++) ybuf[x] = outbuf[x];
 free(outbuf);
}

void filter_i_row(float *ibuf,int xsize)
{
#define FISIZE 29
 static float filter[FISIZE] = { 0.00474, 0.01739, 0.0295, 0.02743, 0.0, -0.0524,
		-0.1123, -0.1467, -0.118, 0.0, 0.20746, 0.4720, 0.73489, 0.92867,
		1.0, 0.92867, 0.73489, 0.4720, 0.20746, 0.0, -0.118, -0.1467,
		-0.1123, -0.0524, 0.0, 0.02743, 0.0295, 0.01739, 0.00474 };
 float sum,fsum;
 float *outbuf;
 int x,n,x0,x1;
 outbuf = (float *) malloc(xsize*sizeof(float));
 for (n=0, fsum=0.0; n<FISIZE; n++) fsum += filter[n];
 for (x=0; x<xsize; x++) {
	x0 = x-(FISIZE-1)/2;
	if (x0<0) x0=0;
	x1 = x+(FISIZE-1)/2;
	if (x1>xsize-1) x1=xsize-1;
	for (n=x0, sum=0.0; n<=x1; n++) sum += filter[n-x0]*ibuf[n];
	outbuf[x] = sum/fsum;
	}
 for (x=0; x<xsize; x++) ibuf[x] = outbuf[x];
 free(outbuf);
}

main(argc,argv)
int argc;
char **argv;
{
 IMAGE *inimage,*outimage;
 int y, xsize, ysize, zsize;
 USHORT *rbuf,*gbuf,*bbuf;
 float *ybuf,*ibuf,*qbuf;

 if (argc<3) {
	fprintf(stderr,"usage: %s inimage.sgi outimage.sgi\n",argv[0]);
	exit(1);
    	} 
 if ((inimage=iopen(argv[1],"r")) == NULL) {
	fprintf(stderr,"%s: can't open input file %s\n",argv[0],argv[1]);
	exit(1);
	}
 xsize = inimage->xsize;
 ysize = inimage->ysize;
 zsize = inimage->zsize;
 outimage = iopen(argv[2],"w",RLE(1),3,xsize,ysize,3);
 rbuf = (USHORT *) malloc(xsize*sizeof(USHORT));
 gbuf = (USHORT *) malloc(xsize*sizeof(USHORT));
 bbuf = (USHORT *) malloc(xsize*sizeof(USHORT));
 ybuf = (float *) malloc(xsize*sizeof(float));
 ibuf = (float *) malloc(xsize*sizeof(float));
 qbuf = (float *) malloc(xsize*sizeof(float));
 for (y=0; y<ysize; y++) {
	getrow(inimage,rbuf,y,0);
	if (zsize > 1) {
		getrow(inimage,gbuf,y,1);
		getrow(inimage,bbuf,y,2);
		}
	else {
		getrow(inimage,gbuf,y,0);
		getrow(inimage,bbuf,y,0);
		}
	to_yiq(rbuf,gbuf,bbuf,ybuf,ibuf,qbuf,xsize);
	filter_y_row(ybuf,xsize);
	filter_i_row(ibuf,xsize);
	filter_i_row(qbuf,xsize);
	from_yiq(rbuf,gbuf,bbuf,ybuf,ibuf,qbuf,xsize);
	putrow(outimage,rbuf,y,0);
	putrow(outimage,gbuf,y,1);
	putrow(outimage,bbuf,y,2);
	}
 iclose(outimage);
 iclose(inimage);
}
