#include "image.h"

main(argc,argv)
int argc;
char **argv;
{
 register IMAGE *image1,*image2;
 register int x, y, xsize, ysize;
 register int z, zsize;
 unsigned short *buf1,*buf2;

 if (argc<3) {
	fprintf(stderr,"usage: %s image1.sgi image2.sgi\n",argv[0]);
	return 2;
    	} 
 if ((image1=iopen(argv[1],"r")) == NULL) {
	fprintf(stderr,"%s: can't open input file %s\n",argv[0],argv[1]);
	return 2;
	}
 if ((image2=iopen(argv[2],"r")) == NULL) {
	fprintf(stderr,"%s: can't open input file %s\n",argv[0],argv[2]);
	return 2;
	}
 xsize = image1->xsize;
 ysize = image1->ysize;
 zsize = image1->zsize;
 if ((image2->xsize != xsize) || (image2->ysize != ysize) ||
	(image2->zsize != zsize)) {
		printf("%s & %s are different dimensions\n",argv[1],argv[2]);
		iclose(image1);
		iclose(image2);
		return 1;
		}
 buf1 = (unsigned short *) malloc(xsize*sizeof(unsigned short));
 buf2 = (unsigned short *) malloc(xsize*sizeof(unsigned short));
 for (y=0; y<ysize; y++)
    for (z=0; z<zsize; z++) {
	getrow(image1,buf1,y,z);
	getrow(image2,buf2,y,z);
	for (x = 0; x<xsize; x++)
		if (buf1[x] != buf2[x]) {
			printf("%s & %s differ at (%d,%d,%d)\n",argv[1],argv[2],
				x,y,z);
			iclose(image1);
			iclose(image2);
			return 1;
			}
	}
 printf("%s & %s are the same\n",argv[1],argv[2]);
 iclose(image1);
 iclose(image2);
 return 0;
}
