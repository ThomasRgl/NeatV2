#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>



#include "neuralNetwork/neuralNetwork.h"
#include "snake/snake.h"




int main() {
    srand(time(NULL));

    struct timespec start, finish;
    double elapsed;




    NewConfig(
        1000,       // size_t taille_population,
        3000,        //size_t nombre de generation

        8,          // size_t nbNeuronsInput,
        8,          // size_t nbNeuronsHidden,
        4,          // size_t nbNeuronsOutput,

        1,          // size_t nbHiddenLayer,

        0.3,        // double mutationRate,
        0.05,       // double sigmaMutation,
        0.3,        // double crossoverRate,

        7           //size_t nbThread
    );




    //lance l'ia et chronometre le temps d'execution
    clock_gettime(CLOCK_MONOTONIC, &start);
    runPere();
    clock_gettime(CLOCK_MONOTONIC, &finish);

    elapsed = (finish.tv_sec - start.tv_sec);
    elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;

    // printf("%ld, %lf\n",i,elapsed );



    return 0;
}
