TARGET = RayCasting1
HEADFILE = 
SRCDIR = ../src
BUILD_DIR = ./bin
PROJECT_SOURCE_FILES ?= \
    main.c vecmath.c ray.c

# Define all object files from source files
OBJS = $(patsubst %.c,%.o,$(PROJECT_SOURCE_FILES)) 

CC = gcc
LD = gcc

WINDRES = windres.exe

RM = cmd //C del //Q //F
RRM = cmd //C rmdir //Q //S
    
INC = -I$(MINGW_DEV_LIB)/include
CFLAGS = -O2 -Wall -Wunused-function -std=c99 -m64 -static

LIB = -L$(MINGW_DEV_LIB)/lib
#LDFLAGS = -O2 -s -m64 -lraylib -lopengl32 -lgdi32 -lwinmm -mwindows
LDFLAGS = -O2 -s -m64 -lraylibdll -lopengl32 -lgdi32 -lwinmm 

$(BUILD_DIR)/${TARGET}: ${OBJS}
	${CC} ${OBJS} ${LIB} ${LDFLAGS} -o ${TARGET}

%.o:${SRCDIR}/%.c ${HEADFILE}
	${CC} ${CFLAGS} ${INC} $< -c

clean: 
	$(RM) *.o
	$(RM) *.exe
