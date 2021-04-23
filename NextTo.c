#include <stdio.h>

main(int argc,char **argv)
{
 unsigned long *a,*b,*c,*p,*malloc();
 int x1,y1,x2,y2,x,i,j;
 if (argc<4) {
	fprintf(stderr,"Usage: %s inimage1 inimage2 outimage\n");
	exit();
	}
 read_sgi_image_long(argv[1],&a,&x1,&y1);
 read_sgi_image_long(argv[2],&b,&x2,&y2);
 x = x1+x2;
 p = c = malloc(x*y1*sizeof(long));
 for (j=0; j<y1; j++) {
	for (i=0; i<x1; i++) *p++ = *a++;
	for (i=0; i<x2; i++) *p++ = *b++;
	}
 write_sgi_image_long(argv[3],c,x,y1,3);
}
