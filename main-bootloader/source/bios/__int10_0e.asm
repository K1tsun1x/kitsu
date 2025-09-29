bits 16

; void __int10_0e(uint8_t c)
global __int10_0e
__int10_0e:
	push bp
	mov bp, sp
	push ax
	push bx

	mov ah, 0x0e
	mov al, [bp + 6]
	int 0x10

	pop bx
	pop ax
	pop bp
	retd