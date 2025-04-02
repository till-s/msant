(*^

::[	Information =

	"This is a Mathematica Notebook file.  It contains ASCII text, and can be
	transferred by email, ftp, or other text-file transfer utility.  It should
	be read or edited using a copy of Mathematica or MathReader.  If you 
	received this as email, use your mail application or copy/paste to save 
	everything from the line containing (*^ down to the line containing ^*)
	into a plain text file.  On some systems you may have to give the file a 
	name ending with ".ma" to allow Mathematica to recognize it as a Notebook.
	The line below identifies what version of Mathematica created this file,
	but it can be opened using any other version as well.";

	FrontEndVersion = "X Window System Mathematica Notebook Front End Version 2.2";

	X11StandardFontEncoding; 
	
	fontset = title, inactive, noPageBreakBelow, noPageBreakInGroup, nohscroll, preserveAspect, groupLikeTitle, center, M7, bold, e8,  24, fontName, "times";
	fontset = subtitle, inactive, noPageBreakBelow, noPageBreakInGroup, nohscroll, preserveAspect, groupLikeTitle, center, M7, bold, e6,  18, fontName, "times";
	fontset = subsubtitle, inactive, noPageBreakBelow, noPageBreakInGroup, nohscroll, preserveAspect, groupLikeTitle, center, M7, italic, e6,  14, fontName, "times";
	fontset = section, inactive, noPageBreakBelow, nohscroll, preserveAspect, groupLikeSection, grayBox, M22, bold, a20,  18, fontName, "times";
	fontset = subsection, inactive, noPageBreakBelow, nohscroll, preserveAspect, groupLikeSection, blackBox, M19, bold, a15,  14, fontName, "times";
	fontset = subsubsection, inactive, noPageBreakBelow, nohscroll, preserveAspect, groupLikeSection, whiteBox, M18, bold, a12,  12, fontName, "times";
	fontset = text, inactive, nohscroll, noKeepOnOnePage, preserveAspect, M7,  12, fontName, "times";
	fontset = smalltext, inactive, nohscroll, noKeepOnOnePage, preserveAspect, M7,  10, fontName, "times";
	fontset = input, noPageBreakInGroup, nowordwrap, preserveAspect, groupLikeInput, M42, N23, bold,  12, fontName, "courier";
	fontset = output, output, inactive, noPageBreakInGroup, nowordwrap, preserveAspect, groupLikeOutput, M42, N23, L-5,  12, fontName, "courier";
	fontset = message, inactive, noPageBreakInGroup, nowordwrap, preserveAspect, groupLikeOutput, M42, N23,  12, fontName, "courier";
	fontset = print, inactive, noPageBreakInGroup, nowordwrap, preserveAspect, groupLikeOutput, M42, N23,  12, fontName, "courier";
	fontset = info, inactive, noPageBreakInGroup, nowordwrap, preserveAspect, groupLikeOutput, M42, N23,  12, fontName, "courier";
	fontset = postscript, PostScript, formatAsPostScript, output, inactive, noPageBreakInGroup, nowordwrap, preserveAspect, groupLikeGraphics, M7, l34, w282, h287,  12, fontName, "courier";
	fontset = name, inactive, noPageBreakInGroup, nohscroll, preserveAspect, M7, italic, B65535,  10, fontName, "times";
	fontset = header, inactive, nohscroll, noKeepOnOnePage, preserveAspect, M7, italic,  12, fontName, "times";
	fontset = leftheader,  12, fontName, "times";
	fontset = footer, inactive, nohscroll, noKeepOnOnePage, preserveAspect, center, M7, italic,  12, fontName, "times";
	fontset = leftfooter,  12, fontName, "times";
	fontset = help, inactive, nohscroll, noKeepOnOnePage, preserveAspect, M7,  12, fontName, "times";
	fontset = clipboard, inactive, nohscroll, noKeepOnOnePage, preserveAspect, M7,  12, fontName, "times";
	fontset = completions, inactive, nohscroll, noKeepOnOnePage, preserveAspect, M7,  12, fontName, "courier";
	fontset = special1, inactive, nohscroll, noKeepOnOnePage, preserveAspect, M7,  12, fontName, "times";
	fontset = special2, inactive, nohscroll, noKeepOnOnePage, preserveAspect, M7,  12, fontName, "times";
	fontset = special3, inactive, nohscroll, noKeepOnOnePage, preserveAspect, M7,  12, fontName, "times";
	fontset = special4, inactive, nohscroll, noKeepOnOnePage, preserveAspect, M7,  12, fontName, "times";
	fontset = special5, inactive, nohscroll, noKeepOnOnePage, preserveAspect, M7,  12, fontName, "times";paletteColors = 128; automaticGrouping; currentKernel; 
]
:[font = title; inactive; preserveAspect; startGroup]
Sommerfeldintegrale durch FFT
:[font = text; inactive; preserveAspect]
Die Idee besteht in der Umwandlung des Sommerfeldintegrals

 S0= kr Int[ BesselJ[0,kr x] f[x], x]
 
in ein Fourier-Integral, sodass mittels FFT die Werte bei verschiedenen kr berechnet werden können. Dies gelingt durch Umwandlung   in eine logarithmische Skala:
w=ln(kr), u=ln(x)

S0= Int[ e^w BesselJ[0, exp(w + u)] f[u] e^u, {u,-Infinity,Infinity}]

    = Int[ exp(w+u) BesselJ[0, exp(w+u)] f[u], {u,-Infinity,Infinity}]
    
Dies sieht aus wie ein Faltungsintegral ausgewertet an der Stelle w!

      exp(u) BesselJ[0, exp(u)] * f[u]  

:[font = section; inactive; preserveAspect; startGroup]
Die Fouriertrafo des Besselkerns
:[font = text; inactive; preserveAspect]
Die Funktion exp(u) BesselJ[0,exp(u)] muss der Fouriertrafo unterzogen werden.
:[font = input; preserveAspect; startGroup]
Integrate[ E^u BesselJ[0,E^u] E^(I v u),{u,-Infinity,Infinity}]
:[font = output; output; inactive; preserveAspect; endGroup]
Integrate[E^(u + I*u*v)*BesselJ[0, E^u], 
 
  {u, DirectedInfinity[-1], DirectedInfinity[1]}]
;[o]
           u + I u v             u
Integrate[E          BesselJ[0, E ], 
 
  {u, -Infinity, Infinity}]
:[font = text; inactive; preserveAspect]
Was Mathematica nicht kann, kann ich doch! (Substitution e^u=z  dz=z du)
:[font = input; preserveAspect; startGroup]
Integrate[ z BesselJ[0,z] z^(I v) 1/z, {z,0,Infinity}]
:[font = output; output; inactive; preserveAspect; endGroup]
((1/4)^(-I/2*v)*Gamma[(1 + I*v)/2])/
 
  Gamma[(1 - I*v)/2]
;[o]
 1 -I/2 v       1 + I v
(-)       Gamma[-------]
 4                 2
------------------------
           1 - I v
     Gamma[-------]
              2
:[font = input; preserveAspect]
besselTrans[v_]:=Evaluate[%]
:[font = input; preserveAspect; startGroup]
Plot[ {Re[besselTrans[x]],Im[besselTrans[x]]},
   {x,-10,10}]
:[font = postscript; PostScript; formatAsPostScript; output; inactive; preserveAspect; pictureLeft = 34; pictureWidth = 282; pictureHeight = 174.25]
%!
%%Creator: Mathematica
%%AspectRatio: .61803 
MathPictureStart
%% Graphics
/Courier findfont 10  scalefont  setfont
% Scaling calculations
0.5 0.047619 0.309017 0.294302 [
[(-10)] .02381 .30902 0 2 Msboxa
[(-5)] .2619 .30902 0 2 Msboxa
[(5)] .7381 .30902 0 2 Msboxa
[(10)] .97619 .30902 0 2 Msboxa
[(-1)] .4875 .01471 1 0 Msboxa
[(-0.5)] .4875 .16187 1 0 Msboxa
[(0.5)] .4875 .45617 1 0 Msboxa
[(1)] .4875 .60332 1 0 Msboxa
[ -0.001 -0.001 0 0 ]
[ 1.001 .61903 0 0 ]
] MathScale
% Start of Graphics
1 setlinecap
1 setlinejoin
newpath
[ ] 0 setdash
0 g
p
p
.002 w
.02381 .30902 m
.02381 .31527 L
s
P
[(-10)] .02381 .30902 0 2 Mshowa
p
.002 w
.2619 .30902 m
.2619 .31527 L
s
P
[(-5)] .2619 .30902 0 2 Mshowa
p
.002 w
.7381 .30902 m
.7381 .31527 L
s
P
[(5)] .7381 .30902 0 2 Mshowa
p
.002 w
.97619 .30902 m
.97619 .31527 L
s
P
[(10)] .97619 .30902 0 2 Mshowa
p
.001 w
.07143 .30902 m
.07143 .31277 L
s
P
p
.001 w
.11905 .30902 m
.11905 .31277 L
s
P
p
.001 w
.16667 .30902 m
.16667 .31277 L
s
P
p
.001 w
.21429 .30902 m
.21429 .31277 L
s
P
p
.001 w
.30952 .30902 m
.30952 .31277 L
s
P
p
.001 w
.35714 .30902 m
.35714 .31277 L
s
P
p
.001 w
.40476 .30902 m
.40476 .31277 L
s
P
p
.001 w
.45238 .30902 m
.45238 .31277 L
s
P
p
.001 w
.54762 .30902 m
.54762 .31277 L
s
P
p
.001 w
.59524 .30902 m
.59524 .31277 L
s
P
p
.001 w
.64286 .30902 m
.64286 .31277 L
s
P
p
.001 w
.69048 .30902 m
.69048 .31277 L
s
P
p
.001 w
.78571 .30902 m
.78571 .31277 L
s
P
p
.001 w
.83333 .30902 m
.83333 .31277 L
s
P
p
.001 w
.88095 .30902 m
.88095 .31277 L
s
P
p
.001 w
.92857 .30902 m
.92857 .31277 L
s
P
p
.002 w
0 .30902 m
1 .30902 L
s
P
p
.002 w
.5 .01471 m
.50625 .01471 L
s
P
[(-1)] .4875 .01471 1 0 Mshowa
p
.002 w
.5 .16187 m
.50625 .16187 L
s
P
[(-0.5)] .4875 .16187 1 0 Mshowa
p
.002 w
.5 .45617 m
.50625 .45617 L
s
P
[(0.5)] .4875 .45617 1 0 Mshowa
p
.002 w
.5 .60332 m
.50625 .60332 L
s
P
[(1)] .4875 .60332 1 0 Mshowa
p
.001 w
.5 .04415 m
.50375 .04415 L
s
P
p
.001 w
.5 .07358 m
.50375 .07358 L
s
P
p
.001 w
.5 .10301 m
.50375 .10301 L
s
P
p
.001 w
.5 .13244 m
.50375 .13244 L
s
P
p
.001 w
.5 .1913 m
.50375 .1913 L
s
P
p
.001 w
.5 .22073 m
.50375 .22073 L
s
P
p
.001 w
.5 .25016 m
.50375 .25016 L
s
P
p
.001 w
.5 .27959 m
.50375 .27959 L
s
P
p
.001 w
.5 .33845 m
.50375 .33845 L
s
P
p
.001 w
.5 .36788 m
.50375 .36788 L
s
P
p
.001 w
.5 .39731 m
.50375 .39731 L
s
P
p
.001 w
.5 .42674 m
.50375 .42674 L
s
P
p
.001 w
.5 .4856 m
.50375 .4856 L
s
P
p
.001 w
.5 .51503 m
.50375 .51503 L
s
P
p
.001 w
.5 .54446 m
.50375 .54446 L
s
P
p
.001 w
.5 .57389 m
.50375 .57389 L
s
P
p
.002 w
.5 0 m
.5 .61803 L
s
P
P
0 0 m
1 0 L
1 .61803 L
0 .61803 L
closepath
clip
newpath
p
p
p
.004 w
.02381 .57058 m
.02629 .58482 L
.02753 .59046 L
.02877 .59509 L
.03001 .5987 L
.03125 .60128 L
.03249 .60282 L
.03373 .60332 L
.03497 .60278 L
.03621 .60122 L
.03745 .59863 L
.03869 .59503 L
.03993 .59044 L
.04117 .58487 L
.04365 .57091 L
.04861 .53252 L
.05357 .48215 L
.06349 .35748 L
.07341 .22392 L
.08333 .109 L
.08829 .06586 L
.09077 .04885 L
.09325 .03513 L
.09449 .02955 L
.09573 .02485 L
.09697 .02103 L
.09821 .01811 L
.09945 .01609 L
.10069 .01497 L
.10193 .01474 L
.10317 .01542 L
.10441 .01699 L
.10565 .01943 L
.10689 .02275 L
.10813 .02693 L
.1131 .05184 L
.11558 .06891 L
.11806 .08874 L
.12302 .13567 L
.14286 .37321 L
.15278 .48385 L
.15774 .52855 L
.16022 .54743 L
.1627 .56376 L
.16518 .57739 L
.16766 .58822 L
.1689 .59256 L
.17014 .59617 L
.17138 .59905 L
.17262 .60119 L
Mistroke
.17386 .60259 L
.1751 .60326 L
.17634 .60319 L
.17758 .6024 L
.17882 .60088 L
.18006 .59865 L
.18254 .59208 L
.18502 .5828 L
.1875 .57092 L
.19246 .53999 L
.20238 .45476 L
.22222 .24211 L
.23214 .14481 L
.2371 .10408 L
.24206 .0702 L
.24702 .04398 L
.2495 .03393 L
.25198 .02597 L
.25446 .02013 L
.2557 .018 L
.25694 .01641 L
.25818 .01534 L
.25942 .01479 L
.26066 .01477 L
.2619 .01526 L
.26314 .01626 L
.26438 .01776 L
.26687 .02224 L
.26811 .0252 L
.26935 .02862 L
.27183 .03683 L
.27679 .05834 L
.28175 .08589 L
.30159 .23636 L
.32143 .4013 L
.33135 .47168 L
.34127 .5281 L
.34623 .55039 L
.35119 .56861 L
.35615 .58279 L
.36111 .59308 L
.36359 .59683 L
.36483 .59837 L
.36607 .59969 L
.36731 .60079 L
.36855 .60169 L
.36979 .60239 L
.37103 .60289 L
.37227 .6032 L
.37351 .60332 L
Mistroke
.37475 .60326 L
.37599 .60302 L
.37723 .60262 L
.37847 .60206 L
.38095 .60047 L
.38343 .5983 L
.38591 .59562 L
.39087 .58891 L
.40079 .57162 L
.42063 .53332 L
.4256 .52526 L
.43056 .51853 L
.43552 .51345 L
.438 .51162 L
.43924 .5109 L
.44048 .51032 L
.44172 .50987 L
.44296 .50957 L
.4442 .50942 L
.44544 .50941 L
.44668 .50956 L
.44792 .50987 L
.44916 .51033 L
.4504 .51096 L
.45164 .51175 L
.45288 .5127 L
.45536 .5151 L
.46032 .5219 L
.46528 .53124 L
.47024 .54279 L
.48016 .56964 L
.48512 .58268 L
.49008 .59354 L
.49256 .59769 L
.49504 .60078 L
.49628 .60188 L
.49752 .60268 L
.49876 .60316 L
.5 .60332 L
.50124 .60316 L
.50248 .60268 L
.50372 .60188 L
.50496 .60078 L
.50744 .59769 L
.50992 .59354 L
.51488 .58268 L
.51984 .56964 L
.52976 .54279 L
.53472 .53124 L
.53968 .5219 L
Mistroke
.54464 .5151 L
.54712 .5127 L
.54836 .51175 L
.5496 .51096 L
.55084 .51033 L
.55208 .50987 L
.55332 .50956 L
.55456 .50941 L
.5558 .50942 L
.55704 .50957 L
.55828 .50987 L
.55952 .51032 L
.562 .51162 L
.56448 .51345 L
.56944 .51853 L
.5744 .52526 L
.57937 .53332 L
.59921 .57162 L
.60913 .58891 L
.61409 .59562 L
.61657 .5983 L
.61905 .60047 L
.62029 .60134 L
.62153 .60206 L
.62277 .60262 L
.62401 .60302 L
.62525 .60326 L
.62649 .60332 L
.62773 .6032 L
.62897 .60289 L
.63021 .60239 L
.63145 .60169 L
.63269 .60079 L
.63393 .59969 L
.63889 .59308 L
.64137 .58841 L
.64385 .58279 L
.64881 .56861 L
.65377 .55039 L
.65873 .5281 L
.67857 .4013 L
.69841 .23636 L
.70833 .15522 L
.71825 .08589 L
.72321 .05834 L
.72569 .04677 L
.72817 .03683 L
.73065 .02862 L
.73189 .0252 L
.73313 .02224 L
Mistroke
.73438 .01975 L
.73562 .01776 L
.73686 .01626 L
.7381 .01526 L
.73934 .01477 L
.74058 .01479 L
.74182 .01534 L
.74306 .01641 L
.7443 .018 L
.74554 .02013 L
.74802 .02597 L
.7505 .03393 L
.75298 .04398 L
.75794 .0702 L
.7629 .10408 L
.76786 .14481 L
.77778 .24211 L
.7877 .3503 L
.79762 .45476 L
.80258 .50072 L
.80754 .53999 L
.81002 .5566 L
.8125 .57092 L
.81498 .5828 L
.81746 .59208 L
.8187 .59571 L
.81994 .59865 L
.82118 .60088 L
.82242 .6024 L
.82366 .60319 L
.8249 .60326 L
.82614 .60259 L
.82738 .60119 L
.82862 .59905 L
.82986 .59617 L
.8311 .59256 L
.83234 .58822 L
.8373 .56376 L
.83978 .54743 L
.84226 .52855 L
.84722 .48385 L
.85714 .37321 L
.86706 .24993 L
.87698 .13567 L
.88194 .08874 L
.88442 .06891 L
.8869 .05184 L
.88938 .03779 L
.89063 .03195 L
.89187 .02693 L
Mistroke
.89311 .02275 L
.89435 .01943 L
.89559 .01699 L
.89683 .01542 L
.89807 .01474 L
.89931 .01497 L
.90055 .01609 L
.90179 .01811 L
.90427 .02485 L
.90551 .02955 L
.90675 .03513 L
.91171 .06586 L
.91667 .109 L
.92659 .22392 L
.93651 .35748 L
.94643 .48215 L
.95139 .53252 L
.95635 .57091 L
.95883 .58487 L
.96007 .59044 L
.96131 .59503 L
.96255 .59863 L
.96379 .60122 L
.96503 .60278 L
.96627 .60332 L
.96751 .60282 L
.96875 .60128 L
.96999 .5987 L
.97123 .59509 L
.97247 .59046 L
.97371 .58482 L
.97619 .57058 L
Mfstroke
P
P
p
p
.004 w
.02381 .17411 m
.03373 .3093 L
.04365 .44327 L
.04861 .50048 L
.05357 .54701 L
.05605 .56549 L
.05853 .58048 L
.06101 .59179 L
.06225 .59602 L
.06349 .5993 L
.10317 .28867 L
.1131 .16592 L
.11806 .11385 L
.12302 .07118 L
.1255 .05397 L
.12798 .03973 L
.13046 .02861 L
.1317 .02425 L
.13294 .02071 L
.13418 .01798 L
.13542 .01607 L
.13666 .01499 L
.1379 .01473 L
.13914 .01529 L
.14038 .01666 L
.14162 .01883 L
.14286 .0218 L
.14534 .03007 L
.14782 .04135 L
.15278 .07227 L
.15774 .11301 L
.1627 .16164 L
.18254 .38957 L
.19246 .4914 L
.19742 .53232 L
.20238 .5647 L
.20486 .57737 L
.20734 .58758 L
.20982 .59527 L
.21106 .59816 L
.2123 .60041 L
.21354 .60202 L
.21478 .60299 L
.21602 .60332 L
.21726 .60302 L
.2185 .60209 L
.21974 .60054 L
.22098 .59838 L
.22222 .59561 L
.2247 .58831 L
Mistroke
.22718 .57874 L
.23214 .55325 L
.24206 .481 L
.2619 .29115 L
.27183 .19709 L
.28175 .1171 L
.28671 .08467 L
.29167 .0581 L
.29663 .03776 L
.29911 .02999 L
.30159 .02382 L
.30407 .01926 L
.30531 .01757 L
.30655 .01628 L
.30779 .01538 L
.30903 .01486 L
.31027 .01472 L
.31151 .01495 L
.31275 .01556 L
.31399 .01653 L
.31647 .01953 L
.31771 .02154 L
.31895 .02389 L
.32143 .02956 L
.32639 .0445 L
.33135 .06375 L
.34127 .11251 L
.36111 .23207 L
.38095 .3499 L
.39087 .39997 L
.40079 .44187 L
.41071 .47505 L
.42063 .49955 L
.4256 .50865 L
.43056 .5157 L
.43304 .51847 L
.43552 .52073 L
.43676 .52167 L
.438 .52248 L
.43924 .52316 L
.44048 .52371 L
.44172 .52412 L
.44296 .5244 L
.4442 .52455 L
.44544 .52455 L
.44668 .52441 L
.44792 .52413 L
.44916 .52369 L
.4504 .52311 L
.45164 .52236 L
Mistroke
.45288 .52145 L
.45536 .51912 L
.45784 .51606 L
.46032 .51223 L
.46528 .50197 L
.47024 .48781 L
.4752 .4692 L
.48016 .44573 L
.49008 .38424 L
.5 .30902 L
.50992 .23379 L
.51488 .20075 L
.51984 .17231 L
.5248 .14884 L
.52976 .13023 L
.53472 .11606 L
.5372 .11048 L
.53968 .10581 L
.54216 .10197 L
.54464 .09892 L
.54712 .09658 L
.54836 .09567 L
.5496 .09493 L
.55084 .09434 L
.55208 .09391 L
.55332 .09362 L
.55456 .09348 L
.5558 .09349 L
.55704 .09363 L
.55828 .09391 L
.55952 .09433 L
.562 .09556 L
.56324 .09637 L
.56448 .0973 L
.56944 .10233 L
.5744 .10938 L
.57937 .11848 L
.58929 .14298 L
.59921 .17616 L
.61905 .26813 L
.63889 .38596 L
.64881 .44768 L
.65873 .50553 L
.66865 .55428 L
.67361 .57353 L
.67609 .58158 L
.67857 .58848 L
.68105 .59414 L
.68353 .59851 L
.68477 .60018 L
Mistroke
.68601 .6015 L
.68725 .60247 L
.68849 .60308 L
.68973 .60332 L
.69097 .60318 L
.69221 .60266 L
.69345 .60175 L
.69469 .60046 L
.69593 .59877 L
.69841 .59421 L
.70089 .58805 L
.70337 .58027 L
.70833 .55993 L
.71329 .53336 L
.71825 .50093 L
.7381 .32688 L
.75794 .13703 L
.7629 .09779 L
.76786 .06478 L
.77282 .03929 L
.7753 .02972 L
.77654 .02578 L
.77778 .02242 L
.77902 .01966 L
.78026 .01749 L
.7815 .01594 L
.78274 .01502 L
.78398 .01472 L
.78522 .01505 L
.78646 .01602 L
.7877 .01762 L
.78894 .01987 L
.79018 .02276 L
.79266 .03045 L
.79514 .04066 L
.79762 .05334 L
.80258 .08571 L
.80754 .12663 L
.81746 .22846 L
.8373 .45639 L
.84226 .50502 L
.84722 .54576 L
.85218 .57668 L
.85466 .58796 L
.8559 .59248 L
.85714 .59623 L
.85838 .5992 L
.85962 .60138 L
.86086 .60275 L
.8621 .60331 L
Mistroke
.86334 .60304 L
.86458 .60196 L
.86582 .60005 L
.86706 .59733 L
.8683 .59378 L
.86954 .58942 L
.87202 .5783 L
.87698 .54685 L
.88194 .50419 L
.8869 .45211 L
.89683 .32936 L
.90675 .20132 L
.91171 .14322 L
.91667 .09313 L
.91915 .07195 L
.92163 .05375 L
.92411 .03879 L
.92659 .02729 L
.92783 .02289 L
.92907 .01942 L
.93031 .0169 L
.93155 .01533 L
.93279 .01472 L
.93403 .01509 L
.93527 .01642 L
.93651 .01873 L
.93775 .02201 L
.93899 .02625 L
.94147 .03756 L
.94395 .05254 L
.94643 .07103 L
.95635 .17476 L
.96627 .30873 L
.97619 .44392 L
Mfstroke
P
P
P
% End of Graphics
MathPictureEnd

:[font = output; output; inactive; preserveAspect; endGroup]
Graphics["<<>>"]
;[o]
-Graphics-
:[font = text; inactive; preserveAspect; endGroup]
Das ist zwar eine etwas wüste Funktion, sie oszilliert wie E^(I x ln(x)), also mit stetig steigender Frequenz! Wir hoffen, dass die Verhältnisse gemildert werden durch eine 'brave' Funktion F[ y ] *-o f[u], welche mit ihr multipliziert wird. Dieses Produkt, d.h. die gesuchte Faltung im Frequenzbereich ist dann wieder ('frequenz-') begrenzt.
:[font = section; inactive; preserveAspect; startGroup]
Definition des Integranden
:[font = text; inactive; preserveAspect]
Der (bereits oft verwendete) Integrand lautet:
:[font = input; initialization; preserveAspect]
*)
Kern[l_, er_, kD_]:= (1- er u[l]/NN[l,er,kD]) l/(l^2-er)
u[l_]             := Sqrt[l^2-1]
ue[l_,er_]        := Sqrt[l^2-er]
NN[l_,er_,kD_]    := ue[l,er] Sinh[ue[l,er] kD] +
                     er u[l]  Cosh[ue[l,er] kD]
