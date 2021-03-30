#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "../snake/snake.h"
#include "neuralNetwork.h"




/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////                              Other                                /////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

double sigmoid(double x){
    return 1 / (1 + exp(-(x)) ); //default sigmoid
}

double rand_gen(){
    return ( (double)(rand()) + 1. )/( (double)(RAND_MAX) + 1. );
}

double normalRandom(){
    double v1=rand_gen();
    double v2=rand_gen();
    return cos(2*3.14*v2)*sqrt(-2.*log(v1));
}


Config NewConfig(
    size_t taille_population,

    size_t nbNeuronsInput,
    size_t nbNeuronsHidden,
    size_t nbNeuronsOutput,

    size_t nbHiddenLayer,

    double mutationRate,
    double sigmaMutation,
    double crossoverRate,

    size_t nbThread
){

    params.taille_population = taille_population;

    params.nbNeuronsInput = nbNeuronsInput;
    params.nbNeuronsHidden = nbNeuronsHidden;
    params.nbNeuronsOutput = nbNeuronsOutput;

    params.nbHiddenLayer = nbHiddenLayer;

    params.mutationRate = mutationRate;
    params.sigmaMutation = sigmaMutation;
    params.crossoverRate = crossoverRate;

    if( params.nbHiddenLayer == 0 ){
        params.totalWeight = ( params.nbNeuronsInput + 1 ) * params.nbNeuronsOutput;
    }
    else{
        params.totalWeight = ( params.nbNeuronsInput + 1 ) * params.nbNeuronsHidden;
        for ( size_t i = 1; i < params.nbHiddenLayer; i ++){
            params.totalWeight +=  ( params.nbNeuronsHidden + 1 ) * params.nbNeuronsHidden;
        }
        params.totalWeight += ( params.nbNeuronsHidden + 1 ) * params.nbNeuronsOutput;
    }

    params.tailleCrossoverMax  = params.totalWeight * params.crossoverRate ;

    params.nbThread = nbThread;
    params.tailleThread = params.taille_population / nbThread;
    return params;

}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////                              Layer                                /////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

Layer * newLayer(size_t size, Layer * previousLayer){
    Layer * new = malloc(sizeof(Layer));

    new->size = size;

    // init neurons
    new->neurons = malloc(new->size * sizeof(double));
    memset(new->neurons, 0, size);

    new->previousLayer = NULL;
    new->nextLayer = NULL;

    if(previousLayer != NULL){

        // init weights
        new->weights = malloc(size * previousLayer->size * sizeof(double));
        memset(new->weights, 0, size * previousLayer->size);

        // init weights
        new->bias = malloc(size * sizeof(double));
        memset(new->bias, 0, size );

        //
        initWeigth(new,  previousLayer->size );

        //
        new->previousLayer = previousLayer;

    }

    return new;
}

void initWeigth(Layer * layer, size_t previousSize){
    for(size_t i = 0; i < layer->size; i++){
        layer->bias[i] = 1 - ((double)rand() / (double)RAND_MAX)*2 ;
    }

    for(size_t i = 0; i < layer->size * previousSize; i++){
        layer->weights[i] = 1 - ((double)rand() / (double)RAND_MAX)*2 ;
    }

}
//
void setInput(Layer * layer, double * inputList){
    for(size_t i = 0; i < params.nbNeuronsInput; i++){
        layer->neurons[i] = sigmoid(inputList[i]);
    }
    free(inputList);
}

//
void computeLayer(Layer * layer){
    double s = 0;

    Layer * previousLayer = layer->previousLayer;
    size_t previousSize = previousLayer->size;
    size_t size = layer->size;

    for (size_t i = 0; i < size ; i++){
        s = layer->bias[i];

        for(size_t j = 0; j < previousSize; j++){
            s += layer->weights[i * previousSize + j] * previousLayer->neurons[j]; // stocker ou appeller à chaque fois ?
        }

        layer->neurons[i] = sigmoid(s);
    }
}

