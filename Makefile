CCFLAGS=-Isdk/include -Isdk --std-c99 -I sdk/_target_sdcc_nrf24le1_32/include/ --opt-code-size
LDFLAGS= -Lsdk/src/
PROGRAMS = main
SOURCES = ${PROGRAMS:=.c}

all: ${PROGRAMS}

${PROGRAMS}: ${SOURCES}
	sdcc --model-large $(CCFLAGS) $(LDFLAGS) main.c

clean:
	rm -rf  main.asm  main.cdb  main.ihx  main.lk  main.lst  main.map  main.mem  main.omf  main.rel  main.rst  main.sym
