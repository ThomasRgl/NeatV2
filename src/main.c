#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "snake/snake.h"
#include "neuralNetwork/neuralNetwork.h"


void jump(int a){
    for( int i = 0; i < a; i++){
        printf("\n");
    }
}


void afficherJeu(int resultat ){
    afficheGrille();
    switch (resultat) {
        case 0:
            printf(">haut\n" );
            break;
        case 1:
            printf(">Bas\n" );
            break;
        case 2:
            printf(">gauche\n" );
            break;
        case 3:
            printf(">droite\n" );
            break;
        default:
            break;
        }
}

void playBest( NeuralNetwork * nn){

    Snake * snake ;

    int resultat = 4;
    int end = 0;

    initialiseGrille();
    snake = malloc(sizeof(Snake));
    initSnake(snake);

    //
    while (end == 0 && snake->health != 0) {
        resultat = computeNN( nn, getInput(snake, params.nbNeuronsInput) );

        //                      Affichage
        jump(10);
        // printNetwork(nn);
        // afficherData(nn);
        printNetwork(nn);
        afficherJeu(resultat);
        printf(">\n");
        getchar();

        switch (resultat) {
            case 0:
                end = move(snake, -1, 0);
                break;
            case 1:
                end = move(snake, 1, 0);
                break;
            case 2:
                end = move(snake, 0, -1);
                break;
            case 3:
                end = move(snake, 0, 1);
                break;
            default:
                //break;
                //printf("fin du jeu\n" );
                printf("%d\n", resultat );
                exit(0);
                end = 1;
                break;
        }

    }

    destroySnake(snake);

    return;
}




int main() {
    srand(time(NULL));

    struct timespec start, finish;
    double elapsed;



    for(size_t i = 1; i < 10; i++){
        NewConfig(
            1000,       // size_t taille_population,
            300,        //size_t nombre de generation

            8,          // size_t nbNeuronsInput,
            8,          // size_t nbNeuronsHidden,
            4,          // size_t nbNeuronsOutput,

            1,          // size_t nbHiddenLayer,

            0.3,        // double mutationRate,
            0.05,       // double sigmaMutation,
            0.3,        // double crossoverRate,

            i           //size_t nbThread
        );




        //lance l'ia et chronometre le temps d'execution
        clock_gettime(CLOCK_MONOTONIC, &start);
        runPere();
        clock_gettime(CLOCK_MONOTONIC, &finish);

        elapsed = (finish.tv_sec - start.tv_sec);
        elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;

        printf("%ld, %lf\n",i,elapsed );
    }


    return 0;
}
