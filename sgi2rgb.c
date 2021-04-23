#include <stdio.h>
#include "dave.h"


main(int argc,char **argv)
{
 unsigned char *r,*g,*b,*buf;
 int x,y,z,fd,i;
 if (argc<3) {
	fprintf(stderr,"Usage: %s in-image out-image\n",argv[0]);
	exit();
	}
 dpReadSgiImage(argv[1],&r,&g,&b,&x,&y,&z);
 buf = (unsigned char *) malloc(x*y*3);
 for (i=0; i<x*y; i++)
	{
	buf[i*3] = r[i];
	buf[i*3+1] = g[i];
	buf[i*3+2] = b[i];
	}
 fd = creat(argv[2],0644);
 if (fd < 0)
	{
	perror(argv[2]);
	}
 else
	{
	write(fd,buf,x*y*3);
	close(fd);
	}
}
