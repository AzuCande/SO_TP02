#ifndef SHELLS
#define SHELLS

#include <shells.h>

#define TOTAL_LINES 63
#define MAX_LINE_LENGTH 129

void updateShell(char * buff, int dim);
void writeToLines(char * buff, int dim);
void changeActiveShell();
void addLine();
void keyPressedShell(char ch);
static void clearShellLine(int line);
static void drawBottomLine();
static int isCommand(char * name);
static int isBuiltin(char *command);
static int isPipe(char *pipe);
static int isAmpersand(char *arg);
static int pipeWrapper(int posCommand1, int posCommand2, char args1[MAX_ARGS][MAX_ARG_LEN], int argc1, char args2[MAX_ARGS][MAX_ARG_LEN], int argc2, int foreground);

static char lines[TOTAL_LINES][MAX_LINE_LENGTH];
static int currentLine = 0;
static int lineCursor = 0;

static int pipeId = 1;

char commandsNames[][MAX_ARG_LEN] = {"datetime", "help", "inforeg", "printmem", "divzero", "invalidopcode", "clear", "echo", "mem", "ps", "kill", "nice", "block", "unblock", "sem", "pipe", "testmem", "testprocess", "testprio", "loop", "cat", "wc", "filter", "phylo"};
int  (* run[])(char args[MAX_ARGS][MAX_ARG_LEN]) = {dateTime, help, infoReg, printmem, divzero, invalidopcode, clear, echo, mem, ps, killCommand, niceCommand, blockCommand, unblockCommand, sem, pipe, testMemCommand, testProcessesCommand, testPrioCommand, loopCommand,catCommand, wcCommand, filterCommand, phyloCommand};
static int totalCommands = 24;

char notBuiltInCommands[][MAX_ARG_LEN] = {"loop", "cat", "wc", "filter"};

void init_shell() {

    uint64_t errCode = getError();

    for (int i = 0; i < TOTAL_LINES; i++) {
        for (int j = 0; j < TOTAL_LINES; j++) {
            lines[i][j] = 0;
        }
    }
    setConsoleUpdateFunction(updateShell);

    if (errCode < 32) {
        uint64_t registers[19];
        getRegistersSyscall(registers);
        printf("ERROR CODE %x ", errCode);
        switch (errCode) {
            case 0: printf("(division by zero)\n");
                break;
            case 6: printf("(invalid operation code)\n");
                break;
        }
        printf("REGISTERS STATUS:\n");
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
    } else {
        printf("Welcome to the Operative Systems 2021 - Q2\n");
        printf("Created by De Luca, Kim and Borracci\n");
        printf("To enter the Help Center, type \"help\" and press ENTER.\n");
        printf("Which command would you like to run?\n");
    }

    drawShellLines();

    while(1) {
        char ch = getChar();
        keyPressedShell(ch);
    }
}

void writeToLines(char * buff, int dim) {
    for (int i = 0; i < dim && buff[i] != 0; i++) {
        if (buff[i] == '\n' || lineCursor == (MAX_LINE_LENGTH - 3)) { //El -3 es para que el ultimo elemento sea un 0 y no toma en cuanta los "> "
            if (lineCursor > 0) {
                addLine();
            }
        } else if (buff[i] == '\b' && lineCursor > 0) {
            lines[currentLine % (TOTAL_LINES-1)][lineCursor-1] = lines[currentLine % (TOTAL_LINES-1)][lineCursor];
            lineCursor-=lineCursor==0?0:1;
        } else {
            lines[currentLine % (TOTAL_LINES-1)][lineCursor] = buff[i];
            lineCursor++;
        }
    }
    drawBottomLine();
    return;
}


void updateShell(char * buff, int dim) {
    writeToLines(buff, dim);
}

void addLine() {
    currentLine++;
    clearShellLine(currentLine);
    lineCursor = 0;
    drawShellLines();
}

static void clearShellLine(int line) {
    for (int i = 0; i < MAX_LINE_LENGTH; i++) {
        lines[line%(TOTAL_LINES-1)][i] = 0;
    }
}


