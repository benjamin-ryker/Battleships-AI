/**
 * @brief Hit All test AI for battleships
 * @file HitAll.cpp
 * @author Stefan Brandle, Jonathan Geisler, Scotti Bozarth, Jordan Richardson
 * @date September, 2004 Updated 2015 for multi-round play, Updates Oct-Nov 2016 for COS 121
 *
 * This Battleships AI is very simple and does nothing beyond playing
 * a legal game. However, that makes it a good starting point for writing
 * a more sophisticated AI.
 *
 * The constructor
 */

/**
 *@brief Strategic Planning
 * Making a Learning Clean Probable Player
 * Shot Placement Finished, specify quadrents to increase probibility
 * FIXME Ship placement bassed on heatmap needs implementation 
 */

#include <iostream>
#include <cstdio>
#include <cmath>
#include <stdlib.h> // in for testing
#include <time.h> // in to see if works, probably dont need
#include "conio.h"
#include "HitAll.h"

using namespace std;
using namespace conio;

/**
 * @brief Constructor that initializes any inter-round data structures.
 * @param boardSize Indication of the size of the board that is in use.
 *
 * The constructor runs when the AI is instantiated (the object gets created)
 * and is responsible for initializing everything that needs to be initialized
 * before any of the rounds happen. The constructor does not get called 
 * before rounds; newRound() gets called before every round.i
 *
 *If random number generator put it in here.
 */
HitAll::HitAll( int boardSize )
    :PlayerV2(boardSize)
{
    for (int i = 0; i< boardSize; ++i) {
        for (int j=0; j< boardSize; ++j) {
            HitHeatmap[i][j] = (10-(abs(4-i) + abs(4-j))); // favors upper left by 1, divides by two so it can start learning imedieatly.
            }
    }
    for (int i = 0; i< boardSize; ++i) {
        for (int j=0; j< boardSize; ++j) {
            ShipHeatmap[i][j] =(10-(abs(5-i) + abs(5-j)));// favors bottom right by 1, divides by 2 so can start learning imediatly
        }
    }
    //srand(time(0));//there is a universal seed.
    for (int i=rand()%2048; i > 0; --i){//to mess with prediction people
        rand();
    }
}

/**
 * @brief Destructor placeholder.
 * If your code does anything that requires cleanup when the object is
 * destroyed, do it here in the destructor.
 */
HitAll::~HitAll( ) {}

/*
 * Private internal function that initializes a MAX_BOARD_SIZE 2D array of char to water.
 */
void HitAll::initializeBoard() {
    for(int row=0; row<boardSize; row++) {
	for(int col=0; col<boardSize; col++) {
	    this->board[row][col] = WATER;
	}
    }
}
/**
 * @brief Hit Functions
 * Functions used for the hit mask to determine where to fire.
 * Order will be determined by quadrant and possibly a learning algorithm.
 */

