.global _exit
.global main
main:
	addi  a0, x0, 1      # 1 = StdOut
	li a0,0
_exit:
