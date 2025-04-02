c     This was _not_ programmed very efficiently! (have a look at
c     the assembly code of the concerned procedures)
c     The subroutines and funtions
c
c     fill0(), mark1() and iget()
c
c     can be implemented _much_ more elegant int c, hence they are moved
c     to bitmod.c
c
c     by T. Straumann, 5/1997
c
c
c     SUBROUTINE FILL0(BITMAP, N)                                        A531      175
C                                                                        A531      176
C     FILL THE FIRST N BITS OF BITMAP WITH ZEROES.                       A531      177
C                                                                        A531      178
c     INTEGER BITMAP(1), N                                               A531      179
C                                                                        A531      180
c     DATA NBPW /31/                                                     A531      181
C     NBPW IS THE MINIMUM NUMBER OF SIGNIFICANT BITS PER WORD USED       A531      182
C     BY INTEGER ARITHMETIC.  THIS IS USUALLY ONE LESS THAN THE          A531      183
C     ACTUAL NUMBER OF BITS PER WORD, BUT AN IMPORTANT EXCEPTION IS      A531      184
C     THE CDC-6000 SERIES OF MACHINES, WHERE NBPW SHOULD BE 48.          A531      185
C                                                                        A531      186
c     LOOP = N/NBPW                                                      A531      187
c     NBLW = MOD(N,NBPW)                                                 A531      188
c     IF (LOOP.EQ.0) GO TO 20                                            A531      189
c     DO 10 I=1,LOOP                                                     A531      190
c       BITMAP(I) = 0                                                    A531      191
c  10 CONTINUE                                                           A531      192
c  20 IF (NBLW.NE.0) BITMAP(LOOP+1) = MOD(BITMAP(LOOP+1),2**(NBPW-NBLW)  A531      193
c    *  )                                                                A531      194
c     RETURN                                                             A531      195
c     END                                                                A531      196
C     *******************************************************************
c     SUBROUTINE MARK1(BITMAP, N)                                        A531      197
C                                                                        A531      198 
C     PUT A ONE IN THE NTH BIT OF BITMAP.                                A531      199
C                                                                        A531      200
c     INTEGER BITMAP(1), N                                               A531      201
C                                                                        A531      202
c     DATA NBPW /31/                                                     A531      203
C     NBPW IS THE MINIMUM NUMBER OF SIGNIFICANT BITS PER WORD USED       A531      204
C     BY INTEGER ARITHMETIC.  THIS IS USUALLY ONE LESS THAN THE          A531      205
C     ACTUAL NUMBER OF BITS PER WORD, BUT AN IMPORTANT EXCEPTION IS      A531      206
C     THE CDC-6000 SERIES OF MACHINES, WHERE NBPW SHOULD BE 48.          A531      207
C                                                                        A531      208
c     NWORD = (N-1)/NBPW                                                 A531      209
c     NBIT = MOD(N-1,NBPW)                                               A531      210
c     I = 2**(NBPW-NBIT-1)                                               A531      211
c     BITMAP(NWORD+1) = BITMAP(NWORD+1) + I*(1-MOD(BITMAP(NWORD+1)/I,2)  A531      212
c    *  )                                                                A531      213
c     RETURN                                                             A531      214
c     END                                                                A531      215
C     *******************************************************************
c     FUNCTION IGET(BITMAP, N)                                           A531      216
C                                                                        A531      217
C     IGET=0 IF THE NTH BIT OF BITMAP IS ZERO, ELSE IGET IS ONE.         A531      218
C                                                                        A531      219
c     INTEGER BITMAP(1), N                                               A531      220
C                                                                        A531      221
c     DATA NBPW /31/                                                     A531      222
C     NBPW IS THE MINIMUM NUMBER OF SIGNIFICANT BITS PER WORD USED       A531      223
C     BY INTEGER ARITHMETIC.  THIS IS USUALLY ONE LESS THAN THE          A531      224
C     ACTUAL NUMBER OF BITS PER WORD, BUT AN IMPORTANT EXCEPTION IS      A531      225
C     THE CDC-6000 SERIES OF MACHINES, WHERE NBPW SHOULD BE 48.          A531      226
C                                                                        A531      227
c     NWORD = (N-1)/NBPW                                                 A531      228
c     NBIT = MOD(N-1,NBPW)                                               A531      229
c     IGET = MOD(BITMAP(NWORD+1)/2**(NBPW-NBIT-1),2)                     A531      230
c     RETURN                                                             A531      231
c     END                                                                A531      232
C     *******************************************************************
      SUBROUTINE GCONTR(Z, NRZ, NX, NY, CV, NCV, ZMAX, BITMAP, DRAW)     A531      233
