#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "snake/snake.h"
#include "neuralNetwork/neuralNetwork.h"

void initGlobalVar(){
    if( NB_HIDDEN_LAYER == 0 ){
        TOTAL_WEIGHT = ( NB_INPUT + 1 ) * NB_NEURONS_OUTPUT;
    }
    else{
        TOTAL_WEIGHT = ( NB_INPUT + 1 ) * NB_NEURONS_HIDDEN;
        for ( int i = 1; i < NB_NEURONS_HIDDEN; i ++)
            TOTAL_WEIGHT =  ( NB_NEURONS_HIDDEN + 1 ) * NB_NEURONS_HIDDEN;
        TOTAL_WEIGHT += ( NB_NEURONS_HIDDEN + 1 ) * NB_NEURONS_OUTPUT;

    }

    TAILLE_CROSSOVER_MAX = TOTAL_WEIGHT * CROSSOVER_RATE ;
    printf("%d\n",TAILLE_CROSSOVER_MAX );
    printf("%d\n",TOTAL_WEIGHT );
    printf("%lf\n",CROSSOVER_RATE );
}

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
        resultat = compute( nn, getInput(snake, NB_INPUT) );

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

void game(NeuralNetwork * nn){

    Snake * snake ;

    int resultat = 4;
    int end = 0;

    initialiseGrille();
    snake = malloc(sizeof(Snake));
    initSnake(snake);

    //
    while (end == 0 && snake->health != 0) {
        resultat = compute( nn, getInput(snake, NB_INPUT) );

        //                      Affichage
        // jump(10);
        // printNetwork(population[i]);
        // afficherData(population[i]);
        // afficherJeu(resultat);
        // printf(">\n");
        // getchar();

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

    setScore(nn, getScore(snake), getFruit(snake));
    destroySnake(snake);

    return;
}

void run(Population *population, size_t gen ){


    for (size_t i = 0; i < TAILLE_POPULATION; i++) {
        game(population->firstPopulation[i]);
    }
    calculateFitness(population);
    // printPopulaton(population);
    // getchar();
    if( gen%500 == 0){
        playBest(bestElement(population));
    }
    evolve(population);

    return;
}


int main() {
    srand(time(NULL));

    initGlobalVar();

    fileScore = openLog("log/fruit.csv");

    Population * population = newPopulation();
    //printNetwork(population->firstPopulation[0]);

    for(size_t i = 0; i < 2001; i++){
        run(population, i);
        printf("gen :  %ld\n", i );
        writeLogScore(fileScore, population);

    }

    freePopulation( population );
    closeLog(fileScore);


    return 0;
}
