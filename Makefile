CC = g++
ENTRYPOINT = exe_main
COPTS = -g -Wall -c -std=gnu++11 -DENTRYPOINT=$(ENTRYPOINT)
OBJS = main.o tui.o parser.o
LD = g++
LDOPTS = 
EXE = tuimu

$(EXE): $(OBJS)
	$(LD) -o $(EXE) $(LDOPTS) $(OBJS)

rebuild: clean $(EXE)

%.o: %.cpp
	$(CC) -o $@ $(COPTS) $<

clean:
	rm -rf *.o $(EXE)

.PHONY: clean rebuild
