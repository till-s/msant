C calculate Sommerfeld-integral by path integration
C along the branch-cut of sqrt( l^2-1). The chosen path
C for l is l=1-I t.
C
C input parameters: krho:  distance to observation point (scaled by
C			   k0=2 Pi f/c; free space wavelength)
C		    eps  : permittivity of substrate
C		    kdick: substrate thickness, scaled by k0
C		    neqs : must be 2, 6 or 10
C			   if > 2 the Hankel function is obtained
C			   by integrating Bessel's DE along the
C			   same path
C			   if > 6 sinh() is also numerically calculated
C			   by integrating its DE along the path
C		    tend:  parameter t, where to stop the integration
C			   7. is an adequate value
C		    nsteps:(RETURNs) nr. of RK-steps
C		    nfns:  (RETURNs) nr. of function evaluations
	complex function pathint(krho,eps,kdick,neqs,tend,nsteps,nfns)
	real 	  krho,eps,kdick,tend
	integer	  nsteps,nfns
c	complex   pathint
	common/global/kr,kr2,er,kD,er2
	real 	  kr,kr2,er,kD,er2
	real 	  IuekD
	external  fcn
	real      besj0,besj1,besy0,besy1
	integer   neqs,ido,i,frst
	real 	  params(50),y(15),t,tol
	data 	  frst/1/
	if (frst.eq.1) then
c initialize params
	  do 1 i=1,50
	    params(i)=0.
1	  continue
c set error norm
	  params(10)=2.
	  params(11)=0.01
c set maxsteps
	  params(4)=1000.
	  frst=0
	endif
	t=0.
	kr=krho
	er=eps
	kD=kdick
	kr2=kr*kr
	er2=er*er
	IuekD=sqrt(er-1)*kD
	y(1)=0.
	y(2)=0.
	if (neqs.gt.2) then
	  y(3)=besj0(kr)
	  y(4)=-besy0(kr)
	  y(5)=kr*besy1(kr)
	  y(6)=kr*besj1(kr)
	end if
	if (neqs.gt.6) then
	  y(7)=0.
	  y(8)=-sin(IuekD)
	  y(9)=cos(IuekD)
	  y(10)=0.
	end if
	tol=1.E-4
	ido=1
	call ivprk(ido,neqs,fcn,t,tend/kr,tol,params,y)
	ido=3
	call ivprk(ido,neqs,fcn,t,tend/kr,tol,params,y)
c	if (neqs.gt.2) then
c	print *,"Hankel Function at the end of path:",cmplx(y(3),y(4))
c	print *,"sinh   Function at the end of path:",cmplx(y(7),y(8)),
c     *           cmplx(0.,1.)*csin(csqrt( cmplx(1-tend**2,-2*tend)-er)*
c     *		                   cmplx(0.,-kD))
c	end if
c	print *,params(34),"steps taken"
	nsteps=params(34)
	nfns  =params(35)
	pathint=cmplx(y(1),y(2))
	return
	end
c
	subroutine fcn(n,t,y,yprime)
	integer n
	real    t
	real    y(*),yprime(*)
	common/global/kr,kr2,er,kD,er2
	real	kr,kr2,er,kD,er2
	complex prod
	complex u,ue
	complex u2,sh,ch
	complex duedt
	complex res
	complex hankel02,hans(2)
	integer nz,ierr
c calculate u ue u^2
	u2=cmplx(-t*t,-2.*t)
	u=csqrt(u2)
	ue=csqrt(u2+1-er)
	if (n.gt.2) then
c integrate Bessel ODE along path
	prod=cmplx(y(5),y(6))/cmplx(t,1.)
c	f'=g
	yprime(3)=y(5)
	yprime(4)=y(6)
c	(f''=) g'= f kr2 - g (t-j)/(1+t^2)
	yprime(5)=y(3)*kr2 - real(prod)
	yprime(6)=y(4)*kr2 - imag(prod)
c 
c 
	hankel02=cmplx(y(3),y(4))
	else
	call cbesh(kr*cmplx(1,-t),0,1,2,1,hans,nz,ierr)
	if (ierr.ne.0) then
	  print *,"Error in 'cbesh':",ierr
	endif
	if (nz.ne.0) then
	  print *,"underflow in 'cbesh'"
	endif
	hankel02=hans(1)
	end if
	if (n.gt.6) then
c integrate sinh ODE along path
	sh=cmplx(y(7),y(8))
	ch=cmplx(y(9),y(10))
	duedt=kD*cmplx(-t,-1.)/ue
	res=ch*duedt
	yprime(7)=real(res)
	yprime(8)=imag(res)
	res=sh*duedt
	yprime(9)=real(res)
	yprime(10)=imag(res)
	else
	sh=cmplx(0.,1.)*csin(ue*cmplx(0.,-kD))
	endif
	res=-2.*er*u*sh/ue/((ue*sh)**2 - (1+sh**2)*er2*u2)*
     *      cmplx(-t,-1.)*hankel02
	yprime(1)=real(res)
	yprime(2)=imag(res)
	return
	end
