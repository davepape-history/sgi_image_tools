#include <stdio.h>
#include <math.h>

main(int argc,char **argv)
{
 unsigned char *inimg,*outimg,*malloc(),*op,*ip;
 int x,y,i,j,starti,endi;
 float fx,fy;
 if (!read_sgi_image_byte(argv[1],&inimg,&x,&y)) exit();
 outimg = malloc(x*y);
 bzero(outimg,x*y);
 for (j=0; j<y; j++) {
	fy = (j*2-y+1)/((float)(y-1.0));
	fx = sqrt(1.0-fy*fy);
	starti = (1.0-fx)*x/2;
	if (starti<0) starti=0;
	endi = (fx+1.0)*x/2;
	if (endi>=x) endi=x-1;
	op = outimg+j*x;
	ip = inimg+j*x;
	if (starti == endi) op[starti] = ip[x/2];
	else for (i=starti; i<=endi; i++)
		op[i] = ip[((i-starti)*(x-1))/(endi-starti)];
	}
 write_sgi_image_byte(argv[2],outimg,x,y);
}
