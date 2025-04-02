	subroutine casl(g0,g1,kr)
	complex g0,g1,frei0,frei1
	real	kr
	frei0=cmplx(0.,1)*cexp(cmplx(0.,-kr))/kr
	frei1=(-frei0)*cmplx(1/kr**2,1/kr)
	print *,cabs(1-g0/frei0),cabs(1-g1/frei1)
	return
	end
