
clean :
	rm -f myprof  *.o  *.s *~ lex.yy.c  y.tab.c y.tab.h yacc_myprof.tab.c yacc_asm.tab.h


BuildProfile.o : BuildProfile.c BuildProfile.h
		 gcc -c BuildProfile.c

all : 
	lex lex_myprof.lex
	yacc -d yacc_myprof.y
	gcc -c y.tab.c
	gcc -c BuildProfile.c
	gcc -o myprof y.tab.c lex.yy.c BuildProfile.o -lfl
	
run: all
	cat trace.asm | ./myprof
