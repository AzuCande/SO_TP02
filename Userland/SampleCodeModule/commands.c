#include <commands.h>

static void format(char *str, int value);
static int buildProcess(char *name, void (*entryPoint) (/*int, */char [][MAX_ARG_LEN]), char args[][MAX_ARG_LEN]);

int dateTime(char args[MAX_ARGS][MAX_ARG_LEN]) {
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

    return 1;
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

int infoReg(char args[MAX_ARGS][MAX_ARG_LEN]) {
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

    return 1;
}

int printmem(char args[MAX_ARGS][MAX_ARG_LEN]) {
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

    return 1;
}

int help(char args[MAX_ARGS][MAX_ARG_LEN]) {
    putChar('\n');
    printf("This is the Help Center\n");

    printf("\tSpecial keys:\n");   
    // printf("\t* F1 - switch between shells\n");
    printf("\t* F12 - saves the values of the registers\n");

    printf("\tCommands:\n");

    /* Old commands */
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

    //printf("\t sh [ARGUMENT] - allows user to execute apps\n");
    printf("\t help - displays list of commands\n");

    printf("\t mem - displays the state of memory\n");

    printf("\t ps - displays list of processes with further data\n");
    printf("\t loop - prints current process ID on loop\n");
    printf("\t kill [PID] - kills process with given ID\n");
    printf("\t nice [PID,priority] - switches process priority with given ID and priority\n");
    printf("\t block [PID] - switches process state with given ID\n");

    printf("\t sem - displays list of semaphores with further data\n");

    printf("\t cat - displays stdin\n");
    printf("\t wc - prints amount of input lines\n");
    printf("\t filter - deletes vowels from the input\n");
    printf("\t pipe - displays list of pipes and further data\n");

    //Tests
    printf("\t testmem - runs test for memory manager\n");
    printf("\t testprocess - runs test for scheduler\n");
    printf("\t testprio - runs test for scheduler\n");


    //printf("\t phylo - starts phylo app\n");    


    return 1;
}

int clear(char args[MAX_ARGS][MAX_ARG_LEN]){
    clearAll();

    return 1;
}

int echo(char args[MAX_ARGS][MAX_ARG_LEN]) {
    putChar('\n');
    for(int i = 1; args[i][0] && i < MAX_ARGS; i++){
        printf("%s ", args[i]);
    }
    putChar('\n');

    return 1;
}

int divzero(char args[MAX_ARGS][MAX_ARG_LEN]) {
    _divzero();
    return 1;
}

int invalidopcode(char args[MAX_ARGS][MAX_ARG_LEN]) {
    _invalidopcode();
    return 1;
}

int mem(char args[MAX_ARGS][MAX_ARG_LEN]) {
    int strSize = 2048;
    char str[strSize];
    memSyscall(str, strSize);
    printf("%s\n", str);

    return 1;
}

int ps(char args[MAX_ARGS][MAX_ARG_LEN]) {
    char buffer[BUFFER_SIZE];
    psSyscall(buffer);
    printf("%s\n", buffer);
    return 1;
}

int killCommand(char args[MAX_ARGS][MAX_ARG_LEN]) {
    unsigned int id = atoi(args[0]);
    kill(id);
    printf("Process successfully killed\n");
    return 1;
}

int niceCommand(char args[MAX_ARGS][MAX_ARG_LEN]) {
    uint64_t id = atoi(args[0]);
    uint64_t priority = atoi(args[1]);
    nice(id, priority);
    printf("Priority successfully changed\n");
    return 1;
}

int blockCommand(char args[MAX_ARGS][MAX_ARG_LEN]) {
    unsigned int id = atoi(args[0]);
    block(id);
    printf("Process state successfully switched\n");
    return 1;
}

int unblockCommand(char args[MAX_ARGS][MAX_ARG_LEN]) {
    unsigned int id = atoi(args[0]);
    unblock(id);
    printf("Process state successfully switched\n");
    return 1;
}

int sem(char args[MAX_ARGS][MAX_ARG_LEN]) {
    char buffer[BUFFER_SIZE];
    semSyscall(buffer);
    printf("%s\n", buffer);
    return 1;
}

int pipe(char args[MAX_ARGS][MAX_ARG_LEN]) {
    char buffer[BUFFER_SIZE];
    pipeSyscall(buffer);
    printf("%s\n", buffer);
    return 1;
}

int testMemCommand(char args[MAX_ARGS][MAX_ARG_LEN]) {
    putChar('\n');
    return buildProcess("testmem", test_mm, args);
}

int testProcessesCommand(char args[MAX_ARGS][MAX_ARG_LEN]) {
    putChar('\n');
    return buildProcess("testprocess", test_processes, args);
}

int testPrioCommand(char args[MAX_ARGS][MAX_ARG_LEN])  {
    putChar('\n');
    return buildProcess("testprio", test_prio, args);
}

void loop(char args[MAX_ARGS][MAX_ARG_LEN]) {
    unsigned int pid = getPid();
    
    while(1) {
        printf("%d\n", pid);

        int ticksEnd = getTicks() + 5;
        while(getTicks() < ticksEnd) {
            ;
        }
    }
}

int loopCommand(char args[MAX_ARGS][MAX_ARG_LEN]) {
    return buildProcess("loop", loop, args);
}

void cat(char args[MAX_ARGS][MAX_ARG_LEN]) {
    putChar('\n');
    for(int i = 1; args[i][0] && i < MAX_ARGS; i++){
        printf("%s ", args[i]);
    }
    putChar('\n');
}

int catCommand(char args[MAX_ARGS][MAX_ARG_LEN]) {
    return buildProcess("cat", cat, args);
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

int wcCommand(char args[MAX_ARGS][MAX_ARG_LEN]) {
    return buildProcess("wc", wc, args);
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

int filterCommand(char args[MAX_ARGS][MAX_ARG_LEN]) {

    return buildProcess("filter", filter, args);
}

void phylo(char args[MAX_ARGS][MAX_ARG_LEN]) {
    //TODO develope
}

int phyloCommand(char args[MAX_ARGS][MAX_ARG_LEN]) {
    return buildProcess("phylo", phylo, args);
}

static int buildProcess(char *name, void (*entryPoint) (/*int, */char [][MAX_ARG_LEN]), char args[][MAX_ARG_LEN]) {
    int j = 0;
    unsigned int argc = atoi(args[j++]);
    
    int foreground = atoi(args[j++]);

    int fds[2];
    fds[0] = atoi(args[j++]);  
    fds[1] = atoi(args[j++]);
    
    // First arg for name
    char argv[argc+1][MAX_ARG_LEN];
    int i = 0;
    strcpy(argv[i], name);
    i++;

    for(; i < argc+1; i++) {
        strcpy(argv[i], args[i + 3]);
    }
    
    return createProcess(entryPoint, foreground, (char**)argv, fds);
}