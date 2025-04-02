
* ======================================================================
* NIST Guide to Available Math Software.
* Fullsource for module DERKF from package CMLIB.
* Retrieved from CAMSUN on Fri Jun 20 10:38:27 1997.
* ======================================================================
      SUBROUTINE DERKF(F,NEQ,T,Y,TOUT,INFO,RTOL,ATOL,IDID,RWORK,LRW,
     1   IWORK,LIW,RPAR,IPAR)
C***BEGIN PROLOGUE  DERKF
C***DATE WRITTEN   800501   (YYMMDD)
C***REVISION DATE  820801   (YYMMDD)
C***CATEGORY NO.  I1A1A
C***KEYWORDS  DEPAC,INITIAL VALUE,ODE,ORDINARY DIFFERENTIAL EQUATIONS,
C             RKF,RUNGE-KUTTA METHODS
C***AUTHOR  WATTS, H. A., (SNLA)
C           SHAMPINE, L. F., (SNLA)
C***PURPOSE  DERKF solves initial value problems in ordinary
C            differential equations.
C***DESCRIPTION
C
C   This is the Runge-Kutta code in the package of differential equation
C   solvers DEPAC, consisting of the codes DERKF, DEABM, and DEBDF.
C   Design of the package was by L. F. Shampine and H. A. Watts.
C   It is documented in
C        SAND-79-2374 , DEPAC - Design of a User Oriented Package of ODE
C                              Solvers.
C   DERKF is a driver for a modification of the code RKF45 written by
C             H. A. Watts and L. F. Shampine
C             Sandia Laboratories
C             Albuquerque, New Mexico 87185
C
C **********************************************************************
C **             DEPAC PACKAGE OVERVIEW           **
C **************************************************
C
C        You have a choice of three differential equation solvers from
C        DEPAC.  The following brief descriptions are meant to aid you
C        in choosing the most appropriate code for your problem.
C
C        DERKF is a fifth order Runge-Kutta code.  It is the simplest of
C        the three choices, both algorithmically and in the use of the
C        code.  DERKF is primarily designed to solve non-stiff and mild-
C        ly stiff differential equations when derivative evaluations are
C        not expensive.  It should generally not be used to get high
C        accuracy results nor answers at a great many specific points.
C        Because DERKF has very low overhead costs, it will usually
C        result in the least expensive integration when solving
C        problems requiring a modest amount of accuracy and having
C        equations that are not costly to evaluate.  DERKF attempts to
C        discover when it is not suitable for the task posed.
C
C        DEABM is a variable order (one through twelve) Adams code.
C        Its complexity lies somewhere between that of DERKF and DEBDF.
C        DEABM is primarily designed to solve non-stiff and mildly stiff
C        differential equations when derivative evaluations are
C        expensive, high accuracy results are needed or answers at
C        many specific points are required.  DEABM attempts to discover
C        when it is not suitable for the task posed.
C
C        DEBDF is a variable order (one through five) backward
C        differentiation formula code.  It is the most complicated of
C        the three choices.  DEBDF is primarily designed to solve stiff
C        differential equations at crude to moderate tolerances.
C        If the problem is very stIff at all, DERKF and DEABM will be
C        quite inefficient compared to DEBDF.  However, DEBDF will be
C        inefficient compared to DERKF and DEABM on non-stiff problems
C        because it uses much more storage, has a much larger overhead,
C        and the low order formulas will not give high accuracies
C        efficiently.
C
C        The concept of stiffness cannot be described in a few words.
C        If you do not know the problem to be stiff, try either DERKF
C        or DEABM.  Both of these codes will inform you of stiffness
C        when the cost of solving such problems becomes important.
C
C **********************************************************************
C ** ABSTRACT **
C **************
C
C   Subroutine DERKF uses a Runge-Kutta-Fehlberg (4,5) method to
C   integrate a system of NEQ first order ordinary differential
C   equations of the form
C                         DU/DX = F(X,U)
C   when the vector Y(*) of initial values for U(*) at X=T is given.
C   The subroutine integrates from T to TOUT. It is easy to continue the
C   integration to get results at additional TOUT.  This is the interval
C   mode of operation.  It is also easy for the routine to return with
C   the solution at each intermediate step on the way to TOUT.  This is
C   the intermediate-output mode of operation.
C
C   DERKF uses subprograms DERKFS, DEFEHL, HSTART, VNORM, R1MACH, and
C   the error handling routine XERRWV.  The only machine dependent
C   parameters to be assigned appear in R1MACH.
C
C **********************************************************************
C ** DESCRIPTION OF THE ARGUMENTS TO DERKF (AN OVERVIEW) **
C *********************************************************
C
C   The parameters are
C
C      F -- This is the name of a subroutine which you provide to
C             define the differential equations.
C
C      NEQ -- This is the number of (first order) differential
C             equations to be integrated.
C
C      T -- This is a value of the independent variable.
C
C      Y(*) -- This array contains the solution components at T.
C
C      TOUT -- This is a point at which a solution is desired.
C
C      INFO(*) -- The basic task of the code is to integrate the
C             differential equations from T to TOUT and return an
C             answer at TOUT.  INFO(*) is an integer array which is used
C             to communicate exactly how you want this task to be
C             carried out.
C
C      RTOL, ATOL -- These quantities represent relative and absolute
C             error tolerances which you provide to indicate how
C             accurately you wish the solution to be computed.  You may
C             choose them to be both scalars or else both vectors.
C
C      IDID -- This scalar quantity is an indicator reporting what
C             the code did.  You must monitor this integer variable to
C             decide what action to take next.
C
C      RWORK(*), LRW -- RWORK(*) is a real work array of length LRW
C             which provides the code with needed storage space.
C
C      IWORK(*), LIW -- IWORK(*) is an integer work array of length LIW
C             which provides the code with needed storage space.
C
C      RPAR, IPAR -- These are real and integer parameter arrays which
C             you can use for communication between your calling
C             program and the F subroutine.
C
C  Quantities which are used as input items are
C             NEQ, T, Y(*), TOUT, INFO(*),
C             RTOL, ATOL, LRW and LIW.
C
C  Quantities which may be altered by the code are
C             T, Y(*), INFO(1), RTOL, ATOL,
C             IDID, RWORK(*) and IWORK(*).
C
C **********************************************************************
C ** INPUT -- WHAT TO DO ON THE FIRST CALL TO DERKF **
C ****************************************************
C
C   The first call of the code is defined to be the start of each new
C   problem.  Read through the descriptions of all the following items,
C   provide sufficient storage space for designated arrays, set
C   appropriate variables for the initialization of the problem, and
C   give information about how you want the problem to be solved.
C
C
C      F -- Provide a subroutine of the form
C                               F(X,U,UPRIME,RPAR,IPAR)
C             to define the system of first order differential equations
C             which is to be solved.  For the given values of X and the
C             vector  U(*)=(U(1),U(2),...,U(NEQ)) , the subroutine must
C             evaluate the NEQ components of the system of differential
C             equations  DU/DX=F(X,U)  and store the derivatives in the
C             array UPRIME(*), that is,  UPRIME(I) = * DU(I)/DX *  for
C             equations I=1,...,NEQ.
C
C             Subroutine F must not alter X or U(*).  You must declare
C             the name F in an external statement in your program that
C             calls DERKF.  You must dimension U and UPRIME in F.
C
C             RPAR and IPAR are real and integer parameter arrays which
C             you can use for communication between your calling program
C             and subroutine F.  They are not used or altered by DERKF.
C             If you do not need RPAR or IPAR, ignore these parameters
C             by treating them as dummy arguments.  If you do choose to
C             use them, dimension them in your calling program and in F
C             as arrays of appropriate length.
C
C      NEQ -- Set it to the number of differential equations.
C             (NEQ .GE. 1)
C
C      T -- Set it to the initial point of the integration.
C             You must use a program variable for T because the code
C             changes its value.
C
C      Y(*) -- Set this vector to the initial values of the NEQ solution
C             components at the initial point.  You must dimension Y at
C             least NEQ in your calling program.
C
C      TOUT -- Set it to the first point at which a solution
C             is desired.  You can take TOUT = T, in which case the code
C             will evaluate the derivative of the solution at T and
C             return.  Integration either forward in T  (TOUT .GT. T) or
C             backward in T  (TOUT .LT. T)  is permitted.
C
C             The code advances the solution from T to TOUT using
C             step sizes which are automatically selected so as to
C             achieve the desired accuracy.  If you wish, the code will
C             return with the solution and its derivative following
C             each intermediate step (intermediate-output mode) so that
C             you can monitor them, but you still must provide TOUT in
C             accord with the basic aim of the code.
C
C             The first step taken by the code is a critical one
C             because it must reflect how fast the solution changes near
C             the initial point.  The code automatically selects an
C             initial step size which is practically always suitable for
C             the problem. by using the fact that the code will not step
C             past TOUT in the first step, you could, if necessary,
C             restrict the length of the initial step size.
C
C             For some problems it may not be permissible to integrate
C             past a point TSTOP because a discontinuity occurs there
C             or the solution or its derivative is not defined beyond
C             TSTOP.  Since DERKF will never step past a TOUT point,
C             you need only make sure that no TOUT lies beyond TSTOP.
C
C      INFO(*) -- Use the INFO array to give the code more details about
C             how you want your problem solved.  This array should be
C             dimensioned of length 15 to accomodate other members of
C             DEPAC or possible future extensions, though DERKF uses
C             only the first three entries.  You must respond to all of
C             the following items which are arranged as questions.  The
C             simplest use of the code corresponds to answering all
C             questions as YES ,i.e. setting all entries of INFO to 0.
C
C        INFO(1) -- This parameter enables the code to initialize
C               itself.  You must set it to indicate the start of every
C               new problem.
C
C            **** Is this the first call for this problem ...
C                  YES -- Set INFO(1) = 0
C                   NO -- Not applicable here.
C                         See below for continuation calls.  ****
C
C        INFO(2) -- How much accuracy you want of your solution
C               is specified by the error tolerances RTOL and ATOL.
C               The simplest use is to take them both to be scalars.
C               To obtain more flexibility, they can both be vectors.
C               The code must be told your choice.
C
C            **** Are both error tolerances RTOL, ATOL scalars ...
C                  YES -- Set INFO(2) = 0
C                         and input scalars for both RTOL and ATOL
C                   NO -- Set INFO(2) = 1
C                         and input arrays for both RTOL and ATOL ****
C
C        INFO(3) -- The code integrates from T in the direction
C               of TOUT by steps.  If you wish, it will return the
C               computed solution and derivative at the next
C               intermediate step (the intermediate-output mode).
C               This is a good way to proceed if you want to see the
C               behavior of the solution.  If you must have solutions at
C               a great many specific TOUT points, this code is
C               inefficient.  The code DEABM in DEPAC handles this task
C               more efficiently.
C
C            **** Do you want the solution only at
C                 TOUT (and not at the next intermediate step) ...
C                  YES -- Set INFO(3) = 0
C                   NO -- Set INFO(3) = 1 ****
C
C      RTOL, ATOL -- You must assign relative (RTOL) and absolute (ATOL)
C             error tolerances to tell the code how accurately you want
C             the solution to be computed.  They must be defined as
C             program variables because the code may change them.  You
C             have two choices --
C                  Both RTOL and ATOL are scalars. (INFO(2)=0)
C                  Both RTOL and ATOL are vectors. (INFO(2)=1)
C             In either case all components must be non-negative.
C
C             The tolerances are used by the code in a local error test
C             at each step which requires roughly that
C                     ABS(LOCAL ERROR) .LE. RTOL*ABS(Y)+ATOL
C             for each vector component.
C             (More specifically, a maximum norm is used to measure
C             the size of vectors, and the error test uses the average
C             of the magnitude of the solution at the beginning and end
C             of the step.)
C
C             The true (global) error is the difference between the true
C             solution of the initial value problem and the computed
C             approximation.  Practically all present day codes,
C             including this one, control the local error at each step
C             and do not even attempt to control the global error
C             directly.  Roughly speaking, they produce a solution Y(T)
C             which satisfies the differential equations with a
C             residual R(T),    DY(T)/DT = F(T,Y(T)) + R(T)   ,
C             and, almost always, R(T) is bounded by the error
C             tolerances.  Usually, but not always, the true accuracy of
C             the computed Y is comparable to the error tolerances. This
C             code will usually, but not always, deliver a more accurate
C             solution if you reduce the tolerances and integrate again.
C             By comparing two such solutions you can get a fairly
C             reliable idea of the true error in the solution at the
C             bigger tolerances.
C
C             Setting ATOL=0.0 results in a pure relative error test on
C             that component.  Setting RTOL=0.0 yields a pure absolute
C             error test on that component.  A mixed test with non-zero
C             RTOL and ATOL corresponds roughly to a relative error
C             test when the solution component is much bigger than ATOL
C             and to an absolute error test when the solution component
C             is smaller than the threshold ATOL.
C
C             Proper selection of the absolute error control parameters
C             ATOL  requires you to have some idea of the scale of the
C             solution components.  To acquire this information may mean
C             you will have to solve the problem more than once.  In
C             the absence of scale information, you should ask for some
C             relative accuracy in all the components (by setting  RTOL
C             values non-zero) and perhaps impose extremely small
C             absolute error tolerances to protect against the danger of
C             a solution component becoming zero.
C
C             The code will not attempt to compute a solution at an
C             accuracy unreasonable for the machine being used.  It will
C             advise you if you ask for too much accuracy and inform
C             you as to the maximum accuracy it believes possible.
C             If you want relative accuracies smaller than about
C             10**(-8), you should not ordinarily use DERKF.  The code
C             DEABM in DEPAC obtains stringent accuracies more
C             efficiently.
C
C      RWORK(*) -- Dimension this real work array of length LRW in your
C             calling program.
C
C      LRW -- Set it to the declared length of the RWORK array.
C             You must have  LRW .GE. 33+7*NEQ
C
C      IWORK(*) -- Dimension this integer work array of length LIW in
C             your calling program.
C
C      LIW -- Set it to the declared length of the IWORK array.
C             You must have  LIW .GE. 33
C
C      RPAR, IPAR -- These are parameter arrays, of real and integer
C             type, respectively.  You can use them for communication
C             between your program that calls DERKF and the  F
C             subroutine.  They are not used or altered by DERKF.  If
C             you do not need RPAR or IPAR, ignore these parameters by
C             treating them as dummy arguments.  If you do choose to use
C             them, dimension them in your calling program and in F as
C             arrays of appropriate length.
C
C **********************************************************************
C ** OUTPUT -- AFTER ANY RETURN FROM DERKF **
C *******************************************
C
C   The principal aim of the code is to return a computed solution at
C   TOUT, although it is also possible to obtain intermediate results
C   along the way.  To find out whether the code achieved its goal
C   or if the integration process was interrupted before the task was
C   completed, you must check the IDID parameter.
C
C
C      T -- The solution was successfully advanced to the
C             output value of T.
C
C      Y(*) -- Contains the computed solution approximation at T.
C             You may also be interested in the approximate derivative
C             of the solution at T.  It is contained in
C             RWORK(21),...,RWORK(20+NEQ).
C
C      IDID -- Reports what the code did
C
C                         *** Task Completed ***
C                   reported by positive values of IDID
C
C             IDID = 1 -- A step was successfully taken in the
C                       intermediate-output mode.  The code has not
C                       yet reached TOUT.
C
C             IDID = 2 -- The integration to TOUT was successfully
C                       completed (T=TOUT) by stepping exactly to TOUT.
C
C                         *** Task Interrupted ***
C                   reported by negative values of IDID
C
C             IDID = -1 -- A large amount of work has been expended.
C                       (500 steps attempted)
C
C             IDID = -2 -- The error tolerances are too stringent.
C
C             IDID = -3 -- The local error test cannot be satisfied
C                       because you specified a zero component in ATOL
C                       and the corresponding computed solution
C                       component is zero.  Thus, a pure relative error
C                       test is impossible for this component.
C
C             IDID = -4 -- The problem appears to be stiff.
C
C             IDID = -5 -- DERKF is being used very inefficiently
C                       because the natural step size is being
C                       restricted by too frequent output.
C
C             IDID = -6,-7,..,-32  -- not applicable for this code but
C                       used by other members of DEPAC or possible
C                       future extensions.
C
C                         *** Task Terminated ***
C                   reported by the value of IDID=-33
C
C             IDID = -33 -- The code has encountered trouble from which
C                       it cannot recover.  A message is printed
C                       explaining the trouble and control is returned
C                       to the calling program.  For example, this
C                       occurs when invalid input is detected.
C
C      RTOL, ATOL -- These quantities remain unchanged except when
C             IDID = -2.  In this case, the error tolerances have been
C             increased by the code to values which are estimated to be
C             appropriate for continuing the integration.  However, the
C             reported solution at T was obtained using the input values
C             of RTOL and ATOL.
C
C      RWORK, IWORK -- Contain information which is usually of no
C             interest to the user but necessary for subsequent calls.
C             However, you may find use for
C
C             RWORK(11)--which contains the step size H to be
C                        attempted on the next step.
C
C             RWORK(12)--If the tolerances have been increased by the
C                        code (IDID = -2) , they were multiplied by the
C                        value in RWORK(12).
C
C             RWORK(20+i)--which contains the approximate derivative
C                        of the solution component Y(I).  In DERKF, it
C                        is always obtained by calling subroutine F to
C                        evaluate the differential equation using T and
C                        Y(*).
C
C **********************************************************************
C ** INPUT -- WHAT TO DO TO CONTINUE THE INTEGRATION **
C **             (CALLS AFTER THE FIRST)             **
C *****************************************************
C
C        This code is organized so that subsequent calls to continue the
C        integration involve little (if any) additional effort on your
C        part.  You must monitor the IDID parameter to determine
C        what to do next.
C
C        Recalling that the principal task of the code is to integrate
C        from T to TOUT (the interval mode), usually all you will need
C        to do is specify a new TOUT upon reaching the current TOUT.
C
C        Do not alter any quantity not specifically permitted below,
C        in particular do not alter NEQ, T, Y(*), RWORK(*), IWORK(*) or
C        the differential equation in subroutine F.  Any such alteration
C        constitutes a new problem and must be treated as such, i.e.
C        you must start afresh.
C
C        You cannot change from vector to scalar error control or vice
C        versa (INFO(2)) but you can change the size of the entries of
C        RTOL, ATOL.  Increasing a tolerance makes the equation easier
C        to integrate.  Decreasing a tolerance will make the equation
C        harder to integrate and should generally be avoided.
C
C        You can switch from the intermediate-output mode to the
C        interval mode (INFO(3)) or vice versa at any time.
C
C        The parameter INFO(1) is used by the code to indicate the
C        beginning of a new problem and to indicate whether integration
C        is to be continued.  You must input the value  INFO(1) = 0
C        when starting a new problem.  You must input the value
C        INFO(1) = 1  if you wish to continue after an interrupted task.
C        Do not set  INFO(1) = 0  on a continuation call unless you
C        want the code to restart at the current T.
C
C                         *** Following a Completed Task ***
C         If
C             IDID = 1, call the code again to continue the integration
C                     another step in the direction of TOUT.
C
C             IDID = 2, define a new TOUT and call the code again.
C                     TOUT must be different from T.  You cannot change
C                     the direction of integration without restarting.
C
C                         *** Following an Interrupted Task ***
C                     To show the code that you realize the task was
C                     interrupted and that you want to continue, you
C                     must take appropriate action and reset INFO(1) = 1
C         If
C             IDID = -1, the code has attempted 500 steps.
C                     If you want to continue, set INFO(1) = 1 and
C                     call the code again.  An additional 500 steps
C                     will be allowed.
C
C             IDID = -2, the error tolerances RTOL, ATOL have been
C                     increased to values the code estimates appropriate
C                     for continuing.  You may want to change them
C                     yourself.  If you are sure you want to continue
C                     with relaxed error tolerances, set INFO(1)=1 and
C                     call the code again.
C
C             IDID = -3, a solution component is zero and you set the
C                     corresponding component of ATOL to zero.  If you
C                     are sure you want to continue, you must first
C                     alter the error criterion to use positive values
C                     for those components of ATOL corresponding to zero
C                     solution components, then set INFO(1)=1 and call
C                     the code again.
C
C             IDID = -4, the problem appears to be stiff.  It is very
C                     inefficient to solve such problems with DERKF.
C                     Code DEBDF in DEPAC handles this task efficiently.
C                     If you are absolutely sure you want to continue
C                     with DERKF, set INFO(1)=1 and call the code again.
C
C             IDID = -5, you are using DERKF very inefficiently by
C                     choosing output points TOUT so close together that
C                     the step size is repeatedly forced to be rather
C                     smaller than necessary.  If you are willing to
C                     accept solutions at the steps chosen by the code,
C                     a good way to proceed is to use the intermediate-
C                     output mode (setting INFO(3)=1).  If you must have
C                     solutions at so many specific TOUT points, the
C                     code DEABM in DEPAC handles this task efficiently.
C                     If you want to continue with DERKF, set INFO(1)=1
C                     and call the code again.
C
C             IDID = -6,-7,..,-32  --- cannot occur with this code but
C                     used by other members of DEPAC or possible future
C                     extensions.
C
C                         *** Following a Terminated Task ***
C         If
C             IDID = -33, you cannot continue the solution of this
C                     problem.  An attempt to do so will result in your
C                     run being terminated.
C***REFERENCES  SHAMPINE L.F., WATTS H.A., *DEPAC - DESIGN OF A USER
C                 ORIENTED PACKAGE OF ODE SOLVERS*, SAND79-2374, SANDIA
C                 LABORATORIES, 1979.
C               SHAMPINE L.F., WATTS H.A.,*PRACTICAL SOLUTION OF
C                 ORDINARY DIFFERENTIAL EQUATIONS BY RUNGE-KUTTA
C                 METHODS*, SAND76-0585, SANDIA LABORATORIES, 1976.
C***ROUTINES CALLED  DERKFS,XERRWV
C***END PROLOGUE  DERKF
C
C
      LOGICAL STIFF,NONSTF