C                                                                        A531      234 
C     THIS SUBROUTINE DRAWS A CONTOUR THROUGH EQUAL VALUES OF AN ARRAY.  A531      235
C                                                                        A531      236
C     *****     FORMAL ARGUMENTS     *********************************** A531      237
C                                                                        A531      238
C     Z IS THE ARRAY FOR WHICH CONTOURS ARE TO BE DRAWN.  THE ELEMENTS   A531      239
C     OF Z ARE ASSUMED TO LIE UPON THE NODES OF A TOPOLOGICALLY          A531      240
C     RECTANGULAR COORDINATE SYSTEM - E.G. CARTESIAN, POLAR (EXCEPT      A531      241
C     THE ORIGIN), ETC.                                                  A531      242
C                                                                        A531      243
C     NRZ IS THE NUMBER OF ROWS DECLARED FOR Z IN THE CALLING PROGRAM.   A531      244
C                                                                        A531      245
C     NX IS THE LIMIT FOR THE FIRST SUBSCRIPT OF Z.                      A531      246
C                                                                        A531      247
C     NY IS THE LIMIT FOR THE SECOND SUBSCRIPT OF Z.                     A531      248
C                                                                        A531      249
C     CV ARE THE VALUES OF THE CONTOURS TO BE DRAWN.                     A531      250
C                                                                        A531      251
C     NCV IS THE NUMBER OF CONTOUR VALUES IN CV.                         A531      252
C                                                                        A531      253
C     ZMAX IS THE MAXIMUM VALUE OF Z FOR CONSIDERATION.  A VALUE OF      A531      254
C     Z(I,J) GREATER THAN ZMAX IS A SIGNAL THAT THAT POINT AND THE       A531      255
C     GRID LINE SEGMENTS RADIATING FROM THAT POINT TO IT'S NEIGHBORS     A531      256
C     ARE TO BE EXCLUDED FROM CONTOURING.                                A531      257
C                                                                        A531      258
C     BITMAP IS A WORK AREA LARGE ENOUGH TO HOLD 2*NX*NY*NCV BITS.  IT   A531      259
C     IS ACCESSED BY LOW-LEVEL ROUTINES, WHICH ARE DESCRIBED BELOW.      A531      260
C     LET J BE THE NUMBER OF USEFUL BITS IN EACH WORD OF BITMAP,         A531      261
C     AS DETERMINED BY THE USER MACHINE AND IMPLEMENTATION OF            A531      262
C     THE BITMAP MANIPULATION SUBPROGRAMS DESCRIBED BELOW.  THEN         A531      263
C     THE NUMBER OF WORDS REQUIRED FOR THE BITMAP IS THE FLOOR OF        A531      264
C         (2*NX*NY*NCV+J-1)/J.                                           A531      265
C                                                                        A531      266
C     DRAW IS A USER-PROVIDED SUBROUTINE USED TO DRAW CONTOURS.          A531      267
C     THE CALLING SEQUENCE FOR DRAW IS:                                  A531      268
C                                                                        A531      269
C         CALL DRAW (X,Y,IFLAG)                                          A531      270
C         LET NX = INTEGER PART OF X, FX = FRACTIONAL PART OF X.         A531      271
C         THEN X SHOULD BE INTERPRETED SUCH THAT INCREASES IN NX         A531      272
C         CORRESPOND TO INCREASES IN THE FIRST SUBSCRIPT OF Z, AND       A531      273
C         FX IS THE FRACTIONAL DISTANCE FROM THE ABSCISSA CORRESPONDING  A531      274
C         TO NX TO THE ABSCISSA CORRESPONDING TO NX+1,                   A531      275
C         AND Y SHOULD BE INTERPRETED SIMILARLY FOR THE SECOND           A531      276
C         SUBSCRIPT OF Z.                                                A531      277
C         THE LOW-ORDER DIGIT OF IFLAG WILL HAVE ONE OF THE VALUES:      A531      278
C             1 - CONTINUE A CONTOUR,                                    A531      279
C             2 - START A CONTOUR AT A BOUNDARY,                         A531      280
C             3 - START A CONTOUR NOT AT A BOUNDARY,                     A531      281
C             4 - FINISH A CONTOUR AT A BOUNDARY,                        A531      282
C             5 - FINISH A CLOSED CONTOUR (NOT AT A BOUNDARY).           A531      283
C                 NOTE THAT REQUESTS 1, 4 AND 5 ARE FOR PEN-DOWN         A531      284
C                 MOVES, AND THAT REQUESTS 2 AND 3 ARE FOR PEN-UP        A531      285
C                 MOVES.                                                 A531      286
C             6 - SET X AND Y TO THE APPROXIMATE 'PEN' POSITION, USING   A531      287
C                 THE NOTATION DISCUSSED ABOVE.  THIS CALL MAY BE        A531      288
C                 IGNORED, THE RESULT BEING THAT THE 'PEN' POSITION      A531      289
C                 IS TAKEN TO CORRESPOND TO Z(1,1).                      A531      290
C         IFLAG/10 IS THE CONTOUR NUMBER.                                A531      291
C                                                                        A531      292
C     *****     EXTERNAL SUBPROGRAMS     ******************************* A531      293
C                                                                        A531      294
C     DRAW IS THE USER-SUPPLIED LINE DRAWING SUBPROGRAM DESCRIBED ABOVE. A531      295
C     DRAW MAY BE SENSITIVE TO THE HOST COMPUTER AND TO THE PLOT DEVICE. A531      296
C     FILL0 IS USED TO FILL A BITMAP WITH ZEROES.  CALL FILL0 (BITMAP,N) A531      297
C     FILLS THE FIRST N BITS OF BITMAP WITH ZEROES.                      A531      298
C     MARK1 IS USED TO PLACE A 1 IN A SPECIFIC BIT OF THE BITMAP.        A531      299
C     CALL MARK1 (BITMAP,N) PUTS A 1 IN THE NTH BIT OF THE BITMAP.       A531      300
C     IGET IS USED TO DETERMINE THE SETTING OF A PARTICULAR BIT IN THE   A531      301
C     BITMAP.  I=IGET(BITMAP,N) SETS I TO ZERO IF THE NTH BIT OF THE     A531      302
C     BITMAP IS ZERO, AND SETS I TO ONE IF THE NTH BIT IS ONE.           A531      303
C     FILL0, MARK1 AND IGET ARE MACHINE SENSITIVE.                       A531      304
C                                                                        A531      305
C     ****************************************************************** A531      306
C                                                                        A531      307
      EXTERNAL DRAW
      REAL Z(NRZ,1), CV(1)                                               A531      308
