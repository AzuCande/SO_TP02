include Makefile.inc

KERNEL=kernel.bin
SOURCES=$(wildcard *.c)
SOURCES_ASM=$(wildcard asm/*.asm)
SOURCES_DRIVERS=$(wildcard drivers/*.c)
SOURCES_LIBS=$(wildcard libs/*.c)
SOURCES_MEMORY=$(wildcard memory/*.c)
SOURCES_SYNC=$(wildcard sync/*.c)

OBJECTS=$(SOURCES:.c=.o)
OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)
OBJECTS_DRIVERS=$(SOURCES_DRIVERS:.c=.o)
OBJECTS_LIBS=$(SOURCES_LIBS:.c=.o)
OBJECTS_MEMORY=$(SOURCES_MEMORY:.c=.o)
OBJECTS_SYNC=$(SOURCES_SYNC:.c=.o)

LOADERSRC=loader.asm
MM=-D NO_BUDDY
LOADEROBJECT=$(LOADERSRC:.asm=.o)
STATICLIBS=

ALL_OBJECTS= $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_ASM) $(OBJECTS_DRIVERS) $(OBJECTS_LIBS) $(OBJECTS_MEMORY) $(OBJECTS_SYNC)

all: $(KERNEL)

$(KERNEL): $(LOADEROBJECT) $(OBJECTS) $(STATICLIBS) $(OBJECTS_ASM) $(OBJECTS_DRIVERS) $(OBJECTS_LIBS) $(OBJECTS_MEMORY) $(OBJECTS_SYNC)
	$(LD) $(LDFLAGS) -T kernel.ld -o $(KERNEL) $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_ASM) $(OBJECTS_DRIVERS) $(OBJECTS_LIBS) $(OBJECTS_MEMORY) $(OBJECTS_SYNC) $(STATICLIBS)
	$(LD) $(LDFLAGS) -T kernel.ld --oformat=elf64-x86-64 -o kernel.elf $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_ASM) $(OBJECTS_DRIVERS) $(OBJECTS_LIBS) $(OBJECTS_MEMORY) $(OBJECTS_SYNC) $(STATICLIBS)

%.o: %.c
	$(GCC) $(GCCFLAGS) -I./include $(MM) -c $< -o $@

%.o : %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

$(LOADEROBJECT):
	$(ASM) $(ASMFLAGS) $(LOADERSRC) -o $(LOADEROBJECT)

clean:
	rm -rf asm/*.o *.o *.bin drivers/*.o libs/*.o memory/*.o

.PHONY: all clean
