#include <stdio.h>
#include "dave.h"

main(int argc,char **argv)
{
 UCHAR *r,*g,*b,*rp,*gp,*bp,map[256];
 int i,xdim,ydim,zdim,n;
 FILE *fp;
 if (argc<4) {
	fprintf(stderr,"Usage: %s inimage outimage map.asc\n",argv[0]);
	exit();
	}
 if (!dpReadSgiImage(argv[1],&r,&g,&b,&xdim,&ydim,&zdim)) exit();
 if (!(fp=fopen(argv[3],"r"))) {
	perror(argv[3]);
	exit();
	}
 for (i=0; i<256; i++) { fscanf(fp,"%d",&n); map[i] = n; }
 for (i=0, rp=r; i<xdim*ydim; i++, rp++) *rp = map[*rp];
 if (zdim>1) for (i=0, gp=g; i<xdim*ydim; i++, gp++) *gp = map[*gp];
 if (zdim>2) for (i=0, bp=b; i<xdim*ydim; i++, bp++) *bp = map[*bp];
 dpWriteSgiImage(argv[2],r,g,b,xdim,ydim,zdim);
}