C      REAL Z(NRZ,NY), CV(1)                                               A531      308
      INTEGER BITMAP(1)                                                  A531      309
      INTEGER L1(4), L2(4), IJ(2)                                        A531      310
      INTEGER LF1, LF2, LF3, LF4
C                                                                        A531      311
C     L1 AND L2 CONTAIN LIMITS USED DURING THE SPIRAL SEARCH FOR THE     A531      312
C     BEGINNING OF A CONTOUR.                                            A531      313
C     IJ STORES SUBCRIPTS USED DURING THE SPIRAL SEARCH.                 A531      314
C                                                                        A531      315
      INTEGER I1(2), I2(2), I3(6)                                        A531      316
C                                                                        A531      317
C     I1, I2 AND I3 ARE USED FOR SUBSCRIPT COMPUTATIONS DURING THE       A531      318
C     EXAMINATION OF LINES FROM Z(I,J) TO IT'S NEIGHBORS.                A531      319
C                                                                        A531      320
      REAL XINT(4)                                                       A531      321
C                                                                        A531      322
C     XINT IS USED TO MARK INTERSECTIONS OF THE CONTOUR UNDER            A531      323
C     CONSIDERATION WITH THE EDGES OF THE CELL BEING EXAMINED.           A531      324
C                                                                        A531      325
      REAL XY(2)                                                         A531      326