(*
:[font = text; inactive; preserveAspect]
Ausserdem müssen wir das Residuum und den Pol auf der reellen Achse berechnen! Letzteres  geht nur numerisch, da die transzendente Gleichung für die Polstelle nicht analytisch gelöst werden kann.
:[font = input; initialization; preserveAspect]
*)
res[l_,er_,kD_]   := - er u[lpsw]/(lpsw^2-er) /
                     D[NN[lpsw,er,kD],lpsw]/.lpsw->l
pol[er_,kD_]      := lp/.FindRoot[NN[lp,er,kD]==0,{lp,1.2}]
resnum[er_,kD_]   := res[pol[er,kD],er,kD]
(*
:[font = text; inactive; preserveAspect]
Hier noch die Entwicklung des Kerns für geringe u (l nahe bei 1)
:[font = input; preserveAspect]
NNapprox[uu_,er_,kD_]:= kD (uu^2+1-er) + 
                       er uu (1+ (uu^2+1-er)/2 kD^2)
:[font = input; preserveAspect]
er:=10;kD:=0.1
:[font = input; preserveAspect]
dewe:=0.1;delta:=0.1
:[font = input; preserveAspect; startGroup]
orig=RotateLeft[
        Table[
            Evaluate[
            N[Kern[E^(x+delta I),er,kD]\
                      /.x->2 Pi/201/dewe i
             ]
            ],
          {i,-100,100}],
        100
     ];
:[font = message; inactive; preserveAspect]
General::ovfl: Overflow occurred in computation.
:[font = message; inactive; preserveAspect]
General::ovfl: Overflow occurred in computation.
:[font = message; inactive; preserveAspect]
General::ovfl: Overflow occurred in computation.
:[font = message; inactive; preserveAspect; endGroup]
General::stop: 
   Further output of General::ovfl
     will be suppressed during this calculation.
:[font = input; preserveAspect; startGroup]
ListPlot[Re[orig],PlotRange->All]
:[font = postscript; PostScript; formatAsPostScript; output; inactive; preserveAspect; pictureLeft = 34; pictureWidth = 282; pictureHeight = 174.25]
%!
%%Creator: Mathematica
%%AspectRatio: .61803 
MathPictureStart
%% Graphics
/Courier findfont 10  scalefont  setfont
% Scaling calculations
0.0190476 0.0047619 0.0147151 13.9167 [
[(50)] .25714 .01472 0 2 Msboxa
[(100)] .49524 .01472 0 2 Msboxa
[(150)] .73333 .01472 0 2 Msboxa
[(200)] .97143 .01472 0 2 Msboxa
[(0.01)] .00655 .15388 1 0 Msboxa
[(0.02)] .00655 .29305 1 0 Msboxa
[(0.03)] .00655 .43221 1 0 Msboxa
[(0.04)] .00655 .57138 1 0 Msboxa
[ -0.001 -0.001 0 0 ]
[ 1.001 .61903 0 0 ]
] MathScale
% Start of Graphics
1 setlinecap
1 setlinejoin
newpath
[ ] 0 setdash
0 g
p
p
.002 w
.25714 .01472 m
.25714 .02097 L
s
P
[(50)] .25714 .01472 0 2 Mshowa
p
.002 w
.49524 .01472 m
.49524 .02097 L
s
P
[(100)] .49524 .01472 0 2 Mshowa
p
.002 w
.73333 .01472 m
.73333 .02097 L
s
P
[(150)] .73333 .01472 0 2 Mshowa
p
.002 w
.97143 .01472 m
.97143 .02097 L
s
P
[(200)] .97143 .01472 0 2 Mshowa
p
.001 w
.06667 .01472 m
.06667 .01847 L
s
P
p
.001 w
.11429 .01472 m
.11429 .01847 L
s
P
p
.001 w
.1619 .01472 m
.1619 .01847 L
s
P
p
.001 w
.20952 .01472 m
.20952 .01847 L
s
P
p
.001 w
.30476 .01472 m
.30476 .01847 L
s
P
p
.001 w
.35238 .01472 m
.35238 .01847 L
s
P
p
.001 w
.4 .01472 m
.4 .01847 L
s
P
p
.001 w
.44762 .01472 m
.44762 .01847 L
s
P
p
.001 w
.54286 .01472 m
.54286 .01847 L
s
P
p
.001 w
.59048 .01472 m
.59048 .01847 L
s
P
p
.001 w
.6381 .01472 m
.6381 .01847 L
s
P
p
.001 w
.68571 .01472 m
.68571 .01847 L
s
P
p
.001 w
.78095 .01472 m
.78095 .01847 L
s
P
p
.001 w
.82857 .01472 m
.82857 .01847 L
s
P
p
.001 w
.87619 .01472 m
.87619 .01847 L
s
P
p
.001 w
.92381 .01472 m
.92381 .01847 L
s
P
p
.002 w
0 .01472 m
1 .01472 L
s
P
p
.002 w
.01905 .15388 m
.0253 .15388 L
s
P
[(0.01)] .00655 .15388 1 0 Mshowa
p
.002 w
.01905 .29305 m
.0253 .29305 L
s
P
[(0.02)] .00655 .29305 1 0 Mshowa
p
.002 w
.01905 .43221 m
.0253 .43221 L
s
P
[(0.03)] .00655 .43221 1 0 Mshowa
p
.002 w
.01905 .57138 m
.0253 .57138 L
s
P
[(0.04)] .00655 .57138 1 0 Mshowa
p
.001 w
.01905 .04255 m
.0228 .04255 L
s
P
p
.001 w
.01905 .07038 m
.0228 .07038 L
s
P
p
.001 w
.01905 .09822 m
.0228 .09822 L
s
P
p
.001 w
.01905 .12605 m
.0228 .12605 L
s
P
p
.001 w
.01905 .18172 m
.0228 .18172 L
s
P
p
.001 w
.01905 .20955 m
.0228 .20955 L
s
P
p
.001 w
.01905 .23738 m
.0228 .23738 L
s
P
p
.001 w
.01905 .26522 m
.0228 .26522 L
s
P
p
.001 w
.01905 .32088 m
.0228 .32088 L
s
P
p
.001 w
.01905 .34871 m
.0228 .34871 L
s
P
p
.001 w
.01905 .37655 m
.0228 .37655 L
s
P
p
.001 w
.01905 .40438 m
.0228 .40438 L
s
P
p
.001 w
.01905 .46005 m
.0228 .46005 L
s
P
p
.001 w
.01905 .48788 m
.0228 .48788 L
s
P
p
.001 w
.01905 .51571 m
.0228 .51571 L
s
P
p
.001 w
.01905 .54355 m
.0228 .54355 L
s
P
p
.001 w
.01905 .59921 m
.0228 .59921 L
s
P
p
.002 w
.01905 0 m
.01905 .61803 L
s
P
P
p
.008 w
.02381 .31938 Mdot
.02857 .34398 Mdot
.03333 .32812 Mdot
.0381 .35212 Mdot
.04286 .39648 Mdot
.04762 .45468 Mdot
.05238 .519 Mdot
.05714 .57533 Mdot
.0619 .60332 Mdot
.06667 .58553 Mdot
.07143 .52135 Mdot
.07619 .42939 Mdot
.08095 .33416 Mdot
.08571 .2523 Mdot
.09048 .18898 Mdot
.09524 .14216 Mdot
.1 .10791 Mdot
.10476 .08288 Mdot
.10952 .06458 Mdot
.11429 .05119 Mdot
.11905 .0414 Mdot
.12381 .03423 Mdot
.12857 .02899 Mdot
.13333 .02516 Mdot
.1381 .02236 Mdot
.14286 .0203 Mdot
.14762 .0188 Mdot
.15238 .01771 Mdot
.15714 .0169 Mdot
.1619 .01632 Mdot
.16667 .01589 Mdot
.17143 .01557 Mdot
.17619 .01534 Mdot
.18095 .01517 Mdot
.18571 .01505 Mdot
.19048 .01496 Mdot
.19524 .01489 Mdot
.2 .01485 Mdot
.20476 .01481 Mdot
.20952 .01479 Mdot
.21429 .01477 Mdot
.21905 .01475 Mdot
.22381 .01474 Mdot
.22857 .01474 Mdot
.23333 .01473 Mdot
.2381 .01473 Mdot
.24286 .01472 Mdot
.24762 .01472 Mdot
.25238 .01472 Mdot
.25714 .01472 Mdot
.2619 .01472 Mdot
.26667 .01472 Mdot
.27143 .01472 Mdot
.27619 .01472 Mdot
.28095 .01472 Mdot
.28571 .01472 Mdot
.29048 .01472 Mdot
.29524 .01472 Mdot
.3 .01472 Mdot
.30476 .01472 Mdot
.30952 .01472 Mdot
.31429 .01472 Mdot
.31905 .01472 Mdot
.32381 .01472 Mdot
.32857 .01472 Mdot
.33333 .01472 Mdot
.3381 .01472 Mdot
.34286 .01472 Mdot
.34762 .01472 Mdot
.35238 .01472 Mdot
.35714 .01472 Mdot
.3619 .01472 Mdot
.36667 .01472 Mdot
.37143 .01472 Mdot
.37619 .01472 Mdot
.38095 .01472 Mdot
.38571 .01472 Mdot
.39048 .01472 Mdot
.39524 .01472 Mdot
.4 .01472 Mdot
.40476 .01472 Mdot
.40952 .01472 Mdot
.41429 .01472 Mdot
.41905 .01472 Mdot
.42381 .01472 Mdot
.42857 .01472 Mdot
.43333 .01472 Mdot
.4381 .01472 Mdot
.44286 .01472 Mdot
.44762 .01472 Mdot
.45238 .01472 Mdot
.45714 .01472 Mdot
.4619 .01472 Mdot
.46667 .01472 Mdot
.47143 .01472 Mdot
.47619 .01472 Mdot
.48095 .01472 Mdot
.48571 .01472 Mdot
.49048 .01472 Mdot
.49524 .01472 Mdot
.5 .01472 Mdot
.50476 .01472 Mdot
.50952 .01472 Mdot
.51429 .01472 Mdot
.51905 .01472 Mdot
.52381 .01472 Mdot
.52857 .01472 Mdot
.53333 .01472 Mdot
.5381 .01472 Mdot
.54286 .01472 Mdot
.54762 .01472 Mdot
.55238 .01472 Mdot
.55714 .01472 Mdot
.5619 .01472 Mdot
.56667 .01472 Mdot
.57143 .01472 Mdot
.57619 .01472 Mdot
.58095 .01472 Mdot
.58571 .01472 Mdot
.59048 .01472 Mdot
.59524 .01472 Mdot
.6 .01472 Mdot
.60476 .01472 Mdot
.60952 .01472 Mdot
.61429 .01472 Mdot
.61905 .01472 Mdot
.62381 .01472 Mdot
.62857 .01472 Mdot
.63333 .01472 Mdot
.6381 .01472 Mdot
.64286 .01472 Mdot
.64762 .01472 Mdot
.65238 .01472 Mdot
.65714 .01472 Mdot
.6619 .01472 Mdot
.66667 .01472 Mdot
.67143 .01472 Mdot
.67619 .01472 Mdot
.68095 .01472 Mdot
.68571 .01472 Mdot
.69048 .01472 Mdot
.69524 .01472 Mdot
.7 .01472 Mdot
.70476 .01472 Mdot
.70952 .01472 Mdot
.71429 .01472 Mdot
.71905 .01472 Mdot
.72381 .01472 Mdot
.72857 .01472 Mdot
.73333 .01472 Mdot
.7381 .01472 Mdot
.74286 .01472 Mdot
.74762 .01472 Mdot
.75238 .01472 Mdot
.75714 .01472 Mdot
.7619 .01472 Mdot
.76667 .01472 Mdot
.77143 .01472 Mdot
.77619 .01472 Mdot
.78095 .01472 Mdot
.78571 .01472 Mdot
.79048 .01472 Mdot
.79524 .01472 Mdot
.8 .01472 Mdot
.80476 .01472 Mdot
.80952 .01472 Mdot
.81429 .01472 Mdot
.81905 .01472 Mdot
.82381 .01472 Mdot
.82857 .01472 Mdot
.83333 .01472 Mdot
.8381 .01472 Mdot
.84286 .01472 Mdot
.84762 .01473 Mdot
.85238 .01473 Mdot
.85714 .01474 Mdot
.8619 .01474 Mdot
.86667 .01475 Mdot
.87143 .01477 Mdot
.87619 .01479 Mdot
.88095 .01482 Mdot
.88571 .01485 Mdot
.89048 .0149 Mdot
.89524 .01497 Mdot
.9 .01507 Mdot
.90476 .0152 Mdot
.90952 .01538 Mdot
.91429 .01562 Mdot
.91905 .01595 Mdot
.92381 .0164 Mdot
.92857 .01702 Mdot
.93333 .01787 Mdot
.9381 .01902 Mdot
.94286 .02061 Mdot
.94762 .02277 Mdot
.95238 .02575 Mdot
.95714 .02983 Mdot
.9619 .03549 Mdot
.96667 .04346 Mdot
.97143 .05531 Mdot
.97619 .07831 Mdot
P
0 0 m
1 0 L
1 .61803 L
0 .61803 L
closepath
clip
newpath
% End of Graphics
MathPictureEnd

:[font = output; output; inactive; preserveAspect; endGroup]
Graphics["<<>>"]
;[o]
-Graphics-
:[font = input; preserveAspect]
Filter:=RotateLeft[
          Table[ N[
                   besselTrans[w] *
                   E^(-I w delta)/.w->dewe i
                ],
                {i,-100,100}
                ],
          100];
:[font = input; preserveAspect]
trafo=InverseFourier[orig];
:[font = input; preserveAspect; startGroup]
ListPlot[trafo//Re,PlotRange->All]
:[font = postscript; PostScript; formatAsPostScript; output; inactive; preserveAspect; pictureLeft = 34; pictureWidth = 282; pictureHeight = 174.25]
%!
%%Creator: Mathematica
%%AspectRatio: .61803 
MathPictureStart
%% Graphics
/Courier findfont 10  scalefont  setfont
% Scaling calculations
0.0190476 0.0047619 0.140959 13.6815 [
[(50)] .25714 .14096 0 2 Msboxa
[(100)] .49524 .14096 0 2 Msboxa
[(150)] .73333 .14096 0 2 Msboxa
[(200)] .97143 .14096 0 2 Msboxa
[(-0.01)] .00655 .00414 1 0 Msboxa
[(0.01)] .00655 .27777 1 0 Msboxa
[(0.02)] .00655 .41459 1 0 Msboxa
[(0.03)] .00655 .5514 1 0 Msboxa
[ -0.001 -0.001 0 0 ]
[ 1.001 .61903 0 0 ]
] MathScale
% Start of Graphics
1 setlinecap
1 setlinejoin
newpath
[ ] 0 setdash
0 g
p
p
.002 w
.25714 .14096 m
.25714 .14721 L
s
P
[(50)] .25714 .14096 0 2 Mshowa
p
.002 w
.49524 .14096 m
.49524 .14721 L
s
P
[(100)] .49524 .14096 0 2 Mshowa
p
.002 w
.73333 .14096 m
.73333 .14721 L
s
P
[(150)] .73333 .14096 0 2 Mshowa
p
.002 w
.97143 .14096 m
.97143 .14721 L
s
P
[(200)] .97143 .14096 0 2 Mshowa
p
.001 w
.06667 .14096 m
.06667 .14471 L
s
P
p
.001 w
.11429 .14096 m
.11429 .14471 L
s
P
p
.001 w
.1619 .14096 m
.1619 .14471 L
s
P
p
.001 w
.20952 .14096 m
.20952 .14471 L
s
P
p
.001 w
.30476 .14096 m
.30476 .14471 L
s
P
p
.001 w
.35238 .14096 m
.35238 .14471 L
s
P
p
.001 w
.4 .14096 m
.4 .14471 L
s
P
p
.001 w
.44762 .14096 m
.44762 .14471 L
s
P
p
.001 w
.54286 .14096 m
.54286 .14471 L
s
P
p
.001 w
.59048 .14096 m
.59048 .14471 L
s
P
p
.001 w
.6381 .14096 m
.6381 .14471 L
s
P
p
.001 w
.68571 .14096 m
.68571 .14471 L
s
P
p
.001 w
.78095 .14096 m
.78095 .14471 L
s
P
p
.001 w
.82857 .14096 m
.82857 .14471 L
s
P
p
.001 w
.87619 .14096 m
.87619 .14471 L
s
P
p
.001 w
.92381 .14096 m
.92381 .14471 L
s
P
p
.002 w
0 .14096 m
1 .14096 L
s
P
p
.002 w
.01905 .00414 m
.0253 .00414 L
s
P
[(-0.01)] .00655 .00414 1 0 Mshowa
p
.002 w
.01905 .27777 m
.0253 .27777 L
s
P
[(0.01)] .00655 .27777 1 0 Mshowa
p
.002 w
.01905 .41459 m
.0253 .41459 L
s
P
[(0.02)] .00655 .41459 1 0 Mshowa
p
.002 w
.01905 .5514 m
.0253 .5514 L
s
P
[(0.03)] .00655 .5514 1 0 Mshowa
p
.001 w
.01905 .03151 m
.0228 .03151 L
s
P
p
.001 w
.01905 .05887 m
.0228 .05887 L
s
P
p
.001 w
.01905 .08623 m
.0228 .08623 L
s
P
p
.001 w
.01905 .1136 m
.0228 .1136 L
s
P
p
.001 w
.01905 .16832 m
.0228 .16832 L
s
P
p
.001 w
.01905 .19568 m
.0228 .19568 L
s
P
p
.001 w
.01905 .22305 m
.0228 .22305 L
s
P
p
.001 w
.01905 .25041 m
.0228 .25041 L
s
P
p
.001 w
.01905 .30514 m
.0228 .30514 L
s
P
p
.001 w
.01905 .3325 m
.0228 .3325 L
s
P
p
.001 w
.01905 .35986 m
.0228 .35986 L
s
P
p
.001 w
.01905 .38723 m
.0228 .38723 L
s
P
p
.001 w
.01905 .44195 m
.0228 .44195 L
s
P
p
.001 w
.01905 .46931 m
.0228 .46931 L
s
P
p
.001 w
.01905 .49668 m
.0228 .49668 L
s
P
p
.001 w
.01905 .52404 m
.0228 .52404 L
s
P
p
.001 w
.01905 .57877 m
.0228 .57877 L
s
P
p
.001 w
.01905 .60613 m
.0228 .60613 L
s
P
p
.002 w
.01905 0 m
.01905 .61803 L
s
P
P
p
.008 w
.02381 .60332 Mdot
.02857 .58471 Mdot
.03333 .53458 Mdot
.0381 .46183 Mdot
.04286 .37786 Mdot
.04762 .29383 Mdot
.05238 .21857 Mdot
.05714 .15774 Mdot
.0619 .1138 Mdot
.06667 .08664 Mdot
.07143 .07438 Mdot
.07619 .07413 Mdot
.08095 .08264 Mdot
.08571 .09674 Mdot
.09048 .11361 Mdot
.09524 .13098 Mdot
.1 .14715 Mdot
.10476 .16097 Mdot
.10952 .17182 Mdot
.11429 .17951 Mdot
.11905 .18416 Mdot
.12381 .18614 Mdot
.12857 .18594 Mdot
.13333 .18412 Mdot
.1381 .18122 Mdot
.14286 .17776 Mdot
.14762 .17414 Mdot
.15238 .17069 Mdot
.15714 .16762 Mdot
.1619 .16506 Mdot
.16667 .16305 Mdot
.17143 .16159 Mdot
.17619 .16059 Mdot
.18095 .15999 Mdot
.18571 .15967 Mdot
.19048 .15955 Mdot
.19524 .15953 Mdot
.2 .15954 Mdot
.20476 .15954 Mdot
.20952 .15948 Mdot
.21429 .15937 Mdot
.21905 .15917 Mdot
.22381 .15892 Mdot
.22857 .15861 Mdot
.23333 .15826 Mdot
.2381 .15789 Mdot
.24286 .1575 Mdot
.24762 .15711 Mdot
.25238 .15674 Mdot
.25714 .15638 Mdot
.2619 .15603 Mdot
.26667 .1557 Mdot
.27143 .15539 Mdot
.27619 .1551 Mdot
.28095 .15482 Mdot
.28571 .15455 Mdot
.29048 .15429 Mdot
.29524 .15404 Mdot
.3 .1538 Mdot
.30476 .15357 Mdot
.30952 .15334 Mdot
.31429 .15312 Mdot
.31905 .1529 Mdot
.32381 .15268 Mdot
.32857 .15247 Mdot
.33333 .15227 Mdot
.3381 .15207 Mdot
.34286 .15187 Mdot
.34762 .15168 Mdot
.35238 .15149 Mdot
.35714 .1513 Mdot
.3619 .15112 Mdot
.36667 .15094 Mdot
.37143 .15077 Mdot
.37619 .1506 Mdot
.38095 .15043 Mdot
.38571 .15027 Mdot
.39048 .15011 Mdot
.39524 .14995 Mdot
.4 .1498 Mdot
.40476 .14965 Mdot
.40952 .14951 Mdot
.41429 .14936 Mdot
.41905 .14922 Mdot
.42381 .14908 Mdot
.42857 .14895 Mdot
.43333 .14882 Mdot
.4381 .14869 Mdot
.44286 .14856 Mdot
.44762 .14844 Mdot
.45238 .14831 Mdot
.45714 .14819 Mdot
.4619 .14807 Mdot
.46667 .14796 Mdot
.47143 .14785 Mdot
.47619 .14773 Mdot
.48095 .14763 Mdot
.48571 .14752 Mdot
.49048 .14741 Mdot
.49524 .14731 Mdot
.5 .14721 Mdot
.50476 .14711 Mdot
.50952 .14701 Mdot
.51429 .14692 Mdot
.51905 .14682 Mdot
.52381 .14673 Mdot
.52857 .14664 Mdot
.53333 .14655 Mdot
.5381 .14646 Mdot
.54286 .14638 Mdot
.54762 .14629 Mdot
.55238 .14621 Mdot
.55714 .14613 Mdot
.5619 .14605 Mdot
.56667 .14597 Mdot
.57143 .14589 Mdot
.57619 .14581 Mdot
.58095 .14574 Mdot
.58571 .14566 Mdot
.59048 .14559 Mdot
.59524 .14552 Mdot
.6 .14545 Mdot
.60476 .14538 Mdot
.60952 .14531 Mdot
.61429 .14525 Mdot
.61905 .14518 Mdot
.62381 .14512 Mdot
.62857 .14506 Mdot
.63333 .145 Mdot
.6381 .14495 Mdot
.64286 .14489 Mdot
.64762 .14483 Mdot
.65238 .14478 Mdot
.65714 .14472 Mdot
.6619 .14466 Mdot
.66667 .1446 Mdot
.67143 .14454 Mdot
.67619 .14448 Mdot
.68095 .14441 Mdot
.68571 .14435 Mdot
.69048 .14428 Mdot
.69524 .14422 Mdot
.7 .14415 Mdot
.70476 .14409 Mdot
.70952 .14403 Mdot
.71429 .14398 Mdot
.71905 .14393 Mdot
.72381 .1439 Mdot
.72857 .14388 Mdot
.73333 .14389 Mdot
.7381 .14391 Mdot
.74286 .14396 Mdot
.74762 .14403 Mdot
.75238 .14412 Mdot
.75714 .14422 Mdot
.7619 .14431 Mdot
.76667 .14438 Mdot
.77143 .14439 Mdot
.77619 .14431 Mdot
.78095 .14412 Mdot
.78571 .14379 Mdot
.79048 .1433 Mdot
.79524 .14265 Mdot
.8 .14185 Mdot
.80476 .14095 Mdot
.80952 .14002 Mdot
.81429 .13917 Mdot
.81905 .13853 Mdot
.82381 .13825 Mdot
.82857 .1385 Mdot
.83333 .13943 Mdot
.8381 .14116 Mdot
.84286 .14374 Mdot
.84762 .14715 Mdot
.85238 .15127 Mdot
.85714 .15584 Mdot
.8619 .16045 Mdot
.86667 .1646 Mdot
.87143 .16763 Mdot
.87619 .16883 Mdot
.88095 .16748 Mdot
.88571 .1629 Mdot
.89048 .15455 Mdot
.89524 .14215 Mdot
.9 .12579 Mdot
.90476 .10601 Mdot
.90952 .08388 Mdot
.91429 .06111 Mdot
.91905 .03999 Mdot
.92381 .02342 Mdot
.92857 .01472 Mdot
.93333 .01739 Mdot
.9381 .03478 Mdot
.94286 .06951 Mdot
.94762 .12287 Mdot
.95238 .19398 Mdot
.95714 .27917 Mdot
.9619 .37151 Mdot
.96667 .46115 Mdot
.97143 .5365 Mdot
.97619 .58648 Mdot
P
0 0 m
1 0 L
1 .61803 L
0 .61803 L
closepath
clip
newpath
% End of Graphics
MathPictureEnd

:[font = output; output; inactive; preserveAspect; endGroup]
Graphics["<<>>"]
;[o]
-Graphics-
:[font = input; preserveAspect]
Inner[Times,trafo,Filter,List];
:[font = input; preserveAspect]
ziel=InverseFourier[%];
:[font = input; preserveAspect]

:[font = input; preserveAspect; startGroup]
ListPlot[ziel//Re,PlotRange->All]
:[font = postscript; PostScript; formatAsPostScript; output; inactive; preserveAspect; pictureLeft = 34; pictureWidth = 457; pictureHeight = 282.375]
%!
%%Creator: Mathematica
%%AspectRatio: .61803 
MathPictureStart
%% Graphics
/Courier findfont 10  scalefont  setfont
% Scaling calculations
0.0190476 0.0047619 0.115549 11.6577 [
[(50)] .25714 .11555 0 2 Msboxa
[(100)] .49524 .11555 0 2 Msboxa
[(150)] .73333 .11555 0 2 Msboxa
[(200)] .97143 .11555 0 2 Msboxa
[(0.01)] .00655 .23213 1 0 Msboxa
[(0.02)] .00655 .3487 1 0 Msboxa
[(0.03)] .00655 .46528 1 0 Msboxa
[(0.04)] .00655 .58186 1 0 Msboxa
[ -0.001 -0.001 0 0 ]
[ 1.001 .61903 0 0 ]
] MathScale
% Start of Graphics
1 setlinecap
1 setlinejoin
newpath
[ ] 0 setdash
0 g
p
p
.002 w
.25714 .11555 m
.25714 .1218 L
s
P
[(50)] .25714 .11555 0 2 Mshowa
p
.002 w
.49524 .11555 m
.49524 .1218 L
s
P
[(100)] .49524 .11555 0 2 Mshowa
p
.002 w
.73333 .11555 m
.73333 .1218 L
s
P
[(150)] .73333 .11555 0 2 Mshowa
p
.002 w
.97143 .11555 m
.97143 .1218 L
s
P
[(200)] .97143 .11555 0 2 Mshowa
p
.001 w
.06667 .11555 m
.06667 .1193 L
s
P
p
.001 w
.11429 .11555 m
.11429 .1193 L
s
P
p
.001 w
.1619 .11555 m
.1619 .1193 L
s
P
p
.001 w
.20952 .11555 m
.20952 .1193 L
s
P
p
.001 w
.30476 .11555 m
.30476 .1193 L
s
P
p
.001 w
.35238 .11555 m
.35238 .1193 L
s
P
p
.001 w
.4 .11555 m
.4 .1193 L
s
P
p
.001 w
.44762 .11555 m
.44762 .1193 L
s
P
p
.001 w
.54286 .11555 m
.54286 .1193 L
s
P
p
.001 w
.59048 .11555 m
.59048 .1193 L
s
P
p
.001 w
.6381 .11555 m
.6381 .1193 L
s
P
p
.001 w
.68571 .11555 m
.68571 .1193 L
s
P
p
.001 w
.78095 .11555 m
.78095 .1193 L
s
P
p
.001 w
.82857 .11555 m
.82857 .1193 L
s
P
p
.001 w
.87619 .11555 m
.87619 .1193 L
s
P
p
.001 w
.92381 .11555 m
.92381 .1193 L
s
P
p
.002 w
0 .11555 m
1 .11555 L
s
P
p
.002 w
.01905 .23213 m
.0253 .23213 L
s
P
[(0.01)] .00655 .23213 1 0 Mshowa
p
.002 w
.01905 .3487 m
.0253 .3487 L
s
P
[(0.02)] .00655 .3487 1 0 Mshowa
p
.002 w
.01905 .46528 m
.0253 .46528 L
s
P
[(0.03)] .00655 .46528 1 0 Mshowa
p
.002 w
.01905 .58186 m
.0253 .58186 L
s
P
[(0.04)] .00655 .58186 1 0 Mshowa
p
.001 w
.01905 .02229 m
.0228 .02229 L
s
P
p
.001 w
.01905 .0456 m
.0228 .0456 L
s
P
p
.001 w
.01905 .06892 m
.0228 .06892 L
s
P
p
.001 w
.01905 .09223 m
.0228 .09223 L
s
P
p
.001 w
.01905 .13886 m
.0228 .13886 L
s
P
p
.001 w
.01905 .16218 m
.0228 .16218 L
s
P
p
.001 w
.01905 .1855 m
.0228 .1855 L
s
P
p
.001 w
.01905 .20881 m
.0228 .20881 L
s
P
p
.001 w
.01905 .25544 m
.0228 .25544 L
s
P
p
.001 w
.01905 .27876 m
.0228 .27876 L
s
P
p
.001 w
.01905 .30207 m
.0228 .30207 L
s
P
p
.001 w
.01905 .32539 m
.0228 .32539 L
s
P
p
.001 w
.01905 .37202 m
.0228 .37202 L
s
P
p
.001 w
.01905 .39533 m
.0228 .39533 L
s
P
p
.001 w
.01905 .41865 m
.0228 .41865 L
s
P
p
.001 w
.01905 .44196 m
.0228 .44196 L
s
P
p
.001 w
.01905 .4886 m
.0228 .4886 L
s
P
p
.001 w
.01905 .51191 m
.0228 .51191 L
s
P
p
.001 w
.01905 .53523 m
.0228 .53523 L
s
P
p
.001 w
.01905 .55854 m
.0228 .55854 L
s
P
p
.001 w
.01905 .60517 m
.0228 .60517 L
s
P
p
.002 w
.01905 0 m
.01905 .61803 L
s
P
P
p
.008 w
.02381 .1658 Mdot
.02857 .11882 Mdot
.03333 .05477 Mdot
.0381 .01472 Mdot
.04286 .042 Mdot
.04762 .1586 Mdot
.05238 .16152 Mdot
.05714 .0736 Mdot
.0619 .13562 Mdot
.06667 .10626 Mdot
.07143 .12068 Mdot
.07619 .11218 Mdot
.08095 .11803 Mdot
.08571 .11359 Mdot
.09048 .11717 Mdot
.09524 .11417 Mdot
.1 .11676 Mdot
.10476 .11448 Mdot
.10952 .11651 Mdot
.11429 .11467 Mdot
.11905 .11635 Mdot
.12381 .11481 Mdot
.12857 .11624 Mdot
.13333 .11491 Mdot
.1381 .11615 Mdot
.14286 .11498 Mdot
.14762 .11609 Mdot
.15238 .11504 Mdot
.15714 .11603 Mdot
.1619 .11509 Mdot
.16667 .11599 Mdot
.17143 .11512 Mdot
.17619 .11596 Mdot
.18095 .11516 Mdot
.18571 .11593 Mdot
.19048 .11518 Mdot
.19524 .1159 Mdot
.2 .11521 Mdot
.20476 .11588 Mdot
.20952 .11523 Mdot
.21429 .11586 Mdot
.21905 .11525 Mdot
.22381 .11584 Mdot
.22857 .11526 Mdot
.23333 .11583 Mdot
.2381 .11528 Mdot
.24286 .11582 Mdot
.24762 .11529 Mdot
.25238 .1158 Mdot
.25714 .1153 Mdot
.2619 .11579 Mdot
.26667 .11531 Mdot
.27143 .11578 Mdot
.27619 .11532 Mdot
.28095 .11578 Mdot
.28571 .11533 Mdot
.29048 .11577 Mdot
.29524 .11533 Mdot
.3 .11576 Mdot
.30476 .11534 Mdot
.30952 .11575 Mdot
.31429 .11535 Mdot
.31905 .11575 Mdot
.32381 .11535 Mdot
.32857 .11574 Mdot
.33333 .11536 Mdot
.3381 .11574 Mdot
.34286 .11536 Mdot
.34762 .11573 Mdot
.35238 .11537 Mdot
.35714 .11573 Mdot
.3619 .11537 Mdot
.36667 .11573 Mdot
.37143 .11537 Mdot
.37619 .11572 Mdot
.38095 .11538 Mdot
.38571 .11572 Mdot
.39048 .11538 Mdot
.39524 .11572 Mdot
.4 .11538 Mdot
.40476 .11571 Mdot
.40952 .11538 Mdot
.41429 .11571 Mdot
.41905 .11539 Mdot
.42381 .11571 Mdot
.42857 .11539 Mdot
.43333 .11571 Mdot
.4381 .11539 Mdot
.44286 .11571 Mdot
.44762 .11539 Mdot
.45238 .11571 Mdot
.45714 .11539 Mdot
.4619 .1157 Mdot
.46667 .11539 Mdot
.47143 .1157 Mdot
.47619 .1154 Mdot
.48095 .1157 Mdot
.48571 .1154 Mdot
.49048 .1157 Mdot
.49524 .1154 Mdot
.5 .1157 Mdot
.50476 .1154 Mdot
.50952 .1157 Mdot
.51429 .1154 Mdot
.51905 .1157 Mdot
.52381 .1154 Mdot
.52857 .1157 Mdot
.53333 .1154 Mdot
.5381 .1157 Mdot
.54286 .1154 Mdot
.54762 .1157 Mdot
.55238 .1154 Mdot
.55714 .1157 Mdot
.5619 .1154 Mdot
.56667 .1157 Mdot
.57143 .1154 Mdot
.57619 .1157 Mdot
.58095 .1154 Mdot
.58571 .1157 Mdot
.59048 .1154 Mdot
.59524 .1157 Mdot
.6 .1154 Mdot
.60476 .1157 Mdot
.60952 .1154 Mdot
.61429 .1157 Mdot
.61905 .11539 Mdot
.62381 .1157 Mdot
.62857 .11539 Mdot
.63333 .11571 Mdot
.6381 .11539 Mdot
.64286 .11571 Mdot
.64762 .11539 Mdot
.65238 .11571 Mdot
.65714 .11539 Mdot
.6619 .11571 Mdot
.66667 .11539 Mdot
.67143 .11571 Mdot
.67619 .11538 Mdot
.68095 .11572 Mdot
.68571 .11538 Mdot
.69048 .11572 Mdot
.69524 .11538 Mdot
.7 .11572 Mdot
.70476 .11538 Mdot
.70952 .11573 Mdot
.71429 .11538 Mdot
.71905 .11574 Mdot
.72381 .11538 Mdot
.72857 .11574 Mdot
.73333 .11538 Mdot
.7381 .11576 Mdot
.74286 .11539 Mdot
.74762 .11578 Mdot
.75238 .1154 Mdot
.75714 .11581 Mdot
.7619 .11544 Mdot
.76667 .11587 Mdot
.77143 .1155 Mdot
.77619 .11597 Mdot
.78095 .11561 Mdot
.78571 .11613 Mdot
.79048 .11581 Mdot
.79524 .11641 Mdot
.8 .11617 Mdot
.80476 .1169 Mdot
.80952 .11679 Mdot
.81429 .11774 Mdot
.81905 .11788 Mdot
.82381 .11919 Mdot
.82857 .11974 Mdot
.83333 .12164 Mdot
.8381 .12291 Mdot
.84286 .12579 Mdot
.84762 .12826 Mdot
.85238 .13273 Mdot
.85714 .13717 Mdot
.8619 .14419 Mdot
.86667 .15176 Mdot
.87143 .16278 Mdot
.87619 .1752 Mdot
.88095 .19225 Mdot
.88571 .21185 Mdot
.89048 .23749 Mdot
.89524 .26694 Mdot
.9 .30372 Mdot
.90476 .34493 Mdot
.90952 .39345 Mdot
.91429 .44452 Mdot
.91905 .49877 Mdot
.92381 .54747 Mdot
.92857 .58688 Mdot
.93333 .60332 Mdot
.9381 .5917 Mdot
.94286 .54344 Mdot
.94762 .47004 Mdot
.95238 .38467 Mdot
.95714 .31449 Mdot
.9619 .26599 Mdot
.96667 .24178 Mdot
.97143 .22183 Mdot
.97619 .20185 Mdot
P
0 0 m
1 0 L
1 .61803 L
0 .61803 L
closepath
clip
newpath
% End of Graphics
MathPictureEnd

:[font = output; output; inactive; preserveAspect; endGroup]
Graphics["<<>>"]
;[o]
-Graphics-
:[font = input; preserveAspect]

data ={ Table[N[E^w /.w->dewe i],{i,-100,100}],
  RotateRight[ziel,100]}//Transpose;
:[font = input; preserveAspect; startGroup]
data[[1]]
:[font = output; output; inactive; preserveAspect; endGroup]
{0.00004539992976248486, 
 
  -0.00001293625337245559 + 
 
   0.00002567184254926176*I}
;[o]
{0.0000453999, -0.0000129363 + 0.0000256718 I}
:[font = input; preserveAspect]

:[font = input; preserveAspect; startGroup]
ListPlot[data//Im,PlotRange->{{0,10},All}]
:[font = postscript; PostScript; formatAsPostScript; output; inactive; preserveAspect; pictureLeft = 0; pictureWidth = 535.375; pictureHeight = 330.812]
%!
%%Creator: Mathematica
%%AspectRatio: .61803 
MathPictureStart
%% Graphics
/Courier findfont 10  scalefont  setfont
% Scaling calculations
0 0.1 0.338414 28.207 [
[(2)] .2 .33841 0 2 Msboxa
[(4)] .4 .33841 0 2 Msboxa
[(6)] .6 .33841 0 2 Msboxa
[(8)] .8 .33841 0 2 Msboxa
[(10)] 1 .33841 0 2 Msboxa
[(-0.01)] -0.0125 .05634 1 0 Msboxa
[(-0.005)] -0.0125 .19738 1 0 Msboxa
[(0)] -0.0125 .33841 1 0 Msboxa
[(0.005)] -0.0125 .47945 1 0 Msboxa
[ -0.001 -0.001 0 0 ]
[ 1.001 .61903 0 0 ]
] MathScale
% Start of Graphics
1 setlinecap
1 setlinejoin
newpath
[ ] 0 setdash
0 g
p
p
.002 w
.2 .33841 m
.2 .34466 L
s
P
[(2)] .2 .33841 0 2 Mshowa
p
.002 w
.4 .33841 m
.4 .34466 L
s
P
[(4)] .4 .33841 0 2 Mshowa
p
.002 w
.6 .33841 m
.6 .34466 L
s
P
[(6)] .6 .33841 0 2 Mshowa
p
.002 w
.8 .33841 m
.8 .34466 L
s
P
[(8)] .8 .33841 0 2 Mshowa
p
.002 w
1 .33841 m
1 .34466 L
s
P
[(10)] 1 .33841 0 2 Mshowa
p
.001 w
.04 .33841 m
.04 .34216 L
s
P
p
.001 w
.08 .33841 m
.08 .34216 L
s
P
p
.001 w
.12 .33841 m
.12 .34216 L
s
P
p
.001 w
.16 .33841 m
.16 .34216 L
s
P
p
.001 w
.24 .33841 m
.24 .34216 L
s
P
p
.001 w
.28 .33841 m
.28 .34216 L
s
P
p
.001 w
.32 .33841 m
.32 .34216 L
s
P
p
.001 w
.36 .33841 m
.36 .34216 L
s
P
p
.001 w
.44 .33841 m
.44 .34216 L
s
P
p
.001 w
.48 .33841 m
.48 .34216 L
s
P
p
.001 w
.52 .33841 m
.52 .34216 L
s
P
p
.001 w
.56 .33841 m
.56 .34216 L
s
P
p
.001 w
.64 .33841 m
.64 .34216 L
s
P
p
.001 w
.68 .33841 m
.68 .34216 L
s
P
p
.001 w
.72 .33841 m
.72 .34216 L
s
P
p
.001 w
.76 .33841 m
.76 .34216 L
s
P
p
.001 w
.84 .33841 m
.84 .34216 L
s
P
p
.001 w
.88 .33841 m
.88 .34216 L
s
P
p
.001 w
.92 .33841 m
.92 .34216 L
s
P
p
.001 w
.96 .33841 m
.96 .34216 L
s
P
p
.002 w
0 .33841 m
1 .33841 L
s
P
p
.002 w
0 .05634 m
.00625 .05634 L
s
P
[(-0.01)] -0.0125 .05634 1 0 Mshowa
p
.002 w
0 .19738 m
.00625 .19738 L
s
P
[(-0.005)] -0.0125 .19738 1 0 Mshowa
p
.002 w
0 .33841 m
.00625 .33841 L
s
P
[(0)] -0.0125 .33841 1 0 Mshowa
p
.002 w
0 .47945 m
.00625 .47945 L
s
P
[(0.005)] -0.0125 .47945 1 0 Mshowa
p
.001 w
0 .08455 m
.00375 .08455 L
s
P
p
.001 w
0 .11276 m
.00375 .11276 L
s
P
p
.001 w
0 .14097 m
.00375 .14097 L
s
P
p
.001 w
0 .16917 m
.00375 .16917 L
s
P
p
.001 w
0 .22559 m
.00375 .22559 L
s
P
p
.001 w
0 .25379 m
.00375 .25379 L
s
P
p
.001 w
0 .282 m
.00375 .282 L
s
P
p
.001 w
0 .31021 m
.00375 .31021 L
s
P
p
.001 w
0 .36662 m
.00375 .36662 L
s
P
p
.001 w
0 .39483 m
.00375 .39483 L
s
P
p
.001 w
0 .42304 m
.00375 .42304 L
s
P
p
.001 w
0 .45124 m
.00375 .45124 L
s
P
p
.001 w
0 .50766 m
.00375 .50766 L
s
P
p
.001 w
0 .53586 m
.00375 .53586 L
s
P
p
.001 w
0 .56407 m
.00375 .56407 L
s
P
p
.001 w
0 .59228 m
.00375 .59228 L
s
P
p
.001 w
0 .02814 m
.00375 .02814 L
s
P
p
.002 w
0 0 m
0 .61803 L
s
P
P
0 0 m
1 0 L
1 .61803 L
0 .61803 L
closepath
clip
newpath
p
.008 w
0 .33914 Mdot
0 .33769 Mdot
0 .33914 Mdot
0 .33769 Mdot
0 .33914 Mdot
0 .33769 Mdot
0 .33913 Mdot
0 .33769 Mdot
0 .33914 Mdot
0 .33769 Mdot
0 .33914 Mdot
0 .33769 Mdot
0 .33914 Mdot
0 .33769 Mdot
0 .33914 Mdot
0 .33769 Mdot
0 .33914 Mdot
0 .33768 Mdot
0 .33915 Mdot
0 .33768 Mdot
0 .33915 Mdot
0 .33767 Mdot
0 .33916 Mdot
0 .33767 Mdot
0 .33916 Mdot
0 .33766 Mdot
0 .33917 Mdot
0 .33765 Mdot
0 .33918 Mdot
0 .33765 Mdot
0 .33919 Mdot
0 .33764 Mdot
0 .3392 Mdot
0 .33763 Mdot
0 .33921 Mdot
0 .33761 Mdot
0 .33922 Mdot
0 .3376 Mdot
0 .33923 Mdot
0 .33759 Mdot
0 .33925 Mdot
0 .33757 Mdot
0 .33926 Mdot
0 .33756 Mdot
0 .33928 Mdot
0 .33754 Mdot
0 .3393 Mdot
0 .33752 Mdot
0 .33932 Mdot
0 .33749 Mdot
0 .33934 Mdot
0 .33746 Mdot
0 .33936 Mdot
0 .33743 Mdot
0 .33938 Mdot
0 .33739 Mdot
0 .33939 Mdot
0 .33734 Mdot
0 .3394 Mdot
0 .33727 Mdot
0 .33939 Mdot
0 .33717 Mdot
0 .33935 Mdot
0 .33703 Mdot
0 .33925 Mdot
0 .3368 Mdot
0 .33907 Mdot
0 .33644 Mdot
0 .33872 Mdot
0 .33587 Mdot
0 .33812 Mdot
0 .33495 Mdot
0 .33708 Mdot
0 .33346 Mdot
0 .33536 Mdot
0 .33111 Mdot
0 .3326 Mdot
0 .32746 Mdot
0 .3283 Mdot
0 .32197 Mdot
0 .32193 Mdot
0 .31417 Mdot
0 .3132 Mdot
0 .30411 Mdot
0 .3028 Mdot
0 .29354 Mdot
0 .29414 Mdot
0 .28815 Mdot
0 .29583 Mdot
0 .2993 Mdot
0 .31971 Mdot
0 .33288 Mdot
0 .35487 Mdot
0 .35071 Mdot
0 .33885 Mdot
0 .29169 Mdot
0 .24921 Mdot
0 .18707 Mdot
0 .1438 Mdot
0 .07746 Mdot
0 .04292 Mdot
0 .01472 Mdot
0 .1086 Mdot
0 .30036 Mdot
0 .60332 Mdot
0 .47231 Mdot
0 .13087 Mdot
0 .44576 Mdot
0 .29035 Mdot
0 .36405 Mdot
0 .32188 Mdot
0 .35047 Mdot
0 .32894 Mdot
0 .34623 Mdot
0 .33176 Mdot
0 .34422 Mdot
0 .33327 Mdot
0 .34304 Mdot
0 .33421 Mdot
0 .34227 Mdot
0 .33486 Mdot
0 .34172 Mdot
0 .33533 Mdot
0 .34131 Mdot
0 .33568 Mdot
0 .341 Mdot
0 .33596 Mdot
0 .34075 Mdot
0 .33619 Mdot
0 .34054 Mdot
0 .33638 Mdot
0 .34037 Mdot
0 .33653 Mdot
0 .34023 Mdot
0 .33666 Mdot
0 .34011 Mdot
0 .33678 Mdot
0 .34 Mdot
0 .33687 Mdot
0 .33991 Mdot
0 .33696 Mdot
0 .33983 Mdot
0 .33703 Mdot
0 .33976 Mdot
0 .3371 Mdot
0 .3397 Mdot
0 .33716 Mdot
0 .33964 Mdot
0 .33721 Mdot
0 .33959 Mdot
0 .33726 Mdot
0 .33955 Mdot
0 .3373 Mdot
0 .33951 Mdot
0 .33734 Mdot
0 .33947 Mdot
0 .33738 Mdot
0 .33943 Mdot
0 .33741 Mdot
0 .3394 Mdot
0 .33744 Mdot
0 .33938 Mdot
0 .33747 Mdot
0 .33935 Mdot
0 .33749 Mdot
0 .33933 Mdot
0 .33751 Mdot
0 .33931 Mdot
0 .33753 Mdot
0 .33929 Mdot
0 .33755 Mdot
0 .33927 Mdot
0 .33757 Mdot
0 .33925 Mdot
0 .33758 Mdot
0 .33924 Mdot
0 .3376 Mdot
0 .33922 Mdot
0 .33761 Mdot
0 .33921 Mdot
0 .33762 Mdot
0 .3392 Mdot
0 .33763 Mdot
0 .33919 Mdot
0 .33764 Mdot
0 .33918 Mdot
0 .33765 Mdot
0 .33917 Mdot
0 .33766 Mdot
0 .33917 Mdot
0 .33767 Mdot
0 .33916 Mdot
0 .33767 Mdot
0 .33915 Mdot
0 .33768 Mdot
0 .33915 Mdot
0 .33768 Mdot
0 .33914 Mdot
0 .33769 Mdot
0 .33914 Mdot
0 .33769 Mdot
P
% End of Graphics
MathPictureEnd

:[font = output; output; inactive; preserveAspect; endGroup]
Graphics["<<>>"]
;[o]
-Graphics-
:[font = input; preserveAspect; startGroup]
Integrate[ z^a/Sqrt[z^2+1],{z,-Infinity,Infinity} ]
:[font = output; output; inactive; preserveAspect; endGroup]
((1 + (-1)^a)*Gamma[-a/2]*Gamma[(1 + a)/2])/
 
  (2*Pi^(1/2))
;[o]
         a        -a        1 + a
(1 + (-1) ) Gamma[--] Gamma[-----]
                  2           2
----------------------------------
            2 Sqrt[Pi]
:[font = input; preserveAspect; startGroup]
%9*(-I)^a/.a->-I w
:[font = output; output; inactive; preserveAspect; endGroup]
((-I)^(-I*w)*(1 + (-1)^(-I*w))*Gamma[(1 - I*w)/2]*
 
    Gamma[I/2*w])/(2*Pi^(1/2))
;[o]
    -I w          -I w        1 - I w        I
(-I)     (1 + (-1)    ) Gamma[-------] Gamma[- w]
                                 2           2
-------------------------------------------------
                   2 Sqrt[Pi]
:[font = input; preserveAspect; startGroup]
%*2^(I w) Gamma[(1+I w)/2]/Gamma[(1-I w)/2]
:[font = output; output; inactive; preserveAspect; endGroup]
((-I)^(-I*w)*2^(I*w)*(1 + (-1)^(-I*w))*
 
    Gamma[(1 + I*w)/2]*Gamma[I/2*w])/(2*Pi^(1/2))
;[o]
     -I w  I w          -I w        1 + I w
((-I)     2    (1 + (-1)    ) Gamma[-------] 
                                       2
 
          I
    Gamma[- w]) / (2 Sqrt[Pi])
          2
:[font = input; preserveAspect; startGroup]
Simplify[%]
:[font = output; output; inactive; preserveAspect; endGroup]
((-I)^(-I*w)*2^(I*w)*(1 + (-1)^(-I*w))*
 
    Gamma[(1 + I*w)/2]*Gamma[I/2*w])/(2*Pi^(1/2))
;[o]
     -I w  I w          -I w        1 + I w
((-I)     2    (1 + (-1)    ) Gamma[-------] 
                                       2
 
          I
    Gamma[- w]) / (2 Sqrt[Pi])
          2
:[font = input; preserveAspect]
orig=RotateLeft[
        Table[
           N[%65/.w->2 Pi/ 0.51/201 i],
           {i,-99,100}
        ],
        99
     ];
:[font = input; preserveAspect]
tr=InverseFourier[ReplacePart[orig,1.,1]];
:[font = input; preserveAspect; startGroup]
ListPlot[orig//Im]
:[font = postscript; PostScript; formatAsPostScript; output; inactive; preserveAspect; pictureLeft = 34; pictureWidth = 282; pictureHeight = 174.25]
%!
%%Creator: Mathematica
%%AspectRatio: .61803 
MathPictureStart
%% Graphics
/Courier findfont 10  scalefont  setfont
% Scaling calculations
0.0238095 0.0047619 0.309017 0.0441905 [
[(50)] .2619 .30902 0 2 Msboxa
[(100)] .5 .30902 0 2 Msboxa
[(150)] .7381 .30902 0 2 Msboxa
[(200)] .97619 .30902 0 2 Msboxa
[(-6)] .01131 .04387 1 0 Msboxa
[(-4)] .01131 .13225 1 0 Msboxa
[(-2)] .01131 .22064 1 0 Msboxa
[(2)] .01131 .3974 1 0 Msboxa
[(4)] .01131 .48578 1 0 Msboxa
[(6)] .01131 .57416 1 0 Msboxa
[ -0.001 -0.001 0 0 ]
[ 1.001 .61903 0 0 ]
] MathScale
% Start of Graphics
1 setlinecap
1 setlinejoin
newpath
[ ] 0 setdash
0 g
p
p
.002 w
.2619 .30902 m
.2619 .31527 L
s
P
[(50)] .2619 .30902 0 2 Mshowa
p
.002 w
.5 .30902 m
.5 .31527 L
s
P
[(100)] .5 .30902 0 2 Mshowa
p
.002 w
.7381 .30902 m
.7381 .31527 L
s
P
[(150)] .7381 .30902 0 2 Mshowa
p
.002 w
.97619 .30902 m
.97619 .31527 L
s
P
[(200)] .97619 .30902 0 2 Mshowa
p
.001 w
.07143 .30902 m
.07143 .31277 L
s
P
p
.001 w
.11905 .30902 m
.11905 .31277 L
s
P
p
.001 w
.16667 .30902 m
.16667 .31277 L
s
P
p
.001 w
.21429 .30902 m
.21429 .31277 L
s
P
p
.001 w
.30952 .30902 m
.30952 .31277 L
s
P
p
.001 w
.35714 .30902 m
.35714 .31277 L
s
P
p
.001 w
.40476 .30902 m
.40476 .31277 L
s
P
p
.001 w
.45238 .30902 m
.45238 .31277 L
s
P
p
.001 w
.54762 .30902 m
.54762 .31277 L
s
P
p
.001 w
.59524 .30902 m
.59524 .31277 L
s
P
p
.001 w
.64286 .30902 m
.64286 .31277 L
s
P
p
.001 w
.69048 .30902 m
.69048 .31277 L
s
P
p
.001 w
.78571 .30902 m
.78571 .31277 L
s
P
p
.001 w
.83333 .30902 m
.83333 .31277 L
s
P
p
.001 w
.88095 .30902 m
.88095 .31277 L
s
P
p
.001 w
.92857 .30902 m
.92857 .31277 L
s
P
p
.002 w
0 .30902 m
1 .30902 L
s
P
p
.002 w
.02381 .04387 m
.03006 .04387 L
s
P
[(-6)] .01131 .04387 1 0 Mshowa
p
.002 w
.02381 .13225 m
.03006 .13225 L
s
P
[(-4)] .01131 .13225 1 0 Mshowa
p
.002 w
.02381 .22064 m
.03006 .22064 L
s
P
[(-2)] .01131 .22064 1 0 Mshowa
p
.002 w
.02381 .3974 m
.03006 .3974 L
s
P
[(2)] .01131 .3974 1 0 Mshowa
p
.002 w
.02381 .48578 m
.03006 .48578 L
s
P
[(4)] .01131 .48578 1 0 Mshowa
p
.002 w
.02381 .57416 m
.03006 .57416 L
s
P
[(6)] .01131 .57416 1 0 Mshowa
p
.001 w
.02381 .06155 m
.02756 .06155 L
s
P
p
.001 w
.02381 .07923 m
.02756 .07923 L
s
P
p
.001 w
.02381 .0969 m
.02756 .0969 L
s
P
p
.001 w
.02381 .11458 m
.02756 .11458 L
s
P
p
.001 w
.02381 .14993 m
.02756 .14993 L
s
P
p
.001 w
.02381 .16761 m
.02756 .16761 L
s
P
p
.001 w
.02381 .18528 m
.02756 .18528 L
s
P
p
.001 w
.02381 .20296 m
.02756 .20296 L
s
P
p
.001 w
.02381 .23831 m
.02756 .23831 L
s
P
p
.001 w
.02381 .25599 m
.02756 .25599 L
s
P
p
.001 w
.02381 .27366 m
.02756 .27366 L
s
P
p
.001 w
.02381 .29134 m
.02756 .29134 L
s
P
p
.001 w
.02381 .32669 m
.02756 .32669 L
s
P
p
.001 w
.02381 .34437 m
.02756 .34437 L
s
P
p
.001 w
.02381 .36205 m
.02756 .36205 L
s
P
p
.001 w
.02381 .37972 m
.02756 .37972 L
s
P
p
.001 w
.02381 .41507 m
.02756 .41507 L
s
P
p
.001 w
.02381 .43275 m
.02756 .43275 L
s
P
p
.001 w
.02381 .45043 m
.02756 .45043 L
s
P
p
.001 w
.02381 .4681 m
.02756 .4681 L
s
P
p
.001 w
.02381 .50346 m
.02756 .50346 L
s
P
p
.001 w
.02381 .52113 m
.02756 .52113 L
s
P
p
.001 w
.02381 .53881 m
.02756 .53881 L
s
P
p
.001 w
.02381 .55648 m
.02756 .55648 L
s
P
p
.001 w
.02381 .0262 m
.02756 .0262 L
s
P
p
.001 w
.02381 .00852 m
.02756 .00852 L
s
P
p
.001 w
.02381 .59184 m
.02756 .59184 L
s
P
p
.001 w
.02381 .60951 m
.02756 .60951 L
s
P
p
.002 w
.02381 0 m
.02381 .61803 L
s
P
P
0 0 m
1 0 L
1 .61803 L
0 .61803 L
closepath
clip
newpath
p
.008 w
.03333 -1.13424 Mdot
.0381 -0.4146 Mdot
.04286 -0.17561 Mdot
.04762 -0.05679 Mdot
.05238 .01397 Mdot
.05714 .06068 Mdot
.0619 .09366 Mdot
.06667 .11804 Mdot
.07143 .1367 Mdot
.07619 .15135 Mdot
.08095 .16308 Mdot
.08571 .17263 Mdot
.09048 .1805 Mdot
.09524 .18706 Mdot
.1 .19257 Mdot
.10476 .19725 Mdot
.10952 .20125 Mdot
.11429 .20468 Mdot
.11905 .20767 Mdot
.12381 .21028 Mdot
.12857 .21258 Mdot
.13333 .21464 Mdot
.1381 .2165 Mdot
.14286 .2182 Mdot
.14762 .21979 Mdot
.15238 .2213 Mdot
.15714 .22276 Mdot
.1619 .2242 Mdot
.16667 .22563 Mdot
.17143 .22709 Mdot
.17619 .2286 Mdot
.18095 .23017 Mdot
.18571 .23183 Mdot
.19048 .23358 Mdot
.19524 .23544 Mdot
.2 .23743 Mdot
.20476 .23956 Mdot
.20952 .24183 Mdot
.21429 .24426 Mdot
.21905 .24684 Mdot
.22381 .2496 Mdot
.22857 .25252 Mdot
.23333 .25562 Mdot
.2381 .25888 Mdot
.24286 .26231 Mdot
.24762 .26591 Mdot
.25238 .26966 Mdot
.25714 .27357 Mdot
.2619 .27761 Mdot
.26667 .28177 Mdot
.27143 .28605 Mdot
.27619 .29042 Mdot
.28095 .29487 Mdot
.28571 .29938 Mdot
.29048 .30392 Mdot
.29524 .30847 Mdot
.3 .31301 Mdot
.30476 .31751 Mdot
.30952 .32194 Mdot
.31429 .32628 Mdot
.31905 .33049 Mdot
.32381 .33454 Mdot
.32857 .33841 Mdot
.33333 .34206 Mdot
.3381 .34547 Mdot
.34286 .34861 Mdot
.34762 .35145 Mdot
.35238 .35396 Mdot
.35714 .35612 Mdot
.3619 .3579 Mdot
.36667 .35929 Mdot
.37143 .36028 Mdot
.37619 .36083 Mdot
.38095 .36096 Mdot
.38571 .36064 Mdot
.39048 .35987 Mdot
.39524 .35866 Mdot
.4 .35701 Mdot
.40476 .35493 Mdot
.40952 .35244 Mdot
.41429 .34955 Mdot
.41905 .34628 Mdot
.42381 .34267 Mdot
.42857 .33874 Mdot
.43333 .33453 Mdot
.4381 .33008 Mdot
.44286 .32544 Mdot
.44762 .32065 Mdot
.45238 .31575 Mdot
.45714 .31081 Mdot
.4619 .30587 Mdot
.46667 .30099 Mdot
.47143 .29622 Mdot
.47619 .29161 Mdot
.48095 .28723 Mdot
.48571 .28312 Mdot
.49048 .27933 Mdot
.49524 .27592 Mdot
.5 .27291 Mdot
.50476 .27036 Mdot
.50952 .34512 Mdot
.51429 .34212 Mdot
.51905 .3387 Mdot
.52381 .33491 Mdot
.52857 .3308 Mdot
.53333 .32642 Mdot
.5381 .32182 Mdot
.54286 .31705 Mdot
.54762 .31217 Mdot
.55238 .30722 Mdot
.55714 .30228 Mdot
.5619 .29739 Mdot
.56667 .29259 Mdot
.57143 .28795 Mdot
.57619 .2835 Mdot
.58095 .2793 Mdot
.58571 .27537 Mdot
.59048 .27175 Mdot
.59524 .26849 Mdot
.6 .2656 Mdot
.60476 .2631 Mdot
.60952 .26102 Mdot
.61429 .25937 Mdot
.61905 .25816 Mdot
.62381 .2574 Mdot
.62857 .25708 Mdot
.63333 .2572 Mdot
.6381 .25776 Mdot
.64286 .25874 Mdot
.64762 .26013 Mdot
.65238 .26192 Mdot
.65714 .26408 Mdot
.6619 .26659 Mdot
.66667 .26942 Mdot
.67143 .27256 Mdot
.67619 .27597 Mdot
.68095 .27962 Mdot
.68571 .28349 Mdot
.69048 .28755 Mdot
.69524 .29176 Mdot
.7 .29609 Mdot
.70476 .30052 Mdot
.70952 .30502 Mdot
.71429 .30956 Mdot
.71905 .31411 Mdot
.72381 .31865 Mdot
.72857 .32316 Mdot
.73333 .32761 Mdot
.7381 .33198 Mdot
.74286 .33626 Mdot
.74762 .34043 Mdot
.75238 .34447 Mdot
.75714 .34837 Mdot
.7619 .35212 Mdot
.76667 .35572 Mdot
.77143 .35915 Mdot
.77619 .36242 Mdot
.78095 .36551 Mdot
.78571 .36843 Mdot
.79048 .37119 Mdot
.79524 .37378 Mdot
.8 .3762 Mdot
.80476 .37848 Mdot
.80952 .3806 Mdot
.81429 .38259 Mdot
.81905 .38446 Mdot
.82381 .38621 Mdot
.82857 .38786 Mdot
.83333 .38943 Mdot
.8381 .39094 Mdot
.84286 .3924 Mdot
.84762 .39384 Mdot
.85238 .39527 Mdot
.85714 .39673 Mdot
.8619 .39824 Mdot
.86667 .39983 Mdot
.87143 .40154 Mdot
.87619 .4034 Mdot
.88095 .40545 Mdot
.88571 .40776 Mdot
.89048 .41037 Mdot
.89524 .41335 Mdot
.9 .41679 Mdot
.90476 .42078 Mdot
.90952 .42546 Mdot
.91429 .43098 Mdot
.91905 .43754 Mdot
.92381 .44541 Mdot
.92857 .45495 Mdot
.93333 .46669 Mdot
.9381 .48133 Mdot
.94286 .49999 Mdot
.94762 .52438 Mdot
.95238 .55735 Mdot
.95714 .60407 Mdot
.9619 .67482 Mdot
.96667 .79365 Mdot
.97143 1.03264 Mdot
.97619 1.75228 Mdot
P
% End of Graphics
MathPictureEnd

:[font = message; inactive; preserveAspect]
Graphics::gptn: 
   Coordinate Im[ComplexInfinity] in 
    {1, Im[ComplexInfinity]}
     is not a floating-point number.
:[font = output; output; inactive; preserveAspect; endGroup]
Graphics["<<>>"]
;[o]
-Graphics-
:[font = input; preserveAspect; startGroup]
ListPlot[tr//Re]
:[font = postscript; PostScript; formatAsPostScript; output; inactive; preserveAspect; pictureLeft = 34; pictureWidth = 282; pictureHeight = 174.25]
%!
%%Creator: Mathematica
%%AspectRatio: .61803 
MathPictureStart
%% Graphics
/Courier findfont 10  scalefont  setfont
% Scaling calculations
0.0238095 0.0047619 0.456616 0.0197037 [
[(50)] .2619 .45662 0 2 Msboxa
[(100)] .5 .45662 0 2 Msboxa
[(150)] .7381 .45662 0 2 Msboxa
[(200)] .97619 .45662 0 2 Msboxa
[(-20)] .01131 .06254 1 0 Msboxa
[(-15)] .01131 .16106 1 0 Msboxa
[(-10)] .01131 .25958 1 0 Msboxa
[(-5)] .01131 .3581 1 0 Msboxa
[(5)] .01131 .55513 1 0 Msboxa
[ -0.001 -0.001 0 0 ]
[ 1.001 .61903 0 0 ]
] MathScale
% Start of Graphics
1 setlinecap
1 setlinejoin
newpath
[ ] 0 setdash
0 g
p
p
.002 w
.2619 .45662 m
.2619 .46287 L
s
P
[(50)] .2619 .45662 0 2 Mshowa
p
.002 w
.5 .45662 m
.5 .46287 L
s
P
[(100)] .5 .45662 0 2 Mshowa
p
.002 w
.7381 .45662 m
.7381 .46287 L
s
P
[(150)] .7381 .45662 0 2 Mshowa
p
.002 w
.97619 .45662 m
.97619 .46287 L
s
P
[(200)] .97619 .45662 0 2 Mshowa
p
.001 w
.07143 .45662 m
.07143 .46037 L
s
P
p
.001 w
.11905 .45662 m
.11905 .46037 L
s
P
p
.001 w
.16667 .45662 m
.16667 .46037 L
s
P
p
.001 w
.21429 .45662 m
.21429 .46037 L
s
P
p
.001 w
.30952 .45662 m
.30952 .46037 L
s
P
p
.001 w
.35714 .45662 m
.35714 .46037 L
s
P
p
.001 w
.40476 .45662 m
.40476 .46037 L
s
P
p
.001 w
.45238 .45662 m
.45238 .46037 L
s
P
p
.001 w
.54762 .45662 m
.54762 .46037 L
s
P
p
.001 w
.59524 .45662 m
.59524 .46037 L
s
P
p
.001 w
.64286 .45662 m
.64286 .46037 L
s
P
p
.001 w
.69048 .45662 m
.69048 .46037 L
s
P
p
.001 w
.78571 .45662 m
.78571 .46037 L
s
P
p
.001 w
.83333 .45662 m
.83333 .46037 L
s
P
p
.001 w
.88095 .45662 m
.88095 .46037 L
s
P
p
.001 w
.92857 .45662 m
.92857 .46037 L
s
P
p
.002 w
0 .45662 m
1 .45662 L
s
P
p
.002 w
.02381 .06254 m
.03006 .06254 L
s
P
[(-20)] .01131 .06254 1 0 Mshowa
p
.002 w
.02381 .16106 m
.03006 .16106 L
s
P
[(-15)] .01131 .16106 1 0 Mshowa
p
.002 w
.02381 .25958 m
.03006 .25958 L
s
P
[(-10)] .01131 .25958 1 0 Mshowa
p
.002 w
.02381 .3581 m
.03006 .3581 L
s
P
[(-5)] .01131 .3581 1 0 Mshowa
p
.002 w
.02381 .55513 m
.03006 .55513 L
s
P
[(5)] .01131 .55513 1 0 Mshowa
p
.001 w
.02381 .08225 m
.02756 .08225 L
s
P
p
.001 w
.02381 .10195 m
.02756 .10195 L
s
P
p
.001 w
.02381 .12165 m
.02756 .12165 L
s
P
p
.001 w
.02381 .14136 m
.02756 .14136 L
s
P
p
.001 w
.02381 .18076 m
.02756 .18076 L
s
P
p
.001 w
.02381 .20047 m
.02756 .20047 L
s
P
p
.001 w
.02381 .22017 m
.02756 .22017 L
s
P
p
.001 w
.02381 .23987 m
.02756 .23987 L
s
P
p
.001 w
.02381 .27928 m
.02756 .27928 L
s
P
p
.001 w
.02381 .29899 m
.02756 .29899 L
s
P
p
.001 w
.02381 .31869 m
.02756 .31869 L
s
P
p
.001 w
.02381 .33839 m
.02756 .33839 L
s
P
p
.001 w
.02381 .3778 m
.02756 .3778 L
s
P
p
.001 w
.02381 .3975 m
.02756 .3975 L
s
P
p
.001 w
.02381 .41721 m
.02756 .41721 L
s
P
p
.001 w
.02381 .43691 m
.02756 .43691 L
s
P
p
.001 w
.02381 .47632 m
.02756 .47632 L
s
P
p
.001 w
.02381 .49602 m
.02756 .49602 L
s
P
p
.001 w
.02381 .51573 m
.02756 .51573 L
s
P
p
.001 w
.02381 .53543 m
.02756 .53543 L
s
P
p
.001 w
.02381 .04284 m
.02756 .04284 L
s
P
p
.001 w
.02381 .02313 m
.02756 .02313 L
s
P
p
.001 w
.02381 .00343 m
.02756 .00343 L
s
P
p
.001 w
.02381 .57484 m
.02756 .57484 L
s
P
p
.001 w
.02381 .59454 m
.02756 .59454 L
s
P
p
.001 w
.02381 .61425 m
.02756 .61425 L
s
P
p
.002 w
.02381 0 m
.02381 .61803 L
s
P
P
0 0 m
1 0 L
1 .61803 L
0 .61803 L
closepath
clip
newpath
p
.008 w
.02857 .4508 Mdot
.03333 .31679 Mdot
.0381 .01472 Mdot
.04286 .35123 Mdot
.04762 .38269 Mdot
.05238 .27838 Mdot
.05714 .35595 Mdot
.0619 .30451 Mdot
.06667 .34562 Mdot
.07143 .31541 Mdot
.07619 .34311 Mdot
.08095 .32209 Mdot
.08571 .3431 Mdot
.09048 .32717 Mdot
.09524 .34418 Mdot
.1 .33148 Mdot
.10476 .34582 Mdot
.10952 .33537 Mdot
.11429 .3478 Mdot
.11905 .33899 Mdot
.12381 .34999 Mdot
.12857 .34244 Mdot
.13333 .35232 Mdot
.1381 .34577 Mdot
.14286 .35475 Mdot
.14762 .34901 Mdot
.15238 .35725 Mdot
.15714 .35219 Mdot
.1619 .35981 Mdot
.16667 .35532 Mdot
.17143 .36241 Mdot
.17619 .35841 Mdot
.18095 .36505 Mdot
.18571 .36147 Mdot
.19048 .36771 Mdot
.19524 .36451 Mdot
.2 .3704 Mdot
.20476 .36753 Mdot
.20952 .3731 Mdot
.21429 .37053 Mdot
.21905 .37582 Mdot
.22381 .37352 Mdot
.22857 .37855 Mdot
.23333 .3765 Mdot
.2381 .38129 Mdot
.24286 .37946 Mdot
.24762 .38404 Mdot
.25238 .38242 Mdot
.25714 .3868 Mdot
.2619 .38537 Mdot
.26667 .38957 Mdot
.27143 .38831 Mdot
.27619 .39234 Mdot
.28095 .39125 Mdot
.28571 .39512 Mdot
.29048 .39418 Mdot
.29524 .39791 Mdot
.3 .39711 Mdot
.30476 .40069 Mdot
.30952 .40003 Mdot
.31429 .40348 Mdot
.31905 .40295 Mdot
.32381 .40628 Mdot
.32857 .40587 Mdot
.33333 .40908 Mdot
.3381 .40878 Mdot
.34286 .41188 Mdot
.34762 .4117 Mdot
.35238 .41468 Mdot
.35714 .4146 Mdot
.3619 .41748 Mdot
.36667 .41751 Mdot
.37143 .42029 Mdot
.37619 .42042 Mdot
.38095 .4231 Mdot
.38571 .42332 Mdot
.39048 .4259 Mdot
.39524 .42623 Mdot
.4 .42872 Mdot
.40476 .42913 Mdot
.40952 .43153 Mdot
.41429 .43203 Mdot
.41905 .43434 Mdot
.42381 .43493 Mdot
.42857 .43715 Mdot
.43333 .43783 Mdot
.4381 .43997 Mdot
.44286 .44072 Mdot
.44762 .44278 Mdot
.45238 .44362 Mdot
.45714 .4456 Mdot
.4619 .44652 Mdot
.46667 .44842 Mdot
.47143 .44941 Mdot
.47619 .45123 Mdot
.48095 .45231 Mdot
.48571 .45405 Mdot
.49048 .45521 Mdot
.49524 .45687 Mdot
.5 .4581 Mdot
.50476 .45969 Mdot
.50952 .461 Mdot
.51429 .46251 Mdot
.51905 .46389 Mdot
.52381 .46532 Mdot
.52857 .46678 Mdot
.53333 .46814 Mdot
.5381 .46968 Mdot
.54286 .47096 Mdot
.54762 .47257 Mdot
.55238 .47378 Mdot
.55714 .47547 Mdot
.5619 .4766 Mdot
.56667 .47836 Mdot
.57143 .47941 Mdot
.57619 .48126 Mdot
.58095 .48223 Mdot
.58571 .48416 Mdot
.59048 .48505 Mdot
.59524 .48705 Mdot
.6 .48786 Mdot
.60476 .48995 Mdot
.60952 .49068 Mdot
.61429 .49285 Mdot
.61905 .49349 Mdot
.62381 .49575 Mdot
.62857 .4963 Mdot
.63333 .49865 Mdot
.6381 .49912 Mdot
.64286 .50155 Mdot
.64762 .50193 Mdot
.65238 .50445 Mdot
.65714 .50474 Mdot
.6619 .50735 Mdot
.66667 .50755 Mdot
.67143 .51026 Mdot
.67619 .51035 Mdot
.68095 .51317 Mdot
.68571 .51316 Mdot
.69048 .51607 Mdot
.69524 .51596 Mdot
.7 .51899 Mdot
.70476 .51876 Mdot
.70952 .5219 Mdot
.71429 .52156 Mdot
.71905 .52481 Mdot
.72381 .52436 Mdot
.72857 .52773 Mdot
.73333 .52715 Mdot
.7381 .53065 Mdot
.74286 .52994 Mdot
.74762 .53358 Mdot
.75238 .53273 Mdot
.75714 .53651 Mdot
.7619 .53551 Mdot
.76667 .53944 Mdot
.77143 .53829 Mdot
.77619 .54238 Mdot
.78095 .54106 Mdot
.78571 .54533 Mdot
.79048 .54382 Mdot
.79524 .54828 Mdot
.8 .54658 Mdot
.80476 .55124 Mdot
.80952 .54933 Mdot
.81429 .55421 Mdot
.81905 .55207 Mdot
.82381 .55719 Mdot
.82857 .55479 Mdot
.83333 .56018 Mdot
.8381 .55751 Mdot
.84286 .56319 Mdot
.84762 .5602 Mdot
.85238 .56621 Mdot
.85714 .56288 Mdot
.8619 .56926 Mdot
.86667 .56554 Mdot
.87143 .57233 Mdot
.87619 .56816 Mdot
.88095 .57544 Mdot
.88571 .57075 Mdot
.89048 .57858 Mdot
.89524 .57329 Mdot
.9 .58178 Mdot
.90476 .57577 Mdot
.90952 .58506 Mdot
.91429 .57817 Mdot
.91905 .58842 Mdot
.92381 .58045 Mdot
.92857 .59192 Mdot
.93333 .58257 Mdot
.9381 .59561 Mdot
.94286 .58441 Mdot
.94762 .59953 Mdot
.95238 .58562 Mdot
.95714 .60332 Mdot
.9619 .58434 Mdot
.96667 .60309 Mdot
.97143 .56791 Mdot
.97619 .56754 Mdot
P
% End of Graphics
MathPictureEnd

:[font = output; output; inactive; preserveAspect; endGroup]
Graphics["<<>>"]
;[o]
-Graphics-
:[font = input; preserveAspect]
Fourier[RotateLeft[Table[N[i],{i,-100,100}],100]];
:[font = input; preserveAspect; startGroup]
ListPlot[%%//Im]
:[font = postscript; PostScript; formatAsPostScript; output; inactive; preserveAspect; pictureLeft = 34; pictureWidth = 282; pictureHeight = 174.25]
%!
%%Creator: Mathematica
%%AspectRatio: .61803 
MathPictureStart
%% Graphics
/Courier findfont 10  scalefont  setfont
% Scaling calculations
0.0238095 0.00473821 0.309017 0.00553679 [
[(50)] .26072 .30902 0 2 Msboxa
[(100)] .49763 .30902 0 2 Msboxa
[(150)] .73454 .30902 0 2 Msboxa
[(200)] .97145 .30902 0 2 Msboxa
[(-40)] .01131 .08755 1 0 Msboxa
[(-20)] .01131 .19828 1 0 Msboxa
[(20)] .01131 .41975 1 0 Msboxa
[(40)] .01131 .53049 1 0 Msboxa
[ -0.001 -0.001 0 0 ]
[ 1.001 .61903 0 0 ]
] MathScale
% Start of Graphics
1 setlinecap
1 setlinejoin
newpath
[ ] 0 setdash
0 g
p
p
.002 w
.26072 .30902 m
.26072 .31527 L
s
P
[(50)] .26072 .30902 0 2 Mshowa
p
.002 w
.49763 .30902 m
.49763 .31527 L
s
P
[(100)] .49763 .30902 0 2 Mshowa
p
.002 w
.73454 .30902 m
.73454 .31527 L
s
P
[(150)] .73454 .30902 0 2 Mshowa
p
.002 w
.97145 .30902 m
.97145 .31527 L
s
P
[(200)] .97145 .30902 0 2 Mshowa
p
.001 w
.07119 .30902 m
.07119 .31277 L
s
P
p
.001 w
.11857 .30902 m
.11857 .31277 L
s
P
p
.001 w
.16596 .30902 m
.16596 .31277 L
s
P
p
.001 w
.21334 .30902 m
.21334 .31277 L
s
P
p
.001 w
.3081 .30902 m
.3081 .31277 L
s
P
p
.001 w
.35548 .30902 m
.35548 .31277 L
s
P
p
.001 w
.40287 .30902 m
.40287 .31277 L
s
P
p
.001 w
.45025 .30902 m
.45025 .31277 L
s
P
p
.001 w
.54501 .30902 m
.54501 .31277 L
s
P
p
.001 w
.5924 .30902 m
.5924 .31277 L
s
P
p
.001 w
.63978 .30902 m
.63978 .31277 L
s
P
p
.001 w
.68716 .30902 m
.68716 .31277 L
s
P
p
.001 w
.78192 .30902 m
.78192 .31277 L
s
P
p
.001 w
.82931 .30902 m
.82931 .31277 L
s
P
p
.001 w
.87669 .30902 m
.87669 .31277 L
s
P
p
.001 w
.92407 .30902 m
.92407 .31277 L
s
P
p
.002 w
0 .30902 m
1 .30902 L
s
P
p
.002 w
.02381 .08755 m
.03006 .08755 L
s
P
[(-40)] .01131 .08755 1 0 Mshowa
p
.002 w
.02381 .19828 m
.03006 .19828 L
s
P
[(-20)] .01131 .19828 1 0 Mshowa
p
.002 w
.02381 .41975 m
.03006 .41975 L
s
P
[(20)] .01131 .41975 1 0 Mshowa
p
.002 w
.02381 .53049 m
.03006 .53049 L
s
P
[(40)] .01131 .53049 1 0 Mshowa
p
.001 w
.02381 .10969 m
.02756 .10969 L
s
P
p
.001 w
.02381 .13184 m
.02756 .13184 L
s
P
p
.001 w
.02381 .15399 m
.02756 .15399 L
s
P
p
.001 w
.02381 .17613 m
.02756 .17613 L
s
P
p
.001 w
.02381 .22043 m
.02756 .22043 L
s
P
p
.001 w
.02381 .24258 m
.02756 .24258 L
s
P
p
.001 w
.02381 .26472 m
.02756 .26472 L
s
P
p
.001 w
.02381 .28687 m
.02756 .28687 L
s
P
p
.001 w
.02381 .33116 m
.02756 .33116 L
s
P
p
.001 w
.02381 .35331 m
.02756 .35331 L
s
P
p
.001 w
.02381 .37546 m
.02756 .37546 L
s
P
p
.001 w
.02381 .39761 m
.02756 .39761 L
s
P
p
.001 w
.02381 .4419 m
.02756 .4419 L
s
P
p
.001 w
.02381 .46405 m
.02756 .46405 L
s
P
p
.001 w
.02381 .48619 m
.02756 .48619 L
s
P
p
.001 w
.02381 .50834 m
.02756 .50834 L
s
P
p
.001 w
.02381 .0654 m
.02756 .0654 L
s
P
p
.001 w
.02381 .04325 m
.02756 .04325 L
s
P
p
.001 w
.02381 .0211 m
.02756 .0211 L
s
P
p
.001 w
.02381 .55264 m
.02756 .55264 L
s
P
p
.001 w
.02381 .57478 m
.02756 .57478 L
s
P
p
.001 w
.02381 .59693 m
.02756 .59693 L
s
P
p
.002 w
.02381 0 m
.02381 .61803 L
s
P
P
0 0 m
1 0 L
1 .61803 L
0 .61803 L
closepath
clip
newpath
p
.008 w
.02855 .30902 Mdot
.03329 2.82027 Mdot
.03802 -0.94676 Mdot
.04276 1.14637 Mdot
.0475 -0.31918 Mdot
.05224 .81176 Mdot
.05698 -0.11012 Mdot
.06172 .66847 Mdot
.06645 -0.0057 Mdot
.07119 .58896 Mdot
.07593 .05688 Mdot
.08067 .53843 Mdot
.08541 .09852 Mdot
.09014 .50352 Mdot
.09488 .12821 Mdot
.09962 .47797 Mdot
.10436 .15042 Mdot
.1091 .45848 Mdot
.11384 .16765 Mdot
.11857 .44315 Mdot
.12331 .18139 Mdot
.12805 .43077 Mdot
.13279 .19259 Mdot
.13753 .42058 Mdot
.14226 .20189 Mdot
.147 .41207 Mdot
.15174 .20972 Mdot
.15648 .40484 Mdot
.16122 .21641 Mdot
.16596 .39865 Mdot
.17069 .22216 Mdot
.17543 .39328 Mdot
.18017 .22717 Mdot
.18491 .38859 Mdot
.18965 .23157 Mdot
.19439 .38447 Mdot
.19912 .23544 Mdot
.20386 .38082 Mdot
.2086 .23888 Mdot
.21334 .37757 Mdot
.21808 .24195 Mdot
.22281 .37467 Mdot
.22755 .24471 Mdot
.23229 .37206 Mdot
.23703 .24719 Mdot
.24177 .3697 Mdot
.24651 .24942 Mdot
.25124 .36757 Mdot
.25598 .25145 Mdot
.26072 .36564 Mdot
.26546 .25329 Mdot
.2702 .36388 Mdot
.27493 .25497 Mdot
.27967 .36228 Mdot
.28441 .2565 Mdot
.28915 .36082 Mdot
.29389 .25789 Mdot
.29863 .35949 Mdot
.30336 .25917 Mdot
.3081 .35827 Mdot
.31284 .26034 Mdot
.31758 .35715 Mdot
.32232 .26141 Mdot
.32706 .35613 Mdot
.33179 .26238 Mdot
.33653 .35519 Mdot
.34127 .26328 Mdot
.34601 .35434 Mdot
.35075 .2641 Mdot
.35548 .35356 Mdot
.36022 .26484 Mdot
.36496 .35284 Mdot
.3697 .26552 Mdot
.37444 .35219 Mdot
.37918 .26614 Mdot
.38391 .3516 Mdot
.38865 .2667 Mdot
.39339 .35107 Mdot
.39813 .26721 Mdot
.40287 .35059 Mdot
.4076 .26766 Mdot
.41234 .35016 Mdot
.41708 .26807 Mdot
.42182 .34978 Mdot
.42656 .26843 Mdot
.4313 .34945 Mdot
.43603 .26874 Mdot
.44077 .34916 Mdot
.44551 .26901 Mdot
.45025 .34891 Mdot
.45499 .26923 Mdot
.45973 .3487 Mdot
.46446 .26942 Mdot
.4692 .34854 Mdot
.47394 .26956 Mdot
.47868 .34841 Mdot
.48342 .26967 Mdot
.48815 .34832 Mdot
.49289 .26974 Mdot
.49763 .34828 Mdot
.50237 .26977 Mdot
.50711 .34827 Mdot
.51185 .26976 Mdot
.51658 .3483 Mdot
.52132 .26971 Mdot
.52606 .34836 Mdot
.5308 .26962 Mdot
.53554 .34847 Mdot
.54027 .2695 Mdot
.54501 .34861 Mdot
.54975 .26933 Mdot
.55449 .3488 Mdot
.55923 .26913 Mdot
.56397 .34903 Mdot
.5687 .26888 Mdot
.57344 .3493 Mdot
.57818 .26859 Mdot
.58292 .34961 Mdot
.58766 .26825 Mdot
.5924 .34997 Mdot
.59713 .26787 Mdot
.60187 .35037 Mdot
.60661 .26744 Mdot
.61135 .35082 Mdot
.61609 .26696 Mdot
.62082 .35133 Mdot
.62556 .26643 Mdot
.6303 .35189 Mdot
.63504 .26584 Mdot
.63978 .35251 Mdot
.64452 .26519 Mdot
.64925 .35319 Mdot
.65399 .26448 Mdot
.65873 .35394 Mdot
.66347 .2637 Mdot
.66821 .35476 Mdot
.67294 .26284 Mdot
.67768 .35565 Mdot
.68242 .2619 Mdot
.68716 .35663 Mdot
.6919 .26088 Mdot
.69664 .3577 Mdot
.70137 .25977 Mdot
.70611 .35887 Mdot
.71085 .25855 Mdot
.71559 .36014 Mdot
.72033 .25721 Mdot
.72507 .36154 Mdot
.7298 .25575 Mdot
.73454 .36307 Mdot
.73928 .25415 Mdot
.74402 .36474 Mdot
.74876 .25239 Mdot
.75349 .36658 Mdot
.75823 .25046 Mdot
.76297 .36861 Mdot
.76771 .24833 Mdot
.77245 .37085 Mdot
.77719 .24598 Mdot
.78192 .37333 Mdot
.78666 .24337 Mdot
.7914 .37608 Mdot
.79614 .24046 Mdot
.80088 .37915 Mdot
.80561 .23721 Mdot
.81035 .38259 Mdot
.81509 .23356 Mdot
.81983 .38647 Mdot
.82457 .22944 Mdot
.82931 .39086 Mdot
.83404 .22475 Mdot
.83878 .39587 Mdot
.84352 .21939 Mdot
.84826 .40163 Mdot
.853 .21319 Mdot
.85774 .40831 Mdot
.86247 .20597 Mdot
.86721 .41614 Mdot
.87195 .19745 Mdot
.87669 .42544 Mdot
.88143 .18726 Mdot
.88616 .43664 Mdot
.8909 .17489 Mdot
.89564 .45038 Mdot
.90038 .15955 Mdot
.90512 .46761 Mdot
.90986 .14006 Mdot
.91459 .48982 Mdot
.91933 .11452 Mdot
.92407 .51951 Mdot
.92881 .0796 Mdot
.93355 .56116 Mdot
.93828 .02908 Mdot
.94302 .62373 Mdot
.94776 -0.05044 Mdot
.9525 .72816 Mdot
.95724 -0.19372 Mdot
.96198 .93721 Mdot
.96671 -0.52834 Mdot
.97145 1.5648 Mdot
.97619 -2.20223 Mdot
P
% End of Graphics
MathPictureEnd

:[font = output; output; inactive; preserveAspect; endGroup]
Graphics["<<>>"]
;[o]
-Graphics-
:[font = input; preserveAspect; startGroup]
Chop[%]
:[font = output; output; inactive; preserveAspect; endGroup]
{0, 3.048917339522305*I, -1.692021471630096*I, 
 
  1.356895867892209*I, -1.356895867892209*I, 
 
  1.692021471630096*I, -3.048917339522305*I}
;[o]
{0, 3.04892 I, -1.69202 I, 1.3569 I, -1.3569 I, 
 
  1.69202 I, -3.04892 I}
:[font = input; preserveAspect]
Table[0.,{201}];
:[font = input; preserveAspect]
ReplacePart[%58,1.,199];
:[font = input; preserveAspect]
InverseFourier[N[%]];
:[font = input; preserveAspect; startGroup]
InverseFourier[{1.,0.,0.,0.}//N]
:[font = output; output; inactive; preserveAspect; endGroup]
{0.5 + 0.*I, 0.5 + 0.*I, 0.5 + 0.*I, 0.5 + 0.*I}
;[o]
{0.5 + 0. I, 0.5 + 0. I, 0.5 + 0. I, 0.5 + 0. I}
:[font = input; preserveAspect; startGroup]
ListPlot[%//Re]
:[font = postscript; PostScript; formatAsPostScript; output; inactive; preserveAspect; pictureLeft = 34; pictureWidth = 282; pictureHeight = 174.25]
%!
%%Creator: Mathematica
%%AspectRatio: .61803 
MathPictureStart
%% Graphics
/Courier findfont 10  scalefont  setfont
% Scaling calculations
0.0238095 0.00473821 0.308855 4.17474 [
[(50)] .26072 .30886 0 2 Msboxa
[(100)] .49763 .30886 0 2 Msboxa
[(150)] .73454 .30886 0 2 Msboxa
[(200)] .97145 .30886 0 2 Msboxa
[(-0.06)] .01131 .05837 1 0 Msboxa
[(-0.04)] .01131 .14187 1 0 Msboxa
[(-0.02)] .01131 .22536 1 0 Msboxa
[(0.02)] .01131 .39235 1 0 Msboxa
[(0.04)] .01131 .47584 1 0 Msboxa
[(0.06)] .01131 .55934 1 0 Msboxa
[ -0.001 -0.001 0 0 ]
[ 1.001 .61903 0 0 ]
] MathScale
% Start of Graphics
1 setlinecap
1 setlinejoin
newpath
[ ] 0 setdash
0 g
p
p
.002 w
.26072 .30886 m
.26072 .31511 L
s
P
[(50)] .26072 .30886 0 2 Mshowa
p
.002 w
.49763 .30886 m
.49763 .31511 L
s
P
[(100)] .49763 .30886 0 2 Mshowa
p
.002 w
.73454 .30886 m
.73454 .31511 L
s
P
[(150)] .73454 .30886 0 2 Mshowa
p
.002 w
.97145 .30886 m
.97145 .31511 L
s
P
[(200)] .97145 .30886 0 2 Mshowa
p
.001 w
.07119 .30886 m
.07119 .31261 L
s
P
p
.001 w
.11857 .30886 m
.11857 .31261 L
s
P
p
.001 w
.16596 .30886 m
.16596 .31261 L
s
P
p
.001 w
.21334 .30886 m
.21334 .31261 L
s
P
p
.001 w
.3081 .30886 m
.3081 .31261 L
s
P
p
.001 w
.35548 .30886 m
.35548 .31261 L
s
P
p
.001 w
.40287 .30886 m
.40287 .31261 L
s
P
p
.001 w
.45025 .30886 m
.45025 .31261 L
s
P
p
.001 w
.54501 .30886 m
.54501 .31261 L
s
P
p
.001 w
.5924 .30886 m
.5924 .31261 L
s
P
p
.001 w
.63978 .30886 m
.63978 .31261 L
s
P
p
.001 w
.68716 .30886 m
.68716 .31261 L
s
P
p
.001 w
.78192 .30886 m
.78192 .31261 L
s
P
p
.001 w
.82931 .30886 m
.82931 .31261 L
s
P
p
.001 w
.87669 .30886 m
.87669 .31261 L
s
P
p
.001 w
.92407 .30886 m
.92407 .31261 L
s
P
p
.002 w
0 .30886 m
1 .30886 L
s
P
p
.002 w
.02381 .05837 m
.03006 .05837 L
s
P
[(-0.06)] .01131 .05837 1 0 Mshowa
p
.002 w
.02381 .14187 m
.03006 .14187 L
s
P
[(-0.04)] .01131 .14187 1 0 Mshowa
p
.002 w
.02381 .22536 m
.03006 .22536 L
s
P
[(-0.02)] .01131 .22536 1 0 Mshowa
p
.002 w
.02381 .39235 m
.03006 .39235 L
s
P
[(0.02)] .01131 .39235 1 0 Mshowa
p
.002 w
.02381 .47584 m
.03006 .47584 L
s
P
[(0.04)] .01131 .47584 1 0 Mshowa
p
.002 w
.02381 .55934 m
.03006 .55934 L
s
P
[(0.06)] .01131 .55934 1 0 Mshowa
p
.001 w
.02381 .07507 m
.02756 .07507 L
s
P
p
.001 w
.02381 .09177 m
.02756 .09177 L
s
P
p
.001 w
.02381 .10847 m
.02756 .10847 L
s
P
p
.001 w
.02381 .12517 m
.02756 .12517 L
s
P
p
.001 w
.02381 .15856 m
.02756 .15856 L
s
P
p
.001 w
.02381 .17526 m
.02756 .17526 L
s
P
p
.001 w
.02381 .19196 m
.02756 .19196 L
s
P
p
.001 w
.02381 .20866 m
.02756 .20866 L
s
P
p
.001 w
.02381 .24206 m
.02756 .24206 L
s
P
p
.001 w
.02381 .25876 m
.02756 .25876 L
s
P
p
.001 w
.02381 .27546 m
.02756 .27546 L
s
P
p
.001 w
.02381 .29216 m
.02756 .29216 L
s
P
p
.001 w
.02381 .32555 m
.02756 .32555 L
s
P
p
.001 w
.02381 .34225 m
.02756 .34225 L
s
P
p
.001 w
.02381 .35895 m
.02756 .35895 L
s
P
p
.001 w
.02381 .37565 m
.02756 .37565 L
s
P
p
.001 w
.02381 .40905 m
.02756 .40905 L
s
P
p
.001 w
.02381 .42575 m
.02756 .42575 L
s
P
p
.001 w
.02381 .44245 m
.02756 .44245 L
s
P
p
.001 w
.02381 .45915 m
.02756 .45915 L
s
P
p
.001 w
.02381 .49254 m
.02756 .49254 L
s
P
p
.001 w
.02381 .50924 m
.02756 .50924 L
s
P
p
.001 w
.02381 .52594 m
.02756 .52594 L
s
P
p
.001 w
.02381 .54264 m
.02756 .54264 L
s
P
p
.001 w
.02381 .04167 m
.02756 .04167 L
s
P
p
.001 w
.02381 .02497 m
.02756 .02497 L
s
P
p
.001 w
.02381 .00827 m
.02756 .00827 L
s
P
p
.001 w
.02381 .57604 m
.02756 .57604 L
s
P
p
.001 w
.02381 .59274 m
.02756 .59274 L
s
P
p
.001 w
.02381 .60944 m
.02756 .60944 L
s
P
p
.002 w
.02381 0 m
.02381 .61803 L
s
P
P
0 0 m
1 0 L
1 .61803 L
0 .61803 L
closepath
clip
newpath
p
.008 w
.02855 .60332 Mdot
.03329 .60203 Mdot
.03802 .59815 Mdot
.04276 .59174 Mdot
.0475 .58284 Mdot
.05224 .57154 Mdot
.05698 .55792 Mdot
.06172 .54212 Mdot
.06645 .52426 Mdot
.07119 .50452 Mdot
.07593 .48305 Mdot
.08067 .46005 Mdot
.08541 .43573 Mdot
.09014 .41029 Mdot
.09488 .38396 Mdot
.09962 .35696 Mdot
.10436 .32955 Mdot
.1091 .30195 Mdot
.11384 .27442 Mdot
.11857 .24718 Mdot
.12331 .22049 Mdot
.12805 .19458 Mdot
.13279 .16967 Mdot
.13753 .14598 Mdot
.14226 .12372 Mdot
.147 .10309 Mdot
.15174 .08427 Mdot
.15648 .06742 Mdot
.16122 .0527 Mdot
.16596 .04022 Mdot
.17069 .03011 Mdot
.17543 .02245 Mdot
.18017 .0173 Mdot
.18491 .01472 Mdot
.18965 .01472 Mdot
.19439 .0173 Mdot
.19912 .02245 Mdot
.20386 .03011 Mdot
.2086 .04022 Mdot
.21334 .0527 Mdot
.21808 .06742 Mdot
.22281 .08427 Mdot
.22755 .10309 Mdot
.23229 .12372 Mdot
.23703 .14598 Mdot
.24177 .16967 Mdot
.24651 .19458 Mdot
.25124 .22049 Mdot
.25598 .24718 Mdot
.26072 .27442 Mdot
.26546 .30195 Mdot
.2702 .32955 Mdot
.27493 .35696 Mdot
.27967 .38396 Mdot
.28441 .41029 Mdot
.28915 .43573 Mdot
.29389 .46005 Mdot
.29863 .48305 Mdot
.30336 .50452 Mdot
.3081 .52426 Mdot
.31284 .54212 Mdot
.31758 .55792 Mdot
.32232 .57154 Mdot
.32706 .58284 Mdot
.33179 .59174 Mdot
.33653 .59815 Mdot
.34127 .60203 Mdot
.34601 .60332 Mdot
.35075 .60203 Mdot
.35548 .59815 Mdot
.36022 .59174 Mdot
.36496 .58284 Mdot
.3697 .57154 Mdot
.37444 .55792 Mdot
.37918 .54212 Mdot
.38391 .52426 Mdot
.38865 .50452 Mdot
.39339 .48305 Mdot
.39813 .46005 Mdot
.40287 .43573 Mdot
.4076 .41029 Mdot
.41234 .38396 Mdot
.41708 .35696 Mdot
.42182 .32955 Mdot
.42656 .30195 Mdot
.4313 .27442 Mdot
.43603 .24718 Mdot
.44077 .22049 Mdot
.44551 .19458 Mdot
.45025 .16967 Mdot
.45499 .14598 Mdot
.45973 .12372 Mdot
.46446 .10309 Mdot
.4692 .08427 Mdot
.47394 .06742 Mdot
.47868 .0527 Mdot
.48342 .04022 Mdot
.48815 .03011 Mdot
.49289 .02245 Mdot
.49763 .0173 Mdot
.50237 .01472 Mdot
.50711 .01472 Mdot
.51185 .0173 Mdot
.51658 .02245 Mdot
.52132 .03011 Mdot
.52606 .04022 Mdot
.5308 .0527 Mdot
.53554 .06742 Mdot
.54027 .08427 Mdot
.54501 .10309 Mdot
.54975 .12372 Mdot
.55449 .14598 Mdot
.55923 .16967 Mdot
.56397 .19458 Mdot
.5687 .22049 Mdot
.57344 .24718 Mdot
.57818 .27442 Mdot
.58292 .30195 Mdot
.58766 .32955 Mdot
.5924 .35696 Mdot
.59713 .38396 Mdot
.60187 .41029 Mdot
.60661 .43573 Mdot
.61135 .46005 Mdot
.61609 .48305 Mdot
.62082 .50452 Mdot
.62556 .52426 Mdot
.6303 .54212 Mdot
.63504 .55792 Mdot
.63978 .57154 Mdot
.64452 .58284 Mdot
.64925 .59174 Mdot
.65399 .59815 Mdot
.65873 .60203 Mdot
.66347 .60332 Mdot
.66821 .60203 Mdot
.67294 .59815 Mdot
.67768 .59174 Mdot
.68242 .58284 Mdot
.68716 .57154 Mdot
.6919 .55792 Mdot
.69664 .54212 Mdot
.70137 .52426 Mdot
.70611 .50452 Mdot
.71085 .48305 Mdot
.71559 .46005 Mdot
.72033 .43573 Mdot
.72507 .41029 Mdot
.7298 .38396 Mdot
.73454 .35696 Mdot
.73928 .32955 Mdot
.74402 .30195 Mdot
.74876 .27442 Mdot
.75349 .24718 Mdot
.75823 .22049 Mdot
.76297 .19458 Mdot
.76771 .16967 Mdot
.77245 .14598 Mdot
.77719 .12372 Mdot
.78192 .10309 Mdot
.78666 .08427 Mdot
.7914 .06742 Mdot
.79614 .0527 Mdot
.80088 .04022 Mdot
.80561 .03011 Mdot
.81035 .02245 Mdot
.81509 .0173 Mdot
.81983 .01472 Mdot
.82457 .01472 Mdot
.82931 .0173 Mdot
.83404 .02245 Mdot
.83878 .03011 Mdot
.84352 .04022 Mdot
.84826 .0527 Mdot
.853 .06742 Mdot
.85774 .08427 Mdot
.86247 .10309 Mdot
.86721 .12372 Mdot
.87195 .14598 Mdot
.87669 .16967 Mdot
.88143 .19458 Mdot
.88616 .22049 Mdot
.8909 .24718 Mdot
.89564 .27442 Mdot
.90038 .30195 Mdot
.90512 .32955 Mdot
.90986 .35696 Mdot
.91459 .38396 Mdot
.91933 .41029 Mdot
.92407 .43573 Mdot
.92881 .46005 Mdot
.93355 .48305 Mdot
.93828 .50452 Mdot
.94302 .52426 Mdot
.94776 .54212 Mdot
.9525 .55792 Mdot
.95724 .57154 Mdot
.96198 .58284 Mdot
.96671 .59174 Mdot
.97145 .59815 Mdot
.97619 .60203 Mdot
P
% End of Graphics
MathPictureEnd

:[font = output; output; inactive; preserveAspect; endGroup]
Graphics["<<>>"]
;[o]
-Graphics-
:[font = input; preserveAspect; startGroup]
Show[%,%%]
:[font = postscript; PostScript; formatAsPostScript; output; inactive; preserveAspect; pictureLeft = 34; pictureWidth = 282; pictureHeight = 174.25]
%!
%%Creator: Mathematica
%%AspectRatio: .61803 
MathPictureStart
%% Graphics
/Courier findfont 10  scalefont  setfont
% Scaling calculations
0.0238095 0.00473821 0.306225 0.0016258 [
[(50)] .26072 .30622 0 2 Msboxa
[(100)] .49763 .30622 0 2 Msboxa
[(150)] .73454 .30622 0 2 Msboxa
[(200)] .97145 .30622 0 2 Msboxa
[(-150)] .01131 .06236 1 0 Msboxa
[(-100)] .01131 .14364 1 0 Msboxa
[(-50)] .01131 .22493 1 0 Msboxa
[(50)] .01131 .38751 1 0 Msboxa
[(100)] .01131 .4688 1 0 Msboxa
[(150)] .01131 .55009 1 0 Msboxa
[ -0.001 -0.001 0 0 ]
[ 1.001 .61903 0 0 ]
] MathScale
% Start of Graphics
1 setlinecap
1 setlinejoin
newpath
[ ] 0 setdash
0 g
p
p
.002 w
.26072 .30622 m
.26072 .31247 L
s
P
[(50)] .26072 .30622 0 2 Mshowa
p
.002 w
.49763 .30622 m
.49763 .31247 L
s
P
[(100)] .49763 .30622 0 2 Mshowa
p
.002 w
.73454 .30622 m
.73454 .31247 L
s
P
[(150)] .73454 .30622 0 2 Mshowa
p
.002 w
.97145 .30622 m
.97145 .31247 L
s
P
[(200)] .97145 .30622 0 2 Mshowa
p
.001 w
.07119 .30622 m
.07119 .30997 L
s
P
p
.001 w
.11857 .30622 m
.11857 .30997 L
s
P
p
.001 w
.16596 .30622 m
.16596 .30997 L
s
P
p
.001 w
.21334 .30622 m
.21334 .30997 L
s
P
p
.001 w
.3081 .30622 m
.3081 .30997 L
s
P
p
.001 w
.35548 .30622 m
.35548 .30997 L
s
P
p
.001 w
.40287 .30622 m
.40287 .30997 L
s
P
p
.001 w
.45025 .30622 m
.45025 .30997 L
s
P
p
.001 w
.54501 .30622 m
.54501 .30997 L
s
P
p
.001 w
.5924 .30622 m
.5924 .30997 L
s
P
p
.001 w
.63978 .30622 m
.63978 .30997 L
s
P
p
.001 w
.68716 .30622 m
.68716 .30997 L
s
P
p
.001 w
.78192 .30622 m
.78192 .30997 L
s
P
p
.001 w
.82931 .30622 m
.82931 .30997 L
s
P
p
.001 w
.87669 .30622 m
.87669 .30997 L
s
P
p
.001 w
.92407 .30622 m
.92407 .30997 L
s
P
p
.002 w
0 .30622 m
1 .30622 L
s
P
p
.002 w
.02381 .06236 m
.03006 .06236 L
s
P
[(-150)] .01131 .06236 1 0 Mshowa
p
.002 w
.02381 .14364 m
.03006 .14364 L
s
P
[(-100)] .01131 .14364 1 0 Mshowa
p
.002 w
.02381 .22493 m
.03006 .22493 L
s
P
[(-50)] .01131 .22493 1 0 Mshowa
p
.002 w
.02381 .38751 m
.03006 .38751 L
s
P
[(50)] .01131 .38751 1 0 Mshowa
p
.002 w
.02381 .4688 m
.03006 .4688 L
s
P
[(100)] .01131 .4688 1 0 Mshowa
p
.002 w
.02381 .55009 m
.03006 .55009 L
s
P
[(150)] .01131 .55009 1 0 Mshowa
p
.001 w
.02381 .07861 m
.02756 .07861 L
s
P
p
.001 w
.02381 .09487 m
.02756 .09487 L
s
P
p
.001 w
.02381 .11113 m
.02756 .11113 L
s
P
p
.001 w
.02381 .12739 m
.02756 .12739 L
s
P
p
.001 w
.02381 .1599 m
.02756 .1599 L
s
P
p
.001 w
.02381 .17616 m
.02756 .17616 L
s
P
p
.001 w
.02381 .19242 m
.02756 .19242 L
s
P
p
.001 w
.02381 .20868 m
.02756 .20868 L
s
P
p
.001 w
.02381 .24119 m
.02756 .24119 L
s
P
p
.001 w
.02381 .25745 m
.02756 .25745 L
s
P
p
.001 w
.02381 .27371 m
.02756 .27371 L
s
P
p
.001 w
.02381 .28997 m
.02756 .28997 L
s
P
p
.001 w
.02381 .32248 m
.02756 .32248 L
s
P
p
.001 w
.02381 .33874 m
.02756 .33874 L
s
P
p
.001 w
.02381 .355 m
.02756 .355 L
s
P
p
.001 w
.02381 .37126 m
.02756 .37126 L
s
P
p
.001 w
.02381 .40377 m
.02756 .40377 L
s
P
p
.001 w
.02381 .42003 m
.02756 .42003 L
s
P
p
.001 w
.02381 .43629 m
.02756 .43629 L
s
P
p
.001 w
.02381 .45255 m
.02756 .45255 L
s
P
p
.001 w
.02381 .48506 m
.02756 .48506 L
s
P
p
.001 w
.02381 .50132 m
.02756 .50132 L
s
P
p
.001 w
.02381 .51758 m
.02756 .51758 L
s
P
p
.001 w
.02381 .53384 m
.02756 .53384 L
s
P
p
.001 w
.02381 .0461 m
.02756 .0461 L
s
P
p
.001 w
.02381 .02984 m
.02756 .02984 L
s
P
p
.001 w
.02381 .01358 m
.02756 .01358 L
s
P
p
.001 w
.02381 .56635 m
.02756 .56635 L
s
P
p
.001 w
.02381 .58261 m
.02756 .58261 L
s
P
p
.001 w
.02381 .59887 m
.02756 .59887 L
s
P
p
.001 w
.02381 .61513 m
.02756 .61513 L
s
P
p
.002 w
.02381 0 m
.02381 .61803 L
s
P
P
0 0 m
1 0 L
1 .61803 L
0 .61803 L
closepath
clip
newpath
p
p
.008 w
.02855 .15276 Mdot
.03329 .32149 Mdot
.03802 .30078 Mdot
.04276 .31076 Mdot
.0475 .3042 Mdot
.05224 .30908 Mdot
.05698 .3052 Mdot
.06172 .30842 Mdot
.06645 .30566 Mdot
.07119 .30808 Mdot
.07593 .30593 Mdot
.08067 .30786 Mdot
.08541 .30611 Mdot
.09014 .30771 Mdot
.09488 .30623 Mdot
.09962 .30761 Mdot
.10436 .30632 Mdot
.1091 .30753 Mdot
.11384 .30639 Mdot
.11857 .30747 Mdot
.12331 .30645 Mdot
.12805 .30742 Mdot
.13279 .30649 Mdot
.13753 .30738 Mdot
.14226 .30653 Mdot
.147 .30734 Mdot
.15174 .30656 Mdot
.15648 .30731 Mdot
.16122 .30659 Mdot
.16596 .30729 Mdot
.17069 .30661 Mdot
.17543 .30727 Mdot
.18017 .30663 Mdot
.18491 .30725 Mdot
.18965 .30665 Mdot
.19439 .30723 Mdot
.19912 .30666 Mdot
.20386 .30722 Mdot
.2086 .30668 Mdot
.21334 .30721 Mdot
.21808 .30669 Mdot
.22281 .3072 Mdot
.22755 .3067 Mdot
.23229 .30719 Mdot
.23703 .30671 Mdot
.24177 .30718 Mdot
.24651 .30672 Mdot
.25124 .30717 Mdot
.25598 .30673 Mdot
.26072 .30716 Mdot
.26546 .30673 Mdot
.2702 .30715 Mdot
.27493 .30674 Mdot
.27967 .30715 Mdot
.28441 .30675 Mdot
.28915 .30714 Mdot
.29389 .30675 Mdot
.29863 .30714 Mdot
.30336 .30676 Mdot
.3081 .30713 Mdot
.31284 .30676 Mdot
.31758 .30713 Mdot
.32232 .30676 Mdot
.32706 .30712 Mdot
.33179 .30677 Mdot
.33653 .30712 Mdot
.34127 .30677 Mdot
.34601 .30712 Mdot
.35075 .30678 Mdot
.35548 .30711 Mdot
.36022 .30678 Mdot
.36496 .30711 Mdot
.3697 .30678 Mdot
.37444 .30711 Mdot
.37918 .30678 Mdot
.38391 .30711 Mdot
.38865 .30679 Mdot
.39339 .3071 Mdot
.39813 .30679 Mdot
.40287 .3071 Mdot
.4076 .30679 Mdot
.41234 .3071 Mdot
.41708 .30679 Mdot
.42182 .3071 Mdot
.42656 .30679 Mdot
.4313 .3071 Mdot
.43603 .30679 Mdot
.44077 .3071 Mdot
.44551 .30679 Mdot
.45025 .3071 Mdot
.45499 .3068 Mdot
.45973 .3071 Mdot
.46446 .3068 Mdot
.4692 .30709 Mdot
.47394 .3068 Mdot
.47868 .30709 Mdot
.48342 .3068 Mdot
.48815 .30709 Mdot
.49289 .3068 Mdot
.49763 .30709 Mdot
.50237 .3068 Mdot
.50711 .30709 Mdot
.51185 .3068 Mdot
.51658 .30709 Mdot
.52132 .3068 Mdot
.52606 .30709 Mdot
.5308 .3068 Mdot
.53554 .30709 Mdot
.54027 .3068 Mdot
.54501 .30709 Mdot
.54975 .3068 Mdot
.55449 .3071 Mdot
.55923 .3068 Mdot
.56397 .3071 Mdot
.5687 .30679 Mdot
.57344 .3071 Mdot
.57818 .30679 Mdot
.58292 .3071 Mdot
.58766 .30679 Mdot
.5924 .3071 Mdot
.59713 .30679 Mdot
.60187 .3071 Mdot
.60661 .30679 Mdot
.61135 .3071 Mdot
.61609 .30679 Mdot
.62082 .3071 Mdot
.62556 .30679 Mdot
.6303 .30711 Mdot
.63504 .30678 Mdot
.63978 .30711 Mdot
.64452 .30678 Mdot
.64925 .30711 Mdot
.65399 .30678 Mdot
.65873 .30711 Mdot
.66347 .30678 Mdot
.66821 .30712 Mdot
.67294 .30677 Mdot
.67768 .30712 Mdot
.68242 .30677 Mdot
.68716 .30712 Mdot
.6919 .30677 Mdot
.69664 .30713 Mdot
.70137 .30676 Mdot
.70611 .30713 Mdot
.71085 .30676 Mdot
.71559 .30714 Mdot
.72033 .30675 Mdot
.72507 .30714 Mdot
.7298 .30675 Mdot
.73454 .30715 Mdot
.73928 .30674 Mdot
.74402 .30715 Mdot
.74876 .30673 Mdot
.75349 .30716 Mdot
.75823 .30673 Mdot
.76297 .30717 Mdot
.76771 .30672 Mdot
.77245 .30718 Mdot
.77719 .30671 Mdot
.78192 .30718 Mdot
.78666 .3067 Mdot
.7914 .30719 Mdot
.79614 .30669 Mdot
.80088 .30721 Mdot
.80561 .30668 Mdot
.81035 .30722 Mdot
.81509 .30667 Mdot
.81983 .30723 Mdot
.82457 .30665 Mdot
.82931 .30725 Mdot
.83404 .30663 Mdot
.83878 .30727 Mdot
.84352 .30661 Mdot
.84826 .30729 Mdot
.853 .30659 Mdot
.85774 .30731 Mdot
.86247 .30657 Mdot
.86721 .30734 Mdot
.87195 .30654 Mdot
.87669 .30737 Mdot
.88143 .3065 Mdot
.88616 .30741 Mdot
.8909 .30646 Mdot
.89564 .30746 Mdot
.90038 .3064 Mdot
.90512 .30752 Mdot
.90986 .30634 Mdot
.91459 .30759 Mdot
.91933 .30625 Mdot
.92407 .3077 Mdot
.92881 .30613 Mdot
.93355 .30783 Mdot
.93828 .30597 Mdot
.94302 .30803 Mdot
.94776 .30572 Mdot
.9525 .30835 Mdot
.95724 .3053 Mdot
.96198 .30891 Mdot
.96671 .3045 Mdot
.97145 .31007 Mdot
.97619 .30391 Mdot
P
p
.008 w
.02855 .27571 Mdot
.03329 .01472 Mdot
.03802 .11529 Mdot
.04276 .05915 Mdot
.0475 .1031 Mdot
.05224 .073 Mdot
.05698 .10181 Mdot
.06172 .08165 Mdot
.06645 .10342 Mdot
.07119 .08852 Mdot
.07593 .10621 Mdot
.08067 .09456 Mdot
.08541 .1096 Mdot
.09014 .10015 Mdot
.09488 .11333 Mdot
.09962 .10547 Mdot
.10436 .11727 Mdot
.1091 .11063 Mdot
.11384 .12136 Mdot
.11857 .11566 Mdot
.12331 .12555 Mdot
.12805 .1206 Mdot
.13279 .12981 Mdot
.13753 .12548 Mdot
.14226 .13413 Mdot
.147 .13032 Mdot
.15174 .13849 Mdot
.15648 .13511 Mdot
.16122 .14289 Mdot
.16596 .13988 Mdot
.17069 .14731 Mdot
.17543 .14462 Mdot
.18017 .15175 Mdot
.18491 .14935 Mdot
.18965 .15621 Mdot
.19439 .15405 Mdot
.19912 .16068 Mdot
.20386 .15875 Mdot
.2086 .16517 Mdot
.21334 .16343 Mdot
.21808 .16967 Mdot
.22281 .1681 Mdot
.22755 .17417 Mdot
.23229 .17276 Mdot
.23703 .17869 Mdot
.24177 .17742 Mdot
.24651 .18321 Mdot
.25124 .18207 Mdot
.25598 .18774 Mdot
.26072 .18671 Mdot
.26546 .19227 Mdot
.2702 .19135 Mdot
.27493 .19681 Mdot
.27967 .19598 Mdot
.28441 .20135 Mdot
.28915 .20061 Mdot
.29389 .2059 Mdot
.29863 .20524 Mdot
.30336 .21045 Mdot
.3081 .20986 Mdot
.31284 .215 Mdot
.31758 .21448 Mdot
.32232 .21956 Mdot
.32706 .21909 Mdot
.33179 .22412 Mdot
.33653 .22371 Mdot
.34127 .22868 Mdot
.34601 .22832 Mdot
.35075 .23324 Mdot
.35548 .23293 Mdot
.36022 .23781 Mdot
.36496 .23754 Mdot
.3697 .24237 Mdot
.37444 .24214 Mdot
.37918 .24694 Mdot
.38391 .24675 Mdot
.38865 .25151 Mdot
.39339 .25135 Mdot
.39813 .25608 Mdot
.40287 .25595 Mdot
.4076 .26066 Mdot
.41234 .26055 Mdot
.41708 .26523 Mdot
.42182 .26515 Mdot
.42656 .26981 Mdot
.4313 .26975 Mdot
.43603 .27439 Mdot
.44077 .27434 Mdot
.44551 .27897 Mdot
.45025 .27894 Mdot
.45499 .28355 Mdot
.45973 .28353 Mdot
.46446 .28813 Mdot
.4692 .28812 Mdot
.47394 .29271 Mdot
.47868 .29271 Mdot
.48342 .2973 Mdot
.48815 .2973 Mdot
.49289 .30188 Mdot
.49763 .30189 Mdot
.50237 .30647 Mdot
.50711 .30648 Mdot
.51185 .31106 Mdot
.51658 .31106 Mdot
.52132 .31564 Mdot
.52606 .31565 Mdot
.5308 .32023 Mdot
.53554 .32023 Mdot
.54027 .32482 Mdot
.54501 .32481 Mdot
.54975 .32942 Mdot
.55449 .3294 Mdot
.55923 .33401 Mdot
.56397 .33398 Mdot
.5687 .3386 Mdot
.57344 .33856 Mdot
.57818 .3432 Mdot
.58292 .34313 Mdot
.58766 .34779 Mdot
.5924 .34771 Mdot
.59713 .35239 Mdot
.60187 .35228 Mdot
.60661 .35699 Mdot
.61135 .35686 Mdot
.61609 .36159 Mdot
.62082 .36143 Mdot
.62556 .3662 Mdot
.6303 .366 Mdot
.63504 .3708 Mdot
.63978 .37057 Mdot
.64452 .37541 Mdot
.64925 .37514 Mdot
.65399 .38001 Mdot
.65873 .3797 Mdot
.66347 .38462 Mdot
.66821 .38427 Mdot
.67294 .38924 Mdot
.67768 .38883 Mdot
.68242 .39385 Mdot
.68716 .39339 Mdot
.6919 .39847 Mdot
.69664 .39794 Mdot
.70137 .40309 Mdot
.70611 .40249 Mdot
.71085 .40771 Mdot
.71559 .40704 Mdot
.72033 .41233 Mdot
.72507 .41159 Mdot
.7298 .41696 Mdot
.73454 .41613 Mdot
.73928 .4216 Mdot
.74402 .42067 Mdot
.74876 .42624 Mdot
.75349 .4252 Mdot
.75823 .43088 Mdot
.76297 .42973 Mdot
.76771 .43553 Mdot
.77245 .43425 Mdot
.77719 .44018 Mdot
.78192 .43877 Mdot
.78666 .44485 Mdot
.7914 .44328 Mdot
.79614 .44952 Mdot
.80088 .44777 Mdot
.80561 .4542 Mdot
.81035 .45226 Mdot
.81509 .45889 Mdot
.81983 .45673 Mdot
.82457 .4636 Mdot
.82931 .46119 Mdot
.83404 .46833 Mdot
.83878 .46563 Mdot
.84352 .47307 Mdot
.84826 .47005 Mdot
.853 .47784 Mdot
.85774 .47444 Mdot
.86247 .48263 Mdot
.86721 .4788 Mdot
.87195 .48747 Mdot
.87669 .48312 Mdot
.88143 .49235 Mdot
.88616 .48738 Mdot
.8909 .4973 Mdot
.89564 .49156 Mdot
.90038 .50234 Mdot
.90512 .49565 Mdot
.90986 .50749 Mdot
.91459 .49959 Mdot
.91933 .51283 Mdot
.92407 .50331 Mdot
.92881 .51843 Mdot
.93355 .50668 Mdot
.93828 .52449 Mdot
.94302 .50944 Mdot
.94776 .5314 Mdot
.9525 .51098 Mdot
.95724 .54016 Mdot
.96198 .5095 Mdot
.96671 .55439 Mdot
.97145 .49632 Mdot
.97619 .60332 Mdot
P
P
% End of Graphics
MathPictureEnd

:[font = output; output; inactive; preserveAspect; endGroup; endGroup; endGroup]
Graphics["<<>>"]
;[o]
-Graphics-
^*)