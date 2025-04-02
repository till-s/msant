SOURCES=nrlib.c driver.c kern.f

CC=acc
CFLAGS=-g
FFLAGS=-g

expint: nrlib.o driver.o kern.o
	f77 -o expint driver.o kern.o nrlib.o

depend:
	makedepend $(SOURCES)
# DO NOT DELETE

nrlib.o: /usr/include/math.h /usr/include/floatingpoint.h
nrlib.o: /usr/include/sys/ieeefp.h /usr/include/stdlib.h
nrlib.o: /usr/include/sys/stdtypes.h /usr/include/stddef.h nrlib.h
nrlib.o: /usr/include/stdio.h
driver.o: /usr/include/stdio.h /usr/include/math.h
driver.o: /usr/include/floatingpoint.h /usr/include/sys/ieeefp.h
driver.o: /usr/include/stdlib.h /usr/include/sys/stdtypes.h
driver.o: /usr/include/stddef.h nrlib.h
