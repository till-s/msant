// Test linear basis functions for a simple singular problem:
// Strip in vacuum.
//
//          -1                  1
//           x------------------x
//
//
// Conformal map from the circle is the Joukowsky function
//
//  w = 1/2 (z+1/z)
//
// Hence, if z = exp(j fi) then w = cos(fi)
//
// Normal field / charge density is  imag(1/z /conj(dw/dz))
// with dw/dz = 1/2 ( 1 - 1/z^2 ).
// On the boundary this becomes  1/2/z (1-1/z).
// Substituting z=exp(i phi) -> j/z sin(phi) =>
//  sig ~ imag(1/z conj((-j) z / sin(phi))) = 1/sin(phi)
//
// -----------------------
// | sig ~ 1/sqrt(1-w^2) |
// -----------------------
//
// Moment's method with triangular basis function
//
//       A           B
//       o-----------o
//
// Charge density at x:  A/w (1-x/w) + B/w x/w  { -> charge on segment is A/2 + B/2; (other half is on adjacent segs) }
//
// -> Integrate   (x/w)^(nu-1) log(sqrt((x-u)^2 + c^2)), x=0..w
// (nu notation useful for non-integers))
//
// a) nu=1 --> I1 = u log(|u|) + (u-w)*log(|u-w|) - w
//    I1(0) = -w, I(1) = -w.
//    or complex:  Re{ (w-zo)*log(w-zo) - (-zo)*log(-zo) - w }
//
//    normalized integral 1/w I1(zo) = Re{ (1-zo/w)*(log(w) + log(1-zo/w)) - (-zo/w)(log(w)+log(-zo/w)) - 1 } 
//                                   = Re{ log(w) - 1 + (1-zo/w)*log(1-zo/w) - (-zo/w)*log(-zo/w)}
//
// b) nu=2 --> I2 = Re{ (w^2-zo^2)*log(w-zo)/2/w - (-zo^2)*log(-zo)/2/w - zo/2 - w/4
//    I2(0) = -1/4, I2(1) = -3/4.
//
//    normalized, 1/w I2(zo) = Re{ (1-(zo/w)^2)/2 (log w + log(1-zo/w)) - (-(zo/w)^2)/2 ( log(w) + log(-zo/w) ) - zo/w/2 - 1/4 }
//                           = 1/2 Re{ log(w) - 1/2 + (1-(zo/w)^2)*log(1-zo/w) - (-(zo/w)^2)*log(-zo/w) - zo/w }
//          0                                 1
//         / (1+x)*G((w x + (xo-u)) dx   +   / (1-x)*G((w x + (xo-u)) dx
//       -1                                 0
//
//                                              1
// first integral can be written as (1+x)=y    / y
//                                            0
//
// Matrix: Sij = I1(zij) - I2(zij) + I2(zi(j-1))
//
//
// For non-integer n:
//
//             1                            1                  w    1  t^(n+1-1)         1
// I(n,zo) =  / t^(n-1) log(w t - zo) dt = --- { log(w-zo) + ----  /  ---------- dt } = --- { log(w-zo) + z L(n+1,z) }
//           0                              n                 zo  0   1 - w/zo t         n
//                    1
// with  L(n+1,z)  = ---  2F1(1,n+1;n+2;z)
//                   n+1
//
// Using  c(1-z) F(a,b,c;z) - c F(a-1,b,c;z) + (c-b)z F(a,b,c+1;z) == 0
// 
// with c = n+1, b=1, a=n+1 ==>
//
//        (n+1)(1-z) F(1,n+1;n+1;z) - (n+1) F(1,n;n+1;z) + n z F(1,n+1;n+2;z) == 0
//
// using F(a,b;b;z) = (1-z)^(-a) yields
//
//  z/(n+1) F(1,n+1;n+2;z) = 1/n { F(1,n;n+1;z) - 1 }
//
// -> z L(n+1,z) = L(n) - 1/n
//
// Recursion to find   z L(n+2,z) = (z L(n+1,z))/z - 1/(n+1) = L(n,z)/z - 1/z/n - 1/(n+1)
//
// Special values at endpoints:
//  a) zo = w, z->1; First term of series (A&S 15.3.10) yields   F(1,n;n+1;1) = n ( Psi(1)-Psi(n)- ln(1-z) )
//     --> log(w-zo) + z L(n+1,z) = log(w-zo) + 1/n { n (Psi(1) - Psi(n) - ln(1-z)) - 1 }
//                                = log(w) + Psi(1) - Psi(n) -1/n
//
//  ==> I(n,w) = 1/n { log(w) + Psi(1) - Psi(n) -1/n }
//
//  b) zo = 0, z->infinity;  F(1,n+1,n+2,z) = (n+1)/n (-1/z) F(1,-n;1-n;1/z) + Gam(2+n)Gam(-n) (-1/z)^(n+1) F(n+1,0;n+1;1/z)
//                                          = (n+1)/n (-1/z) F(1,-n;1-n;1/z) + (n+1) pi csc(-n pi) (-1/z)^(n+1) 
//				     z/(n+1) F(1,n+1,n+2,z) = - 1/n F(1,-n;1-n;1/z) + pi csc(n pi) (-1/z)^n
//     series: ak =  Gam(k-n)/Gam(-n) Gam(1-n) / Gam(1-n+k) (1/z)^k = (-n)/(k-n) (1/z)^k
//
//  ==> I(n,0) = 1/n { log(w) - 1/n }
//
//  general:  I(n+1,zo) = 1/(n+1) { log(w-zo) + z L(n+2,z) } = 1/(n+1) { log(w-zo) + ( z L(n+1,z) )/z - 1/(n+1) ) }
//
//
//  Our case: n = 1/2,    F(1,1/2,3/2,z) = atan(sqrt(w/zo))/sqrt(w/zo)


