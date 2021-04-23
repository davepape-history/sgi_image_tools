#include <stdio.h>

#define UCHAR unsigned char

void sharpen(UCHAR *img,UCHAR *out,int xdim,int ydim);

#define ksize 3
int kernel[ksize][ksize] = { { -1, -2, -1}, {-2, 13, -2}, {-1, -2, -1} };

/*
#define ksize 5
int kernel[ksize][ksize] =
			{ { -1, -1, -1, -1, -1},
			  { -1, -3, -3, -3, -1},
			  { -1, -3, 41, -3, -1},
			  { -1, -3, -3, -3, -1},
			  { -1, -1, -1, -1, -1} };
*/

main(int argc,char **argv)
{
 UCHAR *img[3],*out[3],*malloc();
 int xdim,ydim,zdim,i,delta;
 if (argc<3) {
	fprintf(stderr,"Usage: %s inimage outimage [delta]\n",argv[0]);
	exit();
	}
 if (!read_sgi_image(argv[1],&img[0],&img[1],&img[2],&xdim,&ydim,&zdim)) exit();
 if (argc>3) delta = atoi(argv[3]);
 kernel[(ksize-1)/2][(ksize-1)/2] += delta;
 for (i=0; i<zdim; i++) {
	out[i] = malloc(xdim*ydim);
	sharpen(img[i],out[i],xdim,ydim);
	}
 write_sgi_image(argv[2],out[0],out[1],out[2],xdim,ydim,zdim);
}

void sharpen(UCHAR *img,UCHAR *out,int xdim,int ydim)
{
 int x,y,kx,ky,sum,kmin,kmax,ksum,val;
 kmin = -(ksize-1)/2;
 kmax = -kmin;
 ksum = 0;
 for (ky=kmin; ky<=kmax; ky++)
   for (kx=kmin; kx<=kmax; kx++) ksum += kernel[kx-kmin][ky-kmin];
 for (y=1; y<ydim-1; y++)
   for (x=1; x<xdim-1; x++) {
	sum = 0;
	for (ky=kmin; ky<=kmax; ky++)
	   for (kx=kmin; kx<=kmax; kx++)
		sum += kernel[kx-kmin][ky-kmin] * img[(x+kx)+(y+ky)*xdim];
	val = sum/ksum;
	if (val>255) out[x+y*xdim] = 255;
	else if (val<0) out[x+y*xdim] = 0;
	else out[x+y*xdim] = val;
	}
}
