C calculate Sommerfeld-integral by path integration
C along the branch-cut of sqrt( l^2-1). The chosen path
C for l is l=1-I t.
C
C input parameters: krho:  distance to observation point (scaled by
C			   k0=2 Pi f/c; free space wavelength)
C		    eps  : permittivity of substrate
C		    kdick: substrate thickness, scaled by k0
C		    neqs : must be 4, 8 or 12
C			   if > 4 the Hankel function is obtained
C			   by integrating Bessel's DE along the
C			   same path
C			   if > 8 sinh() is also numerically calculated
C			   by integrating its DE along the path
C		    tend:  parameter t, where to stop the integration
C			   7./krho is an adequate value
C The routine calculates two integrals related to
C the green's function of the dielectric slab
C
C    I1: 1/2 Integrate[((2 elek-mag) H0(krho l) + (elek+mag) H0'(krho l)/krho/l) l dl/dt,{t,0,tend}]
C    I2: 1/2 Integrate[(mag H0(krho l) + (elek+mag)H0'(krho l)/krho/l) l dl/dt,{t,0,tend}]
C
C with the branch cut contributions
C
C    elek: Fel(u(l)) - Fel( -u(l))
C    magn: Fmg(u(l)) - Fmg( -u(l))
C
C where
C   
C    Fel = (1 - eps u(l)/Nel)/(ue(l))^2,
C    Fmg = (1 - ue(l)/Nmg)
C   
C    Nel = ue(l) Sinh(ue(l)kdick) + eps u(l)  Cosh(ue(l) kdick)
C    Nmg = u(l)  Sinh(ue(l)kdick) +     ue(l) Cosh(ue(l) kdick)
C    u   = Sqrt(l^2-1)
C    ue  = Sqrt(l^2-eps)
C
C NOTE !!!!!!!!!!!!!!!!!
C
C To avoid unnecessary multiplications, the multiplication by dl/dt (which is
C a constant due to simple path geometry) is performed OUTSIDE this routine.
C The constant factor of 2 which arises forming the difference Fel(u)-Fel(-u)
C cancels the factor 1/2 (see above).
C
C output parameters:
C
C    y(1) : Re(I1)
C    y(2) : Im(I1)
C    y(3) : Re(I2)
C    y(4) : Im(I2)
C     
C    y(5) : Re( Hankel2(0, krho l))
C    y(6) : Im( Hankel2(0, krho l))
C    y(7) : Re(-Hankel2(1, krho l)/krho/l)
C    y(8) : Im(-Hankel2(1, krho l)/krho/l)
C
C    y(9) : Re( Sinh( ue(l) kdick )
C    y(10): Im( Sinh( ue(l) kdick )
C    y(11): Re( Cosh( ue(l) kdick )
C    y(12): Im( Cosh( ue(l) kdick )
C
#ifdef HASNRECIPES
       subroutine derivs(t,y,yprime)
c	recipes' routines are indexed off 1 so we have to
c	adjust all the indices of y,yprime
#define I1RE	2
#define I1IM	3
#define I2RE	4
#define I2IM	5
#define H0RE	6
#define H0IM	7
#define H1RE	8
#define H1IM	9
#define SHRE   10
#define SHIM   11
#define CHRE   12
#define CHIM   13
#else
#define I1RE	1
#define I1IM	2
#define I2RE	3
#define I2IM	4
#define H0RE	5
#define H0IM	6
#define H1RE	7
#define H1IM	8
#define SHRE	9
#define SHIM   10	
#define CHRE   11 
#define CHIM   12
#ifdef HASIMSL
       subroutine derivs(neq,t,y,yprime)
       integer neq
#else
       subroutine derivs(t,y,yprime,rparms,iparms)
       real	rparms(*)
       integer iparms(*)
#endif
#endif
       real    t
       real    y(*),yprime(*)
       common/derivsglob/kr,akr2,er,kD,akD,er2,alpha,n
       real	kr,er,er2,kD
       complex akr2,akD,alpha,tmp
       integer n
       complex lam
       complex u,ue
       complex u2,ue2,sh,ch,sh2,ch2
       complex duedt
       complex res,magn,elek
       complex h02,h02lam,h12divr,h12lamdivr,hans(2)
       integer nz,ierr
c calculate lam u ue u^2
       lam=1.+alpha*t
       u2=(lam-1.)*(lam+1.)
       u=csqrt(u2)
       ue2=u2+1.-er
       ue=csqrt(ue2)
       if (n.gt.4) then
c integrate Bessel ODE along path
c g=H0(f(t)), h=H0'(f(t))/f(t)
c	g'=h f f'
       h02=cmplx(y(H0RE),y(H0IM))
       h12divr=cmplx(y(H1RE),y(H1IM))
       h12lamdivr=h12divr*lam
       res=h12lamdivr*akr2
       yprime(H0RE)=real(res)
       yprime(H0IM)=imag(res)
c	h'= (H0'' f f' - H0' f')/f^2 = (-(g+h) -h) f'/f
       res=(-h02-h12divr-h12divr)*alpha/lam
       yprime(H1RE)=real(res)
       yprime(H1IM)=imag(res)
c 
       else
#ifdef HASNRECIPES
c not implemented, link with amos library
c	call cbesh(kr*lam,0,1,2,2,hans,nz,ierr)
       ierr=1
#else
#ifdef HASIMSL
c not implemented, call equivalent of cbesh
c	call cbesh(kr*lam,0,1,2,2,hans,nz,ierr)
       ierr=1
#else
       call cbesh(kr*lam,0,1,2,2,hans,nz,ierr)
#endif
#endif
       if (ierr.ne.0) then
         print *,"Error in 'cbesh':",ierr
       endif
       if (nz.ne.0) then
         print *,"underflow in 'cbesh'"
       endif
       h02=hans(1)
       h12lamdivr=(-hans(2))/kr
       h12divr=h12lamdivr/lam
       end if
       if (n.gt.8) then
c integrate sinh ODE along path
       sh=cmplx(y(SHRE),y(SHIM))
       ch=cmplx(y(CHRE),y(CHIM))
       duedt=akD*lam/ue
       res=ch*duedt
       yprime(SHRE)=real(res)
       yprime(SHIM)=imag(res)
       res=sh*duedt
       yprime(CHRE)=real(res)
       yprime(CHIM)=imag(res)
       sh2=sh*sh
       ch2=1.+sh2
       else
c call functions
       sh=cmplx(0.,1.)*csin(ue*cmplx(0.,-kD))
       sh2=sh*sh
       ch2=1.+sh2
       endif
c calculate integrand
       res=u*ue*sh
       h02lam=h02*lam
       elek=er2*res/ue2/(er2*u2*ch2 - ue2*sh2)
       magn=res/(ue2*ch2-u2*sh2)
       tmp=(elek+magn)*h12lamdivr
       res=((elek+elek-magn)*h02lam+tmp)
       yprime(I1RE)=real(res)
       yprime(I1IM)=imag(res)
       res=(h02lam*magn+tmp)
       yprime(I2RE)=real(res)
       yprime(I2IM)=imag(res)
       return
       end

#ifdef HASNRECIPES
       subroutine greeninit(not_implemented)
       sorry, not implemented
#else
#ifdef HASIMSL
       subroutine greeninit(not_implemented)
       sorry, not implemented
#else
c
c Initialization of the integration
c
       subroutine greeninit(t,y)
       real t,y(*)
       common/derivsglob/kr,akr2,er,kD,akD,er2,alpha,n
       real	kr,er,er2,kD
       complex akr2,akD,alpha
       integer n
       complex lam,krlam,ue,hank(2),h0,h1
       lam=1+alpha*t
       krlam=kr*lam
c initialize the sums
       y(I1RE)=0.
       y(I1IM)=0.
       y(I2RE)=0.
       y(I2IM)=0.
c initialize the bessel functions
       if (n.gt.4) then
       call cbesh(krlam,0,1,2,2,hank,nz,ierr)
       h0=hank(1)
       h1=(-hank(2))/krlam
       y(H0RE)=real(h0)
       y(H0IM)=imag(h0)
       y(H1RE)=real(h1)
       y(H1IM)=imag(h1)
c initialize the hyperbolic functions	
       if (n.gt.8) then
       ue=csqrt(lam*lam-er)
       h0=cmplx(0.,1.)*csin(ue*cmplx(0.,-kD))
       h1=ccos(ue*cmplx(0.,-kD))
       y(SHRE)=real(h0)
       y(SHIM)=imag(h0)
       y(CHRE)=real(h1)
       y(CHIM)=imag(h1)
       endif
       endif
       return
       end
#endif
#endif
