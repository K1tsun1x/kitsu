org 0x7c00
bits 16

%define MAIN_BOOTLOADER_DESTINATION			0x1000
%define MAIN_BOOTLOADER_SECTOR_NUMBER		2

PRE_BOOTLOADER_BEGIN:
; BPB:
; 	jmp short MAIN
; 	nop
; 	.OEM_ID:				db "KITSU   "
; 	.BytesPerSector:		dw 512
; 	.SectorsPerCluster:		db 1			; must be changed(?)
; 	.ReservedSectors:		dw 32
; 	.TotalFATs:				db 1			; must be changed
; 	.MaxRootEntries:		dw 0			; must be changed
; 	.NumberOfSectors:		dw 0			; must be changed
; 	.MediaDescriptor:		db 0xf8			; must be changed
; 	.SectorsPerFAT:			dw 0
; 	.SectorsPerTrack:		dw 0
; 	.HeadsPerCylinder:		dw 0
; 	.HiddenSectors:			dd 0			; must be changed(?)
; 	.TotalSectors:			dd 0			; must be changed
; EBPB:
; 	.BigSectorsPerFAT:		dd 1			; must be changed
; 	.Flags:					dw 0
; 	.FSVersion:				dw 0
; 	.RootDirectoryStart:	dd 2
; 	.FSInfoSector:			dw 1
; 	.BackupBootSector:		dw 0
; 	.Reserved:				times 12 db 0
; 	.DriveNumber:			db 0x80
; 	.WinNTFlags:			db 0
; 	.FATSignature:			db 0x29
; 	.VolumeIDSerialNumber:	dd 0
; 	.VolumeLabel:			db "Kitsu      "
; 	.SystemID:				db "FAT32   "
; MAIN:
	jmp 0:LOAD_REGISTERS
LOAD_REGISTERS:
	mov ax, 0
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	mov bp, PRE_BOOTLOADER_BEGIN
	mov sp, PRE_BOOTLOADER_BEGIN
	sti
	cld

	; set video mode to 80x25 text
	mov ax, 0x0003
	int 0x10

	; reset disk system
	mov ax, 0x0080
	int 0x13

	; read main bootloader header
	mov ax, 0x0201
	mov cx, MAIN_BOOTLOADER_SECTOR_NUMBER
	mov dh, 0x00
	mov bx, MAIN_BOOTLOADER_DESTINATION
	int 0x13

	test ah, ah
	jnz FAILED_TO_READ_MAIN_BOOTLOADER_HEADER

	; check signature
	mov bx, [MAIN_BOOTLOADER_DESTINATION]
	cmp bx, 0x4d10
	jne INVALID_MAIN_BOOTLOADER_HEADER

	; read main bootloader
	mov bl, [MAIN_BOOTLOADER_DESTINATION + 2]
	mov ah, 0x02
	mov al, bl
	mov cx, MAIN_BOOTLOADER_SECTOR_NUMBER
	mov dh, 0
	mov bx, MAIN_BOOTLOADER_DESTINATION
	int 0x13

	test ah, ah
	jnz FAILED_TO_LOAD_MAIN_BOOTLOADER

	; set stack registers for main bootloader
	mov bp, MAIN_BOOTLOADER_DESTINATION
	mov sp, bp
	
	; push drive index
	and edx, 0x80
	push edx

	; call main bootloader entry point
	push dword INFINITY_LOOP
	jmp word [MAIN_BOOTLOADER_DESTINATION + 3]

FAILED_TO_READ_MAIN_BOOTLOADER_HEADER:
	mov si, MsgErrorFailedToReadMainBootloader
	call PRINT
	jmp INFINITY_LOOP
INVALID_MAIN_BOOTLOADER_HEADER:
	mov si, MsgErrorInvalidMainBootloaderHeader
	call PRINT
	jmp INFINITY_LOOP
FAILED_TO_LOAD_MAIN_BOOTLOADER:
	mov si, MsgErrorFailedToLoadMainBootloader
	call PRINT
INFINITY_LOOP:
	jmp INFINITY_LOOP

PRINT:
	mov ah, 0x0e
.PUTCHAR:
	lodsb
	test al, al
	jz .FIN
	int 0x10
	jmp .PUTCHAR
.FIN:
	ret

MsgErrorFailedToReadMainBootloader: db "Error: failed to read main bootloader!", 0
MsgErrorInvalidMainBootloaderHeader: db "Error: invalid main bootloader header!", 0
MsgErrorFailedToLoadMainBootloader: db "Error: failed to load main bootloader!", 0

times 510 - $ + $$ db 0
dw 0xaa55
PRE_BOOTLOADER_END:
; FS_INFO_BEGIN:
; 	.LeadSignature:				dd 0x41615252
; 	.Reserved1:					times 480 db 0
; 	.AnotherSignature:			dd 0x61417272
; 	.LastKnownFreeCluster:		dd -1
; 	.AvailableClusters:			dd -1
; 	.Reserved2:					times 12 db 0
; 	.TrailSignature:			dd 0xaa550000
; FS_INFO_END: