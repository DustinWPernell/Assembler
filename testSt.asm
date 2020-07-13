;Test file for assembly of the ST instruction

;This is a comment

.orig xA000
	st R0, L9
	st R1, L8
	st R2, L7
	st R3, L6
	st R4, L5
	st R5, L4
	st R6, L3
	st R7, L2
	st R0, L1
	st R1, L0
L0	.fill	0
L1	.fill	0
L2	.fill	0
L3	.fill	0
L4	.fill	0
L5	.fill	0
L6	.fill	0
L7	.fill	0
L8	.fill	0
L9	.fill	0
	
	.END
