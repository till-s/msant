SOURCES = main.c happy.c fft.c cosint.c tst.c sommod.c cosint.c

# nur mit /usr/lang/libm linken, da
# sincospi() in der normalen library FALSCH ist
# (cospi(1)=1 statt -1) !!!

LIBM=-L/usr/lang -lm
CFLAGS=-O
FFLAGS=$(CFLAGS)


cosint: cosint.o sommod.o
	f77 -o cosint happy.o cosint.o sommod.o $(LIBM)

fft: main.o happy.o fft.o
	f77 -o fft main.o happy.o fft.o $(LIBM) -lmath

tst:	happy.o fft.o tst.o
	f77 -o tst $(CFLAGS) tst.o happy.o fft.o $(LIBM) -lmath

depend:
	makedepend $(SOURCES)
# DO NOT DELETE
