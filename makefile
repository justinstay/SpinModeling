CFLAGS = -c -Wall -Werror -Wextra -O3
LFLAGS = -Wall -lm
LIBNAME = SpinModeling
EXENAME = main
PROGRAMNAME = main

all : $(EXENAME)

debug : CFLAGS += -g
debug : $(EXENAME)

simple : CFLAGS = -c
simple : LFLAGS = -lm
simple : $(EXENAME)

$(EXENAME) : $(PROGRAMNAME).o $(LIBNAME).o
	gcc $(PROGRAMNAME).o $(LIBNAME).o -o $(EXENAME) $(LFLAGS)
main.o : $(PROGRAMNAME).c $(LIBNAME).h
	gcc $(PROGRAMNAME).c $(CFLAGS)
$(LIBNAME).o : $(LIBNAME).c $(LIBNAME).h
	gcc $(LIBNAME).c $(CFLAGS)
clean:
	clear
	rm -f *.o *~ output.txt $(EXENAME)
run:
	./$(EXENAME) | tee output.txt
analyze:
	valgrind --leak-check=yes ./$(EXENAME)
