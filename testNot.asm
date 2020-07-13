;Test file for assembly of the nOt instruction

;This is a comment

.orig x5000
	NOT R0, R1
	Not R0, R3
	NOT R0, R5
	Not R1, R0
	not R1, R7
	NOT R2, R1
	not R2, R6
	Not R2, R7
	Not R3, R3
	not R4, R0
	Not R5, R1
	NOT R6, R2
	NOT R7, R0
	not R7, R3
.END