void drawShellLines() {
  clearScreen(BLACK);
  int y = SCREEN_HEIGHT;
  int x = 0;
  for (int i = 0; i >= -TOTAL_LINES && i >= -currentLine; i--) {
      y-=BASE_CHAR_HEIGHT;
      if (i == 0) {
          drawString(0, y, "> ", 3, LIGHT_GRAY, BLACK, 1, 0);
          x += BASE_CHAR_WIDTH*2;
      } else {
          x = 0;
      }
      if (lines[(i+currentLine)%(TOTAL_LINES-1)][0] == 0) continue;
      drawString(x, y, lines[(i+currentLine)%(TOTAL_LINES-1)], MAX_LINE_LENGTH-1, LIGHT_GRAY, BLACK, 1, 0);
  }
}

static void drawBottomLine() {
    int x = 0;
    int bkgColor = BLACK;
    drawRect(x, SCREEN_HEIGHT-BASE_CHAR_HEIGHT, SCREEN_WIDTH, BASE_CHAR_HEIGHT, bkgColor);
    int fontColor = LIGHT_GRAY;
    int arrowColor = 0xF2E124;
    drawString(x, SCREEN_HEIGHT-BASE_CHAR_HEIGHT, "> ", 3, arrowColor, bkgColor, 1, 0);
    drawString(BASE_CHAR_WIDTH*2, SCREEN_HEIGHT-BASE_CHAR_HEIGHT, lines[(currentLine)%(TOTAL_LINES-1)], MAX_LINE_LENGTH-1, fontColor, bkgColor, 1, 0);
}


static void exeCommand(char * line) {
    
    char commandArgs[MAX_ARGS][MAX_ARG_LEN] = {{0}}; //Max of 10 arguments with 32 chars each
    int foundArgs = 0; // echo a v s
    int index = 0;
    int nameIndex = 0;
    int foreground = 1;

    int pipePos = 0;
    
    while (line[index] != 0 && line[index] != '\n' && foundArgs < 10) {
        if (line[index] != ' ' && line[index] != '-') {
            commandArgs[foundArgs][nameIndex++] = line[index];
        } else if (index!= 0 && line[index-1] == ' ' && isAlfaNum(line[index])) {
            foundArgs++;
            nameIndex = 0;
        } else if(line[index] == '|') {
            pipePos = index;
        }

        index++;
    }

    if(isAmpersand(commandArgs[foundArgs])) {
        foreground = 0;
    }
    
    int i = isCommand(commandArgs[0]);
    int j = isPipe(commandArgs[pipePos]);
    int k = isCommand(commandArgs[pipePos + 1]);
    
    if(i == ERROR)  {
        printf(" - INVALID COMMAND");
        return;
    }

    char readyArgv1[7][MAX_ARG_LEN] = {{0}};  // | and the commands not taken into account
    int readyArgc1;

    for(readyArgc1 = 0; readyArgc1 < pipePos; readyArgc1++) {
        strcpy(readyArgv1[readyArgc1], commandArgs[readyArgc1+1]);
    }

    char readyArgv2[7][MAX_ARG_LEN] = {{0}};  // | and the commands not taken into account
    int readyArgc2;

    for(readyArgc2 = pipePos+2; readyArgc2 < foundArgs; readyArgc2++) {
        strcpy(readyArgv2[readyArgc2], commandArgs[readyArgc2+1]);
    }

    // Check if pipe
    if(j) {
        // Check if second command is valid
        if(k != ERROR) {
            // Check if commands are builtin
            if(!(isBuiltin(commandArgs[0]) || isBuiltin(commandArgs[pipePos+1]))) {
                pipeWrapper(i, k, readyArgv1, readyArgc1, readyArgv2, readyArgc2, foreground);
            } else {
                printf(" - INVALID COMMANDS FOR PIPE");
                return;
            }
        } else {
            printf(" - INVALID PIPE");
            return;
        }
    } else {
        if(isBuiltin(commandArgs[0])) {
            run[i] (commandArgs);
        } else {
            int argQty = foundArgs + 4;    // argc, args, foreground, fdIn, fdOut
            char arguments[argQty][MAX_ARG_LEN];
            
            for(int i=0; i<argQty; i++) {
                char aux[11] = {0};

                if(i==0) {
                    //argc
                    intToString(foundArgs, aux);
                    strcpy(arguments[i], aux);
                } else if(i==argQty-3){
                    //fg
                    intToString(foreground, aux);
                    strcpy(arguments[i], aux);
                } else if(i == argQty - 2) {
                    //fdin
                    intToString(0, aux);
                    strcpy(arguments[i],aux);
                }  else if (i == argQty -1) {
                    //fdout
                    intToString(0, aux);
                    strcpy(arguments[i], aux); 
                } else{
                    //argvs
                    strcpy(arguments[i], commandArgs[i+1]);
                }
            }
            run[i](arguments);
        }
    }

    // if (i >= 0) {
    //     run[i](commandArgs);
    // } else {
    //     printf(" - INVALID COMMAND");
    // }

}

