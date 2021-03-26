#ifndef NN_H
    #define NN_H

    #define TAILLE_POPULATION 1000
    //#define CROSSOVER_NUMBER 100
    #define NB_INPUT 8
    #define NB_HIDDEN_LAYER 1
    #define NB_NEURONS_HIDDEN 8
    #define NB_NEURONS_OUTPUT 4
    #define MUTATION_RATE 0.3
    #define SIGMA_MUTATION 0.15
    //#define SIGMA_CROSSOVER 1
    #define CROSSOVER_RATE 0.3

    int TAILLE_CROSSOVER_MAX ;//10 //= (TAILLE_POPULATION * CROSSOVER_PERCENT ) / 100;
    int TOTAL_WEIGHT ; //40

    char *inputChar[20];// = {"haut","bas","gauche","droite"};
    char *outputChar[4];

    FILE* fileScore;
    FILE* fileId;
    FILE* fileFruit;

    typedef struct Layer
    {
        double * neurons;
        double * weights;
        double * bias;

        size_t size;

        struct Layer * nextLayer;
        struct Layer * previousLayer;
    }Layer;

    //
    typedef struct NeuralNetwork
    {
        double score;
        double bruteScore;
        double nbFruit;
        double fitness;

        size_t size;

        struct Layer * firstLayer;
        struct Layer * lastLayer;

    }NeuralNetwork;

    //
    typedef struct Population
    {

        size_t size;

        /*
        struct NeuralNetwork ***current;
        struct NeuralNetwork ***old;
        */
        struct NeuralNetwork ** firstPopulation;
        struct NeuralNetwork ** secondPopulation;

    }Population;
    /////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////                              Other                                /////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    double sigmoid(double x);
    double rand_gen();
    double normalRandom();

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////                              Layer                                /////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////
    Layer * newLayer();

    void initWeigth(Layer * layer, size_t previousSize);

    void setInput(Layer * layer, double * inputList);

    void computeLayer(Layer * layer);

    void mutateLayer(Layer * layer);

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////                          NeuralNetwork                            /////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////
    NeuralNetwork * newNeuralNetwork();

    size_t compute(NeuralNetwork * nn, double * inputList );

    size_t output(NeuralNetwork * nn );

    void crossover(NeuralNetwork * nn, NeuralNetwork * father, NeuralNetwork * mother );

    void mutate(NeuralNetwork * nn );

    void setScore(NeuralNetwork * nn, double score, double nbFruit);



    /////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////                           population                              /////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    Population * newPopulation();

    // void run(Population *population );
    //
    // void game(NeuralNetwork * nn);

    void calculateFitness(Population *population );

    void evolve(Population *population );

    NeuralNetwork * pickOne(Population *population );

    NeuralNetwork * bestElement(Population *population );


    /////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////                            Affichage                              /////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////
    void printNetwork(NeuralNetwork *nn );
    void printPopulaton(Population *population );

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////                       garbage collector                           /////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    void freePopulation( Population * population);
    void freeNeuralNetwork( NeuralNetwork * nn);
    void freeLayer( Layer * layer);

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////                               Log                                 /////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////
    FILE* openLog( char *fileName );
    void writeLogScore ( FILE* fichier,  Population * population );
    void writeLogId ( FILE* fichier,  Population * population );
    void writeLogFruit( FILE* fichier,  Population * population );
    void closeLog( FILE* fichier);



#endif
