#include <stdio.h>
#include "dave.h"

main(int argc,char **argv)
{
 ULONG *img;
 int x,y,i,pos;
 char str[2],*label;
 if (argc<4) {
	fprintf(stderr,"Usage: %s inimage outimage label\n",argv[0]);
	exit();
	}
 if (!dpReadSgiImageLong(argv[1],&img,&x,&y)) exit();
 prefposition(0,x-1,0,y-1);
 foreground();
 winopen(argv[2]);
 RGBmode();
 gconfig();
 lrectwrite(0,0,x-1,y-1,img);
 label = argv[3];
 pos = (x-strwidth(label))/2;
 cpack(0xffffffff);
 str[1] = 0;
 for (i=0; i<strlen(label); i++) {
	str[0] = label[i];
	cmov2i(pos,5);
	charstr(str);
	pos += strwidth(str);
	}
 lrectread(0,0,x-1,y-1,img);
 dpWriteSgiImageLong(argv[2],img,x,y);
}
