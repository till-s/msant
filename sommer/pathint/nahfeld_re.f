	subroutine calc_params(er,kD,ampl,la1,fi1)
	real    er,kD
	complex la1,fi1
	real    ampl,alfa
	complex a1
	complex catanh
	real    pi
	data    pi / 3.14159265358979324E0 /
	alfa=log((er-1)/(er+1))
	a1=cmplx(-pi,alfa)/2./kD
	la1=csqrt(1-a1*a1)
	fi1=catanh(a1)
	ampl=(pi*pi+alfa*alfa)/2/er/(-alfa)
	return
	end
