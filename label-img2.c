#include <stdio.h>
#include <fmclient.h>
#include "dave.h"

main(int argc,char **argv)
{
 ULONG *img;
 int x,y,i,pos;
 char str[2],*label;
 fmfonthandle font1,font2;
 if (argc<4) {
	fprintf(stderr,"Usage: %s inimage outimage label\n",argv[0]);
	exit();
	}
 if (!read_sgi_image_long(argv[1],&img,&x,&y)) exit();
 prefposition(0,x-1,0,y-1);
 foreground();
 winopen(argv[2]);
 RGBmode();
 gconfig();
 lrectwrite(0,0,x-1,y-1,img);
 fminit();
 font1 = fmfindfont("Times-Roman");
 font2 = fmscalefont(font1,18.0);
 fmsetfont(font2);
 label = argv[3];
 cpack(0xffffffff);
 cmov2i(64,5);
 fmprstr(label);
 lrectread(0,0,x-1,y-1,img);
 write_sgi_image_long(argv[2],img,x,y);
}
