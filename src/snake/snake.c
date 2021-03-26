
#include <stdio.h>
#include <stdlib.h>
#include "snake.h"
#include <math.h>



/* grille du morpion valeurs possibles VIDE, ROND ou CROIX */


double * getInput( Snake * snake, int nbInput) {
    // double * inputList = malloc(nbInput * sizeof(double)) ;
    //
    // int ligneHead = snake->head->ligne;
    // int colonneHead = snake->head->colonne;
    //
    // int ligneFruit = snake->fruit->ligne;
    // int colonneFruit = snake->fruit->colonne;
    //
    // inputList[0] = ligneFruit - ligneHead;
    // inputList[1] = colonneFruit - colonneHead ;
    //
    // //case dispo haut
    // int index = 1;
    // while( (ligneHead - index)>=0 && grille[ligneHead - index][colonneHead] != SNAKE ){
    //     index+=1;
    // }
    // inputList[2] = (index-1)*2 -10 ;
    //
    // //case dispo bas
    // index = 1;
    // while( (ligneHead + index) < NB_LIG && grille[ligneHead + index][colonneHead] != SNAKE ){
    //     index+=1;
    // }
    // inputList[3] =  (index-1)*2 -10 ;
    //
    // //case dispo gauche
    // index = 1;
    // while( (colonneHead - index)>=0 && grille[ligneHead][colonneHead - index] != SNAKE ){
    //     index+=1;
    // }
    // inputList[4] = (index-1)*2 -10 ;
    //
    // //case dispo droite
    // index = 1;
    // while( (colonneHead + index) < NB_COL && grille[ligneHead][colonneHead + index] != SNAKE ){
    //     index+=1;
    // }
    // inputList[5] =  (index-1)*2 -10 ;
    //
    // return inputList;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            //                                            input v2 : 8 input
    double * inputList = malloc(nbInput * sizeof(double)) ;

    int ligneHead = snake->head->ligne;
    int colonneHead = snake->head->colonne;

    inputList[0] = -10 ; //Danger haut
    inputList[1] = -10 ; //Danger bas
    inputList[2] = -10 ; //Danger droite
    inputList[3] = -10 ; //Danger gauche

    if( !(ligneHead-1 >= 0) || grille[ligneHead-1][colonneHead] == SNAKE  )
        inputList[0] = 10;

    if( !(ligneHead+1 < NB_LIG) || grille[ligneHead+1][colonneHead] == SNAKE  )
        inputList[1] = 10;

    if( !(colonneHead+1 < NB_COL) || grille[ligneHead][colonneHead+1] == SNAKE  )
        inputList[2] = 10;

    if( !(colonneHead-1 >= 0)  || grille[ligneHead][colonneHead-1] == SNAKE  )
        inputList[3] = 10;


    //
    int ligneFruit = snake->fruit->ligne;
    int colonneFruit = snake->fruit->colonne;

    inputList[4] = -10 ; //fruit haut
    inputList[5] = -10 ; //fruit bas
    inputList[6] = -10 ; //fruit droite
    inputList[7] = -10 ; //fruit gauche

    if(ligneFruit < ligneHead)
        inputList[4] = 10 ;
    else if (ligneFruit > ligneHead)
        inputList[5] = 10 ;

    if(colonneFruit > colonneHead)
        inputList[6] = 10 ;
    else if (colonneFruit < colonneHead)
        inputList[7] = 10 ;


    return inputList;

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // v3
    // double * inputList = malloc(nbInput * sizeof(double)) ;
    //
    // int ligneHead = snake->head->ligne;
    // int colonneHead = snake->head->colonne;
    //
    // //
    // int ligneFruit = snake->fruit->ligne;
    // int colonneFruit = snake->fruit->colonne;
    //
    // inputList[0] = -10 ; //fruit haut
    // inputList[1] = -10 ; //fruit bas
    // inputList[2] = -10 ; //fruit droite
    // inputList[3] = -10 ; //fruit gauche
    //
    // if(ligneFruit < ligneHead)
    //     inputList[0] = 10 ;
    // else if (ligneFruit > ligneHead)
    //     inputList[1] = 10 ;
    //
    // if(colonneFruit > colonneHead)
    //     inputList[2] = 10 ;
    // else if (colonneFruit < colonneHead)
    //     inputList[3] = 10 ;
    //
    //
    // return inputList;

    //                                                     input v4 : 8 input
    // double * inputList = malloc(nbInput * sizeof(double)) ;
    //
    // //
    // int ligneHead = snake->head->ligne;
    // int colonneHead = snake->head->colonne;
    //
    // //
    // int ligneFruit = snake->fruit->ligne;
    // int colonneFruit = snake->fruit->colonne;
    //
    // //
    // int index = 0;
    //
    //
    // //                          DISTANCE FRUIT
    // inputList[0] = ((10 - sqrt( pow((ligneHead - ligneFruit),2) + pow((colonneHead - colonneFruit),2)) ) * 2 ) -10;
    //
    //
    // //                          FRUIT
    // //
    //
    //
    // // HAUT
    // if(colonneFruit == colonneHead && ligneFruit < ligneHead)
    //     //index = ligneHead - ligneFruit;
    //     index = 99;
    // else
    //     index = -99;
    //
    // inputList[1] = index;
    //
    //
    // // BAS
    // if(colonneFruit == colonneHead && ligneFruit > ligneHead)
    //     //index = ligneHead - ligneFruit;
    //     index = 99;
    // else
    //     index = -99;
    //
    // inputList[2] = index;
    //
    //
    // // DROITE
    // if(ligneFruit == ligneHead && colonneFruit > colonneHead)
    //     //index = ligneHead - ligneFruit;
    //     index = 99;
    // else
    //     index = -99;
    //
    // inputList[3] = index;
    //
    //
    // // GAUCHE
    // if(ligneFruit == ligneHead && colonneFruit < colonneHead)
    //     //index = ligneHead - ligneFruit;
    //     index = 99;
    // else
    //     index = -99;
    //
    // inputList[4] = index;
    //
    //
    // // HAUT DROITE
    // if( (colonneFruit - colonneHead) == (-(ligneFruit - ligneHead)) && (ligneFruit - ligneHead) < 0  )
    //     //index = ligneHead - ligneFruit;
    //     index = 99;
    // else
    //     index = -99;
    //
    // inputList[5] = index;
    //
    //
    // // BAS DROITE
    // if( (colonneFruit - colonneHead) == (-(ligneFruit - ligneHead)) && (ligneFruit - ligneHead) > 0  )
    //     //index = ligneHead - ligneFruit;
    //     index = 99;
    // else
    //     index = -99;
    //
    // inputList[6] = index;
    //
    //
    // // HAUT GAUCHE
    // if( (colonneFruit - colonneHead) == (ligneFruit - ligneHead) && (ligneFruit - ligneHead) < 0  )
    //     //index = ligneHead - ligneFruit;
    //     index = 99;
    // else
    //     index = -99;
    //
    // inputList[7] = index;
    //
    //
    // // BAS GAUCHE
    // if( (colonneFruit - colonneHead) == (ligneFruit - ligneHead) && (ligneFruit - ligneHead) > 0  )
    //     //index = ligneHead - ligneFruit;
    //     index = 99;
    // else
    //     index = -99;
    //
    // inputList[8] = index;
    //
    //
    // //                          SAFE ZONE
    // //HAUT
    // index = 1;
    // while( (ligneHead - index) >= 0 && grille[ligneHead - index][colonneHead] != SNAKE ){
    //     index+=1;
    // }
    // inputList[9] = (index-1)*2 -10 ;
    //
    //
    // //BAS
    // index = 1;
    // while( (ligneHead + index) < NB_LIG && grille[ligneHead + index][colonneHead] != SNAKE ){
    //     index+=1;
    // }
    // inputList[10] =  (index-1)*2 -10 ;
    //
    //
    // //DROITE
    // index = 1;
    // while( (colonneHead + index) < NB_COL && grille[ligneHead][colonneHead + index] != SNAKE ){
    //     index+=1;
    // }
    // inputList[11] =  (index-1)*2 -10 ;
    //
    //
    // //GAUCHE
    // index = 1;
    // while( (colonneHead - index) >= 0 && grille[ligneHead][colonneHead - index] != SNAKE ){
    //     index+=1;
    // }
    // inputList[12] = (index-1)*2 -10 ;


    // //HAUT DROITE
    // index = 1;
    // while( (ligneHead - index) >= 0 && (colonneHead + index) < NB_COL && grille[ligneHead - index][colonneHead + index] != SNAKE ){
    //     index+=1;
    // }
    // inputList[12] = (index-1)*2 -10 ;
    //
    //
    // //BAS DROITE
    // index = 1;
    // while( (ligneHead + index) < NB_LIG && (colonneHead + index) < NB_COL && grille[ligneHead + index][colonneHead + index] != SNAKE ){
    //     index+=1;
    // }
    // inputList[13] =  (index-1)*2 -10 ;
    //
    //
    // //HAUT GAUCHE
    // index = 1;
    // while( (ligneHead - index) >= 0 && (colonneHead - index) >= 0  && grille[ligneHead - index][colonneHead - index] != SNAKE ){
    //     index+=1;
    // }
    // inputList[14] = (index-1)*2 -10 ;
    //
    // //BAS GAUCHE
    // index = 1;
    // while( (ligneHead - index) < NB_LIG && (colonneHead - index) >= 0  && grille[ligneHead + index][colonneHead - index] != SNAKE ){
    //     index+=1;
    // }
    // inputList[15] = (index-1)*2 -10 ;

    return inputList;

}
/*
 * Initiliase la grille du morpion a vide
 */
