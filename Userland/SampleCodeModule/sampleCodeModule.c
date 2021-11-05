/* sampleCodeModule.c */

// includes de prueba
#include <syscalls_asm.h>
#include <colors.h>
#include <shells.h>
#include <stdint.h>
#include <stdio.h>

char * v = (char*)0xB8000 + 79 * 2;

static int var1 = 0;
static int var2 = 0;

int main() {

	// uint64_t errCode = getError();

	// init_shell(errCode);

	// char *argv[] = {"shell"};

	// createProcessSyscall(init_shell, 1, argv, 1, NULL);
	
	// return 0;

	*v = 'X';
	*(v+1) = 0x74;

	char *argv[2];
	argv[0] = "shell";
	argv[1] = NULL;
	createProcess(init_shell,1,argv, NULL);
	// nice(0,20);
	// createProcess(chess,argv);
	while (1)
	{
		/* code */
	}
	


	//Test if BSS is properly set up
	if (var1 == 0 && var2 == 0)
		return 0xDEADC0DE;

	return 0xDEADBEEF;
}
