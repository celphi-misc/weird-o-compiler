LEX=flex
YACC=bison
CC=gcc
RM=rm

LEX_FLAGS=
YACC_FLAGS=-v -t -d
CCLIBS=-ll -ly -lm

SCANNER=scanner
PARSER=parser
OBJS=$(SCANNER).o $(PARSER).o

TARGET=woc

all: $(TARGET)
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(CCLIBS)

%.o: %.c
	$(CC) -Wall $*.c -c $(CCLIBS)
$(PARSER).h: $(PARSER).y
$(PARSER).c: $(PARSER).y
	$(YACC) $(YACC_FLAGS) -o $(PARSER).c $(PARSER).y
$(SCANNER).c: $(SCANNER).l
	$(LEX) $(LEX_FLAGS) -o $(SCANNER).c $(SCANNER).l

clean:
	rm -rf $(SCANNER).c $(PARSER).h $(PARSER).c $(OBJS) $(TARGET)