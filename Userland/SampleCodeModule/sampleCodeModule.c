/* sampleCodeModule.c */

// includes de prueba
#include <syscalls_asm.h>
#include <colors.h>
#include <shells.h>
#include <stdint.h>
#include <stdio.h>

char * v = (char*)0xB8000 + 79 * 2;

int main() {

	// uint64_t errCode = getError();

	// init_shell(errCode);

	char *argv[] = {"shell"};

	createProcessSyscall(init_shell, 1, argv, 1, NULL);
	
	return 0;
}
