	subroutine kern(res,s)
	common/global/a
	complex a
	complex res,dd
	real	s
c	res=cmplx(1/s/s,0.)
	res=cexp(a*s)
	return
	end
