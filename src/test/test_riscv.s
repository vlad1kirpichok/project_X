	.file	"test.c"
	.option nopic
	.text
	.align	1
	.globl	func
	.type	func, @function
func:
	ble	a2,zero,.L6
	addi	sp,sp,-48
	sd	ra,40(sp)
	sd	s0,32(sp)
	sd	s1,24(sp)
	sd	s2,16(sp)
	sd	s3,8(sp)
	mv	s2,a0
	mv	s3,a1
	mv	s1,a2
	li	s0,0
.L3:
	mv	a0,s3
	jalr	s2
	addiw	s0,s0,1
	bne	s1,s0,.L3
	ld	ra,40(sp)
	ld	s0,32(sp)
	ld	s1,24(sp)
	ld	s2,16(sp)
	ld	s3,8(sp)
	addi	sp,sp,48
	jr	ra
.L6:
	ret
	.size	func, .-func
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align	3
.LC0:
	.string	"Injected message!"
	.align	3
.LC1:
	.string	"Hello, World!"
	.text
	.align	1
	.globl	main
	.type	main, @function
main:
	addi	sp,sp,-16
	sd	ra,8(sp)
	li	a2,3
	lui	a1,%hi(.LC0)
	addi	a1,a1,%lo(.LC0)
	lui	a0,%hi(puts)
	addi	a0,a0,%lo(puts)
	call	func
	lui	a0,%hi(.LC1)
	addi	a0,a0,%lo(.LC1)
	call	puts
	li	a0,0
	ld	ra,8(sp)
	addi	sp,sp,16
	jr	ra
	.size	main, .-main
	.ident	"GCC: (GNU) 10.2.0"
	.section	.note.GNU-stack,"",@progbits
