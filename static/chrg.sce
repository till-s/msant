// 2D free space potential of a charge distribution
// along a linear conductor [note that 1/2 < n < +infty; corresponding to
// 'inner' angles of 2pi..0; n = pi/phi].
//
//                    (n-1)
//        sig(t) = S t
//
// If we express S as the total charge on the segment (length 'w'), then //
//            n                         -n
//   Q = S/n w       and hence S = n Q w
//
// as it arises from a current element located at a discontinuous boundary (vertex).
//
//                    -n  w  (n-1)          ->     ->
//    V(P) = eps Q n w   /  t      log( | t eq - r epq | ) dt
//                      0
//
//                    ->
//                    eq     w
//              Q _---------->
//               |\
//                 \
//                  \
//                   \
//                    P
//
// If rPQ is expressed as a complex number 'p' then we can write
//
//                      ~
//     V(P) = eps Q Re{ I(n,w,p) }
//
//     ~             -n  w  (n-1)                        -n   1   (n-1)  n-1
//     I(n,w,p) = n w   /  x       log( x - p ) dx  = n w    /   w      t    log( w t - p ) w dt
//                     0                                    0
//
//                   1                  1   (n-1)
//              = n --- log(p)   +  n  /  t      log( t z - 1 ) dt = log(w) - log(z) + I(n,z)
//                   n                0
//
//     ~                                                       ~
//     I(n,w,p) can be obtained from the 'normalized' integral I(n,1.0,p):
//
//     ~                   ~
//     I(n,w,p) = log(w) + I(n,1.0,p)
//
//
// with z = w/p and substituting
//
//                  1  (n-1)
//     I(n,z) = n  /  t     log( t z - 1 ) dt    
//                0
// 
// we have ( I(n,z) is a correction to the 0th order approximation V=eps Q log(p) )
//
//    V(P) = eps Q ( log(w) + Re{ I(n,z) - log(z) } )
//
//
// the integral can be expressed as
//
//                                    n                                    n
//                            1      t                               1    t
//    I(n,z) = log(z - 1) -  /  ---------- z dt  = log( z - 1 ) + z /  ------- dt
//                          0    t z - 1                           0   1 - z t
//
//           = log(z-1) + z II(n,z)
//
// and the latter can be expressed as a hypergeometric function (Re{c} > Re{b} > 0)
//
//                                       
//                       Gam(c)        1  (b-1)    (c-b-1)     (-a)
//     F(a,b;c;z) = ---------------   /  t     (1-t)      (1-zt)    dt
//                  Gam(b) Gam(c-b)  0    
//
// and hence (b-1 = n, c-b-1 = 0, a = 1)
//
//                  Gam(n+1) Gam(1)                    1
//     II(n,z)    = --------------  F(1,n+1;n+2;z) = ----- F(1,n+1;n+2;z)
//                     Gam(n+2)                      (n+1)
//
// and it can be shown (cf. mmtest.sce) that
// 
//       z                     1
//     ----- F(1,n+1;n+2;z) = --- ( F(1,n;n+1;z) - 1 )
//     (n+1)                   n
//
// so 
//                              1
//     I(n,z)     = log(z-1) + --- ( F(1,n;n+1;z) - 1 )
//                              n
//
// or in integral form
//                                                           (n-1)
//                              1      Gam(n+1)       1     t
//                = log(z-1) + --- ( ------------    /  --------- dt  - 1)
//                              n    Gam(n) Gam(1)  0    1 - z t
//
//
//                                            (n-1)
//                              1       1     t
//                = log(z-1) - ---   + /  --------- dt 
//                              n     0    1 - z t
//
// and hence
//
//                                                             (n-1)
//     ~                                          1     1     t
//     I(n,w,z)   = log(w) - log(z) + log(z-1) - --- + /  --------- dt 
//                                                n   0    1 - z t
//
//                                           (n-1)
//                              1     1     t
//                = log(w-p) - --- + /  --------- dt 
//                              n   0    1 - z t
//
//
// Iteration of the second term; from
//
//     ~
//     I(n,w,z)  = log(w-p) + z II(n,z) = log(w-p) + AA
//
// and the recursion formula  z II(n,z) = II(n-1,z) - 1/n we obtain
//
//   ~                        z II(n-1,z)       1                1        1
//   I(n+1,w,z)  = log(w-p) + -----------  -  ----- = log(w-p) + - AA - -----
//                                 z          (n+1)              z       n+1
//
// NUMERICAL EVALUATION:
//
// a) limits at the endpoints (z->inf, z->1) can readily be obtained analytically
//
// b) for small z (== large distances, r > ~w/.7) the hypergeometric series
//    converges reasonably well.
//
// c) for small p (p = 1/z -> small distances) we can use the transformed hypergeometric
//    series
//                             n
//                          (-p)         n      1
//.    F(1,n;n+1;z) =  { ---------   -  --- (-p)   F(1,1-n;2-n;p) }
//                       sinc(n pi)     1-n 
//
//    If n converges to an integer, the limit can still be calculated (see hyper.sce)
//
// d) for z close to 1 (P close to endpoint of segment)
//    a) small n (n*z < ~10) the 'psi expansion' converges reasonably well
//       but NOT for otherwise large n.
//       NOTE: the 0th term of the series  cancels the 'log(z-1)' in I(n,z)
//    
//    Note, that the most important case of a vertex (angle pi/n), the
//
//    point P is approximately at i*pi/n, so |z*n| is pi
//
// e) for large n the gauss-laguerre quadrature seems to work well
//    (maybe except for the real axis z=0..1 but this shouldn't really
//    happen for normal geometries).
//
// e1) for z close to the interval 0..1 where the denominator becomes
//    singular, evaluation of I(n,z) directly may be desirable.
//    (for n>2, it could be advantageous to integrate the log() once
//    analytically).


//    ~              1        1         1  (n-1)
//    I(n,1,p) = I(n,-) - log(-)  = n  /  t     log( t z - 1 ) dt - log(z)
//                   p        p       0
//
function [y]=qItot(n,p,varargin)
	[lhs,rhs]=argn(0)
	if (rhs>2) then force=varargin(1), else force=0, end
	if ( n < 1/2 | imag(n)~=0 ) then
		error("n must be real and > 0.5")
	end
	if ( p==0 ) then
		y = -1/n
		return
	end
	if ( p==1 ) then
		// i pi appears due to log(z-1) - log(1-z)
		y = dlgamma(1) - dlgamma(n) - 1/n + %i*%pi
		return
	end
	if ( force > 0 | (force == 0 & abs(imag(p)) < 1E-3) ) then
		// complex path; half-circle in upper real plane
		function [w]=kern(t)
			z1 = exp(-%pi*imult(t))
			z  = (1-z1)/2
			dz = (-z1/2)*(-%pi*%i)
			w = z^(n-1)*log(z-p)*dz
		endfunction
	else
		function [w]=kern(t)
			w = t^(n-1)*log(t-p)
		endfunction
	end
	y = intc(0,1,kern)*n
endfunction

// compute using hypergeometric series
//     ~                1         1              1              1
//     I(n,1,p)   = log(- - 1) + --- ( F(1,n;n+1;-) - 1 ) - log(-)
//                      p         n              p              p
//
function [y,kmax]=qIser(n,p)
    [s,kmax] = Fnser(n,1/p)
	y = (s-1)/n + log(1-p)
endfunction

// compute using hypergeometric series transformed to 1/z , i.e., around p=0
function [y,kmax]=qIserinf(n,p)
    [s,kmax] = Fnserinf(n,p)
	y = (s-1)/n + log(1-p)
endfunction

// compute using psi expansion of hypergeometric function
//
//     ~                1         1              1              1
//     I(n,1,p)   = log(- - 1) + --- ( F(1,n;n+1;-) - 1 ) - log(-)
//                      p         n              p              p
//
function [y,kmax]=qIpsi(n,p)
	z = 1../p
	// i pi appears due to log(z-1) - log(1-z)
	[ps,kmax] = psiexp(n,1-z,1E-8);
	y = dlgamma(1) - dlgamma(n) + ps - 1/n - log(z) + %i*%pi
endfunction

// compute gauss quadrature (order k) of f for jacobi weights;
// f is list(f,p1,p2,...)
function [y]=jacq(k,n,f)
	[a,b] = orthpol_jacbi(k,0,n)
	// b(1) contains the measure int(p(t),t=-1..1)
	// which can become very large - the 2^(n+1) contribution
	// cancels with our renormalization when going from int(t^n,0..1) -> a int((1+t)^n,-1..1)
	b(1)  = 1../(n+1)
	[x,w] = orthpol_gaussq(a,b)
	// map -1..1 -> 0..1
	t     = (x+1)/2
	y     = sum(w.*f(1)(t,f(2:$)))	// renormalization by 1/2^(n+1) already done
endfunction

// compute gauss quadrature (order k) of f for laguerre weights
// f is list(f,p1,p2,...)
function [y]=lagq(k,n,f)
	[a,b] = orthpol_glagr(k,0)
	[x,w] = orthpol_gaussq(a,b)
	// map 0..infty -> 0..1
	t     = exp(-x./(n+1))
	y     = sum(w.*f(1)(t,f(2:$)))/(n+1)	// renormalize 
endfunction

// integrand
function [y]=gaussintg(t,p)
	y = log(t-p)
endfunction

// derivative of integrand
function [y]=gaussintgp(t,p)
	y = 1../(t-p)
endfunction

// integrate full integrand with gauss-jacobi
function [y]=qIjac(n,p,varargin)
	[lhs,rhs]=argn(0)
	if ( rhs > 2 ) then N=varargin(1), else N=20, end
	y = jacq(N,n-1,list(gaussintg,p))*n	
endfunction

// integrate partial, then gauss-jacobi
// (rationale: 1/(t-p) easier to compute than log(1-p))
function [y]=qIjac1(n,p,varargin)
	[lhs,rhs]=argn(0)
	if ( rhs > 2 ) then N=varargin(1), else N=20, end
	y = gaussintg(1,p) - jacq(N,n,list(gaussintgp,p))
endfunction

// integrate partial, then gauss-laguerre
// (rationale: 1/(t-p) easier to compute than log(1-p))
function [y]=qIlag1(n,p,varargin)
	[lhs,rhs]=argn(0)
	if ( rhs > 2 ) then N=varargin(1), else N=20, end
	y = gaussintg(1,p) - lagq(N,n,list(gaussintgp,p))
endfunction

// integrate full integrand with gauss-laguerre
function [y]=qIlag(n,p,varargin)
	[lhs,rhs]=argn(0)
	if ( rhs > 2 ) then N=varargin(1), else N=20, end
	y = lagq(N,n-1,list(gaussintg,p))*n	
endfunction

// analytical solution for nu=0.5, 1.5 known
// These are the complete integrals
//
//    ~
//    In(n,1,p)
//
function [y]=qInu0p5(p)
	y = log(1-p)-2*(1-sqrt(p)*atanh(1../sqrt(p)))
endfunction

function [y]=qInu1p5(p)
	nu = 3/2
	y = log(1-p)-((1+3*p)-3*p^nu*atanh(1../sqrt(p)))/nu
endfunction