C
      DIMENSION Y(NEQ),INFO(15),RTOL(1),ATOL(1),RWORK(LRW),IWORK(LIW),
     1          RPAR(1),IPAR(1)
C
      EXTERNAL F
C
C.......................................................................
C
C  INITIALIZE A COUNTER FOR MONITORING AN INFINITE LOOP PITFALL
C
      DATA INFLOP/0/
C
C.......................................................................
C
C     CHECK FOR AN APPARENT INFINITE LOOP
C
C***FIRST EXECUTABLE STATEMENT  DERKF
      IF (INFLOP .LT. 5) GO TO 5
      IF (T .NE. RWORK(21+NEQ)) GO TO 5
      CALL XERRWV( 'DERKF -- AN APPARENT INFINITE LOOP HAS BEEN DETECTED
     1.  YOU HAVE MADE           REPEATED CALLS AT  T = (R1) AND INTEGRA
     2TION HAS NOT ADVANCED.         CHECK THE WAY YOU HAVE SET PARAMETE
     3RS FOR THE CALL TO THE             CODE, PARTICULARLY INFO(1).', 2
     446,13,2,0,0,0,1,T,0.)
      RETURN
C
C     CHECK LRW AND LIW FOR SUFFICIENT STORAGE ALLOCATION
C
    5 IDID=0
      IF (LRW .GE. 30+7*NEQ) GO TO 10
      CALL XERRWV(  'DERKF -- LENGTH OF RWORK ARRAY MUST BE AT LEAST  30
     1 + 7*NEQ.  YOU HAVE         CALLED THE CODE WITH  LRW = (I1).',
     2112,1,1,1,LRW,0,0,0.,0.)
      IDID=-33
C
   10 IF (LIW .GE. 30) GO TO 25
      CALL XERRWV( 'DERKF -- LENGTH OF IWORK ARRAY MUST BE AT LEAST  30.
     1  YOU HAVE CALLED          THE CODE WITH  LIW = (I1).',          1
     205,2,1,1,LIW,0,0,0.,0.)
      IDID=-33
C
C     COMPUTE INDICES FOR THE SPLITTING OF THE RWORK ARRAY
C
   25 KH=11
      KTF=12
      KYP=21
      KTSTAR=KYP+NEQ
      KF1=KTSTAR+1
      KF2=KF1+NEQ
      KF3=KF2+NEQ
      KF4=KF3+NEQ
      KF5=KF4+NEQ
      KYS=KF5+NEQ
      KTO=KYS+NEQ
      KDI=KTO+1
      KU=KDI+1
      KRER=KU+1
C
C **********************************************************************
C     THIS INTERFACING ROUTINE MERELY RELIEVES THE USER OF A LONG
C     CALLING LIST VIA THE SPLITTING APART OF TWO WORKING STORAGE
C     ARRAYS. IF THIS IS NOT COMPATIBLE WITH THE USERS COMPILER,
C     HE MUST USE DERKFS DIRECTLY.
C **********************************************************************
C
      RWORK(KTSTAR)=T
      IF (INFO(1) .EQ. 0) GO TO 50
      STIFF = (IWORK(25) .EQ. 0)
      NONSTF = (IWORK(26) .EQ. 0)
C
   50 CALL DERKFS(F,NEQ,T,Y,TOUT,INFO,RTOL,ATOL,IDID,RWORK(KH),
     1           RWORK(KTF),RWORK(KYP),RWORK(KF1),RWORK(KF2),RWORK(KF3),
     2           RWORK(KF4),RWORK(KF5),RWORK(KYS),RWORK(KTO),RWORK(KDI),
     3           RWORK(KU),RWORK(KRER),IWORK(21),IWORK(22),IWORK(23),
     4           IWORK(24),STIFF,NONSTF,IWORK(27),IWORK(28),RPAR,IPAR)
C
      IWORK(25)=1
      IF (STIFF) IWORK(25)=0
      IWORK(26)=1
      IF (NONSTF) IWORK(26)=0
C
      IF (IDID .NE. (-2)) INFLOP=INFLOP+1
      IF (T .NE. RWORK(KTSTAR)) INFLOP=0
C
      RETURN
      END
      SUBROUTINE DERKFS(F,NEQ,T,Y,TOUT,INFO,RTOL,ATOL,IDID,H,TOLFAC,YP,
     1   F1,F2,F3,F4,F5,YS,TOLD,DTSIGN,U26,RER,INIT,KSTEPS,KOP,IQUIT,
     2   STIFF,NONSTF,NTSTEP,NSTIFS,RPAR,IPAR)
C***BEGIN PROLOGUE  DERKFS
C***REFER TO  DERKF
C
C     Fehlberg Fourth-Fifth order Runge-Kutta Method
C     DERKFS integrates a system of first order ordinary differential
C     equations as described in the comments for DERKF .
C
C     The arrays YP,F1,F2,F3,F4,F5,and YS  (of length at least NEQ)
C     appear in the call list for variable dimensioning purposes.
C
C     The variables H,TOLFAC,TOLD,DTSIGN,U26,RER,INIT,KSTEPS,KOP,IQUIT,
C     STIFF,NONSTF,NTSTEP, and NSTIFS are used internally by the code
C     and appear in the call list to eliminate local retention of
C     variables between calls. Accordingly, these variables and the
C     array YP should not be altered.
C     Items of possible interest are
C         H  - An appropriate step size to be used for the next step
C         TOLFAC - Factor of change in the tolerances
C         YP - Derivative of solution vector at T
C         KSTEPS - Counter on the number of steps attempted
C***ROUTINES CALLED  DEFEHL,HSTART,R1MACH,VNORM,XERRWV
C***REVISION DATE  830630   (YYMMDD)
C***END PROLOGUE  DERKFS
C
C
      LOGICAL HFAILD,OUTPUT,STIFF,NONSTF
C
      DIMENSION Y(NEQ),YP(NEQ),F1(NEQ),F2(NEQ),F3(NEQ),F4(NEQ),F5(NEQ),
     1          YS(NEQ),INFO(15),RTOL(1),ATOL(1),RPAR(1),IPAR(1)
C
      EXTERNAL F
C
C.......................................................................
C
C  A FIFTH ORDER METHOD WILL GENERALLY NOT BE CAPABLE OF DELIVERING
C  ACCURACIES NEAR LIMITING PRECISION ON COMPUTERS WITH LONG
C  WORDLENGTHS. TO PROTECT AGAINST LIMITING PRECISION DIFFICULTIES
C  ARISING FROM UNREASONABLE ACCURACY REQUESTS, AN APPROPRIATE
C  TOLERANCE THRESHOLD REMIN IS ASSIGNED FOR THIS METHOD. THIS VALUE
C  SHOULD NOT BE CHANGED ACROSS DIFFERENT MACHINES.
C
      DATA REMIN/1.E-12/
C
C.......................................................................
C
C  THE EXPENSE OF SOLVING THE PROBLEM IS MONITORED BY COUNTING THE
C  NUMBER OF  STEPS ATTEMPTED. WHEN THIS EXCEEDS  MXSTEP, THE COUNTER
C  IS RESET TO ZERO AND THE USER IS INFORMED ABOUT POSSIBLE EXCESSIVE
C  WORK.
C
      DATA MXSTEP/500/
C
C.......................................................................
C
C  INEFFICIENCY CAUSED BY TOO FREQUENT OUTPUT IS MONITORED BY COUNTING
C  THE NUMBER OF STEP SIZES WHICH ARE SEVERELY SHORTENED DUE SOLELY TO
C  THE CHOICE OF OUTPUT POINTS. WHEN THE NUMBER OF ABUSES EXCEED MXKOP,
C  THE COUNTER IS RESET TO ZERO AND THE USER IS INFORMED ABOUT POSSIBLE
C  MISUSE OF THE CODE.
C
      DATA MXKOP/100/
C
C.......................................................................
C
C***FIRST EXECUTABLE STATEMENT  DERKFS
      IF (INFO(1) .NE. 0) GO TO 1
C
C ON THE FIRST CALL , PERFORM INITIALIZATION --
C        DEFINE THE MACHINE UNIT ROUNDOFF QUANTITY  U  BY CALLING THE
C        FUNCTION ROUTINE  R1MACH. THE USER MUST MAKE SURE THAT THE
C        VALUES SET IN R1MACH ARE RELEVANT TO THE COMPUTER BEING USED.
C
      U=R1MACH(4)
C                       -- SET ASSOCIATED MACHINE DEPENDENT PARAMETERS
      U26=26.*U
      RER=2.*U+REMIN
C                       -- SET TERMINATION FLAG
      IQUIT=0
C                       -- SET INITIALIZATION INDICATOR
      INIT=0
C                       -- SET COUNTER FOR IMPACT OF OUTPUT POINTS
      KOP=0
C                       -- SET COUNTER FOR ATTEMPTED STEPS
      KSTEPS=0
C                       -- SET INDICATORS FOR STIFFNESS DETECTION
      STIFF= .FALSE.
      NONSTF= .FALSE.
C                       -- SET STEP COUNTERS FOR STIFFNESS DETECTION
      NTSTEP=0
      NSTIFS=0
C                       -- RESET INFO(1) FOR SUBSEQUENT CALLS
      INFO(1)=1
C
C.......................................................................
C
C      CHECK VALIDITY OF INPUT PARAMATERS ON EACH ENTRY
C
    1 IF (INFO(1) .EQ. 0  .OR.  INFO(1) .EQ. 1) GO TO 2
      CALL XERRWV( 'DERKF -- INFO(1) MUST BE SET TO 0 FOR THE START OF A
     1 NEW PROBLEM, AND          MUST BE SET TO 1 FOLLOWING AN INTERRUPT
     2ED TASK.  YOU ARE              ATTEMPTING TO CONTINUE THE INTEGRAT
     3ION ILLEGALLY BY CALLING           THE CODE WITH  INFO(1) = (I1).'
     4,249,3,1,1,INFO(1),0,0,0.,0.)
      IDID=-33
C
    2 IF (INFO(2) .EQ. 0  .OR.  INFO(2) .EQ. 1) GO TO 3
      CALL XERRWV( 'DERKF -- INFO(2) MUST BE 0 OR 1 INDICATING SCALAR AN
     1D VECTOR ERROR              TOLERANCES, RESPECTIVELY.  YOU HAVE CA
     2LLED THE CODE WITH             INFO(2) = (I1).',                16
     34,4,1,1,INFO(2),0,0,0.,0.)
      IDID=-33
