.section .data
.section .text
.global _start
_start:
    movl $1, %eax # system.exit
    movl $10, %ebx

    int $0x80
