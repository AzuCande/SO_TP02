#ifndef SHELLS
#define SHELLS
#include <stdGraphics.h>
#include <colors.h>
#include <stdint.h>
#include <stdio.h>
#include <shells.h>
#include <commands.h>
#include <stdlib.h>
#include <syscalls_asm.h>

#define TOTAL_LINES 63
#define MAX_LINE_LENGTH 65
void updateShell(char * buff, int dim);
void writeToLines(char * buff, int dim);
void changeActiveShell();
void addLine();
void keyPressedShell(char ch);
static void clearShellLine(int line);
static void drawShell0Lines();
static void drawShell1Lines();
static void drawBottomLine();
// static void clearScreenLine(uint8_t line);
static void drawBottomLine0();
static void drawBottomLine1();
static int isCommand(char * name);

static char lines[2][TOTAL_LINES][MAX_LINE_LENGTH];
static int currentLine[] = {0, 0};
static int lineCursor[] = {0, 0};
static int activeShell = 1;

char commandsNames[][MAX_ARG_LEN]={"datetime", "help", "inforeg", "printmem", "divzero", "invalidopcode", "clear", "echo"};
void  (* run[])(char args[MAX_ARGS][MAX_ARG_LEN]) = {dateTime, help, infoReg, printmem, divzero, invalidopcode, clear, echo};
static int totalCommands = 8;

void init_shell(uint64_t errCode) {
    for (int i = 0; i < TOTAL_LINES; i++) {
        for (int j = 0; j < TOTAL_LINES; j++) {
            lines[0][i][j] = 0;
            lines[1][i][j] = 0;
        }
    }

    setFunctionKey(1,changeActiveShell);
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
        printf("Welcome to the Computer Architecture Project 2021 - Q1\n");
        printf("Created by De Luca, Kim and Lopez Guzman\n");
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
    for (int i = 0; i < dim && buff[i] != 0 && i < MAX_LINE_LENGTH; i++) {
        if (buff[i] == '\n' || lineCursor[activeShell] == (MAX_LINE_LENGTH - 3)) { //El -3 es para que el ultimo elemento sea un 0 y no toma en cuanta los "> "
            if (lineCursor[activeShell] > 0) {
                addLine();
            }
        } else if (buff[i] == '\b' && lineCursor[activeShell] > 0) {
            lines[activeShell][currentLine[activeShell] % (TOTAL_LINES-1)][lineCursor[activeShell]-1] = lines[activeShell][currentLine[activeShell] % (TOTAL_LINES-1)][lineCursor[activeShell]];
            lineCursor[activeShell]-=lineCursor[activeShell]==0?0:1;
        } else {
            lines[activeShell][currentLine[activeShell] % (TOTAL_LINES-1)][lineCursor[activeShell]] = buff[i];
            lineCursor[activeShell]++;
        }
    }
    drawBottomLine();
    return;
}

void changeActiveShell() {
    activeShell = ((activeShell)? 0 : 1);
    drawBottomLine1();
    drawBottomLine0();
}

void updateShell(char * buff, int dim) {
    writeToLines(buff, dim);
}

void addLine() {
    currentLine[activeShell]++;
    clearShellLine(currentLine[activeShell]);
    lineCursor[activeShell] = 0;
    drawShellLines();
}

static void clearShellLine(int line) {
    for (int i = 0; i < MAX_LINE_LENGTH; i++) {
        lines[activeShell][line%(TOTAL_LINES-1)][i] = 0;
    }
}


void drawShellLines() {
    drawShell0Lines();
    drawShell1Lines();
}

static void drawShell0Lines() {
    drawRect(0, 0, SCREEN_WIDTH/2, SCREEN_HEIGHT, BUTTERFLY_BUSH);
    int y = SCREEN_HEIGHT;
    int x = 0;
    for (int i = 0; i >= -TOTAL_LINES && i >= -currentLine[0]; i--) {
        y-=BASE_CHAR_HEIGHT;
        if (i == 0 && activeShell == 0) {
            drawString(0, y, "> ", 3, 0xF2E124, BUTTERFLY_BUSH, 1, 0);
            x += BASE_CHAR_WIDTH*2;
        } else {
            x = 0;
        }
        if (lines[0][(i+currentLine[0])%(TOTAL_LINES-1)][0] == 0) continue;
        drawString(x, y, lines[0][(i+currentLine[0])%(TOTAL_LINES-1)], MAX_LINE_LENGTH-1, WHITE, BUTTERFLY_BUSH, 1, 0);
    }
}

