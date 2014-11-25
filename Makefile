CCFLAGS=-Iinclude/ -Isrc/--std-c99 -I _target_sdcc_nrf24le1_32/include/ --opt-code-size
LDFLAGS= -Lsrc/
PROGRAMS = main
SOURCES = ${PROGRAMS:=.c}

all: ${PROGRAMS}

${PROGRAMS}: ${SOURCES}
	sdcc --model-large $(CCFLAGS) $(LDFLAGS) main.c

clean:
	rm -rf  main.asm  main.cdb  main.ihx  main.lk  main.lst  main.map  main.mem  main.omf  main.rel  main.rst  main.sym
