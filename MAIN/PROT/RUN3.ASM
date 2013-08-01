	IDEAL
	P386
	
SEGMENT koodi stack 'STACK'
INCLUDE 'run2.inc'
run2end:

	dw      0
	dw      0
	dw      0
	dw      0
	db	4096 dup(0)
ENDS

END run2end-3

