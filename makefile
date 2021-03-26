GCC=gcc

NN = src/neuralNetwork/neuralNetwork.c
SNAKE = src/snake/snake.c
MAIN = src/main.c



OFLAGS_DEBUG=-g -Wall -Werror -Wextra
OFLAGS_DEBUG_GPROF=-pg -Wall -Werror -Wextra #gprof neat gmon.out
OFLAGS=-O3 -Wall  -Wextra
OLD = -O3 -finline-functions -funroll-loops -march=native -mtune=native -flto
OLD_OFLAGS = -I/usr/include

LFLAGS=-lm

neat: $(NN) $(MAIN) $(SNAKE)
	$(GCC) $(CFLAGS) $(OFLAGS_DEBUG_GPROF) -o neat $(NN) $(MAIN) $(SNAKE) $(LFLAGS)


clean:
		rm -Rf *~ *.o
