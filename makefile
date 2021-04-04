GCC=gcc

NN = src/neuralNetwork/neuralNetwork.c
SNAKE = src/snake/snake.c
MAIN = src/main.c






OFLAGS_DEBUG= -g -O3 -finline-functions -funroll-loops -march=native -mtune=native -flto -lpthread
OFLAGS_DEBUG_GPROF=-pg -O3 -finline-functions -funroll-loops -march=native -mtune=native -flto -lpthread

FLAG_OPTI = -O3 -finline-functions -funroll-loops -march=native -mtune=native -flto -lpthread
FLAG_OPTI2 = -O3 -Wall  -Wextra -finline-functions -funroll-loops -march=native -mtune=native -flto -lpthread

OLD_OFLAGS = -I/usr/include

LFLAGS=-lm -lpthread

neat: $(NN) $(MAIN)
	$(GCC) $(CFLAGS) $(OFLAGS_DEBUG_GPROF)  -o neat $(NN) $(MAIN) $(SNAKE) $(LFLAGS)


clean:
		rm -Rf *~ *.o
