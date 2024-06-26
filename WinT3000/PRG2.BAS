10 REM *****ROOM SETPOINT ***************************
20 IF OCCUPIED OR AHU1SCH THEN  AHU1SP = AHU1DSP ELSE  AHU1SP = NITESP
30  AHU1HSP = AHU1SP - 2
40  AHU1CSP = AHU1SP + 2
50 REM ********** NIGHT WARMUP ***************
60 IF  NOT OCCUPIED AND AHU1RAT < NITESP THEN START WARMUP1
70 IF OCCUPIED AND AHU1RAT < AHU1HSP - 4 THEN START WARMUP1
80 IF AHU1RAT > AHU1HSP THEN STOP WARMUP1
90 REM ******** COOLING MODE *****************
100 IF OAT < 10 THEN GOTO 120
110 IF AHU1RAT > AHU1CSP + 1 THEN START COOLMODE
120 IF AHU1RAT < AHU1SP THEN STOP COOLMODE
130 IF  NOT OCCUPIED AND  NOT AHU1OM THEN STOP COOLMODE
140 REM ******** HEATING MODE ********
150 IF AHU1RAT < AHU1SP THEN START HEATMODE
160 IF AHU1RAT > AHU1SP + 1 THEN STOP HEATMODE
170 IF HEATMODE THEN STOP COOLMODE
180 REM **************** FREECOOL AVAIL *************
190 IF AHU1RAT > OAT THEN START AHU1FCA
200 IF AHU1RAT < OAT + 0.50 THEN STOP AHU1FCA
210 REM ****** FAN ********
220 IF OCCUPIED THEN START AHU1-FAN ELSE STOP AHU1-FAN
230 IF WARMUP1 THEN START AHU1-FAN
235 IF AHU1SMOK THEN STOP AHU1-FAN
240 REM *************** DAMPER *********************
250 REM AHU1MAD = MAX( 10 , 1-CON2 * 2 )
260 REM IF  NOT AHU1FCA THEN  AHU1MAD = 10
270 REM IF HEATMODE THEN  AHU1MAD = 10
280 REM IF  NOT AHU1S THEN  AHU1MAD = 0
290 REM IF WARMUP1 THEN  AHU1MAD = 0
300 REM AHU1MAD = MAX( 0 , MIN( 100 , AHU1MAD ) )
301  AHU1MAD = 0.50
310 REM ************** VALVES *************************
320 REM IF AHU1FCA THEN AHU1-VLV = 2 * 1-CON2 - 50
330 REM IF  NOT AHU1FCA THEN AHU1-VLV = 2 * 1-CON2
340 REM IF  NOT COOLMODE THEN AHU1-VLV = 1-CON1
350 IF INTERVAL( 00:00:02 ) THEN GOTO 380
360 REM IF HEATMODE AND AHU1RAT < AHU1HSP THEN  AHU1VLV = AHU1VLV + 1
370 IF HEATMODE AND AHU1RAT > AHU1HSP THEN  AHU1VLV = AHU1VLV - 1
360 IF COOLMODE AND AHU1RAT > AHU1HSP THEN  AHU1VLV = AHU1VLV + 1
370 IF COOLMODE AND AHU1RAT < AHU1HSP THEN  AHU1VLV = AHU1VLV - 1
380 REM IF WARMUP1 THEN  AHU1VLV = 100
390 IF  NOT AHU1-FAN THEN  AHU1VLV = 0
400 REM AHU1VLV = MAX( 0 , MIN( 100 , AHU1VLV ) )
410 REM ***
1000 REM *****
