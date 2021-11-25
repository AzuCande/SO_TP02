#include <phylo.h>

void startPhylo();
void phylo(int);
static void addPhylos();
static void addPhylo()
void removePhilosopher();
void phyloStatus();
void endPhylos(int);

static void createPhylo();

int totalPhilosophers = 0;
PhyloData philosophers[MAX_PHYLOS];

// Number of i's right neighbour
#define RIGHT(i) ((i) + 1) % (totalPhilosophers)
// Number of i's left neighbour
#define LEFT(i) ((i) + totalPhilosophers - 1) % (totalPhilosophers)

void createPhylo(int phyloIdx) {
    char *argv[3];

    strcpy(argv[0], "philosopher");

    char aux[11];
    itoa(phyloIdx, aux, 10);
    strcpy(argv[1], aux);

    argv[2] = NULL;

    philosophers[phyloIdx].pid = createProcess(phylo, BG, argv);
    philosophers[phyloIdx].state = WAIT;
    philosophers[phyloIdx].isAlive = ALIVE;
}

static void addPhylos() {
    for(int i = 0; i < totalPhilosophers; i++) {
        createPhilosopher(i);
    }
}

static addPhylo() {
    createPhilosopher(totalPhilosophers++);
}

static removePhylo() {
    totalPhilosophers--;
    philosophers[totalPhilosophers].state = DEAD;
}

static int phyloWantsToEat(int phyloIdx) {
    if(philosophers[LEFT(phyloIdx)].state == EAT || philosophers[RIGHT(phyloIdx)].state == EAT)
        return 0;
    return 1;
}

static void leaveFork(int phyloIdx) {
    philosophers[phyloIdx].state = WAIT;
}

static void endPhylos() {
    for(int i = 0; i < totalPhilosophers; i++) {
        philosophers[i].isAlive = DEAD;
    }
}

static void phyloStatus() {
    for (int i = 0; i < totalPhilosofers; i++) {
        if (philosofers[i].state == EAT) {
            printf("E");
        } else {
            printf(".");
        }
    }
    putChar('\n');
}

void phylo(int argc, char *argv[]) {
    int phyloSem = sem_open(SEM_PHYLO, 1);
    int currentPhylo = atoi(argv[1]);

    while(philosophers[currentPhylo].isAlive == ALIVE || philosophers[currentPhylo].state == EAT) {
        sem_wait(phyloSem);
        
        // If eating, make the philosopher wait
        if(philosophers[currentPhylo].state == EAT) {
            leaveFork(currentPhylo);
        } else {
            if(phyloWantsToEat(currentPhylo)) {
                philosophers[currentPhylo].state = EAT;
                phyloStatus();
            }
        }

        sem_post(phyloSem);
        yield();
    }
    sem_close(phyloSem);
    processKiller();
} 