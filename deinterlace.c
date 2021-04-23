#include <gl/image.h>

main(argc,argv)
int argc;
char **argv;
{
 IMAGE *inimage,*outimage;
 int x, y, xsize, ysize, z, zsize, zoomflag=0, args=1;
 unsigned short *buf;

 if (argc<3)
	{
	fprintf(stderr,"usage: %s [-z] inimage.sgi outimage.sgi\n",argv[0]);
	exit(1);
	}
 if (!strcmp(argv[1],"-z"))
	{
	zoomflag = 1;
	args = 2;
	}
 if ((inimage=iopen(argv[args],"r")) == NULL)
	{
	fprintf(stderr,"%s: can't open input file %s\n",argv[0],argv[args]);
	exit(1);
	}
 xsize = inimage->xsize;
 ysize = inimage->ysize;
 zsize = inimage->zsize;
 if (zoomflag)
	outimage = iopen(argv[args+1],"w",RLE(1),3,xsize,ysize,zsize);
 else
	outimage = iopen(argv[args+1],"w",RLE(1),3,xsize,ysize/2,zsize);
 buf = (unsigned short *) malloc(xsize*sizeof(unsigned short));
 for (y=0; y<ysize; y+=2)
    for (z=0; z<zsize; z++)
	{
	getrow(inimage,buf,y,z);
	if (zoomflag)
		{
		putrow(outimage,buf,y,z);
		putrow(outimage,buf,y+1,z);
		}
	else
		putrow(outimage,buf,y/2,z);
	}
 iclose(outimage);
 iclose(inimage);
}
