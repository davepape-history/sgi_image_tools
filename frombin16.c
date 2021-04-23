#include "image.h"

#define MAXX 32768

void find_range(char *fname,int *min,int *max);

main(argc,argv)
int argc;
char **argv;
{
 short buf[MAXX];
 IMAGE *oimage;
 int fd,min,max,range;
 unsigned int xsize, ysize;
 unsigned int x,y;
 if (argc<5) {
	fprintf(stderr,"usage: %s inimage.16 outimage.sgi xdim ydim [min max]\n",argv[0]);
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
	min = atoi(argv[5]);
	max = atoi(argv[6]);
	}
 else find_range(argv[1],&min,&max);
 if (min==max) range=1;
 else range = max-min;
 if ((fd=open(argv[1],0)) == -1) {
	perror(argv[1]);
	exit(1);
	}
 oimage = iopen(argv[2],"w",RLE(1),3,xsize,ysize,1); 
 for (y=0; y<ysize; y++) {
	read(fd,buf,xsize*sizeof(short));
	for (x=0; x<xsize; x++)
		if (buf[x]>max) buf[x] = 255;
		else if (buf[x]<min) buf[x] = 0;
		else buf[x] = ((buf[x]-min)*255)/range;
	putrow(oimage,buf,y,0);
	}
 iclose(oimage);
 close(fd);
}

void find_range(char *fname,int *min,int *max)
{
 int fd,nread,i;
 short buf[8192];
 if ((fd=open(fname,0)) == -1) {
	perror(fname);
	exit(1);
	}
 read(fd,buf,2);
 *min = *max = buf[0];
 while (nread = read(fd,buf,sizeof(buf))) {
	nread /= 2;
	for (i=0; i<nread; i++)
		if (*min > buf[i]) *min = buf[i];
		else if (*max < buf[i]) *max = buf[i];
	}
 close(fd);
}
