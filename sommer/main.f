	program tst
	common/global/a
	external kern
	complex kern
	complex dd,ddd
	a=cmplx(1.,0.)
	ddd=kern(dd,1.)
	print *,kern(dd,1.)
	stop 
	end
