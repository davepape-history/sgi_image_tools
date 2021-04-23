#include <stdio.h>
#include "dave.h"

main(int argc,char **argv)
{
 unsigned long *image;
 int x,y;
 if (argc<3) {
	fprintf(stderr,"Usage: %s in-image out-image\n",argv[0]);
	exit();
	}
 dpReadSgiImageLong(argv[1],&image,&x,&y);
 dpWriteSgiImageLong(argv[2],image,x,y);
}
