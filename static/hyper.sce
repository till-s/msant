//
// Evaluation of the Hypergeometric function 2F1(1,nu;nu+1;z) by various means
// ---------------------------------------------------------------------------
//
//
// Transformation formula for z around 1 (A&S, pp 216.)
// ====================================================
//
//                             inf
//                   G(nu+1)   ---  (nu)n (1)n                                        n
// F(1,nu,nu+1,z) = ---------   >  ----------- { Psi(n+1) - Psi(n+nu) - ln(1-z) } (1-z)
//                  G(nu) G(1) ---   (n!) (n!)
//                             n=0
//
// ( Pochhammer (nu)n = G(nu+n)/G(nu) = (nu+n-1) (nu+n-2) .. nu )
//
// with  (z = w/zo)
//
//      1                            1
//     / t^(n-1) log(w t - zo) dt = --- { log(w-zo) + 1/n { F(1,n;n+1;z) - 1} }
//    0                              n
//
//                                   1
//                                = --- { log(zo) + log(z-1) + 1/n { F(1,n;n+1;z) - 1 } }
//                                   n
//
// using above series expansion (G(n+1)/G(n)/G(1)/n == 1) and since we are interested only
// in the real part (-> log(|z-1|) = log(|1-z|)
//
//                                             inf
//  F(1,n;n+1;z)                               ---  (n)k                                        k
//  ------------ = Psi(1) - Psi(n) - ln(1-z) + >  ------ { Psi(k+1) - Psi(k+n) - ln(1-z) } (1-z)
//       n                                     ---  (k!)
//                                             k=1
//                  1
// ==> I(n,w,zo) = --- { log(zo) + Psi(1) - Psi(n) - 1/n + S(z,n) }
//                  n
//
//                               inf
//                               ---                                           k
//     S(z,n)    = { 0 (z==1),   >    ak { Psi(k+1) - Psi(k+n) - ln(1-z) } (1-z)
//                               ---  
//                               k=1
//
//     Recursion:   a(k+1)   (n+k)
//                 ------- = -----    ; a(1) = n ; a(k) = (n+k-1)/k * a(k-1)
//                  a(k)     (k+1)
//
//     Psi(z+1) = Psi(z) + 1/z
//     
//
//     DP(k+1) = Psi(k+2) - Psi(k+1+n) = Psi(k+1) + 1/(k+1) - Psi(k+n) - 1(k+n)
//             = DP(k) + (k+n - k-1)/(k+1)/(k+n) = DP(k) + (n-1)/(k+1)/(k+n)
//     DP(k)   = DP(k-1) + (n-1)/k/(k+n-1)
//
//  Transformation Formula for small p = 1/z (A&S, pp 15.3.7)
//  =========================================================
//  Note: using a limiting process on 15.3.14 probably yields the same result.
//
//        we use p = 1/z
//
//
//                        G(1+n) G(n-1)       1
//      F(1,n;1+n;z) =    ------------- ( -p )   F(1,1-n;2-n;p) 
//                         G(n)   G(n)
//
//                        G(1+n) G(1-n)      n 
//                     +  ------------- ( -p )   F(n,0;n;p) 
//                         G(1)   G(1)
//
//
//                                inf
//                       n        --  k! G(1-n+k) G(2-n)   k         n
//                   = ----- (-p) >   ------------------  p    +  (-p) G(1+n) G(1-n)
//                     (n-1)      --  k! G(1-n) G(2-n+k)
//                                k=0
//
//
//                                inf
//                       n        --   (1-n)   k         n
//                   = ----- (-p) >   ------- p    +  (-p) G(1+n) G(1-n)
//                     (n-1)      --  (1-n+k)
//                                k=0
//
//       using G(s) G(1-s) = pi csc(pi s) = pi / sin(pi s)
//
//        
//                        inf    (k+1)                                inf    k
//                        --     p             n      pi              --    p          n    pi
//                   = n  >   -------   +  (-p) n  --------    = n {  >   -----   + (-p)  --------   }
//                        --  (1-n+k)              sin(pi n)          --  (k-n)           sin(n pi)
//                        k=0                                         k=1
//
//  assume m is the closest integer to n, i.e., n = m+d
//
//
//                           inf    k
//                           --    p         m   1         (m+d)    pi
//                   =  n {  >   -----   +  p  -----  + (-p)     -------------  }
//                           --  (k-n)         m-m-d             sin((m+d) pi)
//                           k=1,
//                           k!=m
//                                                                      m
//  using sin((m+d)pi) = sin(m pi) cos(d pi) + cos(m pi) sin(d pi) = (-1) sin(d pi)
//
//
//                           inf    k                    d
//                           --    p         m    pi (-p)        1
//                   =  n {  >   -----   +  p  { ---------  -   --- } } 
//                           --  (k-n)           sin(pi d)       d
//                           k=1,
//                           k!=m
//
//                   =  n {    S         +  R }
//
//                                         
//                                           -- m-1  p^k
//  If d->0 then the first two sums S become >      -----   - (p)^m  log(1-p)
//                                           -- k=1 (k-m)
//
//  and for R
//                                       d           d
//              R   (-m)          pi (-p)  + d pi (-p) log(-p) - pi cos(pi d)
//        lim  --- p     =  lim   ------------------------------------------
//        d->0  n           d->0         sin(pi d) + d pi cos(pi d)
//
//                                        d                      2    d      2
//                               2 pi (-p)  log(-p) + d pi log(-p) (-p)  + pi  sin(pi d)     2 pi log(-p)
//                       =  lim  -------------------------------------------------------  =  ------------  = log(-p)
//                          d->0    pi cos(pi d) + pi cos(pi d) - d pi pi sin( pi d)             2 pi
//
//
//  Evaluation by integration:
//  ==========================
//  We have
//
//     z/(n+1) F(1,n+1;n+2;z) = 1/n { F(1,n;n+1;z) - 1 }
//
//        1   t^n            1                      1
//       /  ------- dt  = ------ F(1,n+1;n+2;z) = ----- { F(1,n;n+1;z) - 1 }
//      0    1-z t         (n+1)                   z n
//
//   ==> F(1,n;n+1;z) = z n In(z) + 1