static void drawShell1Lines() {
    drawRect(SCREEN_WIDTH/2+1, 0, SCREEN_WIDTH/2, SCREEN_HEIGHT, 0x101010);
    int y = SCREEN_HEIGHT;
    int x = SCREEN_WIDTH/2+1;
    for (int i = 0; i >= -TOTAL_LINES && i >= -currentLine[1]; i--) {
        y-=BASE_CHAR_HEIGHT;
        if (i == 0 && activeShell == 1) {
            drawString(x, y, "> ", 3, 0xFF0000, 0x101010, 1, 0);
            x += BASE_CHAR_WIDTH*2;
        } else {
            x = SCREEN_WIDTH/2+1;
        }
        if (lines[1][(i+currentLine[1])%(TOTAL_LINES-1)][0] == 0) continue;
        drawString(x, y, lines[1][(i+currentLine[1])%(TOTAL_LINES-1)], MAX_LINE_LENGTH-1, LIGHT_GRAY, 0x101010, 1, 0);
    }
}

// static void clearScreenLine(uint8_t line){
//   int x = 0;
//   int color = BUTTERFLY_BUSH;
//   if (activeShell == 1) {
//     x = SCREEN_WIDTH/2 + 1;
//     color = 0x101010;
//   }
//   drawRect(x,SCREEN_HEIGHT-BASE_CHAR_HEIGHT*(line+1),SCREEN_WIDTH/2,BASE_CHAR_HEIGHT, color);
// }

static void drawBottomLine() {
  if (activeShell == 0)
    drawBottomLine0();
  else drawBottomLine1();
}

static void drawBottomLine0() {
  int x = 0;
  int bkgColor = BUTTERFLY_BUSH;
  drawRect(x, SCREEN_HEIGHT-BASE_CHAR_HEIGHT, SCREEN_WIDTH/2, BASE_CHAR_HEIGHT, bkgColor);
  int fontColor = WHITE;
  int arrowColor = 0xF2E124;
  if (activeShell == 0) {
    drawString(x, SCREEN_HEIGHT-BASE_CHAR_HEIGHT, "> ", 3, arrowColor, bkgColor, 1, 0);
    x += BASE_CHAR_WIDTH*2;
  }
  drawString(x, SCREEN_HEIGHT-BASE_CHAR_HEIGHT, lines[0][(currentLine[0])%(TOTAL_LINES-1)], MAX_LINE_LENGTH-1, fontColor, bkgColor, 1, 0);
}

static void drawBottomLine1() {
  int x = SCREEN_WIDTH/2 + 1;
  int bkgColor = 0x101010;
  drawRect(x, SCREEN_HEIGHT-BASE_CHAR_HEIGHT, SCREEN_WIDTH/2, BASE_CHAR_HEIGHT, bkgColor);
  int fontColor = LIGHT_GRAY;
  int arrowColor = 0xFF0000;
  if (activeShell == 1) {
    drawString(x, SCREEN_HEIGHT-BASE_CHAR_HEIGHT, "> ", 3, arrowColor, bkgColor, 1, 0);
    x += BASE_CHAR_WIDTH*2;
  }
  drawString(x, SCREEN_HEIGHT-BASE_CHAR_HEIGHT, lines[1][(currentLine[1])%(TOTAL_LINES-1)], MAX_LINE_LENGTH-1, fontColor, bkgColor, 1, 0);
}

//ejecutaria los commands
static void exeCommand(char * line) {
  char commandArgs[8][32] = {{0}}; //Maximo 8 argumentos de 32 caracteres c/u
  int foundArgs = 0;
  int index = 0;
  int nameIndex = 0;
  while (line[index] != 0 && line[index] != '\n' && foundArgs < 10) {
    if (line[index] != ' ' && line[index] != '-') {
      commandArgs[foundArgs][nameIndex++] = line[index];
    }
    else if (line[index] == ' ') {
      foundArgs++;
      nameIndex = 0;
    }
    index++;
  }

    int i = isCommand(commandArgs[0]);
    if (i >= 0) {
      run[i](commandArgs);
    } else {
      printf(" - INVALID COMMAND");
    }

}

//devuelve que comando es si no esta  devuelve -1
static int isCommand(char * name){
  for (int i = 0; i < totalCommands; i++) {
    if (!strcmp(commandsNames[i],name)){
      return i;
    }
  }
  return -1;
}

void keyPressedShell(char ch) {
  if (ch) {
    if (ch == '\n' && lineCursor[activeShell] > 0) {
      exeCommand(lines[activeShell][(currentLine[activeShell])%(TOTAL_LINES-1)]);
    }
    putChar(ch);
  }
}

void clearAll() {
    currentLine[activeShell] = 0;
    clearShellLine(0);
}

#endif
