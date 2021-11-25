#ifndef PHYLO_H
#define PHYLO_H

#include <syscalls_asm.h>
#include <semaphoresUser.h>
#include <pipesUser.h>
#include <stdio.h>
#include <stdlib.h>
#include <processesUser.h>

#define MAX_PHYLOS 10
#define MIN_PHYLOS 2
#define SEM_PHYLO 600
#define BG 0

typedef enum {WAIT, EAT}phyloState;
typedef enum {DEAD, ALIVE}phyloAlive;

typedef struct PhyloData {
    int id;
    phyloState state;
    phyloAlive isAlive;
} PhyloData;

void startPhylo();
void phylo(int);
void addPhylo();
void removePhylo();
void printPhylo();
void endPhylo(int);

#endif