#include <stdio.h>
#include <math.h>
#include "image.h"

int read_colormap(char *fname,unsigned long *cmap)
{
 register IMAGE *image;
 int y;
 short rowbuf[16];
 if ((image=iopen(fname,"r",VERBATIM(2),2,4,256)) == NULL) {
        fprintf(stderr,"Error: can't open colormap file %s\n",fname);
        return(0);
        }
 for (y=0; y<256; y++) {
        getrow(image,rowbuf,y,0);
        cmap[y] = rowbuf[1] | rowbuf[2]<<8 | rowbuf[3]<<16 | 0xff000000;
        }
 iclose(image);
 return(1);
}

main(int argc,char **argv)
{
 int i,j,count=0,x,y;
 unsigned long *inimg,cmap[256],*ip;
 unsigned char *outimg,*op,*malloc();
 if (argc<4) {
	fprintf(stderr,"Usage: %s inimage.rgb colormap.M outimage.bw\n",argv[0]);
	exit();
	}
 if (!read_colormap(argv[2],cmap)) exit();
 if (!read_sgi_image_long(argv[1],&inimg,&x,&y)) exit();
 outimg = malloc(x*y);
 for (i=x*y, ip=inimg, op=outimg; i; i--, ip++, op++) {
	for (j=0; j<256; j++) if (*ip == cmap[j]) break;
	if (j==256) { count++; *op = 0; }
	else *op = j;
	}
 write_sgi_image_byte(argv[3],outimg,x,y);
 if (count) printf("warning: %d unmatched pixels\n",count);
}