// Brute force integration - doesn't work well if z is close to the
// branch cut 1..+infty
function [y]=Fnint(n,z)
	// complete integrand; 
	function [y]=f1intg(t)
		y = t^n/(1-z*t)
	endfunction
	y = z*n*intc(0,1,f1intg)+1
endfunction

// If we integrate using gauss' rules then the t^n weight is handled by the rule

function [y]=F1factor(t,z)
	y = 1../(1-t*z)
endfunction

// gauss-jacobi quadrature of F1
function [y]=Fnjac(n,z,varargin)
	[lhs,rhs]=argn(0)
	if ( rhs > 2 ) then
		N = varargin(1)
	else
		N = 20
	end
	[a,b] = orthpol_jacbi(N,0,n)
	[x,w] = orthpol_gaussq(a,b)
	t     = (x+1)/2
	y     = z*n*sum(w.*F1factor(t,z))/2^(n+1) + 1
endfunction

// gauss-laguerre quadrature of F1
function [y]=Fnlag(n,z,varargin)
	[lhs,rhs]=argn(0)
	if ( rhs > 2 ) then
		N = varargin(1)
	else
		N = 20
	end
	[a,b] = orthpol_glagr(N,0)
	[x,w] = orthpol_gaussq(a,b)
	t     = exp(-x/(n+1))
	y     = z*n*sum(w.*F1factor(t,z))/(n+1) + 1
endfunction