function [y] = kern(t,nu,w,zo)
	y = real((t/w)^(nu-1)*log(t-zo))
endfunction

function [y] = hkern(t,nu,w,z)
	y = w*real(t^(nu)/(w*t-z))
endfunction

function [i1,i2]=stripIEnd(w,zo)
	be = (abs(zo)<1E-10)
	en = (abs(zo-w)<1E-10)
	i1  = zeros(zo)
	i2  = zeros(zo)
	i1(be) = 2*log(w)-4
	i2(be) = 2/3*log(w)-4/9
	i1(en) = 2*log(w)+4*log(2) - 4
	i2(en) = 2/3*log(w)+4/3*log(2)-16/9
	idx = ~(be|en)
	x   = w./zo(idx)
	xx  = sqrt(x)
	if (or(xx==0)) then
		pause
	end
	ii  = 2*(atanh(xx)./xx - 1)
	ll  = log(w-zo(idx))
	i1(idx) = real(2*(ll + ii))
	i2(idx) = real(2/3*(ll+(ii./x-2/3)))
//	i1(1) = intg(0,w,list(kern,1/2,w,zo(1)))/w
//	i2(1) = intg(0,w,list(kern,3/2,w,zo(1)))/w
endfunction

// Compute
//        1    w
//  i1 = ---  / log(x - zo) dx
//        w  0
//
//
//        1    w  x
//  i2 = ---  /  --- log(x - zo) dx
//        w  0    w
//
function [i1,i2]=stripI(w,zo)
	be = (zo==0)
	en = (zo==w)
	sing = (be|en)
	i1 = zeros(zo)
	i2 = zeros(zo)
	zo=zo(~sing)
	x = zo/w
	a = (1-x).*log(w-zo)
	b = x.*log(-zo)
	i1(~sing) = real(a + b - 1)
	i1(sing)  = log(w)-1
	i2(~sing) = real((1+x).*a+x.*b-x-1/2)/2
	i2(be)   = (log(w)-1/2)/2
	i2(en)   = (log(w)-3/2)/2
endfunction

function [S,z]=fillm(N,baseflag)
	w = 2/N;
	x = [-1:w:1]
	o = ones(x);
	z = o'*x;
	z = z'-z;
	S = zeros(z);
	for i=1:size(z,1)
		[i1,i2]=stripI(w,z(i,:));
		if (baseflag) then
			i2(1)=0; i2($-1)=0; i1($-1)=0;
		else
			S(i,1)=i1(1)-i2(1);
			S(i,$)=i2($-1)
		end
		S(i,2:$-1)   = i1(2:($-1))-i2(2:($-1))+i2(1:($-2))
//		S(i,3:($-1)) = i1(3:($-1))-i2(3:($-1))+i2(2:($-2))
//		S(i,2)       = (i1(2)-i2(2)) + i2(1)*3/4
//		S(i,1)       = (i1(1)-i2(1))*3/8
//		S(i,$)       = i2($-1)
	end
	if (baseflag) then
	[i1,i2] = stripIEnd(w,z(:,1))
	// we see only half the charge from endpoint; weight would be 3/8 on normal boundary
	S(:,1)  = (i1-i2)*3/4;
	S(:,2)  = (S(:,2) + i2)*6/7;
	[i1,i2] = stripIEnd(w,-z(:,$))
	S(:,$)    = (i1-i2)*3/4;
	S(:,($-1))= (S(:,($-1)) + i2)*6/7;
	end
endfunction

function [q]=calcQ(N,baseflag)
	s  = fillm(N,baseflag)
	qs = linsolve(s,ones(size(s,1),1))
	// sum charge in adjacent bins
	q  = zeros(size(qs,'*')-1,1)
	// average over middle segments
	q(3:$-2) = (qs(3:$-3)+qs(4:$-2))/2
	q(2)     = 1/2*qs(3) + 3/7*qs(2)
	q($-1)   = 1/2*qs($-2) + 3/7*qs($-1)
	q($)     = qs($)     + 4/7*qs($-1)
	q(1)     = qs(1)     + 4/7*qs(2)
endfunction

function [qmm,tru]=testQ(N,baseflag)
	qmm = calcQ(N,baseflag)
	qmm = qmm./sum(qmm)
	tru = diff(%pi/2+asin(linspace(-1,1,N+1)'),1)
	tru = tru./sum(tru)
endfunction
