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
:[font = input; preserveAspect]
kr[n_]:=Sqrt[krho^2+(2 kD n + h)^2];
:[font = input; preserveAspect; startGroup]
Plot[
 Sum[ ((1-er)/(1+er))^n NIntegrate[E^(-I kr[n])/kr[n],{h,-kD,kD}],
{n,1,100}]//Re,{krho,0,1}]
:[font = postscript; PostScript; formatAsPostScript; output; inactive; preserveAspect; pictureLeft = 34; pictureWidth = 282; pictureHeight = 174.25]
%!
%%Creator: Mathematica
%%AspectRatio: .61803 
MathPictureStart
%% Graphics
/Courier findfont 10  scalefont  setfont
% Scaling calculations
0.0238095 0.952381 0.603319 1.03685 [
[(0.2)] .21429 .60332 0 2 Msboxa
[(0.4)] .40476 .60332 0 2 Msboxa
[(0.6)] .59524 .60332 0 2 Msboxa
[(0.8)] .78571 .60332 0 2 Msboxa
[(1)] .97619 .60332 0 2 Msboxa
[(-0.5)] .01131 .0849 1 0 Msboxa
[(-0.4)] .01131 .18858 1 0 Msboxa
[(-0.3)] .01131 .29227 1 0 Msboxa
[(-0.2)] .01131 .39595 1 0 Msboxa
[(-0.1)] .01131 .49963 1 0 Msboxa
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
.21429 .60332 m
.21429 .60957 L
s
P
[(0.2)] .21429 .60332 0 2 Mshowa
p
.002 w
.40476 .60332 m
.40476 .60957 L
s
P
[(0.4)] .40476 .60332 0 2 Mshowa
p
.002 w
.59524 .60332 m
.59524 .60957 L
s
P
[(0.6)] .59524 .60332 0 2 Mshowa
p
.002 w
.78571 .60332 m
.78571 .60957 L
s
P
[(0.8)] .78571 .60332 0 2 Mshowa
p
.002 w
.97619 .60332 m
.97619 .60957 L
s
P
[(1)] .97619 .60332 0 2 Mshowa
p
.001 w
.0619 .60332 m
.0619 .60707 L
s
P
p
.001 w
.1 .60332 m
.1 .60707 L
s
P
p
.001 w
.1381 .60332 m
.1381 .60707 L
s
P
p
.001 w
.17619 .60332 m
.17619 .60707 L
s
P
p
.001 w
.25238 .60332 m
.25238 .60707 L
s
P
p
.001 w
.29048 .60332 m
.29048 .60707 L
s
P
p
.001 w
.32857 .60332 m
.32857 .60707 L
s
P
p
.001 w
.36667 .60332 m
.36667 .60707 L
s
P
p
.001 w
.44286 .60332 m
.44286 .60707 L
s
P
p
.001 w
.48095 .60332 m
.48095 .60707 L
s
P
p
.001 w
.51905 .60332 m
.51905 .60707 L
s
P
p
.001 w
.55714 .60332 m
.55714 .60707 L
s
P
p
.001 w
.63333 .60332 m
.63333 .60707 L
s
P
p
.001 w
.67143 .60332 m
.67143 .60707 L
s
P
p
.001 w
.70952 .60332 m
.70952 .60707 L
s
P
p
.001 w
.74762 .60332 m
.74762 .60707 L
s
P
p
.001 w
.82381 .60332 m
.82381 .60707 L
s
P
p
.001 w
.8619 .60332 m
.8619 .60707 L
s
P
p
.001 w
.9 .60332 m
.9 .60707 L
s
P
p
.001 w
.9381 .60332 m
.9381 .60707 L
s
P
p
.002 w
0 .60332 m
1 .60332 L
s
P
p
.002 w
.02381 .0849 m
.03006 .0849 L
s
P
[(-0.5)] .01131 .0849 1 0 Mshowa
p
.002 w
.02381 .18858 m
.03006 .18858 L
s
P
[(-0.4)] .01131 .18858 1 0 Mshowa
p
.002 w
.02381 .29227 m
.03006 .29227 L
s
P
[(-0.3)] .01131 .29227 1 0 Mshowa
p
.002 w
.02381 .39595 m
.03006 .39595 L
s
P
[(-0.2)] .01131 .39595 1 0 Mshowa
p
.002 w
.02381 .49963 m
.03006 .49963 L
s
P
[(-0.1)] .01131 .49963 1 0 Mshowa
p
.001 w
.02381 .10563 m
.02756 .10563 L
s
P
p
.001 w
.02381 .12637 m
.02756 .12637 L
s
P
p
.001 w
.02381 .14711 m
.02756 .14711 L
s
P
p
.001 w
.02381 .16784 m
.02756 .16784 L
s
P
p
.001 w
.02381 .20932 m
.02756 .20932 L
s
P
p
.001 w
.02381 .23005 m
.02756 .23005 L
s
P
p
.001 w
.02381 .25079 m
.02756 .25079 L
s
P
p
.001 w
.02381 .27153 m
.02756 .27153 L
s
P
p
.001 w
.02381 .313 m
.02756 .313 L
s
P
p
.001 w
.02381 .33374 m
.02756 .33374 L
s
P
p
.001 w
.02381 .35448 m
.02756 .35448 L
s
P
p
.001 w
.02381 .37521 m
.02756 .37521 L
s
P
p
.001 w
.02381 .41669 m
.02756 .41669 L
s
P
p
.001 w
.02381 .43742 m
.02756 .43742 L
s
P
p
.001 w
.02381 .45816 m
.02756 .45816 L
s
P
p
.001 w
.02381 .4789 m
.02756 .4789 L
s
P
p
.001 w
.02381 .52037 m
.02756 .52037 L
s
P
p
.001 w
.02381 .54111 m
.02756 .54111 L
s
P
p
.001 w
.02381 .56185 m
.02756 .56185 L
s
P
p
.001 w
.02381 .58258 m
.02756 .58258 L
s
P
p
.001 w
.02381 .06416 m
.02756 .06416 L
s
P
p
.001 w
.02381 .04342 m
.02756 .04342 L
s
P
p
.001 w
.02381 .02269 m
.02756 .02269 L
s
P
p
.001 w
.02381 .00195 m
.02756 .00195 L
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
.004 w
.02381 .01472 m
.02505 .01474 L
.02629 .01482 L
.02753 .01494 L
.02877 .01512 L
.03001 .01534 L
.03125 .01562 L
.03373 .01631 L
.03869 .01829 L
.04365 .02103 L
.05357 .0286 L
.06349 .03867 L
.10317 .09516 L
.14286 .16007 L
.18254 .22045 L
.22222 .27237 L
.2619 .31573 L
.30159 .35166 L
.34127 .38151 L
.38095 .40649 L
.42063 .42761 L
.46032 .44565 L
.5 .46123 L
.53968 .47483 L
.57937 .4868 L
.61905 .49745 L
.65873 .507 L
.69841 .51561 L
.7381 .52344 L
.77778 .53059 L
.81746 .53717 L
.85714 .54324 L
.89683 .54886 L
.93651 .55409 L
.97619 .55898 L
s
P
P
% End of Graphics
MathPictureEnd

:[font = output; output; inactive; preserveAspect; endGroup]
Graphics["<<>>"]
;[o]
-Graphics-
:[font = input; preserveAspect; startGroup]
Plot[
 Sum[ ((1-er)/(1+er))^n NIntegrate[E^(-I kr[n])/kr[n],{h,-kD,kD}],
{n,1,100}]//Im,{krho,0,1}]
:[font = postscript; PostScript; formatAsPostScript; output; inactive; preserveAspect; pictureLeft = 34; pictureWidth = 282; pictureHeight = 174.25]
%!
%%Creator: Mathematica
%%AspectRatio: .61803 
MathPictureStart
%% Graphics
/Courier findfont 10  scalefont  setfont
% Scaling calculations
0.0238095 0.952381 -3.10761 46.4497 [
[(0.2)] .21429 .14387 0 2 Msboxa
[(0.4)] .40476 .14387 0 2 Msboxa
[(0.6)] .59524 .14387 0 2 Msboxa
[(0.8)] .78571 .14387 0 2 Msboxa
[(1)] .97619 .14387 0 2 Msboxa
[(0.068)] .01131 .05097 1 0 Msboxa
[(0.072)] .01131 .23677 1 0 Msboxa
[(0.074)] .01131 .32967 1 0 Msboxa
[(0.076)] .01131 .42256 1 0 Msboxa
[(0.078)] .01131 .51546 1 0 Msboxa
[(0.08)] .01131 .60836 1 0 Msboxa
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
.21429 .14387 m
.21429 .15012 L
s
P
[(0.2)] .21429 .14387 0 2 Mshowa
p
.002 w
.40476 .14387 m
.40476 .15012 L
s
P
[(0.4)] .40476 .14387 0 2 Mshowa
p
.002 w
.59524 .14387 m
.59524 .15012 L
s
P
[(0.6)] .59524 .14387 0 2 Mshowa
p
.002 w
.78571 .14387 m
.78571 .15012 L
s
P
[(0.8)] .78571 .14387 0 2 Mshowa
p
.002 w
.97619 .14387 m
.97619 .15012 L
s
P
[(1)] .97619 .14387 0 2 Mshowa
p
.001 w
.0619 .14387 m
.0619 .14762 L
s
P
p
.001 w
.1 .14387 m
.1 .14762 L
s
P
p
.001 w
.1381 .14387 m
.1381 .14762 L
s
P
p
.001 w
.17619 .14387 m
.17619 .14762 L
s
P
p
.001 w
.25238 .14387 m
.25238 .14762 L
s
P
p
.001 w
.29048 .14387 m
.29048 .14762 L
s
P
p
.001 w
.32857 .14387 m
.32857 .14762 L
s
P
p
.001 w
.36667 .14387 m
.36667 .14762 L
s
P
p
.001 w
.44286 .14387 m
.44286 .14762 L
s
P
p
.001 w
.48095 .14387 m
.48095 .14762 L
s
P
p
.001 w
.51905 .14387 m
.51905 .14762 L
s
P
p
.001 w
.55714 .14387 m
.55714 .14762 L
s
P
p
.001 w
.63333 .14387 m
.63333 .14762 L
s
P
p
.001 w
.67143 .14387 m
.67143 .14762 L
s
P
p
.001 w
.70952 .14387 m
.70952 .14762 L
s
P
p
.001 w
.74762 .14387 m
.74762 .14762 L
s
P
p
.001 w
.82381 .14387 m
.82381 .14762 L
s
P
p
.001 w
.8619 .14387 m
.8619 .14762 L
s
P
p
.001 w
.9 .14387 m
.9 .14762 L
s
P
p
.001 w
.9381 .14387 m
.9381 .14762 L
s
P
p
.002 w
0 .14387 m
1 .14387 L
s
P
p
.002 w
.02381 .05097 m
.03006 .05097 L
s
P
[(0.068)] .01131 .05097 1 0 Mshowa
p
.002 w
.02381 .23677 m
.03006 .23677 L
s
P
[(0.072)] .01131 .23677 1 0 Mshowa
p
.002 w
.02381 .32967 m
.03006 .32967 L
s
P
[(0.074)] .01131 .32967 1 0 Mshowa
p
.002 w
.02381 .42256 m
.03006 .42256 L
s
P
[(0.076)] .01131 .42256 1 0 Mshowa
p
.002 w
.02381 .51546 m
.03006 .51546 L
s
P
[(0.078)] .01131 .51546 1 0 Mshowa
p
.002 w
.02381 .60836 m
.03006 .60836 L
s
P
[(0.08)] .01131 .60836 1 0 Mshowa
p
.001 w
.02381 .06955 m
.02756 .06955 L
s
P
p
.001 w
.02381 .08813 m
.02756 .08813 L
s
P
p
.001 w
.02381 .10671 m
.02756 .10671 L
s
P
p
.001 w
.02381 .12529 m
.02756 .12529 L
s
P
p
.001 w
.02381 .16245 m
.02756 .16245 L
s
P
p
.001 w
.02381 .18103 m
.02756 .18103 L
s
P
p
.001 w
.02381 .19961 m
.02756 .19961 L
s
P
p
.001 w
.02381 .21819 m
.02756 .21819 L
s
P
p
.001 w
.02381 .25535 m
.02756 .25535 L
s
P
p
.001 w
.02381 .27393 m
.02756 .27393 L
s
P
p
.001 w
.02381 .29251 m
.02756 .29251 L
s
P
p
.001 w
.02381 .31109 m
.02756 .31109 L
s
P
p
.001 w
.02381 .34825 m
.02756 .34825 L
s
P
p
.001 w
.02381 .36683 m
.02756 .36683 L
s
P
p
.001 w
.02381 .38541 m
.02756 .38541 L
s
P
p
.001 w
.02381 .40398 m
.02756 .40398 L
s
P
p
.001 w
.02381 .44114 m
.02756 .44114 L
s
P
p
.001 w
.02381 .45972 m
.02756 .45972 L
s
P
p
.001 w
.02381 .4783 m
.02756 .4783 L
s
P
p
.001 w
.02381 .49688 m
.02756 .49688 L
s
P
p
.001 w
.02381 .53404 m
.02756 .53404 L
s
P
p
.001 w
.02381 .55262 m
.02756 .55262 L
s
P
p
.001 w
.02381 .5712 m
.02756 .5712 L
s
P
p
.001 w
.02381 .58978 m
.02756 .58978 L
s
P
p
.001 w
.02381 .03239 m
.02756 .03239 L
s
P
p
.001 w
.02381 .01381 m
.02756 .01381 L
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
.004 w
.02381 .60332 m
.02505 .60332 L
.02629 .60331 L
.02753 .60331 L
.02877 .6033 L
.03001 .60329 L
.03125 .60328 L
.03373 .60325 L
.03621 .60321 L
.03869 .60317 L
.04365 .60305 L
.04861 .6029 L
.05357 .60271 L
.06349 .60224 L
.07341 .60164 L
.08333 .6009 L
.10317 .59902 L
.12302 .59661 L
.14286 .59366 L
.18254 .58615 L
.22222 .57652 L
.2619 .56476 L
.30159 .5509 L
.34127 .53494 L
.38095 .51691 L
.42063 .49681 L
.46032 .47468 L
.5 .45054 L
.53968 .4244 L
.57937 .3963 L
.61905 .36627 L
.65873 .33433 L
.69841 .30053 L
.7381 .26489 L
.77778 .22746 L
.81746 .18826 L
.85714 .14734 L
.89683 .10475 L
.93651 .06053 L
.97619 .01472 L
s
P
P
% End of Graphics
MathPictureEnd

:[font = output; output; inactive; preserveAspect; endGroup]
Graphics["<<>>"]
;[o]
-Graphics-
:[font = input; preserveAspect; startGroup]
Plot[Sqrt[1/x^2+1]-1,{x,1,10}]
:[font = postscript; PostScript; formatAsPostScript; output; inactive; preserveAspect; pictureLeft = 34; pictureWidth = 282; pictureHeight = 174.25]
%!
%%Creator: Mathematica
%%AspectRatio: .61803 
MathPictureStart
%% Graphics
/Courier findfont 10  scalefont  setfont
% Scaling calculations
0.0238095 0.0952381 0.0147151 1.42102 [
[(2)] .21429 .01472 0 2 Msboxa
[(4)] .40476 .01472 0 2 Msboxa
[(6)] .59524 .01472 0 2 Msboxa
[(8)] .78571 .01472 0 2 Msboxa
[(10)] .97619 .01472 0 2 Msboxa
[(0.1)] .01131 .15682 1 0 Msboxa
[(0.2)] .01131 .29892 1 0 Msboxa
[(0.3)] .01131 .44102 1 0 Msboxa
[(0.4)] .01131 .58312 1 0 Msboxa
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
.21429 .01472 m
.21429 .02097 L
s
P
[(2)] .21429 .01472 0 2 Mshowa
p
.002 w
.40476 .01472 m
.40476 .02097 L
s
P
[(4)] .40476 .01472 0 2 Mshowa
p
.002 w
.59524 .01472 m
.59524 .02097 L
s
P
[(6)] .59524 .01472 0 2 Mshowa
p
.002 w
.78571 .01472 m
.78571 .02097 L
s
P
[(8)] .78571 .01472 0 2 Mshowa
p
.002 w
.97619 .01472 m
.97619 .02097 L
s
P
[(10)] .97619 .01472 0 2 Mshowa
p
.001 w
.0619 .01472 m
.0619 .01847 L
s
P
p
.001 w
.1 .01472 m
.1 .01847 L
s
P
p
.001 w
.1381 .01472 m
.1381 .01847 L
s
P
p
.001 w
.17619 .01472 m
.17619 .01847 L
s
P
p
.001 w
.25238 .01472 m
.25238 .01847 L
s
P
p
.001 w
.29048 .01472 m
.29048 .01847 L
s
P
p
.001 w
.32857 .01472 m
.32857 .01847 L
s
P
p
.001 w
.36667 .01472 m
.36667 .01847 L
s
P
p
.001 w
.44286 .01472 m
.44286 .01847 L
s
P
p
.001 w
.48095 .01472 m
.48095 .01847 L
s
P
p
.001 w
.51905 .01472 m
.51905 .01847 L
s
P
p
.001 w
.55714 .01472 m
.55714 .01847 L
s
P
p
.001 w
.63333 .01472 m
.63333 .01847 L
s
P
p
.001 w
.67143 .01472 m
.67143 .01847 L
s
P
p
.001 w
.70952 .01472 m
.70952 .01847 L
s
P
p
.001 w
.74762 .01472 m
.74762 .01847 L
s
P
p
.001 w
.82381 .01472 m
.82381 .01847 L
s
P
p
.001 w
.8619 .01472 m
.8619 .01847 L
s
P
p
.001 w
.9 .01472 m
.9 .01847 L
s
P
p
.001 w
.9381 .01472 m
.9381 .01847 L
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
.02381 .15682 m
.03006 .15682 L
s
P
[(0.1)] .01131 .15682 1 0 Mshowa
p
.002 w
.02381 .29892 m
.03006 .29892 L
s
P
[(0.2)] .01131 .29892 1 0 Mshowa
p
.002 w
.02381 .44102 m
.03006 .44102 L
s
P
[(0.3)] .01131 .44102 1 0 Mshowa
p
.002 w
.02381 .58312 m
.03006 .58312 L
s
P
[(0.4)] .01131 .58312 1 0 Mshowa
p
.001 w
.02381 .04314 m
.02756 .04314 L
s
P
p
.001 w
.02381 .07156 m
.02756 .07156 L
s
P
p
.001 w
.02381 .09998 m
.02756 .09998 L
s
P
p
.001 w
.02381 .1284 m
.02756 .1284 L
s
P
p
.001 w
.02381 .18524 m
.02756 .18524 L
s
P
p
.001 w
.02381 .21366 m
.02756 .21366 L
s
P
p
.001 w
.02381 .24208 m
.02756 .24208 L
s
P
p
.001 w
.02381 .2705 m
.02756 .2705 L
s
P
p
.001 w
.02381 .32734 m
.02756 .32734 L
s
P
p
.001 w
.02381 .35576 m
.02756 .35576 L
s
P
p
.001 w
.02381 .38418 m
.02756 .38418 L
s
P
p
.001 w
.02381 .4126 m
.02756 .4126 L
s
P
p
.001 w
.02381 .46944 m
.02756 .46944 L
s
P
p
.001 w
.02381 .49786 m
.02756 .49786 L
s
P
p
.001 w
.02381 .52628 m
.02756 .52628 L
s
P
p
.001 w
.02381 .5547 m
.02756 .5547 L
s
P
p
.001 w
.02381 .61154 m
.02756 .61154 L
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
.004 w
.11905 .60332 m
.12798 .51912 L
.1369 .45145 L
.14583 .3963 L
.15476 .35078 L
.17262 .28082 L
.19048 .23036 L
.20833 .19282 L
.22619 .1642 L
.24405 .14189 L
.2619 .12418 L
.27976 .1099 L
.29762 .09822 L
.33333 .08046 L
.36905 .06779 L
.40476 .05845 L
.44048 .05136 L
.47619 .04586 L
.5119 .04151 L
.54762 .03801 L
.58333 .03515 L
.61905 .03279 L
.65476 .03081 L
.69048 .02914 L
.72619 .02772 L
.7619 .0265 L
.79762 .02544 L
.83333 .02452 L
.86905 .02371 L
.90476 .02299 L
.94048 .02236 L
.97619 .0218 L
s
P
P
% End of Graphics
MathPictureEnd

:[font = output; output; inactive; preserveAspect; endGroup]
Graphics["<<>>"]
;[o]
-Graphics-
:[font = input; preserveAspect; startGroup]
krho=.
:[font = output; output; inactive; preserveAspect; endGroup]
0.1
;[o]
0.1
:[font = input; preserveAspect; startGroup]
NIntegrate[E^(-I kr[1])/kr[1],{h,-kD,kD}]
:[font = output; output; inactive; preserveAspect; endGroup]
1.078695453686136 - 0.1985595826987662*I
;[o]
1.0787 - 0.19856 I
:[font = input; preserveAspect; startGroup]
E^(-I kr[1])/kr[1] 2 kD/.h->0//N
:[font = output; output; inactive; preserveAspect; endGroup]
0.980066577841242 - 0.1986693307950612*I
;[o]
0.980067 - 0.198669 I
:[font = input; preserveAspect; startGroup]
Plot[2 kD Sum[qu^n E^(-I kr[n])/kr[n],{n,1,100}]
  /.qu->(1-er)/(1+er)
  /.h->0//Re//Evaluate,
  {krho,0,1}]
:[font = postscript; PostScript; formatAsPostScript; output; inactive; preserveAspect; pictureLeft = 34; pictureWidth = 282; pictureHeight = 174.25]
%!
%%Creator: Mathematica
%%AspectRatio: .61803 
MathPictureStart
%% Graphics
/Courier findfont 10  scalefont  setfont
% Scaling calculations
0.0238095 0.952381 0.603319 1.16321 [
[(0.2)] .21429 .60332 0 2 Msboxa
[(0.4)] .40476 .60332 0 2 Msboxa
[(0.6)] .59524 .60332 0 2 Msboxa
[(0.8)] .78571 .60332 0 2 Msboxa
[(1)] .97619 .60332 0 2 Msboxa
[(-0.5)] .01131 .02172 1 0 Msboxa
[(-0.4)] .01131 .13804 1 0 Msboxa
[(-0.3)] .01131 .25436 1 0 Msboxa
[(-0.2)] .01131 .37068 1 0 Msboxa
[(-0.1)] .01131 .487 1 0 Msboxa
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
.21429 .60332 m
.21429 .60957 L
s
P
[(0.2)] .21429 .60332 0 2 Mshowa
p
.002 w
.40476 .60332 m
.40476 .60957 L
s
P
[(0.4)] .40476 .60332 0 2 Mshowa
p
.002 w
.59524 .60332 m
.59524 .60957 L
s
P
[(0.6)] .59524 .60332 0 2 Mshowa
p
.002 w
.78571 .60332 m
.78571 .60957 L
s
P
[(0.8)] .78571 .60332 0 2 Mshowa
p
.002 w
.97619 .60332 m
.97619 .60957 L
s
P
[(1)] .97619 .60332 0 2 Mshowa
p
.001 w
.0619 .60332 m
.0619 .60707 L
s
P
p
.001 w
.1 .60332 m
.1 .60707 L
s
P
p
.001 w
.1381 .60332 m
.1381 .60707 L
s
P
p
.001 w
.17619 .60332 m
.17619 .60707 L
s
P
p
.001 w
.25238 .60332 m
.25238 .60707 L
s
P
p
.001 w
.29048 .60332 m
.29048 .60707 L
s
P
p
.001 w
.32857 .60332 m
.32857 .60707 L
s
P
p
.001 w
.36667 .60332 m
.36667 .60707 L
s
P
p
.001 w
.44286 .60332 m
.44286 .60707 L
s
P
p
.001 w
.48095 .60332 m
.48095 .60707 L
s
P
p
.001 w
.51905 .60332 m
.51905 .60707 L
s
P
p
.001 w
.55714 .60332 m
.55714 .60707 L
s
P
p
.001 w
.63333 .60332 m
.63333 .60707 L
s
P
p
.001 w
.67143 .60332 m
.67143 .60707 L
s
P
p
.001 w
.70952 .60332 m
.70952 .60707 L
s
P
p
.001 w
.74762 .60332 m
.74762 .60707 L
s
P
p
.001 w
.82381 .60332 m
.82381 .60707 L
s
P
p
.001 w
.8619 .60332 m
.8619 .60707 L
s
P
p
.001 w
.9 .60332 m
.9 .60707 L
s
P
p
.001 w
.9381 .60332 m
.9381 .60707 L
s
P
p
.002 w
0 .60332 m
1 .60332 L
s
P
p
.002 w
.02381 .02172 m
.03006 .02172 L
s
P
[(-0.5)] .01131 .02172 1 0 Mshowa
p
.002 w
.02381 .13804 m
.03006 .13804 L
s
P
[(-0.4)] .01131 .13804 1 0 Mshowa
p
.002 w
.02381 .25436 m
.03006 .25436 L
s
P
[(-0.3)] .01131 .25436 1 0 Mshowa
p
.002 w
.02381 .37068 m
.03006 .37068 L
s
P
[(-0.2)] .01131 .37068 1 0 Mshowa
p
.002 w
.02381 .487 m
.03006 .487 L
s
P
[(-0.1)] .01131 .487 1 0 Mshowa
p
.001 w
.02381 .04498 m
.02756 .04498 L
s
P
p
.001 w
.02381 .06824 m
.02756 .06824 L
s
P
p
.001 w
.02381 .09151 m
.02756 .09151 L
s
P
p
.001 w
.02381 .11477 m
.02756 .11477 L
s
P
p
.001 w
.02381 .1613 m
.02756 .1613 L
s
P
p
.001 w
.02381 .18456 m
.02756 .18456 L
s
P
p
.001 w
.02381 .20783 m
.02756 .20783 L
s
P
p
.001 w
.02381 .23109 m
.02756 .23109 L
s
P
p
.001 w
.02381 .27762 m
.02756 .27762 L
s
P
p
.001 w
.02381 .30089 m
.02756 .30089 L
s
P
p
.001 w
.02381 .32415 m
.02756 .32415 L
s
P
p
.001 w
.02381 .34741 m
.02756 .34741 L
s
P
p
.001 w
.02381 .39394 m
.02756 .39394 L
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
.02381 .44047 m
.02756 .44047 L
s
P
p
.001 w
.02381 .46373 m
.02756 .46373 L
s
P
p
.001 w
.02381 .51026 m
.02756 .51026 L
s
P
p
.001 w
.02381 .53353 m
.02756 .53353 L
s
P
p
.001 w
.02381 .55679 m
.02756 .55679 L
s
P
p
.001 w
.02381 .58005 m
.02756 .58005 L
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
.004 w
.02381 .01472 m
.02505 .01473 L
.02629 .01478 L
.02753 .01485 L
.02877 .01496 L
.03001 .0151 L
.03125 .01527 L
.03373 .01571 L
.03621 .01626 L
.03869 .01694 L
.04365 .01866 L
.05357 .02349 L
.06349 .03009 L
.08333 .04796 L
.10317 .07077 L
.14286 .1248 L
.18254 .18131 L
.22222 .2339 L
.2619 .28004 L
.30159 .31941 L
.34127 .35269 L
.38095 .3808 L
.42063 .40468 L
.46032 .42514 L
.5 .44281 L
.53968 .45823 L
.57937 .47182 L
.61905 .48388 L
.65873 .49468 L
.69841 .50443 L
.7381 .51327 L
.77778 .52135 L
.81746 .52877 L
.85714 .53562 L
.89683 .54196 L
.93651 .54786 L
.97619 .55337 L
s
P
P
% End of Graphics
MathPictureEnd

:[font = output; output; inactive; preserveAspect; endGroup]
Graphics["<<>>"]
;[o]
-Graphics-
:[font = input; preserveAspect; startGroup]
 Sum[qu^n NIntegrate[E^(-I kr[n])/kr[n],{h,-kD,kD}],{n,1,100}]\
  /.qu->(1-er)/(1+er)
:[font = output; output; inactive; preserveAspect; endGroup]
-0.04827776529116474 + 0.07565732094552065*I
;[o]
-0.0482778 + 0.0756573 I
:[font = input; preserveAspect; startGroup]
NIntegrate[E^(-I kr[0])/kr[0],{h,0,kD}]
:[font = output; output; inactive; preserveAspect; endGroup]
0.05380083137182458 - 0.0840969192070492*I
;[o]
0.0538008 - 0.0840969 I
:[font = input; preserveAspect; startGroup]
2(%+%%)
:[font = output; output; inactive; preserveAspect; endGroup]
0.01104613216131968 - 0.016879196523057*I
;[o]
0.0110461 - 0.0168792 I
:[font = input; preserveAspect; startGroup]
% 2
:[font = output; output; inactive; preserveAspect; endGroup]
1.623185234120998 - 0.02004674097806783*I
;[o]
1.62319 - 0.0200467 I
:[font = input; preserveAspect; startGroup]
Integer[1.2]
:[font = output; output; inactive; preserveAspect; endGroup]
Integer[1.2]
;[o]
Integer[1.2]
:[font = input; preserveAspect; startGroup]
Round[1.2]
:[font = output; output; inactive; preserveAspect; endGroup]
1
;[o]
1
:[font = input; preserveAspect; startGroup]
Sum[1,{x,1,1.2}]
:[font = output; output; inactive; preserveAspect; endGroup]
1
;[o]
1
:[font = input; preserveAspect; startGroup]
NIntegrate[ E^((Log[(er-1)/(er+1)]+ I (Pi -2 kD))n)/n,
 {n,10,1000}]
:[font = output; output; inactive; preserveAspect; endGroup]
0.0004793908764614196 - 0.0003272255032819209*I
;[o]
0.000479391 - 0.000327226 I
:[font = input; preserveAspect; startGroup]
Log[1-(1-er)/(er+1)E^(-I 2 kD)]
:[font = output; output; inactive; preserveAspect; endGroup]
0.5060185685439152 - 0.07993575846245338*I
;[o]
0.506019 - 0.0799358 I
:[font = input; preserveAspect; startGroup]
Sum[ ((1-er)/(1+er))^n E^(-I 2 kD n)/n,
{n,1,1000}]
:[font = output; output; inactive; preserveAspect; endGroup]
-0.5060185685439154 + 0.07993575846245341*I
;[o]
-0.506019 + 0.0799358 I
:[font = input; preserveAspect; startGroup]
Integrate[1/Sqrt[x^2+y^2+h^2],x]
:[font = output; output; inactive; preserveAspect; endGroup]
Log[x + (h^2 + x^2 + y^2)^(1/2)]
;[o]
              2    2    2
Log[x + Sqrt[h  + x  + y ]]
:[font = input; preserveAspect; startGroup]
Integrate[%,h]
:[font = output; output; inactive; preserveAspect; endGroup]
-h + y*ArcTan[h/y] - y*ArcTan[(h*x)/
 
     (y*(h^2 + x^2 + y^2)^(1/2))] + 
 
  x*Log[h + (h^2 + x^2 + y^2)^(1/2)] + 
 
  h*Log[x + (h^2 + x^2 + y^2)^(1/2)]
;[o]
              h                     h x
-h + y ArcTan[-] - y ArcTan[--------------------] + 
              y                     2    2    2
                            y Sqrt[h  + x  + y ]
 
                  2    2    2                     2    2    2
  x Log[h + Sqrt[h  + x  + y ]] + h Log[x + Sqrt[h  + x  + y ]]
:[font = input; preserveAspect; startGroup]
Plot[x /2 Log[1+2/(Sqrt[1+x^2]-1)]-1,{x,1,10}]
:[font = postscript; PostScript; formatAsPostScript; output; inactive; preserveAspect; pictureLeft = 34; pictureWidth = 282; pictureHeight = 174.25]
%!
%%Creator: Mathematica
%%AspectRatio: .61803 
MathPictureStart
%% Graphics
/Courier findfont 10  scalefont  setfont
% Scaling calculations
0.0238095 0.0952381 0.603319 4.96183 [
[(2)] .21429 .60332 0 2 Msboxa
[(4)] .40476 .60332 0 2 Msboxa
[(6)] .59524 .60332 0 2 Msboxa
[(8)] .78571 .60332 0 2 Msboxa
[(10)] .97619 .60332 0 2 Msboxa
[(-0.12)] .01131 .0079 1 0 Msboxa
[(-0.1)] .01131 .10714 1 0 Msboxa
[(-0.08)] .01131 .20637 1 0 Msboxa
[(-0.06)] .01131 .30561 1 0 Msboxa
[(-0.04)] .01131 .40485 1 0 Msboxa
[(-0.02)] .01131 .50408 1 0 Msboxa
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
.21429 .60332 m
.21429 .60957 L
s
P
[(2)] .21429 .60332 0 2 Mshowa
p
.002 w
.40476 .60332 m
.40476 .60957 L
s
P
[(4)] .40476 .60332 0 2 Mshowa
p
.002 w
.59524 .60332 m
.59524 .60957 L
s
P
[(6)] .59524 .60332 0 2 Mshowa
p
.002 w
.78571 .60332 m
.78571 .60957 L
s
P
[(8)] .78571 .60332 0 2 Mshowa
p
.002 w
.97619 .60332 m
.97619 .60957 L
s
P
[(10)] .97619 .60332 0 2 Mshowa
p
.001 w
.0619 .60332 m
.0619 .60707 L
s
P
p
.001 w
.1 .60332 m
.1 .60707 L
s
P
p
.001 w
.1381 .60332 m
.1381 .60707 L
s
P
p
.001 w
.17619 .60332 m
.17619 .60707 L
s
P
p
.001 w
.25238 .60332 m
.25238 .60707 L
s
P
p
.001 w
.29048 .60332 m
.29048 .60707 L
s
P
p
.001 w
.32857 .60332 m
.32857 .60707 L
s
P
p
.001 w
.36667 .60332 m
.36667 .60707 L
s
P
p
.001 w
.44286 .60332 m
.44286 .60707 L
s
P
p
.001 w
.48095 .60332 m
.48095 .60707 L
s
P
p
.001 w
.51905 .60332 m
.51905 .60707 L
s
P
p
.001 w
.55714 .60332 m
.55714 .60707 L
s
P
p
.001 w
.63333 .60332 m
.63333 .60707 L
s
P
p
.001 w
.67143 .60332 m
.67143 .60707 L
s
P
p
.001 w
.70952 .60332 m
.70952 .60707 L
s
P
p
.001 w
.74762 .60332 m
.74762 .60707 L
s
P
p
.001 w
.82381 .60332 m
.82381 .60707 L
s
P
p
.001 w
.8619 .60332 m
.8619 .60707 L
s
P
p
.001 w
.9 .60332 m
.9 .60707 L
s
P
p
.001 w
.9381 .60332 m
.9381 .60707 L
s
P
p
.002 w
0 .60332 m
1 .60332 L
s
P
p
.002 w
.02381 .0079 m
.03006 .0079 L
s
P
[(-0.12)] .01131 .0079 1 0 Mshowa
p
.002 w
.02381 .10714 m
.03006 .10714 L
s
P
[(-0.1)] .01131 .10714 1 0 Mshowa
p
.002 w
.02381 .20637 m
.03006 .20637 L
s
P
[(-0.08)] .01131 .20637 1 0 Mshowa
p
.002 w
.02381 .30561 m
.03006 .30561 L
s
P
[(-0.06)] .01131 .30561 1 0 Mshowa
p
.002 w
.02381 .40485 m
.03006 .40485 L
s
P
[(-0.04)] .01131 .40485 1 0 Mshowa
p
.002 w
.02381 .50408 m
.03006 .50408 L
s
P
[(-0.02)] .01131 .50408 1 0 Mshowa
p
.001 w
.02381 .02775 m
.02756 .02775 L
s
P
p
.001 w
.02381 .04759 m
.02756 .04759 L
s
P
p
.001 w
.02381 .06744 m
.02756 .06744 L
s
P
p
.001 w
.02381 .08729 m
.02756 .08729 L
s
P
p
.001 w
.02381 .12698 m
.02756 .12698 L
s
P
p
.001 w
.02381 .14683 m
.02756 .14683 L
s
P
p
.001 w
.02381 .16668 m
.02756 .16668 L
s
P
p
.001 w
.02381 .18653 m
.02756 .18653 L
s
P
p
.001 w
.02381 .22622 m
.02756 .22622 L
s
P
p
.001 w
.02381 .24607 m
.02756 .24607 L
s
P
p
.001 w
.02381 .26591 m
.02756 .26591 L
s
P
p
.001 w
.02381 .28576 m
.02756 .28576 L
s
P
p
.001 w
.02381 .32546 m
.02756 .32546 L
s
P
p
.001 w
.02381 .3453 m
.02756 .3453 L
s
P
p
.001 w
.02381 .36515 m
.02756 .36515 L
s
P
p
.001 w
.02381 .385 m
.02756 .385 L
s
P
p
.001 w
.02381 .42469 m
.02756 .42469 L
s
P
p
.001 w
.02381 .44454 m
.02756 .44454 L
s
P
p
.001 w
.02381 .46439 m
.02756 .46439 L
s
P
p
.001 w
.02381 .48424 m
.02756 .48424 L
s
P
p
.001 w
.02381 .52393 m
.02756 .52393 L
s
P
p
.001 w
.02381 .54378 m
.02756 .54378 L
s
P
p
.001 w
.02381 .56362 m
.02756 .56362 L
s
P
p
.001 w
.02381 .58347 m
.02756 .58347 L
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
.004 w
.11905 .01472 m
.1369 .15024 L
.15476 .24577 L
.17262 .31507 L
.19048 .36664 L
.20833 .40588 L
.22619 .43634 L
.24405 .4604 L
.2619 .47971 L
.27976 .49542 L
.29762 .50835 L
.33333 .52818 L
.36905 .54245 L
.40476 .55303 L
.44048 .5611 L
.47619 .56738 L
.5119 .57236 L
.54762 .57638 L
.58333 .57967 L
.61905 .58239 L
.65476 .58467 L
.69048 .5866 L
.72619 .58824 L
.7619 .58965 L
.79762 .59088 L
.83333 .59194 L
.86905 .59288 L
.90476 .5937 L
.94048 .59444 L
.97619 .59509 L
s
P
P
% End of Graphics
MathPictureEnd

:[font = output; output; inactive; preserveAspect; endGroup]
Graphics["<<>>"]
;[o]
-Graphics-
:[font = input; preserveAspect; startGroup]
Integrate[Sqrt[x^2+c^2]^15,x]
:[font = output; output; inactive; preserveAspect; endGroup]
(c^2 + x^2)^(1/2)*((26333*c^14*x)/32768 + 
 
     (34913*c^12*x^3)/16384 + (15955*c^10*x^5)/4096 + 
 
     (66823*c^8*x^7)/14336 + (6513*c^6*x^9)/1792 + 
 
     (1601*c^4*x^11)/896 + (113*c^2*x^13)/224 + x^15/16) + 
 
  (6435*c^16*Log[x + (c^2 + x^2)^(1/2)])/32768
;[o]
                      14            12  3          10  5
      2    2   26333 c   x   34913 c   x    15955 c   x
Sqrt[c  + x ] (----------- + ------------ + ------------ + 
                  32768         16384           4096
 
            8  7         6  9         4  11        2  13    15
     66823 c  x    6513 c  x    1601 c  x     113 c  x     x
     ----------- + ---------- + ----------- + ---------- + ---
        14336         1792          896          224       16
 
               16               2    2
         6435 c   Log[x + Sqrt[c  + x ]]
     ) + -------------------------------
                      32768
:[font = subsection; inactive; preserveAspect; startGroup]
Testen der Nahfeldnäherung
:[font = text; inactive; preserveAspect]
Die Nahfeldnäherung wird über die Substrathöhe numerisch integriert
:[font = input; preserveAspect]
NFEx:= 2 * (NIntegrate[E^(-I kr[0])/kr[0],{h,0,kD}] +
 Sum[qu^n NIntegrate[E^(-I kr[n])/kr[n],{h,-kD,kD}],{n,1,100}]\
  /.qu->(1-er)/(1+er))
:[font = input; preserveAspect]
krho=1; kD=0.1; er=10;
:[font = input; preserveAspect; startGroup]
NFEx
:[font = output; output; inactive; preserveAspect; endGroup]
0.01104613216131968 - 0.016879196523057*I
;[o]
0.0110461 - 0.0168792 I
:[font = text; inactive; preserveAspect]
Die numerische Integration des genauen Kerns entlang dem Verzweigungsschnitt
1 - I t liefert:
:[font = input; preserveAspect; startGroup]
VZS
:[font = output; output; inactive; preserveAspect; endGroup]
0.0120392026399603 - 0.01588749913959573*I
;[o]
0.0120392 - 0.0158875 I
:[font = text; inactive; preserveAspect]
es folgen Test für weitere Werte: bei krho<1 ist die Integration entlang dem Verzweigungsschnitt jedoch nicht sehr zuverlässig!
:[font = input; preserveAspect]
krho=0.01; kD=0.1; er=10;
:[font = input; preserveAspect; startGroup]
NFEx
:[font = output; output; inactive; preserveAspect; endGroup]
4.659495293153985 - 0.02005475314417277*I
;[o]
4.6595 - 0.0200548 I
:[font = input; preserveAspect; startGroup]
VZS
:[font = message; inactive; preserveAspect]
NIntegrate::slwcon: 
   Numerical integration converging too slowly; suspect one of
     the following: singularity, oscillatory integrand, or
     insufficient WorkingPrecision.
:[font = message; inactive; preserveAspect]
NIntegrate::ncvb: 
   NIntegrate failed to converge to prescribed accuracy after 
    7 recursive bisections in t near t = 15.2902.
:[font = output; output; inactive; preserveAspect; endGroup]
0.02359097724236619 - 0.01726378638132261*I
;[o]
0.023591 - 0.0172638 I
:[font = input; preserveAspect]
krho=.;kD=.;er=.
:[font = input; Cclosed; preserveAspect; startGroup]
Table[{krho,kD,er,NFEx,VZS},{krho,0.8,1,0.2},
                            {er,2,10,4},
                            {kD,0.01,0.1,0.03}]
:[font = message; inactive; preserveAspect]
NIntegrate::ncvb: 
   NIntegrate failed to converge to prescribed accuracy after
    7 recursive bisections in t near t = 15.332.
:[font = output; output; inactive; preserveAspect; endGroup]
{{{{0.8, 0.01, 2, 0.00871028208480938 - 
 
      0.0089671334210825*I, 
 
     0.0087035760549708 - 0.00890113672636368*I}, 
 
    {0.8, 0.04, 2, 0.03492860695690443 - 
 
      0.03587947436654731*I, 
 
     0.03482675836695572 - 0.03484562948615554*I}, 
 
    {0.8, 0.07000000000000001, 2, 
 
     0.06146878660412522 - 0.06283124328011929*I, 
 
     0.06117403254664286 - 0.05973049278678557*I}, 
 
    {0.8, 0.1, 2, 0.0885983032320104 - 0.0898521670076781*I, 
 
     0.0880326632260201 - 0.0836527845793142*I}}, 
 
   {{0.8, 0.01, 6, 0.002903611465381029 - 
 
      0.002989067622229738*I, 
 
     0.002900786528158226 - 0.002953445899771272*I}, 
 
    {0.8, 0.04, 6, 0.01165493303128412 - 
 
      0.01196130795684733*I, 
 
     0.01165629454810659 - 0.01145062405576601*I}, 
 
    {0.8, 0.07000000000000001, 6, 
 
     0.02055817107769228 - 0.02095171625693756*I, 
 
     0.0207102681568245 - 0.01955615469702899*I}, 
 
    {0.8, 0.1, 6, 0.02975877257586601 - 
 
      0.0299740430136066*I, 
 
     0.03039446542267639 - 0.02744833545301573*I}}, 
 
   {{0.8, 0.01, 10, 0.001742175709892567 - 
 
      0.001793441697772822*I, 
 
     0.001740879518407103 - 0.00177091581144043*I}, 
 
    {0.8, 0.04, 10, 0.006993541931453878 - 
 
      0.007176855999624116*I, 
 
     0.007028993738612356 - 0.006879898661983475*I}, 
 
    {0.8, 0.07000000000000001, 10, 
 
     0.01233827457297612 - 0.01257141253503896*I, 
 
     0.01262811535984921 - 0.01184305621859809*I}, 
 
    {0.8, 0.1, 10, 0.0178685294888376 - 
 
      0.01798554686956269*I, 
 
     0.01887090191084672 - 0.01685191713171535*I}}}, 
 
  {{{1., 0.01, 2, 0.005403829304980984 - 
 
      0.00841488553345296*I, 
 
     0.005410731732961645 - 0.00835520049912625*I}, 
 
    {1., 0.04, 2, 0.02166389669042447 - 
 
      0.03367008675578134*I, 
 
     0.02177313310800377 - 0.03273456487356123*I}, 
 
    {1., 0.07000000000000001, 2, 
 
     0.03810125614936314 - 0.05896328906183668*I, 
 
     0.03843299999403355 - 0.0561558432709836*I}, 
 
    {1., 0.1, 2, 0.05485810459480152 - 0.0843231451271515*I, 
 
     0.0555311475263871 - 0.07870709699862832*I}}, 
 
   {{1., 0.01, 6, 0.001801378886653542 - 
 
      0.00280498415496043*I, 
 
     0.001805738933991474 - 0.00277279493532521*I}, 
 
    {1., 0.04, 6, 0.007227952675578298 - 
 
      0.01122479174138671*I, 
 
     0.007323026006254728 - 0.01076432411070892*I}, 
 
    {1., 0.07000000000000001, 6, 
 
     0.0127373335467157 - 0.01966210999592827*I, 
 
     0.01311179940793762 - 0.01840740293690104*I}, 
 
    {1., 0.1, 6, 0.01840062778446107 - 
 
      0.02813019323851054*I, 
 
     0.01935241006443868 - 0.02586804738471178*I}}, 
 
   {{1., 0.01, 10, 0.00108083224094372 - 
 
      0.00168299157595388*I, 
 
     0.001083986253897982 - 0.00166265469665261*I}, 
 
    {1., 0.04, 10, 0.004337091999267768 - 
 
      0.006734943693211287*I, 
 
     0.004420300718364384 - 0.006468445232702531*I}, 
 
    {1., 0.07000000000000001, 10, 
 
     0.007644192303243037 - 0.01179763493830602*I, 
 
     0.00800788099582435 - 0.01115011537618403*I}, 
 
    {1., 0.1, 10, 0.01104613216131968 - 0.016879196523057*I, 
 
     0.0120392026399603 - 0.01588749913959573*I}}}}
;[o]
{{{{0.8, 0.01, 2, 0.00871028 - 0.00896713 I, 
 
     0.00870358 - 0.00890114 I}, 
 
    {0.8, 0.04, 2, 0.0349286 - 0.0358795 I, 
 
     0.0348268 - 0.0348456 I}, 
 
    {0.8, 0.07, 2, 0.0614688 - 0.0628312 I, 
 
     0.061174 - 0.0597305 I}, 
 
    {0.8, 0.1, 2, 0.0885983 - 0.0898522 I, 
 
     0.0880327 - 0.0836528 I}}, 
 
   {{0.8, 0.01, 6, 0.00290361 - 0.00298907 I, 
 
     0.00290079 - 0.00295345 I}, 
 
    {0.8, 0.04, 6, 0.0116549 - 0.0119613 I, 
 
     0.0116563 - 0.0114506 I}, 
 
    {0.8, 0.07, 6, 0.0205582 - 0.0209517 I, 
 
     0.0207103 - 0.0195562 I}, 
 
    {0.8, 0.1, 6, 0.0297588 - 0.029974 I, 
 
     0.0303945 - 0.0274483 I}}, 
 
   {{0.8, 0.01, 10, 0.00174218 - 0.00179344 I, 
 
     0.00174088 - 0.00177092 I}, 
 
    {0.8, 0.04, 10, 0.00699354 - 0.00717686 I, 
 
     0.00702899 - 0.0068799 I}, 
 
    {0.8, 0.07, 10, 0.0123383 - 0.0125714 I, 
 
     0.0126281 - 0.0118431 I}, 
 
    {0.8, 0.1, 10, 0.0178685 - 0.0179855 I, 
 
     0.0188709 - 0.0168519 I}}}, 
 
  {{{1., 0.01, 2, 0.00540383 - 0.00841489 I, 
 
     0.00541073 - 0.0083552 I}, 
 
    {1., 0.04, 2, 0.0216639 - 0.0336701 I, 
 
     0.0217731 - 0.0327346 I}, 
 
    {1., 0.07, 2, 0.0381013 - 0.0589633 I, 
 
     0.038433 - 0.0561558 I}, 
 
    {1., 0.1, 2, 0.0548581 - 0.0843231 I, 
 
     0.0555311 - 0.0787071 I}}, 
 
   {{1., 0.01, 6, 0.00180138 - 0.00280498 I, 
 
     0.00180574 - 0.00277279 I}, 
 
    {1., 0.04, 6, 0.00722795 - 0.0112248 I, 
 
     0.00732303 - 0.0107643 I}, 
 
    {1., 0.07, 6, 0.0127373 - 0.0196621 I, 
 
     0.0131118 - 0.0184074 I}, 
 
    {1., 0.1, 6, 0.0184006 - 0.0281302 I, 
 
     0.0193524 - 0.025868 I}}, 
 
   {{1., 0.01, 10, 0.00108083 - 0.00168299 I, 
 
     0.00108399 - 0.00166265 I}, 
 
    {1., 0.04, 10, 0.00433709 - 0.00673494 I, 
 
     0.0044203 - 0.00646845 I}, 
 
    {1., 0.07, 10, 0.00764419 - 0.0117976 I, 
 
     0.00800788 - 0.0111501 I}, 
 
    {1., 0.1, 10, 0.0110461 - 0.0168792 I, 
 
     0.0120392 - 0.0158875 I}}}}
:[font = input; preserveAspect]

:[font = input; preserveAspect; startGroup]
BesselY[1,0]
:[font = output; output; inactive; preserveAspect; endGroup]
DirectedInfinity[]
;[o]
ComplexInfinity
:[font = input; preserveAspect; startGroup]
Integrate[ArcSinh[D/Sqrt[x^2+y^2]],x]
:[font = output; output; inactive; preserveAspect; endGroup]
x*ArcSinh[D/(x^2 + y^2)^(1/2)] - 
 
  y*ArcTan[(D*x*(x^2 + y^2)^(1/2)*
 
       ((D^2 + x^2 + y^2)/(x^2 + y^2))^(1/2))/
 
     (y*(D^2 + x^2 + y^2))] + 
 
  D*Log[x + (x^2 + y^2)^(1/2)*
 
      ((D^2 + x^2 + y^2)/(x^2 + y^2))^(1/2)]
;[o]
                D
x ArcSinh[-------------] - 
                2    2
          Sqrt[x  + y ]
 
                                   2    2    2
                     2    2       D  + x  + y
           D x Sqrt[x  + y ] Sqrt[------------]
                                     2    2
                                    x  + y
  y ArcTan[------------------------------------] + 
                         2    2    2
                     y (D  + x  + y )
 
                                2    2    2
                  2    2       D  + x  + y
  D Log[x + Sqrt[x  + y ] Sqrt[------------]]
                                  2    2
                                 x  + y
:[font = input; preserveAspect; startGroup]
Limit[ x ArcSinh[1/x],x->0]
:[font = output; output; inactive; preserveAspect; endGroup]
0
;[o]
0
:[font = input; preserveAspect; startGroup]
Integrate[x/Sqrt[x^2+y^2 + h^2]^3,{h,0,D}]
:[font = output; output; inactive; preserveAspect; endGroup]
(D*x)/((x^2 + y^2)*(D^2 + x^2 + y^2)^(1/2))
;[o]
            D x
----------------------------
  2    2        2    2    2
(x  + y ) Sqrt[D  + x  + y ]
:[font = input; preserveAspect; startGroup]
FindRoot[ x==Sinh[x] 0.1,{x,Pi + Pi I}]
:[font = output; output; inactive; preserveAspect; endGroup]
{x -> 0. + 2.01948391736579*10^-28*I}
;[o]
                     -28
{x -> 0. + 2.01948 10    I}
:[font = input; preserveAspect; startGroup]
D[ArcSinh[D/x],x]
:[font = output; output; inactive; preserveAspect]
-(D/((1 + D^2/x^2)^(1/2)*x^2))
;[o]
         D
-(---------------)
            2
           D    2
  Sqrt[1 + --] x
            2
           x
:[font = postscript; PostScript; formatAsPostScript; output; inactive; preserveAspect; pictureLeft = 34; pictureWidth = 282; pictureHeight = 174.25]
%!
%%Creator: Mathematica
%%AspectRatio: .61803 
MathPictureStart
%% Graphics
/Courier findfont 10  scalefont  setfont
% Scaling calculations
0.0238095 0.952381 0.603319 1.03685 [
[(0.2)] .21429 .60332 0 2 Msboxa
[(0.4)] .40476 .60332 0 2 Msboxa
[(0.6)] .59524 .60332 0 2 Msboxa
[(0.8)] .78571 .60332 0 2 Msboxa
[(1)] .97619 .60332 0 2 Msboxa
[(-0.5)] .01131 .0849 1 0 Msboxa
[(-0.4)] .01131 .18858 1 0 Msboxa
[(-0.3)] .01131 .29227 1 0 Msboxa
[(-0.2)] .01131 .39595 1 0 Msboxa
[(-0.1)] .01131 .49963 1 0 Msboxa
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
.21429 .60332 m
.21429 .60957 L
s
P
[(0.2)] .21429 .60332 0 2 Mshowa
p
.002 w
.40476 .60332 m
.40476 .60957 L
s
P
[(0.4)] .40476 .60332 0 2 Mshowa
p
.002 w
.59524 .60332 m
.59524 .60957 L
s
P
[(0.6)] .59524 .60332 0 2 Mshowa
p
.002 w
.78571 .60332 m
.78571 .60957 L
s
P
[(0.8)] .78571 .60332 0 2 Mshowa
p
.002 w
.97619 .60332 m
.97619 .60957 L
s
P
[(1)] .97619 .60332 0 2 Mshowa
p
.001 w
.0619 .60332 m
.0619 .60707 L
s
P
p
.001 w
.1 .60332 m
.1 .60707 L
s
P
p
.001 w
.1381 .60332 m
.1381 .60707 L
s
P
p
.001 w
.17619 .60332 m
.17619 .60707 L
s
P
p
.001 w
.25238 .60332 m
.25238 .60707 L
s
P
p
.001 w
.29048 .60332 m
.29048 .60707 L
s
P
p
.001 w
.32857 .60332 m
.32857 .60707 L
s
P
p
.001 w
.36667 .60332 m
.36667 .60707 L
s
P
p
.001 w
.44286 .60332 m
.44286 .60707 L
s
P
p
.001 w
.48095 .60332 m
.48095 .60707 L
s
P
p
.001 w
.51905 .60332 m
.51905 .60707 L
s
P
p
.001 w
.55714 .60332 m
.55714 .60707 L
s
P
p
.001 w
.63333 .60332 m
.63333 .60707 L
s
P
p
.001 w
.67143 .60332 m
.67143 .60707 L
s
P
p
.001 w
.70952 .60332 m
.70952 .60707 L
s
P
p
.001 w
.74762 .60332 m
.74762 .60707 L
s
P
p
.001 w
.82381 .60332 m
.82381 .60707 L
s
P
p
.001 w
.8619 .60332 m
.8619 .60707 L
s
P
p
.001 w
.9 .60332 m
.9 .60707 L
s
P
p
.001 w
.9381 .60332 m
.9381 .60707 L
s
P
p
.002 w
0 .60332 m
1 .60332 L
s
P
p
.002 w
.02381 .0849 m
.03006 .0849 L
s
P
[(-0.5)] .01131 .0849 1 0 Mshowa
p
.002 w
.02381 .18858 m
.03006 .18858 L
s
P
[(-0.4)] .01131 .18858 1 0 Mshowa
p
.002 w
.02381 .29227 m
.03006 .29227 L
s
P
[(-0.3)] .01131 .29227 1 0 Mshowa
p
.002 w
.02381 .39595 m
.03006 .39595 L
s
P
[(-0.2)] .01131 .39595 1 0 Mshowa
p
.002 w
.02381 .49963 m
.03006 .49963 L
s
P
[(-0.1)] .01131 .49963 1 0 Mshowa
p
.001 w
.02381 .10563 m
.02756 .10563 L
s
P
p
.001 w
.02381 .12637 m
.02756 .12637 L
s
P
p
.001 w
.02381 .14711 m
.02756 .14711 L
s
P
p
.001 w
.02381 .16784 m
.02756 .16784 L
s
P
p
.001 w
.02381 .20932 m
.02756 .20932 L
s
P
p
.001 w
.02381 .23005 m
.02756 .23005 L
s
P
p
.001 w
.02381 .25079 m
.02756 .25079 L
s
P
p
.001 w
.02381 .27153 m
.02756 .27153 L
s
P
p
.001 w
.02381 .313 m
.02756 .313 L
s
P
p
.001 w
.02381 .33374 m
.02756 .33374 L
s
P
p
.001 w
.02381 .35448 m
.02756 .35448 L
s
P
p
.001 w
.02381 .37521 m
.02756 .37521 L
s
P
p
.001 w
.02381 .41669 m
.02756 .41669 L
s
P
p
.001 w
.02381 .43742 m
.02756 .43742 L
s
P
p
.001 w
.02381 .45816 m
.02756 .45816 L
s
P
p
.001 w
.02381 .4789 m
.02756 .4789 L
s
P
p
.001 w
.02381 .52037 m
.02756 .52037 L
s
P
p
.001 w
.02381 .54111 m
.02756 .54111 L
s
P
p
.001 w
.02381 .56185 m
.02756 .56185 L
s
P
p
.001 w
.02381 .58258 m
.02756 .58258 L
s
P
p
.001 w
.02381 .06416 m
.02756 .06416 L
s
P
p
.001 w
.02381 .04342 m
.02756 .04342 L
s
P
p
.001 w
.02381 .02269 m
.02756 .02269 L
s
P
p
.001 w
.02381 .00195 m
.02756 .00195 L
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
.004 w
.02381 .01472 m
.02505 .01474 L
.02629 .01482 L
.02753 .01494 L
.02877 .01512 L
.03001 .01534 L
.03125 .01562 L
.03373 .01631 L
.03869 .01829 L
.04365 .02103 L
.05357 .0286 L
.06349 .03867 L
.10317 .09516 L
.14286 .16007 L
.18254 .22045 L
.22222 .27237 L
.2619 .31573 L
.30159 .35166 L
.34127 .38151 L
.38095 .40649 L
.42063 .42761 L
.46032 .44565 L
.5 .46123 L
.53968 .47483 L
.57937 .4868 L
.61905 .49745 L
.65873 .507 L
.69841 .51561 L
.7381 .52344 L
.77778 .53059 L
.81746 .53717 L
.85714 .54324 L
.89683 .54886 L
.93651 .55409 L
.97619 .55898 L
s
P
P
% End of Graphics
MathPictureEnd

:[font = output; output; inactive; preserveAspect; endGroup]
Graphics["<<>>"]
;[o]
-Graphics-
:[font = input; preserveAspect; startGroup]
Plot[BesselY[0,4.17 kr]//Re,{kr,0.001,01}]
:[font = postscript; PostScript; formatAsPostScript; output; inactive; preserveAspect; pictureLeft = 34; pictureWidth = 282; pictureHeight = 174.25]
%!
%%Creator: Mathematica
%%AspectRatio: .61803 
MathPictureStart
%% Graphics
/Courier findfont 10  scalefont  setfont
% Scaling calculations
0.0238095 0.952381 0.528245 0.144154 [
[(0.2)] .21429 .52825 0 2 Msboxa
[(0.4)] .40476 .52825 0 2 Msboxa
[(0.6)] .59524 .52825 0 2 Msboxa
[(0.8)] .78571 .52825 0 2 Msboxa
[(1)] .97619 .52825 0 2 Msboxa
[(-3)] .01131 .09578 1 0 Msboxa
[(-2)] .01131 .23994 1 0 Msboxa
[(-1)] .01131 .38409 1 0 Msboxa
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
.21429 .52825 m
.21429 .5345 L
s
P
[(0.2)] .21429 .52825 0 2 Mshowa
p
.002 w
.40476 .52825 m
.40476 .5345 L
s
P
[(0.4)] .40476 .52825 0 2 Mshowa
p
.002 w
.59524 .52825 m
.59524 .5345 L
s
P
[(0.6)] .59524 .52825 0 2 Mshowa
p
.002 w
.78571 .52825 m
.78571 .5345 L
s
P
[(0.8)] .78571 .52825 0 2 Mshowa
p
.002 w
.97619 .52825 m
.97619 .5345 L
s
P
[(1)] .97619 .52825 0 2 Mshowa
p
.001 w
.0619 .52825 m
.0619 .532 L
s
P
p
.001 w
.1 .52825 m
.1 .532 L
s
P
p
.001 w
.1381 .52825 m
.1381 .532 L
s
P
p
.001 w
.17619 .52825 m
.17619 .532 L
s
P
p
.001 w
.25238 .52825 m
.25238 .532 L
s
P
p
.001 w
.29048 .52825 m
.29048 .532 L
s
P
p
.001 w
.32857 .52825 m
.32857 .532 L
s
P
p
.001 w
.36667 .52825 m
.36667 .532 L
s
P
p
.001 w
.44286 .52825 m
.44286 .532 L
s
P
p
.001 w
.48095 .52825 m
.48095 .532 L
s
P
p
.001 w
.51905 .52825 m
.51905 .532 L
s
P
p
.001 w
.55714 .52825 m
.55714 .532 L
s
P
p
.001 w
.63333 .52825 m
.63333 .532 L
s
P
p
.001 w
.67143 .52825 m
.67143 .532 L
s
P
p
.001 w
.70952 .52825 m
.70952 .532 L
s
P
p
.001 w
.74762 .52825 m
.74762 .532 L
s
P
p
.001 w
.82381 .52825 m
.82381 .532 L
s
P
p
.001 w
.8619 .52825 m
.8619 .532 L
s
P
p
.001 w
.9 .52825 m
.9 .532 L
s
P
p
.001 w
.9381 .52825 m
.9381 .532 L
s
P
p
.002 w
0 .52825 m
1 .52825 L
s
P
p
.002 w
.02381 .09578 m
.03006 .09578 L
s
P
[(-3)] .01131 .09578 1 0 Mshowa
p
.002 w
.02381 .23994 m
.03006 .23994 L
s
P
[(-2)] .01131 .23994 1 0 Mshowa
p
.002 w
.02381 .38409 m
.03006 .38409 L
s
P
[(-1)] .01131 .38409 1 0 Mshowa
p
.001 w
.02381 .12461 m
.02756 .12461 L
s
P
p
.001 w
.02381 .15344 m
.02756 .15344 L
s
P
p
.001 w
.02381 .18227 m
.02756 .18227 L
s
P
p
.001 w
.02381 .21111 m
.02756 .21111 L
s
P
p
.001 w
.02381 .26877 m
.02756 .26877 L
s
P
p
.001 w
.02381 .2976 m
.02756 .2976 L
s
P
p
.001 w
.02381 .32643 m
.02756 .32643 L
s
P
p
.001 w
.02381 .35526 m
.02756 .35526 L
s
P
p
.001 w
.02381 .41292 m
.02756 .41292 L
s
P
p
.001 w
.02381 .44175 m
.02756 .44175 L
s
P
p
.001 w
.02381 .47058 m
.02756 .47058 L
s
P
p
.001 w
.02381 .49941 m
.02756 .49941 L
s
P
p
.001 w
.02381 .06695 m
.02756 .06695 L
s
P
p
.001 w
.02381 .03812 m
.02756 .03812 L
s
P
p
.001 w
.02381 .00929 m
.02756 .00929 L
s
P
p
.001 w
.02381 .55708 m
.02756 .55708 L
s
P
p
.001 w
.02381 .58591 m
.02756 .58591 L
s
P
p
.001 w
.02381 .61474 m
.02756 .61474 L
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
.004 w
.02476 .01472 m
.026 .09119 L
.02724 .13233 L
.02848 .16065 L
.02972 .18227 L
.03096 .19977 L
.03219 .21448 L
.03467 .23832 L
.03963 .273 L
.04458 .29826 L
.05449 .33469 L
.0644 .36114 L
.08423 .39948 L
.10405 .42767 L
.14369 .46934 L
.18333 .50052 L
.22298 .52548 L
.26262 .54598 L
.30226 .56282 L
.3419 .57645 L
.38155 .58711 L
.40137 .59139 L
.42119 .59498 L
.44101 .5979 L
.46083 .60018 L
.47074 .60108 L
.48065 .60182 L
.49057 .60242 L
.49552 .60265 L
.50048 .60286 L
.50543 .60302 L
.51039 .60315 L
.51534 .60324 L
.51782 .60328 L
.51906 .60329 L
.5203 .6033 L
.52154 .60331 L
.52278 .60331 L
.52401 .60332 L
.52525 .60332 L
.52649 .60332 L
.52773 .60332 L
.52897 .60331 L
.53021 .6033 L
.53145 .60329 L
.53269 .60328 L
.53516 .60325 L
.53764 .60322 L
.54012 .60317 L
.54507 .60305 L
Mistroke
.55003 .6029 L
.55994 .60249 L
.56985 .60195 L
.57976 .60129 L
.59958 .59959 L
.6194 .59742 L
.65905 .59178 L
.69869 .58464 L
.73833 .57625 L
.77798 .56692 L
.81762 .55693 L
.85726 .54658 L
.8969 .53618 L
.93655 .52601 L
.97619 .51633 L
Mfstroke
P
P
% End of Graphics
MathPictureEnd

:[font = output; output; inactive; preserveAspect; endGroup]
Graphics["<<>>"]
;[o]
-Graphics-
:[font = input; preserveAspect]
alfa:=Log[(er-1)/(er+1)]/2/kD;
:[font = input; preserveAspect; startGroup]
kD=0.1;er=5
:[font = output; output; inactive; preserveAspect; endGroup]
5
;[o]
5
:[font = input; preserveAspect]
krho=.
:[font = input; preserveAspect; startGroup]
Plot[-NIntegrate[ Cos[(I krho Sqrt[alfa^2+1] Cos[x])],
  {x,-ArcTan[alfa],Pi/2}]+ Pi/2 BesselY[0,krho Sqrt[alfa^2+1]],
  {krho,0.2,1}]

:[font = postscript; PostScript; formatAsPostScript; output; inactive; preserveAspect; pictureLeft = 34; pictureWidth = 282; pictureHeight = 174.25]
%!
%%Creator: Mathematica
%%AspectRatio: .61803 
MathPictureStart
%% Graphics
/Courier findfont 10  scalefont  setfont
% Scaling calculations
-0.214286 1.19048 0.495758 0.37725 [
[(0.2)] .02381 .49576 0 2 Msboxa
[(0.4)] .2619 .49576 0 2 Msboxa
[(0.6)] .5 .49576 0 2 Msboxa
[(0.8)] .7381 .49576 0 2 Msboxa
[(-1.25)] .96369 .0242 1 0 Msboxa
[(-1)] .96369 .11851 1 0 Msboxa
[(-0.75)] .96369 .21282 1 0 Msboxa
[(-0.5)] .96369 .30713 1 0 Msboxa
[(-0.25)] .96369 .40145 1 0 Msboxa
[(0.25)] .96369 .59007 1 0 Msboxa
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
.02381 .49576 m
.02381 .50201 L
s
P
[(0.2)] .02381 .49576 0 2 Mshowa
p
.002 w
.2619 .49576 m
.2619 .50201 L
s
P
[(0.4)] .2619 .49576 0 2 Mshowa
p
.002 w
.5 .49576 m
.5 .50201 L
s
P
[(0.6)] .5 .49576 0 2 Mshowa
p
.002 w
.7381 .49576 m
.7381 .50201 L
s
P
[(0.8)] .7381 .49576 0 2 Mshowa
p
.001 w
.07143 .49576 m
.07143 .49951 L
s
P
p
.001 w
.11905 .49576 m
.11905 .49951 L
s
P
p
.001 w
.16667 .49576 m
.16667 .49951 L
s
P
p
.001 w
.21429 .49576 m
.21429 .49951 L
s
P
p
.001 w
.30952 .49576 m
.30952 .49951 L
s
P
p
.001 w
.35714 .49576 m
.35714 .49951 L
s
P
p
.001 w
.40476 .49576 m
.40476 .49951 L
s
P
p
.001 w
.45238 .49576 m
.45238 .49951 L
s
P
p
.001 w
.54762 .49576 m
.54762 .49951 L
s
P
p
.001 w
.59524 .49576 m
.59524 .49951 L
s
P
p
.001 w
.64286 .49576 m
.64286 .49951 L
s
P
p
.001 w
.69048 .49576 m
.69048 .49951 L
s
P
p
.001 w
.78571 .49576 m
.78571 .49951 L
s
P
p
.001 w
.83333 .49576 m
.83333 .49951 L
s
P
p
.001 w
.88095 .49576 m
.88095 .49951 L
s
P
p
.001 w
.92857 .49576 m
.92857 .49951 L
s
P
p
.002 w
0 .49576 m
1 .49576 L
s
P
p
.002 w
.97619 .0242 m
.98244 .0242 L
s
P
[(-1.25)] .96369 .0242 1 0 Mshowa
p
.002 w
.97619 .11851 m
.98244 .11851 L
s
P
[(-1)] .96369 .11851 1 0 Mshowa
p
.002 w
.97619 .21282 m
.98244 .21282 L
s
P
[(-0.75)] .96369 .21282 1 0 Mshowa
p
.002 w
.97619 .30713 m
.98244 .30713 L
s
P
[(-0.5)] .96369 .30713 1 0 Mshowa
p
.002 w
.97619 .40145 m
.98244 .40145 L
s
P
[(-0.25)] .96369 .40145 1 0 Mshowa
p
.002 w
.97619 .59007 m
.98244 .59007 L
s
P
[(0.25)] .96369 .59007 1 0 Mshowa
p
.001 w
.97619 .04306 m
.97994 .04306 L
s
P
p
.001 w
.97619 .06192 m
.97994 .06192 L
s
P
p
.001 w
.97619 .08078 m
.97994 .08078 L
s
P
p
.001 w
.97619 .09965 m
.97994 .09965 L
s
P
p
.001 w
.97619 .13737 m
.97994 .13737 L
s
P
p
.001 w
.97619 .15623 m
.97994 .15623 L
s
P
p
.001 w
.97619 .1751 m
.97994 .1751 L
s
P
p
.001 w
.97619 .19396 m
.97994 .19396 L
s
P
p
.001 w
.97619 .23168 m
.97994 .23168 L
s
P
p
.001 w
.97619 .25055 m
.97994 .25055 L
s
P
p
.001 w
.97619 .26941 m
.97994 .26941 L
s
P
p
.001 w
.97619 .28827 m
.97994 .28827 L
s
P
p
.001 w
.97619 .326 m
.97994 .326 L
s
P
p
.001 w
.97619 .34486 m
.97994 .34486 L
s
P
p
.001 w
.97619 .36372 m
.97994 .36372 L
s
P
p
.001 w
.97619 .38258 m
.97994 .38258 L
s
P
p
.001 w
.97619 .42031 m
.97994 .42031 L
s
P
p
.001 w
.97619 .43917 m
.97994 .43917 L
s
P
p
.001 w
.97619 .45803 m
.97994 .45803 L
s
P
p
.001 w
.97619 .4769 m
.97994 .4769 L
s
P
p
.001 w
.97619 .51462 m
.97994 .51462 L
s
P
p
.001 w
.97619 .53348 m
.97994 .53348 L
s
P
p
.001 w
.97619 .55235 m
.97994 .55235 L
s
P
p
.001 w
.97619 .57121 m
.97994 .57121 L
s
P
p
.001 w
.97619 .00533 m
.97994 .00533 L
s
P
p
.001 w
.97619 .60893 m
.97994 .60893 L
s
P
p
.002 w
.97619 0 m
.97619 .61803 L
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
.004 w
.02381 .01472 m
.06349 .08126 L
.10317 .14005 L
.14286 .19275 L
.18254 .24044 L
.22222 .28389 L
.2619 .32362 L
.30159 .36 L
.34127 .39332 L
.38095 .42378 L
.42063 .45155 L
.46032 .47675 L
.5 .49946 L
.53968 .51978 L
.57937 .53775 L
.61905 .55345 L
.65873 .56691 L
.69841 .57818 L
.7381 .58731 L
.77778 .59435 L
.79762 .59709 L
.81746 .59933 L
.8373 .60106 L
.84722 .60174 L
.85714 .6023 L
.86706 .60274 L
.87202 .60291 L
.87698 .60305 L
.88194 .60316 L
.88442 .60321 L
.8869 .60324 L
.88938 .60327 L
.89187 .6033 L
.89311 .6033 L
.89435 .60331 L
.89559 .60332 L
.89683 .60332 L
.89807 .60332 L
.89931 .60332 L
.90055 .60332 L
.90179 .60331 L
.90303 .6033 L
.90427 .6033 L
.90675 .60327 L
.90923 .60324 L
.91171 .60321 L
.91667 .60311 L
.92163 .60298 L
.92659 .60283 L
.93651 .60243 L
Mistroke
.94643 .60192 L
.95635 .60129 L
.97619 .5997 L
Mfstroke
P
P
% End of Graphics
MathPictureEnd

:[font = output; output; inactive; preserveAspect; endGroup]
Graphics["<<>>"]
;[o]
-Graphics-
:[font = input; preserveAspect; startGroup]
Plot[NIntegrate[E^(-I kr)/kr/.kr->Sqrt[x^2+y^2],{x,-0.1,0.1}] /
           ( 0.2 E^(-I y)/y) - 1//Abs,{y,0.1,5}]
:[font = postscript; PostScript; formatAsPostScript; output; inactive; preserveAspect; pictureLeft = 34; pictureWidth = 282; pictureHeight = 174.25]
%!
%%Creator: Mathematica
%%AspectRatio: .61803 
MathPictureStart
%% Graphics
/Courier findfont 10  scalefont  setfont
% Scaling calculations
0.0238095 0.190476 0.0147151 7.8084 [
[(1)] .21429 .01472 0 2 Msboxa
[(2)] .40476 .01472 0 2 Msboxa
[(3)] .59524 .01472 0 2 Msboxa
[(4)] .78571 .01472 0 2 Msboxa
[(5)] .97619 .01472 0 2 Msboxa
[(0.01)] .01131 .0928 1 0 Msboxa
[(0.02)] .01131 .17088 1 0 Msboxa
[(0.03)] .01131 .24897 1 0 Msboxa
[(0.04)] .01131 .32705 1 0 Msboxa
[(0.05)] .01131 .40514 1 0 Msboxa
[(0.06)] .01131 .48322 1 0 Msboxa
[(0.07)] .01131 .5613 1 0 Msboxa
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
.21429 .01472 m
.21429 .02097 L
s
P
[(1)] .21429 .01472 0 2 Mshowa
p
.002 w
.40476 .01472 m
.40476 .02097 L
s
P
[(2)] .40476 .01472 0 2 Mshowa
p
.002 w
.59524 .01472 m
.59524 .02097 L
s
P
[(3)] .59524 .01472 0 2 Mshowa
p
.002 w
.78571 .01472 m
.78571 .02097 L
s
P
[(4)] .78571 .01472 0 2 Mshowa
p
.002 w
.97619 .01472 m
.97619 .02097 L
s
P
[(5)] .97619 .01472 0 2 Mshowa
p
.001 w
.0619 .01472 m
.0619 .01847 L
s
P
p
.001 w
.1 .01472 m
.1 .01847 L
s
P
p
.001 w
.1381 .01472 m
.1381 .01847 L
s
P
p
.001 w
.17619 .01472 m
.17619 .01847 L
s
P
p
.001 w
.25238 .01472 m
.25238 .01847 L
s
P
p
.001 w
.29048 .01472 m
.29048 .01847 L
s
P
p
.001 w
.32857 .01472 m
.32857 .01847 L
s
P
p
.001 w
.36667 .01472 m
.36667 .01847 L
s
P
p
.001 w
.44286 .01472 m
.44286 .01847 L
s
P
p
.001 w
.48095 .01472 m
.48095 .01847 L
s
P
p
.001 w
.51905 .01472 m
.51905 .01847 L
s
P
p
.001 w
.55714 .01472 m
.55714 .01847 L
s
P
p
.001 w
.63333 .01472 m
.63333 .01847 L
s
P
p
.001 w
.67143 .01472 m
.67143 .01847 L
s
P
p
.001 w
.70952 .01472 m
.70952 .01847 L
s
P
p
.001 w
.74762 .01472 m
.74762 .01847 L
s
P
p
.001 w
.82381 .01472 m
.82381 .01847 L
s
P
p
.001 w
.8619 .01472 m
.8619 .01847 L
s
P
p
.001 w
.9 .01472 m
.9 .01847 L
s
P
p
.001 w
.9381 .01472 m
.9381 .01847 L
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
.02381 .0928 m
.03006 .0928 L
s
P
[(0.01)] .01131 .0928 1 0 Mshowa
p
.002 w
.02381 .17088 m
.03006 .17088 L
s
P
[(0.02)] .01131 .17088 1 0 Mshowa
p
.002 w
.02381 .24897 m
.03006 .24897 L
s
P
[(0.03)] .01131 .24897 1 0 Mshowa
p
.002 w
.02381 .32705 m
.03006 .32705 L
s
P
[(0.04)] .01131 .32705 1 0 Mshowa
p
.002 w
.02381 .40514 m
.03006 .40514 L
s
P
[(0.05)] .01131 .40514 1 0 Mshowa
p
.002 w
.02381 .48322 m
.03006 .48322 L
s
P
[(0.06)] .01131 .48322 1 0 Mshowa
p
.002 w
.02381 .5613 m
.03006 .5613 L
s
P
[(0.07)] .01131 .5613 1 0 Mshowa
p
.001 w
.02381 .03033 m
.02756 .03033 L
s
P
p
.001 w
.02381 .04595 m
.02756 .04595 L
s
P
p
.001 w
.02381 .06157 m
.02756 .06157 L
s
P
p
.001 w
.02381 .07718 m
.02756 .07718 L
s
P
p
.001 w
.02381 .10842 m
.02756 .10842 L
s
P
p
.001 w
.02381 .12403 m
.02756 .12403 L
s
P
p
.001 w
.02381 .13965 m
.02756 .13965 L
s
P
p
.001 w
.02381 .15527 m
.02756 .15527 L
s
P
p
.001 w
.02381 .1865 m
.02756 .1865 L
s
P
p
.001 w
.02381 .20212 m
.02756 .20212 L
s
P
p
.001 w
.02381 .21773 m
.02756 .21773 L
s
P
p
.001 w
.02381 .23335 m
.02756 .23335 L
s
P
p
.001 w
.02381 .26458 m
.02756 .26458 L
s
P
p
.001 w
.02381 .2802 m
.02756 .2802 L
s
P
p
.001 w
.02381 .29582 m
.02756 .29582 L
s
P
p
.001 w
.02381 .31143 m
.02756 .31143 L
s
P
p
.001 w
.02381 .34267 m
.02756 .34267 L
s
P
p
.001 w
.02381 .35828 m
.02756 .35828 L
s
P
p
.001 w
.02381 .3739 m
.02756 .3739 L
s
P
p
.001 w
.02381 .38952 m
.02756 .38952 L
s
P
p
.001 w
.02381 .42075 m
.02756 .42075 L
s
P
p
.001 w
.02381 .43637 m
.02756 .43637 L
s
P
p
.001 w
.02381 .45199 m
.02756 .45199 L
s
P
p
.001 w
.02381 .4676 m
.02756 .4676 L
s
P
p
.001 w
.02381 .49884 m
.02756 .49884 L
s
P
p
.001 w
.02381 .51445 m
.02756 .51445 L
s
P
p
.001 w
.02381 .53007 m
.02756 .53007 L
s
P
p
.001 w
.02381 .54569 m
.02756 .54569 L
s
P
p
.001 w
.02381 .57692 m
.02756 .57692 L
s
P
p
.001 w
.02381 .59254 m
.02756 .59254 L
s
P
p
.001 w
.02381 .60815 m
.02756 .60815 L
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
.004 w
s
s
s
.04913 .61803 m
.05015 .57817 L
s
.05015 .57817 m
.05258 .50145 L
.05501 .43899 L
.05744 .38756 L
.0623 .30889 L
.06716 .25258 L
.07202 .21102 L
.07688 .17956 L
.08175 .15521 L
.09147 .12058 L
.09633 .10804 L
.10119 .09769 L
.11091 .0818 L
.12063 .07032 L
.13036 .06175 L
.14008 .05519 L
.1498 .05005 L
.15952 .04595 L
.17897 .03986 L
.18869 .03757 L
.19841 .03563 L
.21786 .03255 L
.2373 .03023 L
.25675 .02843 L
.27619 .027 L
.29563 .02583 L
.31508 .02487 L
.35397 .02337 L
.39286 .02227 L
.43175 .02142 L
.47063 .02074 L
.50952 .02019 L
.54841 .01974 L
.5873 .01936 L
.62619 .01903 L
.66508 .01875 L
.70397 .0185 L
.74286 .01828 L
.78175 .01809 L
.82063 .01791 L
.85952 .01776 L
.89841 .01762 L
.9373 .01749 L
.97619 .01737 L
s
P
P
% End of Graphics
MathPictureEnd

:[font = output; output; inactive; preserveAspect; endGroup]
Graphics["<<>>"]
;[o]
-Graphics-
:[font = input; preserveAspect; startGroup]
Integrate[Cos[ l x]/Sqrt[1-x^2],{x,0,1}]
:[font = output; output; inactive; preserveAspect; endGroup; endGroup]
(Pi*BesselJ[0, (l^2)^(1/2)])/2
;[o]
                    2
Pi BesselJ[0, Sqrt[l ]]
-----------------------
           2
^*)