	subroutine hinit(t,y,pars)
	real t,y(*),pars(*)
	complex hank(2)
	complex h0,h1,lam,alf,krl,ue
	real er,kr
	alf=cmplx(pars(1),pars(2))
	lam=1+alf*t
	kr=pars(3)
	er=pars(4)
c	krl=cmplx(kr,0.)*lam
c calculate initial values
c	call cbesh(krl,0,1,2,2,hank,nz,ierr)
c	if (ierr.ne.0) then
c	  print *,"Error in 'cbesh':",ierr
c	endif
c	if (nz.ne.0) then
c	  print *,"underflow in 'cbesh'"
c	endif
c	h0=hank(1)
c	h1=-hank(2)/krl
	ue=csqrt(lam*lam-er)
	h0=cmplx(0.,1.)*csin(ue*cmplx(0.,-kr))
	h1=		ccos(ue*cmplx(0.,-kr))
	y(1)=real(h0)
	y(2)=imag(h0)
	y(3)=real(h1)
	y(4)=imag(h1)
	return
	end

	subroutine derivs(x,y,dy,rpar,ipar)
	real x,y(*),dy(*),rpar(*)
	integer ipar(*)
	real kr,true(4)
	complex h0,h1,lam,alf,krl,tmp,tmp1
	integer j
	alf=cmplx(rpar(1),rpar(2))
	kr=rpar(3)
	er=rpar(4)
	lam=1+alf*x
c	krl=cmplx(kr,0.)*lam
	h0=cmplx(y(1),y(2))
	h1=cmplx(y(3),y(4))
c	tmp=krl*kr*alf*h1
	tmp1=kr*lam*alf/csqrt(lam*lam-er)
	tmp=h1*tmp1
	dy(1)=real(tmp)
	dy(2)=imag(tmp)
c	tmp=(-h0-h1-h1)*alf/lam
	tmp=h0*tmp1
	dy(3)=real(tmp)
	dy(4)=imag(tmp)
c	call hinit(x,true,rpar)
c	dy(5)=y(5)
c	do 10 j=1,4
c	  tmp=y(j)/true(j)-1
c	  dy(5)=dy(5)+abs(real(tmp))+abs(imag(tmp))
c10	continue
	return

	end
