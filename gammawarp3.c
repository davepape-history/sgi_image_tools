/*
 *	gammawarp3 - Gammawarp the 3 channels of an image separately
 */
#include "image.h"
#include "math.h"

short buf[4096];
short tab[4096];
float igamma;

main(argc,argv)
int argc;
char **argv;
{
    IMAGE *iimage, *oimage;
    unsigned int xsize, ysize, zsize;
    unsigned int y, z;

    if( argc<4 ) {
	fprintf(stderr,"usage: gammawarp inimage outimage gamma1 gamma2 gamma3\n");
	exit(1);
    } 
    if( (iimage=iopen(argv[1],"r")) == NULL ) {
	fprintf(stderr,"%s: can't open input file %s\n",argv[0],argv[1]);
	exit(1);
    }
    xsize = iimage->xsize;
    ysize = iimage->ysize;
    zsize = iimage->zsize;
    oimage = iopen(argv[2],"w",RLE(1),iimage->dim,xsize,ysize,iimage->zsize); 
    isetname(oimage,iimage->name);
    oimage->colormap = iimage->colormap;
    for(z=0; z<zsize; z++) {
        if (argc>z+3) igamma = atof(argv[z+3]);
	else igamma = 1.0;
        maketab();
	for(y=0; y<ysize; y++) {
	    getrow(iimage,buf,y,z);
	    gammawarp(buf,xsize);
	    putrow(oimage,buf,y,z);
	}
    }
    iclose(oimage);
}

maketab()
{
    int i;

    for(i=0; i<256; i++) {
	tab[i] = 255.0*pow(i/255.0,igamma)+0.5;
    }
}

gammawarp(sptr,n)
register unsigned short *sptr;
register int n;
{
    while(n--) {
	*sptr = tab[*sptr];
	sptr++;
    }
}
