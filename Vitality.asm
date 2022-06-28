global VitalityDetour

section .text

OriginalCode:
    movss [rax+rcx*4],xmm6

    leave                   ; Short for mov rsp, rbp + pop rbp, returns the stack registers to how they were pre-alignment
    ret

VitalityDetour:
	push rbp                ; Stack alignment
    mov rbp, rsp

    cmp dword [rax+0x20],3  ; compare user type
    jne OriginalCode        ; if not equal 3, call original code

    leave                   ; Short for mov rsp, rbp + pop rbp, returns the stack registers to how they were pre-alignment
    ret