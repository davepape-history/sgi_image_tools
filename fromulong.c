#include "image.h"

void find_range(char *fname,int x,int y,int *min,int *max);

main(argc,argv)
int argc;
char **argv;
{
 unsigned long *ibuf;
 short *obuf;
 IMAGE *oimage;
 int fd;
 unsigned long min,max,range;
 int xsize, ysize;
 int x,y;
 if (argc<5) {
	fprintf(stderr,"usage: %s inimage.32 outimage.sgi xdim ydim [min max]\n",argv[0]);
	exit(1);
	}
 xsize = atoi(argv[3]);
 ysize = atoi(argv[4]);
 if ((xsize<1) || (ysize<1)) {
	fprintf(stderr,"Error: bad dimensions (%d x %d)\n",xsize,ysize);
	exit(1);
	}
 if (argc>6) {
	min = atoi(argv[5]);
	max = atoi(argv[6]);
	}
 else find_range(argv[1],xsize,ysize,&min,&max);
 if (min==max) range=1;
 else range = max-min;
 if ((fd=open(argv[1],0)) == -1) {
	perror(argv[1]);
	exit(1);
	}
 ibuf = (unsigned long *) malloc(xsize*sizeof(unsigned long));
 obuf = (short *) malloc(xsize*sizeof(short));
 oimage = iopen(argv[2],"w",RLE(1),3,xsize,ysize,1); 
 for (y=0; y<ysize; y++) {
	read(fd,ibuf,xsize*sizeof(unsigned long));
	for (x=0; x<xsize; x++)
		if (ibuf[x]>max) obuf[x] = 255;
		else if (ibuf[x]<min) obuf[x] = 0;
		else obuf[x] = ((ibuf[x]-min)*255)/range;
	putrow(oimage,obuf,y,0);
	}
 iclose(oimage);
 close(fd);
}

void find_range(char *fname,int x,int y,unsigned long *min,unsigned long *max)
{
 int fd,nread,i;
 unsigned long *buf;
 if ((fd=open(fname,0)) == -1) {
	perror(fname);
	exit(1);
	}
 buf = (unsigned long *) malloc(x*sizeof(unsigned long));
 read(fd,buf,sizeof(unsigned long));
 *min = *max = buf[0];
 while (nread = read(fd,buf,x*sizeof(unsigned long))) {
	nread /= sizeof(unsigned long);
	for (i=0; i<nread; i++)
		if (*min > buf[i]) *min = buf[i];
		else if (*max < buf[i]) *max = buf[i];
	}
 close(fd);
 free(buf);
}