C
    3 IF (INFO(3) .EQ. 0  .OR.  INFO(3) .EQ. 1) GO TO 5
      CALL XERRWV( 'DERKF -- INFO(3) MUST BE 0 OR 1 INDICATING THE INTER
     1VAL OR                     INTERMEDIATE-OUTPUT MODE OF INTEGRATION
     2, RESPECTIVELY.                YOU HAVE CALLED THE CODE WITH  INFO
     3(3) = (I1).',195,5,1,1,INFO(3),0,0,0.,0.)
      IDID=-33
C
    5 IF (NEQ .GE. 1) GO TO 10
      CALL XERRWV( 'DERKF -- THE NUMBER OF EQUATIONS NEQ MUST BE A POSIT
     1IVE INTEGER.  YOU          HAVE CALLED THE CODE WITH  NEQ = (I1).'
     2,117,6,1,1,NEQ,0,0,0.,0.)
      IDID=-33
C
   10 NRTOLP=0
      NATOLP=0
      DO 15 K=1,NEQ
        IF (NRTOLP .GT. 0) GO TO 12
        IF (RTOL(K) .GE. 0.) GO TO 11
      CALL XERRWV(   'DERKF -- THE RELATIVE ERROR TOLERANCES RTOL MUST B
     1E NON-NEGATIVE.  YOU         HAVE CALLED THE CODE WITH  RTOL(I1) =
     2 (R1).  IN THE CASE OF           VECTOR ERROR TOLERANCES, NO FURTH
     3ER CHECKING OF RTOL                  COMPONENTS IS DONE.',
     4 238,7,1,1,K,0,1,RTOL(K),0.)
        IDID=-33
        IF (NATOLP .GT. 0) GO TO 20
        NRTOLP=1
        GO TO 12
   11   IF (NATOLP .GT. 0) GO TO 13
   12   IF (ATOL(K) .GE. 0.) GO TO 13
      CALL XERRWV(   'DERKF -- THE ABSOLUTE ERROR TOLERANCES ATOL MUST B
     1E NON-NEGATIVE.  YOU         HAVE CALLED THE CODE WITH  ATOL(I1) =
     2 (R1).  IN THE CASE OF           VECTOR ERROR TOLERANCES, NO FURTH
     3ER CHECKING OF ATOL                  COMPONENTS IS DONE.',
     4 238,8,1,1,K,0,1,ATOL(K),0.)
        IDID=-33
        IF (NRTOLP .GT. 0) GO TO 20
        NATOLP=1
   13   IF (INFO(2) .EQ. 0) GO TO 20
   15   CONTINUE
C
   20 IF (INIT .EQ. 0) GO TO 30
C                       CHECK SOME CONTINUATION POSSIBILITIES
      IF (T .NE. TOUT) GO TO 22
      CALL XERRWV( 'DERKF -- YOU HAVE CALLED THE CODE WITH  T = TOUT  AT
     1  T = (R1).  THIS          IS NOT ALLOWED ON CONTINUATION CALLS.',
     2116,9,1,0,0,0,1,T,0.)
      IDID=-33
C
   22 IF (T .EQ. TOLD) GO TO 25
      CALL XERRWV( 'DERKF -- YOU HAVE CHANGED THE VALUE OF T FROM (R1) T
     1O (R2).  THIS IS           NOT ALLOWED ON CONTINUATION CALLS.',  1
     213,10,1,0,0,0,2,TOLD,T)
      IDID=-33
C
   25 IF (INIT .EQ. 1) GO TO 30
      IF (DTSIGN*(TOUT-T) .GE. 0.) GO TO 30
      CALL XERRWV( 'DERKF -- BY CALLING THE CODE WITH  TOUT = (R1) , YOU
     1 ARE ATTEMPTING TO          CHANGE THE DIRECTION OF INTEGRATION. T
     2HIS IS NOT ALLOWED             WITHOUT RESTARTING.',             1
     368,11,1,0,0,0,1,TOUT,0.)
      IDID=-33
C
   30 IF (IDID .NE. (-33)) GO TO 40
      IF (IQUIT .EQ. (-33)) GO TO 33
C
C                       INVALID INPUT DETECTED
      IQUIT=-33
      GO TO 909
C
   33 CALL XERRWV(  'DERKF -- INVALID INPUT WAS DETECTED ON SUCCESSIVE E
     1NTRIES.  IT IS              IMPOSSIBLE TO PROCEED BECAUSE YOU HAVE
     2 NOT CORRECTED THE              PROBLEM, SO EXECUTION IS BEING TER
     3MINATED.',191,12,2,0,0,0,0,0.,0.)
      RETURN
C
C.......................................................................
C
C     RTOL = ATOL = 0. IS ALLOWED AS VALID INPUT AND INTERPRETED AS
C     ASKING FOR THE MOST ACCURATE SOLUTION POSSIBLE. IN THIS CASE,
C     THE RELATIVE ERROR TOLERANCE RTOL IS RESET TO THE SMALLEST VALUE
C     RER WHICH IS LIKELY TO BE REASONABLE FOR THIS METHOD AND MACHINE.
C
   40 DO 50 K=1,NEQ
        IF (RTOL(K)+ATOL(K) .GT. 0.) GO TO 45
        RTOL(K)=RER
        IDID=-2
   45   IF (INFO(2) .EQ. 0) GO TO 55
   50   CONTINUE
C
   55 IF (IDID .NE. (-2)) GO TO 60
C
C                       RTOL=ATOL=0 ON INPUT, SO RTOL WAS CHANGED TO A
C                                                SMALL POSITIVE VALUE
      TOLFAC=1.
      GO TO 909
C
C     BRANCH ON STATUS OF INITIALIZATION INDICATOR
C            INIT=0 MEANS INITIAL DERIVATIVES AND STARTING STEP SIZE
C                   NOT YET COMPUTED
C            INIT=1 MEANS STARTING STEP SIZE NOT YET COMPUTED
C            INIT=2 MEANS NO FUTHER INITIALIZATION REQUIRED
C
   60 IF (INIT .EQ. 0) GO TO 65
      IF (INIT .EQ. 1) GO TO 70
      GO TO 80
C
C.......................................................................
C
C     MORE INITIALIZATION --
C                         -- EVALUATE INITIAL DERIVATIVES
C
   65 INIT=1
      A=T
      CALL F(A,Y,YP,RPAR,IPAR)
      IF (T .EQ. TOUT) GO TO 666
C
C                         -- SET SIGN OF INTEGRATION DIRECTION  AND
C                         -- ESTIMATE STARTING STEP SIZE
C
   70 INIT=2
      DTSIGN=SIGN(1.,TOUT-T)
      U=R1MACH(4)
      BIG=SQRT(R1MACH(2))
      UTE=U**0.375
      DY=UTE*VNORM(Y,NEQ)
      IF (DY .EQ. 0.) DY=UTE
      KTOL=1
      DO 75 K=1,NEQ
        IF (INFO(2) .EQ. 1)  KTOL=K
        TOL=RTOL(KTOL)*ABS(Y(K))+ATOL(KTOL)
        IF (TOL .EQ. 0.) TOL=DY*RTOL(KTOL)
   75   F1(K)=TOL
C
      CALL HSTART (F,NEQ,T,TOUT,Y,YP,F1,4,U,BIG,F2,F3,F4,F5,RPAR,IPAR,H)
C
C.......................................................................
C
C     SET STEP SIZE FOR INTEGRATION IN THE DIRECTION FROM T TO TOUT
C     AND SET OUTPUT POINT INDICATOR
C
   80 DT=TOUT-T
      H=SIGN(H,DT)
      OUTPUT= .FALSE.
C
C     TEST TO SEE IF DERKF IS BEING SEVERELY IMPACTED BY TOO MANY
C     OUTPUT POINTS
C
      IF (ABS(H) .GE. 2.*ABS(DT)) KOP=KOP+1
      IF (KOP .LE. MXKOP) GO TO 85
C
C                       UNNECESSARY FREQUENCY OF OUTPUT IS RESTRICTING
C                                                 THE STEP SIZE CHOICE
      IDID=-5
      KOP=0
      GO TO 909
C
   85 IF (ABS(DT) .GT. U26*ABS(T)) GO TO 100
C
C     IF TOO CLOSE TO OUTPUT POINT,EXTRAPOLATE AND RETURN
C
      DO 90 K=1,NEQ
   90   Y(K)=Y(K)+DT*YP(K)
      A=TOUT
      CALL F(A,Y,YP,RPAR,IPAR)
      KSTEPS=KSTEPS+1
      GO TO 666
C
C **********************************************************************
C **********************************************************************
C     STEP BY STEP INTEGRATION
C
  100 HFAILD= .FALSE.
C
C     TO PROTECT AGAINST IMPOSSIBLE ACCURACY REQUESTS, COMPUTE A
C     TOLERANCE FACTOR BASED ON THE REQUESTED ERROR TOLERANCE AND A
C     LEVEL OF ACCURACY ACHIEVABLE AT LIMITING PRECISION
C
      TOLFAC=0.
      KTOL=1
      DO 125 K=1,NEQ
        IF (INFO(2) .EQ. 1) KTOL=K
        ET=RTOL(KTOL)*ABS(Y(K))+ATOL(KTOL)
        IF (ET .GT. 0.) GO TO 120
        TOLFAC=AMAX1(TOLFAC,RER/RTOL(KTOL))
        GO TO 125
  120   TOLFAC=AMAX1(TOLFAC,ABS(Y(K))*(RER/ET))
  125   CONTINUE
      IF (TOLFAC .LE. 1.) GO TO 150
C
C                       REQUESTED ERROR UNATTAINABLE DUE TO LIMITED
C                                               PRECISION AVAILABLE
      TOLFAC=2.*TOLFAC
      IDID=-2
      GO TO 909
C
C     SET SMALLEST ALLOWABLE STEP SIZE
C
  150 HMIN=U26*ABS(T)
C
C     ADJUST STEP SIZE IF NECESSARY TO HIT THE OUTPUT POINT --
C     LOOK AHEAD TWO STEPS TO AVOID DRASTIC CHANGES IN THE STEP SIZE AND
C     THUS LESSEN THE IMPACT OF OUTPUT POINTS ON THE CODE.
C     STRETCH THE STEP SIZE BY, AT MOST, AN AMOUNT EQUAL TO THE
C     SAFETY FACTOR OF 9/10.
C
      DT=TOUT-T
      IF (ABS(DT) .GE. 2.*ABS(H)) GO TO 200
      IF (ABS(DT) .GT. ABS(H)/0.9) GO TO 175
C
C     THE NEXT STEP, IF SUCCESSFUL, WILL COMPLETE THE INTEGRATION TO
C     THE OUTPUT POINT
C
      OUTPUT= .TRUE.
      H=DT
      GO TO 200
C
  175 H=0.5*DT
C
C
C **********************************************************************
C     CORE INTEGRATOR FOR TAKING A SINGLE STEP
C **********************************************************************
C     TO AVOID PROBLEMS WITH ZERO CROSSINGS,RELATIVE ERROR IS MEASURED
C     USING THE AVERAGE OF THE MAGNITUDES OF THE SOLUTION AT THE
C     BEGINNING AND END OF A STEP.
C     THE ERROR ESTIMATE FORMULA HAS BEEN GROUPED TO CONTROL LOSS OF
C     SIGNIFICANCE.
C     LOCAL ERROR ESTIMATES FOR A FIRST ORDER METHOD USING THE SAME
C     STEP SIZE AS THE FEHLBERG METHOD ARE CALCULATED AS PART OF THE
C     TEST FOR STIFFNESS.
C     TO DISTINGUISH THE VARIOUS ARGUMENTS, H IS NOT PERMITTED
C     TO BECOME SMALLER THAN 26 UNITS OF ROUNDOFF IN T.
C     PRACTICAL LIMITS ON THE CHANGE IN THE STEP SIZE ARE ENFORCED TO
C     SMOOTH THE STEP SIZE SELECTION PROCESS AND TO AVOID EXCESSIVE
C     CHATTERING ON PROBLEMS HAVING DISCONTINUITIES.
C     TO PREVENT UNNECESSARY FAILURES, THE CODE USES 9/10 THE STEP SIZE
C     IT ESTIMATES WILL SUCCEED.
C     AFTER A STEP FAILURE, THE STEP SIZE IS NOT ALLOWED TO INCREASE FOR
C     THE NEXT ATTEMPTED STEP. THIS MAKES THE CODE MORE EFFICIENT ON
C     PROBLEMS HAVING DISCONTINUITIES AND MORE EFFECTIVE IN GENERAL
C     SINCE LOCAL EXTRAPOLATION IS BEING USED AND EXTRA CAUTION SEEMS
C     WARRANTED.
C.......................................................................
C
C     MONITOR NUMBER OF STEPS ATTEMPTED
C
  200 IF (KSTEPS .LE. MXSTEP) GO TO 222
C
C                       A SIGNIFICANT AMOUNT OF WORK HAS BEEN EXPENDED
      IDID=-1
      KSTEPS=0
      IF (.NOT. STIFF) GO TO 909
C
C                       PROBLEM APPEARS TO BE STIFF
      IDID=-4
      STIFF= .FALSE.
      NONSTF= .FALSE.
      NTSTEP=0
      NSTIFS=0
      GO TO 909
C
C     ADVANCE AN APPROXIMATE SOLUTION OVER ONE STEP OF LENGTH H
C
  222 CALL DEFEHL(F,NEQ,T,Y,H,YP,F1,F2,F3,F4,F5,YS,RPAR,IPAR)
      KSTEPS=KSTEPS+1
C
C.......................................................................
C
C     COMPUTE AND TEST ALLOWABLE TOLERANCES VERSUS LOCAL ERROR
C     ESTIMATES.  NOTE THAT RELATIVE ERROR IS MEASURED WITH RESPECT TO
C     THE AVERAGE OF THE MAGNITUDES OF THE SOLUTION AT THE BEGINNING
C     AND END OF THE STEP.
C     LOCAL ERROR ESTIMATES FOR A SPECIAL FIRST ORDER METHOD ARE
C     CALCULATED ONLY WHEN THE STIFFNESS DETECTION IS TURNED ON.
C
      EEOET=0.
      ESTIFF=0.
      KTOL=1
      DO 350 K=1,NEQ
        YAVG=0.5*(ABS(Y(K))+ABS(YS(K)))
        IF (INFO(2) .EQ. 1) KTOL=K
        ET=RTOL(KTOL)*YAVG+ATOL(KTOL)
        IF (ET .GT. 0.) GO TO 325
C
C                       PURE RELATIVE ERROR INAPPROPRIATE WHEN SOLUTION
C                                                              VANISHES
        IDID=-3
        GO TO 909
C
  325   EE=ABS((-2090.*YP(K)+(21970.*F3(K)-15048.*F4(K)))+
     1                        (22528.*F2(K)-27360.*F5(K)))
        IF (STIFF .OR. NONSTF) GO TO 350
        ES=ABS(H*(0.055455*YP(K)-0.035493*F1(K)-0.036571*F2(K)+
     1            0.023107*F3(K)-0.009515*F4(K)+0.003017*F5(K)))
        ESTIFF=AMAX1(ESTIFF,ES/ET)
  350   EEOET=AMAX1(EEOET,EE/ET)
C
      ESTTOL=ABS(H)*EEOET/752400.
C
      IF (ESTTOL .LE. 1.) GO TO 500
C
C.......................................................................
C
C     UNSUCCESSFUL STEP
C
      IF (ABS(H) .GT. HMIN) GO TO 400
C
C                       REQUESTED ERROR UNATTAINABLE AT SMALLEST
C                                            ALLOWABLE STEP SIZE
      TOLFAC=1.69*ESTTOL
      IDID=-2
      GO TO 909
C
C                       REDUCE THE STEP SIZE , TRY AGAIN
C                       THE DECREASE IS LIMITED TO A FACTOR OF 1/10
C
  400 HFAILD= .TRUE.
      OUTPUT= .FALSE.
      S=0.1
      IF (ESTTOL .LT. 59049.) S=0.9/ESTTOL**0.2
      H=SIGN(AMAX1(S*ABS(H),HMIN),H)
      GO TO 200
C
C.......................................................................
C
C     SUCCESSFUL STEP
C                       STORE SOLUTION AT T+H
C                       AND EVALUATE DERIVATIVES THERE
C
  500 T=T+H
      DO 525 K=1,NEQ
  525   Y(K)=YS(K)
      A=T
      CALL F(A,Y,YP,RPAR,IPAR)
C
C                       CHOOSE NEXT STEP SIZE
C                       THE INCREASE IS LIMITED TO A FACTOR OF 5
C                       IF STEP FAILURE HAS JUST OCCURRED, NEXT
C                          STEP SIZE IS NOT ALLOWED TO INCREASE
C
      S=5.
      IF (ESTTOL .GT. 1.889568E-4) S=0.9/ESTTOL**0.2
      IF (HFAILD) S=AMIN1(S,1.)
      H=SIGN(AMAX1(S*ABS(H),HMIN),H)
C
C.......................................................................
C
C     CHECK FOR STIFFNESS (IF NOT ALREADY DETECTED)
C
C     IN A SEQUENCE OF 50 SUCCESSFUL STEPS BY THE FEHLBERG METHOD, 25
C     SUCCESSFUL STEPS BY THE FIRST ORDER METHOD INDICATES STIFFNESS
C     AND TURNS THE TEST OFF. IF 26 FAILURES BY THE FIRST ORDER METHOD
C     OCCUR, THE TEST IS TURNED OFF UNTIL THIS SEQUENCE OF 50 STEPS
C     BY THE FEHLBERG METHOD IS COMPLETED.
C
      IF (STIFF) GO TO 600
      NTSTEP=MOD(NTSTEP+1,50)
      IF (NTSTEP .EQ. 1) NONSTF= .FALSE.
      IF (NONSTF) GO TO 600
      IF (ESTIFF .GT. 1.) GO TO 550
C
C                       SUCCESSFUL STEP WITH FIRST ORDER METHOD
      NSTIFS=NSTIFS+1
C                       TURN TEST OFF AFTER 25 INDICATIONS OF STIFFNESS
      IF (NSTIFS .EQ. 25) STIFF= .TRUE.
      GO TO 600
C
C                       UNSUCCESSFUL STEP WITH FIRST ORDER METHOD
  550 IF (NTSTEP-NSTIFS .LE. 25) GO TO 600
C                       TURN STIFFNESS DETECTION OFF FOR THIS BLOCK OF
C                                                          FIFTY STEPS
      NONSTF= .TRUE.
C                       RESET STIFF STEP COUNTER
      NSTIFS=0
C
C **********************************************************************
C     END OF CORE INTEGRATOR
C **********************************************************************
C
C
C     SHOULD WE TAKE ANOTHER STEP
C
  600 IF (OUTPUT) GO TO 666
      IF (INFO(3) .EQ. 0) GO TO 100
C
C **********************************************************************
C **********************************************************************
C
C     INTEGRATION SUCCESSFULLY COMPLETED
C
C                 ONE-STEP MODE
      IDID=1
      TOLD=T
      RETURN
C
C                 INTERVAL MODE
  666 IDID=2
      T=TOUT
      TOLD=T
      RETURN
C
C     INTEGRATION TASK INTERRUPTED
C
  909 INFO(1)=-1
      TOLD=T
      IF (IDID .NE. (-2)) RETURN
C
C                       THE ERROR TOLERANCES ARE INCREASED TO VALUES
C                               WHICH ARE APPROPRIATE FOR CONTINUING
      RTOL(1)=TOLFAC*RTOL(1)
      ATOL(1)=TOLFAC*ATOL(1)
      IF (INFO(2) .EQ. 0) RETURN
      DO 939 K=2,NEQ
        RTOL(K)=TOLFAC*RTOL(K)
  939   ATOL(K)=TOLFAC*ATOL(K)
      RETURN
      END
      SUBROUTINE HSTART(F,NEQ,A,B,Y,YPRIME,ETOL,MORDER,SMALL,BIG,SPY,
     1   PV,YP,SF,RPAR,IPAR,H)
C***BEGIN PROLOGUE  HSTART
C***REFER TO  DEABM,DEBDF,DERKF
C
C   HSTART computes a starting step size to be used in solving initial
C   value problems in ordinary differential equations.
C **********************************************************************
C  Abstract
C
C     Subroutine HSTART computes a starting step size to be used by an
C     initial value method in solving ordinary differential equations.
C     It is based on an estimate of the local Lipschitz constant for the
C     differential equation   (lower bound on a norm of the Jacobian) ,
C     a bound on the differential equation  (first derivative) , and
C     a bound on the partial derivative of the equation with respect to
C     the independent variable.
C     (all approximated near the initial point A)
C
C     Subroutine HSTART uses a function subprogram VNORM for computing
C     a vector norm. The maximum norm is presently utilized though it
C     can easily be replaced by any other vector norm. It is presumed
C     that any replacement norm routine would be carefully coded to
C     prevent unnecessary underflows or overflows from occurring, and
C     also, would not alter the vector or number of components.
C
C **********************************************************************
C  On input you must provide the following
C
C      F -- This is a subroutine of the form
C                               F(X,U,UPRIME,RPAR,IPAR)
C             which defines the system of first order differential
C             equations to be solved. For the given values of X and the
C             vector  U(*)=(U(1),U(2),...,U(NEQ)) , the subroutine must
C             evaluate the NEQ components of the system of differential
C             equations  dU/DX=F(X,U)  and store the derivatives in the
C             array UPRIME(*), that is,  UPRIME(I) = * dU(I)/DX *  for
C             equations I=1,...,NEQ.
C
C             Subroutine F must NOT alter X or U(*). You must declare
C             the name F in an external statement in your program that
C             calls HSTART. You must dimension U and UPRIME in F.
C
C             RPAR and IPAR are real and integer parameter arrays which
C             you can use for communication between your program and
C             subroutine F. They are not used or altered by HSTART. If
C             you do not need RPAR or IPAR, ignore these parameters by
C             treating them as dummy arguments. If you do choose to use
C             them, dimension them in your program and in F as arrays
C             of appropriate length.
C
C      NEQ -- This is the number of (first order) differential equations
C             to be integrated.
C
C      A -- This is the initial point of integration.
C
C      B -- This is a value of the independent variable used to define
C             the direction of integration. A reasonable choice is to
C             set  B  to the first point at which a solution is desired.
C             you can also use  B, if necessary, to restrict the length
C             of the first integration step because the algorithm will
C             not compute a starting step length which is bigger than
C             ABS(B-A), unless  B  has been chosen too close to  A.
C             (it is presumed that HSTART has been called with  B
C             different from  A  on the machine being used. Also see the
C             discussion about the parameter  SMALL.)
C
C      Y(*) -- This is the vector of initial values of the NEQ solution
C             components at the initial point  A.
C
C      YPRIME(*) -- This is the vector of derivatives of the NEQ
C             solution components at the initial point  A.
C             (defined by the differential equations in subroutine F)
C
C      ETOL -- This is the vector of error tolerances corresponding to
C             the NEQ solution components. It is assumed that all
C             elements are positive. Following the first integration
C             step, the tolerances are expected to be used by the
C             integrator in an error test which roughly requires that
C                        ABS(local error) .LE. ETOL
C             for each vector component.
C
C      MORDER -- This is the order of the formula which will be used by
C             the initial value method for taking the first integration
C             step.
C
C      SMALL -- This is a small positive machine dependent constant
C             which is used for protecting against computations with
C             numbers which are too small relative to the precision of
C             floating point arithmetic.  SMALL  should be set to
C             (approximately) the smallest positive real number such
C             that  (1.+SMALL) .GT. 1.  on the machine being used. The
C             quantity  SMALL**(3/8)  is used in computing increments of
C             variables for approximating derivatives by differences.
C             Also the algorithm will not compute a starting step length
C             which is smaller than  100*SMALL*ABS(A).
C
C      BIG -- This is a large positive machine dependent constant which
C             is used for preventing machine overflows. A reasonable
C             choice is to set big to (approximately) the square root of
C             the largest real number which can be held in the machine.
C
C      SPY(*),PV(*),YP(*),SF(*) -- These are real work arrays of length
C             NEQ which provide the routine with needed storage space.
C
C      RPAR,IPAR -- These are parameter arrays, of real and integer
C             type, respectively, which can be used for communication
C             between your program and the F subroutine. They are not
C             used or altered by HSTART.
C
C **********************************************************************
C  On Output  (after the return from HSTART),
C
C      H -- Is an appropriate starting step size to be attempted by the
C             differential equation method.
C
C           All parameters in the call list remain unchanged except for
C           the working arrays SPY(*),PV(*),YP(*), and SF(*).
C
C **********************************************************************
C***ROUTINES CALLED  VNORM
C***END PROLOGUE  HSTART
C
C
      DIMENSION Y(NEQ),YPRIME(NEQ),ETOL(NEQ),
     1          SPY(NEQ),PV(NEQ),YP(NEQ),SF(NEQ)       ,RPAR(1),IPAR(1)
      EXTERNAL F
C
C.......................................................................
C
C***FIRST EXECUTABLE STATEMENT  HSTART
      DX=B-A
      ABSDX=ABS(DX)
      RELPER=SMALL**0.375
C
C.......................................................................
C
C     COMPUTE AN APPROXIMATE BOUND (DFDXB) ON THE PARTIAL
C     DERIVATIVE OF THE EQUATION WITH RESPECT TO THE
C     INDEPENDENT VARIABLE. PROTECT AGAINST AN OVERFLOW.
C     ALSO COMPUTE A BOUND (FBND) ON THE FIRST DERIVATIVE LOCALLY.
C
      DA=SIGN(AMAX1(AMIN1(RELPER*ABS(A),ABSDX),100.*SMALL*ABS(A)),DX)
      IF (DA .EQ. 0.) DA=RELPER*DX
      CALL F(A+DA,Y,SF,RPAR,IPAR)
      DO 10 J=1,NEQ
   10   YP(J)=SF(J)-YPRIME(J)
      DELF=VNORM(YP,NEQ)
      DFDXB=BIG
      IF (DELF .LT. BIG*ABS(DA)) DFDXB=DELF/ABS(DA)
      FBND=VNORM(SF,NEQ)
C
C.......................................................................
C
C     COMPUTE AN ESTIMATE (DFDUB) OF THE LOCAL LIPSCHITZ CONSTANT FOR
C     THE SYSTEM OF DIFFERENTIAL EQUATIONS. THIS ALSO REPRESENTS AN
C     ESTIMATE OF THE NORM OF THE JACOBIAN LOCALLY.
C     THREE ITERATIONS (TWO WHEN NEQ=1) ARE USED TO ESTIMATE THE
C     LIPSCHITZ CONSTANT BY NUMERICAL DIFFERENCES. THE FIRST
C     PERTURBATION VECTOR IS BASED ON THE INITIAL DERIVATIVES AND
C     DIRECTION OF INTEGRATION. THE SECOND PERTURBATION VECTOR IS
C     FORMED USING ANOTHER EVALUATION OF THE DIFFERENTIAL EQUATION.
C     THE THIRD PERTURBATION VECTOR IS FORMED USING PERTURBATIONS BASED
C     ONLY ON THE INITIAL VALUES. COMPONENTS THAT ARE ZERO ARE ALWAYS
C     CHANGED TO NON-ZERO VALUES (EXCEPT ON THE FIRST ITERATION). WHEN
C     INFORMATION IS AVAILABLE, CARE IS TAKEN TO ENSURE THAT COMPONENTS
C     OF THE PERTURBATION VECTOR HAVE SIGNS WHICH ARE CONSISTENT WITH
C     THE SLOPES OF LOCAL SOLUTION CURVES.
C     ALSO CHOOSE THE LARGEST BOUND (FBND) FOR THE FIRST DERIVATIVE.
C
C                       PERTURBATION VECTOR SIZE IS HELD CONSTANT FOR
C                       ALL ITERATIONS. COMPUTE THIS CHANGE FROM THE
C                               SIZE OF THE VECTOR OF INITIAL VALUES.
      DELY=RELPER*VNORM(Y,NEQ)
      IF (DELY .EQ. 0.) DELY=RELPER
      DELY=SIGN(DELY,DX)
      DELF=VNORM(YPRIME,NEQ)
      FBND=AMAX1(FBND,DELF)
      IF (DELF .EQ. 0.) GO TO 30
C                       USE INITIAL DERIVATIVES FOR FIRST PERTURBATION
      DO 20 J=1,NEQ
        SPY(J)=YPRIME(J)
   20   YP(J)=YPRIME(J)
      GO TO 50
C                       CANNOT HAVE A NULL PERTURBATION VECTOR
   30 DO 40 J=1,NEQ
        SPY(J)=0.
   40   YP(J)=1.
      DELF=VNORM(YP,NEQ)
C
   50 DFDUB=0.
      LK=MIN0(NEQ+1,3)
      DO 140 K=1,LK
C                       DEFINE PERTURBED VECTOR OF INITIAL VALUES
        DO 60 J=1,NEQ
   60     PV(J)=Y(J)+DELY*(YP(J)/DELF)
        IF (K .EQ. 2) GO TO 80
C                       EVALUATE DERIVATIVES ASSOCIATED WITH PERTURBED
C                       VECTOR  AND  COMPUTE CORRESPONDING DIFFERENCES
        CALL F(A,PV,YP,RPAR,IPAR)
        DO 70 J=1,NEQ
   70     PV(J)=YP(J)-YPRIME(J)
        GO TO 100
C                       USE A SHIFTED VALUE OF THE INDEPENDENT VARIABLE
C                                             IN COMPUTING ONE ESTIMATE
   80   CALL F(A+DA,PV,YP,RPAR,IPAR)
        DO 90 J=1,NEQ
   90     PV(J)=YP(J)-SF(J)
C                       CHOOSE LARGEST BOUNDS ON THE FIRST DERIVATIVE
C                                      AND A LOCAL LIPSCHITZ CONSTANT
  100   FBND=AMAX1(FBND,VNORM(YP,NEQ))
        DELF=VNORM(PV,NEQ)
        IF (DELF .GE. BIG*ABS(DELY)) GO TO 150
        DFDUB=AMAX1(DFDUB,DELF/ABS(DELY))
        IF (K .EQ. LK) GO TO 160
C                       CHOOSE NEXT PERTURBATION VECTOR
        IF (DELF .EQ. 0.) DELF=1.
        DO 130 J=1,NEQ
          IF (K .EQ. 2) GO TO 110
          DY=ABS(PV(J))
          IF (DY .EQ. 0.) DY=DELF
          GO TO 120
  110     DY=Y(J)
          IF (DY .EQ. 0.) DY=DELY/RELPER
  120     IF (SPY(J) .EQ. 0.) SPY(J)=YP(J)
          IF (SPY(J) .NE. 0.) DY=SIGN(DY,SPY(J))
  130     YP(J)=DY
  140   DELF=VNORM(YP,NEQ)
C
C                       PROTECT AGAINST AN OVERFLOW
  150 DFDUB=BIG
C
C.......................................................................
C
C     COMPUTE A BOUND (YDPB) ON THE NORM OF THE SECOND DERIVATIVE
C
  160 YDPB=DFDXB+DFDUB*FBND
C
C.......................................................................
C
C     DEFINE THE TOLERANCE PARAMETER UPON WHICH THE STARTING STEP SIZE
C     IS TO BE BASED.  A VALUE IN THE MIDDLE OF THE ERROR TOLERANCE
C     RANGE IS SELECTED.
C
      TOLMIN=BIG
      TOLSUM=0.
      DO 170 K=1,NEQ
        TOLEXP=ALOG10(ETOL(K))
        TOLMIN=AMIN1(TOLMIN,TOLEXP)
 170    TOLSUM=TOLSUM+TOLEXP
      TOLP=10.**(0.5*(TOLSUM/FLOAT(NEQ)+TOLMIN)/FLOAT(MORDER+1))
C
C.......................................................................
C
C     COMPUTE A STARTING STEP SIZE BASED ON THE ABOVE FIRST AND SECOND
C     DERIVATIVE INFORMATION
C
C                       RESTRICT THE STEP LENGTH TO BE NOT BIGGER THAN
C                       ABS(B-A).   (UNLESS  B  IS TOO CLOSE TO  A)
      H=ABSDX
C
      IF (YDPB .NE. 0.  .OR.  FBND .NE. 0.) GO TO 180
C
C                       BOTH FIRST DERIVATIVE TERM (FBND) AND SECOND
C                                    DERIVATIVE TERM (YDPB) ARE ZERO
      IF (TOLP .LT. 1.) H=ABSDX*TOLP
      GO TO 200
C
  180 IF (YDPB .NE. 0.) GO TO 190
C
C                       ONLY SECOND DERIVATIVE TERM (YDPB) IS ZERO
      IF (TOLP .LT. FBND*ABSDX) H=TOLP/FBND
      GO TO 200
C
C                       SECOND DERIVATIVE TERM (YDPB) IS NON-ZERO
  190 SRYDPB=SQRT(0.5*YDPB)
      IF (TOLP .LT. SRYDPB*ABSDX) H=TOLP/SRYDPB
C
C                       FURTHER RESTRICT THE STEP LENGTH TO BE NOT
C                                                 BIGGER THAN  1/DFDUB
  200 IF (H*DFDUB .GT. 1.) H=1./DFDUB
C
C                       FINALLY, RESTRICT THE STEP LENGTH TO BE NOT
C                       SMALLER THAN  100*SMALL*ABS(A).  HOWEVER, IF
C                       A=0. AND THE COMPUTED H UNDERFLOWED TO ZERO,
C                       THE ALGORITHM RETURNS  SMALL*ABS(B)  FOR THE
C                                                       STEP LENGTH.
      H=AMAX1(H,100.*SMALL*ABS(A))
      IF (H .EQ. 0.) H=SMALL*ABS(B)
C
C                       NOW SET DIRECTION OF INTEGRATION
      H=SIGN(H,DX)
C
      RETURN
      END
      FUNCTION VNORM(V,NCOMP)
C***BEGIN PROLOGUE  VNORM
C***REFER TO  DEABM,DEBDF,DERKF
C
C     Compute the maximum norm of the vector V(*) of length NCOMP and
C     return the result as VNORM.
C***ROUTINES CALLED  (NONE)
C***END PROLOGUE  VNORM
C
C
      DIMENSION V(NCOMP)
C***FIRST EXECUTABLE STATEMENT  VNORM
      VNORM=0.
      DO 10 K=1,NCOMP
   10   VNORM=AMAX1(VNORM,ABS(V(K)))
      RETURN
      END
      SUBROUTINE XERROR(MESSG,NMESSG,NERR,LEVEL)
C***BEGIN PROLOGUE  XERROR
C***DATE WRITTEN   790801   (YYMMDD)
C***REVISION DATE  820801   (YYMMDD)
C***CATEGORY NO.  R3C
C***KEYWORDS  ERROR,XERROR PACKAGE
C***AUTHOR  JONES, R. E., (SNLA)
C***PURPOSE  Processes an error (diagnostic) message.
C***DESCRIPTION
C     Abstract
C        XERROR processes a diagnostic message, in a manner
C        determined by the value of LEVEL and the current value
C        of the library error control flag, KONTRL.
C        (See subroutine XSETF for details.)
C
C     Description of Parameters
C      --Input--
C        MESSG - the Hollerith message to be processed, containing
C                no more than 72 characters.
C        NMESSG- the actual number of characters in MESSG.
C        NERR  - the error number associated with this message.
C                NERR must not be zero.
C        LEVEL - error category.
C                =2 means this is an unconditionally fatal error.
C                =1 means this is a recoverable error.  (I.e., it is
C                   non-fatal if XSETF has been appropriately called.)
C                =0 means this is a warning message only.
C                =-1 means this is a warning message which is to be
C                   printed at most once, regardless of how many
C                   times this call is executed.
C
C     Examples
C        CALL XERROR('SMOOTH -- NUM WAS ZERO.',23,1,2)
C        CALL XERROR('INTEG  -- LESS THAN FULL ACCURACY ACHIEVED.',
C                    43,2,1)
C        CALL XERROR('ROOTER -- ACTUAL ZERO OF F FOUND BEFORE INTERVAL F
C    1ULLY COLLAPSED.',65,3,0)
C        CALL XERROR('EXP    -- UNDERFLOWS BEING SET TO ZERO.',39,1,-1)
C
C     Latest revision ---  19 MAR 1980
C     Written by Ron Jones, with SLATEC Common Math Library Subcommittee
C***REFERENCES  JONES R.E., KAHANER D.K., "XERROR, THE SLATEC ERROR-
C                 HANDLING PACKAGE", SAND82-0800, SANDIA LABORATORIES,
C                 1982.
C***ROUTINES CALLED  XERRWV
C***END PROLOGUE  XERROR
      CHARACTER*(*) MESSG
C***FIRST EXECUTABLE STATEMENT  XERROR
      CALL XERRWV(MESSG,NMESSG,NERR,LEVEL,0,0,0,0,0.,0.)
      RETURN
      END
      SUBROUTINE XERRWV(MESSG,NMESSG,NERR,LEVEL,NI,I1,I2,NR,R1,R2)
C***BEGIN PROLOGUE  XERRWV
C***DATE WRITTEN   800319   (YYMMDD)
C***REVISION DATE  820801   (YYMMDD)
C***CATEGORY NO.  R3C
C***KEYWORDS  ERROR,XERROR PACKAGE
C***AUTHOR  JONES, R. E., (SNLA)
C***PURPOSE  Processes error message allowing 2 integer and two real
C            values to be included in the message.
C***DESCRIPTION
C     Abstract
C        XERRWV processes a diagnostic message, in a manner
C        determined by the value of LEVEL and the current value
C        of the library error control flag, KONTRL.
C        (See subroutine XSETF for details.)
C        In addition, up to two integer values and two real
C        values may be printed along with the message.
C
C     Description of Parameters
C      --Input--
C        MESSG - the Hollerith message to be processed.
C        NMESSG- the actual number of characters in MESSG.
C        NERR  - the error number associated with this message.
C                NERR must not be zero.
C        LEVEL - error category.
C                =2 means this is an unconditionally fatal error.
C                =1 means this is a recoverable error.  (I.e., it is
C                   non-fatal if XSETF has been appropriately called.)
C                =0 means this is a warning message only.
C                =-1 means this is a warning message which is to be
C                   printed at most once, regardless of how many
C                   times this call is executed.
C        NI    - number of integer values to be printed. (0 to 2)
C        I1    - first integer value.
C        I2    - second integer value.
C        NR    - number of real values to be printed. (0 to 2)
C        R1    - first real value.
C        R2    - second real value.
C
C     Examples
C        CALL XERRWV('SMOOTH -- NUM (=I1) WAS ZERO.',29,1,2,
C    1   1,NUM,0,0,0.,0.)
C        CALL XERRWV('QUADXY -- REQUESTED ERROR (R1) LESS THAN MINIMUM (
C    1R2).,54,77,1,0,0,0,2,ERRREQ,ERRMIN)
C
C     Latest revision ---  19 MAR 1980
C     Written by Ron Jones, with SLATEC Common Math Library Subcommittee
C***REFERENCES  JONES R.E., KAHANER D.K., "XERROR, THE SLATEC ERROR-
C                 HANDLING PACKAGE", SAND82-0800, SANDIA LABORATORIES,
C                 1982.
C***ROUTINES CALLED  FDUMP,I1MACH,J4SAVE,XERABT,XERCTL,XERPRT,XERSAV,
C                    XGETUA
C***END PROLOGUE  XERRWV
      CHARACTER*(*) MESSG
      CHARACTER*20 LFIRST
      CHARACTER*37 FORM
      DIMENSION LUN(5)
C     GET FLAGS
C***FIRST EXECUTABLE STATEMENT  XERRWV
      LKNTRL = J4SAVE(2,0,.FALSE.)
      MAXMES = J4SAVE(4,0,.FALSE.)
C     CHECK FOR VALID INPUT
      IF ((NMESSG.GT.0).AND.(NERR.NE.0).AND.
     1    (LEVEL.GE.(-1)).AND.(LEVEL.LE.2)) GO TO 10
         IF (LKNTRL.GT.0) CALL XERPRT('FATAL ERROR IN...',17)
         CALL XERPRT('XERROR -- INVALID INPUT',23)
         IF (LKNTRL.GT.0) CALL FDUMP
         IF (LKNTRL.GT.0) CALL XERPRT('JOB ABORT DUE TO FATAL ERROR.',
     1  29)
         IF (LKNTRL.GT.0) CALL XERSAV(' ',0,0,0,KDUMMY)
         CALL XERABT('XERROR -- INVALID INPUT',23)
         RETURN
   10 CONTINUE
C     RECORD MESSAGE
      JUNK = J4SAVE(1,NERR,.TRUE.)
      CALL XERSAV(MESSG,NMESSG,NERR,LEVEL,KOUNT)
C     LET USER OVERRIDE
      LFIRST = MESSG
      LMESSG = NMESSG
      LERR = NERR
      LLEVEL = LEVEL
      CALL XERCTL(LFIRST,LMESSG,LERR,LLEVEL,LKNTRL)
C     RESET TO ORIGINAL VALUES
      LMESSG = NMESSG
      LERR = NERR
      LLEVEL = LEVEL
      LKNTRL = MAX0(-2,MIN0(2,LKNTRL))
      MKNTRL = IABS(LKNTRL)
C     DECIDE WHETHER TO PRINT MESSAGE
      IF ((LLEVEL.LT.2).AND.(LKNTRL.EQ.0)) GO TO 100
      IF (((LLEVEL.EQ.(-1)).AND.(KOUNT.GT.MIN0(1,MAXMES)))
     1.OR.((LLEVEL.EQ.0)   .AND.(KOUNT.GT.MAXMES))
     2.OR.((LLEVEL.EQ.1)   .AND.(KOUNT.GT.MAXMES).AND.(MKNTRL.EQ.1))
     3.OR.((LLEVEL.EQ.2)   .AND.(KOUNT.GT.MAX0(1,MAXMES)))) GO TO 100
         IF (LKNTRL.LE.0) GO TO 20
            CALL XERPRT(' ',1)
C           INTRODUCTION
            IF (LLEVEL.EQ.(-1)) CALL XERPRT
     1('WARNING MESSAGE...THIS MESSAGE WILL ONLY BE PRINTED ONCE.',57)
            IF (LLEVEL.EQ.0) CALL XERPRT('WARNING IN...',13)
            IF (LLEVEL.EQ.1) CALL XERPRT
     1      ('RECOVERABLE ERROR IN...',23)
            IF (LLEVEL.EQ.2) CALL XERPRT('FATAL ERROR IN...',17)
   20    CONTINUE
C        MESSAGE
         CALL XERPRT(MESSG,LMESSG)
         CALL XGETUA(LUN,NUNIT)
         ISIZEI = LOG10(FLOAT(I1MACH(9))) + 1.0
         ISIZEF = LOG10(FLOAT(I1MACH(10))**I1MACH(11)) + 1.0
         DO 50 KUNIT=1,NUNIT
            IUNIT = LUN(KUNIT)
            IF (IUNIT.EQ.0) IUNIT = I1MACH(4)
            DO 22 I=1,MIN(NI,2)
               WRITE (FORM,21) I,ISIZEI
   21          FORMAT ('(11X,21HIN ABOVE MESSAGE, I',I1,'=,I',I2,')   ')
               IF (I.EQ.1) WRITE (IUNIT,FORM) I1
               IF (I.EQ.2) WRITE (IUNIT,FORM) I2
   22       CONTINUE
            DO 24 I=1,MIN(NR,2)
               WRITE (FORM,23) I,ISIZEF+10,ISIZEF
   23          FORMAT ('(11X,21HIN ABOVE MESSAGE, R',I1,'=,E',
     1         I2,'.',I2,')')
               IF (I.EQ.1) WRITE (IUNIT,FORM) R1
               IF (I.EQ.2) WRITE (IUNIT,FORM) R2
   24       CONTINUE
            IF (LKNTRL.LE.0) GO TO 40
C              ERROR NUMBER
               WRITE (IUNIT,30) LERR
   30          FORMAT (15H ERROR NUMBER =,I10)
   40       CONTINUE
   50    CONTINUE
C        TRACE-BACK
         IF (LKNTRL.GT.0) CALL FDUMP
  100 CONTINUE
      IFATAL = 0
      IF ((LLEVEL.EQ.2).OR.((LLEVEL.EQ.1).AND.(MKNTRL.EQ.2)))
     1IFATAL = 1
C     QUIT HERE IF MESSAGE IS NOT FATAL
      IF (IFATAL.LE.0) RETURN
      IF ((LKNTRL.LE.0).OR.(KOUNT.GT.MAX0(1,MAXMES))) GO TO 120
C        PRINT REASON FOR ABORT
         IF (LLEVEL.EQ.1) CALL XERPRT
     1   ('JOB ABORT DUE TO UNRECOVERED ERROR.',35)
         IF (LLEVEL.EQ.2) CALL XERPRT
     1   ('JOB ABORT DUE TO FATAL ERROR.',29)
C        PRINT ERROR SUMMARY
         CALL XERSAV(' ',-1,0,0,KDUMMY)
  120 CONTINUE
C     ABORT
      IF ((LLEVEL.EQ.2).AND.(KOUNT.GT.MAX0(1,MAXMES))) LMESSG = 0
      CALL XERABT(MESSG,LMESSG)
      RETURN
      END
      SUBROUTINE XERSAV(MESSG,NMESSG,NERR,LEVEL,ICOUNT)
C***BEGIN PROLOGUE  XERSAV
C***DATE WRITTEN   800319   (YYMMDD)
C***REVISION DATE  820801   (YYMMDD)
C***CATEGORY NO.  Z
C***KEYWORDS  ERROR,XERROR PACKAGE
C***AUTHOR  JONES, R. E., (SNLA)
C***PURPOSE  Records that an error occurred.
C***DESCRIPTION
C     Abstract
C        Record that this error occurred.
C
C     Description of Parameters
C     --Input--
C       MESSG, NMESSG, NERR, LEVEL are as in XERROR,
C       except that when NMESSG=0 the tables will be
C       dumped and cleared, and when NMESSG is less than zero the
C       tables will be dumped and not cleared.
C     --Output--
C       ICOUNT will be the number of times this message has
C       been seen, or zero if the table has overflowed and
C       does not contain this message specifically.
C       When NMESSG=0, ICOUNT will not be altered.
C
C     Written by Ron Jones, with SLATEC Common Math Library Subcommittee
C     Latest revision ---  19 Mar 1980
C***REFERENCES  JONES R.E., KAHANER D.K., "XERROR, THE SLATEC ERROR-
C                 HANDLING PACKAGE", SAND82-0800, SANDIA LABORATORIES,
C                 1982.
C***ROUTINES CALLED  I1MACH,S88FMT,XGETUA
C***END PROLOGUE  XERSAV
      INTEGER LUN(5)
      CHARACTER*(*) MESSG
      CHARACTER*20 MESTAB(10),MES
      DIMENSION NERTAB(10),LEVTAB(10),KOUNT(10)
      SAVE MESTAB,NERTAB,LEVTAB,KOUNT,KOUNTX
C     NEXT TWO DATA STATEMENTS ARE NECESSARY TO PROVIDE A BLANK
C     ERROR TABLE INITIALLY
      DATA KOUNT(1),KOUNT(2),KOUNT(3),KOUNT(4),KOUNT(5),
     1     KOUNT(6),KOUNT(7),KOUNT(8),KOUNT(9),KOUNT(10)
     2     /0,0,0,0,0,0,0,0,0,0/
      DATA KOUNTX/0/
C***FIRST EXECUTABLE STATEMENT  XERSAV
      IF (NMESSG.GT.0) GO TO 80
C     DUMP THE TABLE
         IF (KOUNT(1).EQ.0) RETURN
C        PRINT TO EACH UNIT
         CALL XGETUA(LUN,NUNIT)
         DO 60 KUNIT=1,NUNIT
            IUNIT = LUN(KUNIT)
            IF (IUNIT.EQ.0) IUNIT = I1MACH(4)
C           PRINT TABLE HEADER
            WRITE (IUNIT,10)
   10       FORMAT (32H0          ERROR MESSAGE SUMMARY/
     1      51H MESSAGE START             NERR     LEVEL     COUNT)
C           PRINT BODY OF TABLE
            DO 20 I=1,10
               IF (KOUNT(I).EQ.0) GO TO 30
               WRITE (IUNIT,15) MESTAB(I),NERTAB(I),LEVTAB(I),KOUNT(I)
   15          FORMAT (1X,A20,3I10)
   20       CONTINUE
   30       CONTINUE
C           PRINT NUMBER OF OTHER ERRORS
            IF (KOUNTX.NE.0) WRITE (IUNIT,40) KOUNTX
   40       FORMAT (41H0OTHER ERRORS NOT INDIVIDUALLY TABULATED=,I10)
            WRITE (IUNIT,50)
   50       FORMAT (1X)
   60    CONTINUE
         IF (NMESSG.LT.0) RETURN
C        CLEAR THE ERROR TABLES
         DO 70 I=1,10
   70       KOUNT(I) = 0
         KOUNTX = 0
         RETURN
   80 CONTINUE
C     PROCESS A MESSAGE...
C     SEARCH FOR THIS MESSG, OR ELSE AN EMPTY SLOT FOR THIS MESSG,
C     OR ELSE DETERMINE THAT THE ERROR TABLE IS FULL.
      MES = MESSG
      DO 90 I=1,10
         II = I
         IF (KOUNT(I).EQ.0) GO TO 110
         IF (MES.NE.MESTAB(I)) GO TO 90
         IF (NERR.NE.NERTAB(I)) GO TO 90
         IF (LEVEL.NE.LEVTAB(I)) GO TO 90
         GO TO 100
   90 CONTINUE
C     THREE POSSIBLE CASES...
C     TABLE IS FULL
         KOUNTX = KOUNTX+1
         ICOUNT = 1
         RETURN
C     MESSAGE FOUND IN TABLE
  100    KOUNT(II) = KOUNT(II) + 1
         ICOUNT = KOUNT(II)
         RETURN
C     EMPTY SLOT FOUND FOR NEW MESSAGE
  110    MESTAB(II) = MES
         NERTAB(II) = NERR
         LEVTAB(II) = LEVEL
         KOUNT(II)  = 1
         ICOUNT = 1
         RETURN
      END
      SUBROUTINE XGETUA(IUNITA,N)
C***BEGIN PROLOGUE  XGETUA
C***DATE WRITTEN   790801   (YYMMDD)
C***REVISION DATE  820801   (YYMMDD)
C***CATEGORY NO.  R3C
C***KEYWORDS  ERROR,XERROR PACKAGE
C***AUTHOR  JONES, R. E., (SNLA)
C***PURPOSE  Returns unit number(s) to which error messages are being
C            sent.
C***DESCRIPTION
C     Abstract
C        XGETUA may be called to determine the unit number or numbers
C        to which error messages are being sent.
C        These unit numbers may have been set by a call to XSETUN,
C        or a call to XSETUA, or may be a default value.
C
C     Description of Parameters
C      --Output--
C        IUNIT - an array of one to five unit numbers, depending
C                on the value of N.  A value of zero refers to the
C                default unit, as defined by the I1MACH machine
C                constant routine.  Only IUNIT(1),...,IUNIT(N) are
C                defined by XGETUA.  The values of IUNIT(N+1),...,
C                IUNIT(5) are not defined (for N .LT. 5) or altered
C                in any way by XGETUA.
C        N     - the number of units to which copies of the
C                error messages are being sent.  N will be in the
C                range from 1 to 5.
C
C     Latest revision ---  19 MAR 1980
C     Written by Ron Jones, with SLATEC Common Math Library Subcommittee
C***REFERENCES  JONES R.E., KAHANER D.K., "XERROR, THE SLATEC ERROR-
C                 HANDLING PACKAGE", SAND82-0800, SANDIA LABORATORIES,
C                 1982.
C***ROUTINES CALLED  J4SAVE
C***END PROLOGUE  XGETUA
      DIMENSION IUNITA(5)
C***FIRST EXECUTABLE STATEMENT  XGETUA
      N = J4SAVE(5,0,.FALSE.)
      DO 30 I=1,N
         INDEX = I+4
         IF (I.EQ.1) INDEX = 3
         IUNITA(I) = J4SAVE(INDEX,0,.FALSE.)
   30 CONTINUE
      RETURN
      END
      SUBROUTINE DEFEHL(F,NEQ,T,Y,H,YP,F1,F2,F3,F4,F5,YS,RPAR,IPAR)
C***BEGIN PROLOGUE  DEFEHL
C***REFER TO  DERKF
C
C     Fehlberg Fourth-Fifth order Runge-Kutta Method
C **********************************************************************
C
C    DEFEHL integrates a system of NEQ first order
C    ordinary differential equations of the form
C               dU/DX = F(X,U)
C    over one step when the vector Y(*) of initial values for U(*) and
C    the vector YP(*) of initial derivatives, satisfying  YP = F(T,Y),
C    are given at the starting point X=T.
C
C    DEFEHL advances the solution over the fixed step H and returns
C    the fifth order (sixth order accurate locally) solution
C    approximation at T+H in the array YS(*).
C    F1,---,F5 are arrays of dimension NEQ which are needed
C    for internal storage.
C    The formulas have been grouped to control loss of significance.
C    DEFEHL should be called with an H not smaller than 13 units of
C    roundoff in T so that the various independent arguments can be
C    distinguished.
C
C    This subroutine has been written with all variables and statement
C    numbers entirely compatible with DERKFS. For greater efficiency,
C    the call to DEFEHL can be replaced by the module beginning with
C    line 222 and extending to the last line just before the return
C    statement.
C
C **********************************************************************
C***ROUTINES CALLED  (NONE)
C***END PROLOGUE  DEFEHL
C
C
      DIMENSION Y(NEQ),YP(NEQ),F1(NEQ),F2(NEQ),F3(NEQ),F4(NEQ),F5(NEQ),
     1          YS(NEQ),RPAR(1),IPAR(1)
C
C***FIRST EXECUTABLE STATEMENT  DEFEHL
  222 CH=H/4.
      DO 230 K=1,NEQ
  230   YS(K)=Y(K)+CH*YP(K)
      CALL F(T+CH,YS,F1,RPAR,IPAR)
C
      CH=3.*H/32.
      DO 240 K=1,NEQ
  240   YS(K)=Y(K)+CH*(YP(K)+3.*F1(K))
      CALL F(T+3.*H/8.,YS,F2,RPAR,IPAR)
C
      CH=H/2197.
      DO 250 K=1,NEQ
  250   YS(K)=Y(K)+CH*(1932.*YP(K)+(7296.*F2(K)-7200.*F1(K)))
      CALL F(T+12.*H/13.,YS,F3,RPAR,IPAR)
C
      CH=H/4104.
      DO 260 K=1,NEQ
  260   YS(K)=Y(K)+CH*((8341.*YP(K)-845.*F3(K))+
     1                            (29440.*F2(K)-32832.*F1(K)))
      CALL F(T+H,YS,F4,RPAR,IPAR)
C
      CH=H/20520.
      DO 270 K=1,NEQ
  270   YS(K)=Y(K)+CH*((-6080.*YP(K)+(9295.*F3(K)-5643.*F4(K)))+
     1                             (41040.*F1(K)-28352.*F2(K)))
      CALL F(T+H/2.,YS,F5,RPAR,IPAR)
C
C     COMPUTE APPROXIMATE SOLUTION AT T+H
C
      CH=H/7618050.
      DO 290 K=1,NEQ
  290   YS(K)=Y(K)+CH*((902880.*YP(K)+(3855735.*F3(K)-1371249.*F4(K)))+
     1                (3953664.*F2(K)+277020.*F5(K)))
C
      RETURN
      END
      SUBROUTINE FDUMP
C***BEGIN PROLOGUE  FDUMP
C***DATE WRITTEN   790801   (YYMMDD)
C***REVISION DATE  820801   (YYMMDD)
C***CATEGORY NO.  Z
C***KEYWORDS  ERROR,XERROR PACKAGE
C***AUTHOR  JONES, R. E., (SNLA)
C***PURPOSE  Symbolic dump (should be locally written).
C***DESCRIPTION
C        ***Note*** Machine Dependent Routine
C        FDUMP is intended to be replaced by a locally written
C        version which produces a symbolic dump.  Failing this,
C        it should be replaced by a version which prints the
C        subprogram nesting list.  Note that this dump must be
C        printed on each of up to five files, as indicated by the
C        XGETUA routine.  See XSETUA and XGETUA for details.
C
C     Written by Ron Jones, with SLATEC Common Math Library Subcommittee
C     Latest revision ---  23 May 1979
C***ROUTINES CALLED  (NONE)
C***END PROLOGUE  FDUMP
C***FIRST EXECUTABLE STATEMENT  FDUMP
      RETURN
      END
      INTEGER FUNCTION I1MACH(I)
C***BEGIN PROLOGUE  I1MACH
C***DATE WRITTEN   750101   (YYMMDD)
C***REVISION DATE  910131   (YYMMDD)
C***CATEGORY NO.  R1
C***KEYWORDS  MACHINE CONSTANTS
C***AUTHOR  FOX, P. A., (BELL LABS)
C           HALL, A. D., (BELL LABS)
C           SCHRYER, N. L., (BELL LABS)
C***PURPOSE  Returns integer machine dependent constants
C***DESCRIPTION
C
C     This is the CMLIB version of I1MACH, the integer machine
C     constants subroutine originally developed for the PORT library.
C
C     I1MACH can be used to obtain machine-dependent parameters
C     for the local machine environment.  It is a function
C     subroutine with one (input) argument, and can be called
C     as follows, for example
C
C          K = I1MACH(I)
C
C     where I=1,...,16.  The (output) value of K above is
C     determined by the (input) value of I.  The results for
C     various values of I are discussed below.
C
C  I/O unit numbers.
C    I1MACH( 1) = the standard input unit.
C    I1MACH( 2) = the standard output unit.
C    I1MACH( 3) = the standard punch unit.
C    I1MACH( 4) = the standard error message unit.
C
C  Words.
C    I1MACH( 5) = the number of bits per integer storage unit.
C    I1MACH( 6) = the number of characters per integer storage unit.
C
C  Integers.
C    assume integers are represented in the S-digit, base-A form
C
C               sign ( X(S-1)*A**(S-1) + ... + X(1)*A + X(0) )
C
C               where 0 .LE. X(I) .LT. A for I=0,...,S-1.
C    I1MACH( 7) = A, the base.
C    I1MACH( 8) = S, the number of base-A digits.
C    I1MACH( 9) = A**S - 1, the largest magnitude.
C
C  Floating-Point Numbers.
C    Assume floating-point numbers are represented in the T-digit,
C    base-B form
C               sign (B**E)*( (X(1)/B) + ... + (X(T)/B**T) )
C
C               where 0 .LE. X(I) .LT. B for I=1,...,T,
C               0 .LT. X(1), and EMIN .LE. E .LE. EMAX.
C    I1MACH(10) = B, the base.
C
C  Single-Precision
C    I1MACH(11) = T, the number of base-B digits.
C    I1MACH(12) = EMIN, the smallest exponent E.
C    I1MACH(13) = EMAX, the largest exponent E.
C
C  Double-Precision
C    I1MACH(14) = T, the number of base-B digits.
C    I1MACH(15) = EMIN, the smallest exponent E.
C    I1MACH(16) = EMAX, the largest exponent E.
C
C  To alter this function for a particular environment,
C  the desired set of DATA statements should be activated by
C  removing the C from column 1.  Also, the values of
C  I1MACH(1) - I1MACH(4) should be checked for consistency
C  with the local operating system.
C***REFERENCES  FOX P.A., HALL A.D., SCHRYER N.L.,*FRAMEWORK FOR A
C                 PORTABLE LIBRARY*, ACM TRANSACTIONS ON MATHEMATICAL
C                 SOFTWARE, VOL. 4, NO. 2, JUNE 1978, PP. 177-188.
C***ROUTINES CALLED  (NONE)
C***END PROLOGUE  I1MACH
C
      INTEGER IMACH(16),OUTPUT
      EQUIVALENCE (IMACH(4),OUTPUT)
C
C     MACHINE CONSTANTS FOR IEEE ARITHMETIC MACHINES, SUCH AS THE AT&T
C     3B SERIES, MOTOROLA 68000 BASED MACHINES (E.G. SUN 3 AND AT&T
C     PC 7300), AND 8087 BASED MICROS (E.G. IBM PC AND AT&T 6300).
C
C === MACHINE = IEEE.MOST-SIG-BYTE-FIRST
C === MACHINE = IEEE.LEAST-SIG-BYTE-FIRST
C === MACHINE = SUN
C === MACHINE = 68000
C === MACHINE = 8087
C === MACHINE = IBM.PC
C === MACHINE = ATT.3B
C === MACHINE = ATT.7300
C === MACHINE = ATT.6300
       DATA IMACH( 1) /    5 /
       DATA IMACH( 2) /    6 /
       DATA IMACH( 3) /    7 /
       DATA IMACH( 4) /    6 /
       DATA IMACH( 5) /   32 /
       DATA IMACH( 6) /    4 /
       DATA IMACH( 7) /    2 /
       DATA IMACH( 8) /   31 /
       DATA IMACH( 9) / 2147483647 /
       DATA IMACH(10) /    2 /
       DATA IMACH(11) /   24 /
       DATA IMACH(12) / -125 /
       DATA IMACH(13) /  128 /
       DATA IMACH(14) /   53 /
       DATA IMACH(15) / -1021 /
       DATA IMACH(16) /  1024 /
C
C     MACHINE CONSTANTS FOR AMDAHL MACHINES.
C
C === MACHINE = AMDAHL
C      DATA IMACH( 1) /   5 /
C      DATA IMACH( 2) /   6 /
C      DATA IMACH( 3) /   7 /
C      DATA IMACH( 4) /   6 /
C      DATA IMACH( 5) /  32 /
C      DATA IMACH( 6) /   4 /
C      DATA IMACH( 7) /   2 /
C      DATA IMACH( 8) /  31 /
C      DATA IMACH( 9) / 2147483647 /
C      DATA IMACH(10) /  16 /
C      DATA IMACH(11) /   6 /
C      DATA IMACH(12) / -64 /
C      DATA IMACH(13) /  63 /
C      DATA IMACH(14) /  14 /
C      DATA IMACH(15) / -64 /
C      DATA IMACH(16) /  63 /
C
C     MACHINE CONSTANTS FOR THE BURROUGHS 1700 SYSTEM.
C
C === MACHINE = BURROUGHS.1700
C      DATA IMACH( 1) /    7 /
C      DATA IMACH( 2) /    2 /
C      DATA IMACH( 3) /    2 /
C      DATA IMACH( 4) /    2 /
C      DATA IMACH( 5) /   36 /
C      DATA IMACH( 6) /    4 /
C      DATA IMACH( 7) /    2 /
C      DATA IMACH( 8) /   33 /
C      DATA IMACH( 9) / Z1FFFFFFFF /
C      DATA IMACH(10) /    2 /
C      DATA IMACH(11) /   24 /
C      DATA IMACH(12) / -256 /
C      DATA IMACH(13) /  255 /
C      DATA IMACH(14) /   60 /
C      DATA IMACH(15) / -256 /
C      DATA IMACH(16) /  255 /
C
C     MACHINE CONSTANTS FOR THE BURROUGHS 5700 SYSTEM.
C
C === MACHINE = BURROUGHS.5700
C      DATA IMACH( 1) /   5 /
C      DATA IMACH( 2) /   6 /
C      DATA IMACH( 3) /   7 /
C      DATA IMACH( 4) /   6 /
C      DATA IMACH( 5) /  48 /
C      DATA IMACH( 6) /   6 /
C      DATA IMACH( 7) /   2 /
C      DATA IMACH( 8) /  39 /
C      DATA IMACH( 9) / O0007777777777777 /
C      DATA IMACH(10) /   8 /
C      DATA IMACH(11) /  13 /
C      DATA IMACH(12) / -50 /
C      DATA IMACH(13) /  76 /
C      DATA IMACH(14) /  26 /
C      DATA IMACH(15) / -50 /
C      DATA IMACH(16) /  76 /
C
C     MACHINE CONSTANTS FOR THE BURROUGHS 6700/7700 SYSTEMS.
C
C === MACHINE = BURROUGHS.6700
C === MACHINE = BURROUGHS.7700
C      DATA IMACH( 1) /   5 /
C      DATA IMACH( 2) /   6 /
C      DATA IMACH( 3) /   7 /
C      DATA IMACH( 4) /   6 /
C      DATA IMACH( 5) /  48 /
C      DATA IMACH( 6) /   6 /
C      DATA IMACH( 7) /   2 /
C      DATA IMACH( 8) /  39 /
C      DATA IMACH( 9) / O0007777777777777 /
C      DATA IMACH(10) /   8 /
C      DATA IMACH(11) /  13 /
C      DATA IMACH(12) / -50 /
C      DATA IMACH(13) /  76 /
C      DATA IMACH(14) /  26 /
C      DATA IMACH(15) / -32754 /
C      DATA IMACH(16) /  32780 /
C
C     MACHINE CONSTANTS FOR THE CONVEX C-120 (NATIVE MODE)
C
C === MACHINE = CONVEX.C1
C      DATA IMACH( 1) /    5 /
C      DATA IMACH( 2) /    6 /
C      DATA IMACH( 3) /    0 /
C      DATA IMACH( 4) /    6 /
C      DATA IMACH( 5) /   32 /
C      DATA IMACH( 6) /    4 /
C      DATA IMACH( 7) /    2 /
C      DATA IMACH( 8) /   31 /
C      DATA IMACH( 9) / 2147483647 /
C      DATA IMACH(10) /    2 /
C      DATA IMACH(11) /   24 /
C      DATA IMACH(12) / -127 /
C      DATA IMACH(13) /  127 /
C      DATA IMACH(14) /   53 /
C      DATA IMACH(15) / -1023 /
C      DATA IMACH(16) /  1023 /
C
C     MACHINE CONSTANTS FOR THE CONVEX (NATIVE MODE)
C     WITH -R8 OPTION
C
C === MACHINE = CONVEX.C1.R8
C      DATA IMACH( 1) /     5 /
C      DATA IMACH( 2) /     6 /
C      DATA IMACH( 3) /     0 /
C      DATA IMACH( 4) /     6 /
C      DATA IMACH( 5) /    32 /
C      DATA IMACH( 6) /     4 /
C      DATA IMACH( 7) /     2 /
C      DATA IMACH( 8) /    31 /
C      DATA IMACH( 9) / 2147483647 /
C      DATA IMACH(10) /     2 /
C      DATA IMACH(11) /    53 /
C      DATA IMACH(12) / -1023 /
C      DATA IMACH(13) /  1023 /
C      DATA IMACH(14) /    53 /
C      DATA IMACH(15) / -1023 /
C      DATA IMACH(16) /  1023 /
C
C     MACHINE CONSTANTS FOR THE CONVEX C-120 (IEEE MODE)
C
C === MACHINE = CONVEX.C1.IEEE
C      DATA IMACH( 1) /    5 /
C      DATA IMACH( 2) /    6 /
C      DATA IMACH( 3) /    0 /
C      DATA IMACH( 4) /    6 /
C      DATA IMACH( 5) /   32 /
C      DATA IMACH( 6) /    4 /
C      DATA IMACH( 7) /    2 /
C      DATA IMACH( 8) /   31 /
C      DATA IMACH( 9) / 2147483647 /
C      DATA IMACH(10) /    2 /
C      DATA IMACH(11) /   24 /
C      DATA IMACH(12) / -125 /
C      DATA IMACH(13) /  128 /
C      DATA IMACH(14) /   53 /
C      DATA IMACH(15) / -1021 /
C      DATA IMACH(16) /  1024 /
C
C     MACHINE CONSTANTS FOR THE CONVEX (IEEE MODE)
C     WITH -R8 OPTION
C
C === MACHINE = CONVEX.C1.IEEE.R8
C      DATA IMACH( 1) /     5 /
C      DATA IMACH( 2) /     6 /
C      DATA IMACH( 3) /     0 /
C      DATA IMACH( 4) /     6 /
C      DATA IMACH( 5) /    32 /
C      DATA IMACH( 6) /     4 /
C      DATA IMACH( 7) /     2 /
C      DATA IMACH( 8) /    31 /
C      DATA IMACH( 9) / 2147483647 /
C      DATA IMACH(10) /     2 /
C      DATA IMACH(11) /    53 /
C      DATA IMACH(12) / -1021 /
C      DATA IMACH(13) /  1024 /
C      DATA IMACH(14) /    53 /
C      DATA IMACH(15) / -1021 /
C      DATA IMACH(16) /  1024 /
C
C     MACHINE CONSTANTS FOR THE CYBER 170/180 SERIES USING NOS (FTN5).
C
C === MACHINE = CYBER.170.NOS
C === MACHINE = CYBER.180.NOS
C      DATA IMACH( 1) /    5 /
C      DATA IMACH( 2) /    6 /
C      DATA IMACH( 3) /    7 /
C      DATA IMACH( 4) /    6 /
C      DATA IMACH( 5) /   60 /
C      DATA IMACH( 6) /   10 /
C      DATA IMACH( 7) /    2 /
C      DATA IMACH( 8) /   48 /
C      DATA IMACH( 9) / O"00007777777777777777" /
C      DATA IMACH(10) /    2 /
C      DATA IMACH(11) /   48 /
C      DATA IMACH(12) / -974 /
C      DATA IMACH(13) / 1070 /
C      DATA IMACH(14) /   96 /
C      DATA IMACH(15) / -927 /
C      DATA IMACH(16) / 1070 /
C
C     MACHINE CONSTANTS FOR THE CDC 180 SERIES USING NOS/VE
C
C === MACHINE = CYBER.180.NOS/VE
C      DATA IMACH( 1) /     5 /
C      DATA IMACH( 2) /     6 /
C      DATA IMACH( 3) /     7 /
C      DATA IMACH( 4) /     6 /
C      DATA IMACH( 5) /    64 /
C      DATA IMACH( 6) /     8 /
C      DATA IMACH( 7) /     2 /
C      DATA IMACH( 8) /    63 /
C      DATA IMACH( 9) / 9223372036854775807 /
C      DATA IMACH(10) /     2 /
C      DATA IMACH(11) /    47 /
C      DATA IMACH(12) / -4095 /
C      DATA IMACH(13) /  4094 /
C      DATA IMACH(14) /    94 /
C      DATA IMACH(15) / -4095 /
C      DATA IMACH(16) /  4094 /
C
C     MACHINE CONSTANTS FOR THE CYBER 205
C
C === MACHINE = CYBER.205
C      DATA IMACH( 1) /      5 /
C      DATA IMACH( 2) /      6 /
C      DATA IMACH( 3) /      7 /
C      DATA IMACH( 4) /      6 /
C      DATA IMACH( 5) /     64 /
C      DATA IMACH( 6) /      8 /
C      DATA IMACH( 7) /      2 /
C      DATA IMACH( 8) /     47 /
C      DATA IMACH( 9) / X'00007FFFFFFFFFFF' /
C      DATA IMACH(10) /      2 /
C      DATA IMACH(11) /     47 /
C      DATA IMACH(12) / -28625 /
C      DATA IMACH(13) /  28718 /
C      DATA IMACH(14) /     94 /
C      DATA IMACH(15) / -28625 /
C      DATA IMACH(16) /  28718 /
C
C     MACHINE CONSTANTS FOR THE CDC 6000/7000 SERIES.
C
C === MACHINE = CDC.6000
C === MACHINE = CDC.7000
C      DATA IMACH( 1) /    5 /
C      DATA IMACH( 2) /    6 /
C      DATA IMACH( 3) /    7 /
C      DATA IMACH( 4) /    6 /
C      DATA IMACH( 5) /   60 /
C      DATA IMACH( 6) /   10 /
C      DATA IMACH( 7) /    2 /
C      DATA IMACH( 8) /   48 /
C      DATA IMACH( 9) / 00007777777777777777B /
C      DATA IMACH(10) /    2 /
C      DATA IMACH(11) /   48 /
C      DATA IMACH(12) / -974 /
C      DATA IMACH(13) / 1070 /
C      DATA IMACH(14) /   96 /
C      DATA IMACH(15) / -927 /
C      DATA IMACH(16) / 1070 /
C
C     MACHINE CONSTANTS FOR THE CRAY 1, XMP, 2, AND 3.
C     USING THE 46 BIT INTEGER COMPILER OPTION
C
C === MACHINE = CRAY.46-BIT-INTEGER
C      DATA IMACH( 1) /     5 /
C      DATA IMACH( 2) /     6 /
C      DATA IMACH( 3) /   102 /
C      DATA IMACH( 4) /     6 /
C      DATA IMACH( 5) /    64 /
C      DATA IMACH( 6) /     8 /
C      DATA IMACH( 7) /     2 /
C      DATA IMACH( 8) /    46 /
C      DATA IMACH( 9) /  777777777777777777777B /
C      DATA IMACH(10) /     2 /
C      DATA IMACH(11) /    47 /
C      DATA IMACH(12) / -8189 /
C      DATA IMACH(13) /  8190 /
C      DATA IMACH(14) /    94 /
C      DATA IMACH(15) / -8099 /
C      DATA IMACH(16) /  8190 /
C
C     MACHINE CONSTANTS FOR THE CRAY 1, XMP, 2, AND 3.
C     USING THE 64 BIT INTEGER COMPILER OPTION
C
C === MACHINE = CRAY.64-BIT-INTEGER
C      DATA IMACH( 1) /     5 /
C      DATA IMACH( 2) /     6 /
C      DATA IMACH( 3) /   102 /
C      DATA IMACH( 4) /     6 /
C      DATA IMACH( 5) /    64 /
C      DATA IMACH( 6) /     8 /
C      DATA IMACH( 7) /     2 /
C      DATA IMACH( 8) /    63 /
C      DATA IMACH( 9) /  777777777777777777777B /
C      DATA IMACH(10) /     2 /
C      DATA IMACH(11) /    47 /
C      DATA IMACH(12) / -8189 /
C      DATA IMACH(13) /  8190 /
C      DATA IMACH(14) /    94 /
C      DATA IMACH(15) / -8099 /
C      DATA IMACH(16) /  8190 /C
C     MACHINE CONSTANTS FOR THE DATA GENERAL ECLIPSE S/200
C
C === MACHINE = DATA_GENERAL.ECLIPSE.S/200
C      DATA IMACH( 1) /   11 /
C      DATA IMACH( 2) /   12 /
C      DATA IMACH( 3) /    8 /
C      DATA IMACH( 4) /   10 /
C      DATA IMACH( 5) /   16 /
C      DATA IMACH( 6) /    2 /
C      DATA IMACH( 7) /    2 /
C      DATA IMACH( 8) /   15 /
C      DATA IMACH( 9) /32767 /
C      DATA IMACH(10) /   16 /
C      DATA IMACH(11) /    6 /
C      DATA IMACH(12) /  -64 /
C      DATA IMACH(13) /   63 /
C      DATA IMACH(14) /   14 /
C      DATA IMACH(15) /  -64 /
C      DATA IMACH(16) /   63 /
C
C     ELXSI  6400
C
C === MACHINE = ELSXI.6400
C      DATA IMACH( 1) /     5 /
C      DATA IMACH( 2) /     6 /
C      DATA IMACH( 3) /     6 /
C      DATA IMACH( 4) /     6 /
C      DATA IMACH( 5) /    32 /
C      DATA IMACH( 6) /     4 /
C      DATA IMACH( 7) /     2 /
C      DATA IMACH( 8) /    32 /
C      DATA IMACH( 9) / 2147483647 /
C      DATA IMACH(10) /     2 /
C      DATA IMACH(11) /    24 /
C      DATA IMACH(12) /  -126 /
C      DATA IMACH(13) /   127 /
C      DATA IMACH(14) /    53 /
C      DATA IMACH(15) / -1022 /
C      DATA IMACH(16) /  1023 /
C
C     MACHINE CONSTANTS FOR THE HARRIS 220
C     MACHINE CONSTANTS FOR THE HARRIS SLASH 6 AND SLASH 7
C
C === MACHINE = HARRIS.220
C === MACHINE = HARRIS.SLASH6
C === MACHINE = HARRIS.SLASH7
C      DATA IMACH( 1) /       5 /
C      DATA IMACH( 2) /       6 /
C      DATA IMACH( 3) /       0 /
C      DATA IMACH( 4) /       6 /
C      DATA IMACH( 5) /      24 /
C      DATA IMACH( 6) /       3 /
C      DATA IMACH( 7) /       2 /
C      DATA IMACH( 8) /      23 /
C      DATA IMACH( 9) / 8388607 /
C      DATA IMACH(10) /       2 /
C      DATA IMACH(11) /      23 /
C      DATA IMACH(12) /    -127 /
C      DATA IMACH(13) /     127 /
C      DATA IMACH(14) /      38 /
C      DATA IMACH(15) /    -127 /
C      DATA IMACH(16) /     127 /
C
C     MACHINE CONSTANTS FOR THE HONEYWELL 600/6000 SERIES.
C     MACHINE CONSTANTS FOR THE HONEYWELL DPS 8/70 SERIES.
C
C === MACHINE = HONEYWELL.600/6000
C === MACHINE = HONEYWELL.DPS.8/70
C      DATA IMACH( 1) /    5 /
C      DATA IMACH( 2) /    6 /
C      DATA IMACH( 3) /   43 /
C      DATA IMACH( 4) /    6 /
C      DATA IMACH( 5) /   36 /
C      DATA IMACH( 6) /    4 /
C      DATA IMACH( 7) /    2 /
C      DATA IMACH( 8) /   35 /
C      DATA IMACH( 9) / O377777777777 /
C      DATA IMACH(10) /    2 /
C      DATA IMACH(11) /   27 /
C      DATA IMACH(12) / -127 /
C      DATA IMACH(13) /  127 /
C      DATA IMACH(14) /   63 /
C      DATA IMACH(15) / -127 /
C      DATA IMACH(16) /  127 /
C
C     MACHINE CONSTANTS FOR THE HP 2100
C     3 WORD DOUBLE PRECISION OPTION WITH FTN4
C
C === MACHINE = HP.2100.3_WORD_DP
C      DATA IMACH(1) /      5/
C      DATA IMACH(2) /      6 /
C      DATA IMACH(3) /      4 /
C      DATA IMACH(4) /      1 /
C      DATA IMACH(5) /     16 /
C      DATA IMACH(6) /      2 /
C      DATA IMACH(7) /      2 /
C      DATA IMACH(8) /     15 /
C      DATA IMACH(9) /  32767 /
C      DATA IMACH(10)/      2 /
C      DATA IMACH(11)/     23 /
C      DATA IMACH(12)/   -128 /
C      DATA IMACH(13)/    127 /
C      DATA IMACH(14)/     39 /
C      DATA IMACH(15)/   -128 /
C      DATA IMACH(16)/    127 /
C
C     MACHINE CONSTANTS FOR THE HP 2100
C     4 WORD DOUBLE PRECISION OPTION WITH FTN4
C
C === MACHINE = HP.2100.4_WORD_DP
C      DATA IMACH(1) /      5 /
C      DATA IMACH(2) /      6 /
C      DATA IMACH(3) /      4 /
C      DATA IMACH(4) /      1 /
C      DATA IMACH(5) /     16 /
C      DATA IMACH(6) /      2 /
C      DATA IMACH(7) /      2 /
C      DATA IMACH(8) /     15 /
C      DATA IMACH(9) /  32767 /
C      DATA IMACH(10)/      2 /
C      DATA IMACH(11)/     23 /
C      DATA IMACH(12)/   -128 /
C      DATA IMACH(13)/    127 /
C      DATA IMACH(14)/     55 /
C      DATA IMACH(15)/   -128 /
C      DATA IMACH(16)/    127 /
C
C     HP 9000
C
C === MACHINE = HP.9000
C      DATA IMACH( 1) /     5 /
C      DATA IMACH( 2) /     6 /
C      DATA IMACH( 3) /     6 /
C      DATA IMACH( 4) /     7 /
C      DATA IMACH( 5) /    32 /
C      DATA IMACH( 6) /     4 /
C      DATA IMACH( 7) /     2 /
C      DATA IMACH( 8) /    32 /
C      DATA IMACH( 9) / 2147483647 /
C      DATA IMACH(10) /     2 /
C      DATA IMACH(11) /    24 /
C      DATA IMACH(12) /  -126 /
C      DATA IMACH(13) /   127 /
C      DATA IMACH(14) /    53 /
C      DATA IMACH(15) / -1015 /
C      DATA IMACH(16) /  1017 /
C
C     MACHINE CONSTANTS FOR THE IBM 360/370 SERIES,
C     THE XEROX SIGMA 5/7/9 AND THE SEL SYSTEMS 85/86 AND
C     THE INTERDATA 3230 AND INTERDATA 7/32.
C
C === MACHINE = IBM.360
C === MACHINE = IBM.370
C === MACHINE = XEROX.SIGMA.5
C === MACHINE = XEROX.SIGMA.7
C === MACHINE = XEROX.SIGMA.9
C === MACHINE = SEL.85
C === MACHINE = SEL.86
C === MACHINE = INTERDATA.3230
C === MACHINE = INTERDATA.7/32
C      DATA IMACH( 1) /   5 /
C      DATA IMACH( 2) /   6 /
C      DATA IMACH( 3) /   7 /
C      DATA IMACH( 4) /   6 /
C      DATA IMACH( 5) /  32 /
C      DATA IMACH( 6) /   4 /
C      DATA IMACH( 7) /   2 /
C      DATA IMACH( 8) /  31 /
C      DATA IMACH( 9) / Z7FFFFFFF /
C      DATA IMACH(10) /  16 /
C      DATA IMACH(11) /   6 /
C      DATA IMACH(12) / -64 /
C      DATA IMACH(13) /  63 /
C      DATA IMACH(14) /  14 /
C      DATA IMACH(15) / -64 /
C      DATA IMACH(16) /  63 /
C
C     MACHINE CONSTANTS FOR THE INTERDATA 8/32
C     WITH THE UNIX SYSTEM FORTRAN 77 COMPILER.
C
C     FOR THE INTERDATA FORTRAN VII COMPILER REPLACE
C     THE Z'S SPECIFYING HEX CONSTANTS WITH Y'S.
C
C === MACHINE = INTERDATA.8/32.UNIX
C      DATA IMACH( 1) /   5 /
C      DATA IMACH( 2) /   6 /
C      DATA IMACH( 3) /   6 /
C      DATA IMACH( 4) /   6 /
C      DATA IMACH( 5) /  32 /
C      DATA IMACH( 6) /   4 /
C      DATA IMACH( 7) /   2 /
C      DATA IMACH( 8) /  31 /
C      DATA IMACH( 9) / Z'7FFFFFFF' /
C      DATA IMACH(10) /  16 /
C      DATA IMACH(11) /   6 /
C      DATA IMACH(12) / -64 /
C      DATA IMACH(13) /  62 /
C      DATA IMACH(14) /  14 /
C      DATA IMACH(15) / -64 /
C      DATA IMACH(16) /  62 /
C
C     MACHINE CONSTANTS FOR THE PDP-10 (KA PROCESSOR).
C
C === MACHINE = PDP-10.KA
C      DATA IMACH( 1) /    5 /
C      DATA IMACH( 2) /    6 /
C      DATA IMACH( 3) /    7 /
C      DATA IMACH( 4) /    6 /
C      DATA IMACH( 5) /   36 /
C      DATA IMACH( 6) /    5 /
C      DATA IMACH( 7) /    2 /
C      DATA IMACH( 8) /   35 /
C      DATA IMACH( 9) / "377777777777 /
C      DATA IMACH(10) /    2 /
C      DATA IMACH(11) /   27 /
C      DATA IMACH(12) / -128 /
C      DATA IMACH(13) /  127 /
C      DATA IMACH(14) /   54 /
C      DATA IMACH(15) / -101 /
C      DATA IMACH(16) /  127 /
C
C     MACHINE CONSTANTS FOR THE PDP-10 (KI PROCESSOR).
C
C === MACHINE = PDP-10.KI
C      DATA IMACH( 1) /    5 /
C      DATA IMACH( 2) /    6 /
C      DATA IMACH( 3) /    7 /
C      DATA IMACH( 4) /    6 /
C      DATA IMACH( 5) /   36 /
C      DATA IMACH( 6) /    5 /
C      DATA IMACH( 7) /    2 /
C      DATA IMACH( 8) /   35 /
C      DATA IMACH( 9) / "377777777777 /
C      DATA IMACH(10) /    2 /
C      DATA IMACH(11) /   27 /
C      DATA IMACH(12) / -128 /
C      DATA IMACH(13) /  127 /
C      DATA IMACH(14) /   62 /
C      DATA IMACH(15) / -128 /
C      DATA IMACH(16) /  127 /
C
C     MACHINE CONSTANTS FOR PDP-11 FORTRANS SUPPORTING
C     32-BIT INTEGER ARITHMETIC.
C
C === MACHINE = PDP-11.32-BIT
C      DATA IMACH( 1) /    5 /
C      DATA IMACH( 2) /    6 /
C      DATA IMACH( 3) /    7 /
C      DATA IMACH( 4) /    6 /
C      DATA IMACH( 5) /   32 /
C      DATA IMACH( 6) /    4 /
C      DATA IMACH( 7) /    2 /
C      DATA IMACH( 8) /   31 /
C      DATA IMACH( 9) / 2147483647 /
C      DATA IMACH(10) /    2 /
C      DATA IMACH(11) /   24 /
C      DATA IMACH(12) / -127 /
C      DATA IMACH(13) /  127 /
C      DATA IMACH(14) /   56 /
C      DATA IMACH(15) / -127 /
C      DATA IMACH(16) /  127 /
C
C     MACHINE CONSTANTS FOR PDP-11 FORTRANS SUPPORTING
C     16-BIT INTEGER ARITHMETIC.
C
C === MACHINE = PDP-11.16-BIT
C      DATA IMACH( 1) /    5 /
C      DATA IMACH( 2) /    6 /
C      DATA IMACH( 3) /    7 /
C      DATA IMACH( 4) /    6 /
C      DATA IMACH( 5) /   16 /
C      DATA IMACH( 6) /    2 /
C      DATA IMACH( 7) /    2 /
C      DATA IMACH( 8) /   15 /
C      DATA IMACH( 9) / 32767 /
C      DATA IMACH(10) /    2 /
C      DATA IMACH(11) /   24 /
C      DATA IMACH(12) / -127 /
C      DATA IMACH(13) /  127 /
C      DATA IMACH(14) /   56 /
C      DATA IMACH(15) / -127 /
C      DATA IMACH(16) /  127 /
C
C     MACHINE CONSTANTS FOR THE SEQUENT BALANCE 8000.
C
C === MACHINE = SEQUENT.BALANCE.8000
C      DATA IMACH( 1) /     0 /
C      DATA IMACH( 2) /     0 /
C      DATA IMACH( 3) /     7 /
C      DATA IMACH( 4) /     0 /
C      DATA IMACH( 5) /    32 /
C      DATA IMACH( 6) /     1 /
C      DATA IMACH( 7) /     2 /
C      DATA IMACH( 8) /    31 /
C      DATA IMACH( 9) /  2147483647 /
C      DATA IMACH(10) /     2 /
C      DATA IMACH(11) /    24 /
C      DATA IMACH(12) /  -125 /
C      DATA IMACH(13) /   128 /
C      DATA IMACH(14) /    53 /
C      DATA IMACH(15) / -1021 /
C      DATA IMACH(16) /  1024 /
C
C     MACHINE CONSTANTS FOR THE UNIVAC 1100 SERIES. FTN COMPILER
C
C === MACHINE = UNIVAC.1100
C      DATA IMACH( 1) /    5 /
C      DATA IMACH( 2) /    6 /
C      DATA IMACH( 3) /    1 /
C      DATA IMACH( 4) /    6 /
C      DATA IMACH( 5) /   36 /
C      DATA IMACH( 6) /    4 /
C      DATA IMACH( 7) /    2 /
C      DATA IMACH( 8) /   35 /
C      DATA IMACH( 9) / O377777777777 /
C      DATA IMACH(10) /    2 /
C      DATA IMACH(11) /   27 /
C      DATA IMACH(12) / -128 /
C      DATA IMACH(13) /  127 /
C      DATA IMACH(14) /   60 /
C      DATA IMACH(15) /-1024 /
C      DATA IMACH(16) / 1023 /
C
C     MACHINE CONSTANTS FOR THE VAX 11/780
C
C === MACHINE = VAX.11/780
C      DATA IMACH(1) /    5 /
C      DATA IMACH(2) /    6 /
C      DATA IMACH(3) /    5 /
C      DATA IMACH(4) /    6 /
C      DATA IMACH(5) /   32 /
C      DATA IMACH(6) /    4 /
C      DATA IMACH(7) /    2 /
C      DATA IMACH(8) /   31 /
C      DATA IMACH(9) /2147483647 /
C      DATA IMACH(10)/    2 /
C      DATA IMACH(11)/   24 /
C      DATA IMACH(12)/ -127 /
C      DATA IMACH(13)/  127 /
C      DATA IMACH(14)/   56 /
C      DATA IMACH(15)/ -127 /
C      DATA IMACH(16)/  127 /
C
C
C***FIRST EXECUTABLE STATEMENT  I1MACH
      IF (I .LT. 1  .OR.  I .GT. 16)
     1   CALL XERROR ( 'I1MACH -- I OUT OF BOUNDS',25,1,2)
C
      I1MACH=IMACH(I)
      RETURN
C
      END
      FUNCTION J4SAVE(IWHICH,IVALUE,ISET)
C***BEGIN PROLOGUE  J4SAVE
C***REFER TO  XERROR
C     Abstract
C        J4SAVE saves and recalls several global variables needed
C        by the library error handling routines.
C
C     Description of Parameters
C      --Input--
C        IWHICH - Index of item desired.
C                = 1 Refers to current error number.
C                = 2 Refers to current error control flag.
C                 = 3 Refers to current unit number to which error
C                    messages are to be sent.  (0 means use standard.)
C                 = 4 Refers to the maximum number of times any
C                     message is to be printed (as set by XERMAX).
C                 = 5 Refers to the total number of units to which
C                     each error message is to be written.
C                 = 6 Refers to the 2nd unit for error messages
C                 = 7 Refers to the 3rd unit for error messages
C                 = 8 Refers to the 4th unit for error messages
C                 = 9 Refers to the 5th unit for error messages
C        IVALUE - The value to be set for the IWHICH-th parameter,
C                 if ISET is .TRUE. .
C        ISET   - If ISET=.TRUE., the IWHICH-th parameter will BE
C                 given the value, IVALUE.  If ISET=.FALSE., the
C                 IWHICH-th parameter will be unchanged, and IVALUE
C                 is a dummy parameter.
C      --Output--
C        The (old) value of the IWHICH-th parameter will be returned
C        in the function value, J4SAVE.
C
C     Written by Ron Jones, with SLATEC Common Math Library Subcommittee
C    Adapted from Bell Laboratories PORT Library Error Handler
C     Latest revision ---  23 MAY 1979
C***REFERENCES  JONES R.E., KAHANER D.K., "XERROR, THE SLATEC ERROR-
C                 HANDLING PACKAGE", SAND82-0800, SANDIA LABORATORIES,
C                 1982.
C***ROUTINES CALLED  (NONE)
C***END PROLOGUE  J4SAVE
      LOGICAL ISET
      INTEGER IPARAM(9)
      SAVE IPARAM
      DATA IPARAM(1),IPARAM(2),IPARAM(3),IPARAM(4)/0,2,0,10/
      DATA IPARAM(5)/1/
      DATA IPARAM(6),IPARAM(7),IPARAM(8),IPARAM(9)/0,0,0,0/
C***FIRST EXECUTABLE STATEMENT  J4SAVE
      J4SAVE = IPARAM(IWHICH)
      IF (ISET) IPARAM(IWHICH) = IVALUE
      RETURN
      END
      SUBROUTINE XERABT(MESSG,NMESSG)
C***BEGIN PROLOGUE  XERABT
C***DATE WRITTEN   790801   (YYMMDD)
C***REVISION DATE  820801   (YYMMDD)
C***CATEGORY NO.  R3C
C***KEYWORDS  ERROR,XERROR PACKAGE
C***AUTHOR  JONES, R. E., (SNLA)
C***PURPOSE  Aborts program execution and prints error message.
C***DESCRIPTION
C     Abstract
C        ***Note*** machine dependent routine
C        XERABT aborts the execution of the program.
C        The error message causing the abort is given in the calling
C        sequence, in case one needs it for printing on a dayfile,
C        for example.
C
C     Description of Parameters
C        MESSG and NMESSG are as in XERROR, except that NMESSG may
C        be zero, in which case no message is being supplied.
C
C     Written by Ron Jones, with SLATEC Common Math Library Subcommittee
C     Latest revision ---  19 MAR 1980
C***REFERENCES  JONES R.E., KAHANER D.K., "XERROR, THE SLATEC ERROR-
C                 HANDLING PACKAGE", SAND82-0800, SANDIA LABORATORIES,
C                 1982.
C***ROUTINES CALLED  (NONE)
C***END PROLOGUE  XERABT
      CHARACTER*(*) MESSG
C***FIRST EXECUTABLE STATEMENT  XERABT
      STOP
      END
      SUBROUTINE XERCTL(MESSG1,NMESSG,NERR,LEVEL,KONTRL)
C***BEGIN PROLOGUE  XERCTL
C***DATE WRITTEN   790801   (YYMMDD)
C***REVISION DATE  820801   (YYMMDD)
C***CATEGORY NO.  R3C
C***KEYWORDS  ERROR,XERROR PACKAGE
C***AUTHOR  JONES, R. E., (SNLA)
C***PURPOSE  Allows user control over handling of individual errors.
C***DESCRIPTION
C     Abstract
C        Allows user control over handling of individual errors.
C        Just after each message is recorded, but before it is
C        processed any further (i.e., before it is printed or
C        a decision to abort is made), a call is made to XERCTL.
C        If the user has provided his own version of XERCTL, he
C        can then override the value of KONTROL used in processing
C        this message by redefining its value.
C        KONTRL may be set to any value from -2 to 2.
C        The meanings for KONTRL are the same as in XSETF, except
C        that the value of KONTRL changes only for this message.
C        If KONTRL is set to a value outside the range from -2 to 2,
C        it will be moved back into that range.
C
C     Description of Parameters
C
C      --Input--
C        MESSG1 - the first word (only) of the error message.
C        NMESSG - same as in the call to XERROR or XERRWV.
C        NERR   - same as in the call to XERROR or XERRWV.
C        LEVEL  - same as in the call to XERROR or XERRWV.
C        KONTRL - the current value of the control flag as set
C                 by a call to XSETF.
C
C      --Output--
C        KONTRL - the new value of KONTRL.  If KONTRL is not
C                 defined, it will remain at its original value.
C                 This changed value of control affects only
C                 the current occurrence of the current message.
C***REFERENCES  JONES R.E., KAHANER D.K., "XERROR, THE SLATEC ERROR-
C                 HANDLING PACKAGE", SAND82-0800, SANDIA LABORATORIES,
C                 1982.
C***ROUTINES CALLED  (NONE)
C***END PROLOGUE  XERCTL
      CHARACTER*20 MESSG1
C***FIRST EXECUTABLE STATEMENT  XERCTL
      RETURN
      END
      SUBROUTINE XERPRT(MESSG,NMESSG)
C***BEGIN PROLOGUE  XERPRT
C***DATE WRITTEN   790801   (YYMMDD)
C***REVISION DATE  820801   (YYMMDD)
C***CATEGORY NO.  Z
C***KEYWORDS  ERROR,XERROR PACKAGE
C***AUTHOR  JONES, R. E., (SNLA)
C***PURPOSE  Prints error messages.
C***DESCRIPTION
C     Abstract
C        Print the Hollerith message in MESSG, of length NMESSG,
C        on each file indicated by XGETUA.
C     Latest revision ---  19 MAR 1980
C***REFERENCES  JONES R.E., KAHANER D.K., "XERROR, THE SLATEC ERROR-
C                 HANDLING PACKAGE", SAND82-0800, SANDIA LABORATORIES,
C                 1982.
C***ROUTINES CALLED  I1MACH,S88FMT,XGETUA
C***END PROLOGUE  XERPRT
      INTEGER LUN(5)
      CHARACTER*(*) MESSG
C     OBTAIN UNIT NUMBERS AND WRITE LINE TO EACH UNIT
C***FIRST EXECUTABLE STATEMENT  XERPRT
      CALL XGETUA(LUN,NUNIT)
      LENMES = LEN(MESSG)
      DO 20 KUNIT=1,NUNIT
         IUNIT = LUN(KUNIT)
         IF (IUNIT.EQ.0) IUNIT = I1MACH(4)
         DO 10 ICHAR=1,LENMES,72
            LAST = MIN0(ICHAR+71 , LENMES)
            WRITE (IUNIT,'(1X,A)') MESSG(ICHAR:LAST)
   10    CONTINUE
   20 CONTINUE
      RETURN
      END
