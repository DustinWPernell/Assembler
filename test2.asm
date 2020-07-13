;Test file for assembly

;This is a comment

.orig x40f4
	and R2, R1, R0
	AND R0, R0, #-10
L5			
	BR L5
	BRN L4
	BRZ L3
	BRP L8
	
ADD R1, R7, R0
add R2, R0, #10
	
BRNZ L5
L4
		BRNP L5
		BRZP L9
		BRNZP L4
		LD R0, L4
		LDR R7, R2, #-15
L3			
		   NOT     R3,     R4		
	ST     	R0    , 	   	L8
	STR 	 R6    ,   R7, #15 
	TRAP     	x20
		TRAP	     x21
	TRAP     			x22
	TRAP    		 x23
		TRAP			x24
	TRAP					x25
	L8		.FILL 	0	
				L9		.FILL 	0			
L0	.FILL 	0		

.END
