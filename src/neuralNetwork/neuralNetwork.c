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
    return ( (double)(rand_r(&seed)) + 1. )/( (double)(RAND_MAX) + 1. );
}

double normalRandom(){
    double v1=rand_gen();
    double v2=rand_gen();
    return cos(2*3.14*v2)*sqrt(-2.*log(v1));
}


Config NewConfig(
    size_t taille_population,
    size_t generation,

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
    params.generation = generation;

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
        layer->bias[i] = 1 - ((double)rand_r(&seed) / (double)RAND_MAX)*2 ;
    }

    for(size_t i = 0; i < layer->size * previousSize; i++){
        layer->weights[i] = 1 - ((double)rand_r(&seed) / (double)RAND_MAX)*2 ;
    }

}
//
void setInput(Layer * layer, double * inputList){
    for(size_t i = 0; i < params.nbNeuronsInput; i++){
        layer->neurons[i] = sigmoid(inputList[i]);
    }
    // free(inputList);
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
        // layer->neurons[i] = s;
    }
}

//
void mutateLayer(Layer * layer){

    size_t previousSize = layer->previousLayer->size;
    size_t size = layer->size;

    for (size_t i = 0; i < size ; i++) {

        if( params.mutationRate > (double) rand_r(&seed) / RAND_MAX ){ (layer->bias)[i] += normalRandom() * params.sigmaMutation; }

        for (size_t j = 0; j < previousSize; j++ ){
            if( params.mutationRate > (double) rand_r(&seed) / RAND_MAX ){ (layer->weights)[i * previousSize + j] += normalRandom() * params.sigmaMutation; }
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////                          NeuralNetwork                            /////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
NeuralNetwork * newNeuralNetwork( size_t numNN ) {

    //
    NeuralNetwork * nn;
    nn = malloc( sizeof(NeuralNetwork) );

    //
    //m_size = 2 + params.nhiddenlayer;
    nn->size = 2 + params.nbHiddenLayer ;
    nn->numNN = numNN;
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
size_t computeNN(NeuralNetwork * nn, double * inputList ){

    //
    Layer * layer = nn->firstLayer;
    setInput(layer, inputList);

    layer = layer->nextLayer;

    //
    while(layer){
        computeLayer(layer);
        // boucle(1000);
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
    int remainingToLocation = ((double) rand_r(&seed)/ (double) RAND_MAX )*(params.totalWeight - params.tailleCrossoverMax);
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
        population->firstPopulation[i] = newNeuralNetwork( i );
        population->secondPopulation[i] = newNeuralNetwork( i );
    }

    return population;
}




void swap(Population * population){
        NeuralNetwork ** swap = population->firstPopulation;
        population->firstPopulation = population->secondPopulation;
        population->secondPopulation = swap;
}


NeuralNetwork * pickOne(Population *population ){
    size_t index = 0;

    double r = (double) rand_r(&seed) / (double) RAND_MAX;
    double rr = r;

    NeuralNetwork ** ListNeuralNetwork = population->firstPopulation;

    while ( r > 0) {
        if(ListNeuralNetwork[index]->fitness <= 0)
            printf("r : %lf rr: %lf index:%ld, fitt : %lf \n",r,rr,index, ListNeuralNetwork[index]->fitness );
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

void boucle( int a ){

    for(size_t i = 0; i < a; i++){}

}

Thread * NewThread(Population *population, size_t numThread, pthread_t * id){

    Thread * thread = malloc(sizeof(Thread));

    thread->count = 0;

    thread->id = id;
    thread->numThread = numThread;
    // thread->mutex = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
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
    // printf("NT - fin : %ld\n", thread->fin );
    // printf("NT - size : %ld\n", thread->size );
    // printf("NT - num : %ld\n\n", thread->numThread );

    thread->ListNeuralNetwork_A = malloc( thread->size * sizeof(Thread) );
    thread->ListNeuralNetwork_B = malloc( thread->size * sizeof(Thread) );

    for( size_t i = 0; i < thread->size; i++ ){
        thread->ListNeuralNetwork_A[i] = population->firstPopulation[ debut + i ];
        thread->ListNeuralNetwork_B[i] = population->secondPopulation[ debut + i ];
        // printf("%ld \n", debut + i);
    }




    return thread;
}

//Fonction que chaque thread fils va executer
void *runFils(void *voidThread ){

    Thread * thread = (Thread *)voidThread;

    // printf("Thread %ld\n", thread->numThread);
    // printf("%ld - thread - début : %ld\n",thread->numThread, thread->debut );
    // printf("%ld - thread - fin : %ld\n\n",thread->numThread, thread->fin );
    // printf("%ld - thread - siez : %ld\n\n",thread->numThread, thread->size );

    for(size_t gen = 0; gen < params.generation; gen++){

        //thread synchronisation pour la game
        pthread_barrier_wait(&barrier1);
        for (size_t i = 0; i < thread->size; i++) {
            game(thread->ListNeuralNetwork_A[i]);
            // boucle( 1000 );
        }

        //thread synchronisation pour le calcule de fitness
        pthread_barrier_wait(&barrier2);
        calculateFitness(thread);
        // boucle( 1000 );

        //thread synchronisation pour evolution
        pthread_barrier_wait(&barrier3);
        evolve(thread);
        // boucle( 1000 );

    }
    return 0;
}






//runPere Lance l'ia celon les parametres de la structure params.
//elle créer n threads fils puis les coordonne un certain nombre de génération
void runPere(){

    //ouverture fichier de log
    // fileScore = openLog("log/fruit.csv");

    //création de la population
    Population * population = newPopulation(  );

    //initialisation des barrières
    //la barriere bloque les threads jusqu'à ce que 'params.nbThread + 1' soient bloqués
    pthread_barrier_init(&barrier1, NULL, params.nbThread + 1);
    pthread_barrier_init(&barrier2, NULL, params.nbThread + 1);
    pthread_barrier_init(&barrier3, NULL, params.nbThread + 1);


    // boucle de création et lancement des threads

    Thread ** threadList = malloc(params.nbThread * sizeof(Thread));
    pthread_t * idList = malloc(params.nbThread * sizeof(pthread_t));

    for( size_t i = 0; i < params.nbThread; i++){
        threadList[i] = NewThread(population, i, &idList[i] );
        // printf("Thread %ld\n", threadList[i]->numThread);
        // printf("thread - début : %ld\n", threadList[i]->debut );
        // printf("thread - fin : %ld\n",threadList[i]->fin );
        // printf("thread - siez : %ld\n\n",threadList[i]->size );
        pthread_create(&idList[i], NULL, runFils, threadList[i] );
        // getchar()
    }

    //
    for(size_t g = 0; g < params.generation; g++){
        // printf("GEN : %ld\n", g );

        // GAME - resynchronise les threads
        pthread_barrier_wait(&barrier1);


        // FITNESS - resynchronise les threads
        pthread_barrier_wait(&barrier2);

        // EVOLUTION - resynchronise les threads
        pthread_barrier_wait(&barrier3);

        // printf("FIN GEN \n" );


        // printf("gen :  %ld\n", g);

        //écrit les scores dans les logs
        // writeLogScore(fileScore, population);
    }

    //termine les threads
    for( size_t i = 0; i < params.nbThread; i++){
        pthread_join(idList[i], NULL );
    }

    // libération de la mémoire
    for( size_t i = 0; i < params.nbThread; i++){
        free(threadList[i]);
    }

    free(threadList);
    free(idList);

    freePopulation( population );
    // closeLog(fileScore);

    return;
}

void calculateFitness(Thread *thread ){
    double sum = 0;

    NeuralNetwork ** ListNeuralNetwork = thread->population->firstPopulation;

    for( size_t i = 0; i < thread->population->size; i++)
        sum += ListNeuralNetwork[i]->score;

    for( size_t i = 0; i < thread->size; i++){
        thread->ListNeuralNetwork_A[i]->fitness = thread->ListNeuralNetwork_A[i]->score  / sum;
        if( thread->ListNeuralNetwork_A[i]->fitness != 0.001000)
            printf("thread: %ld nn:%ld - fit %lf\n",thread->numThread, thread->ListNeuralNetwork_A[i]->numNN ,thread->ListNeuralNetwork_A[i]->fitness);
    }

}

void evolve(Thread *thread ){

    NeuralNetwork * tmp;
    for( size_t i = 0; i < thread->size; i++){
        // tmp = population->secondPopulation[i];
        tmp = thread->ListNeuralNetwork_B[i];

        // crossover(tmp, pickOne(thread->population), pickOne(thread->population));
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
    // printf("%ld game\n", nn->numNN);
    double * tableau = malloc(8 * sizeof(double)); /* déclare un tableau de 10 entiers */

    for (int i = 0; i < 8; i++) /* boucle << classique >> pour le parcours d'un tableau */
    {
        tableau[i] = 0;  /* chaque case du tableau reçoit son indice comme valeur */
    }

    for (int i = 0; i < 10; i++)
        computeNN( nn, tableau );


    free(tableau);
    setScore(nn, 10, 10);
}


void gameNul(NeuralNetwork * nn){

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
            // printf("le fichier n'est pas vide !\n" );
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