C                                                                        A531      327
C     XY IS USED TO COMPUTE COORDINATES FOR THE DRAW SUBROUTINE.         A531      328
C                                                                        A531      329
      EQUIVALENCE (L2(1),IMAX), (L2(2),JMAX), (L2(3),IMIN),              A531      330
     *  (L2(4),JMIN)                                                     A531      331
      EQUIVALENCE (IJ(1),I), (IJ(2),J)                                   A531      332
      EQUIVALENCE (XY(1),X), (XY(2),Y)                                   A531      333
C                                                                        A531      334
      DATA L1(3) /-1/, L1(4) /-1/                                        A531      335
      DATA I1 /1,0/, I2 /1,-1/, I3 /1,0,0,1,1,0/                         A531      336
      DATA LF1 /1/, LF2 /2/, LF3 /3/, LF4 /4/
C                                                                        A531      337
C     ******************************************************************
C     Ueberpruefen der Uebergabeparameter
C
C      WRITE ( 6, '(2X, F6.1)') Z(3,5)
C      WRITE ( 6, '(2X, F6.1)') Z(10,10)
C      WRITE ( 6, '(2X, I4)') NRZ
C      WRITE ( 6, '(2X, I4)') NX
C      WRITE ( 6, '(2X, I4)') NY
C      WRITE ( 6, '(2X, F6.1)') CV(2)
C      WRITE ( 6, '(2X, I4)') NCV
C      WRITE ( 6, '(2X, F6.1)') ZMAX
C
C     ******************************************************************
      L1(1) = NX                                                         A531      338
      L1(2) = NY                                                         A531      339
      DMAX = ZMAX                                                        A531      340
C                                                                        A531      341
C     SET THE CURRENT PEN POSITION.  THE DEFAULT POSITION CORRESPONDS    A531      342
C     TO Z(1,1).                                                         A531      343
C                                                                        A531      344
      X = 1.0                                                            A531      345
      Y = 1.0                                                            A531      346
      CALL DRAW(X, Y, 6)                                                 A531      347
      ICUR = MAX0(1,MIN0(INT(X),NX))                                     A531      348
      JCUR = MAX0(1,MIN0(INT(Y),NY))                                     A531      349
C                                                                        A531      350
C     CLEAR THE BITMAP                                                   A531      351
C                                                                        A531      352
C      WRITE ( 6, '(2X, I4, 2X, I4)') LF1, LF1
      CALL FILL0(BITMAP, 2*NX*NY*NCV)                                    A531      353
