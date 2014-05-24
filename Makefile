TARGET = msp430g2452

GLB_DFN = SMCLK_FREQ=15712623
OBJ = main.o touch_sick.o disp.o cap_cnt.o generator.o isr_proxy.o click.o

CC = msp430-gcc
VPATH = ${LIBMSP430_MACROBULL}/src
export CPATH = ${LIBMSP430_MACROBULL}/include
CFLAGS = -mmcu=$(TARGET) -D $(GLB_DFN) -O2


build: clean out.elf

debug: build
	msp430-objdump -D out.elf > asm.asm


install: out.elf
# 	echo "opt fet_block_size 1024" > ~/.mspdebug
	echo "opt fet_block_size 32" > ~/.mspdebug
	-mspdebug rf2500 "prog out.elf"


out.elf: $(OBJ)
	$(CC) -o out.elf $(CFLAGS) $(OBJ)


%.o : %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	-rm out.elf
	-find -name "*.o" -print -exec rm {} \;
	echo "/home/macrobull/lib/msp430/include" > .kdev_include_paths   # init for include paths
	echo "/opt/cross/msp430/include" >> .kdev_include_paths   # init for include paths
