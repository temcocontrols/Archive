10 REM ************************************************************
20 REM  - CALCULATE COOLING DEMAND BY WATCHING COOLING VALVES
30 REM  - WHEN DEMAND REACHES A THRESHOLD, SWITCH ON THE CHILLER
40 REM  - WHEN THE DEMAND IS SATISFIED, SWITCH OFF THE CHILLER
50 REM ************************************************************
60 REM VAR8 = AVG( AHU1-CCV , AHU2-CCV , AHU3-CCV , AHU4-CCV , AHU5-CCV , AHU6-CCV
70 REM **** CHILLER MODE*******************************
80 IF OAT > CHOATEN THEN START MECHFLAG
90 IF OAT < CHOATEN - 0.50 THEN STOP MECHFLAG
100  MECHTIMR = TIME-ON( MECHFLAG )
110 REM ******** CHILLER STARTUP ******************************
120 IF MECHTIMR > 00:00:20 THEN START CHILREQ
130 IF MECHTIMR = 0 THEN STOP CHILREQ
140 REM ******** NUMBER OF CHILLERS REQUIRED ****************
150 IF CHILREQ THEN  NUMCHILL = MAX( 1 , NUMPUMPS )
160  NUMCHILL = MAX( NUMCHILL , NUMPUMPS )
170 IF TIME-OFF( BOILER ) < 00:00:10 AND BLDSUPT > 30 THEN  NUMCHILL = 0
180 IF NUMPUMPS = 0 OR  NOT P4 THEN  NUMCHILL = 0
190 IF NUMCHILL >= 1 THEN START CH1 ELSE STOP CH1
200 IF NUMCHILL >= 2 THEN START CH2 ELSE STOP CH2
210 IF NUMCHILL >= 3 THEN START CH3 ELSE STOP CH3
220 REM ******** PUMPS REQUIRED? *****************************************
230 IF MECHTIMR > 00:00:10 THEN START COOLREQD
240 IF MECHFLAG THEN START COOLREQD
250 IF TIME-OFF( CH1 ) < 00:00:10 THEN GOTO 270
260 IF  NOT MECHFLAG THEN STOP COOLREQD
270 REM *********** START THE PUMPS *********************************
280 IF COOLREQD THEN START P2 ELSE STOP P2
290 REM IF TIME-ON( P7 ) > 00:00:05 THEN START P2
300 REM IF TIME-ON( P8 ) > 00:00:05 THEN START P2
310 REM IF TIME-ON( ELEV-DN ) > 00:00:05 THEN START P16
320 REM IF TIME-OFF( ELEV-UP ) > 00:00:05 THEN STOP OUT23
330 REM IF TIME-OFF( OUT23 ) > 00:00:05 THEN STOP ELEV-DN
340 REM IF TIME-OFF( ELEV-DN ) > 00:00:05 THEN STOP P16
350 REM ************* COOLING TOWER ****************
360  FANSREQD = (CWRT - CONDSETP) / 3
370  FANSREQD = MAX( 0 , MIN( 3 , FANSREQD ) )
380 IF  NOT P2 THEN  FANSREQD = 0
390 IF FANSREQD > 1 THEN START CT1
400 IF FANSREQD > 2 THEN START CT2
410 IF FANSREQD > 3 THEN START CT3
420 IF FANSREQD < 1 THEN STOP CT1
430 IF FANSREQD < 2 THEN STOP CT2
440 IF FANSREQD < 3 THEN STOP CT3
