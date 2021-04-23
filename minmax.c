#include <stdio.h>

#define FLOAT  1
#define LONG   2
#define ULONG  3
#define SHORT  4
#define USHORT 5
#define CHAR   6
#define UCHAR  7

void usage(char *av0)
{
 fprintf(stderr,"Usage: %s [-<type>] file\n"
		"  Where <type> is one of:  float  long  ulong  short\n"
		"                           ushort char  uchar\n"
		"    (default is uchar)\n",av0);
 exit();
}

void minmax_float(int fd)
{
 float min,max,buf[BUFSIZ];
 int nread,i;
 read(fd,buf,sizeof(float));
 min = max = buf[0];
 while (nread = read(fd,buf,sizeof(buf))) {
	nread /= sizeof(float);
	for (i=0; i<nread; i++) {
		if (min > buf[i]) min = buf[i];
		else if (max < buf[i]) max = buf[i];
		}
	}
 printf("%f %f\n",min,max);
}

void minmax_long(int fd)
{
 long min,max,buf[BUFSIZ];
 int nread,i;
 read(fd,buf,sizeof(long));
 min = max = buf[0];
 while (nread = read(fd,buf,sizeof(buf))) {
	nread /= sizeof(long);
	for (i=0; i<nread; i++) {
		if (min > buf[i]) min = buf[i];
		else if (max < buf[i]) max = buf[i];
		}
	}
 printf("%d %d\n",min,max);
}

void minmax_ulong(int fd)
{
 unsigned long min,max,buf[BUFSIZ];
 int nread,i;
 read(fd,buf,sizeof(unsigned long));
 min = max = buf[0];
 while (nread = read(fd,buf,sizeof(buf))) {
	nread /= sizeof(unsigned long);
	for (i=0; i<nread; i++) {
		if (min > buf[i]) min = buf[i];
		else if (max < buf[i]) max = buf[i];
		}
	}
 printf("%u %u\n",min,max);
}

void minmax_short(int fd)
{
 short min,max,buf[BUFSIZ];
 int nread,i;
 read(fd,buf,sizeof(short));
 min = max = buf[0];
 while (nread = read(fd,buf,sizeof(buf))) {
	nread /= sizeof(short);
	for (i=0; i<nread; i++) {
		if (min > buf[i]) min = buf[i];
		else if (max < buf[i]) max = buf[i];
		}
	}
 printf("%d %d\n",min,max);
}

void minmax_ushort(int fd)
{
 unsigned short min,max,buf[BUFSIZ];
 int nread,i;
 read(fd,buf,sizeof(unsigned short));
 min = max = buf[0];
 while (nread = read(fd,buf,sizeof(buf))) {
	nread /= sizeof(unsigned short);
	for (i=0; i<nread; i++) {
		if (min > buf[i]) min = buf[i];
		else if (max < buf[i]) max = buf[i];
		}
	}
 printf("%d %d\n",min,max);
}

void minmax_char(int fd)
{
 char min,max,buf[BUFSIZ];
 int nread,i;
 read(fd,buf,sizeof(char));
 min = max = buf[0];
 while (nread = read(fd,buf,sizeof(buf))) {
	nread /= sizeof(char);
	for (i=0; i<nread; i++) {
		if (min > buf[i]) min = buf[i];
		else if (max < buf[i]) max = buf[i];
		}
	}
 printf("%d %d\n",min,max);
}

void minmax_uchar(int fd)
{
 unsigned char min,max,buf[BUFSIZ];
 int nread,i;
 read(fd,buf,sizeof(unsigned char));
 min = max = buf[0];
 while (nread = read(fd,buf,sizeof(buf))) {
	nread /= sizeof(unsigned char);
	for (i=0; i<nread; i++) {
		if (min > buf[i]) min = buf[i];
		else if (max < buf[i]) max = buf[i];
		}
	}
 printf("%d %d\n",min,max);
}

main(int argc,char **argv)
{
 int fd,datatype=UCHAR,fnum=1;
 if (argc<2) usage(argv[0]);
 if (argc>2) {
	if (!strcmp(argv[1],"-float")) datatype = FLOAT;
	else if (!strcmp(argv[1],"-long")) datatype = LONG;
	else if (!strcmp(argv[1],"-int")) datatype = LONG;
	else if (!strcmp(argv[1],"-ulong")) datatype = ULONG;
	else if (!strcmp(argv[1],"-uint")) datatype = ULONG;
	else if (!strcmp(argv[1],"-short")) datatype = SHORT;
	else if (!strcmp(argv[1],"-ushort")) datatype = USHORT;
	else if (!strcmp(argv[1],"-char")) datatype = CHAR;
	else if (!strcmp(argv[1],"-uchar")) datatype = UCHAR;
	else usage(argv[0]);
	fnum++;
	}
 if (!strcmp(argv[fnum],"-")) fd = 0;
 else if ((fd = open(argv[fnum],0)) == -1) { perror(argv[fnum]); exit(); }
 switch (datatype) {
	case FLOAT: minmax_float(fd); break;
	case LONG: minmax_long(fd); break;
	case ULONG: minmax_ulong(fd); break;
	case SHORT: minmax_short(fd); break;
	case USHORT: minmax_ushort(fd); break;
	case CHAR: minmax_char(fd); break;
	case UCHAR: minmax_uchar(fd); break;
	}
 close(fd);
}