void HitAll::HitDefaultLU(int locRow, int locCol){//FIXME rewrite a default for each quadrent. Perhaps institue a function that determines up down, left right based on opponent A.I. preference. 
    if (opponentBoard[locRow][locCol+1] == WATER && locCol+1 < boardSize) {
        fireRow = locRow;
        fireCol = locCol +1;
    }
    else if (opponentBoard[locRow+1][locCol] == WATER && locRow+1 < boardSize) {
        fireRow = locRow+1;
        fireCol = locCol;
    }
    else if (opponentBoard[locRow][locCol-1] == WATER && locCol-1 >= 0) {
        fireRow = locRow;
        fireCol = locCol -1;
    }   
    else if (opponentBoard[locRow-1][locCol] == WATER && locRow-1 >= 0) {
        fireRow = locRow-1;
        fireCol = locCol;
    }
}
void HitAll::HitDefaultRU(int locRow, int locCol){//FIXME rewrite a default for each quadrent. Perhaps institue a function that determines up down, left right based on opponent A.I. preference. 
    if (opponentBoard[locRow-1][locCol] == WATER && locRow-1 >= 0) {
        fireRow = locRow-1;
        fireCol = locCol;
    }
    else if (opponentBoard[locRow][locCol+1] == WATER && locCol+1 < boardSize) {
        fireRow = locRow;
        fireCol = locCol +1;
    }
    else if (opponentBoard[locRow+1][locCol] == WATER && locRow+1 < boardSize) {
        fireRow = locRow+1;
        fireCol = locCol;
    }
    else if (opponentBoard[locRow][locCol-1] == WATER && locCol-1 >= 0) {
        fireRow = locRow;
        fireCol = locCol -1;
    }   
}
void HitAll::HitDefaultRD(int locRow, int locCol){//FIXME rewrite a default for each quadrent. Perhaps institue a function that determines up down, left right based on opponent A.I. preference. 
    if (opponentBoard[locRow][locCol-1] == WATER && locCol-1 >= 0) {
        fireRow = locRow;
        fireCol = locCol -1;
    }
    else if (opponentBoard[locRow-1][locCol] == WATER && locRow-1 >= 0) {
        fireRow = locRow-1;
        fireCol = locCol;
    }
    else if (opponentBoard[locRow][locCol+1] == WATER && locCol+1 < boardSize) {
        fireRow = locRow;
        fireCol = locCol +1;
    }
    else if (opponentBoard[locRow+1][locCol] == WATER && locRow+1 < boardSize) {
        fireRow = locRow+1;
        fireCol = locCol;
    }
}
void HitAll::HitDefaultLD(int locRow, int locCol){//FIXME rewrite a default for each quadrent. Perhaps institue a function that determines up down, left right based on opponent A.I. preference. 
    if (opponentBoard[locRow][locCol+1] == WATER && locCol+1 < boardSize) {
        fireRow = locRow;
        fireCol = locCol +1;
    }
    else if (opponentBoard[locRow][locCol-1] == WATER && locCol-1 >= 0) {
        fireRow = locRow;
        fireCol = locCol -1;
    }
    else if (opponentBoard[locRow+1][locCol] == WATER && locRow+1 < boardSize) {
        fireRow = locRow+1;
        fireCol = locCol;
    }
    else if (opponentBoard[locRow-1][locCol] == WATER && locRow-1 >= 0) {
        fireRow = locRow-1;
        fireCol = locCol;
    }
}
void HitAll::HitRight(int locRow, int locCol){
    if (opponentBoard[locRow][locCol+1] == HIT && locCol+1 < boardSize) {
        HitRight(locRow, locCol+1);
    }
    else if (opponentBoard[locRow][locCol+1] == WATER && locCol+1 < boardSize && opponentBoard[locRow][locCol-1] == HIT && locCol-1 >=0) {
        fireRow = locRow;
        fireCol = locCol+1;
    }
}
void HitAll::HitLeft(int locRow, int locCol){
    if (opponentBoard[locRow][locCol-1] == HIT && locCol-1 >= 0) {
        HitLeft(locRow, locCol-1);
    }
    else if (opponentBoard[locRow][locCol-1] == WATER && locCol-1 >= 0 && opponentBoard[locRow][locCol+1] == HIT && locCol+1 < boardSize) {
        fireRow = locRow;
        fireCol = locCol-1;
    }
}
void HitAll::HitUp(int locRow, int locCol){
    if (opponentBoard[locRow+1][locCol] == HIT && locRow+1 < boardSize) {
        HitUp(locRow+1, locCol);
    }
    else if (opponentBoard[locRow+1][locCol] == WATER && locRow+1 < boardSize && opponentBoard[locRow-1][locCol] == HIT && locRow-1 >=0) {
        fireRow = locRow+1;
        fireCol = locCol;
    }
}
void HitAll::HitDown(int locRow, int locCol){
    if (opponentBoard[locRow-1][locCol] == HIT && locRow-1 >= 0) {
        HitDown(locRow-1, locCol);
    }
    else if (opponentBoard[locRow-1][locCol] == WATER && locRow-1 >= 0 && opponentBoard[locRow+1][locCol]==HIT && locRow+1 < boardSize) {
        fireRow = locRow-1;
        fireCol = locCol;
    }
}

/**
 * @brief Specifies the AI's shot choice and returns the information to the caller.
 * @return Message The most important parts of the returned message are 
 * the row and column values. 
 *
 * See the Message class documentation for more information on the 
 * Message constructor.
 */
