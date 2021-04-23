#include <stdio.h>
#include "dave.h"

void find_edges(unsigned char *row,int xdim,int *left,int *right)
{
 int i;
 for (i=0; i<xdim; i++)
	if (row[i]) break;
 *left = i;
 for (i=xdim-1; i; i--)
	if (row[i]) break;
 *right = i;
 if (*left > *right)
	*left = *right = 0;
}

main(int argc,char **argv)
{
 unsigned char *inimg,*outimg,*ip,*op;
 int xdim,ydim,i,j,left,right;
 if (!dpReadSgiImageByte(argv[1],&inimg,&xdim,&ydim))
	exit(1);
 outimg = (unsigned char *) malloc(xdim*ydim);
 for (j=0, ip=inimg, op=outimg; j<ydim; j++, ip+=xdim)
	{
	find_edges(ip,xdim,&left,&right);
	for (i=0; i<xdim; i++)
		*op++ = ip[(i*(right-left))/xdim + left];
	}
 dpWriteSgiImageByte(argv[2],outimg,xdim,ydim);
}
