;Test file for assembly of the BR instruction

;This is a comment

.orig xAFFF
L0
	br	L9
L1
	brn	L8
L2
	brz	L7
L3
	brp	L6
L4
	brnz	L5
L5
	brnp	L4
L6
	brzp	L3
L7
	brnzp	L2
L8
	brzp	L1
L9
	brnz	L0
	
	.END
