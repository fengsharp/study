all:
	@#as 01exit.s -o 01exit.o
	@#ld 01exit.o -o 01exit.out
	
	@#as 02maxnum.s -o 02maxnum.o
	@#ld 02maxnum.o -o 02maxnum.out

	@#as --32 03function.s -o 03function.o
	@#ld -m elf_i386 03function.o -o 03function.out

	as --32 04fun2.s -o 04fun2.o
	ld -m elf_i386 04fun2.o -o 04fun2.out
clean:
	rm -fr *.o *.out