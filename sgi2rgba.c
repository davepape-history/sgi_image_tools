#include <stdio.h>
#include "dave.h"


static void reverseLongwords(unsigned long *img,int size)
{
 int i;
 for (i=0; i<size; i++, img++)
	*img =  ((*img & 0x000000ff) << 24) |
		((*img & 0x0000ff00) << 8) |
		((*img & 0x00ff0000) >> 8) |
		((*img & 0xff000000) >> 24);
}


main(int argc,char **argv)
{
 unsigned long *image;
 int x,y,fd;
 if (argc<3) {
	fprintf(stderr,"Usage: %s in-image out-image\n",argv[0]);
	exit();
	}
 dpReadSgiImageLong(argv[1],&image,&x,&y);
 reverseLongwords(image,x*y);
 fd = creat(argv[2],0644);
 if (fd < 0)
	{
	perror(argv[2]);
	}
 else
	{
	write(fd,image, x*y*sizeof(unsigned long));
	close(fd);
	}
}
