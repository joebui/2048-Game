CC=clang
CFLAGS=-Wall
EXEC=2048
SOURCES = 2048.c key_algorithm.c menu.c score.c
OBJS = $(patsubst %.c,%.o,$(SOURCES))
HEADERS = key_algorithm.h menu.h score.h

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJS) -lncurses
	
$(OBJS): $(HEADERS)

.PHONY: clean
clean:
	rm *.o 
	
.PHONY: cleanall
cleanall:
	rm *.o *~ $(EXEC)
