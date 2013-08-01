.MODEL LARGE
.386
.CODE

mov	eax,ds:[si+1111h]
add	si,4
mov	es:[di],eax
add	di,4000h

END
