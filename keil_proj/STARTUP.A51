$NOMOD51
;-----------------------------------------------------------------------
; Minimal C51 startup: clear IDATA, set stack pointer, jump to main
;-----------------------------------------------------------------------
                NAME    ?C_STARTUP

?C_C51STARTUP   SEGMENT   CODE
?STACK          SEGMENT   IDATA

                RSEG    ?STACK
                DS      1

                EXTRN CODE (?C_START)
                PUBLIC  ?C_STARTUP

                CSEG    AT 0
?C_STARTUP:     LJMP    STARTUP1

                RSEG    ?C_C51STARTUP
STARTUP1:
                MOV     R0,#0FFH
                CLR     A
IDATALOOP:      MOV     @R0,A
                DJNZ    R0,IDATALOOP
                MOV     SP,#?STACK-1
                LJMP    ?C_START

                END
