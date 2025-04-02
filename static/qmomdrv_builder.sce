// generated with intersci 
ilib_name = 'libqmomdrv'		// interface library name
files =['qmomdrv.o';
	'qmom'
	'd1mach'
	'drecur'
	'dgauss'
	'dpsi'
	'dcot'];
libs =['libqmomdrv'];

table =["qmom","intsqmom"];
ilib_build(ilib_name,table,files,libs);