Message HitAll::getMove() {//go through firing solution, on true values look sum all 4 adjacent spots. choose value with the highest number, set it to false and return location. Later implement hit masking that searches current board for hits, and if ANY are found acts upon it.
    solMax = 0;
    solTemp = 0;
    haveShot = false;

    for (int i =0; i<boardSize; ++i){
        for (int j=0; j<boardSize;++j){
            if (haveShot == true) {
                break;
            }
            else if (opponentBoard[i][j] == HIT) {//down goes up right goes right. Inverted Axis
                if (i < 5){
                    if (j < 5){//LU
                        HitDefaultLU(i,j);
                        HitDown(i,j);
                        HitLeft(i,j);
                        HitUp(i,j);
                        HitRight(i,j);
                    }
                    else{//LD
                        HitDefaultLD(i,j);
                        HitUp(i,j);
                        HitLeft(i,j);
                        HitDown(i,j);
                        HitRight(i,j);
                    }
                }
                else {
                    if (j < 5){//RU
                        HitDefaultRU(i,j);
                        HitDown(i,j);
                        HitRight(i,j);
                        HitUp(i,j);
                        HitLeft(i,j);
                    }
                    else {//RD
                        HitDefaultRD(i,j);
                        HitUp(i,j);
                        HitRight(i,j);
                        HitDown(i,j);
                        HitLeft(i,j);
                    }
                }
                haveShot = true;
            }
            else if (firingSolution[i][j] == true){
                solTemp = 0;
                solTemp += HitHeatmap[i][j]; 
                if (i+1 < boardSize){//prevents the function from looking outside the board
                    solTemp +=  HitHeatmap[i+1][j]; 
                }
                if (i-1 >= 0){
                    solTemp +=  HitHeatmap[i-1][j]; 
                }
                if (j+1 < boardSize){
                    solTemp +=  HitHeatmap[i][j+1];
                }
                if (j-1 >= 0){ 
                    solTemp += HitHeatmap[i][j-1];
                } 
                if (solTemp > solMax){
                    fireRow = i;
                    fireCol= j;
                    solMax=solTemp;
                }
            }
        }
    }
    firingSolution[fireRow][fireCol] = false;
    Message result( SHOT, fireRow, fireCol, "Firing Main Cannon", None, 1 );
    return result;
}

/**
 * @brief Tells the AI that a new round is beginning.
 * The AI show reinitialize any intra-round data structures.
 */
void HitAll::newRound() {
    /* DumbPlayer is too simple to do any inter-round learning. Smarter players 
     * reinitialize any round-specific data structures here.
     */
    this->fireRow = 0;
    this->fireCol = 0;
    this->numShipsPlaced = 0;
    this->initializeBoard();
    int temp = rand()%3;
    for (int i = 0; i < boardSize; ++i) {//this breaks when size is not 10, and by breaks i mean isnt most efficient pattern
        for (int j = 0; j < boardSize; ++j) {
            if ((i*boardSize + j)%3 == temp){
                firingSolution[i][j] = true;
            }
            else {
                firingSolution[i][j] = false;
            }
        }
    }
    for (int i = 0; i< boardSize; ++i) {
        for (int j = 0; j< boardSize; ++j){
        myBoard[i][j] = false;
        }
    }
    for (int i = 0; i< boardSize; ++i) {
        for (int j = 0; j< boardSize; ++j){
        opponentBoard[i][j] = WATER;
        }
    }
    rand(); //In here to confuse prediction A.I.
    numShots = 0; //to weight additions to ship heatmap.
}

/**
 * @brief Gets the AI's ship placement choice. This is then returned to the caller.
 * @param length The length of the ship to be placed.
 * @return Message The most important parts of the returned message are 
 * the direction, row, and column values. 
 *
 * The parameters returned via the message are:
 * 1. the operation: must be PLACE_SHIP 
 * 2. ship top row value
 * 3. ship top col value
 * 4. a string for the ship name
 * 5. direction Horizontal/Vertical (see defines.h)
 * 6. ship length (should match the length passed to placeShip)
 */
