; hello.asm

global _start

section .text

_start:
    jmp MESSAGE      ; 1) lets jump to MESSAGE

GOBACK:
    mov eax, 0x4
    mov ebx, 0x1
    pop ecx          ; 3) we are poping into `ecx`, now we have the
                     ; address of "Hello MSU!\r\n" 
    mov edx, 0xF
    int 0x80
    jmp end          ; Injector will insert a jump to original entry point

    ;mov eax, 0x1    ; Intentionally not returning the control
    ;mov ebx, 0x0
    ;int 0x80
    mov edx, 0

MESSAGE:
    call GOBACK       ; 2) we are going back, since we used `call`, that means
                      ; the return address, which is in this case the address 
                      ; of "Hello MSU!\r\n", is pushed into the stack.
    db "Hello, MSU!!!", 0dh, 0ah

end: