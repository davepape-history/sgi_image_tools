#include <stdio.h>

main(argc,argv)
char **argv;
{
 int i,max,max2,x,y;
 int cutoff = 8;
 unsigned char *img;
 int sum[256];
 if (argc<2)
   { fprintf(stderr,"Usage: %s image [cutoff]\n",argv[0]); exit(); }
 if (argc>2) cutoff = atoi(argv[2]);
 if (!read_sgi_image_byte(argv[1],&img,&x,&y)) exit();
 for (i=0; i<256; i++) sum[i]=0;
 for (i=0; i<x*y; i++) sum[img[i]]++;
 for (i=0; i<256; i++) {
	printf("%d: %d",i,sum[i]);
	if ((i+1)%4 == 0) printf("\n");
	else printf("\t");
	if (sum[i]) max=i;
	if (sum[i]>cutoff) max2=i;
	}
 printf("max = %d\n",max);
 printf("max with >%d = %d\n",cutoff,max2);
}
