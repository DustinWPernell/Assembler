
;testing

;This is a comment

.orig x3000
	ADD R1, R2, R3
	add R0, R3, #10
	
	and R1, R1, R2
	AND R0, R3, #-10
	
	
L0			
	BR L1
	BRN L0
	BRZ L1
	BRP L0
	BRNZ L1
	BRNP L0
	BRZP L1
	BRNZP L0
	LD R1, L2
	LDR R1, R2, #-5
	L1			
	NOT R3, R4		
	ST R5, L2
	STR R6, R1, #-5 
	TRAP x20
	TRAP x21
	TRAP x22
	TRAP x23
	TRAP x24
	TRAP x25
	L2	.FILL 	0	
L3	.FILL 	0		
L4	.FILL 	0		

.end
