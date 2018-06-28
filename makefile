LEX=flex
YACC=bison
CC=gcc
RM=rm

LEX_FLAGS=
YACC_FLAGS=-vtd
CCLIBS=-ll -ly -lm
CCWARN=-Wall

SCANNER=scanner
PARSER=parser
AST=ast
OBJS=$(PARSER).o $(SCANNER).o $(AST).o ast_print.o irtree.o assemble.o

OUTPUT=$(PARSER).output

TARGET=woc

all: $(TARGET)
$(TARGET): $(OBJS)
	$(CC) -g $(OBJS) -o $(TARGET) $(CCLIBS)

%.o: %.c
	$(CC) -g $(CCWARN) $*.c -c $(CCLIBS)
$(PARSER).c: $(PARSER).y
	$(YACC) $(YACC_FLAGS) -o $(PARSER).c $(PARSER).y
$(SCANNER).c: $(SCANNER).l
	$(LEX) $(LEX_FLAGS) -o $(SCANNER).c $(SCANNER).l
$(PARSER).h: $(PARSER).y
$(SCANNER).h: $(SCANNER).l
$(PARSER).o: $(SCANNER).h
$(SCANNER).o: $(PARSER).h

clean:
	rm -rf $(SCANNER).c  $(SCANNER).h $(PARSER).h $(PARSER).c $(OBJS) $(TARGET) $(OUTPUT)