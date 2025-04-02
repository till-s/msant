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
:[font = input; preserveAspect; startGroup]
2+2
:[font = output; output; inactive; preserveAspect; endGroup]
4
;[o]
4
:[font = input; preserveAspect; startGroup]
NIntegrate[ Cos[kr]/kr /.kr->Sqrt[u^2+c^2+h^2]\
/.c->0.5,{u,0,0.1},{h,0,00.01}]
:[font = output; output; inactive; preserveAspect; endGroup]
0.001740366502124556
;[o]
0.00174037
:[font = input; preserveAspect; startGroup]
Integrate[Log[h+Sqrt[h^2+y^2+u^2]],u]
:[font = output; output; inactive; preserveAspect; endGroup]
-u + y*ArcTan[u/y] - y*ArcTan[(h*u)/
 
     (y*(h^2 + u^2 + y^2)^(1/2))] + 
 
  u*Log[h + (h^2 + u^2 + y^2)^(1/2)] + 
 
  h*Log[u + (h^2 + u^2 + y^2)^(1/2)]
;[o]
              u                     h u
-u + y ArcTan[-] - y ArcTan[--------------------] + 
              y                     2    2    2
                            y Sqrt[h  + u  + y ]
 
                  2    2    2                     2    2    2
  u Log[h + Sqrt[h  + u  + y ]] + h Log[u + Sqrt[h  + u  + y ]]
:[font = input; preserveAspect]

:[font = input; preserveAspect; startGroup]
Integrate[1/Sqrt[u^2+h^2+c^2],h]
:[font = output; output; inactive; preserveAspect; endGroup]
Log[h + (c^2 + h^2 + u^2)^(1/2)]
;[o]
              2    2    2
