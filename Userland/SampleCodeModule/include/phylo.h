#ifndef PHYLO_H
#define PHYLO_H

#include <syscalls_asm.h>
#include <semaphoresUser.h>
#include <pipesUser.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_PHYLOS 10
#define MIN_PHYLOS 2
#define SEM_PHYLO "phyloApp"

enum phyloState {WAIT, EAT};
enum phyloAlive {DEAD, ALIVE};

typedef struct PhyloData {
    int id;
    enum phyloState state;
    enum phyloAlive isAlive;
} PhyloData;

void startPhylo();
void phylo(int);
void addPhylo();
void removePhylo();
void printPhylo();
void endPhylo(int);

#endif