#include <stdio.h>
#include <image.h>

void write_cfire_file(char *fname,IMAGE *img,int band)
{
 int fd,y,x;
 unsigned char *obuf,*op;
 short *ibuf,*ip;
 if ((fd = creat(fname,0644)) == -1) { perror(fname); exit(); }
 ibuf = (short *) malloc(img->xsize*sizeof(short));
 obuf = (unsigned char *) malloc(img->xsize*sizeof(unsigned char));
 for (y=img->ysize-1; y>=0; y--) {
	getrow(img,ibuf,y,band);
	for (x=img->xsize, ip=ibuf, op=obuf; x; x--) *op++ = *ip++;
	write(fd,obuf,img->xsize);
	}
 close(fd);
}

main(int argc,char **argv)
{
 int zdim,fd;
 char name[80];
 IMAGE *inimg;
 if (argc<3) {
	fprintf(stderr,"Usage: %s rgb-file cfire-basename\n",argv[0]);
	exit();
	}
 if ((inimg = iopen(argv[1],"r")) == NULL) {
	fprintf(stderr,"%s: failed to open \"%s\"\n",argv[0]);
	exit();
	}
 sprintf(name,"%s.r",argv[2]);
 write_cfire_file(name,inimg,0);
 if (inimg->zsize > 1) {
	sprintf(name,"%s.g",argv[2]);
	write_cfire_file(name,inimg,1);
	}
 if (inimg->zsize > 2) {
	sprintf(name,"%s.b",argv[2]);
	write_cfire_file(name,inimg,2);
	}
 iclose(inimg);
}
