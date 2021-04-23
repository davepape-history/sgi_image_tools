LIBDIR = $(HOME)/lib
INCDIR = $(HOME)/include
CFLAGS = -O2 -I$(INCDIR) -I/usr/local/include
LINKFLAGS = -L$(LIBDIR)
FLIBS = -L/usr/local/lib -lforms -lfm -lX11
LIBS = -ldave -limage -lgl -lm -lmalloc

light16: light16.c
	cc $(CFLAGS) -o light16 light16.c $(LINKFLAGS) $(LIBS)

ascmapimg: ascmapimg.c
	cc $(CFLAGS) -o ascmapimg ascmapimg.c $(LINKFLAGS) $(LIBS)

sgi2rgb: sgi2rgb.c
	cc $(CFLAGS) -o sgi2rgb sgi2rgb.c $(LINKFLAGS) $(LIBS)

yiqfilter: yiqfilter.c
	$(CC) $(CFLAGS) $(LINKFLAGS) -o yiqfilter yiqfilter.c $(LIBS)

fill-holes: fill-holes.c
	$(CC) $(CFLAGS) $(LINKFLAGS) -o fill-holes fill-holes.c $(LIBS)

imgcmp: imgcmp.c
	$(CC) $(CFLAGS) $(LINKFLAGS) -o imgcmp imgcmp.c $(LIBS)

get-matte: get-matte.c
	$(CC) $(CFLAGS) $(LINKFLAGS) -o get-matte get-matte.c $(LIBS)

sub2: sub2.c
	$(CC) $(CFLAGS) $(LINKFLAGS) -o sub2 sub2.c $(LIBS)

fromfloat: fromfloat.c
	$(CC) $(CFLAGS) $(LINKFLAGS) -o fromfloat fromfloat.c $(LIBS)

fromlong: fromlong.c
	$(CC) $(CFLAGS) $(LINKFLAGS) -o fromlong fromlong.c $(LIBS)

frombin16: frombin16.c
	$(CC) $(CFLAGS) $(LINKFLAGS) -o frombin16 frombin16.c $(LIBS)

frombin16u: frombin16u.c
	$(CC) $(CFLAGS) $(LINKFLAGS) -o frombin16u frombin16u.c $(LIBS)

remapimg: remapimg.c
	$(CC) $(CFLAGS) $(LINKFLAGS) -o remapimg remapimg.c $(LIBS)

ispline: ispline.c
	$(CC) $(CFLAGS) $(LINKFLAGS) -o ispline ispline.c $(LIBS)

gammawarp3: gammawarp3.c
	$(CC) $(CFLAGS) $(LINKFLAGS) -o gammawarp3 gammawarp3.c $(LIBS)

splinezoom-globe: splinezoom-globe.c
	$(CC) $(CFLAGS) $(LINKFLAGS) -o splinezoom-globe splinezoom-globe.c $(LIBS)

splinezoom: splinezoom.c
	$(CC) $(CFLAGS) $(LINKFLAGS) -o splinezoom splinezoom.c $(LIBS)

imgmerge: imgmerge.c
	$(CC) $(CFLAGS) $(LINKFLAGS) -o imgmerge  imgmerge.c $(LIBS)

imgscroll: imgscroll.c
	$(CC) $(CFLAGS) $(LINKFLAGS) -o imgscroll imgscroll.c $(LIBS)

smear: smear.c
	$(CC) $(CFLAGS) $(LINKFLAGS) -o smear smear.c $(LIBS)

wipe: wipe.c
	$(CC) $(CFLAGS) $(LINKFLAGS) -o wipe wipe.c $(LIBS)

dissolve: dissolve.c
	$(CC) $(CFLAGS) $(LINKFLAGS) -o dissolve dissolve.c $(LIBS)

fade: fade.c
	$(CC) $(CFLAGS) $(LINKFLAGS) -o fade fade.c $(LIBS)

fadein: fadein.c
	$(CC) $(CFLAGS) $(LINKFLAGS) -o fadein fadein.c $(LIBS)

removecolor: removecolor.c
	cc $(CFLAGS) $(LINKFLAGS) -o removecolor removecolor.c $(LIBS)

composite: composite.c
	cc $(CFLAGS) $(LINKFLAGS) -o composite composite.c $(FLIBS) $(LIBS)

trace-warp: trace-warp.c
	cc $(CFLAGS) -o trace-warp trace-warp.c $(LINKFLAGS) $(LIBS)

trace-edit: trace-edit.c
	cc $(CFLAGS) -o trace-edit trace-edit.c $(LINKFLAGS) $(LIBS)

trace: trace.c
	cc $(CFLAGS) -o trace trace.c $(LINKFLAGS) $(LIBS)

traceobj: traceobj.c
	cc $(CFLAGS) -o traceobj traceobj.c $(LINKFLAGS) $(LIBS)

movie2sgi: movie2sgi.c
	cc $(CFLAGS) -o movie2sgi movie2sgi.c $(LINKFLAGS) $(LIBS)

matte: matte.c
	cc $(CFLAGS) -o matte matte.c $(LINKFLAGS) $(LIBS)

colorunder: colorunder.c
	cc $(CFLAGS) -o colorunder colorunder.c $(LINKFLAGS) $(LIBS)

colorover: colorover.c
	cc $(CFLAGS) -o colorover colorover.c $(LINKFLAGS) $(LIBS)

tocfire: tocfire.c
	cc $(CFLAGS) -o tocfire tocfire.c -limage

fmovie: fmovie.o
	$(CC) $(LDFLAGS) -o fmovie fmovie.o $(FLIBS) $(LIBS)