Message HitAll::placeShip(int length) {
    char shipName[10];
    // Create ship names each time called: Ship0, Ship1, Ship2, ...
    snprintf(shipName, sizeof shipName, "Ship%d", numShipsPlaced);
    /*
     * Look at Ship Hetmap, from each tile try down and right. dont do last 2 either way if we want to optimize. Look a the spots available, choose only if all slots are on the board.
     * choose only if an existing ship does not occupy one of those spots (myBoard), choose the one with the smallest sum (ShipHeatmap). Update location on (myBoard). Return placement.
     * NOTE the place ship function tells us what the legnth should be. Unrelated, if we want 
     */
    // parameters = mesg type (PLACE_SHIP), row, col, a string, direction (Horizontal/Vertical)
    // When size is less than 5 all ship placements are incorrect
   solMin=1000000;

    for(int i =0; i< boardSize; ++i){
        for(int j=0; j< boardSize; ++j){
            isTrue = 0;
            for(int k=0; k<length; ++k){
                if(myBoard[i+k][j] == false){
                    ++isTrue;
                }
            }
            if(isTrue == length){
                if(i+length < boardSize){
                    solTemp = 0;
                    for(int k =0;k < length;++k){
                        solTemp += ShipHeatmap[i+k][j];
                    }
                    if(solTemp < solMin){
                        solMin= solTemp;
                        fireRow = i;
                        fireCol = j;
                        placeDirection = Vertical;
                    }    
                }
            }
            isTrue = 0;
            for(int k=0; k<length; ++k){
                if(myBoard[i][j+k] == false){
                    ++isTrue;
                }
             }
             if (isTrue == length){
                if(j+length <boardSize){
                    solTemp = 0;
                    for(int k =0;k < length;++k){
                        solTemp += ShipHeatmap[i][j+k];
                    }
                    if(solTemp < solMin){
                        solMin= solTemp;
                        fireRow = i;
                        fireCol = j;
                        placeDirection = Horizontal;
                     }
	        }
            }
        }	
    }
    for (int k = 0; k< length; ++k){
        if (placeDirection == Vertical){
            myBoard[fireRow+k][fireCol] = true;
        }
        else if (placeDirection == Horizontal){
            myBoard[fireRow][fireCol+k] = true;
        }
    }
    Message response( PLACE_SHIP, fireRow, fireCol, shipName, placeDirection, length );
    numShipsPlaced++;

    return response;
}

/**
 * @brief Updates the AI with the results of its shots and where the opponent is shooting.
 * @param msg Message specifying what happened + row/col as appropriate.
 */
void HitAll::update(Message msg) {
    switch(msg.getMessageType()) {
	case HIT:
            HitHeatmap[msg.getRow()][msg.getCol()]++;//this tells us if we hit, increments most likely spaces for oponents ship to me. 
            opponentBoard[msg.getRow()][msg.getCol()] = msg.getMessageType();
	case KILL://Updates spots with kill when the ship is dead
            opponentBoard[msg.getRow()][msg.getCol()] = msg.getMessageType();
	case MISS://Updates spots with miss if there was no hit.
	    opponentBoard[msg.getRow()][msg.getCol()] = msg.getMessageType();///updates location on the board where the miss occured.
	    break;
	case WIN:
	    break;
	case LOSE:
	    break;
	case TIE:
	    break;
	case OPPONENT_SHOT://Keeps track of where the opponent is most likely to shoot
            ++numShots;
            if (numShots <10) {//increments by 3 if shot at location is in the first 10 shot at locations
                ShipHeatmap[msg.getRow()][msg.getCol()]+=3;
            }
            else if (numShots < 25){//increments by 2 if in the first 25
                ShipHeatmap[msg.getRow()][msg.getCol()]+=2;
            }
            else {//increments by one for the rest
                ShipHeatmap[msg.getRow()][msg.getCol()]++;
            }
	    // TODO: get rid of the cout, but replace in your AI with code that does something
	    // useful with the information about where the opponent is shooting.
	    //cout << gotoRowCol(20, 30) << "DumbPl: opponent shot at "<< msg.getRow() << ", " << msg.getCol() << flush;
	    break;
    }
}

