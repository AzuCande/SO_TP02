#include <commands.h>

static void format(char *str, int value);

void dateTime(char args[MAX_ARGS][MAX_ARG_LEN]) {
    putChar('\n');

    char days[] = "00";
    char month[] = "00";
    char year[] = "00";
    char hours[] = "00";
    char minutes[] = "00";
    char seconds[] = "00";

    format(days, getDays());
    format(month, getMonth());
    format(year, getYear());
    format(hours, getHours());
    format(minutes, getMinutes());
    format(seconds, getSeconds());

    printf("%s/%s/20%s %s:%s:%s", days, month, year, hours, minutes, seconds);
}

static void format(char *str, int value) {
    if (value < 10) {
        str[1] = value + '0';
    } else {
        str[0] = (value / 10) + '0';
        str[1] = (value % 10) + '0';
    }
    str[2] = 0;
}

void infoReg(char args[MAX_ARGS][MAX_ARG_LEN]) {
    uint64_t registers[19];
    getRegisters(registers);
    putChar('\n');

    printf("R15: %X - R14: %X\n", registers[18], registers[17]);
    printf("R13: %X - R12: %X\n", registers[16], registers[15]);
    printf("R11: %X - R10: %X\n", registers[14], registers[13]);
    printf("R9: %X - R8: %X\n", registers[12], registers[11]);
    printf("RSI: %X - RDI: %X\n", registers[10], registers[9]);
    printf("RBP: %X - RDX: %X\n", registers[8], registers[7]);
    printf("RCX: %X - RBX: %X\n", registers[6], registers[5]);
    printf("RAX: %X - RIP: %X\n", registers[4], registers[3]);
    printf("CS: %X - FLAGS: %X\n", registers[2], registers[1]);
    printf("RSP: %X\n", registers[0]);
}

void printmem(char args[MAX_ARGS][MAX_ARG_LEN]) {
  putChar('\n');
  
  int with0x = 0;
  if (args[1][0] == '0' && args[1][1] == 'x') {
    with0x = 2;
  }

  uint64_t aux = atohex(&args[1][with0x]);
  if (aux > 0) {
    uint64_t bytes[32];
    getMemSyscall(aux, bytes, 32);
    for (int i = 0; i < 4; i++) {
      printf("0x%x: ", aux + i*4);
      for (int j = 0; j < 8; j++) {
        printf("%x ", bytes[i*8 + j]);
      }
      printf("\n");
    }
  }

}

void help(char args[MAX_ARGS][MAX_ARG_LEN]) {
    putChar('\n');
    printf("This is the Help Center\n");

    printf("\tSpecial keys:\n");   
    printf("\t* F1 - switch between shells\n");
    printf("\t* F12 - saves the values of the registers\n");

    printf("\tCommands:\n");

    //printf("\t sh [ARGUMENT] - allows user to execute apps\n");
    printf("\t help - displays list of commands\n");

    printf("\t mem - displays the state of memory\n");

    printf("\t ps - displays list of processes with further data\n");
    printf("\t loop - prints current process ID on loop\n");
    printf("\t kill [ARGUMENT] - kills process with given ID\n");
    printf("\t nice [ARGUMENT] - switches process priority with given ID and priority\n");
    printf("\t block [ARGUMENT] - switches process state with given ID\n");

    printf("\t sem - displays list of semaphores with further data\n");

    //printf("\t cat - displays stdin\n");
    //printf("\t wc - prints amount of input lines\n");
    //printf("\t filter - deletes vowels from the input\n");
    printf("\t pipe - displays list of pipes and further data\n");

    //printf("\t phylo - starts phylo app\n");    

    printf("\t* datetime - displays the current date and time of the OS\n");
    printf("\t* inforeg - displays the values of each register\n");
    printf("\t(F12 must have been pressed before this command is used for\n");
    printf("\tit to work correctly)\n");
    printf("\t* printmem [ARGUMENT] - displays 32 bytes of memory,\n");
    printf("\tstarting from the address given in the argument\n");
    printf("\t* clear - clears the current shell\n");
    printf("\t* echo [ARGUMENT] - prints the given argument\n");
    printf("\t* divzero - forces a division by zero and shows the\n");
    printf("\tgenerated exception\n");
    printf("\t* invalidopcode - forces an invalid OP code and shows\n");
    printf("\tthe generated exception\n");
}

void clear(char args[MAX_ARGS][MAX_ARG_LEN]){
    clearAll();
}

void echo(char args[MAX_ARGS][MAX_ARG_LEN]) {
    putChar('\n');
    for(int i = 1; args[i][0] && i < MAX_ARGS; i++){
        printf("%s ", args[i]);
    }
    putChar('\n');
}

void divzero(char args[MAX_ARGS][MAX_ARG_LEN]) {
    _divzero();
}

void invalidopcode(char args[MAX_ARGS][MAX_ARG_LEN]) {
    _invalidopcode();
}

void mem(char args[MAX_ARGS][MAX_ARG_LEN]) {

}

void ps(char args[MAX_ARGS][MAX_ARG_LEN]) {
    char buffer[BUFFER_SIZE];
    psSyscall(buffer);
    printf("%s\n", buffer);
}

void kill(char args[MAX_ARGS][MAX_ARG_LEN]) {
    unsigned int id = atoi(args[0]);
    killSyscall(id);
    printf("Process successfully killed\n");
}

void nice(char args[MAX_ARGS][MAX_ARG_LEN]) {
    unsigned int id = atoi(args[0]);
    unsigned int priority = atoi(args[1]);
    niceSyscall(id, priority);
    printf("Priority successfully changed\n");
}

void block(char args[MAX_ARGS][MAX_ARG_LEN]) {
    unsigned int id = atoi(args[0]);
    blockSyscall(id);
    printf("Process state successfully switched\n");
}

void sem(char args[MAX_ARGS][MAX_ARG_LEN]) {
    char buffer[BUFFER_SIZE];
    semSyscall(buffer);
    printf("%s\n", buffer);
}

void pipe(char args[MAX_ARGS][MAX_ARG_LEN]) {
    char buffer[BUFFER_SIZE];
    pipeSyscall(buffer);
    printf("%s\n", buffer);
}

void testCommand(char args[MAX_ARGS][MAX_ARG_LEN]) {
    putChar('\n');
    test_mm();
    return;
}

void loop(char args[MAX_ARGS][MAX_ARG_LEN]) {
    unsigned int pid = getPidSyscall();
    
    while(1) {
        printf("%d\n", pid);

        int ticksEnd = getTicks() + 5;
        while(getTicks() < ticksEnd) {
            ;
        }
    }
}

void cat(char args[MAX_ARGS][MAX_ARG_LEN]) {
    putChar('\n');
    for(int i = 1; args[i][0] && i < MAX_ARGS; i++){
        printf("%s ", args[i]);
    }
    putChar('\n');
}

void wc(char args[MAX_ARGS][MAX_ARG_LEN]) {
    unsigned int lines = 0;
    char c;
    while((c = getChar()) != '\0'){
        if(c == '\n') {
            lines++;
        }
        
    }
    printf("\n Amount of lines: %d\n", lines);
}

void filter(char args[MAX_ARGS][MAX_ARG_LEN]) {
    char c;
    putChar('\n');

    while((c = getChar()) != '\0'){
        if(!isVowel(c)) {
            putChar(c);
        }
    }
    putChar('\n');
}