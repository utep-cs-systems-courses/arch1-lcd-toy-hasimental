	.arch msp430g2553
	.p2align1,0

	.text // this is for instructions put into instruction memory
	.global drawDoorway // this makes it visble to other files

drawDoorway:
	sub #8 r1 ; // this is to allocate the space needed to save the variables 
	mov r12 6(r1) ;//row
	mov r13 4(r1) ;//col
	mov r14 2(r1) ;//radius
	mov r15 0(r1) ;//colorBGR

	mov r16,8(r1) ;//for diameter
	add r16,r16 ;//for 2*diameter
	mov r17,10(r1) ;//for x
	mov r18,12(r1) ;//for y 
	mov r20,0 ;// for i of the for loop
outerLoop:
	add r16,0(1) ;// to add 1 to diameter
	cmp r20,r16 ;// compare
	goto innerLoop ;// increment

	
