EXE := wakeonlan.exe
SRC := wakeonlan.c
OBJ := $(patsubst %.c,%.o,$(SRC))
CCFLAGS := -Wall -pedantic -Werror -ansi -std=c99 -O2
LDFLAGS :=
LDLIBS := -lwsock32 -lws2_32

.PHONY: all clean
all: $(EXE)

$(EXE): $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $(OBJ) $(LDLIBS) -mwindows

%.o: %.c
	$(CC) $(CCFLAGS) -c $< -o $@

.PHONY: clean
clean:
	CMD /C IF EXIST $(EXE) DEL $(EXE)
	CMD /C FOR %F IN ($(OBJ)) DO IF EXIST %F DEL %F


