10 IF TIME-ON( P3S ) > 00:00:15 THEN START WL1
20 IF TIME-ON( P3S ) > 00:00:30 THEN START WL2
30 IF TIME-ON( P3S ) > 00:00:45 THEN START WL3
40 IF TIME-ON( P4S ) > 00:00:15 THEN START WL1
50 IF TIME-ON( P4S ) > 00:00:30 THEN START WL2
60 IF TIME-ON( P4S ) > 00:00:45 THEN START WL3
61 IF TIME-OFF( P4S ) < 00:00:25 THEN GOTO 100
70 IF TIME-OFF( P3S ) > 00:00:25 THEN STOP WL3
61 IF TIME-OFF( P4S ) < 00:00:35 THEN GOTO 100
80 IF TIME-OFF( P3S ) > 00:00:35 THEN STOP WL2
61 IF TIME-OFF( P4S ) < 00:00:45 THEN GOTO 100
90 IF TIME-OFF( P3S ) > 00:00:45 THEN STOP WL1
100 IF TIME-ON( P3 ) > 00:00:02 THEN START P3S ELSE STOP P3S
110 REM IF TIME-ON( SPARE ) > 00:00:02 THEN START P4S ELSE STOP P4S
