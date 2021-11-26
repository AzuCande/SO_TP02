#include <phylo.h>

static void createPhylo(int phyloIdx);
static void addPhylos();
static void addPhylo();
static void removePhylo();
static int phyloWantsToEat(int phyloIdx);
static void phyloLeaveFork(int phyloIdx);
static void endPhylos();
static void phyloStatus();
static void phylo(int argc, char *argv[]);
static void printWelcomeMessage();
static void philosopherManager(int phyloSem);


int totalPhilosophers = BASE_PHYLOS;
PhyloData philosophers[MAX_PHYLOS];

// Number of i's right neighbour
#define RIGHT(i) ((i) + 1) % (totalPhilosophers)
// Number of i's left neighbour
#define LEFT(i) ((i) + totalPhilosophers - 1) % (totalPhilosophers)

static void createPhylo(int phyloIdx) {
    char *argv[3];

    argv[0] = "philosopher";

    char aux[11];
    itoa(phyloIdx, aux, 10);
    argv[1] = aux;

    argv[2] = NULL;

    philosophers[phyloIdx].id = createProcess(phylo, BG, argv, NULL);
    philosophers[phyloIdx].state = WAIT;
    philosophers[phyloIdx].isAlive = ALIVE;
}

static void addPhylos() {
    for(int i = 0; i < totalPhilosophers; i++) {
        createPhylo(i);
    }
}

static void addPhylo() {
    createPhylo(totalPhilosophers++);
}

static void removePhylo() {
    totalPhilosophers--;
    philosophers[totalPhilosophers].state = DEAD;
}

static int phyloWantsToEat(int phyloIdx) {
    if(philosophers[RIGHT(phyloIdx)].state == EAT || philosophers[LEFT(phyloIdx)].state == EAT)
        return 0;
    return 1;
}

static void phyloLeaveFork(int phyloIdx) {
    philosophers[phyloIdx].state = WAIT;
}

static void endPhylos() {
    for(int i = 0; i < totalPhilosophers; i++) {
        philosophers[i].isAlive = DEAD;
    }
}

static void phyloStatus() {
    for (int i = 0; i < totalPhilosophers; i++) {
        if (philosophers[i].state == EAT) {
            printf("E");
        } else {
            printf(".");
        }
    }
    putChar('\n');
}

static void phylo(int argc, char *argv[]) {
    int phyloSem = sem_open(SEM_PHYLO, 1);
    int currentPhylo = atoi(argv[1]);

    while(philosophers[currentPhylo].isAlive != DEAD || philosophers[currentPhylo].state == EAT) {
        sem_wait(phyloSem);
        
        // If eating, make the philosopher wait
        if(philosophers[currentPhylo].state) {
            phyloLeaveFork(currentPhylo);
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

static void printWelcomeMessage() {
    printf("Welcome to the Philosophers Problem!\n");
    printf("You start with 5 philosophers and have a maximum of 10 philosophers and a minimum of 2\n");
    printf("You can add them with \'a\', delete them with \'d\' and exit with \'q\'.\n");
    printf("The state of each will be displayed as E (Eating) or . (Hungry)\n\n");
}

static void philosopherManager(int phyloSem) {
    char c;

    while(1) {
        c = getChar();
        
        if(c != '\0') {
            switch(c) {
                case 'a':
                    if(totalPhilosophers < MAX_PHYLOS) {
                        printf("A new philosopher joined! There are %d philosophers now.\n", totalPhilosophers + 1);
                        addPhylo();
                    } else {
                        printf("Can\'t add another philosopher. Maximum 10 philosophers.\n");
                    }
                    break;
                case 'r':
                    if(totalPhilosophers > MIN_PHYLOS) {
                        printf("One philosopher has left! There are %d philosophers now.\n", totalPhilosophers - 1);
                        removePhylo();
                    } else {
                        printf("Can\'t remove another philosopher. Minimum 2 philosophers.\n");
                    }
                    break;
                case 'q':
                    printf("Program finished!\n");
                    endPhylos();
                    return;
                case EOF:
                    printf("Program finished!\n");
                    endPhylos();
                    return;
                default:
                    break;
            }
        }

    }
}

void philosopherProblem(/*int argc, char **argv*/) {
    totalPhilosophers = BASE_PHYLOS;
    int phyloSem = sem_open(SEM_PHYLO, 1);

    printWelcomeMessage();

    sem_wait(phyloSem);
    addPhylos();
    sem_post(phyloSem);

    philosopherManager(phyloSem);
    sem_close(phyloSem);

    putChar(EOF);
    for (int i = 0; i < MAX_PHYLOS; i++) {
        kill(philosophers[i].id);
    }
    processKiller();
}