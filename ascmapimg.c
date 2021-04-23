#include <stdio.h>
#include "image.h"

int readColormap(char *filename,unsigned short colormap[256][3])
	{
	FILE *fp;
	char line[512];
	int r,g,b,i;
	fp = fopen(filename,"r");
	if (!fp)
		{
		perror(filename);
		return 0;
		}
	for (i=0; i < 256; i++)
		{
		fgets(line,sizeof(line),fp);
		sscanf(line,"%d%d%d",&r,&g,&b);
		colormap[i][0] = r;
		colormap[i][1] = g;
		colormap[i][2] = b;
		}
	fclose(fp);
	return 1;
	}


main(int argc,char **argv)
	{
	IMAGE *inimage,*outimage;
	int x, y, xsize, ysize;
	unsigned short *rbuf,*gbuf,*bbuf,*bwbuf;
	unsigned short colormap[256][3];
	
	if (argc<4)
		{
		fprintf(stderr,"usage: %s inimage.bw outimage.rgb colorMap.txt\n",argv[0]);
		exit(1);
		}
	if (!readColormap(argv[3],colormap))
		exit(1);
	if ((inimage=iopen(argv[1],"r")) == NULL)
		{
		fprintf(stderr,"%s: can't open input file %s\n",argv[0],argv[1]);
		exit(1);
		}
	xsize = inimage->xsize;
	ysize = inimage->ysize;
	outimage = iopen(argv[2],"w",RLE(1),3,xsize,ysize,3);
	rbuf = (unsigned short *) malloc(xsize*sizeof(unsigned short));
	gbuf = (unsigned short *) malloc(xsize*sizeof(unsigned short));
	bbuf = (unsigned short *) malloc(xsize*sizeof(unsigned short));
	bwbuf = (unsigned short *) malloc(xsize*sizeof(unsigned short));
	for (y=0; y<ysize; y++)
		{
		getrow(inimage,bwbuf,y,0);
		for (x=0; x<xsize; x++)
			{
			rbuf[x] = colormap[bwbuf[x]][0];
			gbuf[x] = colormap[bwbuf[x]][1];
			bbuf[x] = colormap[bwbuf[x]][2];
			}
		putrow(outimage,rbuf,y,0);
		putrow(outimage,gbuf,y,1);
		putrow(outimage,bbuf,y,2);
		}
	iclose(outimage);
	iclose(inimage);
	}
