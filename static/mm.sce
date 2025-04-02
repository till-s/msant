// give polygon 'x' as row vector of complex numbers

// rotate by one left; 
function [y]=rotl(x)
	y = [ x(2:$), x(1) ]
endfunction

// rotate by one right;
function [y]=rotr(x)
	y = [ x($), x(1:$-1) ]
endfunction

// compute inner angles divided by PI
function [y]=angli(x)
	y = 1 - phasemag((rotl(x) - x)./(rotr(x)-x))/180;
endfunction

// compute 'index' (exponent of poisson solution for wedge)
//  nu = pi/phi
function [y]=nuind(x)
	y = 1../angli(x)
endfunction
