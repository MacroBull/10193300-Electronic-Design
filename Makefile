TARGET = msp430g2452

GLB_DFN = RELEASE
OBJ = main.o

CC = msp430-gcc
VPATH = ${LIBMSP430_MACROBULL}/src
export CPATH = ${LIBMSP430_MACROBULL}/include
CFLAGS = -mmcu=$(TARGET) -D $(GLB_DFN) -g


build: clean out.elf

debug: build
	msp430-objdump -D out.elf > asm.asm


flash: out.elf
	echo "opt fet_block_size 1024" > ~/.mspdebug
# 	echo "opt fet_block_size 256" > ~/.mspdebug
	-mspdebug rf2500 "prog out.elf"


out.elf: $(OBJ)
	$(CC) -o out.elf $(CFLAGS) $(OBJ)


%.o : %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	-rm out.elf
	-find -name "*.o" -print -exec rm {} \;
