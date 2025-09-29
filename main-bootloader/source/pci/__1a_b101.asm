bits 16

; bool __1a_b101(uint32_t* pmep, uint8_t* characteristics, uint8_t* majorver, uint8_t* minorver);
global __1a_b101
__1a_b101:
	push bp
	mov bp, sp
	add bp, 6
	push esi
	mov esi, eax
	and esi, 0xffffff00
	push eax
	push edi
	pushfd

	mov ax, 0xb101
	xor edi, edi
	int 0x1a
	jc .fin

	test ah, ah
	jnz .fin

	cmp edx, 0x20494350
	jne .fin

	mov cx, si
	mov si, [bp]
	mov [si], edi
	mov si, [bp + 4]
	mov [si], al
	mov si, [bp + 8]
	mov [si], bh
	mov si, [bp + 12]
	mov [si], bl
	mov si, cx

	or si, 1
	jmp .fin
.fin:
	popfd
	pop edi
	pop eax
	pop esi
	pop bp
	retd