C      WRITE ( 6, '(2X, I4, 2X, I4)') LF1, LF2
C                                                                        A531      354
C     SEARCH ALONG A RECTANGULAR SPIRAL PATH FOR A LINE SEGMENT HAVING   A531      355
C     THE FOLLOWING PROPERTIES:                                          A531      356
C          1.  THE END POINTS ARE NOT EXCLUDED,                          A531      357
C          2.  NO MARK HAS BEEN RECORDED FOR THE SEGMENT,                A531      358
C          3.  THE VALUES OF Z AT THE ENDS OF THE SEGMENT ARE SUCH THAT  A531      359
C              ONE Z IS LESS THAN THE CURRENT CONTOUR VALUE, AND THE     A531      360
C              OTHER IS GREATER THAN OR EQUAL TO THE CURRENT CONTOUR     A531      361
C              VALUE.                                                    A531      362
C                                                                        A531      363
C     SEARCH ALL BOUNDARIES FIRST, THEN SEARCH INTERIOR LINE SEGMENTS.   A531      364
C     NOTE THAT THE INTERIOR LINE SEGMENTS NEAR EXCLUDED POINTS MAY BE   A531      365
C     BOUNDARIES.                                                        A531      366
C                                                                        A531      367
      IBKEY = 0                                                          A531      368
   10 I = ICUR                                                           A531      369
      J = JCUR                                                           A531      370
   20 IMAX = I                                                           A531      371
      IMIN = -I                                                          A531      372
      JMAX = J                                                           A531      373
      JMIN = -J                                                          A531      374
      IDIR = 0                                                           A531      375
C     DIRECTION ZERO IS +I, 1 IS +J, 2 IS -I, 3 IS -J.                   A531      376
   30 NXIDIR = IDIR + 1                                                  A531      377
      K = NXIDIR                                                         A531      378
      IF (NXIDIR.GT.3) NXIDIR = 0                                        A531      379
   40 I = IABS(I)                                                        A531      380
      J = IABS(J)                                                        A531      381
      IF (Z(I,J).GT.DMAX) GO TO 140                                      A531      382
      L = 1                                                              A531      383
C     L=1 MEANS HORIZONTAL LINE, L=2 MEANS VERTICAL LINE.                A531      384
C      WRITE ( 6, '(2X, I4, 2X, I4)') LF1, LF4
   50 IF (IJ(L).GE.L1(L)) GO TO 130                                      A531      385
      II = I + I1(L)                                                     A531      386
      JJ = J + I1(3-L)                                                   A531      387
C      WRITE ( 6, '(2X, I4, 2X, I4)') LF2, LF1
      IF (Z(II,JJ).GT.DMAX) GO TO 130                                    A531      388
      ASSIGN 100 TO JUMP                                                 A531      389
C     THE NEXT 15 STATEMENTS (OR SO) DETECT BOUNDARIES.                  A531      390
   60 IX = 1                                                             A531      391
      IF (IJ(3-L).EQ.1) GO TO 80                                         A531      392
      II = I - I1(3-L)                                                   A531      393
      JJ = J - I1(L)                                                     A531      394
      IF (Z(II,JJ).GT.DMAX) GO TO 70                                     A531      395
      II = I + I2(L)                                                     A531      396
      JJ = J + I2(3-L)                                                   A531      397
      IF (Z(II,JJ).LT.DMAX) IX = 0                                       A531      398
   70 IF (IJ(3-L).GE.L1(3-L)) GO TO 90                                   A531      399
   80 II = I + I1(3-L)                                                   A531      400
      JJ = J + I1(L)                                                     A531      401
      IF (Z(II,JJ).GT.DMAX) GO TO 90                                     A531      402
      IF (Z(I+1,J+1).LT.DMAX) GO TO JUMP, (100, 280)                     A531      403
   90 IX = IX + 2                                                        A531      404
      GO TO JUMP, (100, 280)                                             A531      405
  100 IF (IX.EQ.3) GO TO 130                                             A531      406
      IF (IX+IBKEY.EQ.0) GO TO 130                                       A531      407
