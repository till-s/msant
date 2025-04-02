2,39c2,3
< ! Uncomment the appropiate line for 
< ! either english or german labels
< !
< !	#include "Labels.eng"
< Msant*Viewport.borderWidth:1
< Msant*SlPaned.internalBorderColor:grey80
< Msant*SlPaned.internalBorderWidth:20
< !Msant*Porthole.translations:#override\n\
< <Enter>: FocusSet()\n
< Msant*graf_shadow.shadowWidth:2
< Msant*graf_shadow.background:grey80
< 
< !Msant*PmPanner.resize:False
< !Msant*PmPanner.resizable:True
< Msant*PmPanner.transparentSlider:True
< 
< 
< Msant*PmPanner.shadowWidth:1
< Msant*PmPanner.shadowColor:black
< Msant*PmPanner.internalSpace:2
< Msant*PmPanner.borderWidth:0
< Msant*PmPanner.emboss:True
< Msant*PmPanner.foreground:grey80
< Msant*PmPanner.shadowThickness:1
< 
< !Msant*PmPanner.background:grey90
< 
< Msant.canvasWidth:1000
< Msant.canvasHeight:1000
< Msant*Vpaned*vp_strom.height:100
< Msant*Vpaned*vp_strom.width:700
< 
< #include "msant/Labels.eng"
< 
< !
< 
< Msant*canvasWidth: 1000
< Msant*canvasHeight: 1000
---
> Msant.canvasWidth: 1000
> Msant.canvasHeight: 1000
115,125c79,89
< Msant*font:			lucidasans-10
< Msant*Label.font:		lucidasans-bold-10
< Msant*Berechn_message4.font: 	lucidasans-10
< Msant*Datei_outpfn.font:	lucidasans-10
< Msant*Haupt_outpx.font:		lucidasans-10
< Msant*Haupt_outpy.font:		lucidasans-10
< Msant*Haupt_outpdx.font:	lucidasans-10
< Msant*Haupt_outpdy.font:	lucidasans-10
< Msant*Haupt_outpm.font:		lucidasans-10
< Msant*Berechn_koppvek.font:	lucidasans-10
< Msant*Berechn_koppy.font:	lucidasans-10
---
> Msant*font:			lucidasans-12
> Msant*Label.font:		lucidasans-bold-12
> Msant*Berechn_message4.font: 	lucidasans-12
> Msant*Datei_outpfn.font:	lucidasans-12
> Msant*Haupt_outpx.font:		lucidasans-12
> Msant*Haupt_outpy.font:		lucidasans-12
> Msant*Haupt_outpdx.font:	lucidasans-12
> Msant*Haupt_outpdy.font:	lucidasans-12
> Msant*Haupt_outpm.font:		lucidasans-12
> Msant*Berechn_koppvek.font:	lucidasans-12
> Msant*Berechn_koppy.font:	lucidasans-12
147c111
< Msant.foreground:				black
---
> Msant.foreground:				green
152,153d115
< ! The patch surface
< Msant.patchColour:				light goldenrod yellow
154a117,126
> !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
> !
> ! INCLUDE THE LABEL DEFS HERE
> !
> !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
> ! German labels for MSANT (Athena Version)
> ! Include these into Resource File
> !
> 
> #include "Labels.eng"
319,322d290
< Msant*Viewport*vertical.width: 10
< Msant*Viewport*horizontal.height: 10
< Msant*shadowWidth:1
< Msant*beNiceToColormap: False
329,330c297,298
< Msant*TextField.width: 80
< Msant*internalHeight:1
---
> Msant*TextField.width: 120
> Msant*internalHeight:3
333,336c301,303
< Msant*vp_grafik.height:200
< Msant*vp_grafik.width:200
< Msant*vp_strom.height:100
< Msant*vp_strom.width:800
---
> Msant*vp_grafik.width:500
> Msant*vp_grafik.height:500
> Msant*vp_strom.height:200
339,343c306,308
< Msant*File*Datei_loadbutt.vertDistance:20
< Msant*File*Datei_savebutt.vertDistance:20
< Msant*File*Datei_saveasbutt.vertDistance:20
< Msant*File*New.vertDistance:20
< Msant*File*Datei_dateiname.vertDistance:20
---
> Msant*File*Command.vertDistance:30
> Msant*File*New.vertDistance:30
> Msant*File*Datei_dateiname.vertDistance:30
359c324
< Msant*PmPanner.focusInterest:True
---
> Msant*vp_grafik*focusInterest:True
362,363d326
< Msant*PmPanner.translations:#override\n\
< 	<Enter>: FocusSet()\n
