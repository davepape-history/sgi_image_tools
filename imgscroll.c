#include <stdio.h>
#include "dave.h"

void subimage(UCHAR *in,int inx,int iny,UCHAR *out,int startline,int outy);

void usage(char *av0)
{
 fprintf(stderr,"Usage: %s [-y out-y] in-image out-basename lines-per-frame frames\n",
		av0);
 exit();
}

main(int argc,char **argv)
{
 UCHAR *inr,*ing,*inb,*outr,*outg,*outb;
 int outy=486,frames,step,startline,i,inx,iny,inz,argnum=1;
 char fname[1024];
 if (argc<5) usage(argv[0]);
 if (!strcmp(argv[1],"-y")) {
	if (argc<7) usage(argv[0]);
	outy=atoi(argv[2]);
	argnum=3;
	}
 if (!dpReadSgiImage(argv[argnum],&inr,&ing,&inb,&inx,&iny,&inz)) exit();
 outr = ARRALLOC(UCHAR,inx*outy);
 if (inz>1) outg = ARRALLOC(UCHAR,inx*outy); else outg=NULL;
 if (inz>2) outb = ARRALLOC(UCHAR,inx*outy); else outb=NULL;
 step = atoi(argv[argnum+2]);
 frames = atoi(argv[argnum+3]);
 for (i=0, startline=iny-outy; i<frames; i++, startline-=step) {
	subimage(inr,inx,iny,outr,startline,outy);
	if (inz>1) subimage(ing,inx,iny,outg,startline,outy);
	if (inz>2) subimage(inb,inx,iny,outb,startline,outy);
	sprintf(fname,"%s%04d",argv[argnum+1],i);
	dpWriteSgiImage(fname,outr,outg,outb,inx,outy,inz);
	}
}

void subimage(UCHAR *in,int inx,int iny,UCHAR *out,int startline,int outy)
{
 UCHAR *ip,*op;
 int x,y;
 for (y=startline, op=out; y<startline+outy; y++) {
    if (y<0) ip = in;
    else if (y<iny) ip = in+y*inx;
    else ip = in+(iny-1)*inx;
    for (x=0; x<inx; x++) *op++ = *ip++;
    }
}
