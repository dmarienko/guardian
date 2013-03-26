# makefile, written by dmitry marienko
CC=avr-gcc
OBJCOPY=avr-objcopy

MCU=atmega8
F_CPU=1000000UL
CFLAGS=-g -mmcu=$(MCU) -I../include -Wall -DF_CPU=$(F_CPU) -Wstrict-prototypes -Os -mcall-prologues
OBJS=test.o display.o usart.o
NAME=Project1


all: $(NAME).hex

$(NAME).hex : $(NAME).out
	$(OBJCOPY) -R .eeprom -O ihex $(NAME).out $(NAME).hex 

$(NAME).out : $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME).out -Wl,-Map,test.map $(OBJS)

%.o:%.c
	$(CC) -c $(CFLAGS) $< -o $@  2>.stderr ||( echo -e -n "\033[1m\033[31m" && cat .stderr && echo -e -n "\033[0m\033[37m" && exit 1)
	@echo -e  "\033[1m\033[37m"
	@cat .stderr
	@echo -e -n "\033[0m\033[37m"
	@rm .stderr

load: $(NAME).hex
	avrwrite $(NAME).hex

clean:
	rm -f *.o *.map *.out *.hex
