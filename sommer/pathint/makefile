SOURCES=hankel.f wrap.c pathint.f nahfeld_re.f nf.c
PI_OBJS=hankel.o wrap.o pathint.o
NF_OBJS=nf.o nahfeld_re.o


CFLAGS=$(DEBUG)
# FFLAGS=$(DEBUG) -u
FFLAGS=-O2

LIBS= -L/usr/soft1/lib -lmath -lfuns

nf:	$(NF_OBJS)
	f77 -o nf $(NF_OBJS) -L/usr/soft1/lib -lfuns

hankel.exe: $(PI_OBJS)
	f77 -o hankel.exe $(PI_OBJS) $(LIBS)

depend:
	makedepend $(SOURCES)

clean:
	rm -f $(PI_OBJS) $(NF_OBJS) nf hankel.exe

# DO NOT DELETE

wrap.o: /usr/include/stdlib.h /usr/include/features.h
wrap.o: /usr/include/sys/cdefs.h
wrap.o: /usr/lib/gcc-lib/i486-linux/2.7.2.1/include/stddef.h
wrap.o: /usr/include/errno.h /usr/include/linux/errno.h
wrap.o: /usr/include/asm/errno.h /usr/include/alloca.h /usr/include/math.h
wrap.o: /usr/include/huge_val.h /usr/include/endian.h /usr/include/bytesex.h
wrap.o: /usr/include/nan.h
wrap.o: /usr/lib/gcc-lib/i486-linux/2.7.2.1/include/float.h
wrap.o: /usr/include/values.h /usr/include/ieee754.h /usr/include/ieee854.h
wrap.o: /usr/include/ctype.h
nf.o: /usr/include/math.h /usr/include/features.h /usr/include/sys/cdefs.h
nf.o: /usr/include/huge_val.h /usr/include/endian.h /usr/include/bytesex.h
nf.o: /usr/include/nan.h /usr/lib/gcc-lib/i486-linux/2.7.2.1/include/float.h
nf.o: /usr/include/values.h /usr/include/ieee754.h /usr/include/ieee854.h
