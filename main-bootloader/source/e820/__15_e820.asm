bits 16

; bool __15_e820(uint32_t ebx, uint16_t di, uint32_t* size, uint32_t* next);
global __15_e820
__15_e820:
	push bp
	mov bp, sp
	add bp, 6
	push esi
	mov esi, eax
	and si, 0xff00
	push ecx
	push edx
	push ebx
	push edi
	push es

	push word 0
	pop es

	mov eax, 0x0000e820
	mov ecx, 24
	mov edx, 0x534d4150
	mov ebx, [bp]
	mov di, [bp + 4]
	int 0x15
	jc .fin

	cmp eax, 0x534d4150
	jne .fin

	or si, 1

	mov di, [bp + 8]
	mov [di], ecx
	mov di, [bp + 12]
	mov [di], ebx
.fin:
	mov eax, esi
	pop es
	pop edi
	pop ebx
	pop edx
	pop ecx
	pop esi
	pop bp
	retd