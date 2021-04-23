#include "image.h"
#include <math.h>

#define TYPE unsigned short

#define MAXX 8192

void find_range(char *fname,int size,int *min,int *max);

main(argc,argv)
int argc;
char **argv;
{
 TYPE buf[MAXX];
 short ibuf[MAXX];
 IMAGE *oimage;
 int fd,val;
 int min,max,range;
 unsigned int xsize, ysize;
 unsigned int x,y;
 if (argc<5) {
	fprintf(stderr,"usage: %s indata outimage.sgi xdim ydim [min max]\n",argv[0]);
	exit(1);
	}
 xsize = atoi(argv[3]);
 if (xsize > MAXX) {
	fprintf(stderr,"Error: maximum x dimension is %d\n",MAXX);
	exit(1);
	}
 ysize = atoi(argv[4]);
 if ((xsize<1) || (ysize<1)) {
	fprintf(stderr,"Error: bad dimensions (%d x %d)\n",xsize,ysize);
	exit(1);
	}
 if (argc>6) {
	min = atof(argv[5]);
	max = atof(argv[6]);
	}
 else find_range(argv[1],xsize*ysize,&min,&max);
 if (min==max) range=1;
 else range = max-min;
 if ((fd=open(argv[1],0)) == -1) {
	perror(argv[1]);
	exit(1);
	}
 oimage = iopen(argv[2],"w",RLE(1),3,xsize,ysize,1); 
 for (y=0; y<ysize; y++) {
	read(fd,buf,xsize*sizeof(TYPE));
	for (x=0; x<xsize; x++) {
		val = ((buf[x]-min)*255)/range;
		if (val>255) ibuf[x] = 255;
		else if (val<0) ibuf[x] = 0;
		else ibuf[x] = val;
		}
	putrow(oimage,ibuf,y,0);
	}
 iclose(oimage);
 close(fd);
}

void find_range(char *fname,int size,int *min,int *max)
{
 int fd,nread,i;
 TYPE buf[512];
 int lmin,lmax;
 if ((fd=open(fname,0)) == -1) {
	perror(fname);
	exit(1);
	}
 read(fd,buf,sizeof(TYPE));
 lmin = lmax = buf[0];
 while (((nread = read(fd,buf,sizeof(buf)))>0) && (size>0)) {
	nread /= sizeof(TYPE);
	for (i=0; (i<nread) && (size); i++, size--) {
		if (lmin > buf[i]) lmin = buf[i];
		else if (lmax < buf[i]) lmax = buf[i];
		}
	}
 close(fd);
 *min = lmin;
 *max = lmax;
}
