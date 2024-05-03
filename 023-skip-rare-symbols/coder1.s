	.text
	.def	@feat.00;
	.scl	3;
	.type	0;
	.endef
	.globl	@feat.00
.set @feat.00, 0
	.intel_syntax noprefix
	.file	"fpaq0mw.cpp"
	.def	encode_sim;
	.scl	2;
	.type	32;
	.endef
	.globl	encode_sim                      # -- Begin function encode_sim
	.p2align	4, 0x90
encode_sim:                             # @encode_sim
.seh_proc encode_sim
# %bb.0:
	push	rsi
	.seh_pushreg rsi
	push	rdi
	.seh_pushreg rdi
	push	rbp
	.seh_pushreg rbp
	push	rbx
	.seh_pushreg rbx
	.seh_endprologue
	mov	r9d, edx
	movsxd	r10, dword ptr [rcx + 1540]
	movzx	r8d, word ptr [rcx + 2*r10 + 516]
	movzx	r11d, word ptr [rcx + 2*r10 + 4]
	movzx	esi, word ptr [rcx + 2*r10 + 1028]
	lea	eax, [8*r11]
	lea	edx, [8*r8]
	mov	edi, 32768
	mov	ebx, 32768
	sub	ebx, edx
	mov	ebp, 32868
	sub	ebp, eax
	mov	edx, 32668
	cmp	ebx, ebp
	jg	.LBB0_2
# %bb.1:
	mov	ebp, 32668
	sub	ebp, eax
	cmp	ebx, ebp
	jl	.LBB0_2
# %bb.3:
	test	r9d, r9d
	je	.LBB0_5
.LBB0_4:
	mov	edx, 4096
	mov	eax, 4096
	sub	eax, r11d
	shr	eax, 4
	add	eax, r11d
	sub	edx, r8d
	shr	edx, 8
	add	edx, r8d
	jmp	.LBB0_6
.LBB0_2:
	mov	ebp, 32768
	sub	ebp, eax
	test	r9d, r9d
	mov	eax, 100
	cmove	eax, edx
	sub	eax, ebp
	imul	eax, eax, 5461
	sub	ebx, ebp
	cdq
	idiv	ebx
	imul	edx, esi, 27307
	shr	edx, 15
	add	eax, edx
	cmp	eax, 32768
	cmovge	eax, edi
	xor	edx, edx
	test	eax, eax
	cmovns	edx, eax
push rcx
lea rcx,[rcx + 2*r10 + 1028]
call "track2"
pop rcx
	mov	word ptr [rcx + 2*r10 + 1028], dx
	test	r9d, r9d
	jne	.LBB0_4
.LBB0_5:
	mov	edx, r11d
	shr	edx, 4
	mov	eax, r11d
	sub	eax, edx
	mov	edi, r8d
	shr	edi, 8
	mov	edx, r8d
	sub	edx, edi
.LBB0_6:
	sub	r8d, r11d
	imul	r8d, esi
	sar	r8d, 15
	add	r8d, r11d
push rcx
lea rcx,[rcx + 2*r10 + 4]
call "track2"
pop rcx
	mov	word ptr [rcx + 2*r10 + 4], ax
push rcx
lea rcx,[rcx + 2*r10 + 516]
call "track2"
pop rcx
	mov	word ptr [rcx + 2*r10 + 516], dx
	lea	eax, [r9 + 2*r10]
	cmp	eax, 256
	mov	edx, 1
	cmovl	edx, eax
push rcx
lea rcx,[rcx + 1540]
call "track4"
pop rcx
	mov	dword ptr [rcx + 1540], edx
	mov	eax, r8d
	pop	rbx
	pop	rbp
	pop	rdi
	pop	rsi
	ret
	.seh_endproc
                                        # -- End function
	.section	.drectve,"yni"
	.ascii	" /DEFAULTLIB:libcmt.lib"
	.ascii	" /DEFAULTLIB:libircmt.lib"
	.ascii	" /DEFAULTLIB:svml_dispmt.lib"
	.ascii	" /DEFAULTLIB:libdecimal.lib"
	.ascii	" /DEFAULTLIB:libmmt.lib"
	.ascii	" /DEFAULTLIB:oldnames.lib"
	.ascii	" /FAILIFMISMATCH:\"_CRT_STDIO_ISO_WIDE_SPECIFIERS=0\""
.def track2; .scl 2; .type 32; .endef
.def track4; .scl 2; .type 32; .endef
