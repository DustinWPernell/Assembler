;Test file for assembly of the ADD instruction

;This is a comment

.orig x3000
	ADD R0, R1, R2
	ADD R1, R2, R3
	ADD R2, R3, R4
	ADD R3, R4, R5
	ADD R4, R5, R6
	ADD R5, R6, R7
	ADD R6, R7, R0
	ADD R7, R0, R1
	ADD R0, R0, #15
	ADD R1, R1, #-15
	ADD R2, R2, #10
	ADD R3, R3, #-10
	ADD R5, R4, #0
	ADD R7, R5, #-1
.END
