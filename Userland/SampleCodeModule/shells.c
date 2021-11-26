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
static int pipeWrapper(int posCommand1, int posCommand2, int foreground);
static int processPipeCreator(int commandIndex, int fg, int fdIn, int fdOut);

static char lines[TOTAL_LINES][MAX_LINE_LENGTH];
static int currentLine = 0;
static int lineCursor = 0;

static int pipeId = 1;

char commandsNames[][MAX_ARG_LEN] = {"datetime", "help", "inforeg", "printmem", "divzero", "invalidopcode", "clear", "echo", "mem", "ps", "kill", "nice", "block", "unblock", "sem", "pipe", "testmem", "testprocess", "testprio", "testsync", "testnosync", "loop", "cat", "wc", "filter", "phylo"};
int  (* run[])(char args[MAX_ARGS][MAX_ARG_LEN]) = {dateTime, help, infoReg, printmem, divzero, invalidopcode, clear, echo, mem, ps, killCommand, niceCommand, blockCommand, unblockCommand, sem, pipe, testMemCommand, testProcessesCommand, testPrioCommand, testSync, testNoSync, loopCommand,catCommand, wcCommand, filterCommand, phyloCommand};
static int totalCommands = 26;

char notBuiltInCommands[][MAX_ARG_LEN] = {"loop", "cat", "wc", "filter", "testmem", "testprio", "testprocess", "phylo", "testsync", "testnosync"};

void init_shell() {

    uint64_t errCode = getError();

    for (int i = 0; i < TOTAL_LINES; i++) {
        for (int j = 0; j < TOTAL_LINES; j++) {
            lines[i][j] = 0;
        }
    }

    setFunctionKey(5, processKiller);
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
        if (buff[i] == '\n' || lineCursor == (MAX_LINE_LENGTH - 3)) {
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
    
    while (line[index] != 0 && line[index] != '\n' && foundArgs < 10) {
        if (index != 0 && line[index-1] == ' ') {
            foundArgs++;
            nameIndex = 0;
        }
        if (line[index] != ' ' && line[index] != '-') {
            commandArgs[foundArgs][nameIndex++] = line[index];
        } 
        index++;
    }

    foreground = (strcmp(commandArgs[foundArgs], "&") != 0);
    
    int i = isCommand(commandArgs[0]);
    int j = (strcmp(commandArgs[1], "|") == 0);
    int k = isCommand(commandArgs[2]);
    
    if(i == ERROR)  {
        printf(" - INVALID COMMAND");
        return;
    }

    // Check if pipe
    if(j) {
        // Check if second command is valid
        if(k != ERROR) {
            // Check if commands are builtin
            if(!(isBuiltin(commandArgs[0]) && !isBuiltin(commandArgs[2]))) {
                int res = pipeWrapper(i, k, foreground);
                if(res == ERROR)
                    return;
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
            int argQty = 4;    // argc, foreground, fdIn, fdOut
            char arguments[argQty][MAX_ARG_LEN];

            int index = 0;
            char aux[11] = {0};

            // argc
            itoa(0, aux, 10);
            strcpy(arguments[index++], aux);

            // foreground
            itoa(foreground, aux, 10);
            strcpy(arguments[index++], aux);

            // fdIn
            itoa(0, aux, 10);
            strcpy(arguments[index++],aux);

            // fdOut
            itoa(1, aux, 10);
            strcpy(arguments[index++], aux); 
            run[i](arguments);
        }
    }

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
            return 0;
    }
    return 1;
}

static int pipeWrapper(int posCommand1, int posCommand2, int foreground) {
    int pids[2];

    int pipe = pipe_open(pipeId++);

    if(pipe == ERROR) {
        printf(" - ERROR CREATING PIPE");
        return ERROR;
    }

    pids[0] = processPipeCreator(posCommand2, 0, pipe, 1); // runs in background

    if(pids[0] == ERROR) {
        pipe_close(pipe);
        return ERROR;
    }

    pids[1] = processPipeCreator(posCommand1, foreground, 0, pipe);

    if(pids[1] == ERROR) {
        pipe_close(pipe);
        return ERROR;
    }

    return 1;
}

static int processPipeCreator(int commandIndex, int fg, int fdIn, int fdOut) {
    char arguments[4][MAX_ARG_LEN];

    int index = 0;
    char aux[11] = {0};

    // argc
    itoa(0, aux, 10);
    strcpy(arguments[index++], aux);

    // foreground
    itoa(fg, aux, 10);
    strcpy(arguments[index++], aux);

    // fdIn
    itoa(fdIn, aux, 10);
    strcpy(arguments[index++],aux);

    // fdOut
    itoa(fdOut, aux, 10);
    strcpy(arguments[index++],aux);

    return run[commandIndex](arguments);
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
