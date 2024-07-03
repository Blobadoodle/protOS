global reloadSegments
reloadSegments:
	; Reload CS register:
	push 0x08 ; Kernel code segment
	lea rax, [rel .reload_CS] ; load adress of .reload_CS into RAX
	push rax
	retfq ; far return
.reload_CS:
	; reload data segment registers
	mov ax, 0x10 ; Kernel data segment
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	ret