void initialiseGrille() {
  int i, j;
  for (i=0; i<NB_LIG; i++) {
    for (j=0; j<NB_COL; j++) {
      grille[i][j] = VIDE;
    }
  }
}

/*
  Affiche la grille du morpion
  _ indique case vide, O pion joueur 1 et X pion jour 2
 */
void afficheGrille() {
    int i, j;
    //printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n" );
    for (i=0; i<NB_LIG; i++) {
        for (j=0; j<NB_COL; j++) {
            switch (grille[i][j]) {
                case VIDE:
                    printf("_");
                    break;
                case SNAKE:
                    printf("O");
                    break;
                case FRUIT:
                    printf("X");
                    break;
            }
        }
        printf("\n"); /* fin de la ligne */
    }
}

//
void initSnake( Snake * snake){
    /*snake->head = NULL;
    snake->queue = NULL;*/
    snake->length = 0;
    snake->head = NULL;
    snake->queue = NULL;

    addHead( snake, 0, 0);
    snake->queue = snake->head;
    addHead( snake, 0, 1);

    snake->fruit = malloc(sizeof(Fruit));
    generateFruit(snake);

    snake->score = 1;
    snake->nbFruit = 0;
    snake->health = 100;
}

//
int generateFruit(Snake * snake){

    int taille = 0;
    int tab[100] ;

    for(int l = 0; l < NB_LIG; l++ ){
        for(int c = 0; c < NB_COL; c++ ){
            if( grille[l][c] == VIDE ){
                tab[taille] = l * NB_COL + c;
                taille++;
            }
        }
    }
    if(taille == 0){
        return 1;
    }

    int index = rand()%taille;
    index = tab[index];

    snake->fruit->ligne = index/NB_LIG;
    snake->fruit->colonne = index%NB_COL;

    grille[snake->fruit->ligne][snake->fruit->colonne] = FRUIT;
    return 0;

    // int ligne, colonne;
    // do {
    //     ligne = rand()%NB_LIG;
    //     colonne = rand()%NB_COL;
    // } while( grille[ligne][colonne] != VIDE );
    // snake->fruit->ligne = ligne;
    // snake->fruit->colonne = colonne;
    //
    // grille[ligne][colonne] = FRUIT;

}

