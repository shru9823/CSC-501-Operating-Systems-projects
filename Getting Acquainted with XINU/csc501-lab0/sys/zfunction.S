	.text
	.global zfunction

zfunction:	
	pushl %ebp
	movl %esp, %ebp
	
	movl 8(%ebp), %eax
	andl $0xffc01fff, %eax
	shll $4, %eax
	
	movl %ebp, %esp
	popl %ebp
	ret
