#include <stdio.h>
#include <fcntl.h>

main(int argc,char **argv)
{
 int ifd,nread,size,xdim,ydim,i;
 char fname[80];
 unsigned char *data,*malloc();
 if (argc<5) {
	fprintf(stderr,"Usage: %s input output-basename xdim ydim\n",argv[0]);
	exit();
	}
 if ((ifd=open(argv[1],O_RDONLY)) == -1) { perror(argv[1]); exit(); }
 xdim = atoi(argv[3]);
 ydim = atoi(argv[4]);
 size = xdim*ydim;
 data = malloc(size);
 i=0;
 while (nread = read(ifd,data,size)) {
	sprintf(fname,"%s%04d",argv[2],i);
	write_sgi_image(fname,data,NULL,NULL,xdim,ydim,1);
	i++;
	}
 close(ifd);
}