Log[h + Sqrt[c  + h  + u ]]
:[font = input; preserveAspect; startGroup]
NIntegrate[ Cos[kr]/kr /.kr->Sqrt[u^2+c^2]\
/.c->0.1,{u,0,0.1}]
:[font = output; output; inactive; preserveAspect; endGroup]
0.875641149155856
;[o]
0.875641
:[font = input; preserveAspect; startGroup]
ListPlot[ReadList["contint/sommer/feil"]//Flatten]
:[font = postscript; PostScript; formatAsPostScript; output; inactive; preserveAspect; pictureLeft = 34; pictureWidth = 282; pictureHeight = 174.25]
%!
%%Creator: Mathematica
%%AspectRatio: .61803 
MathPictureStart
%% Graphics
/Courier findfont 10  scalefont  setfont
% Scaling calculations
0.0238095 0.00952381 0.367359 2.46699 [
[(20)] .21429 .36736 0 2 Msboxa
[(40)] .40476 .36736 0 2 Msboxa
[(60)] .59524 .36736 0 2 Msboxa
[(80)] .78571 .36736 0 2 Msboxa
[(100)] .97619 .36736 0 2 Msboxa
[(-0.1)] .01131 .12066 1 0 Msboxa
[(-0.05)] .01131 .24401 1 0 Msboxa
[(0.05)] .01131 .49071 1 0 Msboxa
[(0.1)] .01131 .61406 1 0 Msboxa
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
.21429 .36736 m
.21429 .37361 L
s
P
[(20)] .21429 .36736 0 2 Mshowa
p
.002 w
.40476 .36736 m
.40476 .37361 L
s
P
[(40)] .40476 .36736 0 2 Mshowa
p
.002 w
.59524 .36736 m
.59524 .37361 L
s
P
[(60)] .59524 .36736 0 2 Mshowa
p
.002 w
.78571 .36736 m
.78571 .37361 L
s
P
[(80)] .78571 .36736 0 2 Mshowa
p
.002 w
.97619 .36736 m
.97619 .37361 L
s
P
[(100)] .97619 .36736 0 2 Mshowa
p
.001 w
.0619 .36736 m
.0619 .37111 L
s
P
p
.001 w
.1 .36736 m
.1 .37111 L
s
P
p
.001 w
.1381 .36736 m
.1381 .37111 L
s
P
p
.001 w
.17619 .36736 m
.17619 .37111 L
s
P
p
.001 w
.25238 .36736 m
.25238 .37111 L
s
P
p
.001 w
.29048 .36736 m
.29048 .37111 L
s
P
p
.001 w
.32857 .36736 m
.32857 .37111 L
s
P
p
.001 w
.36667 .36736 m
.36667 .37111 L
s
P
p
.001 w
.44286 .36736 m
.44286 .37111 L
s
P
p
.001 w
.48095 .36736 m
.48095 .37111 L
s
P
p
.001 w
.51905 .36736 m
.51905 .37111 L
s
P
p
.001 w
.55714 .36736 m
.55714 .37111 L
s
P
p
.001 w
.63333 .36736 m
.63333 .37111 L
s
P
p
.001 w
.67143 .36736 m
.67143 .37111 L
s
P
p
.001 w
.70952 .36736 m
.70952 .37111 L
s
P
p
.001 w
.74762 .36736 m
.74762 .37111 L
s
P
p
.001 w
.82381 .36736 m
.82381 .37111 L
s
P
p
.001 w
.8619 .36736 m
.8619 .37111 L
s
P
p
.001 w
.9 .36736 m
.9 .37111 L
s
P
p
.001 w
.9381 .36736 m
.9381 .37111 L
s
P
p
.002 w
0 .36736 m
1 .36736 L
s
P
p
.002 w
.02381 .12066 m
.03006 .12066 L
s
P
[(-0.1)] .01131 .12066 1 0 Mshowa
p
.002 w
.02381 .24401 m
.03006 .24401 L
s
P
[(-0.05)] .01131 .24401 1 0 Mshowa
p
.002 w
.02381 .49071 m
.03006 .49071 L
s
P
[(0.05)] .01131 .49071 1 0 Mshowa
p
.002 w
.02381 .61406 m
.03006 .61406 L
s
P
[(0.1)] .01131 .61406 1 0 Mshowa
p
.001 w
.02381 .02198 m
.02756 .02198 L
s
P
p
.001 w
.02381 .04665 m
.02756 .04665 L
s
P
p
.001 w
.02381 .07132 m
.02756 .07132 L
s
P
p
.001 w
.02381 .09599 m
.02756 .09599 L
s
P
p
.001 w
.02381 .14533 m
.02756 .14533 L
s
P
p
.001 w
.02381 .17 m
.02756 .17 L
s
P
p
.001 w
.02381 .19467 m
.02756 .19467 L
s
P
p
.001 w
.02381 .21934 m
.02756 .21934 L
s
P
p
.001 w
.02381 .26868 m
.02756 .26868 L
s
P
p
.001 w
.02381 .29335 m
.02756 .29335 L
s
P
p
.001 w
.02381 .31802 m
.02756 .31802 L
s
P
p
.001 w
.02381 .34269 m
.02756 .34269 L
s
P
p
.001 w
.02381 .39203 m
.02756 .39203 L
s
P
p
.001 w
.02381 .4167 m
.02756 .4167 L
s
P
p
.001 w
.02381 .44137 m
.02756 .44137 L
s
P
p
.001 w
.02381 .46604 m
.02756 .46604 L
s
P
p
.001 w
.02381 .51538 m
.02756 .51538 L
s
P
p
.001 w
.02381 .54005 m
.02756 .54005 L
s
P
p
.001 w
.02381 .56472 m
.02756 .56472 L
s
P
p
.001 w
.02381 .58939 m
.02756 .58939 L
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
.03333 -1.08381 Mdot
.04286 -0.62002 Mdot
.05238 -0.34 Mdot
.0619 -0.15971 Mdot
.07143 -0.03901 Mdot
.08095 .04434 Mdot
.09048 .10351 Mdot
.1 .14652 Mdot
.10952 .17846 Mdot
.11905 .20259 Mdot
.12857 .2211 Mdot
.1381 .23544 Mdot
.14762 .24663 Mdot
.15714 .25538 Mdot
.16667 .26218 Mdot
.17619 .26737 Mdot
.18571 .27122 Mdot
.19524 .27387 Mdot
.20476 .27542 Mdot
.21429 .2759 Mdot
.22381 .27526 Mdot
.23333 .2734 Mdot
.24286 .27006 Mdot
.25238 .26487 Mdot
.2619 .25711 Mdot
.27143 .24554 Mdot
.28095 .22775 Mdot
.29048 .19845 Mdot
.3 .14358 Mdot
.30952 .00925 Mdot
.31905 -0.77681 Mdot
.32857 1.13236 Mdot
.3381 .63222 Mdot
.34762 .51978 Mdot
.35714 .47048 Mdot
.36667 .44296 Mdot
.37619 .42548 Mdot
.38571 .41346 Mdot
.39524 .40472 Mdot
.40476 .3981 Mdot
.41429 .39293 Mdot
.42381 .38879 Mdot
.43333 .3854 Mdot
.44286 .38259 Mdot
.45238 .38022 Mdot
.4619 .37819 Mdot
.47143 .37644 Mdot
.48095 .37492 Mdot
.49048 .37358 Mdot
.5 .37238 Mdot
.50952 .37132 Mdot
.51905 .37035 Mdot
.52857 .36948 Mdot
.5381 .36869 Mdot
.54762 .36795 Mdot
.55714 .36727 Mdot
.56667 .36664 Mdot
.57619 .36605 Mdot
.58571 .36549 Mdot
.59524 .36496 Mdot
.60476 .36445 Mdot
.61429 .36397 Mdot
.62381 .3635 Mdot
.63333 .36304 Mdot
.64286 .36259 Mdot
.65238 .36214 Mdot
.6619 .3617 Mdot
.67143 .36126 Mdot
.68095 .36081 Mdot
.69048 .36036 Mdot
.7 .3599 Mdot
.70952 .35942 Mdot
.71905 .35892 Mdot
.72857 .3584 Mdot
.7381 .35785 Mdot
.74762 .35726 Mdot
.75714 .35662 Mdot
.76667 .35593 Mdot
.77619 .35518 Mdot
.78571 .35434 Mdot
.79524 .3534 Mdot
.80476 .35232 Mdot
.81429 .35106 Mdot
.82381 .34961 Mdot
.83333 .34787 Mdot
.84286 .34571 Mdot
.85238 .343 Mdot
.8619 .33945 Mdot
.87143 .33453 Mdot
.88095 .32746 Mdot
.89048 .31601 Mdot
.9 .29442 Mdot
.90952 .23872 Mdot
.91905 -0.26925 Mdot
.92857 .57215 Mdot
.9381 .45341 Mdot
.94762 .42075 Mdot
.95714 .40555 Mdot
.96667 .39662 Mdot
P
% End of Graphics
MathPictureEnd

:[font = message; inactive; preserveAspect]
Graphics::gptn: 
   Coordinate Null in {100, Null}
     is not a floating-point number.
:[font = output; output; inactive; preserveAspect; endGroup]
Graphics["<<>>"]
;[o]
-Graphics-
^*)