//
void mutateLayer(Layer * layer){

    size_t previousSize = layer->previousLayer->size;
    size_t size = layer->size;

    for (size_t i = 0; i < size ; i++) {

        if( params.mutationRate > (double) rand() / RAND_MAX ){ (layer->bias)[i] += normalRandom() * params.sigmaMutation; }

        for (size_t j = 0; j < previousSize; j++ ){
            if( params.mutationRate > (double) rand() / RAND_MAX ){ (layer->weights)[i * previousSize + j] += normalRandom() * params.sigmaMutation; }
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////                          NeuralNetwork                            /////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
NeuralNetwork * newNeuralNetwork(){

    //
    NeuralNetwork * nn;
    nn = malloc( sizeof(NeuralNetwork) );

    //
    //m_size = 2 + params.nhiddenlayer;
    nn->size = 2 + params.nbHiddenLayer ;

    //
    Layer * currentLayer = newLayer( params.nbNeuronsInput, NULL);
    Layer * previousLayer;
    nn->firstLayer = currentLayer;


    for(size_t i = 0; i < params.nbHiddenLayer; i++){
            previousLayer = currentLayer;
            currentLayer = newLayer(params.nbNeuronsHidden, previousLayer);
            previousLayer->nextLayer = currentLayer;
            // lastLayer->nextLayer = currentLayer;
            // lastLayer = currentLayer;
    }

    previousLayer = currentLayer;
    currentLayer = newLayer(params.nbNeuronsOutput, previousLayer);
    previousLayer->nextLayer = currentLayer;
    //lastLayer->nextLayer = currentLayer;

    //
    nn->lastLayer = currentLayer;

    nn->score = -1;
    nn->fitness = -1;

    return nn;

}
//
size_t compute(NeuralNetwork * nn, double * inputList ){

    //
    Layer * layer = nn->firstLayer;
    setInput(layer, inputList);

    layer = layer->nextLayer;

    //
    while(layer){
        computeLayer(layer);
        layer = layer->nextLayer;
    }
    return output( nn );
}

size_t output(NeuralNetwork * nn ){
    Layer * layer = nn->lastLayer;

    double max = layer->neurons[0];
    size_t index = 0;

    for (size_t i = 1; i < layer->size; i++) {

        if (layer->neurons[i] > max)
        {
            max = layer->neurons[i];
            index = i;
        }
    }
    // printf("size output %ld\n", layer->size );
    // printf("max output %ld\n", index );
    return index;

}
//
void Oldcrossover(NeuralNetwork * nn, NeuralNetwork * father, NeuralNetwork * mother ){

    size_t tot = 0;

    // //
    // Layer * layer = nn->firstLayer->nextLayer;
    // while(layer){
    //     tot += layer->size * (layer->previousLayer->size + 1);
    //     layer = layer->nextLayer;
    // }
    Layer * layer;

    size_t cut = (tot * params.tailleCrossoverMax);
    size_t remains = tot - cut;

    size_t nweights;
    size_t nbias;

    //
    layer = nn->firstLayer->nextLayer;
    Layer * fatherLayer = father->firstLayer->nextLayer;


    while (cut != 0)
    {

        nweights = layer->size * layer->previousLayer->size;
        nbias = layer->size;

        for (size_t i = 0; i < nweights && cut != 0; i++) {
            layer->weights[i] = fatherLayer->weights[i];
            cut--;
        }

        for (size_t i = 0; i < nbias && cut != 0; i++) {
            layer->bias[i] = fatherLayer->bias[i];
            cut--;
        }
        fatherLayer = fatherLayer->nextLayer;
        layer = layer->nextLayer;
    }

    //
    layer = nn->lastLayer;
    Layer * motherLayer = mother->lastLayer;
    while (remains != 0)
    {
        nweights = layer->size * layer->previousLayer->size ;
        nbias = layer->size;
        for (size_t i = nbias - 1; i != 0 && remains != 0; i--) {
            layer->bias[i] = motherLayer->bias[i];
            remains--;
        }

        for (size_t i = nweights - 1; i != 0 && remains != 0; i--) {
            layer->weights[i] = motherLayer->weights[i];
            remains--;
        }

        motherLayer = motherLayer->previousLayer;
        layer = layer->previousLayer;
    }

}

void crossover(NeuralNetwork * nn, NeuralNetwork * father, NeuralNetwork * mother ){
    int remainingToLocation = ((double) rand()/ (double) RAND_MAX )*(params.totalWeight - params.tailleCrossoverMax);
    int crossoverRemaining = params.tailleCrossoverMax;

    Layer * layerA = father->firstLayer->nextLayer;
    Layer * layerB = mother->firstLayer->nextLayer;
    Layer * layer = nn->firstLayer->nextLayer;

    while(layer){
        for (unsigned long long i = 0; i < layer->size ; i++) {
            if( crossoverRemaining == 0 || remainingToLocation != 0){
                layer->bias[i] = layerA->bias[i];
                remainingToLocation -= 1;
            }
            else {
                layer->bias[i] = layerB->bias[i];
                crossoverRemaining -= 1;
            }

            for (unsigned long long j = 0; j < layer->previousLayer->size; j++ ){

                if( crossoverRemaining == 0 || remainingToLocation != 0){
                    layer->weights[i * layer->size + j] = (layerA->weights)[i * layer->size + j];
                    remainingToLocation -= 1;
                }
                else {
                    layer->weights[i * layer->size + j] = (layerB->weights)[i * layer->size + j];
                    crossoverRemaining -= 1;
                }
            }
        }
        layer = layer->nextLayer;
        layerA = layerA->nextLayer;
        layerB = layerB->nextLayer;

    }

}

void mutate(NeuralNetwork * nn ){
    Layer * layer = nn->firstLayer->nextLayer;

    //
    while(layer){
        mutateLayer(layer );
        layer = layer->nextLayer;
    }
}

void setScore(NeuralNetwork * nn, double score,  double fruit){
    nn->score = score*score;
    // nn->score = exp(score);
    nn->bruteScore = score;
    nn->nbFruit = fruit;
}



/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////                           population                              /////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

//
Population * newPopulation( ){
    Population * population;
    population = malloc( sizeof(Population) );

    population->size = params.taille_population;

    population->firstPopulation = malloc(population->size * sizeof(NeuralNetwork)) ;
    population->secondPopulation = malloc(population->size * sizeof(NeuralNetwork)) ;

    for(size_t i = 0; i < population->size; i++){
        population->firstPopulation[i] = newNeuralNetwork();
        population->secondPopulation[i] = newNeuralNetwork();
    }

    return population;
}



//
// void calculateFitness(Population *population ){
//     double sum = 0;
//
//     NeuralNetwork ** ListNeuralNetwork = population->firstPopulation;
//
//     for( size_t i = 0; i < population->size; i++)
//         sum += ListNeuralNetwork[i]->score;
//
//     for( size_t i = 0; i < population->size; i++)
//         ListNeuralNetwork[i]->fitness = ListNeuralNetwork[i]->score  / sum;
// }
//
// void evolve(Population *population ){
//
//     NeuralNetwork * tmp;
//     for( size_t i = 0; i < population->size; i++){
//         tmp = population->secondPopulation[i];
//
//         crossover(tmp, pickOne(population), pickOne(population));
//         mutate(tmp);
//     }
//     NeuralNetwork ** swap = population->firstPopulation;
//     population->firstPopulation = population->secondPopulation;
//     population->secondPopulation = swap;
//     return;
// }

void swap(Population * population){
        NeuralNetwork ** swap = population->firstPopulation;
        population->firstPopulation = population->secondPopulation;
        population->secondPopulation = swap;
}


NeuralNetwork * pickOne(Population *population ){
    size_t index = 0;

    double r = (double) rand() / (double) RAND_MAX;
    NeuralNetwork ** ListNeuralNetwork = population->firstPopulation;

    while ( r > 0) {
        r -= ListNeuralNetwork[index]->fitness;
        index += 1;
    }

    if (index == 0)
        index = 1;

    return ListNeuralNetwork[index - 1];
}

NeuralNetwork * bestElement(Population *population ){
    double BestScore = 0;
    size_t  index = 0;
    for( size_t i = 0; i < population->size; i++){
        if(population->firstPopulation[i]->bruteScore > BestScore){
            index = i;
            BestScore = population->firstPopulation[i]->bruteScore;
        }

    }

    return population->firstPopulation[index];
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////                             Thread                                /////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

Thread * NewThread(Population *population, size_t numThread, pthread_t * id){

    Thread * thread = malloc(sizeof(Thread));

    thread->id = id;
    thread->mutex = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
    thread->population = population;

    size_t tailleThread = params.tailleThread;

    if(numThread == params.nbThread - 1 ){
        tailleThread += params.taille_population % params.nbThread;
    }

    size_t debut = numThread * params.tailleThread;

    thread->size = tailleThread;
    thread->debut = numThread * params.tailleThread;
    thread->fin = thread->debut + thread->size - 1;

    // printf("NT - début : %ld\n", thread->debut );
    // printf("NT - fin : %ld\n\n", thread->fin );

    thread->ListNeuralNetwork_A = malloc( thread->size * sizeof(Thread) );
    thread->ListNeuralNetwork_B = malloc( thread->size * sizeof(Thread) );

    for( size_t i = 0; i < thread->size; i++ ){
        thread->ListNeuralNetwork_A[i] = population->firstPopulation[ debut + i ];
        thread->ListNeuralNetwork_B[i] = population->secondPopulation[ debut + i ];
        // printf("%ld \n", debut + i);
    }




    return thread;
}

//
void *runFils(void *voidThread ){

    Thread * thread = (Thread *)voidThread;
    for(size_t gen = 0; gen < 500; gen++){

        // pthread_mutex_lock(&thread->mutex);
        // printf("FILS - I START GAMING\n" );
        // for(int k = 0; k < 100000000; k++){}
        // printf("FILS - I FINISH GAMING\n" );
        // thread->lock = 0;
        // pthread_mutex_unlock(&thread->mutex);
        while(thread->lock != 1){}
        printf("FILS - I START GAMING\n" );
        for (size_t i = 0; i < thread->size; i++) {
            game(thread->ListNeuralNetwork_A[i]);
        }
        printf("FILS - I FINISH GAMING\n" );
        thread->lock = 0;

        while( ! thread->lock){}
        printf("FILS - I START FITNESSING\n" );
        calculateFitness(thread);
        printf("FILS - I FINISH FITNESSING\n" );
        thread->lock = 0;

        while( ! thread->lock){}
        printf("FILS - I START EVOLVING\n" );
        evolve(thread);
        printf("FILS - I FINISH EVOLVING\n" );
        thread->lock = 0;



        // printPopulaton(population);
        // getchar();





    }
    // printf("Thread %d\n", thread);
    // printf("thread - début : %ld\n", thread->debut );
    // printf("thread - fin : %ld\n\n", thread->fin );
    return 0;
}

void runPere(){

    fileScore = openLog("log/fruit.csv");


    Population * population = newPopulation(  );
    //printNetwork(population->firstPopulation[0]);

    Thread ** threadList = malloc(params.nbThread * sizeof(Thread));
    pthread_t * idList = malloc(params.nbThread * sizeof(pthread_t));

    //
    for( size_t i = 0; i < params.nbThread; i++){
        threadList[i] = NewThread(population, i, &idList[i] );
        // printf("O -  %d\n", threadList[i] );
        // printf("taille : %ld\n", threadList[i]->size );
        // printf("début : %ld\n", threadList[i]->debut );
        // printf("fin : %ld\n\n", threadList[i]->fin );
        // pthread_mutex_lock(&threadList[i]->mutex);
        pthread_create(&idList[i], NULL, runFils, threadList[i] );
    }

    //
    for(size_t g = 0; g < 500; g++){
        printf("GEN : %ld\n", g );

        // GAME
        for( size_t i = 0; i < params.nbThread; i++){
            // pthread_mutex_lock(&threadList[i]->mutex);
            threadList[i]->lock = 1;
            // pthread_mutex_unlock(&threadList[i]->mutex);
        }

        printf("PERE - j'attend la fin du gaming\n" );

        for( size_t i = 0; i < params.nbThread; i++){
            // pthread_mutex_lock(&threadList[i]->mutex);
            while(threadList[i]->lock){}
            // pthread_mutex_lock(&threadList[i]->mutex);
        }


        // FITNESS
        for( size_t i = 0; i < params.nbThread; i++){
            threadList[i]->lock = 1;
        }

        printf("PERE - j'attend la fin du calcul de fitness\n" );

        for( size_t i = 0; i < params.nbThread; i++){
            while(threadList[i]->lock){}
        }

        // EVOLUTION
        for( size_t i = 0; i < params.nbThread; i++){
            threadList[i]->lock = 1;
        }

        printf("PERE - j'attend la fin du calcul de l'evolution\n" );

        for( size_t i = 0; i < params.nbThread; i++){
            while(threadList[i]->lock){}
        }


        printf("FIN GEN \n" );

        // for( size_t i = 0; i < params.nbThread; i++){
        //     while(!run_thre)
        //     pthread_mutex_lock(&threadList[i]->mutex);
        // }
        // waitRep(FinGamez)
        //
        // SendMsg(calculFitness)
        // waitRep(FinCalculFitness)
        //
        // SendMessage(Evolvez)
        // WaitRep(FinEvolvez)


        // printf("gen :  %ld\n", i );
        writeLogScore(fileScore, population);

    }


    for( size_t i = 0; i < params.nbThread; i++){
        pthread_join(idList[i], NULL );
        printf("FIN THREAD %ld \n", i );
    }


    freePopulation( population );
    closeLog(fileScore);

    return;
}

void calculateFitness(Thread *thread ){
    double sum = 0;

    NeuralNetwork ** ListNeuralNetwork = thread->population->firstPopulation;

    for( size_t i = 0; i < thread->population->size; i++)
        sum += ListNeuralNetwork[i]->score;

    for( size_t i = 0; i < thread->size; i++)
        thread->ListNeuralNetwork_A[i]->fitness = thread->ListNeuralNetwork_A[i]->score  / sum;
}

void evolve(Thread *thread ){

    NeuralNetwork * tmp;
    for( size_t i = 0; i < thread->size; i++){
        // tmp = population->secondPopulation[i];
        tmp = thread->ListNeuralNetwork_B[i];

        crossover(tmp, pickOne(thread->population), pickOne(thread->population));
        mutate(tmp);
    }

    NeuralNetwork ** swap = thread->ListNeuralNetwork_A;
    thread->ListNeuralNetwork_A = thread->ListNeuralNetwork_B;
    thread->ListNeuralNetwork_B = swap;

    return;
}



/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////                              Game                                 /////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////


void game(NeuralNetwork * nn){

    Snake * snake ;

    int resultat = 4;
    int end = 0;

    initialiseGrille();
    snake = malloc(sizeof(Snake));
    initSnake(snake);

    //
    while (end == 0 && snake->health != 0) {
        resultat = compute( nn, getInput(snake, params.nbNeuronsInput) );

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


/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////                            Affichage                              /////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

void printNetwork(NeuralNetwork *nn ){


    Layer * current = nn->firstLayer;
    size_t i = 1;
    while (current->nextLayer) {
        printf("Layer %ld\n",i );
        for(size_t t = 0; t < current->size; t++){
            printf("%ld - [%lf] : ", t+1, current->neurons[t]);
            for(size_t n = 0; n < current->nextLayer->size; n++){
                printf(" {[%lf] -> %ld} ",  current->nextLayer->weights[ n * current->size + t], n+1);
            }
            printf("\n" );
        }
        printf("\n\n\n" );


        current = current->nextLayer;
        i++;
    }
    printf("Layer %ld Ouput\n",i );
    for(size_t t = 0; t < current->size; t++){
        printf("%ld - [%lf] ", t+1, current->neurons[t]);
        printf("\n" );
    }
    printf("\n\n\n\n\n\n" );

    //
    // size_t sizePrev;
    // size_t sizeCurrent;
    // size_t sizeNext;
    //
    // while( current){
    //     if(current->previousLayer)
    //         sizePrev = current->previousLayer->size;
    //     else
    //         sizePrev = 0;
    //
    //
    //     sizeCurrent = current->size;
    //
    //     if(current->nextLayer)
    //         sizeNext = current->nextLayer->size;
    //     else
    //         sizeNext = 0;
    //
    //
    //     printf("%ld > %ld > %ld\n", sizePrev, sizeCurrent, sizeNext );
    //
    //     current = current->nextLayer;
    // }

}
void printPopulaton(Population *population ){
    printf("//////////////////////////////////////////////\n" );
    printf("///////             SECOND            ////////\n" );
    printf("//////////////////////////////////////////////\n" );
    for( size_t i = 0; i < population->size; i++){
        printNetwork( population->secondPopulation[i]);
    }


    printf("//////////////////////////////////////////////\n" );
    printf("///////              FIRST            ////////\n" );
    printf("//////////////////////////////////////////////\n" );
    for( size_t i = 0; i < population->size; i++){
        printNetwork( population->firstPopulation[i]);
    }

}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////                       garbage collector                           /////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

void freePopulation( Population * population ){
    for( size_t i = 0; i < population->size; i++){
        freeNeuralNetwork( population->firstPopulation[i]);
        freeNeuralNetwork( population->secondPopulation[i]);
    }

    free(population->firstPopulation);
    free(population->secondPopulation);

    free(population);
    return;
}

void freeNeuralNetwork(NeuralNetwork * nn){


    Layer * layer = NULL;
    layer = nn->lastLayer;

    Layer * previousLayer = NULL;


    while(layer){
        previousLayer = layer->previousLayer;
        freeLayer(layer);
        layer = previousLayer;
    }
    free(nn);

    return;
}

void freeLayer(Layer * layer){

    free(layer->neurons);
    if(layer->previousLayer){

        free(layer->bias);
        free(layer->weights);
    }


    free(layer);
    return ;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////                               Log                                 /////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
FILE* openLog( char *fileName ){
    FILE* file = NULL ;
    file = fopen(fileName, "r");

    if( file != NULL){
        fseek(file, 0, SEEK_END);
        //printf("%ld\n", ftell(file) );

        if( ftell(file) != 0 ){
            printf("le fichier n'est pas vide !\n" );
            //fclose( file );
            //exit(1);
        }
        fclose( file );
    }
    file = fopen( fileName, "w");
    return file;
}

// void writeLogScore ( FILE* fichier,  NeuralNetwork ** population ){}
// void writeLogId ( FILE* fichier,  NeuralNetwork ** population ){}

void writeLogScore ( FILE* file,  Population * population ){

    for( size_t i = 0; i < params.taille_population; i++){

        fprintf( file, "%lf,",population->firstPopulation[i]->nbFruit);
        //fprintf( file, "%lf,",population[i]->score);
        //fprintf( file, "%lf,",population[i]->fitness);
    }
    fputc('\n', file);
}

void closeLog( FILE* file){
    fclose( file );
}
