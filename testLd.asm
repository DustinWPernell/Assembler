;Test file for assembly of the LD instruction

;This is a comment

.orig xA000
	ld R0, L9
	ld R1, L8
	ld R2, L7
	ld R3, L6
	ld R4, L5
	ld R5, L4
	ld R6, L3
	ld R7, L2
	ld R0, L1
	ld R1, L0
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