//
void deleteQueue(Snake * snake){
    //return;
    Body * old = snake->queue;
    if(! old || !old->next)
        return;
    snake->queue = old->next;
    grille[old->ligne][old->colonne] = VIDE;

    old->next->previous = NULL;
    old->next = NULL;
    free(old);
    return;
}
//
void addHead(Snake * snake, int ligne, int colonne){
    //if( ligne <= NB_LIG)

    //printf("ligne : %d\n colonne : %d\n", ligne, colonne );
    //
    Body * new = malloc(sizeof(Body));

    if(!new)
        exit(EXIT_FAILURE);
    if(snake->head)
        snake->head->next = new;
    new->previous = snake->head;
    new->next = NULL;
    snake->head = new;

    //
    new->ligne = ligne;
    new->colonne = colonne;
    grille[ligne][colonne] = SNAKE;

    snake->length += 1;

    return ;
}

int move(Snake * snake, int i, int j){

    if((snake->head->ligne + i >= NB_LIG) || (snake->head->colonne + j >= NB_COL) || (snake->head->ligne + i < 0 ) || (snake->head->colonne + j < 0) ){
        //printf("tu sorts du cadre\n" );
        return 1;}

    ValeurGrille val = grille[snake->head->ligne + i ][snake->head->colonne + j];

    if( val == SNAKE){;
        //printf("tu as touché le serpent\n" );
        return 1;}

    addHead( snake, snake->head->ligne + i, snake->head->colonne + j);

    //snake->score -= 1;
    snake->health -= 1;
    snake->score += 1;
    if( val == FRUIT){
        if(generateFruit(snake))
            return 2;
        snake->score += 20;
        snake->nbFruit += 1;
        snake->health = 100;
    }


    if(val == VIDE)
        deleteQueue(snake);



    //printf("ccc\n" );
    return 0;
}

void destroySnake( Snake * snake ){
    Body * body = snake->head;
    //body->previous;

    Body * bodyTemp;

    while(body){
        bodyTemp = body->previous;
        free(body);
        body = bodyTemp;
    }

    free(snake->fruit);
    free(snake);
}

double getScore(Snake * snake){
    return snake->score;
}

double getFruit(Snake * snake){
    return snake->nbFruit;
}
/*
int main(int argc, char const *argv[]) {

    srand(time(NULL));

    Snake * snake = malloc(sizeof(Snake));

    initialiseGrille();
    initSnake(snake);
    char choice = 0;
    Boolean end = FALSE;

    while (end == FALSE) {
        afficheGrille();
        printf("\n\n>");
        scanf("%c",&choice );
        getchar();
        switch (choice) {
            case 'z':
                end = move(snake, -1, 0);
                break;
            case 's':
                end = move(snake, 1, 0);
                break;
            case 'q':
                end = move(snake, 0, -1);
                break;
            case 'd':
                end = move(snake, 0, 1);
                break;
            default:
                break;
                printf("fin du jeu\n" );
                end = TRUE;
                break;
        }


    }

    destroySnake(snake);
    return 0;

}
*/