// Returns which command is it
// Returns -1 if it does not exist
static int isCommand(char * name){
    for (int i = 0; i < totalCommands; i++) {
        if (!strcmp(commandsNames[i],name)){
            return i;
        }
    }
    return -1;
}

static int isBuiltin(char *command) {
    for(int i = 0; i < NO_BUILTIN_SIZE; i++) {
        if(strcmp(notBuiltInCommands[i], command) == 0)
            return 1;
    }
    return 0;
}

static int isPipe(char *pipe) {
    return pipe[0] == '|';
}

static int isAmpersand(char *arg) {
    return arg[0] == '&';
}

static int pipeWrapper(int posCommand1, int posCommand2, char args1[MAX_ARGS][MAX_ARG_LEN], int argc1, char args2[MAX_ARGS][MAX_ARG_LEN], int argc2, int foreground) {
    int pids[2];
    unsigned int fds[2];

    int pipe = pipeOpenSyscall(pipeId++);

    if(pipe == ERROR) {
        printf(" - ERROR CREATING PIPE");
        return ERROR;
    }

    fds[0] = pipe;
    fds[1] = 1;

    // cat asdj ajodas | cat asca a
    int argQty = argc1 + 4;    // argc, args1, foreground, fdIn, fdOut
    char arguments1[argQty][MAX_ARG_LEN];
    
    int index = 0;
    char aux[11] = {0};

    // argc
    intToString(argc1, aux);
    strcpy(arguments1[index++], aux);

    // foreground
    intToString(foreground, aux);
    strcpy(arguments1[index++], aux);

    // fdIn
    intToString(fds[0], aux);
    strcpy(arguments1[index++],aux);

    // fdOut
    intToString(fds[1], aux);
    strcpy(arguments1[index++],aux);

    for(int i=0; index< argQty; index++) {
        // argv
        strcpy(arguments1[i], args1[index]);
    }

    pids[0] = run[posCommand1](arguments1);

    if(pids[0] == ERROR) {
        pipeCloseSyscall(pipe);
        return ERROR;
    }

    fds[0] = 0;
    fds[1] = pipe;

    char arguments2[argQty][MAX_ARG_LEN];
    argQty = argc2 + 4;    // argc, foreground, fdIn, fdOut, argv

    index = 0;

    // argc
    intToString(argc2, aux);
    strcpy(arguments2[index++], aux);

    // foreground
    intToString(foreground, aux);
    strcpy(arguments2[index++], aux);

    // fdIn
    intToString(fds[0], aux);
    strcpy(arguments2[index++],aux);

    // fdOut
    intToString(fds[1], aux);
    strcpy(arguments2[index++],aux);

    for(int i=0; index< argQty; index++) {
        // argv
        strcpy(arguments2[i], args2[index]);
    }

    pids[1] = (int) run[posCommand2](arguments2);

    if(pids[1] == ERROR) {
        pipeCloseSyscall(pipe);
        return ERROR;
    }

    // TODO: terminar esto
    return 1;
}

void keyPressedShell(char ch) {
    if (ch) {
        if (ch == '\n' && lineCursor > 0) {
            exeCommand(lines[(currentLine)%(TOTAL_LINES-1)]);
        }
        putChar(ch);
    }
}

void clearAll() {
    currentLine = 0;
    clearShellLine(0);
}

#endif