C     NOW DETERMINE WHETHER THE LINE SEGMENT IS CROSSED BY THE CONTOUR.  A531      408
      II = I + I1(L)                                                     A531      409
      JJ = J + I1(3-L)                                                   A531      410
      Z1 = Z(I,J)                                                        A531      411
      Z2 = Z(II,JJ)                                                      A531      412
      DO 120 ICV=1,NCV                                                   A531      413
        IF (IGET(BITMAP,2*(NX*(NY*(ICV-1)+J-1)+I-1)+L).NE.0) GO TO 120   A531      414
        IF (CV(ICV).LE.AMIN1(Z1,Z2)) GO TO 110                           A531      415
        IF (CV(ICV).LE.AMAX1(Z1,Z2)) GO TO 190                           A531      416
C      WRITE ( 6, '(2X, I4)') LF3
  110   CALL MARK1(BITMAP, 2*(NX*(NY*(ICV-1)+J-1)+I-1)+L)                A531      417
C      WRITE ( 6, '(2X, I4)') LF4
  120 CONTINUE                                                           A531      418
  130 L = L + 1                                                          A531      419
      IF (L.LE.2) GO TO 50                                               A531      420
C      WRITE ( 6, '(2X, I4, 2X, I4)') LF4, LF4
  140 L = MOD(IDIR,2) + 1                                                A531      421
      IJ(L) = ISIGN(IJ(L),L1(K))                                         A531      422
C                                                                        A531      423
C     LINES FROM Z(I,J) TO Z(I+1,J) AND Z(I,J+1) ARE NOT SATISFACTORY.   A531      424
C     CONTINUE THE SPIRAL.                                               A531      425
C                                                                        A531      426
  150 IF (IJ(L).GE.L1(K)) GO TO 170                                      A531      427
      IJ(L) = IJ(L) + 1                                                  A531      428
      IF (IJ(L).GT.L2(K)) GO TO 160                                      A531      429
      GO TO 40                                                           A531      430
  160 L2(K) = IJ(L)                                                      A531      431
      IDIR = NXIDIR                                                      A531      432
      GO TO 30                                                           A531      433
  170 IF (IDIR.EQ.NXIDIR) GO TO 180                                      A531      434
      NXIDIR = NXIDIR + 1                                                A531      435
      IJ(L) = L1(K)                                                      A531      436
      K = NXIDIR                                                         A531      437
      L = 3 - L                                                          A531      438
      IJ(L) = L2(K)                                                      A531      439
      IF (NXIDIR.GT.3) NXIDIR = 0                                        A531      440
      GO TO 150                                                          A531      441
  180 IF (IBKEY.NE.0) RETURN                                             A531      442
      IBKEY = 1                                                          A531      443
      GO TO 10                                                           A531      444
C                                                                        A531      445
C     AN ACCEPTABLE LINE SEGMENT HAS BEEN FOUND.                         A531      446
C     FOLLOW THE CONTOUR UNTIL IT EITHER HITS A BOUNDARY OR CLOSES.      A531      447
C                                                                        A531      448
  190 IEDGE = L                                                          A531      449
      CVAL = CV(ICV)                                                     A531      450
      IF (IX.NE.1) IEDGE = IEDGE + 2                                     A531      451
      IFLAG = 2 + IBKEY                                                  A531      452
      XINT(IEDGE) = (CVAL-Z1)/(Z2-Z1)                                    A531      453
  200 XY(L) = FLOAT(IJ(L)) + XINT(IEDGE)                                 A531      454
      XY(3-L) = FLOAT(IJ(3-L))                                           A531      455
      CALL MARK1(BITMAP, 2*(NX*(NY*(ICV-1)+J-1)+I-1)+L)                  A531      456
      CALL DRAW(X, Y, IFLAG+10*ICV)                                      A531      457
      IF (IFLAG.LT.4) GO TO 210                                          A531      458
      ICUR = I                                                           A531      459
      JCUR = J                                                           A531      460
      GO TO 20                                                           A531      461
