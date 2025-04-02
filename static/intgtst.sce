
if (singular==0) then
// make integrand 'smooth'  (numerically integrate  [ with m = n-1 ] (m) t^(m-1) (t-z)(log(t-z)-1)

// ikern is   (t-z)(log(t-z)-1)
// to be used by the gauss integrators
function [y]=ikern(t,m, z)
	x = (t-z)
	y = zeros(x)
	i = x~=0
	y(i) = x(i).*(log(x(i))-1)*m
endfunction

else
// use 'real' integrand

function [y]=ikern(t,n,z)
	y = log(t-z)
endfunction

end

// iikern is t^(m-1) * ikern
// to be used by the general purpose integrator (for comparison with gauss)
function [y]=iikern(t,m,z)
	y = real((t^(m - 1)) * ikern(t, m, z))
endfunction

// compute gauss quadrature (order k) of ikern for jacobi weights
function [y]=jacq(k,n,z)
	[a,b] = orthpol_jacbi(k,0,n-1)
	[x,w] = orthpol_gaussq(a,b)
	y     = sum(ikern((x+1)/2,n,z).*w)/(2^n)
endfunction

// compute gauss quadrature (order k) of ikern for laguerre weights
function [y]=lagq(k,n,z)
	[a,b] = orthpol_glagr(k,0)
	[x,w] = orthpol_gaussq(a,b)
	y     = sum(w.*ikern(exp(-x./n),n,z))/n
endfunction

// compute error of gauss vs. full-blown integrator
function [er,ea,i,ii]=interr(f,k,n,z)
	i  = intg(0,1,list(iikern,n,z))
	ii = f(k,n,z)
	ea = i-ii
	er = real(ea)/real(i)
endfunction
