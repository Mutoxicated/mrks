CC=gcc
CFLAGS=

SRC= ./utils/toml/tomlc17.c ./utils/macro.c ./src/helpers/helpers.c 
OBJ=$(SRC:%.c=%.o)
DEP=$(OBJ:%.o=%.d)

EXE=./utils/utils.exe

TARGET=/usr/local

all: debug
	./utils/utils.exe

debug: CFLAGS += -g
debug: $(EXE)

remake: clean debug
.NOTPARALLEL: remake

release: CFLAGS += -O3 -DNDEBUG
release: clean $(EXE)
.NOTPARALLEL: release

clean:
	rm -f $(OBJ) $(DEP)

install: all
	cp $(EXE) $(TARGET)/bin

$(EXE): $(OBJ)
	$(CC) -o $@ $^ -Ltomlc17 -Lhelpers 

-include $(DEP)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $< -I ./include