C                                                                        A531      462
C     CONTINUE A CONTOUR.  THE EDGES ARE NUMBERED CLOCKWISE WITH         A531      463
C     THE BOTTOM EDGE BEING EDGE NUMBER ONE.                             A531      464
C                                                                        A531      465
  210 NI = 1                                                             A531      466
      IF (IEDGE.LT.3) GO TO 220                                          A531      467
      I = I - I3(IEDGE)                                                  A531      468
      J = J - I3(IEDGE+2)                                                A531      469
  220 DO 250 K=1,4                                                       A531      470
        IF (K.EQ.IEDGE) GO TO 250                                        A531      471
        II = I + I3(K)                                                   A531      472
        JJ = J + I3(K+1)                                                 A531      473
        Z1 = Z(II,JJ)                                                    A531      474
        II = I + I3(K+1)                                                 A531      475
        JJ = J + I3(K+2)                                                 A531      476
        Z2 = Z(II,JJ)                                                    A531      477
        IF (CVAL.LE.AMIN1(Z1,Z2)) GO TO 250                              A531      478
        IF (CVAL.GT.AMAX1(Z1,Z2)) GO TO 250                              A531      479
        IF (K.EQ.1) GO TO 230                                            A531      480
        IF (K.NE.4) GO TO 240                                            A531      481
  230   ZZ = Z1                                                          A531      482
        Z1 = Z2                                                          A531      483
        Z2 = ZZ                                                          A531      484
  240   XINT(K) = (CVAL-Z1)/(Z2-Z1)                                      A531      485
        NI = NI + 1                                                      A531      486
        KS = K                                                           A531      487
  250 CONTINUE                                                           A531      488
      IF (NI.EQ.2) GO TO 260                                             A531      489
C                                                                        A531      490
C     THE CONTOUR CROSSES ALL FOUR EDGES OF THE CELL BEING EXAMINED.     A531      491
C     CHOOSE THE LINES TOP-TO-LEFT AND BOTTOM-TO-RIGHT IF THE            A531      492
C     INTERPOLATION POINT ON THE TOP EDGE IS LESS THAN THE INTERPOLATION A531      493
C     POINT ON THE BOTTOM EDGE.  OTHERWISE, CHOOSE THE OTHER PAIR.  THIS A531      494
C     METHOD PRODUCES THE SAME RESULTS IF THE AXES ARE REVERSED.  THE    A531      495
C     CONTOUR MAY CLOSE AT ANY EDGE, BUT MUST NOT CROSS ITSELF INSIDE    A531      496
C     ANY CELL.                                                          A531      497
C                                                                        A531      498
      KS = 5 - IEDGE                                                     A531      499
      IF (XINT(3).LT.XINT(1)) GO TO 260                                  A531      500
      KS = 3 - IEDGE                                                     A531      501
      IF (KS.LE.0) KS = KS + 4                                           A531      502
C                                                                        A531      503
C     DETERMINE WHETHER THE CONTOUR WILL CLOSE OR RUN INTO A BOUNDARY    A531      504
C     AT EDGE KS OF THE CURRENT CELL.                                    A531      505
C                                                                        A531      506
  260 L = KS                                                             A531      507
      IFLAG = 1                                                          A531      508
      ASSIGN 280 TO JUMP                                                 A531      509
      IF (KS.LT.3) GO TO 270                                             A531      510
      I = I + I3(KS)                                                     A531      511
      J = J + I3(KS+2)                                                   A531      512
      L = KS - 2                                                         A531      513
C      WRITE ( 6, '(2X, I4, 2X, I4)') LF4, LF1
  270 IF (IGET(BITMAP,2*(NX*(NY*(ICV-1)+J-1)+I-1)+L).EQ.0) GO TO 60      A531      514
      IFLAG = 5                                                          A531      515
      GO TO 290                                                          A531      516
  280 IF (IX.NE.0) IFLAG = 4                                             A531      517
  290 IEDGE = KS + 2                                                     A531      518
      IF (IEDGE.GT.4) IEDGE = IEDGE - 4                                  A531      519
      XINT(IEDGE) = XINT(KS)                                             A531      520
      GO TO 200                                                          A531      521
C                                                                        A531      522
      END                                                                A531      523
