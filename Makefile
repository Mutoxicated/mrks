CC=gcc
CFLAGS=

SRC= ./utils/toml/tomlc17.c ./src/mem_dbg/mem_dbg.c ./utils/macro.c ./src/helpers/strbuf.c 
OBJ=$(SRC:%.c=%.o)
DEP=$(OBJ:%.o=%.d)
EXE=./utils/utils.exe

TARGET=/usr/local

DLLSRC= ./src/lexer/lexer.c ./src/lexer/tokens.c ./src/helpers/helpers.c ./src/helpers/strbuf.c ./src/mem_dbg/mem_dbg.c 
DLLOBJ= $(DLLSRC:%.c=%.o)
THEDLL= /usr/lib/liblexer.so

macro: debug
	./utils/utils.exe verbose

dll: $(DLLOBJ) 
dll: DLL
.NOTPARALLEL: dll

DLL: $(THEDLL)

$(THEDLL): $(DLLOBJ)
	$(CC) -shared -o $(THEDLL) $^ -I ./include

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