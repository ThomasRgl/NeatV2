GCC=gcc

NN = src/neuralNetwork/neuralNetwork.c
SNAKE = src/snake/snake.c
MAIN = src/main.c



OFLAGS2=-g -Wall -Werror -Wextra
OFLAGS=-O3 -Wall  -Wextra
OLD = -O3 -finline-functions -funroll-loops -march=native -mtune=native -flto
OLD_OFLAGS = -I/usr/include

LFLAGS=-lm

neat: $(NN) $(MAIN) $(SNAKE) 
	$(GCC) $(CFLAGS) $(OFLAGS) -o neat $(NN) $(MAIN) $(SNAKE) $(LFLAGS)


clean:
		rm -Rf *~ *.o
