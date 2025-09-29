bits 16

; uint8_t __13_08(uint8_t disk, uint16_t* dx, uint16_t* cx)
global __13_08
__13_08:
	push cx
	push dx
	push bx
	push di
	push es
	push si
	mov si, ax

	mov ah, 0x08
	mov bx, sp
	mov dl, [bx + 16]
	push word 0
	pop es
	xor di, di
	int 0x13
	jc .fin
	
	test ah, ah
	jnz .fin

	mov bx, sp
	mov di, [bx + 20]
	mov [di], dx
	mov di, [bx + 24]
	mov [di], cx
.fin:
	shr ax, 8
	and si, 0xff00
	or ax, si

	pop si
	pop es
	pop di
	pop bx
	pop dx
	pop cx
	retd