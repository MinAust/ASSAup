CC=gcc
CFLAGS=-c -Wall -O3
LDFLAGS=-s
SOURCES=assaup.c conclr.c sep.c gotoxy.c timefmt.c fexist.c
RES=ASSAup.Res
RES64=ASSAup64.Res
RC=assaup.rc
RC64=assaup64.rc
RCFLAGS=-O coff
RCFLAGS64=-F pe-x86-64 -O coff
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=ASSAup.Exe

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS)
	windres $(RC) $(RCFLAGS) -o $(RES)
	$(CC) $(LDFLAGS) $(OBJECTS) $(RES) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJECTS)
	rm -f $(EXECUTABLE)
	rm -f $(RES)
	rm -f $(RES64)

