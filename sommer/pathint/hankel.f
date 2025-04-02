	program   hankel
	integer   iargc,isyesorno,nsteps,nfns
	integer   i,inthank,neqs
	character str*50
	real      krho,eps,kdick,tend
	real	  x
	external  function pathint
	if (iargc().lt.3) then
	print *, "usage sommer [ODE-hankel (y/n)] kr er kD [tmax] "
	stop
	end if
	i=1
	call getarg(i,str)
	print *,"string",str
        inthank=isyesorno(str)
        if (inthank.ne.0) then
	 i=i+1
	 call getarg(i,str)
	 print *,"string",str
	end if
        call atof(krho,str)
	i=i+1
	call getarg(i,str)
	print *,"string",str
	call atof(eps,str)
	i=i+1
	call getarg(i,str)
	print *,"string",str
	call atof(kdick,str)
	i=i+1
	if (iargc().ge.i) then
	call getarg(i,str)
	print *,"string",str
	call atof(tend,str)
	else
	tend=7.
	end if
	if (inthank.ne.-1) then
	 neqs=10
	 print *,"ODE"
	else
	 print *,"SUBRUT"
	 neqs=2
	end if
	do i=1,20
	x=0.4+0.05*i
	print *,"{",x,",",real(pathint(x,eps,kdick,neqs,tend,nsteps,nfns)),"},"
	end do
c	print *,nsteps,"steps, ",nfns," function calls"
	stop
	end
