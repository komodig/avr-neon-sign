MCU=atmega328p
CC=avr-gcc
OBJCOPY=avr-objcopy
fname=test

ALLHEADERS = $(foreach dir,$(SRC_DIR),$(wildcard $(dir)/*.h))
BUILD_DIR = $(shell pwd)/obj
SRC_DIR = $(shell pwd)/src
DOXYGEN_DIR = $(shell grep OUTPUT_DIRECTORY Doxyfile | sed -e 's/^.*=\ *//g')

OBJS = ${BUILD_DIR}/${fname}.o \
       ${BUILD_DIR}/gpio.o \
       ${BUILD_DIR}/timer.o \
       ${BUILD_DIR}/led_circle.o
#       ${BUILD_DIR}/usart.o
#
# Optimization level, can be [0, 1, 2, 3, s].
# 0 = turn off optimization. s = optimize for size.
# (Note: 3 is not always the best optimization level. See avr-libc FAQ.)
# CFLAGS=-g -mmcu=$(MCU) -Wall -Wstrict-prototypes -Os -mcall-prologues
# to enable floating-point conversion with printf use:
CFLAGS=-g -mmcu=$(MCU) -Wstrict-prototypes -Os -mcall-prologues -Wl,-u,vfprintf -lprintf_flt -lm
# use without floating-point conversion:
# CFLAGS=-g -mmcu=$(MCU) -Wall -Wstrict-prototypes -Os -mcall-prologues

#
# targets
#
all: ${fname}.hex

doxygen:
	+@[ -d ${DOXYGEN_DIR} ] || mkdir -p ${DOXYGEN_DIR}
	+@[ -f ${DOXYFILE} ] && doxygen ${DOXYFILE}

help: 
	@echo "Usage: make [all|clean]"

clean:
	rm -f ${BUILD_DIR}/*.o *.map *.out *.hex

#
# dependencies
# 
${fname}.hex : ${fname}.out 
	$(OBJCOPY) -R .eeprom -O ihex ${fname}.out ${fname}.hex 
${fname}.out : ${OBJS} 
	$(CC) $(CFLAGS) -o ${fname}.out -Wl,-Map,${fname}.map ${OBJS}  
${BUILD_DIR}/%.o: ${SRC_DIR}/%.c ${ALLHEADERS} 
	+@[ -d ${BUILD_DIR} ] || mkdir -p ${BUILD_DIR}
	$(CC) -c -o"$@" ${CFLAGS} $<

