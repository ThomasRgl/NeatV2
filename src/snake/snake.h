#ifndef _SNAKE_H_
    #define _SNAKE_H_

    #define NB_LIG 10
    #define NB_COL 10


    typedef enum {VIDE, SNAKE, FRUIT} ValeurGrille;
    typedef enum {FALSE, TRUE} Boolean;


    typedef struct Body
    {
        int ligne;
        int colonne;

        //ValeurGrille value;

        struct Body * next;
        struct Body * previous;

    } Body;

    typedef struct Snake
    {
        double score;
        int length;
        double nbFruit;
        double health ;

        ValeurGrille grille[NB_LIG][NB_COL];

        int ligneFruit;
        int colonneFruit;

        struct Body * head;
        struct Body * queue;
    } Snake;
    unsigned int  seed;

    void initialiseGrille(Snake * snake);
    void afficheGrille(Snake * snake);
    void initSnake(Snake * snake );
    int generateFruit(Snake * snake );
    void deleteQueue(Snake * snake );
    void addHead(Snake * snake, int ligne, int colonne);
    int move(Snake * snake, int i, int j);
    //void destroyBody(Body * body);
    double * getInput(Snake * snake, int nbInput);

    //
    double getScore(Snake * snake);
    double getFruit(Snake * snake);

    void destroySnake(Snake * snake);
    //void destroyFruit(Fruit * fruit);
    void destroyBody(Body * body);


#endif
