;Test file for assembly of the AnD instruction

;This is a comment

.orig x4000
	AND R0, R1, R2
	and R1, R2, R3
	And R2, R3, R4
	And R3, R4, R5
	AnD R4, R5, R6
	AnD R5, R6, R7
	AnD R6, R7, R0
	AnD R7, R0, R1
	AnD R0, R0, #15
	AnD R1, R1, #-15
	AnD R2, R2, #10
	AnD R3, R3, #-10
	AND R5, R4, #0
	AND R7, R5, #-1
.END
