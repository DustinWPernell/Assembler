;Test file for assembly of the ldr instruction

;This is a comment

.orig x8000
	ldr R0, R1, #-1
	ldr R0, R3, #-4
	ldr R0, R5, #15
	ldr R1, R0, #-16
	ldr R1, R7, #5
	ldr R2, R1, #7
	ldr R2, R6, #3
	ldr R2, R7, #4
	ldr R3, R3, #-5
	ldr R4, R0, #-8
	ldr R5, R1, #-15
	ldr R6, R2, #14
	ldr R7, R0, #6
	ldr R7, R3, #0
.END
