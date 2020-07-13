;Test file for assembly of the str instruction

;This is a comment

.orig x8000
	str R0, R1, #-1
	str R0, R3, #-4
	str R0, R5, #15
	str R1, R0, #-16
	str R1, R7, #5
	str R2, R1, #7
	str R2, R6, #3
	str R2, R7, #4
	str R3, R3, #-5
	str R4, R0, #-8
	str R5, R1, #-15
	str R6, R2, #14
	str R7, R0, #6
	str R7, R3, #0
.END
