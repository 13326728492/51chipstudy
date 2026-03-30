	EXTRN DATA  (_CurrentTask)
	EXTRN CODE  (_Switch1)
	PUBLIC _Schedule


_Schedule:
	PUSH ACC
	PUSH B
	PUSH  PSW
    PUSH  DPL
	PUSH  DPH
    PUSH  0
    PUSH  1
    PUSH  2
    PUSH  3
    PUSH  4
    PUSH  5
    PUSH  6
    PUSH  7

    MOV R0 , _CurrentTask		
    MOV @R0 , SP

    LCALL _Switch1		

    MOV R0, _CurrentTask
    MOV SP, @R0

    

    

    POP 7
    POP 6
    POP 5
    POP 4
    POP 3
    POP 2
    POP 1
    POP 0
    POP DPH
    POP DPL
    POP PSW
    POP B
    POP ACC

    RET
	END
        