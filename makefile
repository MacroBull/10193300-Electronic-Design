####
LD_LIBRARY_PATH = .
TARGET = msp430g2452
CC = msp430-gcc
VPATH = /home/macrobull/lib/msp430/include
DEFINES = SMCLK_FREQ=16000000
CFLAGS = -mmcu=$(TARGET) -I$(VPATH) -g -D$(DEFINES)
OBJ = ed.o sqr_gen.o disp.o capture.o pwm/pwm.o
#OBJ = plus_test.o
#LDSCRIPTS = test.x

main: clean clear out.elf

debug: main
	msp430-objdump -D out.elf > asm.asm

flash: out.elf
	mspdebug rf2500 "prog out.elf"

out.elf: $(OBJ)
	$(CC) -o out.elf $(CFLAGS) $(OBJ) $(LDSCRIPTS)

%.o : %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -o $@ -c $<

clear:
	clear

clean:
	-rm out.elf *.o
	#-rm $(VPATH)/*.o
