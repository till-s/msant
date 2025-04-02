// test inner angle computation

function [bet] = anglp(x,y)
	bet = %pi-atan((y(2)*x(1)-x(2)*y(1)),x(1)*x(2)+y(1)*y(2))
endfunction

// wait for 3 clicks on input
function [bet,betc]=angl()
	disp('click three points (draw vertex of polygon)')
	x = zeros(1,3)
	y = zeros(x)
	[b,x(1),y(1)]=xclick();
	[b,x(2),y(2)]=xclick();
	xarrows(x,y)
	[b,x(3),y(3)]=xclick();
	xarrows(x(2:3),y(2:3))
	x = diff(x,1)
	y = diff(y,1)
	bet = %pi./anglp(x,y)
	z   = x+imult(y)
	z   = z(2)./z(1)
	betc= %pi./(%pi-atan(imag(z),real(z)))
endfunction
	
