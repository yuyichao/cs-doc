;
; File Name   :	C:\Temp\v1.exe
; Format      :	Portable executable for	IBM PC (PE)
; Imagebase   :	400000
; Section 1. (virtual address 00001000)
; Virtual size			: 00008082 (  32898.)
; Section size in file		: 00009000 (  36864.)
; Offset to raw	data for section: 00001000
; Flags	60000020: Text Executable Readable
; Alignment	: 16 bytes ?
; OS type	  :  MS	Windows
; Application type:  Executable	32bit
;


unicode		macro page,string,zero
		irpc c,<string>
		db '&c', page
		endm
		ifnb <zero>
		dw zero
		endif
endm

		model flat

; ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ

; Segment type:	Pure code
; Segment permissions: Read/Execute
_text		segment	para public 'CODE' use32
		assume cs:_text
		;org 401000h
		assume es:nothing, ss:nothing, ds:_data, fs:nothing, gs:nothing
		dd 0CCCCCCCCh
		db 0CCh

; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


_main		proc near		; CODE XREF: mainCRTStartup+16Ep
		jmp	main
_main		endp

; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		align 10h

; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ

; Attributes: bp-based frame

main		proc near		; CODE XREF: _mainj

var_20		= dword	ptr -20h
arg_0		= dword	ptr  8
arg_4		= dword	ptr  0Ch

		push	ebp
		mov	ebp, esp
		sub	esp, 20h
		cmp	[ebp+arg_0], 1
		jle	short loc_40102F
		mov	eax, [ebp+arg_4]
		mov	ecx, [eax+4]
		push	ecx
		lea	edx, [ebp+var_20]
		push	edx
		call	strcpy
		add	esp, 8

loc_40102F:				; CODE XREF: main+Aj
		lea	eax, [ebp+var_20]
		push	eax
		push	offset aBuffS	; "buff	: %s\n"
		call	printf
		add	esp, 8
		xor	eax, eax
		mov	esp, ebp
		pop	ebp
		retn
main		endp

; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		dd 3 dup(0CCCCCCCCh)
		db 0CCh

; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


printf		proc near		; CODE XREF: main+28p

arg_0		= dword	ptr  10h
arg_4		= dword	ptr  14h

		push	ebx
		push	esi
		push	edi
		mov	esi, offset dword_40CB80
		push	esi
		call	_stbuf
		mov	edi, eax
		lea	eax, [esp+4+arg_4]
		push	eax
		push	[esp+8+arg_0]
		push	esi
		call	_output
		push	esi
		push	edi
		mov	ebx, eax
		call	_ftbuf
		add	esp, 18h
		pop	edi
		pop	esi
		mov	eax, ebx
		pop	ebx
		retn
printf		endp

; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		align 10h

; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


strcpy		proc near		; CODE XREF: main+17p _NMSG_WRITE+94p	...

arg_0		= dword	ptr  8

		push	edi
		mov	edi, [esp+arg_0]
		jmp	short loc_401105
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		align 10h

strcat:					; CODE XREF: _NMSG_WRITE+102p
					; _NMSG_WRITE+10Dp ...
		mov	ecx, [esp+4]
		push	edi
		test	ecx, 3
		jz	short loc_4010C0

loc_4010AD:				; CODE XREF: strcpy+2Cj
		mov	al, [ecx]
		add	ecx, 1
		test	al, al
		jz	short loc_4010F3
		test	ecx, 3
		jnz	short loc_4010AD
		mov	edi, edi

loc_4010C0:				; CODE XREF: strcpy+1Bj strcpy+46j ...
		mov	eax, [ecx]
		mov	edx, 7EFEFEFFh
		add	edx, eax
		xor	eax, 0FFFFFFFFh
		xor	eax, edx
		add	ecx, 4
		test	eax, 81010100h
		jz	short loc_4010C0
		mov	eax, [ecx-4]
		test	al, al
		jz	short loc_401102
		test	ah, ah
		jz	short loc_4010FD
		test	eax, 0FF0000h
		jz	short loc_4010F8
		test	eax, 0FF000000h
		jz	short loc_4010F3
		jmp	short loc_4010C0
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_4010F3:				; CODE XREF: strcpy+24j strcpy+5Fj
		lea	edi, [ecx-1]
		jmp	short loc_401105
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_4010F8:				; CODE XREF: strcpy+58j
		lea	edi, [ecx-2]
		jmp	short loc_401105
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_4010FD:				; CODE XREF: strcpy+51j
		lea	edi, [ecx-3]
		jmp	short loc_401105
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_401102:				; CODE XREF: strcpy+4Dj
		lea	edi, [ecx-4]

loc_401105:				; CODE XREF: strcpy+5j	strcpy+66j ...
		mov	ecx, [esp+4+arg_0]
		test	ecx, 3
		jz	short loc_40112E

loc_401111:				; CODE XREF: strcpy+95j
		mov	dl, [ecx]
		add	ecx, 1
		test	dl, dl
		jz	short loc_401180
		mov	[edi], dl
		add	edi, 1
		test	ecx, 3
		jnz	short loc_401111
		jmp	short loc_40112E
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_401129:				; CODE XREF: strcpy+B6j strcpy+D0j
		mov	[edi], edx
		add	edi, 4

loc_40112E:				; CODE XREF: strcpy+7Fj strcpy+97j
		mov	edx, 7EFEFEFFh
		mov	eax, [ecx]
		add	edx, eax
		xor	eax, 0FFFFFFFFh
		xor	eax, edx
		mov	edx, [ecx]
		add	ecx, 4
		test	eax, 81010100h
		jz	short loc_401129
		test	dl, dl
		jz	short loc_401180
		test	dh, dh
		jz	short loc_401177
		test	edx, 0FF0000h
		jz	short loc_40116A
		test	edx, 0FF000000h
		jz	short loc_401162
		jmp	short loc_401129
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_401162:				; CODE XREF: strcpy+CEj
		mov	[edi], edx
		mov	eax, [esp+8]
		pop	edi
		retn
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_40116A:				; CODE XREF: strcpy+C6j
		mov	[edi], dx
		mov	eax, [esp+8]
		mov	byte ptr [edi+2], 0
		pop	edi
		retn
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_401177:				; CODE XREF: strcpy+BEj
		mov	[edi], dx
		mov	eax, [esp+8]
		pop	edi
		retn
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_401180:				; CODE XREF: strcpy+88j strcpy+BAj
		mov	[edi], dl
		mov	eax, [esp+8]
		pop	edi
		retn
strcpy		endp ; sp = -4


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


_amsg_exit	proc near		; CODE XREF: mainCRTStartup+104p
					; mainCRTStartup+12Ap ...

arg_0		= dword	ptr  4

		cmp	__error_mode, 2
		jz	short loc_401196
		call	_FF_MSGBANNER

loc_401196:				; CODE XREF: _amsg_exit+7j
		push	[esp+arg_0]
		call	_NMSG_WRITE
		push	0FFh		; int
		call	_aexit_rtn
		pop	ecx
		pop	ecx
		retn
_amsg_exit	endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


fast_error_exit	proc near

arg_0		= dword	ptr  4

		cmp	__error_mode, 2
		jz	short loc_4011BB
		call	_FF_MSGBANNER

loc_4011BB:				; CODE XREF: fast_error_exit+7j
		push	[esp+arg_0]
		call	_NMSG_WRITE
		push	0FFh
		call	__crtExitProcess
		pop	ecx
		pop	ecx
		retn
fast_error_exit	endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


check_managed_app proc near
		push	0
		call	ds:__imp__GetModuleHandleA@4 ; __declspec(dllimport) GetModuleHandleA(x)
		cmp	word ptr [eax],	5A4Dh
		jnz	short loc_4011FF
		mov	ecx, [eax+3Ch]
		add	ecx, eax
		cmp	dword ptr [ecx], 4550h
		jnz	short loc_4011FF
		movzx	eax, word ptr [ecx+18h]
		cmp	eax, 10Bh
		jz	short loc_401215
		cmp	eax, 20Bh
		jz	short loc_401202

loc_4011FF:				; CODE XREF: check_managed_app+Dj
					; check_managed_app+1Aj
		xor	eax, eax
		retn
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_401202:				; CODE XREF: check_managed_app+2Cj
		xor	eax, eax
		cmp	dword ptr [ecx+84h], 0Eh
		jbe	short locret_401226
		cmp	[ecx+0F8h], eax
		jmp	short loc_401223
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_401215:				; CODE XREF: check_managed_app+25j
		xor	eax, eax
		cmp	dword ptr [ecx+74h], 0Eh
		jbe	short locret_401226
		cmp	[ecx+0E8h], eax

loc_401223:				; CODE XREF: check_managed_app+42j
		setnz	al

locret_401226:				; CODE XREF: check_managed_app+3Aj
					; check_managed_app+4Aj
		retn
check_managed_app endp

; [000001C7 BYTES: COLLAPSED FUNCTION mainCRTStartup. PRESS KEYPAD "+" TO EXPAND]

; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


_stbuf		proc near		; CODE XREF: printf+9p

arg_0		= dword	ptr  8

		push	esi
		mov	esi, [esp+arg_0]
		push	dword ptr [esi+10h]
		call	_isatty
		test	eax, eax
		pop	ecx
		jz	short loc_401472
		cmp	esi, offset dword_40CB80
		jnz	short loc_40140C
		xor	eax, eax
		jmp	short loc_401417
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_40140C:				; CODE XREF: _stbuf+18j
		cmp	esi, offset dword_40CBA0
		jnz	short loc_401472
		xor	eax, eax
		inc	eax

loc_401417:				; CODE XREF: _stbuf+1Cj
		inc	_cflush
		test	word ptr [esi+0Ch], 10Ch
		jnz	short loc_401472
		push	ebx
		push	edi
		lea	edi, ds:40D42Ch[eax*4]
		cmp	dword ptr [edi], 0
		mov	ebx, 1000h
		jnz	short loc_401458
		push	ebx
		call	malloc
		test	eax, eax
		pop	ecx
		mov	[edi], eax
		jnz	short loc_401458
		lea	eax, [esi+14h]
		push	2
		mov	[esi+8], eax
		mov	[esi], eax
		pop	eax
		mov	[esi+18h], eax
		mov	[esi+4], eax
		jmp	short loc_401465
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_401458:				; CODE XREF: _stbuf+48j _stbuf+55j
		mov	edi, [edi]
		mov	[esi+8], edi
		mov	[esi], edi
		mov	[esi+18h], ebx
		mov	[esi+4], ebx

loc_401465:				; CODE XREF: _stbuf+68j
		or	word ptr [esi+0Ch], 1102h
		pop	edi
		xor	eax, eax
		pop	ebx
		inc	eax
		pop	esi
		retn
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_401472:				; CODE XREF: _stbuf+10j _stbuf+24j ...
		xor	eax, eax
		pop	esi
		retn
_stbuf		endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


_ftbuf		proc near		; CODE XREF: printf+23p

arg_0		= dword	ptr  4
arg_4		= dword	ptr  8

		cmp	[esp+arg_0], 0
		push	esi
		jz	short loc_40149F
		mov	esi, [esp+4+arg_4]
		test	byte ptr [esi+0Dh], 10h
		jz	short loc_4014B0
		push	esi
		call	_flush
		and	byte ptr [esi+0Dh], 0EEh
		and	dword ptr [esi+18h], 0
		and	dword ptr [esi], 0
		and	dword ptr [esi+8], 0
		jmp	short loc_4014AF
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_40149F:				; CODE XREF: _ftbuf+6j
		mov	eax, [esp+4+arg_4]
		test	byte ptr [eax+0Dh], 10h
		jz	short loc_4014B0
		push	eax
		call	_flush

loc_4014AF:				; CODE XREF: _ftbuf+27j
		pop	ecx

loc_4014B0:				; CODE XREF: _ftbuf+10j _ftbuf+31j
		pop	esi
		retn
_ftbuf		endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


write_char	proc near		; CODE XREF: write_multi_char+11p
					; write_string+22p ...
		test	byte ptr [ecx+0Ch], 40h
		jz	short loc_4014BE
		cmp	dword ptr [ecx+8], 0
		jz	short loc_4014E2

loc_4014BE:				; CODE XREF: write_char+4j
		dec	dword ptr [ecx+4]
		js	short loc_4014CE
		mov	edx, [ecx]
		mov	[edx], al
		inc	dword ptr [ecx]
		movzx	eax, al
		jmp	short loc_4014DA
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_4014CE:				; CODE XREF: write_char+Fj
		movsx	eax, al
		push	ecx
		push	eax
		call	_flsbuf
		pop	ecx
		pop	ecx

loc_4014DA:				; CODE XREF: write_char+1Aj
		cmp	eax, 0FFFFFFFFh
		jnz	short loc_4014E2
		or	[esi], eax
		retn
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_4014E2:				; CODE XREF: write_char+Aj
					; write_char+2Bj
		inc	dword ptr [esi]
		retn
write_char	endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ

; Attributes: bp-based frame

write_multi_char proc near		; CODE XREF: _output+6EFp _output+71Dp ...

arg_0		= byte ptr  8
arg_4		= dword	ptr  0Ch
arg_8		= dword	ptr  10h

		push	ebp
		mov	ebp, esp
		push	esi
		mov	esi, eax
		jmp	short loc_401500
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_4014ED:				; CODE XREF: write_multi_char+1Fj
		mov	ecx, [ebp+arg_8]
		mov	al, [ebp+arg_0]
		dec	[ebp+arg_4]
		call	write_char
		cmp	dword ptr [esi], 0FFFFFFFFh
		jz	short loc_401506

loc_401500:				; CODE XREF: write_multi_char+6j
		cmp	[ebp+arg_4], 0
		jg	short loc_4014ED

loc_401506:				; CODE XREF: write_multi_char+19j
		pop	esi
		pop	ebp
		retn
write_multi_char endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


write_string	proc near		; CODE XREF: _output+706p _output+767p ...

arg_0		= dword	ptr  4

		test	byte ptr [edi+0Ch], 40h
		push	ebx
		push	esi
		mov	esi, eax
		mov	ebx, ecx
		jz	short loc_401536
		cmp	dword ptr [edi+8], 0
		jnz	short loc_401536
		mov	eax, [esp+8+arg_0]
		add	[esi], eax
		jmp	short loc_40153D
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_401523:				; CODE XREF: write_string+32j
		mov	al, [ebx]
		dec	[esp+8+arg_0]
		mov	ecx, edi
		call	write_char
		inc	ebx
		cmp	dword ptr [esi], 0FFFFFFFFh
		jz	short loc_40153D

loc_401536:				; CODE XREF: write_string+Aj
					; write_string+10j
		cmp	[esp+8+arg_0], 0
		jg	short loc_401523

loc_40153D:				; CODE XREF: write_string+18j
					; write_string+2Bj
		pop	esi
		pop	ebx
		retn
write_string	endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


get_int_arg	proc near
		add	dword ptr [eax], 4
		mov	eax, [eax]
		mov	eax, [eax-4]
		retn
get_int_arg	endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


get_int64_arg	proc near
		add	dword ptr [eax], 8
		mov	ecx, [eax]
		mov	eax, [ecx-8]
		mov	edx, [ecx-4]
		retn
get_int64_arg	endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


get_short_arg	proc near
		add	dword ptr [eax], 4
		mov	eax, [eax]
		mov	ax, [eax-4]
		retn
get_short_arg	endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


_output		proc near		; CODE XREF: printf+1Ap

var_1D4		= dword	ptr -1D4h

		push	ebp
		lea	ebp, [esp+var_1D4]
		sub	esp, 254h
		mov	eax, __security_cookie
		mov	[ebp+1D0h], eax
		xor	eax, eax
		mov	[ebp-48h], eax
		mov	[ebp-4Ch], eax
		mov	[ebp-60h], eax
		mov	eax, [ebp+1E0h]
		push	ebx
		mov	bl, [eax]
		xor	ecx, ecx
		test	bl, bl
		jz	loc_401D22
		push	esi
		push	edi
		mov	edi, eax
		jmp	short loc_40159F
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_40159C:				; CODE XREF: _output+7BBj
		mov	ecx, [ebp-70h]

loc_40159F:				; CODE XREF: _output+3Bj
		inc	edi
		cmp	dword ptr [ebp-4Ch], 0
		mov	[ebp+1E0h], edi
		jl	loc_401D20
		cmp	bl, 20h
		jl	short loc_4015C9
		cmp	bl, 78h
		jg	short loc_4015C9
		movsx	eax, bl
		movsx	eax, ds:byte_40A010[eax]
		and	eax, 0Fh
		jmp	short loc_4015CB
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_4015C9:				; CODE XREF: _output+54j _output+59j
		xor	eax, eax

loc_4015CB:				; CODE XREF: _output+68j
		movsx	eax, ds:__lookuptable[ecx+eax*8]
		push	7
		sar	eax, 4
		pop	ecx
		cmp	eax, ecx
		mov	[ebp-70h], eax
		ja	loc_401D10
		jmp	ds:off_401D39[eax*4]

loc_4015EB:				; DATA XREF: .text:00401D3Do
		xor	eax, eax
		or	dword ptr [ebp-40h], 0FFFFFFFFh
		mov	[ebp-6Ch], eax
		mov	[ebp-64h], eax
		mov	[ebp-58h], eax
		mov	[ebp-54h], eax
		mov	[ebp-3Ch], eax
		mov	[ebp-5Ch], eax
		jmp	loc_401D10
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_401608:				; CODE XREF: _output+85j
					; DATA XREF: .text:00401D41o
		movsx	eax, bl
		sub	eax, 20h
		jz	short loc_40164B
		sub	eax, 3
		jz	short loc_401642
		sub	eax, 8
		jz	short loc_401639
		dec	eax
		dec	eax
		jz	short loc_401630
		sub	eax, 3
		jnz	loc_401D10
		or	dword ptr [ebp-3Ch], 8
		jmp	loc_401D10
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_401630:				; CODE XREF: _output+BDj
		or	dword ptr [ebp-3Ch], 4
		jmp	loc_401D10
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_401639:				; CODE XREF: _output+B9j
		or	dword ptr [ebp-3Ch], 1
		jmp	loc_401D10
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_401642:				; CODE XREF: _output+B4j
		or	byte ptr [ebp-3Ch], 80h
		jmp	loc_401D10
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_40164B:				; CODE XREF: _output+AFj
		or	dword ptr [ebp-3Ch], 2
		jmp	loc_401D10
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_401654:				; CODE XREF: _output+85j
					; DATA XREF: .text:00401D45o
		cmp	bl, 2Ah
		jnz	short loc_401680
		add	dword ptr [ebp+1E4h], 4
		mov	eax, [ebp+1E4h]
		mov	eax, [eax-4]
		test	eax, eax
		mov	[ebp-58h], eax
		jge	loc_401D10
		or	dword ptr [ebp-3Ch], 4
		neg	dword ptr [ebp-58h]
		jmp	loc_401D10
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_401680:				; CODE XREF: _output+F8j
		mov	eax, [ebp-58h]
		movsx	ecx, bl
		lea	eax, [eax+eax*4]
		lea	eax, [ecx+eax*2-30h]
		mov	[ebp-58h], eax
		jmp	loc_401D10
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_401695:				; CODE XREF: _output+85j
					; DATA XREF: .text:00401D49o
		and	dword ptr [ebp-40h], 0
		jmp	loc_401D10
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_40169E:				; CODE XREF: _output+85j
					; DATA XREF: .text:00401D4Do
		cmp	bl, 2Ah
		jnz	short loc_4016C7
		add	dword ptr [ebp+1E4h], 4
		mov	eax, [ebp+1E4h]
		mov	eax, [eax-4]
		test	eax, eax
		mov	[ebp-40h], eax
		jge	loc_401D10
		or	dword ptr [ebp-40h], 0FFFFFFFFh
		jmp	loc_401D10
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_4016C7:				; CODE XREF: _output+142j
		mov	eax, [ebp-40h]
		movsx	ecx, bl
		lea	eax, [eax+eax*4]
		lea	eax, [ecx+eax*2-30h]
		mov	[ebp-40h], eax
		jmp	loc_401D10
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_4016DC:				; CODE XREF: _output+85j
					; DATA XREF: .text:00401D51o
		cmp	bl, 49h
		jz	short loc_40170F
		cmp	bl, 68h
		jz	short loc_401706
		cmp	bl, 6Ch
		jz	short loc_4016FD
		cmp	bl, 77h
		jnz	loc_401D10
		or	byte ptr [ebp-3Bh], 8
		jmp	loc_401D10
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_4016FD:				; CODE XREF: _output+18Aj
		or	dword ptr [ebp-3Ch], 10h
		jmp	loc_401D10
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_401706:				; CODE XREF: _output+185j
		or	dword ptr [ebp-3Ch], 20h
		jmp	loc_401D10
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_40170F:				; CODE XREF: _output+180j
		mov	al, [edi]
		cmp	al, 36h
		jnz	short loc_40172C
		cmp	byte ptr [edi+1], 34h
		jnz	short loc_40172C
		inc	edi
		inc	edi
		or	byte ptr [ebp-3Bh], 80h
		mov	[ebp+1E0h], edi
		jmp	loc_401D10
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_40172C:				; CODE XREF: _output+1B4j _output+1BAj
		cmp	al, 33h
		jnz	short loc_401747
		cmp	byte ptr [edi+1], 32h
		jnz	short loc_401747
		inc	edi
		inc	edi
		and	byte ptr [ebp-3Bh], 7Fh
		mov	[ebp+1E0h], edi
		jmp	loc_401D10
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_401747:				; CODE XREF: _output+1CFj _output+1D5j
		cmp	al, 64h
		jz	loc_401D10
		cmp	al, 69h
		jz	loc_401D10
		cmp	al, 6Fh
		jz	loc_401D10
		cmp	al, 75h
		jz	loc_401D10
		cmp	al, 78h
		jz	loc_401D10
		cmp	al, 58h
		jz	loc_401D10
		and	dword ptr [ebp-70h], 0

loc_40177B:				; CODE XREF: _output+85j
					; DATA XREF: .text:off_401D39o
		mov	ecx, _pctype
		and	dword ptr [ebp-5Ch], 0
		movzx	eax, bl
		test	byte ptr [ecx+eax*2+1],	80h
		jz	short loc_4017A8
		mov	ecx, [ebp+1DCh]
		lea	esi, [ebp-4Ch]
		mov	al, bl
		call	write_char
		mov	bl, [edi]
		inc	edi
		mov	[ebp+1E0h], edi

loc_4017A8:				; CODE XREF: _output+22Ej
		mov	ecx, [ebp+1DCh]
		lea	esi, [ebp-4Ch]
		mov	al, bl
		call	write_char
		jmp	loc_401D10
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_4017BD:				; CODE XREF: _output+85j
					; DATA XREF: .text:00401D55o
		movsx	eax, bl
		cmp	eax, 67h
		jg	loc_401A21
		cmp	eax, 65h
		jge	loc_401858
		cmp	eax, 58h
		jg	loc_4018B9
		jz	loc_401AA2
		sub	eax, 43h
		jz	loc_401878
		dec	eax
		dec	eax
		jz	short loc_40184E
		dec	eax
		dec	eax
		jz	short loc_40184E
		sub	eax, 0Ch
		jnz	loc_401BFC
		test	word ptr [ebp-3Ch], 830h
		jnz	short loc_401807
		or	byte ptr [ebp-3Bh], 8

loc_401807:				; CODE XREF: _output+2A2j _output+4E1j
		mov	ecx, [ebp-40h]
		cmp	ecx, 0FFFFFFFFh
		jnz	short loc_401814
		mov	ecx, 7FFFFFFFh

loc_401814:				; CODE XREF: _output+2AEj
		add	dword ptr [ebp+1E4h], 4
		test	word ptr [ebp-3Ch], 810h
		mov	eax, [ebp+1E4h]
		mov	eax, [eax-4]
		mov	[ebp-44h], eax
		jz	loc_401A77
		test	eax, eax
		jnz	short loc_40183F
		mov	eax, __wnullstring
		mov	[ebp-44h], eax

loc_40183F:				; CODE XREF: _output+2D6j
		mov	eax, [ebp-44h]
		mov	dword ptr [ebp-5Ch], 1
		jmp	loc_401A69
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_40184E:				; CODE XREF: _output+28Dj _output+291j
		mov	dword ptr [ebp-6Ch], 1
		add	bl, 20h

loc_401858:				; CODE XREF: _output+26Dj
		or	dword ptr [ebp-3Ch], 40h
		cmp	dword ptr [ebp-40h], 0
		lea	esi, [ebp-38h]
		mov	[ebp-44h], esi
		jge	loc_401965
		mov	dword ptr [ebp-40h], 6
		jmp	loc_4019AC
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_401878:				; CODE XREF: _output+285j
		test	word ptr [ebp-3Ch], 830h
		jnz	short loc_401884
		or	byte ptr [ebp-3Bh], 8

loc_401884:				; CODE XREF: _output+31Fj _output+362j
		add	dword ptr [ebp+1E4h], 4
		test	word ptr [ebp-3Ch], 810h
		mov	eax, [ebp+1E4h]
		jz	short loc_4018FE
		movsx	eax, word ptr [eax-4]
		push	eax
		lea	eax, [ebp-38h]
		push	eax
		call	wctomb
		test	eax, eax
		pop	ecx
		pop	ecx
		mov	[ebp-48h], eax
		jge	short loc_40190B
		mov	dword ptr [ebp-64h], 1
		jmp	short loc_40190B
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_4018B9:				; CODE XREF: _output+276j
		sub	eax, 5Ah
		jz	short loc_401916
		sub	eax, 9
		jz	short loc_401884
		dec	eax
		jnz	loc_401BFC

loc_4018CA:				; CODE XREF: _output+4C5j
		or	dword ptr [ebp-3Ch], 40h

loc_4018CE:				; CODE XREF: _output+4E9j
		mov	dword ptr [ebp-48h], 0Ah

loc_4018D5:				; CODE XREF: _output+551j _output+56Aj ...
		mov	ebx, [ebp-3Ch]
		mov	esi, 8000h
		test	esi, ebx
		jz	loc_401B18
		mov	ecx, [ebp+1E4h]
		mov	eax, [ecx]
		mov	edx, [ecx+4]
		add	ecx, 8
		mov	[ebp+1E4h], ecx
		jmp	loc_401B46
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_4018FE:				; CODE XREF: _output+338j
		mov	al, [eax-4]
		mov	[ebp-38h], al
		mov	dword ptr [ebp-48h], 1

loc_40190B:				; CODE XREF: _output+34Fj _output+358j
		lea	eax, [ebp-38h]
		mov	[ebp-44h], eax
		jmp	loc_401BFC
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_401916:				; CODE XREF: _output+35Dj
		add	dword ptr [ebp+1E4h], 4
		mov	eax, [ebp+1E4h]
		mov	eax, [eax-4]
		test	eax, eax
		jz	short loc_401957
		mov	ecx, [eax+4]
		test	ecx, ecx
		jz	short loc_401957
		test	byte ptr [ebp-3Bh], 8
		movsx	eax, word ptr [eax]
		mov	[ebp-44h], ecx
		jz	short loc_40194E
		cdq
		sub	eax, edx
		sar	eax, 1
		mov	dword ptr [ebp-5Ch], 1
		jmp	loc_401BF9
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_40194E:				; CODE XREF: _output+3DCj
		and	dword ptr [ebp-5Ch], 0
		jmp	loc_401BF9
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_401957:				; CODE XREF: _output+3C9j _output+3D0j
		mov	eax, __nullstring
		mov	[ebp-44h], eax
		push	eax
		jmp	loc_401A16
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_401965:				; CODE XREF: _output+307j
		jnz	short loc_401975
		cmp	bl, 67h
		jnz	short loc_4019AC
		mov	dword ptr [ebp-40h], 1
		jmp	short loc_4019AC
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_401975:				; CODE XREF: _output:loc_401965j
		mov	eax, 200h
		cmp	[ebp-40h], eax
		jle	short loc_401982
		mov	[ebp-40h], eax

loc_401982:				; CODE XREF: _output+41Ej
		mov	edi, 0A3h
		cmp	[ebp-40h], edi
		jle	short loc_4019AC
		mov	eax, [ebp-40h]
		add	eax, 15Dh
		push	eax
		call	malloc
		test	eax, eax
		pop	ecx
		mov	[ebp-60h], eax
		jz	short loc_4019A9
		mov	[ebp-44h], eax
		mov	esi, eax
		jmp	short loc_4019AC
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_4019A9:				; CODE XREF: _output+441j
		mov	[ebp-40h], edi

loc_4019AC:				; CODE XREF: _output+314j _output+40Bj ...
		mov	eax, [ebp+1E4h]
		mov	ecx, [eax]
		push	dword ptr [ebp-6Ch]
		add	eax, 8
		push	dword ptr [ebp-40h]
		mov	[ebp+1E4h], eax
		mov	eax, [eax-4]
		mov	[ebp-7Ch], eax
		movsx	eax, bl
		push	eax
		lea	eax, [ebp-80h]
		push	esi
		push	eax
		mov	[ebp-80h], ecx
		call	_cfltcvt_tab
		mov	edi, [ebp-3Ch]
		add	esp, 14h
		and	edi, 80h
		jz	short loc_4019F7
		cmp	dword ptr [ebp-40h], 0
		jnz	short loc_4019F7
		push	esi
		call	off_40CF1C
		pop	ecx

loc_4019F7:				; CODE XREF: _output+488j _output+48Ej
		cmp	bl, 67h
		jnz	short loc_401A08
		test	edi, edi
		jnz	short loc_401A08
		push	esi
		call	off_40CF14
		pop	ecx

loc_401A08:				; CODE XREF: _output+49Bj _output+49Fj
		cmp	byte ptr [esi],	2Dh
		jnz	short loc_401A15
		or	byte ptr [ebp-3Bh], 1
		inc	esi
		mov	[ebp-44h], esi

loc_401A15:				; CODE XREF: _output+4ACj
		push	esi

loc_401A16:				; CODE XREF: _output+401j
		call	strlen
		pop	ecx
		jmp	loc_401BF9
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_401A21:				; CODE XREF: _output+264j
		sub	eax, 69h
		jz	loc_4018CA
		sub	eax, 5
		jz	loc_401AE8
		dec	eax
		jz	loc_401ACE
		dec	eax
		jz	short loc_401A9B
		sub	eax, 3
		jz	loc_401807
		dec	eax
		dec	eax
		jz	loc_4018CE
		sub	eax, 3
		jnz	loc_401BFC
		mov	dword ptr [ebp-68h], 27h
		jmp	short loc_401AA5
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_401A60:				; CODE XREF: _output+50Cj
		dec	ecx
		cmp	word ptr [eax],	0
		jz	short loc_401A6D
		inc	eax
		inc	eax

loc_401A69:				; CODE XREF: _output+2EAj
		test	ecx, ecx
		jnz	short loc_401A60

loc_401A6D:				; CODE XREF: _output+506j
		sub	eax, [ebp-44h]
		sar	eax, 1
		jmp	loc_401BF9
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_401A77:				; CODE XREF: _output+2CEj
		test	eax, eax
		jnz	short loc_401A83
		mov	eax, __nullstring
		mov	[ebp-44h], eax

loc_401A83:				; CODE XREF: _output+51Aj
		mov	eax, [ebp-44h]
		jmp	short loc_401A8F
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_401A88:				; CODE XREF: _output+532j
		dec	ecx
		cmp	byte ptr [eax],	0
		jz	short loc_401A93
		inc	eax

loc_401A8F:				; CODE XREF: _output+527j
		test	ecx, ecx
		jnz	short loc_401A88

loc_401A93:				; CODE XREF: _output+52Dj
		sub	eax, [ebp-44h]
		jmp	loc_401BF9
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_401A9B:				; CODE XREF: _output+4DCj
		mov	dword ptr [ebp-40h], 8

loc_401AA2:				; CODE XREF: _output+27Cj
		mov	[ebp-68h], ecx

loc_401AA5:				; CODE XREF: _output+4FFj
		test	byte ptr [ebp-3Ch], 80h
		mov	dword ptr [ebp-48h], 10h
		jz	loc_4018D5
		mov	al, [ebp-68h]
		add	al, 51h
		mov	byte ptr [ebp-50h], 30h
		mov	[ebp-4Fh], al
		mov	dword ptr [ebp-54h], 2
		jmp	loc_4018D5
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_401ACE:				; CODE XREF: _output+4D5j
		test	byte ptr [ebp-3Ch], 80h
		mov	dword ptr [ebp-48h], 8
		jz	loc_4018D5
		or	byte ptr [ebp-3Bh], 2
		jmp	loc_4018D5
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_401AE8:				; CODE XREF: _output+4CEj
		add	dword ptr [ebp+1E4h], 4
		test	byte ptr [ebp-3Ch], 20h
		mov	eax, [ebp+1E4h]
		mov	eax, [eax-4]
		jz	short loc_401B07
		mov	cx, [ebp-4Ch]
		mov	[eax], cx
		jmp	short loc_401B0C
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_401B07:				; CODE XREF: _output+59Dj
		mov	ecx, [ebp-4Ch]
		mov	[eax], ecx

loc_401B0C:				; CODE XREF: _output+5A6j
		mov	dword ptr [ebp-64h], 1
		jmp	loc_401CFD
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_401B18:				; CODE XREF: _output+380j
		add	dword ptr [ebp+1E4h], 4
		test	bl, 20h
		mov	eax, [ebp+1E4h]
		jz	short loc_401B3C
		test	bl, 40h
		jz	short loc_401B36
		movsx	eax, word ptr [eax-4]

loc_401B33:				; CODE XREF: _output+5DBj _output+5E3j
		cdq
		jmp	short loc_401B46
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_401B36:				; CODE XREF: _output+5CEj
		movzx	eax, word ptr [eax-4]
		jmp	short loc_401B33
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_401B3C:				; CODE XREF: _output+5C9j
		test	bl, 40h
		mov	eax, [eax-4]
		jnz	short loc_401B33
		xor	edx, edx

loc_401B46:				; CODE XREF: _output+39Aj _output+5D5j
		test	bl, 40h
		jz	short loc_401B60
		test	edx, edx
		jg	short loc_401B60
		jl	short loc_401B55
		test	eax, eax
		jnb	short loc_401B60

loc_401B55:				; CODE XREF: _output+5F0j
		neg	eax
		adc	edx, 0
		neg	edx
		or	byte ptr [ebp-3Bh], 1

loc_401B60:				; CODE XREF: _output+5EAj _output+5EEj ...
		test	[ebp-3Ch], esi
		mov	ebx, eax
		mov	edi, edx
		jnz	short loc_401B6B
		xor	edi, edi

loc_401B6B:				; CODE XREF: _output+608j
		cmp	dword ptr [ebp-40h], 0
		jge	short loc_401B7A
		mov	dword ptr [ebp-40h], 1
		jmp	short loc_401B8B
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_401B7A:				; CODE XREF: _output+610j
		and	dword ptr [ebp-3Ch], 0FFFFFFF7h
		mov	eax, 200h
		cmp	[ebp-40h], eax
		jle	short loc_401B8B
		mov	[ebp-40h], eax

loc_401B8B:				; CODE XREF: _output+619j _output+627j
		mov	eax, ebx
		or	eax, edi
		jnz	short loc_401B95
		and	dword ptr [ebp-54h], 0

loc_401B95:				; CODE XREF: _output+630j
		lea	esi, [ebp+1C7h]

loc_401B9B:				; CODE XREF: _output+66Ej
		mov	eax, [ebp-40h]
		dec	dword ptr [ebp-40h]
		test	eax, eax
		jg	short loc_401BAB
		mov	eax, ebx
		or	eax, edi
		jz	short loc_401BCF

loc_401BAB:				; CODE XREF: _output+644j
		mov	eax, [ebp-48h]
		cdq
		push	edx
		push	eax
		push	edi
		push	ebx
		call	_aulldvrm
		add	ecx, 30h
		cmp	ecx, 39h
		mov	[ebp-74h], ebx
		mov	ebx, eax
		mov	edi, edx
		jle	short loc_401BCA
		add	ecx, [ebp-68h]

loc_401BCA:				; CODE XREF: _output+666j
		mov	[esi], cl
		dec	esi
		jmp	short loc_401B9B
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_401BCF:				; CODE XREF: _output+64Aj
		lea	eax, [ebp+1C7h]
		sub	eax, esi
		inc	esi
		test	byte ptr [ebp-3Bh], 2
		mov	[ebp-48h], eax
		mov	[ebp-44h], esi
		jz	short loc_401BFC
		mov	ecx, esi
		cmp	byte ptr [ecx],	30h
		jnz	short loc_401BEF
		test	eax, eax
		jnz	short loc_401BFC

loc_401BEF:				; CODE XREF: _output+68Aj
		dec	dword ptr [ebp-44h]
		mov	ecx, [ebp-44h]
		mov	byte ptr [ecx],	30h
		inc	eax

loc_401BF9:				; CODE XREF: _output+3EAj _output+3F3j ...
		mov	[ebp-48h], eax

loc_401BFC:				; CODE XREF: _output+296j _output+365j ...
		cmp	dword ptr [ebp-64h], 0
		jnz	loc_401CFD
		mov	ebx, [ebp-3Ch]
		test	bl, 40h
		jz	short loc_401C34
		test	bh, 1
		jz	short loc_401C19
		mov	byte ptr [ebp-50h], 2Dh
		jmp	short loc_401C2D
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_401C19:				; CODE XREF: _output+6B2j
		test	bl, 1
		jz	short loc_401C24
		mov	byte ptr [ebp-50h], 2Bh
		jmp	short loc_401C2D
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_401C24:				; CODE XREF: _output+6BDj
		test	bl, 2
		jz	short loc_401C34
		mov	byte ptr [ebp-50h], 20h

loc_401C2D:				; CODE XREF: _output+6B8j _output+6C3j
		mov	dword ptr [ebp-54h], 1

loc_401C34:				; CODE XREF: _output+6ADj _output+6C8j
		mov	esi, [ebp-58h]
		sub	esi, [ebp-54h]
		sub	esi, [ebp-48h]
		test	bl, 0Ch
		jnz	short loc_401C56
		push	dword ptr [ebp+1DCh]
		lea	eax, [ebp-4Ch]
		push	esi
		push	20h
		call	write_multi_char
		add	esp, 0Ch

loc_401C56:				; CODE XREF: _output+6E1j
		push	dword ptr [ebp-54h]
		mov	edi, [ebp+1DCh]
		lea	eax, [ebp-4Ch]
		lea	ecx, [ebp-50h]
		call	write_string
		test	bl, 8
		pop	ecx
		jz	short loc_401C84
		test	bl, 4
		jnz	short loc_401C84
		push	edi
		push	esi
		push	30h
		lea	eax, [ebp-4Ch]
		call	write_multi_char
		add	esp, 0Ch

loc_401C84:				; CODE XREF: _output+70Fj _output+714j
		cmp	dword ptr [ebp-5Ch], 0
		jz	short loc_401CD4
		cmp	dword ptr [ebp-48h], 0
		jle	short loc_401CD4
		mov	eax, [ebp-48h]
		mov	ebx, [ebp-44h]
		mov	[ebp-74h], eax

loc_401C99:				; CODE XREF: _output+771j
		dec	dword ptr [ebp-74h]
		xor	eax, eax
		mov	ax, [ebx]
		push	eax
		lea	eax, [ebp+1C8h]
		push	eax
		call	wctomb
		inc	ebx
		pop	ecx
		inc	ebx
		test	eax, eax
		pop	ecx
		jle	short loc_401CE3
		mov	edi, [ebp+1DCh]
		push	eax
		lea	eax, [ebp-4Ch]
		lea	ecx, [ebp+1C8h]
		call	write_string
		cmp	dword ptr [ebp-74h], 0
		pop	ecx
		jnz	short loc_401C99
		jmp	short loc_401CE3
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_401CD4:				; CODE XREF: _output+729j _output+72Fj
		push	dword ptr [ebp-48h]
		mov	ecx, [ebp-44h]
		lea	eax, [ebp-4Ch]
		call	write_string
		pop	ecx

loc_401CE3:				; CODE XREF: _output+755j _output+773j
		test	byte ptr [ebp-3Ch], 4
		jz	short loc_401CFD
		push	dword ptr [ebp+1DCh]
		lea	eax, [ebp-4Ch]
		push	esi
		push	20h
		call	write_multi_char
		add	esp, 0Ch

loc_401CFD:				; CODE XREF: _output+5B4j _output+6A1j ...
		cmp	dword ptr [ebp-60h], 0
		jz	short loc_401D10
		push	dword ptr [ebp-60h]
		call	free
		and	dword ptr [ebp-60h], 0
		pop	ecx

loc_401D10:				; CODE XREF: _output+7Fj _output+A4j ...
		mov	edi, [ebp+1E0h]
		mov	bl, [edi]
		test	bl, bl
		jnz	loc_40159C

loc_401D20:				; CODE XREF: _output+4Bj
		pop	edi
		pop	esi

loc_401D22:				; CODE XREF: _output+31j
		mov	ecx, [ebp+1D0h]
		mov	eax, [ebp-4Ch]
		pop	ebx
		call	__security_check_cookie
		add	ebp, 1D4h
		leave
		retn
_output		endp

; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
off_401D39	dd offset loc_40177B	; DATA XREF: _output+85r
		dd offset loc_4015EB
		dd offset loc_401608
		dd offset loc_401654
		dd offset loc_401695
		dd offset loc_40169E
		dd offset loc_4016DC
		dd offset loc_4017BD

; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


__iob_func	proc near
		mov	eax, offset _iob
		retn
__iob_func	endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


__initstdio	proc near		; DATA XREF: .data:pinito
		mov	eax, _nstream
		test	eax, eax
		push	esi
		push	14h
		pop	esi
		jnz	short loc_401D73
		mov	eax, 200h
		jmp	short loc_401D79
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_401D73:				; CODE XREF: __initstdio+Bj
		cmp	eax, esi
		jge	short loc_401D7E
		mov	eax, esi

loc_401D79:				; CODE XREF: __initstdio+12j
		mov	_nstream, eax

loc_401D7E:				; CODE XREF: __initstdio+16j
		push	4
		push	eax
		call	calloc
		test	eax, eax
		pop	ecx
		pop	ecx
		mov	__piob,	eax
		jnz	short loc_401DAF
		push	4
		push	esi
		mov	_nstream, esi
		call	calloc
		test	eax, eax
		pop	ecx
		pop	ecx
		mov	__piob,	eax
		jnz	short loc_401DAF
		push	1Ah
		pop	eax
		pop	esi
		retn
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_401DAF:				; CODE XREF: __initstdio+30j
					; __initstdio+49j
		xor	edx, edx
		mov	ecx, offset _iob
		jmp	short loc_401DBD
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_401DB8:				; CODE XREF: __initstdio+6Dj
		mov	eax, __piob

loc_401DBD:				; CODE XREF: __initstdio+57j
		mov	[edx+eax], ecx
		add	ecx, 20h
		add	edx, 4
		cmp	ecx, offset rterrs
		jl	short loc_401DB8
		xor	edx, edx
		mov	ecx, offset dword_40CB70

loc_401DD5:				; CODE XREF: __initstdio+A0j
		mov	eax, edx
		sar	eax, 5
		mov	eax, __pioinfo[eax*4]
		mov	esi, edx
		and	esi, 1Fh
		mov	eax, [eax+esi*8]
		cmp	eax, 0FFFFFFFFh
		jz	short loc_401DF2
		test	eax, eax
		jnz	short loc_401DF5

loc_401DF2:				; CODE XREF: __initstdio+8Dj
		or	dword ptr [ecx], 0FFFFFFFFh

loc_401DF5:				; CODE XREF: __initstdio+91j
		add	ecx, 20h
		inc	edx
		cmp	ecx, offset dword_40CBD0
		jl	short loc_401DD5
		xor	eax, eax
		pop	esi
		retn
__initstdio	endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


__endstdio	proc near		; DATA XREF: .data:ptermo
		call	_flushall
		cmp	_exitflag, 0
		jz	short locret_401E18
		jmp	_fcloseall
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

locret_401E18:				; CODE XREF: __endstdio+Cj
		retn
__endstdio	endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


__crtExitProcess proc near		; CODE XREF: fast_error_exit+1Cp
					; mainCRTStartup+EAp ...

arg_0		= dword	ptr  4

		push	offset ??_C@_0M@MBOPBNBK@mscoree?4dll?$AA@ ; "mscoree.dll"
		call	ds:__imp__GetModuleHandleA@4 ; __declspec(dllimport) GetModuleHandleA(x)
		test	eax, eax
		jz	short loc_401E3E
		push	offset ??_C@_0P@MIGLKIOC@CorExitProcess?$AA@ ; "CorExitProcess"
		push	eax
		call	ds:__imp__GetProcAddress@8 ; __declspec(dllimport) GetProcAddress(x,x)
		test	eax, eax
		jz	short loc_401E3E
		push	[esp+arg_0]
		call	eax

loc_401E3E:				; CODE XREF: __crtExitProcess+Dj
					; __crtExitProcess+1Dj
		push	dword ptr [esp+4]
		call	ds:__imp__ExitProcess@4	; __declspec(dllimport)	ExitProcess(x)
		int	3		; Trap to Debugger
__crtExitProcess endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


_initterm	proc near

arg_0		= dword	ptr  8

		push	esi
		mov	esi, eax
		jmp	short loc_401E59
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_401E4E:				; CODE XREF: _initterm+14j
		mov	eax, [esi]
		test	eax, eax
		jz	short loc_401E56
		call	eax

loc_401E56:				; CODE XREF: _initterm+9j
		add	esi, 4

loc_401E59:				; CODE XREF: _initterm+3j
		cmp	esi, [esp+arg_0]
		jb	short loc_401E4E
		pop	esi
		retn
_initterm	endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


_initterm_e	proc near

arg_0		= dword	ptr  8

		push	esi
		mov	esi, eax
		xor	eax, eax
		jmp	short loc_401E77
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_401E68:				; CODE XREF: _initterm_e+1Aj
		test	eax, eax
		jnz	short loc_401E7D
		mov	ecx, [esi]
		test	ecx, ecx
		jz	short loc_401E74
		call	ecx

loc_401E74:				; CODE XREF: _initterm_e+Fj
		add	esi, 4

loc_401E77:				; CODE XREF: _initterm_e+5j
		cmp	esi, [esp+arg_0]
		jb	short loc_401E68

loc_401E7D:				; CODE XREF: _initterm_e+9j
		pop	esi
		retn
_initterm_e	endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


_cinit		proc near		; CODE XREF: mainCRTStartup+143p

arg_0		= dword	ptr  4

		mov	eax, _FPinit
		test	eax, eax
		jz	short loc_401E8F
		push	[esp+arg_0]
		call	eax
		pop	ecx

loc_401E8F:				; CODE XREF: _cinit+7j
		push	esi
		push	edi
		mov	ecx, offset __xi_a
		mov	edi, offset __xi_z
		xor	eax, eax
		cmp	ecx, edi
		mov	esi, ecx
		jnb	short loc_401EBA

loc_401EA3:				; CODE XREF: _cinit+35j
		test	eax, eax
		jnz	short loc_401EE6
		mov	ecx, [esi]
		test	ecx, ecx
		jz	short loc_401EAF
		call	ecx

loc_401EAF:				; CODE XREF: _cinit+2Cj
		add	esi, 4
		cmp	esi, edi
		jb	short loc_401EA3
		test	eax, eax
		jnz	short loc_401EE6

loc_401EBA:				; CODE XREF: _cinit+22j
		push	offset _RTC_Terminate
		call	atexit
		mov	esi, offset __xc_a
		mov	eax, esi
		mov	edi, offset __xc_z
		cmp	eax, edi
		pop	ecx
		jnb	short loc_401EE4

loc_401ED5:				; CODE XREF: _cinit+63j
		mov	eax, [esi]
		test	eax, eax
		jz	short loc_401EDD
		call	eax

loc_401EDD:				; CODE XREF: _cinit+5Aj
		add	esi, 4
		cmp	esi, edi
		jb	short loc_401ED5

loc_401EE4:				; CODE XREF: _cinit+54j
		xor	eax, eax

loc_401EE6:				; CODE XREF: _cinit+26j _cinit+39j
		pop	edi
		pop	esi
		retn
_cinit		endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ

; Attributes: bp-based frame

doexit		proc near		; CODE XREF: exit+8p _exit+8p	...

arg_0		= dword	ptr  8
arg_4		= dword	ptr  0Ch
arg_8		= byte ptr  10h

		push	ebp
		mov	ebp, esp
		push	esi
		xor	esi, esi
		inc	esi
		cmp	_C_Exit_Done, esi
		push	edi
		jnz	short loc_401F09
		push	[ebp+arg_0]
		call	ds:__imp__GetCurrentProcess@0 ;	__declspec(dllimport) GetCurrentProcess()
		push	eax
		call	ds:__imp__TerminateProcess@8 ; __declspec(dllimport) TerminateProcess(x,x)

loc_401F09:				; CODE XREF: doexit+Ej
		cmp	[ebp+arg_4], 0
		mov	al, [ebp+arg_8]
		mov	_C_Termination_Done, esi
		mov	_exitflag, al
		jnz	short loc_401F6F
		mov	ecx, __onexitbegin
		test	ecx, ecx
		jz	short loc_401F50
		mov	eax, __onexitend
		sub	eax, 4
		cmp	eax, ecx
		jmp	short loc_401F49
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_401F33:				; CODE XREF: doexit+65j
		mov	eax, [eax]
		test	eax, eax
		jz	short loc_401F3B
		call	eax

loc_401F3B:				; CODE XREF: doexit+4Ej
		mov	eax, __onexitend
		sub	eax, 4
		cmp	eax, __onexitbegin

loc_401F49:				; CODE XREF: doexit+48j
		mov	__onexitend, eax
		jnb	short loc_401F33

loc_401F50:				; CODE XREF: doexit+3Cj
		mov	eax, offset __xp_a
		mov	esi, offset __xp_z
		cmp	eax, esi
		mov	edi, eax
		jnb	short loc_401F6F

loc_401F60:				; CODE XREF: doexit+84j
		mov	eax, [edi]
		test	eax, eax
		jz	short loc_401F68
		call	eax

loc_401F68:				; CODE XREF: doexit+7Bj
		add	edi, 4
		cmp	edi, esi
		jb	short loc_401F60

loc_401F6F:				; CODE XREF: doexit+32j doexit+75j
		mov	eax, offset __xt_a
		mov	esi, offset __xt_z
		cmp	eax, esi
		mov	edi, eax
		jnb	short loc_401F8E

loc_401F7F:				; CODE XREF: doexit+A3j
		mov	eax, [edi]
		test	eax, eax
		jz	short loc_401F87
		call	eax

loc_401F87:				; CODE XREF: doexit+9Aj
		add	edi, 4
		cmp	edi, esi
		jb	short loc_401F7F

loc_401F8E:				; CODE XREF: doexit+94j
		cmp	dword ptr [ebp+arg_8], 0
		pop	edi
		pop	esi
		jnz	short loc_401FA8
		push	[ebp+arg_0]
		mov	_C_Exit_Done, 1
		call	__crtExitProcess

loc_401FA8:				; CODE XREF: doexit+ABj
		pop	ebp
		retn
doexit		endp ; sp = -4


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


exit		proc near		; CODE XREF: mainCRTStartup+181p

arg_0		= dword	ptr  4

		push	0
		push	0
		push	[esp+8+arg_0]
		call	doexit
		add	esp, 0Ch
		retn
exit		endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


; void __cdecl exit(int)
_exit		proc near		; CODE XREF: _amsg_exit+1Cp
					; mainCRTStartup+1AEp ...

arg_0		= dword	ptr  4

		push	0
		push	1
		push	[esp+8+arg_0]
		call	doexit
		add	esp, 0Ch
		retn
_exit		endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


_cexit		proc near		; CODE XREF: mainCRTStartup:loc_4013ADp
		push	1
		push	0
		push	0
		call	doexit
		add	esp, 0Ch
		retn
_cexit		endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


_c_exit		proc near		; CODE XREF: mainCRTStartup:loc_4013DAp
		push	1
		push	1
		push	0
		call	doexit
		add	esp, 0Ch
		retn
_c_exit		endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


_NMSG_WRITE	proc near		; CODE XREF: _amsg_exit+12p
					; fast_error_exit+12p ...

var_8C		= dword	ptr -8Ch

		push	ebp
		lea	ebp, [esp+var_8C]
		sub	esp, 10Ch
		mov	eax, __security_cookie
		mov	ecx, [ebp+94h]
		push	ebx
		push	esi
		mov	[ebp+88h], eax
		xor	edx, edx
		push	edi
		xor	eax, eax

loc_402010:				; CODE XREF: _NMSG_WRITE+33j
		cmp	ecx, rterrs[eax*8]
		jz	short loc_40201F
		inc	eax
		cmp	eax, 13h
		jb	short loc_402010

loc_40201F:				; CODE XREF: _NMSG_WRITE+2Dj
		mov	esi, eax
		shl	esi, 3
		cmp	ecx, rterrs[esi]
		jnz	loc_402145
		mov	eax, __error_mode
		cmp	eax, 1
		jz	loc_40211D
		cmp	eax, edx
		jnz	short loc_40204F
		cmp	__app_type, 1
		jz	loc_40211D

loc_40204F:				; CODE XREF: _NMSG_WRITE+56j
		cmp	ecx, 0FCh
		jz	loc_402145
		push	104h
		lea	eax, [ebp-80h]
		push	eax
		push	edx
		mov	[ebp+84h], dl
		call	ds:__imp__GetModuleFileNameA@12	; __declspec(dllimport)	GetModuleFileNameA(x,x,x)
		test	eax, eax
		jnz	short loc_402085
		lea	eax, [ebp-80h]
		push	offset ??_C@_0BH@DNAGHKFM@?$DMprogram?5name?5unknown?$DO?$AA@ ;	"<program name unknown>"
		push	eax
		call	strcpy
		pop	ecx
		pop	ecx

loc_402085:				; CODE XREF: _NMSG_WRITE+89j
		lea	edi, [ebp-80h]
		mov	eax, edi
		push	eax
		call	strlen
		inc	eax
		cmp	eax, 3Ch
		pop	ecx
		jbe	short loc_4020B9
		mov	eax, edi
		push	eax
		call	strlen
		mov	edi, eax
		lea	eax, [ebp-80h]
		sub	eax, 3Bh
		push	3
		add	edi, eax
		push	offset ??_C@_03KHICJKCI@?4?4?4?$AA@ ; "..."
		push	edi
		call	strncpy
		add	esp, 10h

loc_4020B9:				; CODE XREF: _NMSG_WRITE+ABj
		push	edi
		call	strlen
		push	dword_40CDE4[esi]
		mov	ebx, eax
		call	strlen
		lea	eax, [ebx+eax+1Ch]
		pop	ecx
		add	eax, 3
		pop	ecx
		and	eax, 0FFFFFFFCh
		call	_chkstk
		mov	ebx, esp
		push	offset ??_C@_0BK@OFGJDLJJ@Runtime?5Error?$CB?6?6Program?3?5?$AA@ ; "Runtime Error!\n\nProgram: "
		push	ebx
		call	strcpy
		push	edi
		push	ebx
		call	strcat
		push	offset ??_C@_02PHMGELLB@?6?6?$AA@ ; "\n\n"
		push	ebx
		call	strcat
		push	dword_40CDE4[esi]
		push	ebx
		call	strcat
		push	12010h
		push	offset ??_C@_0CF@GOGNBNAK@Microsoft?5Visual?5C?$CL?$CL?5Runtime?5Lib@ ;	"Microsoft Visual C++ Runtime Library"
		push	ebx
		call	__crtMessageBoxA
		add	esp, 2Ch
		jmp	short loc_402145
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_40211D:				; CODE XREF: _NMSG_WRITE+4Ej
					; _NMSG_WRITE+5Fj
		push	edx
		lea	eax, [ebp+94h]
		push	eax
		lea	esi, dword_40CDE4[esi]
		push	dword ptr [esi]
		call	strlen
		pop	ecx
		push	eax
		push	dword ptr [esi]
		push	0FFFFFFF4h
		call	ds:__imp__GetStdHandle@4 ; __declspec(dllimport) GetStdHandle(x)
		push	eax
		call	ds:__imp__WriteFile@20 ; __declspec(dllimport) WriteFile(x,x,x,x,x)

loc_402145:				; CODE XREF: _NMSG_WRITE+40j
					; _NMSG_WRITE+6Bj ...
		lea	esp, [ebp-8Ch]
		mov	ecx, [ebp+88h]
		call	__security_check_cookie
		pop	edi
		pop	esi
		pop	ebx
		add	ebp, 8Ch
		leave
		retn
_NMSG_WRITE	endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


_GET_RTERRMSG	proc near

arg_0		= dword	ptr  4

		mov	ecx, [esp+arg_0]
		xor	eax, eax

loc_402167:				; CODE XREF: _GET_RTERRMSG+13j
		cmp	ecx, rterrs[eax*8]
		jz	short loc_402176
		inc	eax
		cmp	eax, 13h
		jb	short loc_402167

loc_402176:				; CODE XREF: _GET_RTERRMSG+Dj
		shl	eax, 3
		cmp	ecx, rterrs[eax]
		jnz	short loc_402188
		mov	eax, dword_40CDE4[eax]
		retn
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_402188:				; CODE XREF: _GET_RTERRMSG+1Ej
		xor	eax, eax
		retn
_GET_RTERRMSG	endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


_FF_MSGBANNER	proc near		; CODE XREF: _amsg_exit+9p
					; fast_error_exit+9p ...
		mov	eax, __error_mode
		cmp	eax, 1
		jz	short loc_4021A2
		test	eax, eax
		jnz	short locret_4021C3
		cmp	__app_type, 1
		jnz	short locret_4021C3

loc_4021A2:				; CODE XREF: _FF_MSGBANNER+8j
		push	0FCh
		call	_NMSG_WRITE
		mov	eax, _adbgmsg
		test	eax, eax
		pop	ecx
		jz	short loc_4021B8
		call	eax

loc_4021B8:				; CODE XREF: _FF_MSGBANNER+29j
		push	0FFh
		call	_NMSG_WRITE
		pop	ecx

locret_4021C3:				; CODE XREF: _FF_MSGBANNER+Cj
					; _FF_MSGBANNER+15j
		retn
_FF_MSGBANNER	endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


xcptlookup	proc near
		mov	ecx, _XcptActTabCount
		mov	eax, offset _XcptActTab
		push	esi

loc_4021D0:				; CODE XREF: xcptlookup+1Fj
		cmp	[eax], edx
		jz	short loc_4021E5
		lea	esi, [ecx+ecx*2]
		add	eax, 0Ch
		lea	esi, ds:40CE78h[esi*4]
		cmp	eax, esi
		jb	short loc_4021D0

loc_4021E5:				; CODE XREF: xcptlookup+Ej
		lea	ecx, [ecx+ecx*2]
		lea	ecx, ds:40CE78h[ecx*4]
		cmp	eax, ecx
		pop	esi
		jnb	short loc_4021F8
		cmp	[eax], edx
		jz	short locret_4021FA

loc_4021F8:				; CODE XREF: xcptlookup+2Ej
		xor	eax, eax

locret_4021FA:				; CODE XREF: xcptlookup+32j
		retn
xcptlookup	endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ

; Attributes: bp-based frame

_XcptFilter	proc near		; CODE XREF: mainCRTStartup+199p
					; __CppXcptFilter+10p

arg_0		= dword	ptr  8
arg_4		= dword	ptr  0Ch

		push	ebp
		mov	ebp, esp
		mov	edx, [ebp+arg_0]
		mov	eax, _XcptActTabCount
		push	ebx
		mov	ecx, offset _XcptActTab
		push	esi

loc_40220D:				; CODE XREF: _XcptFilter+25j
		cmp	[ecx], edx
		jz	short loc_402222
		lea	esi, [eax+eax*2]
		add	ecx, 0Ch
		lea	esi, ds:40CE78h[esi*4]
		cmp	ecx, esi
		jb	short loc_40220D

loc_402222:				; CODE XREF: _XcptFilter+14j
		lea	eax, [eax+eax*2]
		lea	eax, ds:40CE78h[eax*4]
		cmp	ecx, eax
		jnb	short loc_402234
		cmp	[ecx], edx
		jz	short loc_402236

loc_402234:				; CODE XREF: _XcptFilter+33j
		xor	ecx, ecx

loc_402236:				; CODE XREF: _XcptFilter+37j
		test	ecx, ecx
		jz	loc_40235F
		mov	ebx, [ecx+8]
		test	ebx, ebx
		jz	loc_40235F
		cmp	ebx, 5
		jnz	short loc_40225A
		and	dword ptr [ecx+8], 0
		xor	eax, eax
		inc	eax
		jmp	loc_402368
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_40225A:				; CODE XREF: _XcptFilter+51j
		cmp	ebx, 1
		jz	loc_40235A
		mov	eax, _pxcptinfoptrs
		mov	[ebp+arg_0], eax
		mov	eax, [ebp+arg_4]
		mov	_pxcptinfoptrs,	eax
		mov	eax, [ecx+4]
		cmp	eax, 8
		jnz	loc_40234A
		mov	eax, _First_FPE_Indx
		mov	edx, _Num_FPE
		add	edx, eax
		cmp	eax, edx
		jge	short loc_4022A5
		lea	esi, [eax+eax*2]
		lea	esi, ds:40CE80h[esi*4]
		sub	edx, eax

loc_40229C:				; CODE XREF: _XcptFilter+A8j
		and	dword ptr [esi], 0
		add	esi, 0Ch
		dec	edx
		jnz	short loc_40229C

loc_4022A5:				; CODE XREF: _XcptFilter+93j
		mov	ecx, [ecx]
		cmp	ecx, 0C000008Eh
		mov	esi, _fpecode
		jnz	short loc_4022C1
		mov	_fpecode, 83h
		jmp	short loc_402337
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_4022C1:				; CODE XREF: _XcptFilter+B8j
		cmp	ecx, 0C0000090h
		jnz	short loc_4022D5
		mov	_fpecode, 81h
		jmp	short loc_402337
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_4022D5:				; CODE XREF: _XcptFilter+CCj
		cmp	ecx, 0C0000091h
		jnz	short loc_4022E9
		mov	_fpecode, 84h
		jmp	short loc_402337
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_4022E9:				; CODE XREF: _XcptFilter+E0j
		cmp	ecx, 0C0000093h
		jnz	short loc_4022FD
		mov	_fpecode, 85h
		jmp	short loc_402337
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_4022FD:				; CODE XREF: _XcptFilter+F4j
		cmp	ecx, 0C000008Dh
		jnz	short loc_402311
		mov	_fpecode, 82h
		jmp	short loc_402337
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_402311:				; CODE XREF: _XcptFilter+108j
		cmp	ecx, 0C000008Fh
		jnz	short loc_402325
		mov	_fpecode, 86h
		jmp	short loc_402337
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_402325:				; CODE XREF: _XcptFilter+11Cj
		cmp	ecx, 0C0000092h
		jnz	short loc_402337
		mov	_fpecode, 8Ah

loc_402337:				; CODE XREF: _XcptFilter+C4j
					; _XcptFilter+D8j ...
		push	_fpecode
		push	8
		call	ebx
		pop	ecx
		mov	_fpecode, esi
		jmp	short loc_402351
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_40234A:				; CODE XREF: _XcptFilter+7Ej
		and	dword ptr [ecx+8], 0
		push	eax
		call	ebx

loc_402351:				; CODE XREF: _XcptFilter+14Dj
		mov	eax, [ebp+arg_0]
		pop	ecx
		mov	_pxcptinfoptrs,	eax

loc_40235A:				; CODE XREF: _XcptFilter+62j
		or	eax, 0FFFFFFFFh
		jmp	short loc_402368
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_40235F:				; CODE XREF: _XcptFilter+3Dj
					; _XcptFilter+48j
		push	[ebp+arg_4]
		call	ds:__imp__UnhandledExceptionFilter@4 ; __declspec(dllimport) UnhandledExceptionFilter(x)

loc_402368:				; CODE XREF: _XcptFilter+5Aj
					; _XcptFilter+162j
		pop	esi
		pop	ebx
		pop	ebp
		retn
_XcptFilter	endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


__CppXcptFilter	proc near

arg_0		= dword	ptr  4
arg_4		= dword	ptr  8

		mov	eax, 0E06D7363h
		cmp	[esp+arg_0], eax
		jnz	short loc_402384
		push	[esp+arg_4]
		push	eax
		call	_XcptFilter
		pop	ecx
		pop	ecx
		retn
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_402384:				; CODE XREF: __CppXcptFilter+9j
		xor	eax, eax
		retn
__CppXcptFilter	endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


_setenvp	proc near		; CODE XREF: mainCRTStartup:loc_401357p
		push	ebx
		xor	ebx, ebx
		cmp	__mbctype_initialized, ebx
		push	esi
		push	edi
		jnz	short loc_402399
		call	__initmbctable

loc_402399:				; CODE XREF: _setenvp+Bj
		mov	esi, _aenvptr
		xor	edi, edi
		cmp	esi, ebx
		jnz	short loc_4023B7
		jmp	short loc_4023D7
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_4023A7:				; CODE XREF: _setenvp+34j
		cmp	al, 3Dh
		jz	short loc_4023AC
		inc	edi

loc_4023AC:				; CODE XREF: _setenvp+22j
		push	esi
		call	strlen
		pop	ecx
		lea	esi, [esi+eax+1]

loc_4023B7:				; CODE XREF: _setenvp+1Cj
		mov	al, [esi]
		cmp	al, bl
		jnz	short loc_4023A7
		lea	eax, ds:4[edi*4]
		push	eax
		call	malloc
		mov	edi, eax
		cmp	edi, ebx
		pop	ecx
		mov	_environ, edi
		jnz	short loc_4023DC

loc_4023D7:				; CODE XREF: _setenvp+1Ej
		or	eax, 0FFFFFFFFh
		jmp	short loc_402434
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_4023DC:				; CODE XREF: _setenvp+4Ej
		mov	esi, _aenvptr
		push	ebp
		jmp	short loc_40240F
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_4023E5:				; CODE XREF: _setenvp+8Aj
		push	esi
		call	strlen
		mov	ebp, eax
		inc	ebp
		cmp	byte ptr [esi],	3Dh
		pop	ecx
		jz	short loc_40240D
		push	ebp
		call	malloc
		cmp	eax, ebx
		pop	ecx
		mov	[edi], eax
		jz	short loc_402438
		push	esi
		push	eax
		call	strcpy
		pop	ecx
		pop	ecx
		add	edi, 4

loc_40240D:				; CODE XREF: _setenvp+6Bj
		add	esi, ebp

loc_40240F:				; CODE XREF: _setenvp+5Cj
		cmp	[esi], bl
		jnz	short loc_4023E5
		push	_aenvptr
		call	free
		mov	_aenvptr, ebx
		mov	[edi], ebx
		mov	__env_initialized, 1
		xor	eax, eax

loc_402432:				; CODE XREF: _setenvp+C5j
		pop	ecx
		pop	ebp

loc_402434:				; CODE XREF: _setenvp+53j
		pop	edi
		pop	esi
		pop	ebx
		retn
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_402438:				; CODE XREF: _setenvp+78j
		push	_environ
		call	free
		mov	_environ, ebx
		or	eax, 0FFFFFFFFh
		jmp	short loc_402432
_setenvp	endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ

; Attributes: bp-based frame

parse_cmdline	proc near		; CODE XREF: _setargv+54p _setargv+85p

var_4		= dword	ptr -4
arg_0		= dword	ptr  8
arg_4		= dword	ptr  0Ch

		push	ebp
		mov	ebp, esp
		push	ecx
		push	ebx
		mov	ebx, [ebp+arg_4]
		xor	edx, edx
		cmp	[ebp+arg_0], edx
		push	edi
		mov	[esi], edx
		mov	edi, ecx
		mov	dword ptr [ebx], 1
		jz	short loc_402471
		mov	ecx, [ebp+arg_0]
		add	[ebp+arg_0], 4
		mov	[ecx], edi

loc_402471:				; CODE XREF: parse_cmdline+18j
					; parse_cmdline+65j ...
		cmp	byte ptr [eax],	22h
		jnz	short loc_402484
		xor	ecx, ecx
		test	edx, edx
		setz	cl
		inc	eax
		mov	edx, ecx
		mov	cl, 22h
		jmp	short loc_4024B1
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_402484:				; CODE XREF: parse_cmdline+26j
		inc	dword ptr [esi]
		test	edi, edi
		jz	short loc_40248F
		mov	cl, [eax]
		mov	[edi], cl
		inc	edi

loc_40248F:				; CODE XREF: parse_cmdline+3Aj
		mov	cl, [eax]
		movzx	ebx, cl
		inc	eax
		test	byte_40D701[ebx], 4
		jz	short loc_4024AA
		inc	dword ptr [esi]
		test	edi, edi
		jz	short loc_4024A9
		mov	bl, [eax]
		mov	[edi], bl
		inc	edi

loc_4024A9:				; CODE XREF: parse_cmdline+54j
		inc	eax

loc_4024AA:				; CODE XREF: parse_cmdline+4Ej
		test	cl, cl
		mov	ebx, [ebp+arg_4]
		jz	short loc_4024E3

loc_4024B1:				; CODE XREF: parse_cmdline+34j
		test	edx, edx
		jnz	short loc_402471
		cmp	cl, 20h
		jz	short loc_4024BF
		cmp	cl, 9
		jnz	short loc_402471

loc_4024BF:				; CODE XREF: parse_cmdline+6Aj
		test	edi, edi
		jz	short loc_4024C7
		mov	byte ptr [edi-1], 0

loc_4024C7:				; CODE XREF: parse_cmdline+73j
					; parse_cmdline+96j
		and	[ebp+var_4], 0

loc_4024CB:				; CODE XREF: parse_cmdline+157j
		cmp	byte ptr [eax],	0
		jz	loc_4025AA

loc_4024D4:				; CODE XREF: parse_cmdline+93j
		mov	cl, [eax]
		cmp	cl, 20h
		jz	short loc_4024E0
		cmp	cl, 9
		jnz	short loc_4024E6

loc_4024E0:				; CODE XREF: parse_cmdline+8Bj
		inc	eax
		jmp	short loc_4024D4
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_4024E3:				; CODE XREF: parse_cmdline+61j
		dec	eax
		jmp	short loc_4024C7
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_4024E6:				; CODE XREF: parse_cmdline+90j
		cmp	byte ptr [eax],	0
		jz	loc_4025AA
		cmp	[ebp+arg_0], 0
		jz	short loc_4024FE
		mov	ecx, [ebp+arg_0]
		add	[ebp+arg_0], 4
		mov	[ecx], edi

loc_4024FE:				; CODE XREF: parse_cmdline+A5j
		inc	dword ptr [ebx]

loc_402500:				; CODE XREF: parse_cmdline+145j
		xor	ebx, ebx
		inc	ebx
		xor	edx, edx
		jmp	short loc_402509
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_402507:				; CODE XREF: parse_cmdline+BEj
		inc	eax
		inc	edx

loc_402509:				; CODE XREF: parse_cmdline+B7j
		cmp	byte ptr [eax],	5Ch
		jz	short loc_402507
		cmp	byte ptr [eax],	22h
		jnz	short loc_402539
		test	dl, 1
		jnz	short loc_402537
		cmp	[ebp+var_4], 0
		jz	short loc_40252A
		lea	ecx, [eax+1]
		cmp	byte ptr [ecx],	22h
		jnz	short loc_40252A
		mov	eax, ecx
		jmp	short loc_40252C
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_40252A:				; CODE XREF: parse_cmdline+CEj
					; parse_cmdline+D6j
		xor	ebx, ebx

loc_40252C:				; CODE XREF: parse_cmdline+DAj
		xor	ecx, ecx
		cmp	[ebp+var_4], ecx
		setz	cl
		mov	[ebp+var_4], ecx

loc_402537:				; CODE XREF: parse_cmdline+C8j
		shr	edx, 1

loc_402539:				; CODE XREF: parse_cmdline+C3j
		test	edx, edx
		jz	short loc_40254A

loc_40253D:				; CODE XREF: parse_cmdline+FAj
		test	edi, edi
		jz	short loc_402545
		mov	byte ptr [edi],	5Ch
		inc	edi

loc_402545:				; CODE XREF: parse_cmdline+F1j
		inc	dword ptr [esi]
		dec	edx
		jnz	short loc_40253D

loc_40254A:				; CODE XREF: parse_cmdline+EDj
		mov	cl, [eax]
		test	cl, cl
		jz	short loc_402598
		cmp	[ebp+var_4], 0
		jnz	short loc_402560
		cmp	cl, 20h
		jz	short loc_402598
		cmp	cl, 9
		jz	short loc_402598

loc_402560:				; CODE XREF: parse_cmdline+106j
		test	ebx, ebx
		jz	short loc_402592
		test	edi, edi
		jz	short loc_402581
		movzx	edx, cl
		test	byte_40D701[edx], 4
		jz	short loc_40257A
		mov	[edi], cl
		inc	edi
		inc	eax
		inc	dword ptr [esi]

loc_40257A:				; CODE XREF: parse_cmdline+124j
		mov	cl, [eax]
		mov	[edi], cl
		inc	edi
		jmp	short loc_402590
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_402581:				; CODE XREF: parse_cmdline+118j
		movzx	ecx, cl
		test	byte_40D701[ecx], 4
		jz	short loc_402590
		inc	eax
		inc	dword ptr [esi]

loc_402590:				; CODE XREF: parse_cmdline+131j
					; parse_cmdline+13Dj
		inc	dword ptr [esi]

loc_402592:				; CODE XREF: parse_cmdline+114j
		inc	eax
		jmp	loc_402500
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_402598:				; CODE XREF: parse_cmdline+100j
					; parse_cmdline+10Bj ...
		test	edi, edi
		jz	short loc_4025A0
		mov	byte ptr [edi],	0
		inc	edi

loc_4025A0:				; CODE XREF: parse_cmdline+14Cj
		inc	dword ptr [esi]
		mov	ebx, [ebp+arg_4]
		jmp	loc_4024CB
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_4025AA:				; CODE XREF: parse_cmdline+80j
					; parse_cmdline+9Bj
		mov	eax, [ebp+arg_0]
		test	eax, eax
		jz	short loc_4025B4
		and	dword ptr [eax], 0

loc_4025B4:				; CODE XREF: parse_cmdline+161j
		inc	dword ptr [ebx]
		pop	edi
		pop	ebx
		leave
		retn
parse_cmdline	endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ

; Attributes: bp-based frame

_setargv	proc near		; CODE XREF: mainCRTStartup+11Fp

var_8		= dword	ptr -8
var_4		= dword	ptr -4

		push	ebp
		mov	ebp, esp
		push	ecx
		push	ecx
		push	ebx
		push	esi
		push	edi
		xor	edi, edi
		cmp	__mbctype_initialized, edi
		jnz	short loc_4025D1
		call	__initmbctable

loc_4025D1:				; CODE XREF: _setargv+10j
		push	104h
		mov	esi, offset dword_40D490
		push	esi
		push	edi
		mov	byte_40D594, 0
		call	ds:__imp__GetModuleFileNameA@12	; __declspec(dllimport)	GetModuleFileNameA(x,x,x)
		mov	eax, _acmdln
		cmp	eax, edi
		mov	_pgmptr, esi
		jz	short loc_402600
		cmp	byte ptr [eax],	0
		mov	ebx, eax
		jnz	short loc_402602

loc_402600:				; CODE XREF: _setargv+3Dj
		mov	ebx, esi

loc_402602:				; CODE XREF: _setargv+44j
		lea	eax, [ebp+var_4]
		push	eax
		push	edi
		lea	esi, [ebp+var_8]
		xor	ecx, ecx
		mov	eax, ebx
		call	parse_cmdline
		mov	esi, [ebp+var_4]
		mov	eax, [ebp+var_8]
		shl	esi, 2
		add	eax, esi
		push	eax
		call	malloc
		mov	edi, eax
		add	esp, 0Ch
		test	edi, edi
		jnz	short loc_402632
		or	eax, 0FFFFFFFFh
		jmp	short loc_402657
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_402632:				; CODE XREF: _setargv+71j
		lea	eax, [ebp+var_4]
		push	eax
		lea	ecx, [esi+edi]
		push	edi
		lea	esi, [ebp+var_8]
		mov	eax, ebx
		call	parse_cmdline
		mov	eax, [ebp+var_4]
		dec	eax
		pop	ecx
		mov	__argc,	eax
		pop	ecx
		mov	__argv,	edi
		xor	eax, eax

loc_402657:				; CODE XREF: _setargv+76j
		pop	edi
		pop	esi
		pop	ebx
		leave
		retn
_setargv	endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


__crtGetEnvironmentStringsA proc near	; CODE XREF: mainCRTStartup+115p

var_8		= dword	ptr -8
var_4		= dword	ptr -4

		push	ecx
		push	ecx
		mov	eax, dword_40D598
		push	ebx
		push	ebp
		push	esi
		push	edi
		mov	edi, ds:__imp__GetEnvironmentStringsW@0	; __declspec(dllimport)	GetEnvironmentStringsW()
		xor	ebx, ebx
		xor	esi, esi
		cmp	eax, ebx
		push	2
		pop	ebp
		jnz	short loc_4026A5
		call	edi ; __declspec(dllimport) GetEnvironmentStringsW() ; __declspec(dllimport) GetEnvironmentStringsW()
		mov	esi, eax
		cmp	esi, ebx
		jz	short loc_40268C
		mov	dword_40D598, 1
		jmp	short loc_4026AA
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_40268C:				; CODE XREF: __crtGetEnvironmentStringsA+22j
		call	ds:__imp__GetLastError@0 ; __declspec(dllimport) GetLastError()
		cmp	eax, 78h
		jnz	short loc_4026A0
		mov	eax, ebp
		mov	dword_40D598, eax
		jmp	short loc_4026A5
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_4026A0:				; CODE XREF: __crtGetEnvironmentStringsA+39j
		mov	eax, dword_40D598

loc_4026A5:				; CODE XREF: __crtGetEnvironmentStringsA+1Aj
					; __crtGetEnvironmentStringsA+42j
		cmp	eax, 1
		jnz	short loc_402727

loc_4026AA:				; CODE XREF: __crtGetEnvironmentStringsA+2Ej
		cmp	esi, ebx
		jnz	short loc_4026B6
		call	edi ; __declspec(dllimport) GetEnvironmentStringsW() ; __declspec(dllimport) GetEnvironmentStringsW()
		mov	esi, eax
		cmp	esi, ebx
		jz	short loc_40272F

loc_4026B6:				; CODE XREF: __crtGetEnvironmentStringsA+50j
		cmp	[esi], bx
		mov	eax, esi
		jz	short loc_4026CB

loc_4026BD:				; CODE XREF: __crtGetEnvironmentStringsA+66j
					; __crtGetEnvironmentStringsA+6Dj
		add	eax, ebp
		cmp	[eax], bx
		jnz	short loc_4026BD
		add	eax, ebp
		cmp	[eax], bx
		jnz	short loc_4026BD

loc_4026CB:				; CODE XREF: __crtGetEnvironmentStringsA+5Fj
		mov	edi, ds:__imp__WideCharToMultiByte@32 ;	__declspec(dllimport) WideCharToMultiByte(x,x,x,x,x,x,x,x)
		push	ebx
		push	ebx
		push	ebx
		sub	eax, esi
		push	ebx
		sar	eax, 1
		inc	eax
		push	eax
		push	esi
		push	ebx
		push	ebx
		mov	[esp+38h+var_4], eax
		call	edi ; __declspec(dllimport) WideCharToMultiByte(x,x,x,x,x,x,x,x) ; __declspec(dllimport) WideCharToMultiByte(x,x,x,x,x,x,x,x)
		mov	ebp, eax
		cmp	ebp, ebx
		jz	short loc_40271C
		push	ebp
		call	malloc
		cmp	eax, ebx
		pop	ecx
		mov	[esp+18h+var_8], eax
		jz	short loc_40271C
		push	ebx
		push	ebx
		push	ebp
		push	eax
		push	[esp+28h+var_4]
		push	esi
		push	ebx
		push	ebx
		call	edi ; __declspec(dllimport) WideCharToMultiByte(x,x,x,x,x,x,x,x) ; __declspec(dllimport) WideCharToMultiByte(x,x,x,x,x,x,x,x)
		test	eax, eax
		jnz	short loc_402718
		push	[esp+18h+var_8]
		call	free
		pop	ecx
		mov	[esp+18h+var_8], ebx

loc_402718:				; CODE XREF: __crtGetEnvironmentStringsA+ACj
		mov	ebx, [esp+18h+var_8]

loc_40271C:				; CODE XREF: __crtGetEnvironmentStringsA+8Cj
					; __crtGetEnvironmentStringsA+9Bj
		push	esi
		call	ds:__imp__FreeEnvironmentStringsW@4 ; __declspec(dllimport) FreeEnvironmentStringsW(x)
		mov	eax, ebx
		jmp	short loc_402777
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_402727:				; CODE XREF: __crtGetEnvironmentStringsA+4Cj
		cmp	eax, ebp
		jz	short loc_402733
		cmp	eax, ebx
		jz	short loc_402733

loc_40272F:				; CODE XREF: __crtGetEnvironmentStringsA+58j
					; __crtGetEnvironmentStringsA+E1j
		xor	eax, eax
		jmp	short loc_402777
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_402733:				; CODE XREF: __crtGetEnvironmentStringsA+CDj
					; __crtGetEnvironmentStringsA+D1j
		call	ds:__imp__GetEnvironmentStrings@0 ; __declspec(dllimport) GetEnvironmentStrings()
		mov	esi, eax
		cmp	esi, ebx
		jz	short loc_40272F
		cmp	[esi], bl
		jz	short loc_40274D

loc_402743:				; CODE XREF: __crtGetEnvironmentStringsA+EAj
					; __crtGetEnvironmentStringsA+EFj
		inc	eax
		cmp	[eax], bl
		jnz	short loc_402743
		inc	eax
		cmp	[eax], bl
		jnz	short loc_402743

loc_40274D:				; CODE XREF: __crtGetEnvironmentStringsA+E5j
		sub	eax, esi
		inc	eax
		mov	ebp, eax
		push	ebp
		call	malloc
		mov	edi, eax
		cmp	edi, ebx
		pop	ecx
		jnz	short loc_402763
		xor	edi, edi
		jmp	short loc_40276E
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_402763:				; CODE XREF: __crtGetEnvironmentStringsA+101j
		push	ebp
		push	esi
		push	edi
		call	memcpy
		add	esp, 0Ch

loc_40276E:				; CODE XREF: __crtGetEnvironmentStringsA+105j
		push	esi
		call	ds:__imp__FreeEnvironmentStringsA@4 ; __declspec(dllimport) FreeEnvironmentStringsA(x)
		mov	eax, edi

loc_402777:				; CODE XREF: __crtGetEnvironmentStringsA+C9j
					; __crtGetEnvironmentStringsA+D5j
		pop	edi
		pop	esi
		pop	ebp
		pop	ebx
		pop	ecx
		pop	ecx
		retn
__crtGetEnvironmentStringsA endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


_ioinit		proc near		; CODE XREF: mainCRTStartup+F9p

var_44		= dword	ptr -44h
var_12		= word ptr -12h
var_10		= dword	ptr -10h

		sub	esp, 44h
		push	100h
		call	malloc
		test	eax, eax
		pop	ecx
		jnz	short loc_402798
		or	eax, 0FFFFFFFFh
		jmp	loc_402925
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_402798:				; CODE XREF: _ioinit+10j
		mov	__pioinfo, eax
		mov	_nhandle, 20h
		lea	ecx, [eax+100h]
		jmp	short loc_4027C9
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_4027AF:				; CODE XREF: _ioinit+4Dj
		or	dword ptr [eax], 0FFFFFFFFh
		mov	byte ptr [eax+4], 0
		mov	byte ptr [eax+5], 0Ah
		mov	ecx, __pioinfo
		add	eax, 8
		add	ecx, 100h

loc_4027C9:				; CODE XREF: _ioinit+2Fj
		cmp	eax, ecx
		jb	short loc_4027AF
		push	ebx
		push	esi
		push	edi
		lea	eax, [esp+50h+var_44]
		push	eax
		call	ds:__imp__GetStartupInfoA@4 ; __declspec(dllimport) GetStartupInfoA(x)
		cmp	[esp+50h+var_12], 0
		jz	loc_4028AE
		mov	eax, [esp+50h+var_10]
		test	eax, eax
		jz	loc_4028AE
		mov	esi, [eax]
		push	ebp
		lea	ebp, [eax+4]
		mov	eax, 800h
		cmp	esi, eax
		lea	ebx, [esi+ebp]
		jl	short loc_402807
		mov	esi, eax

loc_402807:				; CODE XREF: _ioinit+85j
		cmp	_nhandle, esi
		jge	short loc_402861
		mov	edi, offset dword_40D984

loc_402814:				; CODE XREF: _ioinit+D9j
		push	100h
		call	malloc
		test	eax, eax
		pop	ecx
		jz	short loc_40285B
		add	_nhandle, 20h
		mov	[edi], eax
		lea	ecx, [eax+100h]
		jmp	short loc_40284A
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_402834:				; CODE XREF: _ioinit+CEj
		or	dword ptr [eax], 0FFFFFFFFh
		mov	byte ptr [eax+4], 0
		mov	byte ptr [eax+5], 0Ah
		mov	ecx, [edi]
		add	eax, 8
		add	ecx, 100h

loc_40284A:				; CODE XREF: _ioinit+B4j
		cmp	eax, ecx
		jb	short loc_402834
		add	edi, 4
		cmp	_nhandle, esi
		jl	short loc_402814
		jmp	short loc_402861
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_40285B:				; CODE XREF: _ioinit+A3j
		mov	esi, _nhandle

loc_402861:				; CODE XREF: _ioinit+8Fj _ioinit+DBj
		xor	edi, edi
		test	esi, esi
		jle	short loc_4028AD

loc_402867:				; CODE XREF: _ioinit+12Dj
		mov	eax, [ebx]
		cmp	eax, 0FFFFFFFFh
		jz	short loc_4028A4
		mov	cl, [ebp+0]
		test	cl, 1
		jz	short loc_4028A4
		test	cl, 8
		jnz	short loc_402886
		push	eax
		call	ds:__imp__GetFileType@4	; __declspec(dllimport)	GetFileType(x)
		test	eax, eax
		jz	short loc_4028A4

loc_402886:				; CODE XREF: _ioinit+FBj
		mov	eax, edi
		sar	eax, 5
		mov	eax, __pioinfo[eax*4]
		mov	ecx, edi
		and	ecx, 1Fh
		lea	eax, [eax+ecx*8]
		mov	ecx, [ebx]
		mov	[eax], ecx
		mov	cl, [ebp+0]
		mov	[eax+4], cl

loc_4028A4:				; CODE XREF: _ioinit+EEj _ioinit+F6j ...
		inc	edi
		inc	ebp
		add	ebx, 4
		cmp	edi, esi
		jl	short loc_402867

loc_4028AD:				; CODE XREF: _ioinit+E7j
		pop	ebp

loc_4028AE:				; CODE XREF: _ioinit+63j _ioinit+6Fj
		xor	ebx, ebx

loc_4028B0:				; CODE XREF: _ioinit+194j
		mov	eax, __pioinfo
		lea	esi, [eax+ebx*8]
		cmp	dword ptr [esi], 0FFFFFFFFh
		jnz	short loc_40290A
		test	ebx, ebx
		mov	byte ptr [esi+4], 81h
		jnz	short loc_4028CA
		push	0FFFFFFF6h
		pop	eax
		jmp	short loc_4028D4
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_4028CA:				; CODE XREF: _ioinit+145j
		mov	eax, ebx
		dec	eax
		neg	eax
		sbb	eax, eax
		add	eax, 0FFFFFFF5h

loc_4028D4:				; CODE XREF: _ioinit+14Aj
		push	eax
		call	ds:__imp__GetStdHandle@4 ; __declspec(dllimport) GetStdHandle(x)
		mov	edi, eax
		cmp	edi, 0FFFFFFFFh
		jz	short loc_4028F9
		push	edi
		call	ds:__imp__GetFileType@4	; __declspec(dllimport)	GetFileType(x)
		test	eax, eax
		jz	short loc_4028F9
		and	eax, 0FFh
		cmp	eax, 2
		mov	[esi], edi
		jnz	short loc_4028FF

loc_4028F9:				; CODE XREF: _ioinit+162j _ioinit+16Dj
		or	byte ptr [esi+4], 40h
		jmp	short loc_40290E
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_4028FF:				; CODE XREF: _ioinit+179j
		cmp	eax, 3
		jnz	short loc_40290E
		or	byte ptr [esi+4], 8
		jmp	short loc_40290E
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_40290A:				; CODE XREF: _ioinit+13Dj
		or	byte ptr [esi+4], 80h

loc_40290E:				; CODE XREF: _ioinit+17Fj _ioinit+184j ...
		inc	ebx
		cmp	ebx, 3
		jl	short loc_4028B0
		push	_nhandle
		call	ds:__imp__SetHandleCount@4 ; __declspec(dllimport) SetHandleCount(x)
		pop	edi
		pop	esi
		xor	eax, eax
		pop	ebx

loc_402925:				; CODE XREF: _ioinit+15j
		add	esp, 44h
		retn
_ioinit		endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


_ioterm		proc near
		push	esi
		mov	esi, offset __pioinfo

loc_40292F:				; CODE XREF: _ioterm+1Fj
		mov	eax, [esi]
		test	eax, eax
		jz	short loc_40293F
		push	eax
		call	free
		and	dword ptr [esi], 0
		pop	ecx

loc_40293F:				; CODE XREF: _ioterm+Aj
		add	esi, 4
		cmp	esi, offset __env_initialized
		jl	short loc_40292F
		pop	esi
		retn
_ioterm		endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


_RTC_Initialize	proc near		; CODE XREF: mainCRTStartup:loc_401318p
		push	0Ch
		push	offset dword_40A4D0
		call	__SEH_prolog
		mov	dword ptr [ebp-1Ch], offset dword_40B290

loc_40295F:				; CODE XREF: _RTC_Initialize+3Cj
		cmp	dword ptr [ebp-1Ch], offset __rtc_izz
		jnb	short loc_40298A
		and	dword ptr [ebp-4], 0
		mov	eax, [ebp-1Ch]
		mov	eax, [eax]
		test	eax, eax
		jz	short loc_402980
		call	eax
		jmp	short loc_402980
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		xor	eax, eax
		inc	eax
		retn
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		mov	esp, [ebp-18h]

loc_402980:				; CODE XREF: _RTC_Initialize+27j
					; _RTC_Initialize+2Bj
		or	dword ptr [ebp-4], 0FFFFFFFFh
		add	dword ptr [ebp-1Ch], 4
		jmp	short loc_40295F
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_40298A:				; CODE XREF: _RTC_Initialize+1Aj
		call	__SEH_epilog
		retn
_RTC_Initialize	endp ; sp = -8


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


_RTC_Terminate	proc near		; DATA XREF: _cinit:loc_401EBAo
		push	0Ch
		push	offset dword_40A4E0
		call	__SEH_prolog
		mov	dword ptr [ebp-1Ch], offset dword_40B498

loc_4029A3:				; CODE XREF: _RTC_Terminate+3Cj
		cmp	dword ptr [ebp-1Ch], offset __rtc_tzz
		jnb	short loc_4029CE
		and	dword ptr [ebp-4], 0
		mov	eax, [ebp-1Ch]
		mov	eax, [eax]
		test	eax, eax
		jz	short loc_4029C4
		call	eax
		jmp	short loc_4029C4
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		xor	eax, eax
		inc	eax
		retn
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		mov	esp, [ebp-18h]

loc_4029C4:				; CODE XREF: _RTC_Terminate+27j
					; _RTC_Terminate+2Bj
		or	dword ptr [ebp-4], 0FFFFFFFFh
		add	dword ptr [ebp-1Ch], 4
		jmp	short loc_4029A3
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_4029CE:				; CODE XREF: _RTC_Terminate+1Aj
		call	__SEH_epilog
		retn
_RTC_Terminate	endp ; sp = -8


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


__heap_select	proc near		; CODE XREF: _heap_init+20p
		cmp	_osplatform, 2
		jnz	short loc_4029EA
		cmp	_winmajor, 5
		jb	short loc_4029EA
		xor	eax, eax
		inc	eax
		retn
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_4029EA:				; CODE XREF: __heap_select+7j
					; __heap_select+10j
		push	3
		pop	eax
		retn
__heap_select	endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


_heap_init	proc near		; CODE XREF: mainCRTStartup+C6p

arg_0		= dword	ptr  4

		xor	eax, eax
		cmp	[esp+arg_0], eax
		push	0
		setz	al
		push	1000h
		push	eax
		call	ds:__imp__HeapCreate@12	; __declspec(dllimport)	HeapCreate(x,x,x)
		test	eax, eax
		mov	_crtheap, eax
		jz	short loc_402A38
		call	__heap_select
		cmp	eax, 3
		mov	__active_heap, eax
		jnz	short loc_402A3B
		push	3F8h
		call	__sbh_heap_init
		test	eax, eax
		pop	ecx
		jnz	short loc_402A3B
		push	_crtheap
		call	ds:__imp__HeapDestroy@4	; __declspec(dllimport)	HeapDestroy(x)

loc_402A38:				; CODE XREF: _heap_init+1Ej
		xor	eax, eax
		retn
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_402A3B:				; CODE XREF: _heap_init+2Dj
					; _heap_init+3Cj
		xor	eax, eax
		inc	eax
		retn
_heap_init	endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


_heap_term	proc near
		cmp	__active_heap, 3
		jnz	short loc_402AB1
		push	ebx
		xor	ebx, ebx
		cmp	__sbh_cntHeaderList, ebx
		push	ebp
		mov	ebp, ds:__imp__HeapFree@12 ; __declspec(dllimport) HeapFree(x,x,x)
		jle	short loc_402A9F
		push	esi
		mov	esi, __sbh_pHeaderList
		push	edi
		mov	edi, ds:__imp__VirtualFree@12 ;	__declspec(dllimport) VirtualFree(x,x,x)
		add	esi, 0Ch

loc_402A6B:				; CODE XREF: _heap_term+5Cj
		push	4000h
		push	100000h
		push	dword ptr [esi]
		call	edi ; __declspec(dllimport) VirtualFree(x,x,x) ; __declspec(dllimport) VirtualFree(x,x,x)
		push	8000h
		push	0
		push	dword ptr [esi]
		call	edi ; __declspec(dllimport) VirtualFree(x,x,x) ; __declspec(dllimport) VirtualFree(x,x,x)
		push	dword ptr [esi+4]
		push	0
		push	_crtheap
		call	ebp ; __declspec(dllimport) HeapFree(x,x,x) ; __declspec(dllimport) HeapFree(x,x,x)
		add	esi, 14h
		inc	ebx
		cmp	ebx, __sbh_cntHeaderList
		jl	short loc_402A6B
		pop	edi
		pop	esi

loc_402A9F:				; CODE XREF: _heap_term+19j
		push	__sbh_pHeaderList
		push	0
		push	_crtheap
		call	ebp ; __declspec(dllimport) HeapFree(x,x,x) ; __declspec(dllimport) HeapFree(x,x,x)
		pop	ebp
		pop	ebx

loc_402AB1:				; CODE XREF: _heap_term+7j
		push	_crtheap
		call	ds:__imp__HeapDestroy@4	; __declspec(dllimport)	HeapDestroy(x)
		retn
_heap_term	endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


_get_heap_handle proc near
		mov	eax, _crtheap
		retn
_get_heap_handle endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


__SEH_prolog	proc near		; CODE XREF: mainCRTStartup+7p
					; _RTC_Initialize+7p ...

arg_4		= dword	ptr  8

		push	offset _except_handler3
		mov	eax, large fs:0
		push	eax
		mov	eax, [esp+8+arg_4]
		mov	[esp+8+arg_4], ebp
		lea	ebp, [esp+8+arg_4]
		sub	esp, eax
		push	ebx
		push	esi
		push	edi
		mov	eax, [ebp-8]
		mov	[ebp-18h], esp
		push	eax
		mov	eax, [ebp-4]
		mov	dword ptr [ebp-4], 0FFFFFFFFh
		mov	[ebp-8], eax
		lea	eax, [ebp-10h]
		mov	large fs:0, eax
		retn
__SEH_prolog	endp ; sp = -18h


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


__SEH_epilog	proc near		; CODE XREF: mainCRTStartup+1C1p
					; _RTC_Initialize:loc_40298Ap ...
		mov	ecx, [ebp-10h]
		mov	large fs:0, ecx
		pop	ecx
		pop	edi
		pop	esi
		pop	ebx
		leave
		push	ecx
		retn
__SEH_epilog	endp

; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		push	esi
		inc	ebx
		xor	dh, [eax]
		pop	eax
		inc	ebx
		xor	[eax], dh

; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ

; Attributes: bp-based frame

_except_handler3 proc near		; DATA XREF: __SEH_prologo

var_8		= dword	ptr -8
var_4		= dword	ptr -4
arg_0		= dword	ptr  8
arg_4		= dword	ptr  0Ch
arg_8		= dword	ptr  10h

		push	ebp
		mov	ebp, esp
		sub	esp, 8
		push	ebx
		push	esi
		push	edi
		push	ebp
		cld
		mov	ebx, [ebp+arg_4]
		mov	eax, [ebp+arg_0]
		test	dword ptr [eax+4], 6
		jnz	loc_402BE1
		mov	[ebp+var_8], eax
		mov	eax, [ebp+arg_8]
		mov	[ebp+var_4], eax
		lea	eax, [ebp+var_8]
		mov	[ebx-4], eax
		mov	esi, [ebx+0Ch]
		mov	edi, [ebx+8]
		push	ebx
		call	_ValidateEH3RN
		add	esp, 4
		or	eax, eax
		jz	short loc_402BD3

loc_402B58:				; CODE XREF: _except_handler3+B2j
		cmp	esi, 0FFFFFFFFh
		jz	short loc_402BDA
		lea	ecx, [esi+esi*2]
		mov	eax, [edi+ecx*4+4]
		or	eax, eax
		jz	short loc_402BC1
		push	esi
		push	ebp
		lea	ebp, [ebx+10h]
		xor	ebx, ebx
		xor	ecx, ecx
		xor	edx, edx
		xor	esi, esi
		xor	edi, edi
		call	eax
		pop	ebp
		pop	esi
		mov	ebx, [ebp+arg_4]
		or	eax, eax
		jz	short loc_402BC1
		js	short loc_402BCC
		mov	edi, [ebx+8]
		push	ebx
		call	__global_unwind2
		add	esp, 4
		lea	ebp, [ebx+10h]
		push	esi
		push	ebx
		call	__local_unwind2
		add	esp, 8
		lea	ecx, [esi+esi*2]
		push	1
		mov	eax, [edi+ecx*4+8]
		call	_NLG_Notify
		mov	eax, [edi+ecx*4]
		mov	[ebx+0Ch], eax
		mov	eax, [edi+ecx*4+8]
		xor	ebx, ebx
		xor	ecx, ecx
		xor	edx, edx
		xor	esi, esi
		xor	edi, edi
		call	eax

loc_402BC1:				; CODE XREF: _except_handler3+4Ej
					; _except_handler3+68j
		mov	edi, [ebx+8]
		lea	ecx, [esi+esi*2]
		mov	esi, [edi+ecx*4]
		jmp	short loc_402B58
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_402BCC:				; CODE XREF: _except_handler3+6Aj
		mov	eax, 0
		jmp	short loc_402BF6
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_402BD3:				; CODE XREF: _except_handler3+3Ej
		mov	eax, [ebp+arg_0]
		or	dword ptr [eax+4], 8

loc_402BDA:				; CODE XREF: _except_handler3+43j
		mov	eax, 1
		jmp	short loc_402BF6
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_402BE1:				; CODE XREF: _except_handler3+18j
		push	ebp
		lea	ebp, [ebx+10h]
		push	0FFFFFFFFh
		push	ebx
		call	__local_unwind2
		add	esp, 8
		pop	ebp
		mov	eax, 1

loc_402BF6:				; CODE XREF: _except_handler3+B9j
					; _except_handler3+C7j
		pop	ebp
		pop	edi
		pop	esi
		pop	ebx
		mov	esp, ebp
		pop	ebp
		retn
_except_handler3 endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


; __stdcall _seh_longjmp_unwind(x)
__seh_longjmp_unwind@4 proc near

arg_0		= dword	ptr  8

		push	ebp
		mov	ecx, [esp+arg_0]
		mov	ebp, [ecx]
		mov	eax, [ecx+1Ch]
		push	eax
		mov	eax, [ecx+18h]
		push	eax
		call	__local_unwind2
		add	esp, 8
		pop	ebp
		retn	4
__seh_longjmp_unwind@4 endp

; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		align 10h

; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


_chkstk		proc near		; CODE XREF: mainCRTStartup+13p
					; _NMSG_WRITE+EEp ...

arg_0		= dword	ptr  4

		cmp	eax, 1000h
		jnb	short loc_402C35
		neg	eax
		add	eax, esp
		add	eax, 4
		test	[eax], eax
		xchg	eax, esp
		mov	eax, [eax]
		push	eax
		retn
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_402C35:				; CODE XREF: _chkstk+5j
		push	ecx
		lea	ecx, [esp+4+arg_0]

loc_402C3A:				; CODE XREF: _chkstk+2Cj
		sub	ecx, 1000h
		sub	eax, 1000h
		test	[ecx], eax
		cmp	eax, 1000h
		jnb	short loc_402C3A
		sub	ecx, eax
		mov	eax, esp
		test	[ecx], eax
		mov	esp, ecx
		mov	ecx, [eax]
		mov	eax, [eax+4]
		push	eax
		retn
_chkstk		endp ; sp = -8


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


_heap_alloc	proc near		; CODE XREF: _nh_malloc+Bp

arg_0		= dword	ptr  4

		cmp	__active_heap, 3
		push	esi
		mov	esi, [esp+4+arg_0]
		jnz	short loc_402C7E
		cmp	esi, __sbh_threshold
		ja	short loc_402C7E
		push	esi
		call	__sbh_alloc_block
		test	eax, eax
		pop	ecx
		jnz	short loc_402CA1

loc_402C7E:				; CODE XREF: _heap_alloc+Cj
					; _heap_alloc+14j
		test	esi, esi
		jnz	short loc_402C83
		inc	esi

loc_402C83:				; CODE XREF: _heap_alloc+23j
		cmp	__active_heap, 1
		jz	short loc_402C92
		add	esi, 0Fh
		and	esi, 0FFFFFFF0h

loc_402C92:				; CODE XREF: _heap_alloc+2Dj
		push	esi
		push	0
		push	_crtheap
		call	ds:__imp__HeapAlloc@12 ; __declspec(dllimport) HeapAlloc(x,x,x)

loc_402CA1:				; CODE XREF: _heap_alloc+1Fj
		pop	esi
		retn
_heap_alloc	endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


_nh_malloc	proc near		; CODE XREF: malloc+Ap

arg_0		= dword	ptr  4
arg_4		= dword	ptr  8

		cmp	[esp+arg_0], 0FFFFFFE0h
		ja	short loc_402CCC

loc_402CAA:				; CODE XREF: _nh_malloc+27j
		push	[esp+arg_0]
		call	_heap_alloc
		test	eax, eax
		pop	ecx
		jnz	short locret_402CCE
		cmp	[esp+arg_4], eax
		jz	short locret_402CCE
		push	[esp+arg_0]
		call	_callnewh
		test	eax, eax
		pop	ecx
		jnz	short loc_402CAA

loc_402CCC:				; CODE XREF: _nh_malloc+5j
		xor	eax, eax

locret_402CCE:				; CODE XREF: _nh_malloc+13j
					; _nh_malloc+19j
		retn
_nh_malloc	endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


malloc		proc near		; CODE XREF: _stbuf+4Bp _output+436p ...

arg_0		= dword	ptr  4

		push	_newmode
		push	[esp+4+arg_0]
		call	_nh_malloc
		pop	ecx
		pop	ecx
		retn
malloc		endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


_isatty		proc near		; CODE XREF: _stbuf+8p	_flsbuf+64p

arg_0		= dword	ptr  4

		mov	eax, [esp+arg_0]
		cmp	eax, _nhandle
		jb	short loc_402CF0
		xor	eax, eax
		retn
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_402CF0:				; CODE XREF: _isatty+Aj
		mov	ecx, eax
		sar	ecx, 5
		mov	ecx, __pioinfo[ecx*4]
		and	eax, 1Fh
		movsx	eax, byte ptr [ecx+eax*8+4]
		and	eax, 40h
		retn
_isatty		endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


_flush		proc near		; CODE XREF: _ftbuf+13p _ftbuf+34p ...

arg_0		= dword	ptr  0Ch

		push	ebx
		push	esi
		mov	esi, [esp+arg_0]
		mov	eax, [esi+0Ch]
		mov	ecx, eax
		and	cl, 3
		xor	ebx, ebx
		cmp	cl, 2
		jnz	short loc_402D57
		test	ax, 108h
		jz	short loc_402D57
		mov	eax, [esi+8]
		push	edi
		mov	edi, [esi]
		sub	edi, eax
		test	edi, edi
		jle	short loc_402D56
		push	edi
		push	eax
		push	dword ptr [esi+10h]
		call	_write
		add	esp, 0Ch
		cmp	eax, edi
		jnz	short loc_402D4F
		mov	eax, [esi+0Ch]
		test	al, al
		jns	short loc_402D56
		and	eax, 0FFFFFFFDh
		mov	[esi+0Ch], eax
		jmp	short loc_402D56
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_402D4F:				; CODE XREF: _flush+36j
		or	dword ptr [esi+0Ch], 20h
		or	ebx, 0FFFFFFFFh

loc_402D56:				; CODE XREF: _flush+25j _flush+3Dj ...
		pop	edi

loc_402D57:				; CODE XREF: _flush+13j _flush+19j
		mov	eax, [esi+8]
		and	dword ptr [esi+4], 0
		mov	[esi], eax
		pop	esi
		mov	eax, ebx
		pop	ebx
		retn
_flush		endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


fflush		proc near		; CODE XREF: flsall+2Dp flsall+48p

arg_0		= dword	ptr  8

		push	esi
		mov	esi, [esp+arg_0]
		test	esi, esi
		jnz	short loc_402D77
		push	esi
		call	flsall
		pop	ecx
		pop	esi
		retn
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_402D77:				; CODE XREF: fflush+7j
		push	esi
		call	_flush
		test	eax, eax
		pop	ecx
		jz	short loc_402D87
		or	eax, 0FFFFFFFFh
		pop	esi
		retn
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_402D87:				; CODE XREF: fflush+1Bj
		test	byte ptr [esi+0Dh], 40h
		jz	short loc_402D9C
		push	dword ptr [esi+10h]
		call	_commit
		pop	ecx
		neg	eax
		sbb	eax, eax
		pop	esi
		retn
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_402D9C:				; CODE XREF: fflush+26j
		xor	eax, eax
		pop	esi
		retn
fflush		endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


flsall		proc near		; CODE XREF: fflush+Ap	_flushall+2p

arg_0		= dword	ptr  10h

		push	ebx
		push	esi
		push	edi
		xor	esi, esi
		xor	ebx, ebx
		xor	edi, edi
		cmp	_nstream, esi
		jle	short loc_402DFE

loc_402DB1:				; CODE XREF: flsall+5Cj
		mov	eax, __piob
		mov	eax, [eax+esi*4]
		test	eax, eax
		jz	short loc_402DF5
		mov	ecx, [eax+0Ch]
		test	cl, 83h
		jz	short loc_402DF5
		cmp	[esp+arg_0], 1
		jnz	short loc_402DDB
		push	eax
		call	fflush
		cmp	eax, 0FFFFFFFFh
		pop	ecx
		jz	short loc_402DF5
		inc	ebx
		jmp	short loc_402DF5
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_402DDB:				; CODE XREF: flsall+2Aj
		cmp	[esp+arg_0], 0
		jnz	short loc_402DF5
		test	cl, 2
		jz	short loc_402DF5
		push	eax
		call	fflush
		cmp	eax, 0FFFFFFFFh
		pop	ecx
		jnz	short loc_402DF5
		or	edi, eax

loc_402DF5:				; CODE XREF: flsall+1Bj flsall+23j ...
		inc	esi
		cmp	esi, _nstream
		jl	short loc_402DB1

loc_402DFE:				; CODE XREF: flsall+Fj
		cmp	[esp+arg_0], 1
		mov	eax, ebx
		jz	short loc_402E09
		mov	eax, edi

loc_402E09:				; CODE XREF: flsall+65j
		pop	edi
		pop	esi
		pop	ebx
		retn
flsall		endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


_flushall	proc near		; CODE XREF: __endstdiop
		push	1
		call	flsall
		pop	ecx
		retn
_flushall	endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ

; Attributes: bp-based frame

_flsbuf		proc near		; CODE XREF: write_char+21p

arg_0		= dword	ptr  8
arg_4		= dword	ptr  0Ch

		push	ebp
		mov	ebp, esp
		push	ebx
		push	esi
		mov	esi, [ebp+arg_4]
		mov	eax, [esi+0Ch]
		test	al, 82h
		mov	ebx, [esi+10h]
		jz	loc_402F1F
		test	al, 40h
		jnz	loc_402F1F
		test	al, 1
		jz	short loc_402E4F
		and	dword ptr [esi+4], 0
		test	al, 10h
		jz	loc_402F1F
		mov	ecx, [esi+8]
		and	eax, 0FFFFFFFEh
		mov	[esi], ecx
		mov	[esi+0Ch], eax

loc_402E4F:				; CODE XREF: _flsbuf+20j
		mov	eax, [esi+0Ch]
		and	dword ptr [esi+4], 0
		and	[ebp+arg_4], 0
		and	eax, 0FFFFFFEFh
		or	eax, 2
		test	ax, 10Ch
		mov	[esi+0Ch], eax
		jnz	short loc_402E8B
		cmp	esi, offset dword_40CB80
		jz	short loc_402E79
		cmp	esi, offset dword_40CBA0
		jnz	short loc_402E84

loc_402E79:				; CODE XREF: _flsbuf+59j
		push	ebx
		call	_isatty
		test	eax, eax
		pop	ecx
		jnz	short loc_402E8B

loc_402E84:				; CODE XREF: _flsbuf+61j
		push	esi
		call	_getbuf
		pop	ecx

loc_402E8B:				; CODE XREF: _flsbuf+51j _flsbuf+6Cj
		test	word ptr [esi+0Ch], 108h
		push	edi
		jz	short loc_402EF5
		mov	eax, [esi+8]
		mov	edi, [esi]
		lea	ecx, [eax+1]
		mov	[esi], ecx
		mov	ecx, [esi+18h]
		sub	edi, eax
		dec	ecx
		test	edi, edi
		mov	[esi+4], ecx
		jle	short loc_402EB8
		push	edi
		push	eax
		push	ebx
		call	_write
		mov	[ebp+arg_4], eax
		jmp	short loc_402EE8
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_402EB8:				; CODE XREF: _flsbuf+93j
		cmp	ebx, 0FFFFFFFFh
		jz	short loc_402ED3
		mov	eax, ebx
		sar	eax, 5
		mov	eax, __pioinfo[eax*4]
		mov	ecx, ebx
		and	ecx, 1Fh
		lea	eax, [eax+ecx*8]
		jmp	short loc_402ED8
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_402ED3:				; CODE XREF: _flsbuf+A5j
		mov	eax, offset __badioinfo

loc_402ED8:				; CODE XREF: _flsbuf+BBj
		test	byte ptr [eax+4], 20h
		jz	short loc_402EEB
		push	2
		push	0
		push	ebx
		call	_lseek

loc_402EE8:				; CODE XREF: _flsbuf+A0j
		add	esp, 0Ch

loc_402EEB:				; CODE XREF: _flsbuf+C6j
		mov	eax, [esi+8]
		mov	cl, byte ptr [ebp+arg_0]
		mov	[eax], cl
		jmp	short loc_402F09
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_402EF5:				; CODE XREF: _flsbuf+7Cj
		xor	edi, edi
		inc	edi
		push	edi
		lea	eax, [ebp+arg_0]
		push	eax
		push	ebx
		call	_write
		add	esp, 0Ch
		mov	[ebp+arg_4], eax

loc_402F09:				; CODE XREF: _flsbuf+DDj
		cmp	[ebp+arg_4], edi
		pop	edi
		jz	short loc_402F15
		or	dword ptr [esi+0Ch], 20h
		jmp	short loc_402F25
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_402F15:				; CODE XREF: _flsbuf+F7j
		mov	eax, [ebp+arg_0]
		and	eax, 0FFh
		jmp	short loc_402F28
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_402F1F:				; CODE XREF: _flsbuf+10j _flsbuf+18j ...
		or	eax, 20h
		mov	[esi+0Ch], eax

loc_402F25:				; CODE XREF: _flsbuf+FDj
		or	eax, 0FFFFFFFFh

loc_402F28:				; CODE XREF: _flsbuf+107j
		pop	esi
		pop	ebx
		pop	ebp
		retn
_flsbuf		endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


free		proc near		; CODE XREF: _output+7A7p _setenvp+92p ...

arg_0		= dword	ptr  8

		push	esi
		mov	esi, [esp+arg_0]
		test	esi, esi
		jz	short loc_402F62
		cmp	__active_heap, 3
		push	esi
		jnz	short loc_402F54
		call	__sbh_find_block
		test	eax, eax
		pop	ecx
		push	esi
		jz	short loc_402F54
		push	eax
		call	__sbh_free_block
		pop	ecx
		pop	ecx
		pop	esi
		retn
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_402F54:				; CODE XREF: free+11j free+1Cj
		push	0
		push	_crtheap
		call	ds:__imp__HeapFree@12 ;	__declspec(dllimport) HeapFree(x,x,x)

loc_402F62:				; CODE XREF: free+7j
		pop	esi
		retn
free		endp

; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		align 10h

; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


strlen		proc near		; CODE XREF: _output:loc_401A16p
					; _NMSG_WRITE+A1p ...

arg_0		= dword	ptr  4

		mov	ecx, [esp+arg_0]
		test	ecx, 3
		jz	short loc_402FA0

loc_402F7C:				; CODE XREF: strlen+1Bj
		mov	al, [ecx]
		add	ecx, 1
		test	al, al
		jz	short loc_402FD3
		test	ecx, 3
		jnz	short loc_402F7C
		add	eax, 0
		lea	esp, [esp+0]
		lea	esp, [esp+0]

loc_402FA0:				; CODE XREF: strlen+Aj	strlen+46j ...
		mov	eax, [ecx]
		mov	edx, 7EFEFEFFh
		add	edx, eax
		xor	eax, 0FFFFFFFFh
		xor	eax, edx
		add	ecx, 4
		test	eax, 81010100h
		jz	short loc_402FA0
		mov	eax, [ecx-4]
		test	al, al
		jz	short loc_402FF1
		test	ah, ah
		jz	short loc_402FE7
		test	eax, 0FF0000h
		jz	short loc_402FDD
		test	eax, 0FF000000h
		jz	short loc_402FD3
		jmp	short loc_402FA0
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_402FD3:				; CODE XREF: strlen+13j strlen+5Fj
		lea	eax, [ecx-1]
		mov	ecx, [esp+arg_0]
		sub	eax, ecx
		retn
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_402FDD:				; CODE XREF: strlen+58j
		lea	eax, [ecx-2]
		mov	ecx, [esp+arg_0]
		sub	eax, ecx
		retn
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_402FE7:				; CODE XREF: strlen+51j
		lea	eax, [ecx-3]
		mov	ecx, [esp+arg_0]
		sub	eax, ecx
		retn
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_402FF1:				; CODE XREF: strlen+4Dj
		lea	eax, [ecx-4]
		mov	ecx, [esp+arg_0]
		sub	eax, ecx
		retn
strlen		endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ

; Attributes: bp-based frame

wctomb		proc near		; CODE XREF: _output+343p _output+74Ap

arg_0		= dword	ptr  8
arg_4		= word ptr  0Ch

		push	ebp
		mov	ebp, esp
		mov	eax, [ebp+arg_0]
		push	esi
		xor	esi, esi
		cmp	eax, esi
		jnz	short loc_40300C
		xor	eax, eax
		jmp	short loc_40305E
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_40300C:				; CODE XREF: wctomb+Bj
		cmp	dword_40D614, esi
		jnz	short loc_403026
		mov	cx, [ebp+arg_4]
		cmp	cx, 0FFh
		ja	short loc_403051
		mov	[eax], cl
		xor	eax, eax
		inc	eax
		jmp	short loc_40305E
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_403026:				; CODE XREF: wctomb+17j
		lea	ecx, [ebp+arg_0]
		push	ecx
		push	esi
		push	__mb_cur_max
		mov	[ebp+arg_0], esi
		push	eax
		push	1
		lea	eax, [ebp+arg_4]
		push	eax
		push	esi
		push	__lc_codepage
		call	ds:__imp__WideCharToMultiByte@32 ; __declspec(dllimport) WideCharToMultiByte(x,x,x,x,x,x,x,x)
		cmp	eax, esi
		jz	short loc_403051
		cmp	[ebp+arg_0], esi
		jz	short loc_40305E

loc_403051:				; CODE XREF: wctomb+22j wctomb+4Fj
		mov	errno, 2Ah
		or	eax, 0FFFFFFFFh

loc_40305E:				; CODE XREF: wctomb+Fj	wctomb+29j ...
		pop	esi
		pop	ebp
		retn
wctomb		endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


__pwctype_func	proc near
		mov	eax, _pwctype
		retn
__pwctype_func	endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


__pctype_func	proc near
		mov	eax, _pctype
		retn
__pctype_func	endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


report_failure	proc near		; CODE XREF: __security_check_cookie:loc_4030A7j
		push	8
		push	offset dword_40A9F8
		call	__SEH_prolog
		and	dword ptr [ebp-4], 0
		push	0
		push	1
		call	__security_error_handler
		pop	ecx
		pop	ecx
		jmp	short loc_403091
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		xor	eax, eax
		inc	eax
		retn
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		mov	esp, [ebp-18h]

loc_403091:				; CODE XREF: report_failure+1Bj
		or	dword ptr [ebp-4], 0FFFFFFFFh
		push	3
		call	ds:__imp__ExitProcess@4	; __declspec(dllimport)	ExitProcess(x)
		int	3		; Trap to Debugger
report_failure	endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


__security_check_cookie	proc near	; CODE XREF: _output+7CDp
					; _NMSG_WRITE+167p ...
		cmp	ecx, __security_cookie
		jnz	short loc_4030A7
		retn
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_4030A7:				; CODE XREF: __security_check_cookie+6j
		jmp	report_failure
__security_check_cookie	endp

; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		align 10h

; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


_aulldvrm	proc near		; CODE XREF: _output+654p

arg_0		= dword	ptr  8
arg_4		= dword	ptr  0Ch
arg_8		= dword	ptr  10h
arg_C		= dword	ptr  14h

		push	esi
		mov	eax, [esp+arg_C]
		or	eax, eax
		jnz	short loc_4030E1
		mov	ecx, [esp+arg_8]
		mov	eax, [esp+arg_4]
		xor	edx, edx
		div	ecx
		mov	ebx, eax
		mov	eax, [esp+arg_0]
		div	ecx
		mov	esi, eax
		mov	eax, ebx
		mul	[esp+arg_8]
		mov	ecx, eax
		mov	eax, esi
		mul	[esp+arg_8]
		add	edx, ecx
		jmp	short loc_403128
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_4030E1:				; CODE XREF: _aulldvrm+7j
		mov	ecx, eax
		mov	ebx, [esp+arg_8]
		mov	edx, [esp+arg_4]
		mov	eax, [esp+arg_0]

loc_4030EF:				; CODE XREF: _aulldvrm+49j
		shr	ecx, 1
		rcr	ebx, 1
		shr	edx, 1
		rcr	eax, 1
		or	ecx, ecx
		jnz	short loc_4030EF
		div	ebx
		mov	esi, eax
		mul	[esp+arg_C]
		mov	ecx, eax
		mov	eax, [esp+arg_8]
		mul	esi
		add	edx, ecx
		jb	short loc_40311D
		cmp	edx, [esp+arg_4]
		ja	short loc_40311D
		jb	short loc_403126
		cmp	eax, [esp+arg_0]
		jbe	short loc_403126

loc_40311D:				; CODE XREF: _aulldvrm+5Dj
					; _aulldvrm+63j
		dec	esi
		sub	eax, [esp+arg_8]
		sbb	edx, [esp+arg_C]

loc_403126:				; CODE XREF: _aulldvrm+65j
					; _aulldvrm+6Bj
		xor	ebx, ebx

loc_403128:				; CODE XREF: _aulldvrm+2Fj
		sub	eax, [esp+arg_0]
		sbb	edx, [esp+arg_4]
		neg	edx
		neg	eax
		sbb	edx, 0
		mov	ecx, edx
		mov	edx, ebx
		mov	ebx, ecx
		mov	ecx, eax
		mov	eax, esi
		pop	esi
		retn	10h
_aulldvrm	endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


calloc		proc near		; CODE XREF: __initstdio+22p
					; __initstdio+3Bp ...

arg_0		= dword	ptr  0Ch
arg_4		= dword	ptr  10h

		push	ebx
		push	esi
		mov	esi, [esp+arg_0]
		imul	esi, [esp+arg_4]
		test	esi, esi
		push	edi
		mov	ebx, esi
		jnz	short loc_403158
		inc	esi

loc_403158:				; CODE XREF: calloc+10j calloc+65j
		xor	edi, edi
		cmp	esi, 0FFFFFFE0h
		ja	short loc_403198
		cmp	__active_heap, 3
		jnz	short loc_403183
		add	esi, 0Fh
		and	esi, 0FFFFFFF0h
		cmp	ebx, __sbh_threshold
		ja	short loc_403183
		push	ebx
		call	__sbh_alloc_block
		mov	edi, eax
		test	edi, edi
		pop	ecx
		jnz	short loc_4031AE

loc_403183:				; CODE XREF: calloc+21j calloc+2Fj
		push	esi
		push	8
		push	_crtheap
		call	ds:__imp__HeapAlloc@12 ; __declspec(dllimport) HeapAlloc(x,x,x)
		mov	edi, eax
		test	edi, edi
		jnz	short loc_4031BA

loc_403198:				; CODE XREF: calloc+18j
		cmp	_newmode, 0
		jz	short loc_4031BA
		push	esi
		call	_callnewh
		test	eax, eax
		pop	ecx
		jnz	short loc_403158
		jmp	short loc_4031BC
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_4031AE:				; CODE XREF: calloc+3Cj
		push	ebx
		push	0
		push	edi
		call	memset
		add	esp, 0Ch

loc_4031BA:				; CODE XREF: calloc+51j calloc+5Aj
		mov	eax, edi

loc_4031BC:				; CODE XREF: calloc+67j
		pop	edi
		pop	esi
		pop	ebx
		retn
calloc		endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


_fcloseall	proc near		; CODE XREF: __endstdio+Ej
		push	esi
		push	edi
		push	3
		pop	esi
		xor	edi, edi
		cmp	_nstream, esi
		jle	short loc_403213

loc_4031CF:				; CODE XREF: _fcloseall+51j
		mov	eax, __piob
		mov	eax, [eax+esi*4]
		test	eax, eax
		jz	short loc_40320A
		test	byte ptr [eax+0Ch], 83h
		jz	short loc_4031EE
		push	eax
		call	fclose
		cmp	eax, 0FFFFFFFFh
		pop	ecx
		jz	short loc_4031EE
		inc	edi

loc_4031EE:				; CODE XREF: _fcloseall+1Fj
					; _fcloseall+2Bj
		cmp	esi, 14h
		jl	short loc_40320A
		mov	eax, __piob
		push	dword ptr [eax+esi*4]
		call	free
		mov	eax, __piob
		and	dword ptr [eax+esi*4], 0
		pop	ecx

loc_40320A:				; CODE XREF: _fcloseall+19j
					; _fcloseall+31j
		inc	esi
		cmp	esi, _nstream
		jl	short loc_4031CF

loc_403213:				; CODE XREF: _fcloseall+Dj
		mov	eax, edi
		pop	edi
		pop	esi
		retn
_fcloseall	endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


; _onexit_t __cdecl onexit(_onexit_t)
_onexit		proc near		; CODE XREF: atexit+4p

arg_0		= dword	ptr  8

		push	esi
		push	__onexitbegin
		call	_msize
		pop	ecx
		mov	ecx, __onexitend
		mov	esi, eax
		mov	eax, __onexitbegin
		mov	edx, ecx
		sub	edx, eax
		add	edx, 4
		cmp	esi, edx
		jnb	short loc_40328B
		mov	ecx, 800h
		cmp	esi, ecx
		jnb	short loc_403248
		mov	ecx, esi

loc_403248:				; CODE XREF: _onexit+2Cj
		add	ecx, esi
		push	ecx
		push	eax
		call	realloc
		test	eax, eax
		pop	ecx
		pop	ecx
		jnz	short loc_40326E
		add	esi, 10h
		push	esi
		push	__onexitbegin
		call	realloc
		test	eax, eax
		pop	ecx
		pop	ecx
		jnz	short loc_40326E
		pop	esi
		retn
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_40326E:				; CODE XREF: _onexit+3Dj _onexit+52j
		mov	ecx, __onexitend
		sub	ecx, __onexitbegin
		mov	__onexitbegin, eax
		sar	ecx, 2
		lea	ecx, [eax+ecx*4]
		mov	__onexitend, ecx

loc_40328B:				; CODE XREF: _onexit+23j
		mov	eax, [esp+arg_0]
		mov	[ecx], eax
		add	__onexitend, 4
		pop	esi
		retn
_onexit		endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


atexit		proc near		; CODE XREF: _cinit+40p

arg_0		= dword	ptr  4

		push	[esp+arg_0]	; _onexit_t
		call	_onexit
		neg	eax
		sbb	eax, eax
		neg	eax
		pop	ecx
		dec	eax
		retn
atexit		endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


__onexitinit	proc near		; DATA XREF: .data:0040C414o
		push	80h
		call	malloc
		test	eax, eax
		pop	ecx
		mov	__onexitbegin, eax
		jnz	short loc_4032C4
		push	18h
		pop	eax
		retn
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_4032C4:				; CODE XREF: __onexitinit+12j
		and	dword ptr [eax], 0
		mov	eax, __onexitbegin
		mov	__onexitend, eax
		xor	eax, eax
		retn
__onexitinit	endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ

; Attributes: bp-based frame

__crtMessageBoxA proc near		; CODE XREF: _NMSG_WRITE+129p
					; __security_error_handler+138p

var_10		= dword	ptr -10h
var_8		= byte ptr -8
var_4		= dword	ptr -4
arg_0		= dword	ptr  8
arg_4		= dword	ptr  0Ch
arg_A		= byte ptr  12h

		push	ebp
		mov	ebp, esp
		sub	esp, 10h
		push	ebx
		xor	ebx, ebx
		cmp	dword_40D59C, ebx
		push	esi
		push	edi
		jnz	short loc_403354
		push	offset ??_C@_0L@GMPLCCII@user32?4dll?$AA@ ; "user32.dll"
		call	ds:__imp__LoadLibraryA@4 ; __declspec(dllimport) LoadLibraryA(x)
		mov	edi, eax
		cmp	edi, ebx
		jz	loc_40338F
		mov	esi, ds:__imp__GetProcAddress@8	; __declspec(dllimport)	GetProcAddress(x,x)
		push	offset ??_C@_0M@CHKKJDAI@MessageBoxA?$AA@ ; "MessageBoxA"
		push	edi
		call	esi ; __declspec(dllimport) GetProcAddress(x,x)	; __declspec(dllimport)	GetProcAddress(x,x)
		test	eax, eax
		mov	dword_40D59C, eax
		jz	short loc_40338F
		push	offset ??_C@_0BA@HNOPNCHB@GetActiveWindow?$AA@ ; "GetActiveWindow"
		push	edi
		call	esi ; __declspec(dllimport) GetProcAddress(x,x)	; __declspec(dllimport)	GetProcAddress(x,x)
		push	offset ??_C@_0BD@HHGDFDBJ@GetLastActivePopup?$AA@ ; "GetLastActivePopup"
		push	edi
		mov	dword_40D5A0, eax
		call	esi ; __declspec(dllimport) GetProcAddress(x,x)	; __declspec(dllimport)	GetProcAddress(x,x)
		cmp	_osplatform, 2
		mov	dword_40D5A4, eax
		jnz	short loc_403354
		push	offset ??_C@_0BK@CIDNPOGP@GetUserObjectInformationA?$AA@ ; "GetUserObjectInformationA"
		push	edi
		call	esi ; __declspec(dllimport) GetProcAddress(x,x)	; __declspec(dllimport)	GetProcAddress(x,x)
		test	eax, eax
		mov	dword_40D5AC, eax
		jz	short loc_403354
		push	offset ??_C@_0BI@DFKBFLJE@GetProcessWindowStation?$AA@ ; "GetProcessWindowStation"
		push	edi
		call	esi ; __declspec(dllimport) GetProcAddress(x,x)	; __declspec(dllimport)	GetProcAddress(x,x)
		mov	dword_40D5A8, eax

loc_403354:				; CODE XREF: __crtMessageBoxA+11j
					; __crtMessageBoxA+60j	...
		mov	eax, dword_40D5A8
		test	eax, eax
		jz	short loc_403399
		call	eax
		test	eax, eax
		jz	short loc_403380
		lea	ecx, [ebp+var_4]
		push	ecx
		push	0Ch
		lea	ecx, [ebp+var_10]
		push	ecx
		push	1
		push	eax
		call	dword_40D5AC
		test	eax, eax
		jz	short loc_403380
		test	[ebp+var_8], 1
		jnz	short loc_403399

loc_403380:				; CODE XREF: __crtMessageBoxA+8Dj
					; __crtMessageBoxA+A4j
		cmp	_winmajor, 4
		jb	short loc_403393
		or	[ebp+arg_A], 20h
		jmp	short loc_4033B8
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_40338F:				; CODE XREF: __crtMessageBoxA+22j
					; __crtMessageBoxA+3Dj
		xor	eax, eax
		jmp	short loc_4033C8
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_403393:				; CODE XREF: __crtMessageBoxA+B3j
		or	[ebp+arg_A], 4
		jmp	short loc_4033B8
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_403399:				; CODE XREF: __crtMessageBoxA+87j
					; __crtMessageBoxA+AAj
		mov	eax, dword_40D5A0
		test	eax, eax
		jz	short loc_4033B8
		call	eax
		mov	ebx, eax
		test	ebx, ebx
		jz	short loc_4033B8
		mov	eax, dword_40D5A4
		test	eax, eax
		jz	short loc_4033B8
		push	ebx
		call	eax
		mov	ebx, eax

loc_4033B8:				; CODE XREF: __crtMessageBoxA+B9j
					; __crtMessageBoxA+C3j	...
		push	dword ptr [ebp+10h]
		push	[ebp+arg_4]
		push	[ebp+arg_0]
		push	ebx
		call	dword_40D59C

loc_4033C8:				; CODE XREF: __crtMessageBoxA+BDj
		pop	edi
		pop	esi
		pop	ebx
		leave
		retn
__crtMessageBoxA endp

; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		align 10h

; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


strncpy		proc near		; CODE XREF: _NMSG_WRITE+C7p
					; __security_error_handler+D2p

arg_0		= dword	ptr  4
arg_4		= dword	ptr  8
arg_8		= dword	ptr  0Ch
arg_C		= dword	ptr  10h

		mov	ecx, [esp+arg_8]
		push	edi
		test	ecx, ecx
		jz	loc_40346F
		push	esi
		push	ebx
		mov	ebx, ecx
		mov	esi, [esp+0Ch+arg_4]
		test	esi, 3
		mov	edi, [esp+0Ch+arg_0]
		jnz	short loc_4033FC
		shr	ecx, 2
		jnz	loc_40347F
		jmp	short loc_403423
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_4033FC:				; CODE XREF: strncpy+1Fj strncpy+45j
		mov	al, [esi]
		add	esi, 1
		mov	[edi], al
		add	edi, 1
		sub	ecx, 1
		jz	short loc_403436
		test	al, al
		jz	short loc_40343E
		test	esi, 3
		jnz	short loc_4033FC
		mov	ebx, ecx
		shr	ecx, 2
		jnz	short loc_40347F

loc_40341E:				; CODE XREF: strncpy+ADj
		and	ebx, 3
		jz	short loc_403436

loc_403423:				; CODE XREF: strncpy+2Aj strncpy+64j
		mov	al, [esi]
		add	esi, 1
		mov	[edi], al
		add	edi, 1
		test	al, al
		jz	short loc_403468
		sub	ebx, 1
		jnz	short loc_403423

loc_403436:				; CODE XREF: strncpy+39j strncpy+51j
		mov	eax, [esp+0Ch+arg_0]
		pop	ebx
		pop	esi
		pop	edi
		retn
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_40343E:				; CODE XREF: strncpy+3Dj
		test	edi, 3
		jz	short loc_40345C

loc_403446:				; CODE XREF: strncpy+8Aj
		mov	[edi], al
		add	edi, 1
		sub	ecx, 1
		jz	loc_4034EC
		test	edi, 3
		jnz	short loc_403446

loc_40345C:				; CODE XREF: strncpy+74j
		mov	ebx, ecx
		shr	ecx, 2
		jnz	short loc_4034D7

loc_403463:				; CODE XREF: strncpy+9Bj strncpy+116j
		mov	[edi], al
		add	edi, 1

loc_403468:				; CODE XREF: strncpy+5Fj
		sub	ebx, 1
		jnz	short loc_403463
		pop	ebx
		pop	esi

loc_40346F:				; CODE XREF: strncpy+7j
		mov	eax, [esp+4+arg_0]
		pop	edi
		retn
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_403475:				; CODE XREF: strncpy+C7j strncpy+DFj
		mov	[edi], edx
		add	edi, 4
		sub	ecx, 1
		jz	short loc_40341E

loc_40347F:				; CODE XREF: strncpy+24j strncpy+4Cj
		mov	edx, 7EFEFEFFh
		mov	eax, [esi]
		add	edx, eax
		xor	eax, 0FFFFFFFFh
		xor	eax, edx
		mov	edx, [esi]
		add	esi, 4
		test	eax, 81010100h
		jz	short loc_403475
		test	dl, dl
		jz	short loc_4034C9
		test	dh, dh
		jz	short loc_4034BF
		test	edx, 0FF0000h
		jz	short loc_4034B5
		test	edx, 0FF000000h
		jnz	short loc_403475
		mov	[edi], edx
		jmp	short loc_4034CD
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_4034B5:				; CODE XREF: strncpy+D7j
		and	edx, 0FFFFh
		mov	[edi], edx
		jmp	short loc_4034CD
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_4034BF:				; CODE XREF: strncpy+CFj
		and	edx, 0FFh
		mov	[edi], edx
		jmp	short loc_4034CD
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_4034C9:				; CODE XREF: strncpy+CBj
		xor	edx, edx
		mov	[edi], edx

loc_4034CD:				; CODE XREF: strncpy+E3j strncpy+EDj ...
		add	edi, 4
		xor	eax, eax
		sub	ecx, 1
		jz	short loc_4034E3

loc_4034D7:				; CODE XREF: strncpy+91j
		xor	eax, eax

loc_4034D9:				; CODE XREF: strncpy+111j
		mov	[edi], eax
		add	edi, 4
		sub	ecx, 1
		jnz	short loc_4034D9

loc_4034E3:				; CODE XREF: strncpy+105j
		and	ebx, 3
		jnz	loc_403463

loc_4034EC:				; CODE XREF: strncpy+7Ej
		mov	eax, [esp+arg_C]
		pop	ebx
		pop	esi
		pop	edi
		retn
strncpy		endp ; sp =  0Ch


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


getSystemCP	proc near
		and	fSystemSet, 0
		cmp	eax, 0FFFFFFFEh
		jnz	short loc_403510
		mov	fSystemSet, 1
		jmp	ds:__imp__GetOEMCP@0 ; __declspec(dllimport) GetOEMCP()
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_403510:				; CODE XREF: getSystemCP+Aj
		cmp	eax, 0FFFFFFFDh
		jnz	short loc_403525
		mov	fSystemSet, 1
		jmp	ds:__imp__GetACP@0 ; __declspec(dllimport) GetACP()
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_403525:				; CODE XREF: getSystemCP+1Fj
		cmp	eax, 0FFFFFFFCh
		jnz	short locret_403539
		mov	eax, __lc_codepage
		mov	fSystemSet, 1

locret_403539:				; CODE XREF: getSystemCP+34j
		retn
getSystemCP	endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


CPtoLCID	proc near		; CODE XREF: _setmbcp+157p
					; _setmbcp+19Cp
		sub	eax, 3A4h
		jz	short loc_403563
		sub	eax, 4
		jz	short loc_40355D
		sub	eax, 0Dh
		jz	short loc_403557
		dec	eax
		jz	short loc_403551
		xor	eax, eax
		retn
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_403551:				; CODE XREF: CPtoLCID+12j
		mov	eax, 404h
		retn
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_403557:				; CODE XREF: CPtoLCID+Fj
		mov	eax, 412h
		retn
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_40355D:				; CODE XREF: CPtoLCID+Aj
		mov	eax, 804h
		retn
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_403563:				; CODE XREF: CPtoLCID+5j
		mov	eax, 411h
		retn
CPtoLCID	endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


setSBCS		proc near		; CODE XREF: _setmbcp:loc_4038E6p
		push	edi
		push	40h
		xor	eax, eax
		pop	ecx
		mov	edi, offset _mbctype
		rep stosd
		stosb
		xor	eax, eax
		mov	__mbcodepage, eax
		mov	__ismbcodepage,	eax
		mov	__mblcid, eax
		mov	edi, offset __mbulinfo
		stosd
		stosd
		stosd
		pop	edi
		retn
setSBCS		endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ

; Attributes: bp-based frame

setSBUpLow	proc near		; CODE XREF: _setmbcp:loc_4038EBp

var_518		= dword	ptr -518h
var_318		= dword	ptr -318h
var_218		= dword	ptr -218h
var_118		= dword	ptr -118h
var_18		= dword	ptr -18h
var_12		= byte ptr -12h
var_11		= dword	ptr -11h
var_4		= dword	ptr -4

		push	ebp
		mov	ebp, esp
		sub	esp, 518h
		mov	eax, __security_cookie
		mov	[ebp+var_4], eax
		push	esi
		lea	eax, [ebp+var_18]
		push	eax
		push	__mbcodepage
		call	ds:__imp__GetCPInfo@8 ;	__declspec(dllimport) GetCPInfo(x,x)
		cmp	eax, 1
		mov	esi, 100h
		jnz	loc_4036CF
		xor	eax, eax

loc_4035C4:				; CODE XREF: setSBUpLow+3Cj
		mov	byte ptr [ebp+eax+var_118], al
		inc	eax
		cmp	eax, esi
		jb	short loc_4035C4
		mov	al, [ebp+var_12]
		test	al, al
		mov	byte ptr [ebp+var_118],	20h
		jz	short loc_403614
		push	ebx
		lea	edx, [ebp+var_11]
		push	edi

loc_4035E3:				; CODE XREF: setSBUpLow+7Ej
		movzx	ecx, byte ptr [edx]
		movzx	eax, al
		cmp	eax, ecx
		ja	short loc_40360A
		sub	ecx, eax
		inc	ecx
		mov	ebx, ecx
		shr	ecx, 2
		lea	edi, [ebp+eax+var_118]
		mov	eax, 20202020h
		rep stosd
		mov	ecx, ebx
		and	ecx, 3
		rep stosb

loc_40360A:				; CODE XREF: setSBUpLow+59j
		inc	edx
		mov	al, [edx]
		inc	edx
		test	al, al
		jnz	short loc_4035E3
		pop	edi
		pop	ebx

loc_403614:				; CODE XREF: setSBUpLow+4Aj
		push	0
		push	__mblcid
		lea	eax, [ebp+var_518]
		push	__mbcodepage
		push	eax
		push	esi
		lea	eax, [ebp+var_118]
		push	eax
		push	1
		call	__crtGetStringTypeA
		push	0
		push	__mbcodepage
		lea	eax, [ebp+var_218]
		push	esi
		push	eax
		push	esi
		lea	eax, [ebp+var_118]
		push	eax
		push	esi
		push	__mblcid
		call	__crtLCMapStringA
		push	0
		push	__mbcodepage
		lea	eax, [ebp+var_318]
		push	esi
		push	eax
		push	esi
		lea	eax, [ebp+var_118]
		push	eax
		push	200h
		push	__mblcid
		call	__crtLCMapStringA
		add	esp, 5Ch
		xor	eax, eax

loc_403689:				; CODE XREF: setSBUpLow+139j
		mov	cx, word ptr [ebp+eax*2+var_518]
		test	cl, 1
		jz	short loc_4036AC
		or	byte_40D701[eax], 10h
		mov	cl, byte ptr [ebp+eax+var_218]

loc_4036A4:				; CODE XREF: setSBUpLow+12Dj
		mov	_mbcasemap[eax], cl
		jmp	short loc_4036C8
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_4036AC:				; CODE XREF: setSBUpLow+102j
		test	cl, 2
		jz	short loc_4036C1
		or	byte_40D701[eax], 20h
		mov	cl, byte ptr [ebp+eax+var_318]
		jmp	short loc_4036A4
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_4036C1:				; CODE XREF: setSBUpLow+11Dj
		mov	_mbcasemap[eax], 0

loc_4036C8:				; CODE XREF: setSBUpLow+118j
		inc	eax
		cmp	eax, esi
		jb	short loc_403689
		jmp	short loc_403713
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_4036CF:				; CODE XREF: setSBUpLow+2Aj
		xor	eax, eax

loc_4036D1:				; CODE XREF: setSBUpLow+17Fj
		cmp	eax, 41h
		jb	short loc_4036EF
		cmp	eax, 5Ah
		ja	short loc_4036EF
		or	byte_40D701[eax], 10h
		mov	cl, al
		add	cl, 20h

loc_4036E7:				; CODE XREF: setSBUpLow+173j
		mov	_mbcasemap[eax], cl
		jmp	short loc_40370E
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_4036EF:				; CODE XREF: setSBUpLow+142j
					; setSBUpLow+147j
		cmp	eax, 61h
		jb	short loc_403707
		cmp	eax, 7Ah
		ja	short loc_403707
		or	byte_40D701[eax], 20h
		mov	cl, al
		sub	cl, 20h
		jmp	short loc_4036E7
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_403707:				; CODE XREF: setSBUpLow+160j
					; setSBUpLow+165j
		mov	_mbcasemap[eax], 0

loc_40370E:				; CODE XREF: setSBUpLow+15Bj
		inc	eax
		cmp	eax, esi
		jb	short loc_4036D1

loc_403713:				; CODE XREF: setSBUpLow+13Bj
		mov	ecx, [ebp+var_4]
		pop	esi
		call	__security_check_cookie
		leave
		retn
setSBUpLow	endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ

; Attributes: bp-based frame

_setmbcp	proc near		; CODE XREF: __initmbctable+Bp

var_1C		= dword	ptr -1Ch
var_18		= dword	ptr -18h
var_12		= byte ptr -12h
var_11		= dword	ptr -11h
var_4		= dword	ptr -4
arg_0		= dword	ptr  8

		push	ebp
		mov	ebp, esp
		sub	esp, 1Ch
		mov	eax, __security_cookie
		push	ebx
		push	esi
		mov	esi, [ebp+arg_0]
		xor	ebx, ebx
		cmp	esi, 0FFFFFFFEh
		mov	[ebp+var_4], eax
		push	edi
		mov	fSystemSet, ebx
		jnz	short loc_403751
		mov	fSystemSet, 1
		call	ds:__imp__GetOEMCP@0 ; __declspec(dllimport) GetOEMCP()
		jmp	short loc_40377C
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_403751:				; CODE XREF: _setmbcp+1Fj
		cmp	esi, 0FFFFFFFDh
		jnz	short loc_403768
		mov	fSystemSet, 1
		call	ds:__imp__GetACP@0 ; __declspec(dllimport) GetACP()
		jmp	short loc_40377C
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_403768:				; CODE XREF: _setmbcp+36j
		cmp	esi, 0FFFFFFFCh
		jnz	short loc_403781
		mov	eax, __lc_codepage
		mov	fSystemSet, 1

loc_40377C:				; CODE XREF: _setmbcp+31j _setmbcp+48j
		mov	[ebp+arg_0], eax
		mov	esi, eax

loc_403781:				; CODE XREF: _setmbcp+4Dj
		cmp	esi, __mbcodepage
		jz	loc_4038F0
		cmp	esi, ebx
		jz	loc_4038E6
		xor	edx, edx
		xor	eax, eax

loc_403799:				; CODE XREF: _setmbcp+8Cj
		cmp	__rgcode_page_info[eax], esi
		jz	short loc_403808
		add	eax, 30h
		inc	edx
		cmp	eax, 0F0h
		jb	short loc_403799
		lea	eax, [ebp+var_18]
		push	eax
		push	esi
		call	ds:__imp__GetCPInfo@8 ;	__declspec(dllimport) GetCPInfo(x,x)
		cmp	eax, 1
		jnz	loc_4038DE
		push	40h
		xor	eax, eax
		pop	ecx
		mov	edi, offset _mbctype
		rep stosd
		stosb
		xor	edi, edi
		inc	edi
		cmp	[ebp+var_18], edi
		mov	__mbcodepage, esi
		mov	__mblcid, ebx
		jbe	loc_4038CC
		cmp	[ebp+var_12], 0
		jz	loc_4038A7
		lea	ecx, [ebp+var_11]

loc_4037F2:				; CODE XREF: _setmbcp+183j
		mov	dl, [ecx]
		test	dl, dl
		jz	loc_4038A7
		movzx	eax, byte ptr [ecx-1]
		movzx	edx, dl
		jmp	loc_403897
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_403808:				; CODE XREF: _setmbcp+81j
		push	40h
		xor	eax, eax
		pop	ecx
		mov	edi, offset _mbctype
		rep stosd
		lea	ecx, [edx+edx*2]
		shl	ecx, 4
		mov	[ebp+var_1C], ebx
		stosb
		lea	ebx, dword_40CF58[ecx]

loc_403824:				; CODE XREF: _setmbcp+143j
		mov	al, [ebx]
		mov	esi, ebx
		jmp	short loc_403853
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_40382A:				; CODE XREF: _setmbcp+137j
		mov	dl, [esi+1]
		test	dl, dl
		jz	short loc_403857
		movzx	eax, al
		movzx	edi, dl
		cmp	eax, edi
		ja	short loc_40384F
		mov	edx, [ebp+var_1C]
		mov	dl, __rgctypeflag[edx]

loc_403844:				; CODE XREF: _setmbcp+12Fj
		or	byte_40D701[eax], dl
		inc	eax
		cmp	eax, edi
		jbe	short loc_403844

loc_40384F:				; CODE XREF: _setmbcp+11Bj
		inc	esi
		inc	esi
		mov	al, [esi]

loc_403853:				; CODE XREF: _setmbcp+10Aj
		test	al, al
		jnz	short loc_40382A

loc_403857:				; CODE XREF: _setmbcp+111j
		inc	[ebp+var_1C]
		add	ebx, 8
		cmp	[ebp+var_1C], 4
		jb	short loc_403824
		mov	eax, [ebp+arg_0]
		mov	__mbcodepage, eax
		mov	__ismbcodepage,	1
		call	CPtoLCID
		lea	esi, dword_40CF4C[ecx]
		mov	edi, offset __mbulinfo
		movsd
		movsd
		mov	__mblcid, eax
		movsd
		jmp	short loc_4038EB
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_40388F:				; CODE XREF: _setmbcp+17Bj
		or	byte_40D701[eax], 4
		inc	eax

loc_403897:				; CODE XREF: _setmbcp+E5j
		cmp	eax, edx
		jbe	short loc_40388F
		inc	ecx
		inc	ecx
		cmp	byte ptr [ecx-1], 0
		jnz	loc_4037F2

loc_4038A7:				; CODE XREF: _setmbcp+CBj _setmbcp+D8j
		mov	eax, edi

loc_4038A9:				; CODE XREF: _setmbcp+198j
		or	byte_40D701[eax], 8
		inc	eax
		cmp	eax, 0FFh
		jb	short loc_4038A9
		mov	eax, esi
		call	CPtoLCID
		mov	__mblcid, eax
		mov	__ismbcodepage,	edi
		jmp	short loc_4038D2
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_4038CC:				; CODE XREF: _setmbcp+C1j
		mov	__ismbcodepage,	ebx

loc_4038D2:				; CODE XREF: _setmbcp+1ACj
		xor	eax, eax
		mov	edi, offset __mbulinfo
		stosd
		stosd
		stosd
		jmp	short loc_4038EB
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_4038DE:				; CODE XREF: _setmbcp+9Cj
		cmp	fSystemSet, ebx
		jz	short loc_4038F4

loc_4038E6:				; CODE XREF: _setmbcp+71j
		call	setSBCS

loc_4038EB:				; CODE XREF: _setmbcp+16Fj
					; _setmbcp+1BEj
		call	setSBUpLow

loc_4038F0:				; CODE XREF: _setmbcp+69j
		xor	eax, eax
		jmp	short loc_4038F7
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_4038F4:				; CODE XREF: _setmbcp+1C6j
		or	eax, 0FFFFFFFFh

loc_4038F7:				; CODE XREF: _setmbcp+1D4j
		mov	ecx, [ebp+var_4]
		pop	edi
		pop	esi
		pop	ebx
		call	__security_check_cookie
		leave
		retn
_setmbcp	endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


_getmbcp	proc near
		mov	eax, __ismbcodepage
		neg	eax
		sbb	eax, eax
		and	eax, __mbcodepage
		retn
_getmbcp	endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


__initmbctable	proc near		; CODE XREF: _setenvp+Dp _setargv+12p
					; DATA XREF: ...
		cmp	__mbctype_initialized, 0
		jnz	short loc_40392F
		push	0FFFFFFFDh
		call	_setmbcp
		pop	ecx
		mov	__mbctype_initialized, 1

loc_40392F:				; CODE XREF: __initmbctable+7j
		xor	eax, eax
		retn
__initmbctable	endp

; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		align 10h

memcpy:					; CODE XREF: __crtGetEnvironmentStringsA+10Ap
					; realloc+8Dp ...
		push	ebp
		mov	ebp, esp
		push	edi
		push	esi
		mov	esi, [ebp+0Ch]
		mov	ecx, [ebp+10h]
		mov	edi, [ebp+8]
		mov	eax, ecx
		mov	edx, ecx
		add	eax, esi
		cmp	edi, esi
		jbe	short loc_403960
		cmp	edi, eax
		jb	loc_403ADC

loc_403960:				; CODE XREF: .text:00403956j
		test	edi, 3
		jnz	short loc_40397C
		shr	ecx, 2
		and	edx, 3
		cmp	ecx, 8
		jb	short near ptr dword_40399C
		rep movsd
		jmp	ds:off_403A8C[edx*4]
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_40397C:				; CODE XREF: .text:00403966j
		mov	eax, edi
		mov	edx, 3
		sub	ecx, 4
		jb	short loc_403994
		and	eax, 3
		add	ecx, eax
		jmp	ds:dword_4039A0[eax*4]
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_403994:				; CODE XREF: .text:00403986j
		jmp	dword ptr ds:loc_403A9C[ecx*4]
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		align 4
dword_40399C	dd 208D24FFh		; CODE XREF: .text:00403971j
dword_4039A0	dd 9000403Ah		; DATA XREF: .text:0040398Dr

; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


sub_4039A4	proc near
		mov	al, 39h
		inc	eax
		add	ah, bl
		cmp	[eax+0], eax
		add	[edx], bh
		inc	eax
		add	[ebx], ah
		ror	dword ptr [edx-75F877FAh], 1
		inc	esi
		add	[eax+468A0147h], ecx
		add	al, cl
		jmp	near ptr 287C1C7h
sub_4039A4	endp

; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		dd 8303C683h, 0F98303C7h, 0F3CC7208h, 9524FFA5h, 403A8Ch
		dd 2300498Dh, 88068AD1h, 1468A07h, 8802E9C1h, 0C6830147h
		dd 2C78302h, 7208F983h,	0FFA5F3A6h, 3A8C9524h, 23900040h
		dd 88068AD1h, 1C68307h,	8302E9C1h, 0F98301C7h, 0F3887208h
		dd 9524FFA5h, 403A8Ch
		align 10h

; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


sub_403A20	proc near
		cmp	dword ptr [edx], 40h
		add	[eax+3Ah], dh
		inc	eax
		add	[eax+3Ah], ch
		inc	eax
		add	[eax+3Ah], ah
		inc	eax
		add	[eax+3Ah], bl
		inc	eax
		add	[eax+3Ah], dl
		inc	eax
		add	[eax+3Ah], cl
		inc	eax
		add	[eax+3Ah], al
		inc	eax
		add	[ebx-761B71BCh], cl
		inc	esp
		pop	esp
		mov	eax, [esi+ecx*4-18h]
		mov	[edi+ecx*4-18h], eax
		mov	eax, [esi+ecx*4-14h]
		mov	[edi+ecx*4-14h], eax
		mov	eax, [esi+ecx*4-10h]
		mov	[edi+ecx*4-10h], eax
		mov	eax, [esi+ecx*4-0Ch]
		mov	[edi+ecx*4-0Ch], eax
		mov	eax, [esi+ecx*4-8]
		mov	[edi+ecx*4-8], eax
		mov	eax, [esi+ecx*4-4]
		mov	[edi+ecx*4-4], eax
		lea	eax, ds:0[ecx*4]
		add	esi, eax
		add	edi, eax
		jmp	ds:off_403A8C[edx*4]
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		align 4
off_403A8C	dd offset loc_403A9C	; DATA XREF: .text:00403975r
					; sub_403A20+63r
		dd offset loc_403AA4
		dd offset loc_403AB0
		dd offset loc_403AC4
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_403A9C:				; CODE XREF: .text:00403975j
					; sub_403A20+63j
					; DATA XREF: ...
		mov	eax, [ebp+8]
		pop	esi
		pop	edi
		leave
		retn
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		align 4

loc_403AA4:				; CODE XREF: sub_403A20+63j
					; DATA XREF: sub_403A20+70o
		mov	al, [esi]
		mov	[edi], al
		mov	eax, [ebp+8]
		pop	esi
		pop	edi
		leave
		retn
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		align 10h

loc_403AB0:				; CODE XREF: sub_403A20+63j
					; DATA XREF: sub_403A20+74o
		mov	al, [esi]
		mov	[edi], al
		mov	al, [esi+1]
		mov	[edi+1], al
		mov	eax, [ebp+8]
		pop	esi
		pop	edi
		leave
		retn
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		align 4

loc_403AC4:				; CODE XREF: sub_403A20+63j
					; DATA XREF: sub_403A20+78o
		mov	al, [esi]
		mov	[edi], al
		mov	al, [esi+1]
		mov	[edi+1], al
		mov	al, [esi+2]
		mov	[edi+2], al
		mov	eax, [ebp+8]
		pop	esi
		pop	edi
		leave
		retn
sub_403A20	endp ; sp =  4

; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		align 4

loc_403ADC:				; CODE XREF: .text:0040395Aj
		lea	esi, [ecx+esi-4]
		lea	edi, [ecx+edi-4]
		test	edi, 3
		jnz	short loc_403B10
		shr	ecx, 2
		and	edx, 3
		cmp	ecx, 8
		jb	short loc_403B04
		std
		rep movsd
		cld
		jmp	dword ptr ds:sub_403C28[edx*4]
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		align 4

loc_403B04:				; CODE XREF: .text:00403AF5j
					; .text:00403B50j ...
		neg	ecx
		jmp	dword ptr ds:loc_403BD7+1[ecx*4]
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		align 10h

loc_403B10:				; CODE XREF: .text:00403AEAj
		mov	eax, edi
		mov	edx, 3
		cmp	ecx, 4
		jb	short near ptr dword_403B28
		and	eax, 3
		sub	ecx, eax
		jmp	ds:dword_403B2C[eax*4]
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
dword_403B28	dd 288D24FFh		; CODE XREF: .text:00403B1Aj
dword_403B2C	dd 9000403Ch		; DATA XREF: .text:00403B21r

; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


sub_403B30	proc near
		cmp	al, 3Bh
		inc	eax
		add	[eax+3Bh], ah
		inc	eax
		add	[eax-75FFBFC5h], cl
		inc	esi
		add	esp, [ebx]
		ror	dword ptr [eax-117CFCB9h], 1
		add	ecx, eax
		jmp	near ptr 22FBE4Fh
sub_403B30	endp

; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		cmp	ecx, 8
		jb	short loc_403B04
		std
		rep movsd
		cld
		jmp	dword ptr ds:sub_403C28[edx*4]
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		align 10h
		mov	al, [esi+3]
		and	edx, ecx
		mov	[edi+3], al
		mov	al, [esi+2]
		shr	ecx, 2
		mov	[edi+2], al
		sub	esi, 2
		sub	edi, 2
		cmp	ecx, 8
		jb	short loc_403B04
		std
		rep movsd
		cld
		jmp	dword ptr ds:sub_403C28[edx*4]
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		align 4
		mov	al, [esi+3]
		and	edx, ecx
		mov	[edi+3], al
		mov	al, [esi+2]
		mov	[edi+2], al
		mov	al, [esi+1]
		shr	ecx, 2
		mov	[edi+1], al
		sub	esi, 3
		sub	edi, 3
		cmp	ecx, 8
		jb	loc_403B04
		std
		rep movsd
		cld
		jmp	dword ptr ds:sub_403C28[edx*4]
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		align 4

; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


sub_403BBC	proc near
		fdivr	qword ptr [ebx]
		inc	eax
		add	ah, ah
		cmp	eax, [eax+0]
		in	al, dx		; DMA controller, 8237A-5.
					; channel 1 current word count
		cmp	eax, [eax+0]
		hlt
		cmp	eax, [eax+0]
		cld
		cmp	eax, [eax+0]
		add	al, 3Ch
		inc	eax
		add	[esp+edi+0], cl
		inc	eax

loc_403BD7:				; DATA XREF: .text:00403B06r
		add	[edi], bl
		cmp	al, 40h
		add	[ebx-76E371BCh], cl
		inc	esp
		pop	dword ptr [ebx+ecx*4]
		inc	esp
		mov	ds, word ptr [eax]
		mov	[edi+ecx*4+18h], eax
		mov	eax, [esi+ecx*4+14h]
		mov	[edi+ecx*4+14h], eax
		mov	eax, [esi+ecx*4+10h]
		mov	[edi+ecx*4+10h], eax
		mov	eax, [esi+ecx*4+0Ch]
		mov	[edi+ecx*4+0Ch], eax
		mov	eax, [esi+ecx*4+8]
		mov	[edi+ecx*4+8], eax
		mov	eax, [esi+ecx*4+4]
		mov	[edi+ecx*4+4], eax
		lea	eax, ds:0[ecx*4]
		add	esi, eax
		add	edi, eax
		jmp	dword ptr ds:sub_403C28[edx*4]
sub_403BBC	endp

; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		align 4

; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


sub_403C28	proc near		; DATA XREF: .text:00403AFBr
					; .text:00403B56r ...

arg_3C		= byte ptr  40h

		cmp	[eax+eax*2], bh
		add	[eax+3Ch], al
		inc	eax
		add	[eax+3Ch], dl
		inc	eax
		add	[esp+edi+arg_3C], ah
		add	[ebx+5F5E0845h], cl
		leave
		retn
sub_403C28	endp ; sp =  4

; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		align 10h
		mov	al, [esi+3]
		mov	[edi+3], al
		mov	eax, [ebp+8]
		pop	esi
		pop	edi
		leave
		retn
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		align 10h
		mov	al, [esi+3]
		mov	[edi+3], al
		mov	al, [esi+2]
		mov	[edi+2], al
		mov	eax, [ebp+8]
		pop	esi
		pop	edi
		leave
		retn
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		align 4
		mov	al, [esi+3]
		mov	[edi+3], al
		mov	al, [esi+2]
		mov	[edi+2], al
		mov	al, [esi+1]
		mov	[edi+1], al
		mov	eax, [ebp+8]
		pop	esi
		pop	edi
		leave
		retn

; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


_get_sbh_threshold proc	near
		mov	eax, __active_heap
		sub	eax, 3
		neg	eax
		sbb	eax, eax
		not	eax
		and	eax, __sbh_threshold
		retn
_get_sbh_threshold endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


__sbh_heap_init	proc near		; CODE XREF: _heap_init+34p
					; _set_sbh_threshold+38p

arg_0		= dword	ptr  4

		push	140h
		push	0
		push	_crtheap
		call	ds:__imp__HeapAlloc@12 ; __declspec(dllimport) HeapAlloc(x,x,x)
		test	eax, eax
		mov	__sbh_pHeaderList, eax
		jnz	short loc_403CAF
		retn
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_403CAF:				; CODE XREF: __sbh_heap_init+1Aj
		mov	ecx, [esp+arg_0]
		and	__sbh_pHeaderDefer, 0
		and	__sbh_cntHeaderList, 0
		mov	__sbh_pHeaderScan, eax
		xor	eax, eax
		mov	__sbh_threshold, ecx
		mov	__sbh_sizeHeaderList, 10h
		inc	eax
		retn
__sbh_heap_init	endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


__sbh_find_block proc near		; CODE XREF: free+13p realloc+48p ...

arg_0		= dword	ptr  4

		mov	eax, __sbh_cntHeaderList
		lea	ecx, [eax+eax*4]
		mov	eax, __sbh_pHeaderList
		lea	ecx, [eax+ecx*4]
		jmp	short loc_403CFE
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_403CEC:				; CODE XREF: __sbh_find_block+26j
		mov	edx, [esp+arg_0]
		sub	edx, [eax+0Ch]
		cmp	edx, 100000h
		jb	short locret_403D04
		add	eax, 14h

loc_403CFE:				; CODE XREF: __sbh_find_block+10j
		cmp	eax, ecx
		jb	short loc_403CEC
		xor	eax, eax

locret_403D04:				; CODE XREF: __sbh_find_block+1Fj
		retn
__sbh_find_block endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ

; Attributes: bp-based frame

__sbh_free_block proc near		; CODE XREF: free+1Fp realloc+9Cp ...

var_10		= dword	ptr -10h
var_C		= dword	ptr -0Ch
var_8		= dword	ptr -8
var_4		= dword	ptr -4
arg_0		= dword	ptr  8
arg_4		= dword	ptr  0Ch

		push	ebp
		mov	ebp, esp
		sub	esp, 10h
		mov	ecx, [ebp+arg_0]
		mov	eax, [ecx+10h]
		push	esi
		mov	esi, [ebp+arg_4]
		push	edi
		mov	edi, esi
		sub	edi, [ecx+0Ch]
		add	esi, 0FFFFFFFCh
		shr	edi, 0Fh
		mov	ecx, edi
		imul	ecx, 204h
		lea	ecx, [ecx+eax+144h]
		mov	[ebp+var_10], ecx
		mov	ecx, [esi]
		dec	ecx
		test	cl, 1
		mov	[ebp+var_4], ecx
		jnz	loc_404019
		push	ebx
		lea	ebx, [ecx+esi]
		mov	edx, [ebx]
		mov	[ebp+var_C], edx
		mov	edx, [esi-4]
		mov	[ebp+var_8], edx
		mov	edx, [ebp+var_C]
		test	dl, 1
		mov	[ebp+arg_4], ebx
		jnz	short loc_403DD0
		sar	edx, 4
		dec	edx
		cmp	edx, 3Fh
		jbe	short loc_403D68
		push	3Fh
		pop	edx

loc_403D68:				; CODE XREF: __sbh_free_block+5Ej
		mov	ecx, [ebx+4]
		cmp	ecx, [ebx+8]
		jnz	short loc_403DB2
		cmp	edx, 20h
		mov	ebx, 80000000h
		jnb	short loc_403D93
		mov	ecx, edx
		shr	ebx, cl
		lea	ecx, [edx+eax+4]
		not	ebx
		and	[eax+edi*4+44h], ebx
		dec	byte ptr [ecx]
		jnz	short loc_403DAF
		mov	ecx, [ebp+arg_0]
		and	[ecx], ebx
		jmp	short loc_403DAF
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_403D93:				; CODE XREF: __sbh_free_block+73j
		lea	ecx, [edx-20h]
		shr	ebx, cl
		lea	ecx, [edx+eax+4]
		not	ebx
		and	[eax+edi*4+0C4h], ebx
		dec	byte ptr [ecx]
		jnz	short loc_403DAF
		mov	ecx, [ebp+arg_0]
		and	[ecx+4], ebx

loc_403DAF:				; CODE XREF: __sbh_free_block+85j
					; __sbh_free_block+8Cj	...
		mov	ebx, [ebp+arg_4]

loc_403DB2:				; CODE XREF: __sbh_free_block+69j
		mov	edx, [ebx+8]
		mov	ebx, [ebx+4]
		mov	ecx, [ebp+var_4]
		add	ecx, [ebp+var_C]
		mov	[edx+4], ebx
		mov	edx, [ebp+arg_4]
		mov	ebx, [edx+4]
		mov	edx, [edx+8]
		mov	[ebx+8], edx
		mov	[ebp+var_4], ecx

loc_403DD0:				; CODE XREF: __sbh_free_block+55j
		mov	edx, ecx
		sar	edx, 4
		dec	edx
		cmp	edx, 3Fh
		jbe	short loc_403DDE
		push	3Fh
		pop	edx

loc_403DDE:				; CODE XREF: __sbh_free_block+D4j
		mov	ebx, [ebp+var_8]
		and	ebx, 1
		mov	[ebp+var_C], ebx
		jnz	loc_403E7C
		sub	esi, [ebp+var_8]
		mov	ebx, [ebp+var_8]
		sar	ebx, 4
		push	3Fh
		mov	[ebp+arg_4], esi
		dec	ebx
		pop	esi
		cmp	ebx, esi
		jbe	short loc_403E03
		mov	ebx, esi

loc_403E03:				; CODE XREF: __sbh_free_block+FAj
		add	ecx, [ebp+var_8]
		mov	edx, ecx
		sar	edx, 4
		dec	edx
		cmp	edx, esi
		mov	[ebp+var_4], ecx
		jbe	short loc_403E15
		mov	edx, esi

loc_403E15:				; CODE XREF: __sbh_free_block+10Cj
		cmp	ebx, edx
		jz	short loc_403E77
		mov	ecx, [ebp+arg_4]
		mov	esi, [ecx+4]
		cmp	esi, [ecx+8]
		jnz	short loc_403E5F
		cmp	ebx, 20h
		mov	esi, 80000000h
		jnb	short loc_403E45
		mov	ecx, ebx
		shr	esi, cl
		not	esi
		and	[eax+edi*4+44h], esi
		dec	byte ptr [ebx+eax+4]
		jnz	short loc_403E5F
		mov	ecx, [ebp+arg_0]
		and	[ecx], esi
		jmp	short loc_403E5F
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_403E45:				; CODE XREF: __sbh_free_block+127j
		lea	ecx, [ebx-20h]
		shr	esi, cl
		not	esi
		and	[eax+edi*4+0C4h], esi
		dec	byte ptr [ebx+eax+4]
		jnz	short loc_403E5F
		mov	ecx, [ebp+arg_0]
		and	[ecx+4], esi

loc_403E5F:				; CODE XREF: __sbh_free_block+11Dj
					; __sbh_free_block+137j ...
		mov	ecx, [ebp+arg_4]
		mov	esi, [ecx+8]
		mov	ecx, [ecx+4]
		mov	[esi+4], ecx
		mov	ecx, [ebp+arg_4]
		mov	esi, [ecx+4]
		mov	ecx, [ecx+8]
		mov	[esi+8], ecx

loc_403E77:				; CODE XREF: __sbh_free_block+112j
		mov	esi, [ebp+arg_4]
		jmp	short loc_403E7F
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_403E7C:				; CODE XREF: __sbh_free_block+E2j
		mov	ebx, [ebp+arg_0]

loc_403E7F:				; CODE XREF: __sbh_free_block+175j
		cmp	[ebp+var_C], 0
		jnz	short loc_403E8D
		cmp	ebx, edx
		jz	loc_403F0D

loc_403E8D:				; CODE XREF: __sbh_free_block+17Ej
		mov	ecx, [ebp+var_10]
		lea	ecx, [ecx+edx*8]
		mov	ebx, [ecx+4]
		mov	[esi+8], ecx
		mov	[esi+4], ebx
		mov	[ecx+4], esi
		mov	ecx, [esi+4]
		mov	[ecx+8], esi
		mov	ecx, [esi+4]
		cmp	ecx, [esi+8]
		jnz	short loc_403F0D
		mov	cl, [edx+eax+4]
		mov	byte ptr [ebp+arg_4+3],	cl
		inc	cl
		cmp	edx, 20h
		mov	[edx+eax+4], cl
		jnb	short loc_403EE4
		cmp	byte ptr [ebp+arg_4+3],	0
		jnz	short loc_403ED3
		mov	ecx, edx
		mov	ebx, 80000000h
		shr	ebx, cl
		mov	ecx, [ebp+arg_0]
		or	[ecx], ebx

loc_403ED3:				; CODE XREF: __sbh_free_block+1BEj
		mov	ebx, 80000000h
		mov	ecx, edx
		shr	ebx, cl
		lea	eax, [eax+edi*4+44h]
		or	[eax], ebx
		jmp	short loc_403F0D
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_403EE4:				; CODE XREF: __sbh_free_block+1B8j
		cmp	byte ptr [ebp+arg_4+3],	0
		jnz	short loc_403EFA
		lea	ecx, [edx-20h]
		mov	ebx, 80000000h
		shr	ebx, cl
		mov	ecx, [ebp+arg_0]
		or	[ecx+4], ebx

loc_403EFA:				; CODE XREF: __sbh_free_block+1E3j
		lea	ecx, [edx-20h]
		mov	edx, 80000000h
		shr	edx, cl
		lea	eax, [eax+edi*4+0C4h]
		or	[eax], edx

loc_403F0D:				; CODE XREF: __sbh_free_block+182j
					; __sbh_free_block+1A6j ...
		mov	eax, [ebp+var_4]
		mov	[esi], eax
		mov	[eax+esi-4], eax
		mov	eax, [ebp+var_10]
		dec	dword ptr [eax]
		jnz	loc_404018
		mov	eax, __sbh_pHeaderDefer
		test	eax, eax
		jz	loc_40400A
		mov	ecx, __sbh_indGroupDefer
		mov	esi, ds:__imp__VirtualFree@12 ;	__declspec(dllimport) VirtualFree(x,x,x)
		push	4000h
		shl	ecx, 0Fh
		add	ecx, [eax+0Ch]
		mov	ebx, 8000h
		push	ebx
		push	ecx
		call	esi ; __declspec(dllimport) VirtualFree(x,x,x) ; __declspec(dllimport) VirtualFree(x,x,x)
		mov	ecx, __sbh_indGroupDefer
		mov	eax, __sbh_pHeaderDefer
		mov	edx, 80000000h
		shr	edx, cl
		or	[eax+8], edx
		mov	eax, __sbh_pHeaderDefer
		mov	eax, [eax+10h]
		mov	ecx, __sbh_indGroupDefer
		and	dword ptr [eax+ecx*4+0C4h], 0
		mov	eax, __sbh_pHeaderDefer
		mov	eax, [eax+10h]
		dec	byte ptr [eax+43h]
		mov	eax, __sbh_pHeaderDefer
		mov	ecx, [eax+10h]
		cmp	byte ptr [ecx+43h], 0
		jnz	short loc_403F9B
		and	dword ptr [eax+4], 0FFFFFFFEh
		mov	eax, __sbh_pHeaderDefer

loc_403F9B:				; CODE XREF: __sbh_free_block+28Bj
		cmp	dword ptr [eax+8], 0FFFFFFFFh
		jnz	short loc_40400A
		push	ebx
		push	0
		push	dword ptr [eax+0Ch]
		call	esi ; __declspec(dllimport) VirtualFree(x,x,x) ; __declspec(dllimport) VirtualFree(x,x,x)
		mov	eax, __sbh_pHeaderDefer
		push	dword ptr [eax+10h]
		push	0
		push	_crtheap
		call	ds:__imp__HeapFree@12 ;	__declspec(dllimport) HeapFree(x,x,x)
		mov	eax, __sbh_cntHeaderList
		mov	edx, __sbh_pHeaderList
		lea	eax, [eax+eax*4]
		shl	eax, 2
		mov	ecx, eax
		mov	eax, __sbh_pHeaderDefer
		sub	ecx, eax
		lea	ecx, [ecx+edx-14h]
		push	ecx
		lea	ecx, [eax+14h]
		push	ecx
		push	eax
		call	memmove
		mov	eax, [ebp+arg_0]
		add	esp, 0Ch
		dec	__sbh_cntHeaderList
		cmp	eax, __sbh_pHeaderDefer
		jbe	short loc_404000
		sub	[ebp+arg_0], 14h

loc_404000:				; CODE XREF: __sbh_free_block+2F5j
		mov	eax, __sbh_pHeaderList
		mov	__sbh_pHeaderScan, eax

loc_40400A:				; CODE XREF: __sbh_free_block+223j
					; __sbh_free_block+29Aj
		mov	eax, [ebp+arg_0]
		mov	__sbh_pHeaderDefer, eax
		mov	__sbh_indGroupDefer, edi

loc_404018:				; CODE XREF: __sbh_free_block+216j
		pop	ebx

loc_404019:				; CODE XREF: __sbh_free_block+37j
		pop	edi
		pop	esi
		leave
		retn
__sbh_free_block endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


__sbh_alloc_new_region proc near	; CODE XREF: __sbh_alloc_block+150p
		mov	eax, __sbh_cntHeaderList
		mov	ecx, __sbh_sizeHeaderList
		push	edi
		xor	edi, edi
		cmp	eax, ecx
		jnz	short loc_404063
		lea	eax, [ecx+ecx*4+50h]
		shl	eax, 2
		push	eax
		push	__sbh_pHeaderList
		push	edi
		push	_crtheap
		call	ds:__imp__HeapReAlloc@16 ; __declspec(dllimport) HeapReAlloc(x,x,x,x)
		cmp	eax, edi
		jnz	short loc_404052
		xor	eax, eax
		pop	edi
		retn
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_404052:				; CODE XREF: __sbh_alloc_new_region+2Fj
		add	__sbh_sizeHeaderList, 10h
		mov	__sbh_pHeaderList, eax
		mov	eax, __sbh_cntHeaderList

loc_404063:				; CODE XREF: __sbh_alloc_new_region+10j
		mov	ecx, __sbh_pHeaderList
		push	esi
		push	41C4h
		push	8
		push	_crtheap
		lea	eax, [eax+eax*4]
		lea	esi, [ecx+eax*4]
		call	ds:__imp__HeapAlloc@12 ; __declspec(dllimport) HeapAlloc(x,x,x)
		cmp	eax, edi
		mov	[esi+10h], eax
		jnz	short loc_40408E

loc_40408A:				; CODE XREF: __sbh_alloc_new_region+9Bj
		xor	eax, eax
		jmp	short loc_4040D1
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_40408E:				; CODE XREF: __sbh_alloc_new_region+6Bj
		push	4
		push	2000h
		push	100000h
		push	edi
		call	ds:__imp__VirtualAlloc@16 ; __declspec(dllimport) VirtualAlloc(x,x,x,x)
		cmp	eax, edi
		mov	[esi+0Ch], eax
		jnz	short loc_4040BA
		push	dword ptr [esi+10h]
		push	edi
		push	_crtheap
		call	ds:__imp__HeapFree@12 ;	__declspec(dllimport) HeapFree(x,x,x)
		jmp	short loc_40408A
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_4040BA:				; CODE XREF: __sbh_alloc_new_region+89j
		or	dword ptr [esi+8], 0FFFFFFFFh
		mov	[esi], edi
		mov	[esi+4], edi
		inc	__sbh_cntHeaderList
		mov	eax, [esi+10h]
		or	dword ptr [eax], 0FFFFFFFFh
		mov	eax, esi

loc_4040D1:				; CODE XREF: __sbh_alloc_new_region+6Fj
		pop	esi
		pop	edi
		retn
__sbh_alloc_new_region endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ

; Attributes: bp-based frame

__sbh_alloc_new_group proc near		; CODE XREF: __sbh_alloc_block+15Fp

var_8		= dword	ptr -8
var_4		= dword	ptr -4
arg_0		= dword	ptr  8

		push	ebp
		mov	ebp, esp
		push	ecx
		push	ecx
		mov	ecx, [ebp+arg_0]
		mov	eax, [ecx+8]
		push	ebx
		push	esi
		mov	esi, [ecx+10h]
		push	edi
		xor	ebx, ebx
		jmp	short loc_4040EC
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_4040E9:				; CODE XREF: __sbh_alloc_new_group+1Aj
		shl	eax, 1
		inc	ebx

loc_4040EC:				; CODE XREF: __sbh_alloc_new_group+13j
		test	eax, eax
		jge	short loc_4040E9
		mov	eax, ebx
		imul	eax, 204h
		lea	eax, [eax+esi+144h]
		push	3Fh
		mov	[ebp+var_8], eax
		pop	edx

loc_404105:				; CODE XREF: __sbh_alloc_new_group+3Bj
		mov	[eax+8], eax
		mov	[eax+4], eax
		add	eax, 8
		dec	edx
		jnz	short loc_404105
		push	4
		mov	edi, ebx
		push	1000h
		shl	edi, 0Fh
		add	edi, [ecx+0Ch]
		push	8000h
		push	edi
		call	ds:__imp__VirtualAlloc@16 ; __declspec(dllimport) VirtualAlloc(x,x,x,x)
		test	eax, eax
		jnz	short loc_404138
		or	eax, 0FFFFFFFFh
		jmp	loc_4041D5
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_404138:				; CODE XREF: __sbh_alloc_new_group+5Aj
		lea	edx, [edi+7000h]
		cmp	edi, edx
		mov	[ebp+var_4], edx
		ja	short loc_404188
		mov	ecx, edx
		sub	ecx, edi
		shr	ecx, 0Ch
		lea	eax, [edi+10h]
		inc	ecx

loc_404150:				; CODE XREF: __sbh_alloc_new_group+AFj
		or	dword ptr [eax-8], 0FFFFFFFFh
		or	dword ptr [eax+0FECh], 0FFFFFFFFh
		lea	edx, [eax+0FFCh]
		mov	[eax], edx
		lea	edx, [eax-1004h]
		mov	dword ptr [eax-4], 0FF0h
		mov	[eax+4], edx
		mov	dword ptr [eax+0FE8h], 0FF0h
		add	eax, 1000h
		dec	ecx
		jnz	short loc_404150
		mov	edx, [ebp+var_4]

loc_404188:				; CODE XREF: __sbh_alloc_new_group+6Fj
		mov	eax, [ebp+var_8]
		add	eax, 1F8h
		lea	ecx, [edi+0Ch]
		mov	[eax+4], ecx
		mov	[ecx+8], eax
		lea	ecx, [edx+0Ch]
		mov	[eax+8], ecx
		mov	[ecx+4], eax
		and	dword ptr [esi+ebx*4+44h], 0
		xor	edi, edi
		inc	edi
		mov	[esi+ebx*4+0C4h], edi
		mov	al, [esi+43h]
		mov	cl, al
		inc	cl
		test	al, al
		mov	eax, [ebp+arg_0]
		mov	[esi+43h], cl
		jnz	short loc_4041C5
		or	[eax+4], edi

loc_4041C5:				; CODE XREF: __sbh_alloc_new_group+ECj
		mov	edx, 80000000h
		mov	ecx, ebx
		shr	edx, cl
		not	edx
		and	[eax+8], edx
		mov	eax, ebx

loc_4041D5:				; CODE XREF: __sbh_alloc_new_group+5Fj
		pop	edi
		pop	esi
		pop	ebx
		leave
		retn
__sbh_alloc_new_group endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ

; Attributes: bp-based frame

__sbh_resize_block proc	near		; CODE XREF: realloc+63p

var_C		= dword	ptr -0Ch
var_8		= dword	ptr -8
var_4		= dword	ptr -4
arg_0		= dword	ptr  8
arg_4		= dword	ptr  0Ch
arg_8		= dword	ptr  10h

		push	ebp
		mov	ebp, esp
		sub	esp, 0Ch
		mov	ecx, [ebp+arg_0]
		mov	eax, [ecx+10h]
		push	ebx
		push	esi
		mov	esi, [ebp+arg_8]
		push	edi
		mov	edi, [ebp+arg_4]
		mov	edx, edi
		sub	edx, [ecx+0Ch]
		add	esi, 17h
		shr	edx, 0Fh
		mov	ecx, edx
		imul	ecx, 204h
		lea	ecx, [ecx+eax+144h]
		mov	[ebp+var_C], ecx
		mov	ecx, [edi-4]
		and	esi, 0FFFFFFF0h
		dec	ecx
		cmp	esi, ecx
		lea	edi, [ecx+edi-4]
		mov	ebx, [edi]
		mov	[ebp+arg_8], ecx
		mov	[ebp+var_4], ebx
		jle	loc_40437C
		test	bl, 1
		jnz	loc_404375
		add	ebx, ecx
		cmp	esi, ebx
		jg	loc_404375
		mov	ecx, [ebp+var_4]
		sar	ecx, 4
		dec	ecx
		cmp	ecx, 3Fh
		mov	[ebp+var_8], ecx
		jbe	short loc_40424F
		push	3Fh
		pop	ecx
		mov	[ebp+var_8], ecx

loc_40424F:				; CODE XREF: __sbh_resize_block+6Dj
		mov	ebx, [edi+4]
		cmp	ebx, [edi+8]
		jnz	short loc_40429A
		cmp	ecx, 20h
		mov	ebx, 80000000h
		jnb	short loc_40427B
		shr	ebx, cl
		mov	ecx, [ebp+var_8]
		lea	ecx, [ecx+eax+4]
		not	ebx
		and	[eax+edx*4+44h], ebx
		dec	byte ptr [ecx]
		jnz	short loc_40429A
		mov	ecx, [ebp+arg_0]
		and	[ecx], ebx
		jmp	short loc_40429A
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_40427B:				; CODE XREF: __sbh_resize_block+85j
		add	ecx, 0FFFFFFE0h
		shr	ebx, cl
		mov	ecx, [ebp+var_8]
		lea	ecx, [ecx+eax+4]
		not	ebx
		and	[eax+edx*4+0C4h], ebx
		dec	byte ptr [ecx]
		jnz	short loc_40429A
		mov	ecx, [ebp+arg_0]
		and	[ecx+4], ebx

loc_40429A:				; CODE XREF: __sbh_resize_block+7Bj
					; __sbh_resize_block+98j ...
		mov	ecx, [edi+8]
		mov	ebx, [edi+4]
		mov	[ecx+4], ebx
		mov	ecx, [edi+4]
		mov	edi, [edi+8]
		mov	[ecx+8], edi
		mov	ecx, [ebp+arg_8]
		sub	ecx, esi
		add	[ebp+var_4], ecx
		cmp	[ebp+var_4], 0
		jle	loc_404363
		mov	edi, [ebp+var_4]
		mov	ecx, [ebp+arg_4]
		sar	edi, 4
		dec	edi
		cmp	edi, 3Fh
		lea	ecx, [ecx+esi-4]
		jbe	short loc_4042D4
		push	3Fh
		pop	edi

loc_4042D4:				; CODE XREF: __sbh_resize_block+F5j
		mov	ebx, [ebp+var_C]
		lea	ebx, [ebx+edi*8]
		mov	[ebp+arg_8], ebx
		mov	ebx, [ebx+4]
		mov	[ecx+4], ebx
		mov	ebx, [ebp+arg_8]
		mov	[ecx+8], ebx
		mov	[ebx+4], ecx
		mov	ebx, [ecx+4]
		mov	[ebx+8], ecx
		mov	ebx, [ecx+4]
		cmp	ebx, [ecx+8]
		jnz	short loc_404351
		mov	cl, [edi+eax+4]
		mov	byte ptr [ebp+arg_8+3],	cl
		inc	cl
		cmp	edi, 20h
		mov	[edi+eax+4], cl
		jnb	short loc_404328
		cmp	byte ptr [ebp+arg_8+3],	0
		jnz	short loc_404320
		mov	ecx, edi
		mov	ebx, 80000000h
		shr	ebx, cl
		mov	ecx, [ebp+arg_0]
		or	[ecx], ebx

loc_404320:				; CODE XREF: __sbh_resize_block+136j
		lea	eax, [eax+edx*4+44h]
		mov	ecx, edi
		jmp	short loc_404348
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_404328:				; CODE XREF: __sbh_resize_block+130j
		cmp	byte ptr [ebp+arg_8+3],	0
		jnz	short loc_40433E
		lea	ecx, [edi-20h]
		mov	ebx, 80000000h
		shr	ebx, cl
		mov	ecx, [ebp+arg_0]
		or	[ecx+4], ebx

loc_40433E:				; CODE XREF: __sbh_resize_block+152j
		lea	eax, [eax+edx*4+0C4h]
		lea	ecx, [edi-20h]

loc_404348:				; CODE XREF: __sbh_resize_block+14Cj
		mov	edx, 80000000h
		shr	edx, cl
		or	[eax], edx

loc_404351:				; CODE XREF: __sbh_resize_block+11Ej
		mov	edx, [ebp+arg_4]
		mov	ecx, [ebp+var_4]
		lea	eax, [edx+esi-4]
		mov	[eax], ecx
		mov	[ecx+eax-4], ecx
		jmp	short loc_404366
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_404363:				; CODE XREF: __sbh_resize_block+DEj
		mov	edx, [ebp+arg_4]

loc_404366:				; CODE XREF: __sbh_resize_block+187j
		lea	eax, [esi+1]
		mov	[edx-4], eax
		mov	[edx+esi-8], eax
		jmp	loc_4044B1
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_404375:				; CODE XREF: __sbh_resize_block+50j
					; __sbh_resize_block+5Aj
		xor	eax, eax
		jmp	loc_4044B4
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_40437C:				; CODE XREF: __sbh_resize_block+47j
		jge	loc_4044B1
		mov	ebx, [ebp+arg_4]
		sub	[ebp+arg_8], esi
		lea	ecx, [esi+1]
		mov	[ebx-4], ecx
		lea	ebx, [ebx+esi-4]
		mov	esi, [ebp+arg_8]
		sar	esi, 4
		dec	esi
		cmp	esi, 3Fh
		mov	[ebp+arg_4], ebx
		mov	[ebx-4], ecx
		jbe	short loc_4043A7
		push	3Fh
		pop	esi

loc_4043A7:				; CODE XREF: __sbh_resize_block+1C8j
		test	byte ptr [ebp+var_4], 1
		jnz	loc_404431
		mov	esi, [ebp+var_4]
		sar	esi, 4
		dec	esi
		cmp	esi, 3Fh
		jbe	short loc_4043C0
		push	3Fh
		pop	esi

loc_4043C0:				; CODE XREF: __sbh_resize_block+1E1j
		mov	ecx, [edi+4]
		cmp	ecx, [edi+8]
		jnz	short loc_40440A
		cmp	esi, 20h
		mov	ebx, 80000000h
		jnb	short loc_4043EB
		mov	ecx, esi
		shr	ebx, cl
		lea	esi, [esi+eax+4]
		not	ebx
		and	[eax+edx*4+44h], ebx
		dec	byte ptr [esi]
		jnz	short loc_404407
		mov	ecx, [ebp+arg_0]
		and	[ecx], ebx
		jmp	short loc_404407
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_4043EB:				; CODE XREF: __sbh_resize_block+1F6j
		lea	ecx, [esi-20h]
		shr	ebx, cl
		lea	ecx, [esi+eax+4]
		not	ebx
		and	[eax+edx*4+0C4h], ebx
		dec	byte ptr [ecx]
		jnz	short loc_404407
		mov	ecx, [ebp+arg_0]
		and	[ecx+4], ebx

loc_404407:				; CODE XREF: __sbh_resize_block+208j
					; __sbh_resize_block+20Fj ...
		mov	ebx, [ebp+arg_4]

loc_40440A:				; CODE XREF: __sbh_resize_block+1ECj
		mov	ecx, [edi+8]
		mov	esi, [edi+4]
		mov	[ecx+4], esi
		mov	esi, [edi+8]
		mov	ecx, [edi+4]
		mov	[ecx+8], esi
		mov	esi, [ebp+arg_8]
		add	esi, [ebp+var_4]
		mov	[ebp+arg_8], esi
		sar	esi, 4
		dec	esi
		cmp	esi, 3Fh
		jbe	short loc_404431
		push	3Fh
		pop	esi

loc_404431:				; CODE XREF: __sbh_resize_block+1D1j
					; __sbh_resize_block+252j
		mov	ecx, [ebp+var_C]
		lea	ecx, [ecx+esi*8]
		mov	edi, [ecx+4]
		mov	[ebx+8], ecx
		mov	[ebx+4], edi
		mov	[ecx+4], ebx
		mov	ecx, [ebx+4]
		mov	[ecx+8], ebx
		mov	ecx, [ebx+4]
		cmp	ecx, [ebx+8]
		jnz	short loc_4044A8
		mov	cl, [esi+eax+4]
		mov	byte ptr [ebp+arg_4+3],	cl
		inc	cl
		cmp	esi, 20h
		mov	[esi+eax+4], cl
		jnb	short loc_40447F
		cmp	byte ptr [ebp+arg_4+3],	0
		jnz	short loc_404477
		mov	ecx, esi
		mov	edi, 80000000h
		shr	edi, cl
		mov	ecx, [ebp+arg_0]
		or	[ecx], edi

loc_404477:				; CODE XREF: __sbh_resize_block+28Dj
		lea	eax, [eax+edx*4+44h]
		mov	ecx, esi
		jmp	short loc_40449F
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_40447F:				; CODE XREF: __sbh_resize_block+287j
		cmp	byte ptr [ebp+arg_4+3],	0
		jnz	short loc_404495
		lea	ecx, [esi-20h]
		mov	edi, 80000000h
		shr	edi, cl
		mov	ecx, [ebp+arg_0]
		or	[ecx+4], edi

loc_404495:				; CODE XREF: __sbh_resize_block+2A9j
		lea	eax, [eax+edx*4+0C4h]
		lea	ecx, [esi-20h]

loc_40449F:				; CODE XREF: __sbh_resize_block+2A3j
		mov	edx, 80000000h
		shr	edx, cl
		or	[eax], edx

loc_4044A8:				; CODE XREF: __sbh_resize_block+275j
		mov	eax, [ebp+arg_8]
		mov	[ebx], eax
		mov	[eax+ebx-4], eax

loc_4044B1:				; CODE XREF: __sbh_resize_block+196j
					; __sbh_resize_block:loc_40437Cj
		xor	eax, eax
		inc	eax

loc_4044B4:				; CODE XREF: __sbh_resize_block+19Dj
		pop	edi
		pop	esi
		pop	ebx
		leave
		retn
__sbh_resize_block endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


__sbh_heapmin	proc near
		mov	eax, __sbh_pHeaderDefer
		test	eax, eax
		jz	locret_404589
		mov	ecx, __sbh_indGroupDefer
		push	4000h
		shl	ecx, 0Fh
		add	ecx, [eax+0Ch]
		push	8000h
		push	ecx
		call	ds:__imp__VirtualFree@12 ; __declspec(dllimport) VirtualFree(x,x,x)
		mov	ecx, __sbh_indGroupDefer
		mov	eax, __sbh_pHeaderDefer
		mov	edx, 80000000h
		shr	edx, cl
		or	[eax+8], edx
		mov	eax, __sbh_pHeaderDefer
		mov	eax, [eax+10h]
		mov	ecx, __sbh_indGroupDefer
		and	dword ptr [eax+ecx*4+0C4h], 0
		mov	eax, __sbh_pHeaderDefer
		mov	eax, [eax+10h]
		dec	byte ptr [eax+43h]
		mov	eax, __sbh_pHeaderDefer
		mov	ecx, [eax+10h]
		cmp	byte ptr [ecx+43h], 0
		jnz	short loc_404530
		and	dword ptr [eax+4], 0FFFFFFFEh
		mov	eax, __sbh_pHeaderDefer

loc_404530:				; CODE XREF: __sbh_heapmin+6Cj
		cmp	dword ptr [eax+8], 0FFFFFFFFh
		jnz	short loc_404582
		cmp	__sbh_cntHeaderList, 1
		jle	short loc_404582
		push	dword ptr [eax+10h]
		push	0
		push	_crtheap
		call	ds:__imp__HeapFree@12 ;	__declspec(dllimport) HeapFree(x,x,x)
		mov	eax, __sbh_cntHeaderList
		mov	edx, __sbh_pHeaderList
		lea	eax, [eax+eax*4]
		shl	eax, 2
		mov	ecx, eax
		mov	eax, __sbh_pHeaderDefer
		sub	ecx, eax
		lea	ecx, [ecx+edx-14h]
		push	ecx
		lea	ecx, [eax+14h]
		push	ecx
		push	eax
		call	memmove
		add	esp, 0Ch
		dec	__sbh_cntHeaderList

loc_404582:				; CODE XREF: __sbh_heapmin+7Bj
					; __sbh_heapmin+84j
		and	__sbh_pHeaderDefer, 0

locret_404589:				; CODE XREF: __sbh_heapmin+7j
		retn
__sbh_heapmin	endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ

; Attributes: bp-based frame

__sbh_heap_check proc near

var_138		= dword	ptr -138h
var_38		= dword	ptr -38h
var_34		= dword	ptr -34h
var_30		= dword	ptr -30h
var_2C		= dword	ptr -2Ch
var_28		= dword	ptr -28h
var_24		= dword	ptr -24h
var_20		= dword	ptr -20h
var_1C		= dword	ptr -1Ch
var_18		= dword	ptr -18h
var_14		= dword	ptr -14h
var_10		= dword	ptr -10h
var_C		= dword	ptr -0Ch
var_8		= dword	ptr -8
var_4		= dword	ptr -4

		push	ebp
		mov	ebp, esp
		sub	esp, 138h
		mov	eax, __sbh_cntHeaderList
		lea	eax, [eax+eax*4]
		shl	eax, 2
		push	eax
		push	__sbh_pHeaderList
		call	ds:__imp__IsBadWritePtr@8 ; __declspec(dllimport) IsBadWritePtr(x,x)
		test	eax, eax
		jz	short loc_4045B4
		or	eax, 0FFFFFFFFh
		leave
		retn
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_4045B4:				; CODE XREF: __sbh_heap_check+23j
		push	ebx
		mov	ebx, __sbh_pHeaderList
		push	esi
		push	edi
		xor	edi, edi
		cmp	__sbh_cntHeaderList, edi
		mov	[ebp+var_34], ebx
		mov	[ebp+var_1C], edi
		jg	short loc_4045D6

loc_4045CD:				; CODE XREF: __sbh_heap_check+2D4j
		xor	eax, eax

loc_4045CF:				; CODE XREF: __sbh_heap_check+2DCj
		pop	edi
		pop	esi
		pop	ebx
		leave
		retn
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_4045D4:				; CODE XREF: __sbh_heap_check+2CEj
		xor	edi, edi

loc_4045D6:				; CODE XREF: __sbh_heap_check+41j
		mov	esi, [ebx+10h]
		push	41C4h
		push	esi
		call	ds:__imp__IsBadWritePtr@8 ; __declspec(dllimport) IsBadWritePtr(x,x)
		test	eax, eax
		jnz	loc_404863
		mov	eax, [ebx+0Ch]
		mov	[ebp+var_8], eax
		lea	eax, [esi+144h]
		mov	[ebp+var_24], eax
		mov	eax, [ebx+8]
		add	esi, 0C4h
		mov	[ebp+var_20], eax
		mov	[ebp+var_10], edi
		mov	[ebp+var_14], edi
		mov	[ebp+var_C], edi
		mov	[ebp+var_38], esi
		jmp	short loc_404618
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_404616:				; CODE XREF: __sbh_heap_check+2A7j
		xor	edi, edi

loc_404618:				; CODE XREF: __sbh_heap_check+8Aj
		mov	[ebp+var_28], edi
		mov	[ebp+var_18], edi
		mov	[ebp+var_4], edi
		push	40h
		xor	eax, eax
		cmp	[ebp+var_20], eax
		pop	ecx
		lea	edi, [ebp+var_138]
		rep stosd
		jl	loc_4047FC
		push	8000h
		push	[ebp+var_8]
		call	ds:__imp__IsBadWritePtr@8 ; __declspec(dllimport) IsBadWritePtr(x,x)
		test	eax, eax
		jnz	loc_40486B
		mov	edx, [ebp+var_8]
		xor	ebx, ebx
		add	edx, 0FFCh

loc_404658:				; CODE XREF: __sbh_heap_check+15Cj
		cmp	dword ptr [edx-0FF4h], 0FFFFFFFFh
		lea	esi, [edx-0FF0h]
		jnz	loc_40487B
		cmp	dword ptr [edx], 0FFFFFFFFh
		jnz	loc_40487B

loc_404674:				; CODE XREF: __sbh_heap_check+14Aj
		mov	ecx, [esi]
		test	cl, 1
		mov	edi, ecx
		jz	short loc_40468F
		dec	ecx
		cmp	ecx, 400h
		jg	loc_40486F
		inc	[ebp+var_4]
		jmp	short loc_4046A6
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_40468F:				; CODE XREF: __sbh_heap_check+F1j
		mov	eax, ecx
		sar	eax, 4
		dec	eax
		cmp	eax, 3Fh
		jle	short loc_40469D
		push	3Fh
		pop	eax

loc_40469D:				; CODE XREF: __sbh_heap_check+10Ej
		lea	eax, [ebp+eax*4+var_138]
		inc	dword ptr [eax]

loc_4046A6:				; CODE XREF: __sbh_heap_check+103j
		cmp	ecx, 10h
		jl	loc_404877
		test	cl, 0Fh
		jnz	loc_404877
		cmp	ecx, 0FF0h
		jg	loc_404877
		lea	eax, [ecx+esi]
		cmp	[eax-4], edi
		jnz	loc_404873
		mov	esi, eax
		cmp	esi, edx
		jb	short loc_404674
		jnz	loc_404873
		add	edx, 1000h
		inc	ebx
		cmp	ebx, 8
		jl	loc_404658
		mov	eax, [ebp+var_4]
		mov	edi, [ebp+var_24]
		cmp	[edi], eax
		jnz	loc_40487F
		xor	esi, esi

loc_4046FC:				; CODE XREF: __sbh_heap_check+266j
		and	[ebp+var_4], 0
		lea	ebx, [edi+8]
		mov	eax, [ebx-4]
		cmp	eax, edi
		mov	[ebp+var_2C], edi
		mov	[ebp+var_30], ebx
		jz	loc_4047C6

loc_404714:				; CODE XREF: __sbh_heap_check+20Fj
		mov	ecx, [ebp+var_4]
		cmp	ecx, [ebp+esi*4+var_138]
		jz	short loc_40479F
		mov	ecx, [ebp+var_8]
		cmp	eax, ecx
		jb	loc_40488F
		add	ecx, 8000h
		cmp	eax, ecx
		jnb	loc_40488F
		mov	ecx, eax
		and	ecx, 0FFFFF000h
		add	ecx, 0Ch
		lea	edx, [ecx+0FF0h]
		cmp	ecx, edx
		jz	loc_404883

loc_404752:				; CODE XREF: __sbh_heap_check+1D8j
		cmp	ecx, eax
		jz	short loc_404764
		mov	ebx, [ecx]
		and	ebx, 0FFFFFFFEh
		add	ecx, ebx
		cmp	ecx, edx
		mov	ebx, [ebp+var_30]
		jnz	short loc_404752

loc_404764:				; CODE XREF: __sbh_heap_check+1CAj
		cmp	ecx, edx
		jz	loc_404883
		mov	ecx, [eax]
		sar	ecx, 4
		dec	ecx
		cmp	ecx, 3Fh
		jle	short loc_40477A
		push	3Fh
		pop	ecx

loc_40477A:				; CODE XREF: __sbh_heap_check+1EBj
		cmp	ecx, esi
		jnz	loc_404887
		mov	ecx, [ebp+var_2C]
		cmp	[eax+8], ecx
		jnz	loc_40488B
		inc	[ebp+var_4]
		mov	[ebp+var_2C], eax
		mov	eax, [eax+4]
		cmp	eax, edi
		jnz	loc_404714

loc_40479F:				; CODE XREF: __sbh_heap_check+194j
		cmp	[ebp+var_4], 0
		jz	short loc_4047C6
		cmp	esi, 20h
		mov	eax, 80000000h
		jge	short loc_4047BB
		mov	ecx, esi
		shr	eax, cl
		or	[ebp+var_28], eax
		or	[ebp+var_10], eax
		jmp	short loc_4047C6
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_4047BB:				; CODE XREF: __sbh_heap_check+223j
		lea	ecx, [esi-20h]
		shr	eax, cl
		or	[ebp+var_18], eax
		or	[ebp+var_14], eax

loc_4047C6:				; CODE XREF: __sbh_heap_check+184j
					; __sbh_heap_check+219j ...
		mov	eax, [ebp+var_2C]
		cmp	[eax+4], edi
		jnz	loc_404897
		mov	ecx, [ebp+var_4]
		cmp	ecx, [ebp+esi*4+var_138]
		jnz	loc_404897
		cmp	[ebx], eax
		jnz	loc_404893
		inc	esi
		cmp	esi, 40h
		mov	edi, ebx
		jl	loc_4046FC
		mov	esi, [ebp+var_38]
		mov	ebx, [ebp+var_34]

loc_4047FC:				; CODE XREF: __sbh_heap_check+A7j
		mov	eax, [ebp+var_28]
		cmp	eax, [esi-80h]
		jnz	loc_40489B
		mov	eax, [ebp+var_18]
		cmp	eax, [esi]
		jnz	loc_40489B
		add	[ebp+var_8], 8000h
		add	[ebp+var_24], 204h
		shl	[ebp+var_20], 1
		inc	[ebp+var_C]
		add	esi, 4
		cmp	[ebp+var_C], 20h
		mov	[ebp+var_38], esi
		jl	loc_404616
		mov	eax, [ebp+var_10]
		cmp	eax, [ebx]
		jnz	short loc_40489F
		mov	eax, [ebp+var_14]
		cmp	eax, [ebx+4]
		jnz	short loc_40489F
		add	ebx, 14h
		inc	[ebp+var_1C]
		mov	eax, [ebp+var_1C]
		cmp	eax, __sbh_cntHeaderList
		mov	[ebp+var_34], ebx
		jl	loc_4045D4
		jmp	loc_4045CD
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_404863:				; CODE XREF: __sbh_heap_check+5Dj
		push	0FFFFFFFEh

loc_404865:				; CODE XREF: __sbh_heap_check+2E3j
					; __sbh_heap_check+2E7j ...
		pop	eax
		jmp	loc_4045CF
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_40486B:				; CODE XREF: __sbh_heap_check+BDj
		push	0FFFFFFFCh
		jmp	short loc_404865
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_40486F:				; CODE XREF: __sbh_heap_check+FAj
		push	0FFFFFFFAh
		jmp	short loc_404865
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_404873:				; CODE XREF: __sbh_heap_check+140j
					; __sbh_heap_check+14Cj
		push	0FFFFFFF8h
		jmp	short loc_404865
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_404877:				; CODE XREF: __sbh_heap_check+11Fj
					; __sbh_heap_check+128j ...
		push	0FFFFFFF9h
		jmp	short loc_404865
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_40487B:				; CODE XREF: __sbh_heap_check+DBj
					; __sbh_heap_check+E4j
		push	0FFFFFFFBh
		jmp	short loc_404865
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_40487F:				; CODE XREF: __sbh_heap_check+16Aj
		push	0FFFFFFF7h
		jmp	short loc_404865
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_404883:				; CODE XREF: __sbh_heap_check+1C2j
					; __sbh_heap_check+1DCj
		push	0FFFFFFF5h
		jmp	short loc_404865
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_404887:				; CODE XREF: __sbh_heap_check+1F2j
		push	0FFFFFFF4h
		jmp	short loc_404865
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_40488B:				; CODE XREF: __sbh_heap_check+1FEj
		push	0FFFFFFF3h
		jmp	short loc_404865
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_40488F:				; CODE XREF: __sbh_heap_check+19Bj
					; __sbh_heap_check+1A9j
		push	0FFFFFFF6h
		jmp	short loc_404865
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_404893:				; CODE XREF: __sbh_heap_check+25Aj
		push	0FFFFFFF1h
		jmp	short loc_404865
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_404897:				; CODE XREF: __sbh_heap_check+242j
					; __sbh_heap_check+252j
		push	0FFFFFFF2h
		jmp	short loc_404865
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_40489B:				; CODE XREF: __sbh_heap_check+278j
					; __sbh_heap_check+283j
		push	0FFFFFFF0h
		jmp	short loc_404865
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_40489F:				; CODE XREF: __sbh_heap_check+2B2j
					; __sbh_heap_check+2BAj
		push	0FFFFFFEFh
		jmp	short loc_404865
__sbh_heap_check endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


_set_sbh_threshold proc	near

arg_0		= dword	ptr  4

		mov	eax, __active_heap
		cmp	eax, 3
		jnz	short loc_4048C4
		mov	eax, [esp+arg_0]
		cmp	eax, 3F8h
		ja	short loc_4048C1
		mov	__sbh_threshold, eax
		xor	eax, eax
		inc	eax
		retn
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_4048C1:				; CODE XREF: _set_sbh_threshold+13j
		xor	eax, eax
		retn
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_4048C4:				; CODE XREF: _set_sbh_threshold+8j
		cmp	eax, 1
		push	esi
		jnz	short loc_4048FA
		mov	esi, [esp+4+arg_0]
		test	esi, esi
		jbe	short loc_4048FA
		cmp	esi, 3F8h
		ja	short loc_4048FA
		push	esi
		call	__sbh_heap_init
		test	eax, eax
		pop	ecx
		jz	short loc_4048FA
		xor	eax, eax
		mov	__sbh_threshold, esi
		mov	__active_heap, 3
		inc	eax
		pop	esi
		retn
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_4048FA:				; CODE XREF: _set_sbh_threshold+25j
					; _set_sbh_threshold+2Dj ...
		xor	eax, eax
		pop	esi
		retn
_set_sbh_threshold endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ

; Attributes: bp-based frame

__sbh_alloc_block proc near		; CODE XREF: _heap_alloc+17p
					; calloc+32p ...

var_14		= dword	ptr -14h
var_10		= dword	ptr -10h
var_C		= dword	ptr -0Ch
var_8		= dword	ptr -8
var_4		= dword	ptr -4
arg_0		= dword	ptr  8

		push	ebp
		mov	ebp, esp
		sub	esp, 14h
		mov	ecx, [ebp+arg_0]
		mov	eax, __sbh_cntHeaderList
		mov	edx, __sbh_pHeaderList
		add	ecx, 17h
		and	ecx, 0FFFFFFF0h
		push	ebx
		mov	[ebp+var_10], ecx
		sar	ecx, 4
		push	esi
		lea	eax, [eax+eax*4]
		push	edi
		dec	ecx
		cmp	ecx, 20h
		lea	edi, [edx+eax*4]
		mov	[ebp+var_4], edi
		jge	short loc_40493B
		or	esi, 0FFFFFFFFh
		shr	esi, cl
		or	[ebp+var_8], 0FFFFFFFFh
		jmp	short loc_404948
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_40493B:				; CODE XREF: __sbh_alloc_block+30j
		add	ecx, 0FFFFFFE0h
		or	eax, 0FFFFFFFFh
		xor	esi, esi
		shr	eax, cl
		mov	[ebp+var_8], eax

loc_404948:				; CODE XREF: __sbh_alloc_block+3Bj
		mov	eax, __sbh_pHeaderScan
		mov	ebx, eax
		mov	[ebp+var_C], esi
		cmp	ebx, edi
		jmp	short loc_40496A
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_404956:				; CODE XREF: __sbh_alloc_block+6Fj
		mov	ecx, [ebx+4]
		mov	edi, [ebx]
		and	ecx, [ebp+var_8]
		and	edi, esi
		or	ecx, edi
		jnz	short loc_40496F
		add	ebx, 14h
		cmp	ebx, [ebp+var_4]

loc_40496A:				; CODE XREF: __sbh_alloc_block+56j
		mov	[ebp+arg_0], ebx
		jb	short loc_404956

loc_40496F:				; CODE XREF: __sbh_alloc_block+64j
		cmp	ebx, [ebp+var_4]
		jnz	short loc_404998
		mov	ebx, edx
		jmp	short loc_404989
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_404978:				; CODE XREF: __sbh_alloc_block+90j
		mov	ecx, [ebx+4]
		mov	edi, [ebx]
		and	ecx, [ebp+var_8]
		and	edi, esi
		or	ecx, edi
		jnz	short loc_404990
		add	ebx, 14h

loc_404989:				; CODE XREF: __sbh_alloc_block+78j
		cmp	ebx, eax
		mov	[ebp+arg_0], ebx
		jb	short loc_404978

loc_404990:				; CODE XREF: __sbh_alloc_block+86j
		cmp	ebx, eax
		jz	loc_404A2C

loc_404998:				; CODE XREF: __sbh_alloc_block+74j
					; __sbh_alloc_block+170j
		mov	__sbh_pHeaderScan, ebx
		mov	eax, [ebx+10h]
		mov	edx, [eax]
		cmp	edx, 0FFFFFFFFh
		mov	[ebp+var_4], edx
		jz	short loc_4049BF
		mov	ecx, [eax+edx*4+0C4h]
		mov	edi, [eax+edx*4+44h]
		and	ecx, [ebp+var_8]
		and	edi, esi
		or	ecx, edi
		jnz	short loc_4049F5

loc_4049BF:				; CODE XREF: __sbh_alloc_block+ABj
		mov	edx, [eax+0C4h]
		and	edx, [ebp+var_8]
		and	[ebp+var_4], 0
		lea	ecx, [eax+44h]
		mov	esi, [ecx]
		and	esi, [ebp+var_C]
		or	edx, esi
		mov	esi, [ebp+var_C]
		jnz	short loc_4049F2

loc_4049DB:				; CODE XREF: __sbh_alloc_block+F2j
		mov	edx, [ecx+84h]
		and	edx, [ebp+var_8]
		inc	[ebp+var_4]
		add	ecx, 4
		mov	edi, [ecx]
		and	edi, esi
		or	edx, edi
		jz	short loc_4049DB

loc_4049F2:				; CODE XREF: __sbh_alloc_block+DBj
		mov	edx, [ebp+var_4]

loc_4049F5:				; CODE XREF: __sbh_alloc_block+BFj
		mov	ecx, edx
		imul	ecx, 204h
		lea	ecx, [ecx+eax+144h]
		mov	[ebp+var_C], ecx
		mov	ecx, [eax+edx*4+44h]
		xor	edi, edi
		and	ecx, esi
		jnz	short loc_404A7E
		mov	ecx, [eax+edx*4+0C4h]
		and	ecx, [ebp+var_8]
		push	20h
		pop	edi
		jmp	short loc_404A7E
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_404A20:				; CODE XREF: __sbh_alloc_block+131j
		cmp	dword ptr [ebx+8], 0
		jnz	short loc_404A31
		add	ebx, 14h
		mov	[ebp+arg_0], ebx

loc_404A2C:				; CODE XREF: __sbh_alloc_block+94j
		cmp	ebx, [ebp+var_4]
		jb	short loc_404A20

loc_404A31:				; CODE XREF: __sbh_alloc_block+126j
		cmp	ebx, [ebp+var_4]
		jnz	short loc_404A5C
		mov	ebx, edx
		jmp	short loc_404A43
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_404A3A:				; CODE XREF: __sbh_alloc_block+14Aj
		cmp	dword ptr [ebx+8], 0
		jnz	short loc_404A4A
		add	ebx, 14h

loc_404A43:				; CODE XREF: __sbh_alloc_block+13Aj
		cmp	ebx, eax
		mov	[ebp+arg_0], ebx
		jb	short loc_404A3A

loc_404A4A:				; CODE XREF: __sbh_alloc_block+140j
		cmp	ebx, eax
		jnz	short loc_404A5C
		call	__sbh_alloc_new_region
		mov	ebx, eax
		test	ebx, ebx
		mov	[ebp+arg_0], ebx
		jz	short loc_404A74

loc_404A5C:				; CODE XREF: __sbh_alloc_block+136j
					; __sbh_alloc_block+14Ej
		push	ebx
		call	__sbh_alloc_new_group
		pop	ecx
		mov	ecx, [ebx+10h]
		mov	[ecx], eax
		mov	eax, [ebx+10h]
		cmp	dword ptr [eax], 0FFFFFFFFh
		jnz	loc_404998

loc_404A74:				; CODE XREF: __sbh_alloc_block+15Cj
		xor	eax, eax
		jmp	loc_404BF5
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_404A7B:				; CODE XREF: __sbh_alloc_block+182j
		shl	ecx, 1
		inc	edi

loc_404A7E:				; CODE XREF: __sbh_alloc_block+111j
					; __sbh_alloc_block+120j
		test	ecx, ecx
		jge	short loc_404A7B
		mov	ecx, [ebp+var_C]
		mov	edx, [ecx+edi*8+4]
		mov	ecx, [edx]
		sub	ecx, [ebp+var_10]
		mov	esi, ecx
		sar	esi, 4
		dec	esi
		cmp	esi, 3Fh
		mov	[ebp+var_8], ecx
		jle	short loc_404A9F
		push	3Fh
		pop	esi

loc_404A9F:				; CODE XREF: __sbh_alloc_block+19Cj
		cmp	esi, edi
		jz	loc_404BA8
		mov	ecx, [edx+4]
		cmp	ecx, [edx+8]
		jnz	short loc_404B0B
		cmp	edi, 20h
		mov	ebx, 80000000h
		jge	short loc_404ADF
		mov	ecx, edi
		shr	ebx, cl
		mov	ecx, [ebp+var_4]
		lea	edi, [eax+edi+4]
		not	ebx
		mov	[ebp+var_14], ebx
		and	ebx, [eax+ecx*4+44h]
		mov	[eax+ecx*4+44h], ebx
		dec	byte ptr [edi]
		jnz	short loc_404B08
		mov	ecx, [ebp+var_14]
		mov	ebx, [ebp+arg_0]
		and	[ebx], ecx
		jmp	short loc_404B0B
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_404ADF:				; CODE XREF: __sbh_alloc_block+1B9j
		lea	ecx, [edi-20h]
		shr	ebx, cl
		mov	ecx, [ebp+var_4]
		lea	ecx, [eax+ecx*4+0C4h]
		lea	edi, [eax+edi+4]
		not	ebx
		and	[ecx], ebx
		dec	byte ptr [edi]
		mov	[ebp+var_14], ebx
		jnz	short loc_404B08
		mov	ebx, [ebp+arg_0]
		mov	ecx, [ebp+var_14]
		and	[ebx+4], ecx
		jmp	short loc_404B0B
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_404B08:				; CODE XREF: __sbh_alloc_block+1D5j
					; __sbh_alloc_block+1FDj
		mov	ebx, [ebp+arg_0]

loc_404B0B:				; CODE XREF: __sbh_alloc_block+1AFj
					; __sbh_alloc_block+1DFj ...
		cmp	[ebp+var_8], 0
		mov	ecx, [edx+8]
		mov	edi, [edx+4]
		mov	[ecx+4], edi
		mov	ecx, [edx+4]
		mov	edi, [edx+8]
		mov	[ecx+8], edi
		jz	loc_404BB4
		mov	ecx, [ebp+var_C]
		lea	ecx, [ecx+esi*8]
		mov	edi, [ecx+4]
		mov	[edx+8], ecx
		mov	[edx+4], edi
		mov	[ecx+4], edx
		mov	ecx, [edx+4]
		mov	[ecx+8], edx
		mov	ecx, [edx+4]
		cmp	ecx, [edx+8]
		jnz	short loc_404BA5
		mov	cl, [esi+eax+4]
		mov	byte ptr [ebp+arg_0+3],	cl
		inc	cl
		cmp	esi, 20h
		mov	[esi+eax+4], cl
		jge	short loc_404B7C
		cmp	byte ptr [ebp+arg_0+3],	0
		jnz	short loc_404B6A
		mov	edi, 80000000h
		mov	ecx, esi
		shr	edi, cl
		or	[ebx], edi

loc_404B6A:				; CODE XREF: __sbh_alloc_block+25Fj
		mov	ecx, esi
		mov	edi, 80000000h
		shr	edi, cl
		mov	ecx, [ebp+var_4]
		or	[eax+ecx*4+44h], edi
		jmp	short loc_404BA5
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_404B7C:				; CODE XREF: __sbh_alloc_block+259j
		cmp	byte ptr [ebp+arg_0+3],	0
		jnz	short loc_404B8F
		lea	ecx, [esi-20h]
		mov	edi, 80000000h
		shr	edi, cl
		or	[ebx+4], edi

loc_404B8F:				; CODE XREF: __sbh_alloc_block+282j
		mov	ecx, [ebp+var_4]
		lea	edi, [eax+ecx*4+0C4h]
		lea	ecx, [esi-20h]
		mov	esi, 80000000h
		shr	esi, cl
		or	[edi], esi

loc_404BA5:				; CODE XREF: __sbh_alloc_block+247j
					; __sbh_alloc_block+27Cj
		mov	ecx, [ebp+var_8]

loc_404BA8:				; CODE XREF: __sbh_alloc_block+1A3j
		test	ecx, ecx
		jz	short loc_404BB7
		mov	[edx], ecx
		mov	[ecx+edx-4], ecx
		jmp	short loc_404BB7
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_404BB4:				; CODE XREF: __sbh_alloc_block+223j
		mov	ecx, [ebp+var_8]

loc_404BB7:				; CODE XREF: __sbh_alloc_block+2ACj
					; __sbh_alloc_block+2B4j
		mov	esi, [ebp+var_10]
		add	edx, ecx
		lea	ecx, [esi+1]
		mov	[edx], ecx
		mov	[edx+esi-4], ecx
		mov	esi, [ebp+var_C]
		mov	ecx, [esi]
		test	ecx, ecx
		lea	edi, [ecx+1]
		mov	[esi], edi
		jnz	short loc_404BED
		cmp	ebx, __sbh_pHeaderDefer
		jnz	short loc_404BED
		mov	ecx, [ebp+var_4]
		cmp	ecx, __sbh_indGroupDefer
		jnz	short loc_404BED
		and	__sbh_pHeaderDefer, 0

loc_404BED:				; CODE XREF: __sbh_alloc_block+2D3j
					; __sbh_alloc_block+2DBj ...
		mov	ecx, [ebp+var_4]
		mov	[eax], ecx
		lea	eax, [edx+4]

loc_404BF5:				; CODE XREF: __sbh_alloc_block+178j
		pop	edi
		pop	esi
		pop	ebx
		leave
		retn
__sbh_alloc_block endp

; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		align 4

; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ

; Attributes: bp-based frame

__global_unwind2 proc near		; CODE XREF: _except_handler3+70p

arg_0		= dword	ptr  8

		push	ebp
		mov	ebp, esp
		push	ebx
		push	esi
		push	edi
		push	ebp
		push	0
		push	0
		push	offset loc_404C14
		push	[ebp+arg_0]
		call	_RtlUnwind@16	; RtlUnwind(x,x,x,x)

loc_404C14:				; DATA XREF: __global_unwind2+Bo
		pop	ebp
		pop	edi
		pop	esi
		pop	ebx
		mov	esp, ebp
		pop	ebp
		retn
__global_unwind2 endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


__unwind_handler proc near		; DATA XREF: __local_unwind2+Ao
					; __abnormal_termination+9o

arg_0		= dword	ptr  4
arg_4		= dword	ptr  8
arg_C		= dword	ptr  10h

		mov	ecx, [esp+arg_0]
		test	dword ptr [ecx+4], 6
		mov	eax, 1
		jz	short locret_404C3D
		mov	eax, [esp+arg_4]
		mov	edx, [esp+arg_C]
		mov	[edx], eax
		mov	eax, 3

locret_404C3D:				; CODE XREF: __unwind_handler+10j
		retn
__unwind_handler endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


__local_unwind2	proc near		; CODE XREF: _except_handler3+7Dp
					; _except_handler3+D0p	...

var_8		= dword	ptr -8
arg_0		= dword	ptr  10h
arg_4		= dword	ptr  14h

		push	ebx
		push	esi
		push	edi
		mov	eax, [esp+arg_0]
		push	eax
		push	0FFFFFFFEh
		push	offset __unwind_handler
		push	large dword ptr	fs:0
		mov	large fs:0, esp

loc_404C5B:				; CODE XREF: __local_unwind2:__NLG_Return2j
		mov	eax, [esp+10h+arg_0]
		mov	ebx, [eax+8]
		mov	esi, [eax+0Ch]
		cmp	esi, 0FFFFFFFFh
		jz	short loc_404C98
		cmp	esi, [esp+10h+arg_4]
		jz	short loc_404C98
		lea	esi, [esi+esi*2]
		mov	ecx, [ebx+esi*4]
		mov	[esp+10h+var_8], ecx
		mov	[eax+0Ch], ecx
		cmp	dword ptr [ebx+esi*4+4], 0
		jnz	short __NLG_Return2
		push	101h
		mov	eax, [ebx+esi*4+8]
		call	_NLG_Notify
		call	dword ptr [ebx+esi*4+8]

__NLG_Return2:				; CODE XREF: __local_unwind2+44j
		jmp	short loc_404C5B
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_404C98:				; CODE XREF: __local_unwind2+2Aj
					; __local_unwind2+30j
		pop	large dword ptr	fs:0
		add	esp, 0Ch
		pop	edi
		pop	esi
		pop	ebx
		retn
__local_unwind2	endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


; int _abnormal_termination(void)
__abnormal_termination proc near
		xor	eax, eax
		mov	ecx, large fs:0
		cmp	dword ptr [ecx+4], offset __unwind_handler
		jnz	short locret_404CC8
		mov	edx, [ecx+0Ch]
		mov	edx, [edx+0Ch]
		cmp	[ecx+8], edx
		jnz	short locret_404CC8
		mov	eax, 1

locret_404CC8:				; CODE XREF: __abnormal_termination+10j
					; __abnormal_termination+1Bj
		retn
__abnormal_termination endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


_NLG_Notify1	proc near
		push	ebx
		push	ecx
		mov	ebx, offset __NLG_Destination
		jmp	short loc_404CDC
_NLG_Notify1	endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


_NLG_Notify	proc near		; CODE XREF: _except_handler3+8Ep
					; __local_unwind2+4Fp
		push	ebx
		push	ecx
		mov	ebx, offset __NLG_Destination
		mov	ecx, [ebp+8]

loc_404CDC:				; CODE XREF: _NLG_Notify1+7j
		mov	[ebx+8], ecx
		mov	[ebx+4], eax
		mov	[ebx+0Ch], ebp

__NLG_Dispatch:
		pop	ecx
		pop	ebx
		retn	4
_NLG_Notify	endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ

; Attributes: bp-based frame

_ValidateEH3RN	proc near		; CODE XREF: _except_handler3+34p

var_20		= dword	ptr -20h
var_1C		= dword	ptr -1Ch
var_C		= byte ptr -0Ch
var_8		= dword	ptr -8
var_4		= dword	ptr -4
arg_0		= dword	ptr  8

		push	ebp
		mov	ebp, esp
		sub	esp, 20h
		push	ebx
		push	esi
		mov	esi, [ebp+arg_0]
		mov	ebx, [esi+8]
		test	bl, 3
		jnz	short loc_404D18
		mov	eax, large fs:18h
		mov	[ebp+arg_0], eax
		mov	eax, [ebp+arg_0]
		mov	ecx, [eax+8]
		cmp	ebx, ecx
		mov	[ebp+var_4], ecx
		jb	short loc_404D1F
		cmp	ebx, [eax+4]
		jnb	short loc_404D1F

loc_404D18:				; CODE XREF: _ValidateEH3RN+11j
		xor	eax, eax
		jmp	loc_404F0F
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_404D1F:				; CODE XREF: _ValidateEH3RN+27j
					; _ValidateEH3RN+2Cj
		push	edi
		mov	edi, [esi+0Ch]
		cmp	edi, 0FFFFFFFFh
		jnz	short loc_404D30

loc_404D28:				; CODE XREF: _ValidateEH3RN+139j
					; _ValidateEH3RN+19Cj ...
		xor	eax, eax
		inc	eax
		jmp	loc_404F0E
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_404D30:				; CODE XREF: _ValidateEH3RN+3Cj
		xor	edx, edx
		mov	[ebp+arg_0], edx
		mov	eax, ebx

loc_404D37:				; CODE XREF: _ValidateEH3RN+6Bj
		mov	ecx, [eax]
		cmp	ecx, 0FFFFFFFFh
		jz	short loc_404D46
		cmp	ecx, edx
		jnb	loc_404E8B

loc_404D46:				; CODE XREF: _ValidateEH3RN+52j
		cmp	dword ptr [eax+4], 0
		jz	short loc_404D4F
		inc	[ebp+arg_0]

loc_404D4F:				; CODE XREF: _ValidateEH3RN+60j
		inc	edx
		add	eax, 0Ch
		cmp	edx, edi
		jbe	short loc_404D37
		cmp	[ebp+arg_0], 0
		jz	short loc_404D71
		mov	eax, [esi-8]
		cmp	eax, [ebp+var_4]
		jb	loc_404E8B
		cmp	eax, esi
		jnb	loc_404E8B

loc_404D71:				; CODE XREF: _ValidateEH3RN+71j
		mov	eax, nValidPages
		mov	edi, ebx
		and	edi, 0FFFFF000h
		xor	esi, esi
		test	eax, eax
		jle	short loc_404D96

loc_404D84:				; CODE XREF: _ValidateEH3RN+AAj
		cmp	rgValidPages[esi*4], edi
		jz	loc_404E8F
		inc	esi
		cmp	esi, eax
		jl	short loc_404D84

loc_404D96:				; CODE XREF: _ValidateEH3RN+98j
		push	1Ch
		lea	eax, [ebp+var_20]
		push	eax
		push	ebx
		call	ds:__imp__VirtualQuery@12 ; __declspec(dllimport) VirtualQuery(x,x,x)
		test	eax, eax
		jz	loc_404F0B
		cmp	[ebp+var_8], 1000000h
		jnz	loc_404F0B
		test	[ebp+var_C], 0CCh
		jz	short loc_404E14
		mov	ecx, [ebp+var_1C]
		cmp	word ptr [ecx],	5A4Dh
		jnz	loc_404F0B
		mov	eax, [ecx+3Ch]
		add	eax, ecx
		cmp	dword ptr [eax], 4550h
		jnz	loc_404F0B
		cmp	word ptr [eax+18h], 10Bh
		jnz	loc_404F0B
		sub	ebx, ecx
		cmp	word ptr [eax+6], 0
		movzx	ecx, word ptr [eax+14h]
		lea	ecx, [ecx+eax+18h]
		jbe	loc_404F0B
		mov	eax, [ecx+0Ch]
		cmp	ebx, eax
		jb	short loc_404E14
		mov	edx, [ecx+8]
		add	edx, eax
		cmp	ebx, edx
		jnb	short loc_404E14
		test	byte ptr [ecx+27h], 80h
		jnz	short loc_404E8B

loc_404E14:				; CODE XREF: _ValidateEH3RN+D2j
					; _ValidateEH3RN+119j ...
		push	1
		push	offset lModifying
		call	ds:__imp__InterlockedExchange@8	; __declspec(dllimport)	InterlockedExchange(x,x)
		test	eax, eax
		jnz	loc_404D28
		mov	ecx, nValidPages
		test	ecx, ecx
		mov	edx, ecx
		jle	short loc_404E48
		lea	eax, ds:40D5BCh[ecx*4]

loc_404E3C:				; CODE XREF: _ValidateEH3RN+15Cj
		cmp	[eax], edi
		jz	short loc_404E48
		dec	edx
		sub	eax, 4
		test	edx, edx
		jg	short loc_404E3C

loc_404E48:				; CODE XREF: _ValidateEH3RN+149j
					; _ValidateEH3RN+154j
		test	edx, edx
		jnz	short loc_404E79
		push	0Fh
		pop	ebx
		cmp	ecx, ebx
		jg	short loc_404E55
		mov	ebx, ecx

loc_404E55:				; CODE XREF: _ValidateEH3RN+167j
		xor	edx, edx
		test	ebx, ebx
		jl	short loc_404E6D

loc_404E5B:				; CODE XREF: _ValidateEH3RN+181j
		lea	eax, ds:40D5C0h[edx*4]
		mov	esi, [eax]
		inc	edx
		cmp	edx, ebx
		mov	[eax], edi
		mov	edi, esi
		jle	short loc_404E5B

loc_404E6D:				; CODE XREF: _ValidateEH3RN+16Fj
		cmp	ecx, 10h
		jge	short loc_404E79
		inc	ecx
		mov	nValidPages, ecx

loc_404E79:				; CODE XREF: _ValidateEH3RN+160j
					; _ValidateEH3RN+186j
		push	0
		push	offset lModifying
		call	ds:__imp__InterlockedExchange@8	; __declspec(dllimport)	InterlockedExchange(x,x)
		jmp	loc_404D28
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_404E8B:				; CODE XREF: _ValidateEH3RN+56j
					; _ValidateEH3RN+79j ...
		xor	eax, eax
		jmp	short loc_404F0E
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_404E8F:				; CODE XREF: _ValidateEH3RN+A1j
		test	esi, esi
		jle	loc_404D28
		mov	ebx, ds:__imp__InterlockedExchange@8 ; __declspec(dllimport) InterlockedExchange(x,x)
		push	1
		push	offset lModifying
		call	ebx ; __declspec(dllimport) InterlockedExchange(x,x) ; __declspec(dllimport) InterlockedExchange(x,x)
		test	eax, eax
		jnz	loc_404D28
		cmp	rgValidPages[esi*4], edi
		jz	short loc_404EE5
		mov	eax, nValidPages
		lea	esi, [eax-1]
		test	esi, esi
		jl	short loc_404ED3

loc_404EC3:				; CODE XREF: _ValidateEH3RN+1E3j
		cmp	rgValidPages[esi*4], edi
		jz	short loc_404ECF
		dec	esi
		jns	short loc_404EC3

loc_404ECF:				; CODE XREF: _ValidateEH3RN+1E0j
		test	esi, esi
		jge	short loc_404EE3

loc_404ED3:				; CODE XREF: _ValidateEH3RN+1D7j
		cmp	eax, 10h
		jge	short loc_404EDE
		inc	eax
		mov	nValidPages, eax

loc_404EDE:				; CODE XREF: _ValidateEH3RN+1ECj
		lea	esi, [eax-1]
		jmp	short loc_404EE5
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_404EE3:				; CODE XREF: _ValidateEH3RN+1E7j
		jz	short loc_404EFD

loc_404EE5:				; CODE XREF: _ValidateEH3RN+1CBj
					; _ValidateEH3RN+1F7j
		xor	ecx, ecx
		test	esi, esi
		jl	short loc_404EFD

loc_404EEB:				; CODE XREF: _ValidateEH3RN+211j
		lea	eax, ds:40D5C0h[ecx*4]
		mov	edx, [eax]
		inc	ecx
		cmp	ecx, esi
		mov	[eax], edi
		mov	edi, edx
		jle	short loc_404EEB

loc_404EFD:				; CODE XREF: _ValidateEH3RN:loc_404EE3j
					; _ValidateEH3RN+1FFj
		push	0
		push	offset lModifying
		call	ebx ; __declspec(dllimport) InterlockedExchange(x,x) ; __declspec(dllimport) InterlockedExchange(x,x)
		jmp	loc_404D28
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_404F0B:				; CODE XREF: _ValidateEH3RN+BBj
					; _ValidateEH3RN+C8j ...
		or	eax, 0FFFFFFFFh

loc_404F0E:				; CODE XREF: _ValidateEH3RN+41j
					; _ValidateEH3RN+1A3j
		pop	edi

loc_404F0F:				; CODE XREF: _ValidateEH3RN+30j
		pop	esi
		pop	ebx
		leave
		retn
_ValidateEH3RN	endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


_set_new_handler proc near

arg_0		= dword	ptr  4

		mov	ecx, [esp+arg_0]
		mov	eax, _pnhHeap
		mov	_pnhHeap, ecx
		retn
_set_new_handler endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


_query_new_handler proc	near
		mov	eax, _pnhHeap
		retn
_query_new_handler endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


_callnewh	proc near		; CODE XREF: _nh_malloc+1Fp calloc+5Dp ...

arg_0		= dword	ptr  4

		mov	eax, _pnhHeap
		test	eax, eax
		jz	short loc_404F41
		push	[esp+arg_0]
		call	eax
		test	eax, eax
		pop	ecx
		jz	short loc_404F41
		xor	eax, eax
		inc	eax
		retn
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_404F41:				; CODE XREF: _callnewh+7j
					; _callnewh+12j
		xor	eax, eax
		retn
_callnewh	endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


_write		proc near		; CODE XREF: _flush+2Cp _flsbuf+98p ...

var_39C		= dword	ptr -39Ch

		push	ebp
		lea	ebp, [esp+var_39C]
		sub	esp, 41Ch
		mov	ecx, [ebp+3A4h]
		cmp	ecx, _nhandle
		mov	eax, __security_cookie
		push	ebx
		push	esi
		mov	[ebp+398h], eax
		push	edi
		jnb	loc_40510A
		mov	eax, ecx
		sar	eax, 5
		lea	ebx, ds:40D980h[eax*4]
		mov	eax, [ebx]
		mov	esi, ecx
		and	esi, 1Fh
		shl	esi, 3
		mov	al, [eax+esi+4]
		test	al, 1
		mov	[ebp-80h], ebx
		jz	loc_40510A
		xor	edi, edi
		cmp	[ebp+3ACh], edi
		mov	[ebp-74h], edi
		mov	[ebp-7Ch], edi
		jnz	short loc_404FAE

loc_404FA7:				; CODE XREF: _write+1A9j
		xor	eax, eax
		jmp	loc_40511E
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_404FAE:				; CODE XREF: _write+61j
		test	al, 20h
		jz	short loc_404FBF
		push	2
		push	edi
		push	edi
		push	ecx
		call	_lseeki64
		add	esp, 10h

loc_404FBF:				; CODE XREF: _write+6Cj
		mov	eax, [ebx]
		add	eax, esi
		test	byte ptr [eax+4], 80h
		jz	loc_405094
		cmp	[ebp+3ACh], edi
		mov	eax, [ebp+3A8h]
		mov	[ebp-70h], eax
		mov	[ebp+3A4h], edi
		jbe	loc_4050DB

loc_404FE8:				; CODE XREF: _write+119j
		mov	ecx, [ebp-70h]
		sub	ecx, [ebp+3A8h]
		lea	eax, [ebp-6Ch]

loc_404FF4:				; CODE XREF: _write+DBj
		cmp	ecx, [ebp+3ACh]
		jnb	short loc_405021
		mov	edx, [ebp-70h]
		inc	dword ptr [ebp-70h]
		mov	dl, [edx]
		inc	ecx
		cmp	dl, 0Ah
		jnz	short loc_405012
		inc	dword ptr [ebp-7Ch]
		mov	byte ptr [eax],	0Dh
		inc	eax
		inc	edi

loc_405012:				; CODE XREF: _write+C4j
		mov	ebx, [ebp-80h]
		mov	[eax], dl
		inc	eax
		inc	edi
		cmp	edi, 400h
		jl	short loc_404FF4

loc_405021:				; CODE XREF: _write+B6j
		mov	edi, eax
		lea	eax, [ebp-6Ch]
		sub	edi, eax
		push	0
		lea	eax, [ebp-78h]
		push	eax
		push	edi
		lea	eax, [ebp-6Ch]
		push	eax
		mov	eax, [ebx]
		push	dword ptr [eax+esi]
		call	ds:__imp__WriteFile@20 ; __declspec(dllimport) WriteFile(x,x,x,x,x)
		test	eax, eax
		jz	short loc_405061
		mov	eax, [ebp-78h]
		add	[ebp-74h], eax
		cmp	eax, edi
		jl	short loc_40506D
		mov	eax, [ebp-70h]
		sub	eax, [ebp+3A8h]
		xor	edi, edi
		cmp	eax, [ebp+3ACh]
		jb	short loc_404FE8
		jmp	short loc_40506F
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_405061:				; CODE XREF: _write+FCj
		call	ds:__imp__GetLastError@0 ; __declspec(dllimport) GetLastError()
		mov	[ebp+3A4h], eax

loc_40506D:				; CODE XREF: _write+106j
		xor	edi, edi

loc_40506F:				; CODE XREF: _write+11Bj _write+179j ...
		mov	eax, [ebp-74h]
		cmp	eax, edi
		jnz	loc_405105
		cmp	[ebp+3A4h], edi
		jz	short loc_4050DB
		push	5
		pop	eax
		cmp	[ebp+3A4h], eax
		jnz	short loc_4050CD
		mov	_doserrno, eax
		jmp	short loc_405111
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_405094:				; CODE XREF: _write+83j
		push	edi
		lea	ecx, [ebp-78h]
		push	ecx
		push	dword ptr [ebp+3ACh]
		push	dword ptr [ebp+3A8h]
		push	dword ptr [eax]
		call	ds:__imp__WriteFile@20 ; __declspec(dllimport) WriteFile(x,x,x,x,x)
		test	eax, eax
		jz	short loc_4050BF
		mov	eax, [ebp-78h]
		mov	[ebp+3A4h], edi
		mov	[ebp-74h], eax
		jmp	short loc_40506F
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_4050BF:				; CODE XREF: _write+16Bj
		call	ds:__imp__GetLastError@0 ; __declspec(dllimport) GetLastError()
		mov	[ebp+3A4h], eax
		jmp	short loc_40506F
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_4050CD:				; CODE XREF: _write+147j
		push	dword ptr [ebp+3A4h]
		call	_dosmaperr
		pop	ecx
		jmp	short loc_40511B
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_4050DB:				; CODE XREF: _write+9Ej _write+13Cj
		mov	eax, [ebx]
		test	byte ptr [eax+esi+4], 40h
		jz	short loc_4050F3
		mov	eax, [ebp+3A8h]
		cmp	byte ptr [eax],	1Ah
		jz	loc_404FA7

loc_4050F3:				; CODE XREF: _write+19Ej
		mov	errno, 1Ch
		mov	_doserrno, edi
		jmp	short loc_40511B
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_405105:				; CODE XREF: _write+130j
		sub	eax, [ebp-7Ch]
		jmp	short loc_40511E
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_40510A:				; CODE XREF: _write+28j _write+4Dj
		and	_doserrno, 0

loc_405111:				; CODE XREF: _write+14Ej
		mov	errno, 9

loc_40511B:				; CODE XREF: _write+195j _write+1BFj
		or	eax, 0FFFFFFFFh

loc_40511E:				; CODE XREF: _write+65j _write+1C4j
		mov	ecx, [ebp+398h]
		pop	edi
		pop	esi
		pop	ebx
		call	__security_check_cookie
		add	ebp, 39Ch
		leave
		retn
_write		endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


_commit		proc near		; CODE XREF: fflush+2Bp

arg_0		= dword	ptr  4

		mov	eax, [esp+arg_0]
		cmp	eax, _nhandle
		jnb	short loc_40517D
		mov	ecx, eax
		sar	ecx, 5
		mov	ecx, __pioinfo[ecx*4]
		mov	edx, eax
		and	edx, 1Fh
		test	byte ptr [ecx+edx*8+4],	1
		jz	short loc_40517D
		push	eax
		call	_get_osfhandle
		pop	ecx
		push	eax
		call	ds:__imp__FlushFileBuffers@4 ; __declspec(dllimport) FlushFileBuffers(x)
		test	eax, eax
		jnz	short loc_405172
		call	ds:__imp__GetLastError@0 ; __declspec(dllimport) GetLastError()
		jmp	short loc_405174
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_405172:				; CODE XREF: _commit+34j
		xor	eax, eax

loc_405174:				; CODE XREF: _commit+3Cj
		test	eax, eax
		jz	short locret_40518A
		mov	_doserrno, eax

loc_40517D:				; CODE XREF: _commit+Aj _commit+22j
		mov	errno, 9
		or	eax, 0FFFFFFFFh

locret_40518A:				; CODE XREF: _commit+42j
		retn
_commit		endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


_lseek		proc near		; CODE XREF: _flsbuf+CDp

arg_0		= dword	ptr  4
arg_4		= dword	ptr  8
arg_8		= dword	ptr  0Ch

		mov	eax, [esp+arg_0]
		cmp	eax, _nhandle
		push	ebx
		push	esi
		push	edi
		jnb	short loc_4051FF
		mov	ecx, eax
		sar	ecx, 5
		mov	esi, eax
		and	esi, 1Fh
		lea	edi, ds:40D980h[ecx*4]
		mov	ecx, [edi]
		shl	esi, 3
		test	byte ptr [ecx+esi+4], 1
		jz	short loc_4051FF
		push	eax
		call	_get_osfhandle
		cmp	eax, 0FFFFFFFFh
		pop	ecx
		jz	short loc_405206
		push	[esp+0Ch+arg_8]
		push	0
		push	[esp+14h+arg_4]
		push	eax
		call	ds:__imp__SetFilePointer@16 ; __declspec(dllimport) SetFilePointer(x,x,x,x)
		mov	ebx, eax
		cmp	ebx, 0FFFFFFFFh
		jnz	short loc_4051E3
		call	ds:__imp__GetLastError@0 ; __declspec(dllimport) GetLastError()
		jmp	short loc_4051E5
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_4051E3:				; CODE XREF: _lseek+4Ej
		xor	eax, eax

loc_4051E5:				; CODE XREF: _lseek+56j
		test	eax, eax
		jz	short loc_4051F2
		push	eax
		call	_dosmaperr
		pop	ecx
		jmp	short loc_405210
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_4051F2:				; CODE XREF: _lseek+5Cj
		mov	eax, [edi]
		lea	eax, [eax+esi+4]
		and	byte ptr [eax],	0FDh
		mov	eax, ebx
		jmp	short loc_405213
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_4051FF:				; CODE XREF: _lseek+Dj	_lseek+2Aj
		and	_doserrno, 0

loc_405206:				; CODE XREF: _lseek+36j
		mov	errno, 9

loc_405210:				; CODE XREF: _lseek+65j
		or	eax, 0FFFFFFFFh

loc_405213:				; CODE XREF: _lseek+72j
		pop	edi
		pop	esi
		pop	ebx
		retn
_lseek		endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


_getbuf		proc near		; CODE XREF: _flsbuf+6Fp

arg_0		= dword	ptr  4

		inc	_cflush
		push	1000h
		call	malloc
		test	eax, eax
		pop	ecx
		mov	ecx, [esp+arg_0]
		mov	[ecx+8], eax
		jz	short loc_405240
		or	dword ptr [ecx+0Ch], 8
		mov	dword ptr [ecx+18h], 1000h
		jmp	short loc_405251
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_405240:				; CODE XREF: _getbuf+1Aj
		or	dword ptr [ecx+0Ch], 4
		lea	eax, [ecx+14h]
		mov	[ecx+8], eax
		mov	dword ptr [ecx+18h], 2

loc_405251:				; CODE XREF: _getbuf+27j
		mov	eax, [ecx+8]
		and	dword ptr [ecx+4], 0
		mov	[ecx], eax
		retn
_getbuf		endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


_fptrap		proc near		; CODE XREF: _output+476p _output+491p ...
		push	2
		call	_amsg_exit
		pop	ecx
		retn
_fptrap		endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ

; Attributes: bp-based frame

__security_init_cookie proc near	; DATA XREF: .data:init_cookieo

var_10		= dword	ptr -10h
var_C		= dword	ptr -0Ch
var_8		= dword	ptr -8
var_4		= dword	ptr -4

		push	ebp
		mov	ebp, esp
		sub	esp, 10h
		mov	eax, __security_cookie
		test	eax, eax
		jz	short loc_40527A
		cmp	eax, 0BB40E64Eh
		jnz	short locret_4052C8

loc_40527A:				; CODE XREF: __security_init_cookie+Dj
		push	esi
		lea	eax, [ebp+var_8]
		push	eax
		call	ds:__imp__GetSystemTimeAsFileTime@4 ; __declspec(dllimport) GetSystemTimeAsFileTime(x)
		mov	esi, [ebp+var_4]
		xor	esi, [ebp+var_8]
		call	ds:__imp__GetCurrentProcessId@0	; __declspec(dllimport)	GetCurrentProcessId()
		xor	esi, eax
		call	ds:__imp__GetCurrentThreadId@0 ; __declspec(dllimport) GetCurrentThreadId()
		xor	esi, eax
		call	ds:__imp__GetTickCount@0 ; __declspec(dllimport) GetTickCount()
		xor	esi, eax
		lea	eax, [ebp+var_10]
		push	eax
		call	ds:__imp__QueryPerformanceCounter@4 ; __declspec(dllimport) QueryPerformanceCounter(x)
		mov	eax, [ebp+var_C]
		xor	eax, [ebp+var_10]
		xor	esi, eax
		mov	__security_cookie, esi
		jnz	short loc_4052C7
		mov	__security_cookie, 0BB40E64Eh

loc_4052C7:				; CODE XREF: __security_init_cookie+57j
		pop	esi

locret_4052C8:				; CODE XREF: __security_init_cookie+14j
		leave
		retn
__security_init_cookie endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


__security_error_handler proc near	; CODE XREF: report_failure+14p
					; __buffer_overrun+4p

var_8		= dword	ptr -8

		push	118h
		push	offset dword_40AC18
		call	__SEH_prolog
		mov	eax, __security_cookie
		mov	[ebp-1Ch], eax
		mov	eax, user_handler
		xor	ecx, ecx
		cmp	eax, ecx
		jz	short loc_40530B
		mov	[ebp-4], ecx
		push	dword ptr [ebp+0Ch]
		push	dword ptr [ebp+8]
		call	eax
		pop	ecx
		pop	ecx

loc_4052F9:				; CODE XREF: __security_error_handler+3Fj
		or	dword ptr [ebp-4], 0FFFFFFFFh
		jmp	loc_40540A
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		xor	eax, eax
		inc	eax
		retn
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		mov	esp, [ebp-18h]
		jmp	short loc_4052F9
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_40530B:				; CODE XREF: __security_error_handler+20j
		mov	eax, [ebp+8]
		dec	eax
		jz	short loc_405327
		mov	edi, offset ??_C@_0CD@GMKACBEK@Unknown?5security?5failure?5detecte@ ; "Unknown security	failure	detected!"
		mov	dword ptr [ebp-128h], offset ??_C@_0LB@BPLHHLFD@A?5security?5error?5of?5unknown?5caus@ ; "A security error of unknown cause has	b"...
		mov	esi, 0D4h
		jmp	short loc_40533B
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_405327:				; CODE XREF: __security_error_handler+45j
		mov	edi, offset ??_C@_0BJ@MDEKIEOJ@Buffer?5overrun?5detected?$CB?$AA@ ; "Buffer overrun detected!"
		mov	dword ptr [ebp-128h], offset ??_C@_0KA@JEHFIFGP@A?5buffer?5overrun?5has?5been?5detect@ ; "A buffer overrun has been detected whic"...
		mov	esi, 0B9h

loc_40533B:				; CODE XREF: __security_error_handler+5Bj
		mov	[ebp-20h], cl
		push	104h
		lea	eax, [ebp-124h]
		push	eax
		push	ecx
		call	ds:__imp__GetModuleFileNameA@12	; __declspec(dllimport)	GetModuleFileNameA(x,x,x)
		test	eax, eax
		jnz	short loc_405368
		push	offset ??_C@_0BH@DNAGHKFM@?$DMprogram?5name?5unknown?$DO?$AA@ ;	"<program name unknown>"
		lea	eax, [ebp-124h]
		push	eax
		call	strcpy
		pop	ecx
		pop	ecx

loc_405368:				; CODE XREF: __security_error_handler+89j
		lea	ebx, [ebp-124h]
		mov	eax, ebx
		push	eax
		call	strlen
		pop	ecx
		add	eax, 0Bh
		cmp	eax, 3Ch
		jbe	short loc_4053A4
		mov	eax, ebx
		push	eax
		call	strlen
		mov	ebx, eax
		lea	eax, [ebp-124h]
		sub	eax, 31h
		add	ebx, eax
		push	3
		push	offset ??_C@_03KHICJKCI@?4?4?4?$AA@ ; "..."
		push	ebx
		call	strncpy
		add	esp, 10h

loc_4053A4:				; CODE XREF: __security_error_handler+B3j
		push	ebx
		call	strlen
		pop	ecx
		lea	eax, [eax+esi+0Ch]
		add	eax, 3
		and	eax, 0FFFFFFFCh
		call	_chkstk
		mov	[ebp-18h], esp
		mov	esi, esp
		push	edi
		push	esi
		call	strcpy
		mov	edi, offset ??_C@_02PHMGELLB@?6?6?$AA@ ; "\n\n"
		push	edi
		push	esi
		call	strcat
		push	offset ??_C@_09KLGCKDOD@Program?3?5?$AA@ ; "Program: "
		push	esi
		call	strcat
		push	ebx
		push	esi
		call	strcat
		push	edi
		push	esi
		call	strcat
		push	dword ptr [ebp-128h]
		push	esi
		call	strcat
		push	12010h
		push	offset ??_C@_0CF@GOGNBNAK@Microsoft?5Visual?5C?$CL?$CL?5Runtime?5Lib@ ;	"Microsoft Visual C++ Runtime Library"
		push	esi
		call	__crtMessageBoxA
		add	esp, 3Ch

loc_40540A:				; CODE XREF: __security_error_handler+33j
		push	3		; int
		call	_exit
		int	3		; Trap to Debugger

_set_security_error_handler:
		mov	ecx, [esp+0Ch+var_8]
		mov	eax, user_handler
		mov	user_handler, ecx
		retn
__security_error_handler endp ;	sp = -0Ch


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


__buffer_overrun proc near
		push	0
		push	1
		call	__security_error_handler
		pop	ecx
		pop	ecx
		retn
__buffer_overrun endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


__set_buffer_overrun_handler proc near

arg_0		= dword	ptr  4

		mov	ecx, [esp+arg_0]
		mov	eax, user_handler
		mov	user_handler, ecx
		retn
__set_buffer_overrun_handler endp

; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		align 10h

; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


memset		proc near		; CODE XREF: calloc+6Dp
					; __crtLCMapStringA+2EFp ...

arg_0		= dword	ptr  4
arg_4		= byte ptr  8
arg_8		= dword	ptr  0Ch

		mov	edx, [esp+arg_8]
		mov	ecx, [esp+arg_0]
		test	edx, edx
		jz	short loc_40549B
		xor	eax, eax
		mov	al, [esp+arg_4]
		push	edi
		mov	edi, ecx
		cmp	edx, 4
		jb	short loc_40548B
		neg	ecx
		and	ecx, 3
		jz	short loc_40546D
		sub	edx, ecx

loc_405463:				; CODE XREF: memset+2Bj
		mov	[edi], al
		add	edi, 1
		sub	ecx, 1
		jnz	short loc_405463

loc_40546D:				; CODE XREF: memset+1Fj
		mov	ecx, eax
		shl	eax, 8
		add	eax, ecx
		mov	ecx, eax
		shl	eax, 10h
		add	eax, ecx
		mov	ecx, edx
		and	edx, 3
		shr	ecx, 2
		jz	short loc_40548B
		rep stosd
		test	edx, edx
		jz	short loc_405495

loc_40548B:				; CODE XREF: memset+18j memset+43j ...
		mov	[edi], al
		add	edi, 1
		sub	edx, 1
		jnz	short loc_40548B

loc_405495:				; CODE XREF: memset+49j
		mov	eax, [esp+4+arg_0]
		pop	edi
		retn
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_40549B:				; CODE XREF: memset+Aj
		mov	eax, [esp+arg_0]
		retn
memset		endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


fclose		proc near		; CODE XREF: _fcloseall+22p

arg_0		= dword	ptr  8

		push	esi
		mov	esi, [esp+arg_0]
		mov	eax, [esi+0Ch]
		push	edi
		or	edi, 0FFFFFFFFh
		test	al, 40h
		jz	short loc_4054B5
		or	eax, 0FFFFFFFFh
		jmp	short loc_4054EF
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_4054B5:				; CODE XREF: fclose+Ej
		test	al, 83h
		jz	short loc_4054ED
		push	esi
		call	_flush
		push	esi
		mov	edi, eax
		call	_freebuf
		push	dword ptr [esi+10h]
		call	_close
		add	esp, 0Ch
		test	eax, eax
		jge	short loc_4054DB
		or	edi, 0FFFFFFFFh
		jmp	short loc_4054ED
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_4054DB:				; CODE XREF: fclose+34j
		mov	eax, [esi+1Ch]
		test	eax, eax
		jz	short loc_4054ED
		push	eax
		call	free
		and	dword ptr [esi+1Ch], 0
		pop	ecx

loc_4054ED:				; CODE XREF: fclose+17j fclose+39j ...
		mov	eax, edi

loc_4054EF:				; CODE XREF: fclose+13j
		and	dword ptr [esi+0Ch], 0
		pop	edi
		pop	esi
		retn
fclose		endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


realloc		proc near		; CODE XREF: _onexit+34p _onexit+49p

arg_0		= dword	ptr  8
arg_4		= dword	ptr  0Ch

		push	ebx
		mov	ebx, [esp+arg_0]
		test	ebx, ebx
		push	ebp
		push	edi
		jnz	short loc_405510
		push	[esp+8+arg_4]
		call	malloc
		pop	ecx
		jmp	loc_405654
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_405510:				; CODE XREF: realloc+9j
		push	esi
		mov	esi, [esp+0Ch+arg_4]
		test	esi, esi
		jnz	short loc_405525
		push	ebx
		call	free
		pop	ecx
		jmp	loc_405651
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_405525:				; CODE XREF: realloc+21j
		cmp	__active_heap, 3
		jnz	loc_40561D

loc_405532:				; CODE XREF: realloc+11Bj
		xor	edi, edi
		cmp	esi, 0FFFFFFE0h
		ja	loc_4055FF
		push	ebx
		call	__sbh_find_block
		mov	ebp, eax
		test	ebp, ebp
		pop	ecx
		jz	loc_4055DE
		cmp	esi, __sbh_threshold
		ja	short loc_40559E
		push	esi
		push	ebx
		push	ebp
		call	__sbh_resize_block
		add	esp, 0Ch
		test	eax, eax
		jz	short loc_405569
		mov	edi, ebx
		jmp	short loc_40559A
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_405569:				; CODE XREF: realloc+6Dj
		push	esi
		call	__sbh_alloc_block
		mov	edi, eax
		test	edi, edi
		pop	ecx
		jz	short loc_40559E
		mov	eax, [ebx-4]
		dec	eax
		cmp	eax, esi
		jb	short loc_405580
		mov	eax, esi

loc_405580:				; CODE XREF: realloc+86j
		push	eax
		push	ebx
		push	edi
		call	memcpy
		push	ebx
		call	__sbh_find_block
		mov	ebp, eax
		push	ebx
		push	ebp
		call	__sbh_free_block
		add	esp, 18h

loc_40559A:				; CODE XREF: realloc+71j
		test	edi, edi
		jnz	short loc_4055DA

loc_40559E:				; CODE XREF: realloc+5Ej realloc+7Ej
		test	esi, esi
		jnz	short loc_4055A3
		inc	esi

loc_4055A3:				; CODE XREF: realloc+AAj
		add	esi, 0Fh
		and	esi, 0FFFFFFF0h
		push	esi
		push	0
		push	_crtheap
		call	ds:__imp__HeapAlloc@12 ; __declspec(dllimport) HeapAlloc(x,x,x)
		mov	edi, eax
		test	edi, edi
		jz	short loc_4055DA
		mov	eax, [ebx-4]
		dec	eax
		cmp	eax, esi
		jb	short loc_4055C8
		mov	eax, esi

loc_4055C8:				; CODE XREF: realloc+CEj
		push	eax
		push	ebx
		push	edi
		call	memcpy
		push	ebx
		push	ebp
		call	__sbh_free_block
		add	esp, 14h

loc_4055DA:				; CODE XREF: realloc+A6j realloc+C6j
		test	ebp, ebp
		jnz	short loc_4055FB

loc_4055DE:				; CODE XREF: realloc+52j
		test	esi, esi
		jnz	short loc_4055E3
		inc	esi

loc_4055E3:				; CODE XREF: realloc+EAj
		add	esi, 0Fh
		and	esi, 0FFFFFFF0h
		push	esi
		push	ebx
		push	0
		push	_crtheap
		call	ds:__imp__HeapReAlloc@16 ; __declspec(dllimport) HeapReAlloc(x,x,x,x)
		mov	edi, eax

loc_4055FB:				; CODE XREF: realloc+E6j
		test	edi, edi
		jnz	short loc_405619

loc_4055FF:				; CODE XREF: realloc+41j
		cmp	_newmode, 0
		jz	short loc_405619
		push	esi
		call	_callnewh
		test	eax, eax
		pop	ecx
		jnz	loc_405532
		jmp	short loc_405651
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_405619:				; CODE XREF: realloc+107j realloc+110j
		mov	eax, edi
		jmp	short loc_405653
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_40561D:				; CODE XREF: realloc+36j realloc+159j
		xor	eax, eax
		cmp	esi, 0FFFFFFE0h
		ja	short loc_40563D
		test	esi, esi
		jnz	short loc_405629
		inc	esi

loc_405629:				; CODE XREF: realloc+130j
		push	esi
		push	ebx
		push	0
		push	_crtheap
		call	ds:__imp__HeapReAlloc@16 ; __declspec(dllimport) HeapReAlloc(x,x,x,x)
		test	eax, eax
		jnz	short loc_405653

loc_40563D:				; CODE XREF: realloc+12Cj
		cmp	_newmode, 0
		jz	short loc_405653
		push	esi
		call	_callnewh
		test	eax, eax
		pop	ecx
		jnz	short loc_40561D

loc_405651:				; CODE XREF: realloc+2Aj realloc+121j
		xor	eax, eax

loc_405653:				; CODE XREF: realloc+125j realloc+145j ...
		pop	esi

loc_405654:				; CODE XREF: realloc+15j
		pop	edi
		pop	ebp
		pop	ebx
		retn
realloc		endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


_msize		proc near		; CODE XREF: _onexit+7p

arg_0		= dword	ptr  4

		cmp	__active_heap, 3
		push	esi
		jnz	short loc_40567C
		mov	esi, [esp+4+arg_0]
		push	esi
		call	__sbh_find_block
		test	eax, eax
		pop	ecx
		jz	short loc_405679
		mov	eax, [esi-4]
		sub	eax, 9
		pop	esi
		retn
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_405679:				; CODE XREF: _msize+17j
		push	esi
		jmp	short loc_405680
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_40567C:				; CODE XREF: _msize+8j
		push	[esp+4+arg_0]

loc_405680:				; CODE XREF: _msize+22j
		push	0
		push	_crtheap
		call	ds:__imp__HeapSize@12 ;	__declspec(dllimport) HeapSize(x,x,x)
		pop	esi
		retn
_msize		endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


strncnt		proc near

arg_0		= dword	ptr  4

		mov	ecx, [esp+arg_0]
		test	ecx, ecx
		jz	short loc_4056A3

loc_405698:				; CODE XREF: strncnt+11j
		dec	ecx
		cmp	byte ptr [eax],	0
		jz	short loc_4056A4
		inc	eax
		test	ecx, ecx
		jnz	short loc_405698

loc_4056A3:				; CODE XREF: strncnt+6j
		dec	ecx

loc_4056A4:				; CODE XREF: strncnt+Cj
		mov	eax, [esp+arg_0]
		sub	eax, ecx
		dec	eax
		retn
strncnt		endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


__crtLCMapStringA proc near		; CODE XREF: setSBUpLow+C5p
					; setSBUpLow+EDp
		push	38h
		push	offset dword_40AC28
		call	__SEH_prolog
		xor	ebx, ebx
		cmp	dword_40D630, ebx
		jnz	short loc_4056FA
		push	ebx
		push	ebx
		xor	esi, esi
		inc	esi
		push	esi
		push	offset ??_C@_13NOLLCAOD@?$AA?$AA?$AA?$AA@
		push	100h
		push	ebx
		call	ds:__imp__LCMapStringW@24 ; __declspec(dllimport) LCMapStringW(x,x,x,x,x,x)
		test	eax, eax
		jz	short loc_4056E5
		mov	dword_40D630, esi
		jmp	short loc_4056FA
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_4056E5:				; CODE XREF: __crtLCMapStringA+2Fj
		call	ds:__imp__GetLastError@0 ; __declspec(dllimport) GetLastError()
		cmp	eax, 78h
		jnz	short loc_4056FA
		mov	dword_40D630, 2

loc_4056FA:				; CODE XREF: __crtLCMapStringA+14j
					; __crtLCMapStringA+37j ...
		cmp	[ebp+14h], ebx
		jle	short loc_40571A
		mov	ecx, [ebp+14h]
		mov	eax, [ebp+10h]

loc_405705:				; CODE XREF: __crtLCMapStringA+61j
		dec	ecx
		cmp	[eax], bl
		jz	short loc_405712
		inc	eax
		cmp	ecx, ebx
		jnz	short loc_405705
		or	ecx, 0FFFFFFFFh

loc_405712:				; CODE XREF: __crtLCMapStringA+5Cj
		or	eax, 0FFFFFFFFh
		sub	eax, ecx
		add	[ebp+14h], eax

loc_40571A:				; CODE XREF: __crtLCMapStringA+51j
		mov	eax, dword_40D630
		cmp	eax, 2
		jz	loc_405904
		cmp	eax, ebx
		jz	loc_405904
		cmp	eax, 1
		jnz	loc_405937
		xor	edi, edi
		mov	[ebp-2Ch], edi
		mov	[ebp-38h], ebx
		mov	[ebp-34h], ebx
		cmp	[ebp+20h], ebx
		jnz	short loc_405751
		mov	eax, __lc_codepage
		mov	[ebp+20h], eax

loc_405751:				; CODE XREF: __crtLCMapStringA+9Bj
		push	ebx
		push	ebx
		push	dword ptr [ebp+14h]
		push	dword ptr [ebp+10h]
		xor	eax, eax
		cmp	[ebp+24h], ebx
		setnz	al
		lea	eax, ds:1[eax*8]
		push	eax
		push	dword ptr [ebp+20h]
		call	ds:__imp__MultiByteToWideChar@24 ; __declspec(dllimport) MultiByteToWideChar(x,x,x,x,x,x)
		mov	esi, eax
		mov	[ebp-30h], esi
		cmp	esi, ebx
		jz	loc_405937
		mov	dword ptr [ebp-4], 1
		lea	eax, [esi+esi]
		add	eax, 3
		and	eax, 0FFFFFFFCh
		call	_chkstk
		mov	[ebp-18h], esp
		mov	eax, esp
		mov	[ebp-1Ch], eax
		or	dword ptr [ebp-4], 0FFFFFFFFh
		jmp	short loc_4057BD
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		xor	eax, eax
		inc	eax
		retn
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		mov	esp, [ebp-18h]
		call	_resetstkoflw
		xor	ebx, ebx
		mov	[ebp-1Ch], ebx
		or	dword ptr [ebp-4], 0FFFFFFFFh
		mov	edi, [ebp-2Ch]
		mov	esi, [ebp-30h]

loc_4057BD:				; CODE XREF: __crtLCMapStringA+F4j
		cmp	[ebp-1Ch], ebx
		jnz	short loc_4057DE
		lea	eax, [esi+esi]
		push	eax
		call	malloc
		pop	ecx
		mov	[ebp-1Ch], eax
		cmp	eax, ebx
		jz	loc_405937
		mov	dword ptr [ebp-38h], 1

loc_4057DE:				; CODE XREF: __crtLCMapStringA+114j
		push	esi
		push	dword ptr [ebp-1Ch]
		push	dword ptr [ebp+14h]
		push	dword ptr [ebp+10h]
		push	1
		push	dword ptr [ebp+20h]
		call	ds:__imp__MultiByteToWideChar@24 ; __declspec(dllimport) MultiByteToWideChar(x,x,x,x,x,x)
		test	eax, eax
		jz	loc_4058E1
		push	ebx
		push	ebx
		push	esi
		push	dword ptr [ebp-1Ch]
		push	dword ptr [ebp+0Ch]
		push	dword ptr [ebp+8]
		call	ds:__imp__LCMapStringW@24 ; __declspec(dllimport) LCMapStringW(x,x,x,x,x,x)
		mov	edi, eax
		mov	[ebp-2Ch], edi
		cmp	edi, ebx
		jz	loc_4058E1
		test	byte ptr [ebp+0Dh], 4
		jz	short loc_40584D
		cmp	[ebp+1Ch], ebx
		jz	loc_4058E1
		cmp	edi, [ebp+1Ch]
		jg	loc_4058E1
		push	dword ptr [ebp+1Ch]
		push	dword ptr [ebp+18h]
		push	esi
		push	dword ptr [ebp-1Ch]
		push	dword ptr [ebp+0Ch]
		push	dword ptr [ebp+8]
		call	ds:__imp__LCMapStringW@24 ; __declspec(dllimport) LCMapStringW(x,x,x,x,x,x)
		jmp	loc_4058E1
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_40584D:				; CODE XREF: __crtLCMapStringA+172j
		mov	dword ptr [ebp-4], 2
		lea	eax, [edi+edi]
		add	eax, 3
		and	eax, 0FFFFFFFCh
		call	_chkstk
		mov	[ebp-18h], esp
		mov	eax, esp
		mov	[ebp-20h], eax
		or	dword ptr [ebp-4], 0FFFFFFFFh
		jmp	short loc_40588B
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		xor	eax, eax
		inc	eax
		retn
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		mov	esp, [ebp-18h]
		call	_resetstkoflw
		xor	ebx, ebx
		mov	[ebp-20h], ebx
		or	dword ptr [ebp-4], 0FFFFFFFFh
		mov	edi, [ebp-2Ch]
		mov	esi, [ebp-30h]

loc_40588B:				; CODE XREF: __crtLCMapStringA+1C2j
		cmp	[ebp-20h], ebx
		jnz	short loc_4058A8
		lea	eax, [edi+edi]
		push	eax
		call	malloc
		pop	ecx
		mov	[ebp-20h], eax
		cmp	eax, ebx
		jz	short loc_4058E1
		mov	dword ptr [ebp-34h], 1

loc_4058A8:				; CODE XREF: __crtLCMapStringA+1E2j
		push	edi
		push	dword ptr [ebp-20h]
		push	esi
		push	dword ptr [ebp-1Ch]
		push	dword ptr [ebp+0Ch]
		push	dword ptr [ebp+8]
		call	ds:__imp__LCMapStringW@24 ; __declspec(dllimport) LCMapStringW(x,x,x,x,x,x)
		test	eax, eax
		jz	short loc_4058E1
		push	ebx
		push	ebx
		cmp	[ebp+1Ch], ebx
		jnz	short loc_4058CB
		push	ebx
		push	ebx
		jmp	short loc_4058D1
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_4058CB:				; CODE XREF: __crtLCMapStringA+219j
		push	dword ptr [ebp+1Ch]
		push	dword ptr [ebp+18h]

loc_4058D1:				; CODE XREF: __crtLCMapStringA+21Dj
		push	edi
		push	dword ptr [ebp-20h]
		push	ebx
		push	dword ptr [ebp+20h]
		call	ds:__imp__WideCharToMultiByte@32 ; __declspec(dllimport) WideCharToMultiByte(x,x,x,x,x,x,x,x)
		mov	edi, eax

loc_4058E1:				; CODE XREF: __crtLCMapStringA+149j
					; __crtLCMapStringA+168j ...
		cmp	[ebp-34h], ebx
		jz	short loc_4058EF
		push	dword ptr [ebp-20h]
		call	free
		pop	ecx

loc_4058EF:				; CODE XREF: __crtLCMapStringA+238j
		cmp	[ebp-38h], ebx
		jz	short loc_4058FD
		push	dword ptr [ebp-1Ch]
		call	free
		pop	ecx

loc_4058FD:				; CODE XREF: __crtLCMapStringA+246j
		mov	eax, edi
		jmp	loc_405A5F
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_405904:				; CODE XREF: __crtLCMapStringA+76j
					; __crtLCMapStringA+7Ej
		mov	[ebp-28h], ebx
		xor	edi, edi
		mov	[ebp-3Ch], ebx
		cmp	[ebp+8], ebx
		jnz	short loc_405919
		mov	eax, dword_40D614
		mov	[ebp+8], eax

loc_405919:				; CODE XREF: __crtLCMapStringA+263j
		cmp	[ebp+20h], ebx
		jnz	short loc_405926
		mov	eax, __lc_codepage
		mov	[ebp+20h], eax

loc_405926:				; CODE XREF: __crtLCMapStringA+270j
		push	dword ptr [ebp+8]
		call	__ansicp
		pop	ecx
		mov	[ebp-40h], eax
		cmp	eax, 0FFFFFFFFh
		jnz	short loc_40593E

loc_405937:				; CODE XREF: __crtLCMapStringA+87j
					; __crtLCMapStringA+CDj ...
		xor	eax, eax
		jmp	loc_405A5F
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_40593E:				; CODE XREF: __crtLCMapStringA+289j
		cmp	eax, [ebp+20h]
		jz	loc_405A35
		push	ebx
		push	ebx
		lea	ecx, [ebp+14h]
		push	ecx
		push	dword ptr [ebp+10h]
		push	eax
		push	dword ptr [ebp+20h]
		call	__convertcp
		add	esp, 18h
		mov	[ebp-28h], eax
		cmp	eax, ebx
		jz	short loc_405937
		push	ebx
		push	ebx
		push	dword ptr [ebp+14h]
		push	eax
		push	dword ptr [ebp+0Ch]
		push	dword ptr [ebp+8]
		call	ds:__imp__LCMapStringA@24 ; __declspec(dllimport) LCMapStringA(x,x,x,x,x,x)
		mov	esi, eax
		mov	[ebp-24h], esi
		cmp	esi, ebx
		jz	loc_405A24
		mov	[ebp-4], ebx
		add	eax, 3
		and	eax, 0FFFFFFFCh
		call	_chkstk
		mov	[ebp-18h], esp
		mov	edi, esp
		mov	[ebp-44h], edi
		push	esi
		push	ebx
		push	edi
		call	memset
		add	esp, 0Ch
		jmp	short loc_4059B5
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		xor	eax, eax
		inc	eax
		retn
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		mov	esp, [ebp-18h]
		call	_resetstkoflw
		xor	ebx, ebx
		xor	edi, edi

loc_4059B5:				; CODE XREF: __crtLCMapStringA+2F7j
		or	dword ptr [ebp-4], 0FFFFFFFFh
		cmp	edi, ebx
		jnz	short loc_4059E0
		push	dword ptr [ebp-24h]
		call	malloc
		pop	ecx
		mov	edi, eax
		cmp	edi, ebx
		jz	short loc_4059FD
		push	dword ptr [ebp-24h]
		push	ebx
		push	edi
		call	memset
		add	esp, 0Ch
		mov	dword ptr [ebp-3Ch], 1

loc_4059E0:				; CODE XREF: __crtLCMapStringA+30Fj
		push	dword ptr [ebp-24h]
		push	edi
		push	dword ptr [ebp+14h]
		push	dword ptr [ebp-28h]
		push	dword ptr [ebp+0Ch]
		push	dword ptr [ebp+8]
		call	ds:__imp__LCMapStringA@24 ; __declspec(dllimport) LCMapStringA(x,x,x,x,x,x)
		mov	[ebp-24h], eax
		cmp	eax, ebx
		jnz	short loc_405A01

loc_4059FD:				; CODE XREF: __crtLCMapStringA+31Ej
		xor	esi, esi
		jmp	short loc_405A27
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_405A01:				; CODE XREF: __crtLCMapStringA+34Fj
		push	dword ptr [ebp+1Ch]
		push	dword ptr [ebp+18h]
		lea	eax, [ebp-24h]
		push	eax
		push	edi
		push	dword ptr [ebp+20h]
		push	dword ptr [ebp-40h]
		call	__convertcp
		add	esp, 18h
		mov	esi, eax
		neg	esi
		sbb	esi, esi
		neg	esi
		jmp	short loc_405A27
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_405A24:				; CODE XREF: __crtLCMapStringA+2D0j
		mov	esi, [ebp-48h]

loc_405A27:				; CODE XREF: __crtLCMapStringA+353j
					; __crtLCMapStringA+376j
		cmp	[ebp-3Ch], ebx
		jz	short loc_405A4F
		push	edi
		call	free
		pop	ecx
		jmp	short loc_405A4F
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_405A35:				; CODE XREF: __crtLCMapStringA+295j
		push	dword ptr [ebp+1Ch]
		push	dword ptr [ebp+18h]
		push	dword ptr [ebp+14h]
		push	dword ptr [ebp+10h]
		push	dword ptr [ebp+0Ch]
		push	dword ptr [ebp+8]
		call	ds:__imp__LCMapStringA@24 ; __declspec(dllimport) LCMapStringA(x,x,x,x,x,x)
		mov	esi, eax

loc_405A4F:				; CODE XREF: __crtLCMapStringA+37Ej
					; __crtLCMapStringA+387j
		cmp	[ebp-28h], ebx
		jz	short loc_405A5D
		push	dword ptr [ebp-28h]
		call	free
		pop	ecx

loc_405A5D:				; CODE XREF: __crtLCMapStringA+3A6j
		mov	eax, esi

loc_405A5F:				; CODE XREF: __crtLCMapStringA+253j
					; __crtLCMapStringA+28Dj
		lea	esp, [ebp-54h]
		call	__SEH_epilog
		retn
__crtLCMapStringA endp ; sp = -54h


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


__crtGetStringTypeA proc near		; CODE XREF: setSBUpLow+A1p
					; _ismbcspace+4Fp ...
		push	1Ch
		push	offset dword_40AC50
		call	__SEH_prolog
		xor	esi, esi
		cmp	dword_40D634, esi
		jnz	short loc_405AB3
		lea	eax, [ebp-1Ch]
		push	eax
		xor	edi, edi
		inc	edi
		push	edi
		push	offset ??_C@_13NOLLCAOD@?$AA?$AA?$AA?$AA@
		push	edi
		call	ds:__imp__GetStringTypeW@16 ; __declspec(dllimport) GetStringTypeW(x,x,x,x)
		test	eax, eax
		jz	short loc_405A9E
		mov	dword_40D634, edi
		jmp	short loc_405AB3
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_405A9E:				; CODE XREF: __crtGetStringTypeA+2Cj
		call	ds:__imp__GetLastError@0 ; __declspec(dllimport) GetLastError()
		cmp	eax, 78h
		jnz	short loc_405AB3
		mov	dword_40D634, 2

loc_405AB3:				; CODE XREF: __crtGetStringTypeA+14j
					; __crtGetStringTypeA+34j ...
		mov	eax, dword_40D634
		cmp	eax, 2
		jz	loc_405BAB
		cmp	eax, esi
		jz	loc_405BAB
		cmp	eax, 1
		jnz	loc_405BD1
		mov	[ebp-24h], esi
		mov	[ebp-20h], esi
		cmp	[ebp+18h], esi
		jnz	short loc_405AE5
		mov	eax, __lc_codepage
		mov	[ebp+18h], eax

loc_405AE5:				; CODE XREF: __crtGetStringTypeA+73j
		push	esi
		push	esi
		push	dword ptr [ebp+10h]
		push	dword ptr [ebp+0Ch]
		xor	eax, eax
		cmp	[ebp+20h], esi
		setnz	al
		lea	eax, ds:1[eax*8]
		push	eax
		push	dword ptr [ebp+18h]
		call	ds:__imp__MultiByteToWideChar@24 ; __declspec(dllimport) MultiByteToWideChar(x,x,x,x,x,x)
		mov	edi, eax
		mov	[ebp-28h], edi
		test	edi, edi
		jz	loc_405BD1
		and	dword ptr [ebp-4], 0
		lea	ebx, [edi+edi]
		mov	eax, ebx
		add	eax, 3
		and	eax, 0FFFFFFFCh
		call	_chkstk
		mov	[ebp-18h], esp
		mov	esi, esp
		mov	[ebp-2Ch], esi
		push	ebx
		push	0
		push	esi
		call	memset
		add	esp, 0Ch
		or	dword ptr [ebp-4], 0FFFFFFFFh
		jmp	short loc_405B56
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		xor	eax, eax
		inc	eax
		retn
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		mov	esp, [ebp-18h]
		call	_resetstkoflw
		xor	esi, esi
		or	dword ptr [ebp-4], 0FFFFFFFFh
		mov	edi, [ebp-28h]

loc_405B56:				; CODE XREF: __crtGetStringTypeA+D7j
		test	esi, esi
		jnz	short loc_405B71
		push	edi
		push	2
		call	calloc
		pop	ecx
		pop	ecx
		mov	esi, eax
		test	esi, esi
		jz	short loc_405BD1
		mov	dword ptr [ebp-20h], 1

loc_405B71:				; CODE XREF: __crtGetStringTypeA+F0j
		push	edi
		push	esi
		push	dword ptr [ebp+10h]
		push	dword ptr [ebp+0Ch]
		push	1
		push	dword ptr [ebp+18h]
		call	ds:__imp__MultiByteToWideChar@24 ; __declspec(dllimport) MultiByteToWideChar(x,x,x,x,x,x)
		test	eax, eax
		jz	short loc_405B99
		push	dword ptr [ebp+14h]
		push	eax
		push	esi
		push	dword ptr [ebp+8]
		call	ds:__imp__GetStringTypeW@16 ; __declspec(dllimport) GetStringTypeW(x,x,x,x)
		mov	[ebp-24h], eax

loc_405B99:				; CODE XREF: __crtGetStringTypeA+11Ej
		cmp	dword ptr [ebp-20h], 0
		jz	short loc_405BA6
		push	esi
		call	free
		pop	ecx

loc_405BA6:				; CODE XREF: __crtGetStringTypeA+135j
		mov	eax, [ebp-24h]
		jmp	short loc_405C19
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_405BAB:				; CODE XREF: __crtGetStringTypeA+53j
					; __crtGetStringTypeA+5Bj
		mov	ebx, [ebp+1Ch]
		cmp	ebx, esi
		jnz	short loc_405BB8
		mov	ebx, dword_40D614

loc_405BB8:				; CODE XREF: __crtGetStringTypeA+148j
		mov	edi, [ebp+18h]
		test	edi, edi
		jnz	short loc_405BC5
		mov	edi, __lc_codepage

loc_405BC5:				; CODE XREF: __crtGetStringTypeA+155j
		push	ebx
		call	__ansicp
		pop	ecx
		cmp	eax, 0FFFFFFFFh
		jnz	short loc_405BD5

loc_405BD1:				; CODE XREF: __crtGetStringTypeA+64j
					; __crtGetStringTypeA+A5j ...
		xor	eax, eax
		jmp	short loc_405C19
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_405BD5:				; CODE XREF: __crtGetStringTypeA+167j
		cmp	eax, edi
		jz	short loc_405BF7
		push	0
		push	0
		lea	ecx, [ebp+10h]
		push	ecx
		push	dword ptr [ebp+0Ch]
		push	eax
		push	edi
		call	__convertcp
		add	esp, 18h
		mov	esi, eax
		test	esi, esi
		jz	short loc_405BD1
		mov	[ebp+0Ch], esi

loc_405BF7:				; CODE XREF: __crtGetStringTypeA+16Fj
		push	dword ptr [ebp+14h]
		push	dword ptr [ebp+10h]
		push	dword ptr [ebp+0Ch]
		push	dword ptr [ebp+8]
		push	ebx
		call	ds:__imp__GetStringTypeA@20 ; __declspec(dllimport) GetStringTypeA(x,x,x,x,x)
		mov	edi, eax
		test	esi, esi
		jz	short loc_405C17
		push	esi
		call	free
		pop	ecx

loc_405C17:				; CODE XREF: __crtGetStringTypeA+1A6j
		mov	eax, edi

loc_405C19:				; CODE XREF: __crtGetStringTypeA+141j
					; __crtGetStringTypeA+16Bj
		lea	esp, [ebp-38h]
		call	__SEH_epilog
		retn
__crtGetStringTypeA endp ; sp =	-38h

; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		align 10h

memmove:				; CODE XREF: __sbh_free_block+2DEp
					; __sbh_heapmin+BBp
		push	ebp
		mov	ebp, esp
		push	edi
		push	esi
		mov	esi, [ebp+0Ch]
		mov	ecx, [ebp+10h]
		mov	edi, [ebp+8]
		mov	eax, ecx
		mov	edx, ecx
		add	eax, esi
		cmp	edi, esi
		jbe	short loc_405C50
		cmp	edi, eax
		jb	loc_405DCC

loc_405C50:				; CODE XREF: .text:00405C46j
		test	edi, 3
		jnz	short loc_405C6C
		shr	ecx, 2
		and	edx, 3
		cmp	ecx, 8
		jb	short near ptr dword_405C8C
		rep movsd
		jmp	ds:TrailUpVec[edx*4]
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_405C6C:				; CODE XREF: .text:00405C56j
		mov	eax, edi
		mov	edx, 3
		sub	ecx, 4
		jb	short loc_405C84
		and	eax, 3
		add	ecx, eax
		jmp	ds:dword_405C90[eax*4]
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_405C84:				; CODE XREF: .text:00405C76j
		jmp	dword ptr ds:loc_405D8C[ecx*4]
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		align 4
dword_405C8C	dd 108D24FFh		; CODE XREF: .text:00405C61j
dword_405C90	dd 9000405Dh		; DATA XREF: .text:00405C7Dr

; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


LeadUpVec	proc near
		mov	al, ds:0CC00405Ch
		pop	esp
		inc	eax
		add	al, dh
		pop	esp
		inc	eax
		add	[ebx], ah
		ror	dword ptr [edx-75F877FAh], 1
		inc	esi
		add	[eax+468A0147h], ecx
		add	al, cl
		jmp	near ptr 287E4B7h
LeadUpVec	endp

; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		dd 8303C683h, 0F98303C7h, 0F3CC7208h, 9524FFA5h, 405D7Ch
		dd 2300498Dh, 88068AD1h, 1468A07h, 8802E9C1h, 0C6830147h
		dd 2C78302h, 7208F983h,	0FFA5F3A6h, 5D7C9524h, 23900040h
		dd 88068AD1h, 1C68307h,	8302E9C1h, 0F98301C7h, 0F3887208h
		dd 9524FFA5h, 405D7Ch
		align 10h

; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


UnwindUpVec	proc near
		jnb	short loc_405D6F
		inc	eax
		add	[eax+5Dh], ah
		inc	eax
		add	[eax+5Dh], bl
		inc	eax
		add	[eax+5Dh], dl
		inc	eax
		add	[eax+5Dh], cl
		inc	eax
		add	[eax+5Dh], al
		inc	eax
		add	[eax], bh
		pop	ebp
		inc	eax
		add	[eax], dh
		pop	ebp
		inc	eax
		add	[ebx-761B71BCh], cl
		inc	esp
		pop	esp
		mov	eax, [esi+ecx*4-18h]
		mov	[edi+ecx*4-18h], eax
		mov	eax, [esi+ecx*4-14h]
		mov	[edi+ecx*4-14h], eax
		mov	eax, [esi+ecx*4-10h]
		mov	[edi+ecx*4-10h], eax
		mov	eax, [esi+ecx*4-0Ch]
		mov	[edi+ecx*4-0Ch], eax
		mov	eax, [esi+ecx*4-8]
		mov	[edi+ecx*4-8], eax
		mov	eax, [esi+ecx*4-4]
		mov	[edi+ecx*4-4], eax
		lea	eax, ds:0[ecx*4]

loc_405D6F:				; CODE XREF: UnwindUpVecj
		add	esi, eax
		add	edi, eax
		jmp	ds:TrailUpVec[edx*4]
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		align 4
TrailUpVec	dd offset loc_405D8C	; DATA XREF: .text:00405C65r
					; UnwindUpVec+63r
		dd offset loc_405D94
		dd offset loc_405DA0
		dd offset loc_405DB4
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_405D8C:				; CODE XREF: .text:00405C65j
					; UnwindUpVec+63j
					; DATA XREF: ...
		mov	eax, [ebp+8]
		pop	esi
		pop	edi
		leave
		retn
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		align 4

loc_405D94:				; CODE XREF: UnwindUpVec+63j
					; DATA XREF: UnwindUpVec+70o
		mov	al, [esi]
		mov	[edi], al
		mov	eax, [ebp+8]
		pop	esi
		pop	edi
		leave
		retn
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		align 10h

loc_405DA0:				; CODE XREF: UnwindUpVec+63j
					; DATA XREF: UnwindUpVec+74o
		mov	al, [esi]
		mov	[edi], al
		mov	al, [esi+1]
		mov	[edi+1], al
		mov	eax, [ebp+8]
		pop	esi
		pop	edi
		leave
		retn
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		align 4

loc_405DB4:				; CODE XREF: UnwindUpVec+63j
					; DATA XREF: UnwindUpVec+78o
		mov	al, [esi]
		mov	[edi], al
		mov	al, [esi+1]
		mov	[edi+1], al
		mov	al, [esi+2]
		mov	[edi+2], al
		mov	eax, [ebp+8]
		pop	esi
		pop	edi
		leave
		retn
UnwindUpVec	endp ; sp =  4

; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		align 4

loc_405DCC:				; CODE XREF: .text:00405C4Aj
		lea	esi, [ecx+esi-4]
		lea	edi, [ecx+edi-4]
		test	edi, 3
		jnz	short loc_405E00
		shr	ecx, 2
		and	edx, 3
		cmp	ecx, 8
		jb	short loc_405DF4
		std
		rep movsd
		cld
		jmp	dword ptr ds:TrailDownVec[edx*4]
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		align 4

loc_405DF4:				; CODE XREF: .text:00405DE5j
					; LeadDownVec+20j ...
		neg	ecx
		jmp	ds:off_405EC8[ecx*4]
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		align 10h

loc_405E00:				; CODE XREF: .text:00405DDAj
		mov	eax, edi
		mov	edx, 3
		cmp	ecx, 4
		jb	short near ptr dword_405E18
		and	eax, 3
		sub	ecx, eax
		jmp	ds:dword_405E1C[eax*4]
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
dword_405E18	dd 188D24FFh		; CODE XREF: .text:00405E0Aj
dword_405E1C	dd 9000405Fh		; DATA XREF: .text:00405E11r

; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


LeadDownVec	proc near
		sub	al, 5Eh
		inc	eax
		add	[eax+5Eh], dl
		inc	eax
		add	[eax+5Eh], bh
		inc	eax
		add	[edx-2EDCFCBAh], cl
		mov	[edi+3], al
		sub	esi, 1
		shr	ecx, 2
		sub	edi, 1
		cmp	ecx, 8
		jb	short loc_405DF4
		std
		rep movsd
		cld
		jmp	dword ptr ds:TrailDownVec[edx*4]
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		align 10h
		mov	al, [esi+3]
		and	edx, ecx
		mov	[edi+3], al
		mov	al, [esi+2]
		shr	ecx, 2
		mov	[edi+2], al
		sub	esi, 2
		sub	edi, 2
		cmp	ecx, 8
		jb	short loc_405DF4
		std
		rep movsd
		cld
		jmp	dword ptr ds:TrailDownVec[edx*4]
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		align 4
		mov	al, [esi+3]
		and	edx, ecx
		mov	[edi+3], al
		mov	al, [esi+2]
		mov	[edi+2], al
		mov	al, [esi+1]
		shr	ecx, 2
		mov	[edi+1], al
		sub	esi, 3
		sub	edi, 3
		cmp	ecx, 8
		jb	loc_405DF4
		std
		rep movsd
		cld
		jmp	dword ptr ds:TrailDownVec[edx*4]
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		align 4
UnwindDownVec	dd offset loc_405ECC
		dd offset loc_405ED4
		dd offset loc_405EDC
		dd offset loc_405EE4
		dd offset loc_405EEC
		dd offset loc_405EF4
		dd offset loc_405EFC
off_405EC8	dd offset loc_405F0F	; DATA XREF: .text:00405DF6r
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_405ECC:				; DATA XREF: LeadDownVec:UnwindDownVeco
		mov	eax, [esi+ecx*4+1Ch]
		mov	[edi+ecx*4+1Ch], eax

loc_405ED4:				; DATA XREF: LeadDownVec+90o
		mov	eax, [esi+ecx*4+18h]
		mov	[edi+ecx*4+18h], eax

loc_405EDC:				; DATA XREF: LeadDownVec+94o
		mov	eax, [esi+ecx*4+14h]
		mov	[edi+ecx*4+14h], eax

loc_405EE4:				; DATA XREF: LeadDownVec+98o
		mov	eax, [esi+ecx*4+10h]
		mov	[edi+ecx*4+10h], eax

loc_405EEC:				; DATA XREF: LeadDownVec+9Co
		mov	eax, [esi+ecx*4+0Ch]
		mov	[edi+ecx*4+0Ch], eax

loc_405EF4:				; DATA XREF: LeadDownVec+A0o
		mov	eax, [esi+ecx*4+8]
		mov	[edi+ecx*4+8], eax

loc_405EFC:				; DATA XREF: LeadDownVec+A4o
		mov	eax, [esi+ecx*4+4]
		mov	[edi+ecx*4+4], eax
		lea	eax, ds:0[ecx*4]
		add	esi, eax
		add	edi, eax

loc_405F0F:				; CODE XREF: .text:00405DF6j
					; DATA XREF: LeadDownVec:off_405EC8o
		jmp	dword ptr ds:TrailDownVec[edx*4]
LeadDownVec	endp

; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		align 4

; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


TrailDownVec	proc near		; DATA XREF: .text:00405DEBr
					; LeadDownVec+26r ...
		sub	[edi+40h], bl
		add	[eax], dh
		pop	edi
		inc	eax
		add	[eax+5Fh], al
		inc	eax
		add	[edi+ebx*2+40h], dl
		add	[ebx+5F5E0845h], cl
		leave
		retn
TrailDownVec	endp ; sp =  4

; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		align 10h
		mov	al, [esi+3]
		mov	[edi+3], al
		mov	eax, [ebp+8]
		pop	esi
		pop	edi
		leave
		retn
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		align 10h
		mov	al, [esi+3]
		mov	[edi+3], al
		mov	al, [esi+2]
		mov	[edi+2], al
		mov	eax, [ebp+8]
		pop	esi
		pop	edi
		leave
		retn
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		align 4
		mov	al, [esi+3]
		mov	[edi+3], al
		mov	al, [esi+2]
		mov	[edi+2], al
		mov	al, [esi+1]
		mov	[edi+1], al
		mov	eax, [ebp+8]
		pop	esi
		pop	edi
		leave
		retn

; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


_dosmaperr	proc near		; CODE XREF: _write+18Fp _lseek+5Fp ...

arg_0		= dword	ptr  4

		mov	eax, [esp+arg_0]
		mov	_doserrno, eax
		xor	ecx, ecx

loc_405F78:				; CODE XREF: _dosmaperr+18j
		cmp	eax, errtable[ecx*8]
		jz	short loc_405F9C
		inc	ecx
		cmp	ecx, 2Dh
		jb	short loc_405F78
		cmp	eax, 13h
		jb	short loc_405FA9
		cmp	eax, 24h
		ja	short loc_405FA9
		mov	errno, 0Dh
		retn
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_405F9C:				; CODE XREF: _dosmaperr+12j
		mov	eax, dword_40D064[ecx*8]
		mov	errno, eax
		retn
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_405FA9:				; CODE XREF: _dosmaperr+1Dj
					; _dosmaperr+22j
		cmp	eax, 0BCh
		jb	short loc_405FC1
		cmp	eax, 0CAh
		mov	errno, 8
		jbe	short locret_405FCB

loc_405FC1:				; CODE XREF: _dosmaperr+41j
		mov	errno, 16h

locret_405FCB:				; CODE XREF: _dosmaperr+52j
		retn
_dosmaperr	endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ

; Attributes: bp-based frame

_lseeki64	proc near		; CODE XREF: _write+73p

var_8		= dword	ptr -8
var_4		= dword	ptr -4
arg_0		= dword	ptr  8
arg_4		= dword	ptr  0Ch
arg_8		= dword	ptr  10h
arg_C		= dword	ptr  14h

		push	ebp
		mov	ebp, esp
		push	ecx
		push	ecx
		mov	eax, [ebp+arg_0]
		cmp	eax, _nhandle
		push	esi
		push	edi
		jnb	short loc_406050
		mov	ecx, eax
		sar	ecx, 5
		mov	esi, eax
		and	esi, 1Fh
		lea	edi, ds:40D980h[ecx*4]
		mov	ecx, [edi]
		shl	esi, 3
		test	byte ptr [ecx+esi+4], 1
		jz	short loc_406050
		mov	ecx, [ebp+arg_4]
		mov	[ebp+var_8], ecx
		mov	ecx, [ebp+arg_8]
		push	eax
		mov	[ebp+var_4], ecx
		call	_get_osfhandle
		cmp	eax, 0FFFFFFFFh
		pop	ecx
		jz	short loc_406057
		push	[ebp+arg_C]
		lea	ecx, [ebp+var_4]
		push	ecx
		push	[ebp+var_8]
		push	eax
		call	ds:__imp__SetFilePointer@16 ; __declspec(dllimport) SetFilePointer(x,x,x,x)
		cmp	eax, 0FFFFFFFFh
		mov	[ebp+var_8], eax
		jnz	short loc_40603F
		call	ds:__imp__GetLastError@0 ; __declspec(dllimport) GetLastError()
		test	eax, eax
		jz	short loc_40603F
		push	eax
		call	_dosmaperr
		pop	ecx
		jmp	short loc_406061
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_40603F:				; CODE XREF: _lseeki64+5Ej
					; _lseeki64+68j
		mov	eax, [edi]
		lea	eax, [eax+esi+4]
		and	byte ptr [eax],	0FDh
		mov	eax, [ebp+var_8]
		mov	edx, [ebp+var_4]
		jmp	short loc_406067
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_406050:				; CODE XREF: _lseeki64+10j
					; _lseeki64+2Dj
		and	_doserrno, 0

loc_406057:				; CODE XREF: _lseeki64+45j
		mov	errno, 9

loc_406061:				; CODE XREF: _lseeki64+71j
		or	eax, 0FFFFFFFFh
		or	edx, 0FFFFFFFFh

loc_406067:				; CODE XREF: _lseeki64+82j
		pop	edi
		pop	esi
		leave
		retn
_lseeki64	endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


_alloc_osfhnd	proc near		; CODE XREF: _open_osfhandle+53p
		push	ebx
		push	ebp
		push	esi
		push	edi
		or	ebx, 0FFFFFFFFh
		xor	esi, esi
		xor	edx, edx
		mov	ecx, offset __pioinfo
		mov	edi, 100h

loc_406080:				; CODE XREF: _alloc_osfhnd+54j
		mov	eax, [ecx]
		test	eax, eax
		jz	short loc_4060C3
		lea	ebp, [eax+100h]
		jmp	short loc_40609B
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_40608E:				; CODE XREF: _alloc_osfhnd+32j
		test	byte ptr [eax+4], 1
		jz	short loc_4060A1
		mov	ebp, [ecx]
		add	eax, 8
		add	ebp, edi

loc_40609B:				; CODE XREF: _alloc_osfhnd+21j
		cmp	eax, ebp
		jb	short loc_40608E
		jmp	short loc_4060B2
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_4060A1:				; CODE XREF: _alloc_osfhnd+27j
		or	dword ptr [eax], 0FFFFFFFFh
		sub	eax, [ecx]
		sar	eax, 3
		add	eax, edx
		mov	ebx, eax
		cmp	ebx, 0FFFFFFFFh
		jnz	short loc_406101

loc_4060B2:				; CODE XREF: _alloc_osfhnd+34j
		add	ecx, 4
		inc	esi
		add	edx, 20h
		cmp	ecx, offset __env_initialized
		jl	short loc_406080
		jmp	short loc_406101
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_4060C3:				; CODE XREF: _alloc_osfhnd+19j
		push	edi
		call	malloc
		test	eax, eax
		pop	ecx
		jz	short loc_406101
		add	_nhandle, 20h
		lea	ecx, ds:40D980h[esi*4]
		mov	[ecx], eax
		lea	edx, [eax+100h]
		jmp	short loc_4060F8
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_4060E6:				; CODE XREF: _alloc_osfhnd+8Fj
		or	dword ptr [eax], 0FFFFFFFFh
		mov	byte ptr [eax+4], 0
		mov	byte ptr [eax+5], 0Ah
		mov	edx, [ecx]
		add	eax, 8
		add	edx, edi

loc_4060F8:				; CODE XREF: _alloc_osfhnd+79j
		cmp	eax, edx
		jb	short loc_4060E6
		shl	esi, 5
		mov	ebx, esi

loc_406101:				; CODE XREF: _alloc_osfhnd+45j
					; _alloc_osfhnd+56j ...
		pop	edi
		pop	esi
		pop	ebp
		mov	eax, ebx
		pop	ebx
		retn
_alloc_osfhnd	endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


_set_osfhnd	proc near		; CODE XREF: _open_osfhandle+78p

arg_0		= dword	ptr  4
arg_4		= dword	ptr  8

		mov	eax, [esp+arg_0]
		cmp	eax, _nhandle
		push	esi
		push	edi
		jnb	short loc_406168
		mov	ecx, eax
		sar	ecx, 5
		mov	esi, eax
		and	esi, 1Fh
		lea	edi, ds:40D980h[ecx*4]
		mov	ecx, [edi]
		shl	esi, 3
		cmp	dword ptr [esi+ecx], 0FFFFFFFFh
		jnz	short loc_406168
		cmp	__app_type, 1
		push	ebx
		mov	ebx, [esp+0Ch+arg_4]
		jnz	short loc_40615E
		sub	eax, 0
		jz	short loc_406155
		dec	eax
		jz	short loc_406150
		dec	eax
		jnz	short loc_40615E
		push	ebx
		push	0FFFFFFF4h
		jmp	short loc_406158
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_406150:				; CODE XREF: _set_osfhnd+3Ej
		push	ebx
		push	0FFFFFFF5h
		jmp	short loc_406158
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_406155:				; CODE XREF: _set_osfhnd+3Bj
		push	ebx
		push	0FFFFFFF6h

loc_406158:				; CODE XREF: _set_osfhnd+46j
					; _set_osfhnd+4Bj
		call	ds:__imp__SetStdHandle@8 ; __declspec(dllimport) SetStdHandle(x,x)

loc_40615E:				; CODE XREF: _set_osfhnd+36j
					; _set_osfhnd+41j
		mov	eax, [edi]
		mov	[esi+eax], ebx
		xor	eax, eax
		pop	ebx
		jmp	short loc_40617C
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_406168:				; CODE XREF: _set_osfhnd+Cj
					; _set_osfhnd+28j
		and	_doserrno, 0
		mov	errno, 9
		or	eax, 0FFFFFFFFh

loc_40617C:				; CODE XREF: _set_osfhnd+5Ej
		pop	edi
		pop	esi
		retn
_set_osfhnd	endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


_free_osfhnd	proc near		; CODE XREF: _close+7Cp

arg_0		= dword	ptr  4

		mov	ecx, [esp+arg_0]
		cmp	ecx, _nhandle
		push	esi
		push	edi
		jnb	short loc_4061E2
		mov	eax, ecx
		sar	eax, 5
		mov	esi, ecx
		lea	edi, ds:40D980h[eax*4]
		mov	eax, [edi]
		and	esi, 1Fh
		shl	esi, 3
		add	eax, esi
		test	byte ptr [eax+4], 1
		jz	short loc_4061E2
		cmp	dword ptr [eax], 0FFFFFFFFh
		jz	short loc_4061E2
		cmp	__app_type, 1
		jnz	short loc_4061D8
		xor	eax, eax
		sub	ecx, eax
		jz	short loc_4061CF
		dec	ecx
		jz	short loc_4061CA
		dec	ecx
		jnz	short loc_4061D8
		push	eax
		push	0FFFFFFF4h
		jmp	short loc_4061D2
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_4061CA:				; CODE XREF: _free_osfhnd+41j
		push	eax
		push	0FFFFFFF5h
		jmp	short loc_4061D2
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_4061CF:				; CODE XREF: _free_osfhnd+3Ej
		push	eax
		push	0FFFFFFF6h

loc_4061D2:				; CODE XREF: _free_osfhnd+49j
					; _free_osfhnd+4Ej
		call	ds:__imp__SetStdHandle@8 ; __declspec(dllimport) SetStdHandle(x,x)

loc_4061D8:				; CODE XREF: _free_osfhnd+38j
					; _free_osfhnd+44j
		mov	eax, [edi]
		or	dword ptr [esi+eax], 0FFFFFFFFh
		xor	eax, eax
		jmp	short loc_4061F6
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_4061E2:				; CODE XREF: _free_osfhnd+Cj
					; _free_osfhnd+2Aj ...
		and	_doserrno, 0
		mov	errno, 9
		or	eax, 0FFFFFFFFh

loc_4061F6:				; CODE XREF: _free_osfhnd+61j
		pop	edi
		pop	esi
		retn
_free_osfhnd	endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


_get_osfhandle	proc near		; CODE XREF: _commit+25p _lseek+2Dp ...

arg_0		= dword	ptr  4

		mov	eax, [esp+arg_0]
		cmp	eax, _nhandle
		jnb	short loc_406220
		mov	ecx, eax
		sar	ecx, 5
		mov	ecx, __pioinfo[ecx*4]
		and	eax, 1Fh
		lea	eax, [ecx+eax*8]
		test	byte ptr [eax+4], 1
		jz	short loc_406220
		mov	eax, [eax]
		retn
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_406220:				; CODE XREF: _get_osfhandle+Aj
					; _get_osfhandle+22j
		and	_doserrno, 0
		mov	errno, 9
		or	eax, 0FFFFFFFFh
		retn
_get_osfhandle	endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ

; Attributes: bp-based frame

_open_osfhandle	proc near

arg_0		= dword	ptr  8
arg_4		= byte ptr  0Ch
arg_5		= byte ptr  0Dh

		push	ebp
		mov	ebp, esp
		push	ebx
		xor	bl, bl
		test	[ebp+arg_4], 8
		jz	short loc_406244
		add	bl, 20h

loc_406244:				; CODE XREF: _open_osfhandle+Aj
		test	[ebp+arg_5], 40h
		jz	short loc_40624D
		or	bl, 80h

loc_40624D:				; CODE XREF: _open_osfhandle+13j
		test	[ebp+arg_4], 80h
		jz	short loc_406256
		or	bl, 10h

loc_406256:				; CODE XREF: _open_osfhandle+1Cj
		push	[ebp+arg_0]
		call	ds:__imp__GetFileType@4	; __declspec(dllimport)	GetFileType(x)
		test	eax, eax
		jnz	short loc_406275
		call	ds:__imp__GetLastError@0 ; __declspec(dllimport) GetLastError()
		push	eax
		call	_dosmaperr
		pop	ecx
		or	eax, 0FFFFFFFFh
		jmp	short loc_4062CF
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_406275:				; CODE XREF: _open_osfhandle+2Cj
		cmp	eax, 2
		jnz	short loc_40627F
		or	bl, 40h
		jmp	short loc_406287
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_40627F:				; CODE XREF: _open_osfhandle+43j
		cmp	eax, 3
		jnz	short loc_406287
		or	bl, 8

loc_406287:				; CODE XREF: _open_osfhandle+48j
					; _open_osfhandle+4Dj
		push	esi
		call	_alloc_osfhnd
		mov	esi, eax
		or	eax, 0FFFFFFFFh
		cmp	esi, eax
		jnz	short loc_4062A9
		and	_doserrno, 0
		mov	errno, 18h
		jmp	short loc_4062CE
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_4062A9:				; CODE XREF: _open_osfhandle+5Fj
		push	[ebp+arg_0]
		push	esi
		call	_set_osfhnd
		pop	ecx
		mov	eax, esi
		sar	eax, 5
		mov	eax, __pioinfo[eax*4]
		pop	ecx
		or	bl, 1
		mov	ecx, esi
		and	ecx, 1Fh
		mov	[eax+ecx*8+4], bl
		mov	eax, esi

loc_4062CE:				; CODE XREF: _open_osfhandle+72j
		pop	esi

loc_4062CF:				; CODE XREF: _open_osfhandle+3Ej
		pop	ebx
		pop	ebp
		retn
_open_osfhandle	endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


_close		proc near		; CODE XREF: fclose+2Ap

arg_0		= dword	ptr  14h

		push	ebx
		push	ebp
		push	esi
		push	edi
		mov	edi, [esp+arg_0]
		cmp	edi, _nhandle
		jnb	loc_40636C
		mov	eax, edi
		sar	eax, 5
		mov	esi, edi
		and	esi, 1Fh
		lea	ebx, ds:40D980h[eax*4]
		mov	eax, [ebx]
		shl	esi, 3
		test	byte ptr [eax+esi+4], 1
		jz	short loc_40636C
		push	edi
		call	_get_osfhandle
		cmp	eax, 0FFFFFFFFh
		pop	ecx
		jz	short loc_40634B
		cmp	edi, 1
		jz	short loc_406319
		cmp	edi, 2
		jnz	short loc_40632F

loc_406319:				; CODE XREF: _close+40j
		push	2
		call	_get_osfhandle
		push	1
		mov	ebp, eax
		call	_get_osfhandle
		cmp	eax, ebp
		pop	ecx
		pop	ecx
		jz	short loc_40634B

loc_40632F:				; CODE XREF: _close+45j
		push	edi
		call	_get_osfhandle
		pop	ecx
		push	eax
		call	ds:__imp__CloseHandle@4	; __declspec(dllimport)	CloseHandle(x)
		test	eax, eax
		jnz	short loc_40634B
		call	ds:__imp__GetLastError@0 ; __declspec(dllimport) GetLastError()
		mov	ebp, eax
		jmp	short loc_40634D
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_40634B:				; CODE XREF: _close+3Bj _close+5Bj ...
		xor	ebp, ebp

loc_40634D:				; CODE XREF: _close+77j
		push	edi
		call	_free_osfhnd
		test	ebp, ebp
		mov	eax, [ebx]
		pop	ecx
		mov	byte ptr [eax+esi+4], 0
		jz	short loc_406368
		push	ebp
		call	_dosmaperr
		pop	ecx
		jmp	short loc_40637D
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_406368:				; CODE XREF: _close+8Bj
		xor	eax, eax
		jmp	short loc_406380
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_40636C:				; CODE XREF: _close+Ej	_close+2Fj
		and	_doserrno, 0
		mov	errno, 9

loc_40637D:				; CODE XREF: _close+94j
		or	eax, 0FFFFFFFFh

loc_406380:				; CODE XREF: _close+98j
		pop	edi
		pop	esi
		pop	ebp
		pop	ebx
		retn
_close		endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


_freebuf	proc near		; CODE XREF: fclose+22p

arg_0		= dword	ptr  8

		push	esi
		mov	esi, [esp+arg_0]
		mov	eax, [esi+0Ch]
		test	al, 83h
		jz	short loc_4063AE
		test	al, 8
		jz	short loc_4063AE
		push	dword ptr [esi+8]
		call	free
		and	word ptr [esi+0Ch], 0FBF7h
		xor	eax, eax
		pop	ecx
		mov	[esi], eax
		mov	[esi+8], eax
		mov	[esi+4], eax

loc_4063AE:				; CODE XREF: _freebuf+Aj _freebuf+Ej
		pop	esi
		retn
_freebuf	endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ

; Attributes: bp-based frame

__ansicp	proc near		; CODE XREF: __crtLCMapStringA+27Dp
					; __crtGetStringTypeA+15Ep

var_C		= dword	ptr -0Ch
var_6		= byte ptr -6
var_4		= dword	ptr -4
arg_0		= dword	ptr  8

		push	ebp
		mov	ebp, esp
		sub	esp, 0Ch
		mov	eax, __security_cookie
		push	6
		mov	[ebp+var_4], eax
		lea	eax, [ebp+var_C]
		push	eax
		push	1004h
		push	[ebp+arg_0]
		mov	[ebp+var_6], 0
		call	ds:__imp__GetLocaleInfoA@16 ; __declspec(dllimport) GetLocaleInfoA(x,x,x,x)
		test	eax, eax
		jnz	short loc_4063DF
		or	eax, 0FFFFFFFFh
		jmp	short loc_4063E9
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_4063DF:				; CODE XREF: __ansicp+28j
		lea	eax, [ebp+var_C]
		push	eax
		call	atol
		pop	ecx

loc_4063E9:				; CODE XREF: __ansicp+2Dj
		mov	ecx, [ebp+var_4]
		call	__security_check_cookie
		leave
		retn
__ansicp	endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


__convertcp	proc near		; CODE XREF: __crtLCMapStringA+2A8p
					; __crtLCMapStringA+366p ...
		push	38h
		push	offset dword_40AC60
		call	__SEH_prolog
		mov	eax, __security_cookie
		mov	[ebp-1Ch], eax
		xor	edi, edi
		mov	[ebp-34h], edi
		mov	[ebp-44h], edi
		mov	eax, [ebp+14h]
		mov	ebx, [eax]
		mov	[ebp-40h], ebx
		mov	[ebp-3Ch], edi
		mov	eax, [ebp+8]
		cmp	eax, [ebp+0Ch]
		jz	loc_406599
		lea	ecx, [ebp-30h]
		push	ecx
		push	eax
		mov	esi, ds:__imp__GetCPInfo@8 ; __declspec(dllimport) GetCPInfo(x,x)
		call	esi ; __declspec(dllimport) GetCPInfo(x,x) ; __declspec(dllimport) GetCPInfo(x,x)
		test	eax, eax
		jz	short loc_406457
		cmp	dword ptr [ebp-30h], 1
		jnz	short loc_406457
		lea	eax, [ebp-30h]
		push	eax
		push	dword ptr [ebp+0Ch]
		call	esi ; __declspec(dllimport) GetCPInfo(x,x) ; __declspec(dllimport) GetCPInfo(x,x)
		test	eax, eax
		jz	short loc_406457
		cmp	dword ptr [ebp-30h], 1
		jnz	short loc_406457
		mov	dword ptr [ebp-3Ch], 1

loc_406457:				; CODE XREF: __convertcp+42j
					; __convertcp+48j ...
		cmp	[ebp-3Ch], edi
		jz	short loc_406476
		cmp	ebx, 0FFFFFFFFh
		jz	short loc_406465
		mov	esi, ebx
		jmp	short loc_406471
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_406465:				; CODE XREF: __convertcp+6Cj
		push	dword ptr [ebp+10h]
		call	strlen
		pop	ecx
		mov	esi, eax
		inc	esi

loc_406471:				; CODE XREF: __convertcp+70j
		mov	[ebp-38h], esi
		jmp	short loc_406479
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_406476:				; CODE XREF: __convertcp+67j
		mov	esi, [ebp-38h]

loc_406479:				; CODE XREF: __convertcp+81j
		cmp	[ebp-3Ch], edi
		jnz	short loc_406498
		push	edi
		push	edi
		push	ebx
		push	dword ptr [ebp+10h]
		push	1
		push	dword ptr [ebp+8]
		call	ds:__imp__MultiByteToWideChar@24 ; __declspec(dllimport) MultiByteToWideChar(x,x,x,x,x,x)
		mov	esi, eax
		mov	[ebp-38h], esi
		cmp	esi, edi
		jz	short loc_4064F0

loc_406498:				; CODE XREF: __convertcp+89j
		mov	[ebp-4], edi
		lea	eax, [esi+esi]
		add	eax, 3
		and	eax, 0FFFFFFFCh
		call	_chkstk
		mov	[ebp-18h], esp
		mov	ebx, esp
		mov	[ebp-48h], ebx
		lea	eax, [esi+esi]
		push	eax
		push	edi
		push	ebx
		call	memset
		add	esp, 0Ch
		or	dword ptr [ebp-4], 0FFFFFFFFh
		jmp	short loc_4064DC
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		xor	eax, eax
		inc	eax
		retn
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		mov	esp, [ebp-18h]
		call	_resetstkoflw
		xor	edi, edi
		xor	ebx, ebx
		or	dword ptr [ebp-4], 0FFFFFFFFh
		mov	esi, [ebp-38h]

loc_4064DC:				; CODE XREF: __convertcp+D0j
		cmp	ebx, edi
		jnz	short loc_4064FE
		push	esi
		push	2
		call	calloc
		pop	ecx
		pop	ecx
		mov	ebx, eax
		cmp	ebx, edi
		jnz	short loc_4064F7

loc_4064F0:				; CODE XREF: __convertcp+A3j
		xor	eax, eax
		jmp	loc_4065AB
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_4064F7:				; CODE XREF: __convertcp+FBj
		mov	dword ptr [ebp-44h], 1

loc_4064FE:				; CODE XREF: __convertcp+EBj
		push	esi
		push	ebx
		push	dword ptr [ebp-40h]
		push	dword ptr [ebp+10h]
		push	1
		push	dword ptr [ebp+8]
		call	ds:__imp__MultiByteToWideChar@24 ; __declspec(dllimport) MultiByteToWideChar(x,x,x,x,x,x)
		test	eax, eax
		jz	loc_40659C
		cmp	[ebp+18h], edi
		jz	short loc_40653E
		push	edi
		push	edi
		push	dword ptr [ebp+1Ch]
		push	dword ptr [ebp+18h]
		push	esi
		push	ebx
		push	edi
		push	dword ptr [ebp+0Ch]
		call	ds:__imp__WideCharToMultiByte@32 ; __declspec(dllimport) WideCharToMultiByte(x,x,x,x,x,x,x,x)
		test	eax, eax
		jz	short loc_40659C
		mov	eax, [ebp+18h]
		mov	[ebp-34h], eax
		jmp	short loc_40659C
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_40653E:				; CODE XREF: __convertcp+129j
		cmp	[ebp-3Ch], edi
		jnz	short loc_406559
		push	edi
		push	edi
		push	edi
		push	edi
		push	esi
		push	ebx
		push	edi
		push	dword ptr [ebp+0Ch]
		call	ds:__imp__WideCharToMultiByte@32 ; __declspec(dllimport) WideCharToMultiByte(x,x,x,x,x,x,x,x)
		mov	esi, eax
		cmp	esi, edi
		jz	short loc_40659C

loc_406559:				; CODE XREF: __convertcp+14Ej
		push	esi
		push	1
		call	calloc
		pop	ecx
		pop	ecx
		mov	[ebp-34h], eax
		cmp	eax, edi
		jz	short loc_40659C
		push	edi
		push	edi
		push	esi
		push	eax
		push	esi
		push	ebx
		push	edi
		push	dword ptr [ebp+0Ch]
		call	ds:__imp__WideCharToMultiByte@32 ; __declspec(dllimport) WideCharToMultiByte(x,x,x,x,x,x,x,x)
		cmp	eax, edi
		jnz	short loc_40658C
		push	dword ptr [ebp-34h]
		call	free
		pop	ecx
		mov	[ebp-34h], edi
		jmp	short loc_40659C
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_40658C:				; CODE XREF: __convertcp+189j
		cmp	dword ptr [ebp-40h], 0FFFFFFFFh
		jz	short loc_40659C
		mov	ecx, [ebp+14h]
		mov	[ecx], eax
		jmp	short loc_40659C
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_406599:				; CODE XREF: __convertcp+2Dj
		mov	ebx, [ebp-48h]

loc_40659C:				; CODE XREF: __convertcp+120j
					; __convertcp+141j ...
		cmp	[ebp-44h], edi
		jz	short loc_4065A8
		push	ebx
		call	free
		pop	ecx

loc_4065A8:				; CODE XREF: __convertcp+1ACj
		mov	eax, [ebp-34h]

loc_4065AB:				; CODE XREF: __convertcp+FFj
		lea	esp, [ebp-54h]
		mov	ecx, [ebp-1Ch]
		call	__security_check_cookie
		call	__SEH_epilog
		retn
__convertcp	endp ; sp = -54h


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ

; Attributes: bp-based frame

_resetstkoflw	proc near		; CODE XREF: __crtLCMapStringA+FDp
					; __crtLCMapStringA+1CBp ...

var_58		= dword	ptr -58h
var_4C		= dword	ptr -4Ch
var_48		= dword	ptr -48h
var_28		= dword	ptr -28h
var_24		= dword	ptr -24h
var_1C		= dword	ptr -1Ch
var_18		= dword	ptr -18h
var_13		= byte ptr -13h
var_C		= dword	ptr -0Ch
var_8		= dword	ptr -8
var_4		= dword	ptr -4

		push	ebp
		mov	ebp, esp
		sub	esp, 4Ch
		push	ebx
		push	esi
		push	edi
		push	4
		pop	eax
		call	_chkstk
		mov	esi, esp
		push	1Ch
		lea	eax, [ebp+var_28]
		push	eax
		push	esi
		call	ds:__imp__VirtualQuery@12 ; __declspec(dllimport) VirtualQuery(x,x,x)
		test	eax, eax
		jz	short loc_406657
		mov	ebx, [ebp+var_24]
		lea	eax, [ebp+var_4C]
		push	eax
		call	ds:__imp__GetSystemInfo@4 ; __declspec(dllimport) GetSystemInfo(x)
		mov	ecx, [ebp+var_48]
		mov	eax, _osplatform
		lea	edi, [ecx-1]
		not	edi
		and	edi, esi
		sub	edi, ecx
		mov	esi, eax
		dec	esi
		neg	esi
		sbb	esi, esi
		and	esi, 0FFFFFFF1h
		add	esi, 11h
		imul	esi, ecx
		add	esi, ebx
		cmp	edi, esi
		mov	[ebp+var_8], ecx
		jb	short loc_406657
		cmp	eax, 1
		jz	short loc_406678
		mov	[ebp+var_4], ebx
		mov	ebx, 1000h

loc_406624:				; CODE XREF: _resetstkoflw+84j
		push	1Ch
		lea	eax, [ebp+var_28]
		push	eax
		push	[ebp+var_4]
		call	ds:__imp__VirtualQuery@12 ; __declspec(dllimport) VirtualQuery(x,x,x)
		test	eax, eax
		jz	short loc_406657
		mov	eax, [ebp+var_1C]
		add	[ebp+var_4], eax
		test	[ebp+var_18], ebx
		jz	short loc_406624
		test	[ebp+var_13], 1
		mov	eax, [ebp+var_28]
		mov	[ebp+var_4], eax
		jz	short loc_406653
		xor	eax, eax
		inc	eax
		jmp	short loc_406697
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_406653:				; CODE XREF: _resetstkoflw+90j
		cmp	edi, eax
		jnb	short loc_40665B

loc_406657:				; CODE XREF: _resetstkoflw+22j
					; _resetstkoflw+59j ...
		xor	eax, eax
		jmp	short loc_406697
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_40665B:				; CODE XREF: _resetstkoflw+99j
		cmp	eax, esi
		jnb	short loc_406662
		mov	[ebp+var_4], esi

loc_406662:				; CODE XREF: _resetstkoflw+A1j
		push	4
		push	ebx
		push	[ebp+var_8]
		push	[ebp+var_4]
		call	ds:__imp__VirtualAlloc@16 ; __declspec(dllimport) VirtualAlloc(x,x,x,x)
		mov	eax, _osplatform
		jmp	short loc_40667B
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_406678:				; CODE XREF: _resetstkoflw+5Ej
		mov	[ebp+var_4], edi

loc_40667B:				; CODE XREF: _resetstkoflw+BAj
		dec	eax
		neg	eax
		sbb	eax, eax
		and	eax, 103h
		lea	ecx, [ebp+var_C]
		push	ecx
		inc	eax
		push	eax
		push	[ebp+var_8]
		push	[ebp+var_4]
		call	ds:__imp__VirtualProtect@16 ; __declspec(dllimport) VirtualProtect(x,x,x,x)

loc_406697:				; CODE XREF: _resetstkoflw+95j
					; _resetstkoflw+9Dj
		lea	esp, [ebp-58h]
		pop	edi
		pop	esi
		pop	ebx
		leave
		retn
_resetstkoflw	endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


atol		proc near		; CODE XREF: __ansicp+33p atoij

arg_0		= dword	ptr  8

		push	esi
		mov	esi, [esp+arg_0]
		jmp	short loc_4066A7
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_4066A6:				; CODE XREF: atol+14j
		inc	esi

loc_4066A7:				; CODE XREF: atol+5j
		movzx	eax, byte ptr [esi]
		push	eax
		call	_ismbcspace
		test	eax, eax
		pop	ecx
		jnz	short loc_4066A6
		movzx	ecx, byte ptr [esi]
		inc	esi
		cmp	ecx, 2Dh
		mov	edx, ecx
		jz	short loc_4066C5
		cmp	ecx, 2Bh
		jnz	short loc_4066C9

loc_4066C5:				; CODE XREF: atol+1Fj
		movzx	ecx, byte ptr [esi]
		inc	esi

loc_4066C9:				; CODE XREF: atol+24j
		xor	eax, eax

loc_4066CB:				; CODE XREF: atol+4Dj
		cmp	ecx, 30h
		jl	short loc_4066DA
		cmp	ecx, 39h
		jg	short loc_4066DA
		sub	ecx, 30h
		jmp	short loc_4066DD
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_4066DA:				; CODE XREF: atol+2Fj atol+34j
		or	ecx, 0FFFFFFFFh

loc_4066DD:				; CODE XREF: atol+39j
		cmp	ecx, 0FFFFFFFFh
		jz	short loc_4066EE
		lea	eax, [eax+eax*4]
		lea	eax, [ecx+eax*2]
		movzx	ecx, byte ptr [esi]
		inc	esi
		jmp	short loc_4066CB
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_4066EE:				; CODE XREF: atol+41j
		cmp	edx, 2Dh
		pop	esi
		jnz	short locret_4066F6
		neg	eax

locret_4066F6:				; CODE XREF: atol+53j
		retn
atol		endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


atoi		proc near
		jmp	atol
atoi		endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


_atoi64		proc near

arg_0		= dword	ptr  14h

		push	ebx
		push	ebp
		push	esi
		push	edi
		mov	edi, [esp+arg_0]
		jmp	short loc_406707
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_406706:				; CODE XREF: _atoi64+17j
		inc	edi

loc_406707:				; CODE XREF: _atoi64+8j
		movzx	eax, byte ptr [edi]
		push	eax
		call	_ismbcspace
		test	eax, eax
		pop	ecx
		jnz	short loc_406706
		movzx	esi, byte ptr [edi]
		inc	edi
		cmp	esi, 2Dh
		mov	ebx, esi
		jz	short loc_406725
		cmp	esi, 2Bh
		jnz	short loc_406729

loc_406725:				; CODE XREF: _atoi64+22j
		movzx	esi, byte ptr [edi]
		inc	edi

loc_406729:				; CODE XREF: _atoi64+27j
		xor	eax, eax
		xor	edx, edx

loc_40672D:				; CODE XREF: _atoi64+66j
		cmp	esi, 30h
		jl	short loc_40673C
		cmp	esi, 39h
		jg	short loc_40673C
		sub	esi, 30h
		jmp	short loc_40673F
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_40673C:				; CODE XREF: _atoi64+34j _atoi64+39j
		or	esi, 0FFFFFFFFh

loc_40673F:				; CODE XREF: _atoi64+3Ej
		cmp	esi, 0FFFFFFFFh
		jz	short loc_406764
		push	0
		push	0Ah
		push	edx
		push	eax
		call	_allmul
		mov	ecx, eax
		mov	ebp, edx
		mov	eax, esi
		movzx	esi, byte ptr [edi]
		cdq
		add	ecx, eax
		adc	ebp, edx
		mov	eax, ecx
		mov	edx, ebp
		inc	edi
		jmp	short loc_40672D
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_406764:				; CODE XREF: _atoi64+46j
		pop	edi
		pop	esi
		pop	ebp
		cmp	ebx, 2Dh
		pop	ebx
		jnz	short locret_406774
		neg	eax
		adc	edx, 0
		neg	edx

locret_406774:				; CODE XREF: _atoi64+6Fj
		retn
_atoi64		endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ

; Attributes: bp-based frame

_ismbcspace	proc near		; CODE XREF: atol+Cp _atoi64+Fp

var_4		= dword	ptr -4
arg_0		= dword	ptr  8

		push	ebp
		mov	ebp, esp
		push	ecx
		mov	ecx, [ebp+arg_0]
		cmp	ecx, 0FFh
		jbe	short loc_4067E2
		and	word ptr [ebp+var_4], 0
		push	edi
		xor	eax, eax
		lea	edi, [ebp-2]
		stosw
		mov	eax, ecx
		shr	eax, 8
		cmp	__ismbcodepage,	0
		mov	byte ptr [ebp+arg_0+2],	al
		mov	byte ptr [ebp+arg_0+3],	cl
		pop	edi
		jnz	short loc_4067AA

loc_4067A6:				; CODE XREF: _ismbcspace+59j
					; _ismbcspace+60j ...
		xor	eax, eax
		leave
		retn
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_4067AA:				; CODE XREF: _ismbcspace+2Fj
		push	1
		push	__mblcid
		lea	eax, [ebp+var_4]
		push	__mbcodepage
		push	eax
		push	2
		lea	eax, [ebp+arg_0+2]
		push	eax
		push	1
		call	__crtGetStringTypeA
		add	esp, 1Ch
		test	eax, eax
		jz	short loc_4067A6
		cmp	word ptr [ebp+var_4+2],	0
		jnz	short loc_4067A6
		test	byte ptr [ebp+var_4], 8
		jz	short loc_4067A6
		xor	eax, eax
		inc	eax
		leave
		retn
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_4067E2:				; CODE XREF: _ismbcspace+Dj
		cmp	__mb_cur_max, 1
		jle	short loc_4067F7
		push	8
		push	ecx
		call	_isctype
		pop	ecx
		pop	ecx
		leave
		retn
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_4067F7:				; CODE XREF: _ismbcspace+74j
		mov	eax, _pctype
		movzx	eax, byte ptr [eax+ecx*2]
		and	eax, 8
		leave
		retn
_ismbcspace	endp

; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		align 10h

; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


_allmul		proc near		; CODE XREF: _atoi64+4Ep

arg_0		= dword	ptr  4
arg_4		= dword	ptr  8
arg_8		= dword	ptr  0Ch
arg_C		= dword	ptr  10h

		mov	eax, [esp+arg_4]
		mov	ecx, [esp+arg_C]
		or	ecx, eax
		mov	ecx, [esp+arg_8]
		jnz	short loc_406829
		mov	eax, [esp+arg_0]
		mul	ecx
		retn	10h
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_406829:				; CODE XREF: _allmul+Ej
		push	ebx
		mul	ecx
		mov	ebx, eax
		mov	eax, [esp+4+arg_0]
		mul	[esp+4+arg_C]
		add	ebx, eax
		mov	eax, [esp+4+arg_0]
		mul	ecx
		add	edx, ebx
		pop	ebx
		retn	10h
_allmul		endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


_chvalidator	proc near

arg_0		= dword	ptr  4
arg_4		= dword	ptr  8

		mov	eax, [esp+arg_0]
		mov	ecx, _pctype
		movzx	eax, word ptr [ecx+eax*2]
		and	eax, [esp+arg_4]
		retn
_chvalidator	endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ

; Attributes: bp-based frame

_isctype	proc near		; CODE XREF: _ismbcspace+79p

var_4		= byte ptr -4
var_2		= byte ptr -2
arg_0		= dword	ptr  8
arg_4		= dword	ptr  0Ch

		push	ebp
		mov	ebp, esp
		push	ecx
		mov	eax, [ebp+arg_0]
		lea	ecx, [eax+1]
		cmp	ecx, 100h
		ja	short loc_406875
		mov	ecx, _pctype
		movzx	eax, word ptr [ecx+eax*2]
		jmp	short loc_4068D0
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_406875:				; CODE XREF: _isctype+10j
		mov	ecx, eax
		sar	ecx, 8
		push	esi
		mov	esi, _pctype
		movzx	edx, cl
		test	byte ptr [esi+edx*2+1],	80h
		pop	esi
		jz	short loc_40689B
		push	2
		mov	[ebp-3], al
		mov	[ebp+var_4], cl
		mov	[ebp+var_2], 0
		pop	eax
		jmp	short loc_4068A5
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_40689B:				; CODE XREF: _isctype+33j
		mov	[ebp+var_4], al
		xor	eax, eax
		mov	byte ptr [ebp-3], 0
		inc	eax

loc_4068A5:				; CODE XREF: _isctype+42j
		push	1
		push	dword_40D614
		lea	ecx, [ebp+arg_0+2]
		push	__lc_codepage
		push	ecx
		push	eax
		lea	eax, [ebp+var_4]
		push	eax
		push	1
		call	__crtGetStringTypeA
		add	esp, 1Ch
		test	eax, eax
		jnz	short loc_4068CC
		leave
		retn
; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

loc_4068CC:				; CODE XREF: _isctype+71j
		movzx	eax, word ptr [ebp+arg_0+2]

loc_4068D0:				; CODE XREF: _isctype+1Cj
		and	eax, [ebp+arg_4]
		leave
		retn
_isctype	endp

; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		align 2

; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


; __stdcall GetModuleHandleA(x)
_GetModuleHandleA@4 proc near
		jmp	ds:__imp__GetModuleHandleA@4 ; __declspec(dllimport) GetModuleHandleA(x)
_GetModuleHandleA@4 endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


; __stdcall GetCommandLineA()
_GetCommandLineA@0 proc	near
		jmp	ds:__imp__GetCommandLineA@0 ; __declspec(dllimport) GetCommandLineA()
_GetCommandLineA@0 endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


; __stdcall GetVersionExA(x)
_GetVersionExA@4 proc near
		jmp	ds:__imp__GetVersionExA@4 ; Get	extended information about the
_GetVersionExA@4 endp			; version of the operating system


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


; __stdcall ExitProcess(x)
_ExitProcess@4	proc near
		jmp	ds:__imp__ExitProcess@4	; __declspec(dllimport)	ExitProcess(x)
_ExitProcess@4	endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


; __stdcall GetProcAddress(x,x)
_GetProcAddress@8 proc near
		jmp	ds:__imp__GetProcAddress@8 ; __declspec(dllimport) GetProcAddress(x,x)
_GetProcAddress@8 endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


; __stdcall TerminateProcess(x,x)
_TerminateProcess@8 proc near
		jmp	ds:__imp__TerminateProcess@8 ; __declspec(dllimport) TerminateProcess(x,x)
_TerminateProcess@8 endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


; __stdcall GetCurrentProcess()
_GetCurrentProcess@0 proc near
		jmp	ds:__imp__GetCurrentProcess@0 ;	__declspec(dllimport) GetCurrentProcess()
_GetCurrentProcess@0 endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


; __stdcall WriteFile(x,x,x,x,x)
_WriteFile@20	proc near
		jmp	ds:__imp__WriteFile@20 ; __declspec(dllimport) WriteFile(x,x,x,x,x)
_WriteFile@20	endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


; __stdcall GetStdHandle(x)
_GetStdHandle@4	proc near
		jmp	ds:__imp__GetStdHandle@4 ; __declspec(dllimport) GetStdHandle(x)
_GetStdHandle@4	endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


; __stdcall GetModuleFileNameA(x,x,x)
_GetModuleFileNameA@12 proc near
		jmp	ds:__imp__GetModuleFileNameA@12	; __declspec(dllimport)	GetModuleFileNameA(x,x,x)
_GetModuleFileNameA@12 endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


; __stdcall UnhandledExceptionFilter(x)
_UnhandledExceptionFilter@4 proc near
		jmp	ds:__imp__UnhandledExceptionFilter@4 ; __declspec(dllimport) UnhandledExceptionFilter(x)
_UnhandledExceptionFilter@4 endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


; __stdcall FreeEnvironmentStringsA(x)
_FreeEnvironmentStringsA@4 proc	near
		jmp	ds:__imp__FreeEnvironmentStringsA@4 ; __declspec(dllimport) FreeEnvironmentStringsA(x)
_FreeEnvironmentStringsA@4 endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


; __stdcall GetEnvironmentStrings()
_GetEnvironmentStrings@0 proc near
		jmp	ds:__imp__GetEnvironmentStrings@0 ; __declspec(dllimport) GetEnvironmentStrings()
_GetEnvironmentStrings@0 endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


; __stdcall FreeEnvironmentStringsW(x)
_FreeEnvironmentStringsW@4 proc	near
		jmp	ds:__imp__FreeEnvironmentStringsW@4 ; __declspec(dllimport) FreeEnvironmentStringsW(x)
_FreeEnvironmentStringsW@4 endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


; __stdcall WideCharToMultiByte(x,x,x,x,x,x,x,x)
_WideCharToMultiByte@32	proc near
		jmp	ds:__imp__WideCharToMultiByte@32 ; __declspec(dllimport) WideCharToMultiByte(x,x,x,x,x,x,x,x)
_WideCharToMultiByte@32	endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


; __stdcall GetLastError()
_GetLastError@0	proc near
		jmp	ds:__imp__GetLastError@0 ; __declspec(dllimport) GetLastError()
_GetLastError@0	endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


; __stdcall GetEnvironmentStringsW()
_GetEnvironmentStringsW@0 proc near
		jmp	ds:__imp__GetEnvironmentStringsW@0 ; __declspec(dllimport) GetEnvironmentStringsW()
_GetEnvironmentStringsW@0 endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


; __stdcall SetHandleCount(x)
_SetHandleCount@4 proc near
		jmp	ds:__imp__SetHandleCount@4 ; __declspec(dllimport) SetHandleCount(x)
_SetHandleCount@4 endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


; __stdcall GetFileType(x)
_GetFileType@4	proc near
		jmp	ds:__imp__GetFileType@4	; __declspec(dllimport)	GetFileType(x)
_GetFileType@4	endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


; __stdcall GetStartupInfoA(x)
_GetStartupInfoA@4 proc	near
		jmp	ds:__imp__GetStartupInfoA@4 ; __declspec(dllimport) GetStartupInfoA(x)
_GetStartupInfoA@4 endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


; __stdcall HeapDestroy(x)
_HeapDestroy@4	proc near
		jmp	ds:__imp__HeapDestroy@4	; __declspec(dllimport)	HeapDestroy(x)
_HeapDestroy@4	endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


; __stdcall HeapCreate(x,x,x)
_HeapCreate@12	proc near
		jmp	ds:__imp__HeapCreate@12	; __declspec(dllimport)	HeapCreate(x,x,x)
_HeapCreate@12	endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


; __stdcall VirtualFree(x,x,x)
_VirtualFree@12	proc near
		jmp	ds:__imp__VirtualFree@12 ; __declspec(dllimport) VirtualFree(x,x,x)
_VirtualFree@12	endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


; __stdcall HeapFree(x,x,x)
_HeapFree@12	proc near
		jmp	ds:__imp__HeapFree@12 ;	__declspec(dllimport) HeapFree(x,x,x)
_HeapFree@12	endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


; __stdcall HeapAlloc(x,x,x)
_HeapAlloc@12	proc near
		jmp	ds:__imp__HeapAlloc@12 ; __declspec(dllimport) HeapAlloc(x,x,x)
_HeapAlloc@12	endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


; __stdcall LoadLibraryA(x)
_LoadLibraryA@4	proc near
		jmp	ds:__imp__LoadLibraryA@4 ; __declspec(dllimport) LoadLibraryA(x)
_LoadLibraryA@4	endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


; __stdcall GetACP()
_GetACP@0	proc near
		jmp	ds:__imp__GetACP@0 ; __declspec(dllimport) GetACP()
_GetACP@0	endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


; __stdcall GetOEMCP()
_GetOEMCP@0	proc near
		jmp	ds:__imp__GetOEMCP@0 ; __declspec(dllimport) GetOEMCP()
_GetOEMCP@0	endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


; __stdcall GetCPInfo(x,x)
_GetCPInfo@8	proc near
		jmp	ds:__imp__GetCPInfo@8 ;	__declspec(dllimport) GetCPInfo(x,x)
_GetCPInfo@8	endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


; __stdcall VirtualAlloc(x,x,x,x)
_VirtualAlloc@16 proc near
		jmp	ds:__imp__VirtualAlloc@16 ; __declspec(dllimport) VirtualAlloc(x,x,x,x)
_VirtualAlloc@16 endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


; __stdcall HeapReAlloc(x,x,x,x)
_HeapReAlloc@16	proc near
		jmp	ds:__imp__HeapReAlloc@16 ; __declspec(dllimport) HeapReAlloc(x,x,x,x)
_HeapReAlloc@16	endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


; __stdcall IsBadWritePtr(x,x)
_IsBadWritePtr@8 proc near
		jmp	ds:__imp__IsBadWritePtr@8 ; __declspec(dllimport) IsBadWritePtr(x,x)
_IsBadWritePtr@8 endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


; __stdcall RtlUnwind(x,x,x,x)
_RtlUnwind@16	proc near		; CODE XREF: __global_unwind2+13p
		jmp	ds:__imp__RtlUnwind@16 ; __declspec(dllimport) RtlUnwind(x,x,x,x)
_RtlUnwind@16	endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


; __stdcall InterlockedExchange(x,x)
_InterlockedExchange@8 proc near
		jmp	ds:__imp__InterlockedExchange@8	; __declspec(dllimport)	InterlockedExchange(x,x)
_InterlockedExchange@8 endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


; __stdcall VirtualQuery(x,x,x)
_VirtualQuery@12 proc near
		jmp	ds:__imp__VirtualQuery@12 ; __declspec(dllimport) VirtualQuery(x,x,x)
_VirtualQuery@12 endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


; __stdcall FlushFileBuffers(x)
_FlushFileBuffers@4 proc near
		jmp	ds:__imp__FlushFileBuffers@4 ; __declspec(dllimport) FlushFileBuffers(x)
_FlushFileBuffers@4 endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


; __stdcall SetFilePointer(x,x,x,x)
_SetFilePointer@16 proc	near
		jmp	ds:__imp__SetFilePointer@16 ; __declspec(dllimport) SetFilePointer(x,x,x,x)
_SetFilePointer@16 endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


; __stdcall QueryPerformanceCounter(x)
_QueryPerformanceCounter@4 proc	near
		jmp	ds:__imp__QueryPerformanceCounter@4 ; __declspec(dllimport) QueryPerformanceCounter(x)
_QueryPerformanceCounter@4 endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


; __stdcall GetTickCount()
_GetTickCount@0	proc near
		jmp	ds:__imp__GetTickCount@0 ; __declspec(dllimport) GetTickCount()
_GetTickCount@0	endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


; __stdcall GetCurrentThreadId()
_GetCurrentThreadId@0 proc near
		jmp	ds:__imp__GetCurrentThreadId@0 ; __declspec(dllimport) GetCurrentThreadId()
_GetCurrentThreadId@0 endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


; __stdcall GetCurrentProcessId()
_GetCurrentProcessId@0 proc near
		jmp	ds:__imp__GetCurrentProcessId@0	; __declspec(dllimport)	GetCurrentProcessId()
_GetCurrentProcessId@0 endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


; __stdcall GetSystemTimeAsFileTime(x)
_GetSystemTimeAsFileTime@4 proc	near
		jmp	ds:__imp__GetSystemTimeAsFileTime@4 ; __declspec(dllimport) GetSystemTimeAsFileTime(x)
_GetSystemTimeAsFileTime@4 endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


; __stdcall HeapSize(x,x,x)
_HeapSize@12	proc near
		jmp	ds:__imp__HeapSize@12 ;	__declspec(dllimport) HeapSize(x,x,x)
_HeapSize@12	endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


; __stdcall LCMapStringA(x,x,x,x,x,x)
_LCMapStringA@24 proc near
		jmp	ds:__imp__LCMapStringA@24 ; __declspec(dllimport) LCMapStringA(x,x,x,x,x,x)
_LCMapStringA@24 endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


; __stdcall MultiByteToWideChar(x,x,x,x,x,x)
_MultiByteToWideChar@24	proc near
		jmp	ds:__imp__MultiByteToWideChar@24 ; __declspec(dllimport) MultiByteToWideChar(x,x,x,x,x,x)
_MultiByteToWideChar@24	endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


; __stdcall LCMapStringW(x,x,x,x,x,x)
_LCMapStringW@24 proc near
		jmp	ds:__imp__LCMapStringW@24 ; __declspec(dllimport) LCMapStringW(x,x,x,x,x,x)
_LCMapStringW@24 endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


; __stdcall GetStringTypeA(x,x,x,x,x)
_GetStringTypeA@20 proc	near
		jmp	ds:__imp__GetStringTypeA@20 ; __declspec(dllimport) GetStringTypeA(x,x,x,x,x)
_GetStringTypeA@20 endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


; __stdcall GetStringTypeW(x,x,x,x)
_GetStringTypeW@16 proc	near
		jmp	ds:__imp__GetStringTypeW@16 ; __declspec(dllimport) GetStringTypeW(x,x,x,x)
_GetStringTypeW@16 endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


; __stdcall SetStdHandle(x,x)
_SetStdHandle@8	proc near
		jmp	ds:__imp__SetStdHandle@8 ; __declspec(dllimport) SetStdHandle(x,x)
_SetStdHandle@8	endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


; __stdcall CloseHandle(x)
_CloseHandle@4	proc near
		jmp	ds:__imp__CloseHandle@4	; __declspec(dllimport)	CloseHandle(x)
_CloseHandle@4	endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


; __stdcall GetLocaleInfoA(x,x,x,x)
_GetLocaleInfoA@16 proc	near
		jmp	ds:__imp__GetLocaleInfoA@16 ; __declspec(dllimport) GetLocaleInfoA(x,x,x,x)
_GetLocaleInfoA@16 endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


; __stdcall VirtualProtect(x,x,x,x)
_VirtualProtect@16 proc	near
		jmp	ds:__imp__VirtualProtect@16 ; __declspec(dllimport) VirtualProtect(x,x,x,x)
_VirtualProtect@16 endp


; ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ S U B	R O U T	I N E ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ


; __stdcall GetSystemInfo(x)
_GetSystemInfo@4 proc near
		jmp	ds:__imp__GetSystemInfo@4 ; __declspec(dllimport) GetSystemInfo(x)
_GetSystemInfo@4 endp

; ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ'
		db 'ÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌÌ',0
		align 1000h
_text		ends

; Section 2. (virtual address 0000A000)
; Virtual size			: 0000169C (   5788.)
; Section size in file		: 00002000 (   8192.)
; Offset to raw	data for section: 0000A000
; Flags	40000040: Data Readable
; Alignment	: 16 bytes ?
; ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ

; Segment type:	Pure data
; Segment permissions: Read
_rdata		segment	para public 'DATA' use32
		assume cs:_rdata
		;org 40A000h
		dd 0, 41BEC41Fh, 0, 2
byte_40A010	db 27h			; DATA XREF: _output+5Er
		align 4
		dd 2 dup(0B044h), 0
dword_40A020	dd 0FFFFFFFFh, 4013B4h,	4013C8h, 0 ; DATA XREF:	mainCRTStartup+2o
__lookuptable	db 6			; DATA XREF: _output:loc_4015CBr
		dd 60000h, 10000001h, 60300h, 4100206h,	5454545h, 5050505h
		dd 50003035h, 0, 50382820h, 80758h, 57303037h, 750h, 82020h
		dd 8000000h, 60606860h,	6060h, 78787070h, 7087878h, 7000008h
		dd 8080800h, 80000h, 8070008h
		align 4
??_C@_1O@CEDCILHN@?$AA?$CI?$AAn?$AAu?$AAl?$AAl?$AA?$CJ?$AA?$AA@:
		unicode	0, <(null)>,0
		align 4
??_C@_06OJHGLDPL@?$CInull?$CJ?$AA@ db '(null)',0
		align 4
??_C@_0P@MIGLKIOC@CorExitProcess?$AA@ db 'CorExitProcess',0 ; DATA XREF: __crtExitProcess+Fo
		align 4
??_C@_0M@MBOPBNBK@mscoree?4dll?$AA@ db 'mscoree.dll',0 ; DATA XREF: __crtExitProcesso
??_C@_0P@GGKDKEEG@runtime?5error?5?$AA@	db 'runtime error ',0
		align 10h
??_C@_02PCIJFNDE@?$AN?6?$AA@ db	0Dh,0Ah,0
		align 4
??_C@_0O@MJNIPGOH@TLOSS?5error?$AN?6?$AA@ db 'TLOSS error',0Dh,0Ah,0
		align 4
??_C@_0N@BONNPDHF@SING?5error?$AN?6?$AA@ db 'SING error',0Dh,0Ah,0
		align 4
??_C@_0P@MBDAJNE@DOMAIN?5error?$AN?6?$AA@ db 'DOMAIN error',0Dh,0Ah,0
		dd 0
		align 4
??_C@_0KC@BDDGGBEJ@R6029?$AN?6?9?5This?5application?5cannot@ db	'R6029',0Dh,0Ah
		db '- This application cannot run using the active version of th'
		db 'e Microsoft .NET Runtime',0Ah
		db 'Please contact the application',27h,'s support team for more'
		db ' information.',0Dh,0Ah,0
		align 4
??_C@_0CF@KJDAIALP@R6028?$AN?6?9?5unable?5to?5initialize?5he@ db 'R6028',0Dh,0Ah
		db '- unable to initialize heap',0Dh,0Ah,0
		align 4
??_C@_0DF@GAKBPDFP@R6027?$AN?6?9?5not?5enough?5space?5for?5lo@ db 'R6027',0Dh,0Ah
		db '- not enough space for lowio initialization',0Dh,0Ah,0
		align 4
??_C@_0DF@IBLNNAIG@R6026?$AN?6?9?5not?5enough?5space?5for?5st@ db 'R6026',0Dh,0Ah
		db '- not enough space for stdio initialization',0Dh,0Ah,0
		align 4
??_C@_0CG@IOHKGFNJ@R6025?$AN?6?9?5pure?5virtual?5function?5c@ db 'R6025',0Dh,0Ah
		db '- pure virtual function call',0Dh,0Ah,0
		align 4
??_C@_0DF@JIJJFJDC@R6024?$AN?6?9?5not?5enough?5space?5for?5_o@ db 'R6024',0Dh,0Ah
		db '- not enough space for _onexit/atexit table',0Dh,0Ah,0
		align 4
??_C@_0CJ@EGEJHFME@R6019?$AN?6?9?5unable?5to?5open?5console?5@ db 'R6019',0Dh,0Ah
		db '- unable to open console device',0Dh,0Ah,0
		align 10h
??_C@_0CB@GNHNIELH@R6018?$AN?6?9?5unexpected?5heap?5error?$AN?6@ db 'R6018',0Dh,0Ah
		db '- unexpected heap error',0Dh,0Ah,0
		align 4
??_C@_0CN@FOLDKNG@R6017?$AN?6?9?5unexpected?5multithread?5@ db 'R6017',0Dh,0Ah
		db '- unexpected multithread lock error',0Dh,0Ah,0
		align 4
??_C@_0CM@FAHFJNHO@R6016?$AN?6?9?5not?5enough?5space?5for?5th@ db 'R6016',0Dh,0Ah
		db '- not enough space for thread data',0Dh,0Ah,0
??_C@_0JG@LBKMGKLD@?$AN?6This?5application?5has?5requested@ db 0Dh,0Ah
		db 'This application has requested the Runtime to terminate it i'
		db 'n an unusual way.',0Ah
		db 'Please contact the application',27h,'s support team for more'
		db ' information.',0Dh,0Ah,0
		align 4
??_C@_0CM@FLCNIPKK@R6009?$AN?6?9?5not?5enough?5space?5for?5en@ db 'R6009',0Dh,0Ah
		db '- not enough space for environment',0Dh,0Ah,0
??_C@_0CK@KMMJOAPP@R6008?$AN?6?9?5not?5enough?5space?5for?5ar@ db 'R6008',0Dh,0Ah
		db '- not enough space for arguments',0Dh,0Ah,0
		align 10h
??_C@_0CF@HOHMHDEK@R6002?$AN?6?9?5floating?5point?5not?5load@ db 'R6002',0Dh,0Ah
		db '- floating point not loaded',0Dh,0Ah,0
		align 4
??_C@_0CF@GOGNBNAK@Microsoft?5Visual?5C?$CL?$CL?5Runtime?5Lib@ db 'Microsoft Visual C++ Runtime Library',0
					; DATA XREF: _NMSG_WRITE+123o
					; __security_error_handler+132o
		align 10h
??_C@_02PHMGELLB@?6?6?$AA@ db 0Ah	; DATA XREF: _NMSG_WRITE+107o
					; __security_error_handler+FCo
		db 0Ah,0
		align 4
??_C@_0BK@OFGJDLJJ@Runtime?5Error?$CB?6?6Program?3?5?$AA@ db 'Runtime Error!',0Ah
					; DATA XREF: _NMSG_WRITE+F5o
		db 0Ah
		db 'Program: ',0
		align 10h
??_C@_03KHICJKCI@?4?4?4?$AA@ db	'...',0 ; DATA XREF: _NMSG_WRITE+C1o
					; __security_error_handler+CCo
??_C@_0BH@DNAGHKFM@?$DMprogram?5name?5unknown?$DO?$AA@ db '<program name unknown>',0
					; DATA XREF: _NMSG_WRITE+8Eo
					; __security_error_handler+8Bo
		dd 0
		align 10h
dword_40A4D0	dd 0FFFFFFFFh, 402979h,	40297Dh, 0 ; DATA XREF:	_RTC_Initialize+2o
dword_40A4E0	dd 0FFFFFFFFh, 4029BDh,	4029C1h, 0 ; DATA XREF:	_RTC_Terminate+2o
__newctype	dd 40h dup(0), 4 dup(200020h), 280020h,	2 dup(280028h)
		dd 9 dup(200020h), 100048h, 7 dup(100010h), 5 dup(840084h)
		dd 3 dup(100010h), 810010h, 2 dup(810081h), 10081h, 9 dup(10001h)
		dd 100001h, 2 dup(100010h), 820010h, 2 dup(820082h), 20082h
		dd 9 dup(20002h), 100002h, 100010h, 200010h, 40h dup(0)
_wctype		dd 200000h, 4 dup(200020h), 280068h, 280028h, 200028h
		dd 8 dup(200020h), 480020h, 7 dup(100010h), 840010h, 4 dup(840084h)
		dd 100084h, 3 dup(100010h), 3 dup(1810181h), 0Ah dup(1010101h)
		dd 3 dup(100010h), 3 dup(1820182h), 0Ah	dup(1020102h)
		dd 2 dup(100010h), 10h dup(200020h), 480020h, 8	dup(100010h)
		dd 140010h, 100014h, 2 dup(100010h), 100014h, 2	dup(100010h)
		dd 1010010h, 0Bh dup(1010101h),	1010010h, 3 dup(1010101h)
		dd 0Ch dup(1020102h), 1020010h,	3 dup(1020102h), 1010102h
		dd 0
dword_40A9F8	dd 0FFFFFFFFh, 40308Ah,	40308Eh	; DATA XREF: report_failure+2o
??_C@_0BI@DFKBFLJE@GetProcessWindowStation?$AA@	db 'GetProcessWindowStation',0
					; DATA XREF: __crtMessageBoxA+73o
??_C@_0BK@CIDNPOGP@GetUserObjectInformationA?$AA@ db 'GetUserObjectInformationA',0
					; DATA XREF: __crtMessageBoxA+62o
		align 4
??_C@_0BD@HHGDFDBJ@GetLastActivePopup?$AA@ db 'GetLastActivePopup',0
					; DATA XREF: __crtMessageBoxA+47o
		align 4
??_C@_0BA@HNOPNCHB@GetActiveWindow?$AA@	db 'GetActiveWindow',0
					; DATA XREF: __crtMessageBoxA+3Fo
??_C@_0M@CHKKJDAI@MessageBoxA?$AA@ db 'MessageBoxA',0 ; DATA XREF: __crtMessageBoxA+2Eo
??_C@_0L@GMPLCCII@user32?4dll?$AA@ db 'user32.dll',0 ; DATA XREF: __crtMessageBoxA+13o
		align 4
??_C@_09KLGCKDOD@Program?3?5?$AA@ db 'Program: ',0
					; DATA XREF: __security_error_handler+108o
		align 10h
??_C@_0KA@JEHFIFGP@A?5buffer?5overrun?5has?5been?5detect@ db 'A buffer overrun has been detected which has corrupted the p'
					; DATA XREF: __security_error_handler+62o
		db 'rogram',27h,'s',0Ah
		db 'internal state.  The program cannot safely continue executio'
		db 'n and must',0Ah
		db 'now be terminated.',0Ah,0
??_C@_0BJ@MDEKIEOJ@Buffer?5overrun?5detected?$CB?$AA@ db 'Buffer overrun detected!',0
					; DATA XREF: __security_error_handler:loc_405327o
		dd 0
		align 10h
??_C@_0LB@BPLHHLFD@A?5security?5error?5of?5unknown?5caus@ db 'A security error of unknown cause has been detected which ha'
					; DATA XREF: __security_error_handler+4Co
		db 's',0Ah
		db 'corrupted the program',27h,'s internal state.  The program c'
		db 'annot safely',0Ah
		db 'continue execution and must now be terminated.',0Ah,0
		align 4
??_C@_0CD@GMKACBEK@Unknown?5security?5failure?5detecte@	db 'Unknown security failure detected!',0
					; DATA XREF: __security_error_handler+47o
		align 4
dword_40AC18	dd 0FFFFFFFFh, 405302h,	405306h	; DATA XREF: __security_error_handler+5o
??_C@_13NOLLCAOD@?$AA?$AA?$AA?$AA@ dd 0	; DATA XREF: __crtLCMapStringA+1Co
					; __crtGetStringTypeA+1Eo
dword_40AC28	dd 0FFFFFFFFh, 4059A5h,	4059A9h, 0FFFFFFFFh, 4057A2h, 4057A6h
					; DATA XREF: __crtLCMapStringA+2o
		dd 0FFFFFFFFh, 405870h,	405874h, 0
dword_40AC50	dd 0FFFFFFFFh, 405B41h,	405B45h, 0 ; DATA XREF:	__crtGetStringTypeA+2o
dword_40AC60	dd 0FFFFFFFFh, 4064C5h,	4064C9h, 0 ; DATA XREF:	__convertcp+2o
_load_config_used dd 48h, 0Eh dup(0), 40CF30h, 40B180h,	2, 0E3h	dup(0),	53445352h
		dd 57675CE1h, 49B2AF61h, 0B4136BCh, 3947BBF2h, 4, 545C3A43h
		dd 5C706D65h, 702E3176h, 6264h,	45h dup(0)
__safe_se_handler_table	dd 2B18h, 4C1Ch, 41h dup(0)
__rtc_iaa	dd 0
dword_40B290	dd 40h dup(0)		; DATA XREF: _RTC_Initialize+Co
__rtc_izz	dd 41h dup(0)		; DATA XREF: _RTC_Initialize:loc_40295Fo
__rtc_taa	dd 0
dword_40B498	dd 40h dup(0)		; DATA XREF: _RTC_Terminate+Co
__rtc_tzz	dd 29Ah	dup(0)		; DATA XREF: _RTC_Terminate:loc_4029A3o
_rdata		ends

; Section 3. (virtual address 0000C000)
; Virtual size			: 00002DF0 (  11760.)
; Section size in file		: 00002000 (   8192.)
; Offset to raw	data for section: 0000C000
; Flags	C0000040: Data Readable	Writable
; Alignment	: 16 bytes ?
; ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ

; Segment type:	Pure data
; Segment permissions: Read/Write
_data		segment	para public 'DATA' use32
		assume cs:_data
		;org 40C000h
__xc_a		dd 41h dup(0)		; DATA XREF: _cinit+45o
init_cookie	dd offset __security_init_cookie
		dd 40h dup(0)
__xc_z		dd 41h dup(0)		; DATA XREF: _cinit+4Co
__xi_a		dd 41h dup(0)		; DATA XREF: _cinit+12o
pinit		dd offset __initstdio
		dd offset __onexitinit
		dd offset __initmbctable
		dd 41h dup(0)
__xi_z		dd 41h dup(0)		; DATA XREF: _cinit+17o
__xp_a		dd 41h dup(0)		; DATA XREF: doexit:loc_401F50o
pterm		dd offset __endstdio
		dd 40h dup(0)
__xp_z		dd 41h dup(0)		; DATA XREF: doexit+6Co
__xt_a		dd 41h dup(0)		; DATA XREF: doexit:loc_401F6Fo
__xt_z		dd 43h dup(0)		; DATA XREF: doexit+8Bo
aBuffS		db 'buff : %s',0Ah,0    ; DATA XREF: main+23o
		dd 0
		align 10h
_aexit_rtn	dd offset _exit		; DATA XREF: _amsg_exit+1Cr
__app_type	dd 1			; DATA XREF: _NMSG_WRITE+58r
					; _FF_MSGBANNER+Er ...
__nullstring	dd 40A09Ch		; DATA XREF: _output:loc_401957r
					; _output+51Cr
__wnullstring	dd 40A08Ch		; DATA XREF: _output+2D8r
_iob		dd offset _bufin	; DATA XREF: __iob_funco
					; __initstdio+52o
		dd 0, 40DDE0h, 101h
dword_40CB70	dd 2 dup(0), 1000h, 0	; DATA XREF: __initstdio+71o
dword_40CB80	dd 3 dup(0), 2,	1, 3 dup(0) ; DATA XREF: printf+3o _stbuf+12o	...
dword_40CBA0	dd 3 dup(0), 2 dup(2), 7 dup(0)	; DATA XREF: _stbuf:loc_40140Co
					; _flsbuf+5Bo
dword_40CBD0	dd 84h dup(0)		; DATA XREF: __initstdio+9Ao
rterrs		dd 2			; DATA XREF: __initstdio+67o
					; _NMSG_WRITE:loc_402010r ...
dword_40CDE4	dd 40A440h		; DATA XREF: _NMSG_WRITE+D5r
					; _NMSG_WRITE+112r ...
		dd 8, 40A414h, 9, 40A3E8h, 0Ah,	40A350h, 10h, 40A324h
		dd 11h,	40A2F4h, 12h, 40A2D0h, 13h, 40A2A4h, 18h, 40A26Ch
		dd 19h,	40A244h, 1Ah, 40A20Ch, 1Bh, 40A1D4h, 1Ch, 40A1ACh
		dd 1Dh,	40A108h, 78h, 40A0F4h, 79h, 40A0E4h, 7Ah, 40A0D4h
		dd 0FCh, 40A0D0h, 0FFh,	40A0C0h
_XcptActTab	dd 0C0000005h, 0Bh, 0, 0C000001Dh, 4, 0, 0C0000096h, 4
					; DATA XREF: xcptlookup+6o
					; _XcptFilter+Co
		dd 0, 0C000008Dh, 8, 0,	0C000008Eh, 8, 0, 0C000008Fh, 8
		dd 0, 0C0000090h, 8, 0,	0C0000091h, 8, 0, 0C0000092h, 8
		dd 0, 0C0000093h, 8, 0
_First_FPE_Indx	dd 3			; DATA XREF: _XcptFilter+84r
_Num_FPE	dd 7			; DATA XREF: _XcptFilter+89r
_XcptActTabCount dd 0Ah			; DATA XREF: xcptlookupr
					; _XcptFilter+6r
_fpecode	dd 8Ch			; DATA XREF: _XcptFilter+B2r
					; _XcptFilter+BAw ...
__badioinfo	dd 0FFFFFFFFh, 0A80h	; DATA XREF: _flsbuf:loc_402ED3o
_amblksiz	dd 10h,	0
_cfltcvt_tab	dd offset _fptrap	; DATA XREF: _output+476r
off_40CF14	dd offset _fptrap	; DATA XREF: _output+4A2r
		dd offset _fptrap
off_40CF1C	dd offset _fptrap	; DATA XREF: _output+491r
		dd offset _fptrap
		dd offset _fptrap
_pctype		dd 40A5F0h		; DATA XREF: _output:loc_40177Br
					; __pctype_funcr ...
_pwctype	dd 40A7F2h		; DATA XREF: __pwctype_funcr
__security_cookie dd 0BB40E64Eh		; DATA XREF: _output+Er _NMSG_WRITE+Er ...
		dd 3 dup(0)
__rgctypeflag	db 1			; DATA XREF: _setmbcp+120r
		dd 80402h
		align 4
__rgcode_page_info dd 3A4h		; DATA XREF: _setmbcp:loc_403799r
dword_40CF4C	dd 82798260h		; DATA XREF: _setmbcp+15Cr
		dd 21h,	0
dword_40CF58	dd 0DFA6h		; DATA XREF: _setmbcp+100r
		dd 0, 0A5A1h, 0, 0FCE09F81h, 0,	0FC807E40h, 0, 3A8h, 0A3DAA3C1h
		dd 20h,	5 dup(0), 0FE81h, 0, 0FE40h, 0,	3B5h, 0A3DAA3C1h
		dd 20h,	5 dup(0), 0FE81h, 0, 0FE41h, 0,	3B6h, 0A2E4A2CFh
		dd 0A2E5001Ah, 5BA2E8h,	4 dup(0), 0FE81h, 0, 0FEA17E40h
		dd 0, 551h, 0DA5EDA51h,	0DA5F0020h, 32DA6Ah, 4 dup(0)
		dd 0DED8D381h, 0F9E0h, 0FE817E31h, 3 dup(0)
__NLG_Destination dd 19930520h,	3 dup(0) ; DATA	XREF: _NLG_Notify1+2o
					; _NLG_Notify+2o
__lc_clike	dd 1
__mb_cur_max	dd 1			; DATA XREF: wctomb+30r
					; _ismbcspace:loc_4067E2r
__decimal_point	dd 2Eh
__decimal_point_length dd 1
errtable	dd 1			; DATA XREF: _dosmaperr:loc_405F78r
dword_40D064	dd 16h			; DATA XREF: _dosmaperr:loc_405F9Cr
		dd 2 dup(2), 3,	2, 4, 18h, 5, 0Dh, 6, 9, 7, 0Ch, 8, 0Ch
		dd 9, 0Ch, 0Ah,	7, 0Bh,	8, 0Ch,	16h, 0Dh, 16h, 0Fh, 2
		dd 10h,	0Dh, 11h, 2 dup(12h), 2, 21h, 0Dh, 35h,	2, 41h
		dd 0Dh,	43h, 2,	50h, 11h, 52h, 0Dh, 53h, 0Dh, 57h, 16h
		dd 59h,	0Bh, 6Ch, 0Dh, 6Dh, 20h, 70h, 1Ch, 72h,	9, 6, 16h
		dd 80h,	0Ah, 81h, 0Ah, 82h, 9, 83h, 16h, 84h, 0Dh, 91h
		dd 29h,	9Eh, 0Dh, 0A1h,	2, 0A4h, 0Bh, 0A7h, 0Dh, 0B7h
		dd 11h,	0CEh, 2, 0D7h, 0Bh, 718h, 0Ch, 96h dup(0)
_aenvptr	dd 0			; DATA XREF: mainCRTStartup+11Aw
					; _setenvp:loc_402399r	...
_wenvptr	dd 0
__error_mode	dd 0			; DATA XREF: _amsg_exitr
					; fast_error_exitr ...
_stdbuf		dd 2 dup(0)
_cflush		dd 0			; DATA XREF: _stbuf:loc_401417w
					; _getbufw
errno		dd 0			; DATA XREF: wctomb:loc_403051w
					; _write:loc_4050F3w ...
_doserrno	dd 0			; DATA XREF: _write+149w _write+1B9w ...
_umaskval	dd 0
_osplatform	dd 0			; DATA XREF: mainCRTStartup+29w
					; __heap_selectr ...
_osver		dd 0			; DATA XREF: mainCRTStartup+49w
					; mainCRTStartup+5Aw
_winver		dd 0			; DATA XREF: mainCRTStartup+65w
_winmajor	dd 0			; DATA XREF: mainCRTStartup+32w
					; __heap_select+9r ...
_winminor	dd 0			; DATA XREF: mainCRTStartup+3Aw
__argc		dd 0			; DATA XREF: mainCRTStartup+168r
					; _setargv+8Fw
__argv		dd 0			; DATA XREF: mainCRTStartup+162r
					; _setargv+95w
__wargv		dd 0
_environ	dd 0			; DATA XREF: mainCRTStartup:loc_40137Er
					; _setenvp+48w	...
__initenv	dd 0			; DATA XREF: mainCRTStartup+15Cw
_wenviron	dd 0
__winitenv	dd 0
_pgmptr		dd 0			; DATA XREF: _setargv+37w
_wpgmptr	dd 0
_exitflag	db 0			; DATA XREF: __endstdio+5r doexit+2Dw
		align 10h
_C_Termination_Done dd 0		; DATA XREF: doexit+27w
_C_Exit_Done	dd 0			; DATA XREF: doexit+7r	doexit+B0w
_adbgmsg	dd 0			; DATA XREF: _FF_MSGBANNER+21r
_pxcptinfoptrs	dd 0			; DATA XREF: _XcptFilter+68r
					; _XcptFilter+73w ...
dword_40D490	dd 41h dup(0)		; DATA XREF: _setargv+1Co
byte_40D594	db 0			; DATA XREF: _setargv+23w
		align 4
dword_40D598	dd 0			; DATA XREF: __crtGetEnvironmentStringsA+2r
					; __crtGetEnvironmentStringsA+24w ...
dword_40D59C	dd 0			; DATA XREF: __crtMessageBoxA+9r
					; __crtMessageBoxA+38w	...
dword_40D5A0	dd 0			; DATA XREF: __crtMessageBoxA+4Dw
					; __crtMessageBoxA:loc_403399r
dword_40D5A4	dd 0			; DATA XREF: __crtMessageBoxA+5Bw
					; __crtMessageBoxA+D6r
dword_40D5A8	dd 0			; DATA XREF: __crtMessageBoxA+7Bw
					; __crtMessageBoxA:loc_403354r
dword_40D5AC	dd 0			; DATA XREF: __crtMessageBoxA+6Cw
					; __crtMessageBoxA+9Cr
fSystemSet	dd 0			; DATA XREF: getSystemCPw
					; getSystemCP+Cw ...
		dd 0
nValidPages	dd 0			; DATA XREF: _ValidateEH3RN:loc_404D71r
					; _ValidateEH3RN+13Fr ...
		dd 0
rgValidPages	dd 0			; DATA XREF: _ValidateEH3RN:loc_404D84r
					; _ValidateEH3RN+1C4r ...
		dd 0Fh dup(0)
lModifying	dd 0			; DATA XREF: _ValidateEH3RN+12Co
					; _ValidateEH3RN+191o ...
_pnhHeap	dd 0			; DATA XREF: _set_new_handler+4r
					; _set_new_handler+9w ...
_newmode	dd 0			; DATA XREF: mallocr
					; calloc:loc_403198r ...
__lc_handle	dd 2 dup(0)
dword_40D614	dd 0			; DATA XREF: wctomb:loc_40300Cr
					; __crtLCMapStringA+265r ...
		dd 3 dup(0)
__lc_codepage	dd 0			; DATA XREF: wctomb+41r
					; getSystemCP+36r ...
__lc_collate_cp	dd 0
user_handler	dd 0			; DATA XREF: __security_error_handler+17r
					; __security_error_handler+14Cr ...
dword_40D630	dd 0			; DATA XREF: __crtLCMapStringA+Er
					; __crtLCMapStringA+31w ...
dword_40D634	dd 0			; DATA XREF: __crtGetStringTypeA+Er
					; __crtGetStringTypeA+2Ew ...
__sbh_initialized dd 5 dup(0)
__sbh_pHeaderDefer dd 0			; DATA XREF: __sbh_heap_init+21w
					; __sbh_free_block+21Cr ...
__sbh_cntHeaderList dd 0		; DATA XREF: _heap_term+Cr
					; _heap_term+56r ...
__sbh_pHeaderList dd 0			; DATA XREF: _heap_term+1Cr
					; _heap_term:loc_402A9Fr ...
__sbh_threshold	dd 0			; DATA XREF: _heap_alloc+Er calloc+29r ...
__sbh_pHeaderScan dd 0			; DATA XREF: __sbh_heap_init+2Fw
					; __sbh_free_block+300w ...
__sbh_sizeHeaderList dd	0		; DATA XREF: __sbh_heap_init+3Cw
					; __sbh_alloc_new_region+5r ...
__sbh_indGroupDefer dd 0		; DATA XREF: __sbh_free_block+229r
					; __sbh_free_block+249r ...
__mblcid	dd 0			; DATA XREF: setSBCS+1Aw
					; setSBUpLow+84r ...
		dd 1Dh dup(0)
__ismbcodepage	dd 0			; DATA XREF: setSBCS+15w _setmbcp+14Dw ...
		dd 7 dup(0)
_mbctype	db 0			; DATA XREF: setSBCS+6o _setmbcp+A7o ...
byte_40D701	db 0			; DATA XREF: parse_cmdline+47r
					; parse_cmdline+11Dr ...
		dd 40h dup(0)
		align 4
__mbcodepage	dd 0			; DATA XREF: setSBCS+10w
					; setSBUpLow+16r ...
		dd 2 dup(0)
__mbulinfo	dd 4 dup(0)		; DATA XREF: setSBCS+1Fo _setmbcp+162o ...
_mbcasemap	db 0			; DATA XREF: setSBUpLow:loc_4036A4w
					; setSBUpLow:loc_4036C1w ...
		dd 3Fh dup(0)
		align 10h
_crtheap	dd 0			; DATA XREF: _heap_init+19w
					; _heap_init+3Er ...
		dd 3 dup(0)
__active_heap	dd 0			; DATA XREF: _heap_init+28w
					; _heap_termr ...
_nhandle	dd 0			; DATA XREF: _ioinit+1Fw
					; _ioinit:loc_402807r ...
		dd 12h dup(0)
__pioinfo	dd 0			; DATA XREF: __initstdio+7Br
					; _ioinit:loc_402798w ...
dword_40D984	dd 3Fh dup(0)		; DATA XREF: _ioinit+91o
__env_initialized dd 0			; DATA XREF: _setenvp+9Fw _ioterm+19o	...
		dd 3 dup(0)
__onexitend	dd 0			; DATA XREF: doexit+3Er
					; doexit:loc_401F3Br ...
__onexitbegin	dd 0			; DATA XREF: doexit+34r doexit+5Ar ...
__mbctype_initialized dd 0		; DATA XREF: _setenvp+3r _setargv+Ar ...
_FPinit		dd 0			; DATA XREF: _cinitr
__piob		dd 0			; DATA XREF: __initstdio+2Bw
					; __initstdio+44w ...
		dd 0CFh	dup(0)
_bufin		dd 88h dup(0)		; DATA XREF: .data:_iobo
		dd 378h	dup(?)
_nstream	dd ?			; DATA XREF: __initstdior
					; __initstdio:loc_401D79w ...
_acmdln		dd ?			; DATA XREF: mainCRTStartup+110w
					; _setargv+30r
		align 10h
_data		ends

;
; Imports from KERNEL32
;
; Section 4. (virtual address 0000F000)
; Virtual size			: 00000791 (   1937.)
; Section size in file		: 00001000 (   4096.)
; Offset to raw	data for section: 0000E000
; Flags	C0000040: Data Readable	Writable
; Alignment	: 16 bytes ?
; ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ

; Segment type:	Externs
; _idata
; __declspec(dllimport)	__stdcall GetModuleHandleA(x)
		extrn __imp__GetModuleHandleA@4:dword ;	DATA XREF: check_managed_app+2r
					; mainCRTStartup+6Dr ...
; __declspec(dllimport)	__stdcall GetCommandLineA()
		extrn __imp__GetCommandLineA@0:dword ; DATA XREF: mainCRTStartup:loc_401331r
					; GetCommandLineA()r
; __declspec(dllimport)	__stdcall GetVersionExA(x)
		extrn __imp__GetVersionExA@4:dword ; DATA XREF:	mainCRTStartup+20r
					; GetVersionExA(x)r
					; Get extended information about the
					; version of the operating system
; __declspec(dllimport)	__stdcall ExitProcess(x)
		extrn __imp__ExitProcess@4:dword ; DATA	XREF: __crtExitProcess+29r
					; report_failure+2Ar ...
; __declspec(dllimport)	__stdcall GetProcAddress(x,x)
		extrn __imp__GetProcAddress@8:dword ; DATA XREF: __crtExitProcess+15r
					; __crtMessageBoxA+28r	...
; __declspec(dllimport)	__stdcall TerminateProcess(x,x)
		extrn __imp__TerminateProcess@8:dword ;	DATA XREF: doexit+1Ar
					; TerminateProcess(x,x)r
; __declspec(dllimport)	__stdcall GetCurrentProcess()
		extrn __imp__GetCurrentProcess@0:dword ; DATA XREF: doexit+13r
					; GetCurrentProcess()r
; __declspec(dllimport)	__stdcall WriteFile(x,x,x,x,x)
		extrn __imp__WriteFile@20:dword	; DATA XREF: _NMSG_WRITE+155r
					; _write+F4r ...
; __declspec(dllimport)	__stdcall GetStdHandle(x)
		extrn __imp__GetStdHandle@4:dword ; DATA XREF: _NMSG_WRITE+14Er
					; _ioinit+157r	...
; __declspec(dllimport)	__stdcall GetModuleFileNameA(x,x,x)
		extrn __imp__GetModuleFileNameA@12:dword ; DATA	XREF: _NMSG_WRITE+81r
					; _setargv+2Ar	...
; __declspec(dllimport)	__stdcall UnhandledExceptionFilter(x)
		extrn __imp__UnhandledExceptionFilter@4:dword ;	DATA XREF: _XcptFilter+167r
					; UnhandledExceptionFilter(x)r
; __declspec(dllimport)	__stdcall FreeEnvironmentStringsA(x)
		extrn __imp__FreeEnvironmentStringsA@4:dword
					; DATA XREF: __crtGetEnvironmentStringsA+113r
					; FreeEnvironmentStringsA(x)r
; __declspec(dllimport)	__stdcall GetEnvironmentStrings()
		extrn __imp__GetEnvironmentStrings@0:dword
					; DATA XREF: __crtGetEnvironmentStringsA:loc_402733r
					; GetEnvironmentStrings()r
; __declspec(dllimport)	__stdcall FreeEnvironmentStringsW(x)
		extrn __imp__FreeEnvironmentStringsW@4:dword
					; DATA XREF: __crtGetEnvironmentStringsA+C1r
					; FreeEnvironmentStringsW(x)r
; __declspec(dllimport)	__stdcall WideCharToMultiByte(x,x,x,x,x,x,x,x)
		extrn __imp__WideCharToMultiByte@32:dword
					; DATA XREF: __crtGetEnvironmentStringsA:loc_4026CBr
					; __crtGetEnvironmentStringsA+86r ...
; __declspec(dllimport)	__stdcall GetLastError()
		extrn __imp__GetLastError@0:dword
					; DATA XREF: __crtGetEnvironmentStringsA:loc_40268Cr
					; _write:loc_405061r ...
; __declspec(dllimport)	__stdcall GetEnvironmentStringsW()
		extrn __imp__GetEnvironmentStringsW@0:dword
					; DATA XREF: __crtGetEnvironmentStringsA+Br
					; __crtGetEnvironmentStringsA+1Cr ...
; __declspec(dllimport)	__stdcall SetHandleCount(x)
		extrn __imp__SetHandleCount@4:dword ; DATA XREF: _ioinit+19Cr
					; SetHandleCount(x)r
; __declspec(dllimport)	__stdcall GetFileType(x)
		extrn __imp__GetFileType@4:dword ; DATA	XREF: _ioinit+FEr
					; _ioinit+165r	...
; __declspec(dllimport)	__stdcall GetStartupInfoA(x)
		extrn __imp__GetStartupInfoA@4:dword ; DATA XREF: _ioinit+57r
					; GetStartupInfoA(x)r
; __declspec(dllimport)	__stdcall HeapDestroy(x)
		extrn __imp__HeapDestroy@4:dword ; DATA	XREF: _heap_init+44r
					; _heap_term+78r ...
; __declspec(dllimport)	__stdcall HeapCreate(x,x,x)
		extrn __imp__HeapCreate@12:dword ; DATA	XREF: _heap_init+11r
					; HeapCreate(x,x,x)r
; __declspec(dllimport)	__stdcall VirtualFree(x,x,x)
		extrn __imp__VirtualFree@12:dword ; DATA XREF: _heap_term+23r
					; _heap_term+38r ...
; __declspec(dllimport)	__stdcall HeapFree(x,x,x)
		extrn __imp__HeapFree@12:dword ; DATA XREF: _heap_term+13r
					; _heap_term+50r ...
; __declspec(dllimport)	__stdcall HeapAlloc(x,x,x)
		extrn __imp__HeapAlloc@12:dword	; DATA XREF: _heap_alloc+3Er
					; calloc+47r ...
; __declspec(dllimport)	__stdcall LoadLibraryA(x)
		extrn __imp__LoadLibraryA@4:dword ; DATA XREF: __crtMessageBoxA+18r
					; LoadLibraryA(x)r
; __declspec(dllimport)	__stdcall GetACP()
		extrn __imp__GetACP@0:dword ; DATA XREF: getSystemCP+2Br
					; _setmbcp+42r	...
; __declspec(dllimport)	__stdcall GetOEMCP()
		extrn __imp__GetOEMCP@0:dword ;	DATA XREF: getSystemCP+16r
					; _setmbcp+2Br	...
; __declspec(dllimport)	__stdcall GetCPInfo(x,x)
		extrn __imp__GetCPInfo@8:dword ; DATA XREF: setSBUpLow+1Cr
					; _setmbcp+93r	...
; __declspec(dllimport)	__stdcall VirtualAlloc(x,x,x,x)
		extrn __imp__VirtualAlloc@16:dword ; DATA XREF:	__sbh_alloc_new_region+7Er
					; __sbh_alloc_new_group+52r ...
; __declspec(dllimport)	__stdcall HeapReAlloc(x,x,x,x)
		extrn __imp__HeapReAlloc@16:dword ; DATA XREF: __sbh_alloc_new_region+27r
					; realloc+FDr ...
; __declspec(dllimport)	__stdcall IsBadWritePtr(x,x)
		extrn __imp__IsBadWritePtr@8:dword ; DATA XREF:	__sbh_heap_check+1Br
					; __sbh_heap_check+55r	...
; __declspec(dllimport)	__stdcall RtlUnwind(x,x,x,x)
		extrn __imp__RtlUnwind@16:dword	; DATA XREF: RtlUnwind(x,x,x,x)r
; __declspec(dllimport)	__stdcall InterlockedExchange(x,x)
		extrn __imp__InterlockedExchange@8:dword ; DATA	XREF: _ValidateEH3RN+131r
					; _ValidateEH3RN+196r ...
; __declspec(dllimport)	__stdcall VirtualQuery(x,x,x)
		extrn __imp__VirtualQuery@12:dword ; DATA XREF:	_ValidateEH3RN+B3r
					; _resetstkoflw+1Ar ...
; __declspec(dllimport)	__stdcall FlushFileBuffers(x)
		extrn __imp__FlushFileBuffers@4:dword ;	DATA XREF: _commit+2Cr
					; FlushFileBuffers(x)r
; __declspec(dllimport)	__stdcall SetFilePointer(x,x,x,x)
		extrn __imp__SetFilePointer@16:dword ; DATA XREF: _lseek+43r
					; _lseeki64+52r ...
; __declspec(dllimport)	__stdcall QueryPerformanceCounter(x)
		extrn __imp__QueryPerformanceCounter@4:dword
					; DATA XREF: __security_init_cookie+43r
					; QueryPerformanceCounter(x)r
; __declspec(dllimport)	__stdcall GetTickCount()
		extrn __imp__GetTickCount@0:dword ; DATA XREF: __security_init_cookie+37r
					; GetTickCount()r
; __declspec(dllimport)	__stdcall GetCurrentThreadId()
		extrn __imp__GetCurrentThreadId@0:dword
					; DATA XREF: __security_init_cookie+2Fr
					; GetCurrentThreadId()r
; __declspec(dllimport)	__stdcall GetCurrentProcessId()
		extrn __imp__GetCurrentProcessId@0:dword
					; DATA XREF: __security_init_cookie+27r
					; GetCurrentProcessId()r
; __declspec(dllimport)	__stdcall GetSystemTimeAsFileTime(x)
		extrn __imp__GetSystemTimeAsFileTime@4:dword
					; DATA XREF: __security_init_cookie+1Br
					; GetSystemTimeAsFileTime(x)r
; __declspec(dllimport)	__stdcall HeapSize(x,x,x)
		extrn __imp__HeapSize@12:dword ; DATA XREF: _msize+30r
					; HeapSize(x,x,x)r
; __declspec(dllimport)	__stdcall LCMapStringA(x,x,x,x,x,x)
		extrn __imp__LCMapStringA@24:dword ; DATA XREF:	__crtLCMapStringA+2C3r
					; __crtLCMapStringA+344r ...
; __declspec(dllimport)	__stdcall MultiByteToWideChar(x,x,x,x,x,x)
		extrn __imp__MultiByteToWideChar@24:dword ; DATA XREF: __crtLCMapStringA+C0r
					; __crtLCMapStringA+141r ...
; __declspec(dllimport)	__stdcall LCMapStringW(x,x,x,x,x,x)
		extrn __imp__LCMapStringW@24:dword ; DATA XREF:	__crtLCMapStringA+27r
					; __crtLCMapStringA+15Br ...
; __declspec(dllimport)	__stdcall GetStringTypeA(x,x,x,x,x)
		extrn __imp__GetStringTypeA@20:dword ; DATA XREF: __crtGetStringTypeA+19Cr
					; GetStringTypeA(x,x,x,x,x)r
; __declspec(dllimport)	__stdcall GetStringTypeW(x,x,x,x)
		extrn __imp__GetStringTypeW@16:dword ; DATA XREF: __crtGetStringTypeA+24r
					; __crtGetStringTypeA+128r ...
; __declspec(dllimport)	__stdcall SetStdHandle(x,x)
		extrn __imp__SetStdHandle@8:dword ; DATA XREF: _set_osfhnd:loc_406158r
					; _free_osfhnd:loc_4061D2r ...
; __declspec(dllimport)	__stdcall CloseHandle(x)
		extrn __imp__CloseHandle@4:dword ; DATA	XREF: _close+65r
					; CloseHandle(x)r
; __declspec(dllimport)	__stdcall GetLocaleInfoA(x,x,x,x)
		extrn __imp__GetLocaleInfoA@16:dword ; DATA XREF: __ansicp+20r
					; GetLocaleInfoA(x,x,x,x)r
; __declspec(dllimport)	__stdcall VirtualProtect(x,x,x,x)
		extrn __imp__VirtualProtect@16:dword ; DATA XREF: _resetstkoflw+D5r
					; VirtualProtect(x,x,x,x)r
; __declspec(dllimport)	__stdcall GetSystemInfo(x)
		extrn __imp__GetSystemInfo@4:dword ; DATA XREF:	_resetstkoflw+2Br
					; GetSystemInfo(x)r
		extrn _KERNEL32_NULL_THUNK_DATA:dword


		end mainCRTStartup