// psi series expansion for z close to 1. This is numerically problematic for
// large n since the 'an' coefficients grow very fast before they are
// eventually knocked down by the (1-z)^k.
// Not a problem if (1-z)*n not too large (1..10).
// This function only does the terms for k>=1 (so we can cancel the log(1-z)
// in the 0th order term with what appeared as a result of the t^(n-1) integration.
function [y,a]=psiexp(n,z,k)
	lim = 10000
	if ( k >= 1 ) then
		idx = k
		y = zeros(k)
		a = zeros(k)
		eps = 0
	else
		idx = lim
		eps = k
		y = 0
	end
	k = 0

	A = 1
	Z = 1
	L = log(z)
	del = 1
	DP = dlgamma(1)-dlgamma(n)
	while (k<idx & abs(del) > eps) do
		k=k+1
		A = A*(n+k-1)/k
		Z = Z*z
	    DP   = DP + (n-1)/k/(k+n-1)
		del  = A*(DP-L)*Z
		if (eps==0) then
			a(k) = A
			y(k) = del
		else
			y = y+del
		end
	end
	if ( k >= lim ) then
		error("Psi expansion didnt converge")
	end
	if ( eps ~= 0 ) then
		a = k
	end
endfunction

// series expansion around z=0
function [y,k]=Fnser(n,z)
	if (abs(z)>1 | (z==1)) then
		error("Hypergeometric series doesn''t converge for your ''z''");
	end
	Z   = 1;
	y   = 0;
	k   = 0;
	del = 1;
	eps = 1E-8
//
//  F(1,n,n+1,z) --> (1)k (n)k         G(n+k) G(n+1)    n G(n)             n
//                  ----------  z^k  = --------------------------  z^k = ----- z^k
//                   (n+1)k k!         G(n)  G(n+k+1) (n+k) G(n+k)        n+k
///
	while ( (abs(del) > eps) ) do
		Z   = Z*z
		k   = k+1
		if ( k > 10000 ) then
			error("Hypergeometric series apparently does not converge")
		end
		del = Z/(n+k)
		y   = y + del
	end
	y = y*n+1
endfunction

// Series expansion around p = 0 (z->infinity)
function [y,k]=Fnserinf(n,p)
	if (abs(p)>=1) then
		error("Hypergeometric series (expansion at inf) doesn''t converge for your ''p'' (must be < 1)");
	end
	Z   = 1;
	y   = 0;
	k   = 0;
	del = 1;
	eps = 1E-8
	m   = round(n)
//
//
	while ( (abs(del) > eps) ) do
		Z   = Z*p
		k   = k+1
		if ( k > 10000 ) then
			error("Hypergeometric series apparently does not converge")
		end
		if ( k ~= m ) then
			del = Z/(k-n)
			y   = y + del
		end
	end
	if ( p ~= 0 ) then
		d = n-m
		if ( d == 0 ) then
			R = log(-p)
		else
			R = ((-p)^d / sinc(%pi*d) - 1)/d
		end
		y = y + p^m * R
	end
	y = y*n
endfunction

function [y,k]=Fnpsi(n,z)
	eps = 1E-8
	// psi expansion
	zz = 1-z
	[s,k] = psiexp(n,zz,eps)
	y = n*(dlgamma(1) - dlgamma(n) - log(zz) + sum(s))
endfunction

function [y,k]=Fn(n,z,varargin)
	[lhs,rhs]=argn(0)
	if (rhs>2) then
		sep = varargin(1)
	else
		sep = 0.5
	end
	if (abs(z)<sep) then
		[y,k] = Fnser(n,z)
	elseif (abs(1-z)<sep) then
		[y,k] = Fnpsi(n,z)
	else
		if (imag(z)==0) then
			error('cannot integrate on branch cut')
		end
		y = Fnint(n,z)
		k = -1
	end
endfunction

function [y]=far(n,zo)
	z = 1/zo
	y = (log(zo) + log(z-1) + 1/n*( Fn(n,z) - 1 ))/n
endfunction
