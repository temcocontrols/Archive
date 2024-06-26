11 REM ****** WATER FLOW SIMULATION *************
12  BLDFLOW = (BLDFLOW * 99 + NUMPUMPS * 25) / 100
10 REM ******* BOILER HEAT MASS BALANCE ********
30 REM ********ENERGY ADDED BY BOILER*************
40  BDELTATP = B1S * BLDFLOW / 20 * (V1 + V2) / 600
41  BDELTATP = 0 - (CH1 + CH2 + CH3)
90 IF NUMPUMPS = 0 THEN  BDELTATP = 0
50  BDELTATP = MAX(  -1 , MIN( BDELTATP , 1 ) )
60 REM ********** ENERGY LOST (GAINED) IN BUILDING********
70  BDELTATM = 1 - OAT / 20
80  BDELTATM = BDELTATM / 3 * NUMPUMPS
90 IF NUMPUMPS = 0 THEN  BDELTATM = 0
100  BDELTATM = MAX(  -1 , MIN( 1 , BDELTATM ) )
110 REM ********** TEMPERATURES *********************
120  BLDRETT = (BLDRETT * 49 + BLDSUPT) / 50 - BDELTATM
130  BLDSUPT = (BLDSUPT * 49 + BLDRETT) / 50 + BDELTATP
170 REM **** LIMITS ******
180  BLDSUPT = MAX( 0 , MIN( 99 , BLDSUPT ) )
190  BLDRETT = MAX( 0 , MIN( 99 , BLDRETT ) )
200 IF HEATMODE THEN  BLDRETT = MAX( OAT , BLDRETT )
200 IF  NOT HEATMODE AND  NOT CH1S THEN  BLDRETT = MAX( OAT , BLDRETT )
