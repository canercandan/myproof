
clean :
	rm -f myprof  *.o  *.s *~ lex.yy.c  y.tab.c y.tab.h yacc_myprof.tab.c yacc_asm.tab.h

all : 
	lex lex_myprof.lex
	yacc -d yacc_myprof.y
	gcc -o myprof y.tab.c lex.yy.c -lfl
	
run: all
	cat trace.asm | ./myprof
