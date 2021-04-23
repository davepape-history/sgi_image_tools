#include <stdio.h>
#include <fcntl.h>

#define UCHAR unsigned char

void filter(UCHAR *in,UCHAR *out,int xdim,int ydim);

usage(av)
char *av;
{
 fprintf(stderr,"Usage: %s inimage outimage\n",av);
 exit();
}

open_failed(fnam)
char *fnam;
{
 fprintf(stderr,"Failed to open %s\n",fnam);
 exit();
}

#define UCHAR unsigned char

/*
Median Finding on a 3-by-3 Grid
by Alan Paeth
from "Graphics Gems", Academic Press, 1990
*/

#define s2(a,b) if (b<a) a^=b^=a^=b;
#define mn3(a,b,c) s2(a,b); s2(a,c);
#define mx3(a,b,c) s2(b,c); s2(a,c);
#define mnmx3(a,b,c) mx3(a,b,c); s2(a,b);
#define mnmx4(a,b,c,d) s2(a,b); s2(c,d); s2(a,c); s2(b,d);
#define mnmx5(a,b,c,d,e) s2(a,b); s2(c,d); mn3(a,c,e); mx3(b,d,e);
#define mnmx6(a,b,c,d,e,f) s2(a,d); s2(b,e); s2(c,f);\
                            mn3(a,b,c); mx3(d,e,f);

UCHAR med3x3(b1, b2, b3)
    UCHAR *b1, *b2, *b3;
/*
 * Find median on a 3x3 input box of integers.
 * b1, b2, b3 are pointers to the left-hand edge of three
 * parallel scan-lines to form a 3x3 spatial median.
 * Rewriting b2 and b3 as b1 yields code which forms median
 * on input presented as a linear array of nine elements.
 */
    {
    register UCHAR r1, r2, r3, r4, r5, r6;
    r1 = *b1++; r2 = *b1++; r3 = *b1++;
    r4 = *b2++; r5 = *b2++; r6 = *b2++;
    mnmx6(r1, r2, r3, r4, r5, r6);
    r1 = *b3++;
    mnmx5(r1, r2, r3, r4, r5);
    r1 = *b3++;
    mnmx4(r1, r2, r3, r4);
    r1 = *b3++;
    mnmx3(r1, r2, r3);
    return(r2);
    }

main(argc,argv)
int argc; char **argv;
{
 UCHAR *in[3],*out[3],*malloc();
 int xdim,ydim,zdim,i;
 if (argc<3) usage(argv[0]);
 if (!dpReadSgiImage(argv[1],&in[0],&in[1],&in[2],&xdim,&ydim,&zdim)) exit();
 for (i=0; i<zdim; i++) {
	out[i] = malloc(xdim*ydim);
	filter(in[i],out[i],xdim,ydim);
	}
 dpWriteSgiImage(argv[2],out[0],out[1],out[2],xdim,ydim,zdim);
}

void filter(UCHAR *in,UCHAR *out,int xdim,int ydim)
{
 unsigned char *p1,*p2,*p3,*op;
 int i,j;
 for (i=0, p2=in, p3=in+xdim, op=out; i<xdim; i++, p2++, p3++, op++)
	*op = med3x3(p2,p2,p3);
 for (j=1, p1=in; j<ydim-1; j++)
    for (i=0; i<xdim; i++, p1++, p2++, p3++, op++)
	*op = med3x3(p1,p2,p3);
 for (i=0; i<xdim; i++, p1++, p2++, op++)
	*op = med3x3(p1,p2,p2);
}
