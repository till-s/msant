(***********************************************************************
This file was generated automatically by the Mathematica front end.
It contains Initialization cells from a Notebook file, which typically
will have the same name as this file except ending in ".nb" instead of
".m".

This file is intended to be loaded into the Mathematica kernel using
the package loading commands Get or Needs.  Doing so is equivalent to
using the Evaluate Initialiation Cells menu command in the front end.

DO NOT EDIT THIS FILE.  This entire file is regenerated automatically 
each time the parent Notebook file is saved in the Mathematica front end.
Any changes you make to this file will be overwritten.
***********************************************************************)





BeginPackage["sommerfeld`"]

chi::usage="Sqrt[l^2-1]"

chie::usage="Sqrt[l^2-er]"

nenn::usage="nenn[l,chi]:= chie Sinh[chie kD]+ chi er Cosh[chie kD]"

rauhkern::usage="(1-er chi/nenn)/chie^2"

kern::usage="(1-er chi/nenn(chi)) lam/chie^2"

kernlinks::usage="(1+er chi/nenn(-chi)) lam/chie^2"

Hankel02::usage ="Hankel function of the second kind and order zero"

Hankel::usage=
  "Hankel[n,z]: hankel function of the second kind (order n, argument z)"

(*

kernpur

kernlinks

logkern

kernpurlinks

kernpurkorr

*)

POLrule::usage=
  "POLrule finds the real root of 'nenn'; a substituation rule of the form \
'lpsw->1.234' is retured"

SFW::usage=
  "SFW[kr_,kern_] computes the surface wave pole contribution of the SFI with \
the spectral function 'kern'"

residuum::usage=
  "residuum[kern] computes the residue of 'kern' (implicitely assuming 'nenn' \
as denominator). The lies on the right side of the integrating contour."

wegint::usage=
  "utility to compute complex line integrals \
wegint[kern,f,lam[t],t,ta,te,kr]:=Nintegrate[kern[lam] f[krho lam] \
Dt[lam,t],{t,ta,te}]"



Begin["`Private`"]




Global`er::nonum="Global`er; permittivity must be numerical"
		

Global`kD::nonum="Global`kD; substrate height must be numerical"

er:=(If[! NumberQ[Global`er],Message[Global`er::nonum]]; Global`er)

kD:=(If[! NumberQ[Global`kD],Message[Global`kD::nonum]]; Global`kD)



Hankel02[argu_] := 
   BesselJ[0, argu] - I*BesselY[0, argu];
Hankel[n_,z_]:=BesselJ[n,z]-I BesselY[n, z];



chi[lam_]:=Sqrt[lam^2-1];chie[lam_]:=Sqrt[lam^2-er];
nenn[lam_,chi_]:=chie[lam]Sinh[chie[lam]kD]+
		er chi[lam]Cosh[chie[lam]kD];



rauhkern[lam_]:=(1-er chi[lam]/nenn[lam,chi[#]&])/(lam^2-er);
kern[lam_]:=rauhkern[lam]lam;
kernlinks[lam_]:=(1+er chi[lam]/nenn[lam,-chi[#]&])lam/(lam^2-er);





logkern[lam_]:=kern[lam]-(1-E^(-Sign[Re[lam]]lam kD))/lam



kernpur[lam_]:=er lam Cosh[chie[lam]kD]/nenn[lam,chi[#]&] ;
kernpurlinks[lam_]:=
	er lam Cosh[chie[lam] kD]/nenn[lam,-chi[#]&];
kernpurkorr[lam_]:=kernpur[lam]- Sign[Re[lam]] er/(1+er) (*
er lam Cosh[chi[lam]kD]/(Sinh[chi[lam]kD]+er Cosh[chi[lam]kD]) /chi[lam] *);



POLrule:=FindRoot[nenn[lpsw,chi[#]&]==0,{lpsw,1.2}]

residuum[kern_]:=-2 Pi I kern[lpsw] *
    nenn[lpsw,chi[#]&]/Dt[nenn[lpsw,chi[#]&],lpsw]

SFW[kr_,kern_]:=residuum[kern] * Hankel02[lpsw kr]/.POLrule
	

























wegint[kern_,func_,lam_,t_,tanf_,tend_,krho_]:=
	 NIntegrate[kern[lam]func[krho lam] Dt[lam,t]//Evaluate,{t,tanf,tend}]









SFI1[krho_,ker_]:=
  wegint[ker,Hankel02,(1+I)t,t,If[krho>0.1,-7/krho,-70],0,krho]+
wegint[ker,Hankel02,(1-E^(-I fi)),fi,0,Pi,krho]+
wegint[ker,Hankel02,2+(1-I)t,t,0,If[krho>0.1,7/krho,70],krho]

SFIgen[krho_,ker_,fun_,tend_]:=wegint[ker,fun,(1+I)t,t,-tend,0,krho]+
wegint[ker,fun,(1-E^(-I fi)),fi,0,Pi,krho]+
wegint[ker,fun,2+(1-I)t,t,0,tend,krho]



SFIcirc[krho_,ker_]:=
  2 (wegint[ker,BesselJ[0,#]&,(1-E^(-I fi)),fi,0,Pi,krho]+
        wegint[ker,BesselJ[0,#]&,2+t,t,0,If[krho>0.1,200/krho,2000],krho])





lague[n_,a_]:=LegendreP[n,u]u^(n+1) /a/.u->a/Sqrt[1+a^2]

legen[l_,a_]:=(-1)^l (a/2)^(2l)*Gamma[l+1/2]/Gamma[l+1]/Gamma[2l+3/2]*
    HypergeometricPFQ[{l+1/2},{l+1,2l+3/2},-(a/2)^2]

cheby[l_,a_]:= Pi BesselJ[-l,a/2] BesselJ[l,a/2];
chebycomp[l_,a_]:=(-1)^l Pi BesselJ[l, a/2]*(BesselJ[l,a/2]-I BesselY[l,a/2])

coeffint[f_,krho_]:= 
  NIntegrate[Evaluate[f[krho,x]Dt[x,t]/.x->-2+(1+I)t],{t,-7/krho,0}](* +
	NIntegrate[Evaluate[f[krho,x]Dt[x,t]/.
				x->1-E^(-I t)],{t,0,Pi}] *)+ 
NIntegrate[Evaluate[f[krho ,x]Dt[x,t]/.
				x->2+(1-I)t],{t,0,7/krho}]



End[]
EndPackage[]