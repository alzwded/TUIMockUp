CC = g++
ENTRYPOINT = exe_main
COPTS = -g -Wall -c -std=gnu++11 -DENTRYPOINT=$(ENTRYPOINT)
OBJS = main.o tui.o
LD = g++
LDOPTS = 
EXE = tuimu
HEADERS = tui.h

$(EXE): $(OBJS)
	$(LD) -o $(EXE) $(LDOPTS) $(OBJS)

rebuild: clean $(EXE)

%.o: %.cpp
	$(CC) -o $@ $(COPTS) $<

clean:
	rm -rf *.o $(EXE)

.PHONY: clean rebuild
