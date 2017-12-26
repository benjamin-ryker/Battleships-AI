/**
 * @brief DumbPlayer AI for battleships
 * @file myPlayer.cpp
 * @author Stefan Brandle, Jonathan Geisler
 * @date September, 2004 Updated 2015 for multi-round play.
 *
 * This Battleships AI is very simple and does nothing beyond playing
 * a legal game. However, that makes it a good starting point for writing
 * a more sophisticated AI.
 *
 * The constructor
 */

#include <iostream>
#include <cstdio>
#include <stdlib.h>
#include <time.h>

#include "conio.h"
#include "myPlayer.h"

using namespace conio;

/**
 * @brief Constructor that initializes any inter-round data structures.
 * @param boardSize Indication of the size of the board that is in use.
 *
 * The constructor runs when the AI is instantiated (the object gets created)
 * and is responsible for initializing everything that needs to be initialized
 * before any of the rounds happen. The constructor does not get called 
 * before rounds; newRound() gets called before every round.
 */
myPlayer::myPlayer( int boardSize )
    :PlayerV2(boardSize)
{
    // Could do any initialization of inter-round data structures here.
    for(int row=0; row<MAX_BOARD_SIZE; row++) {
        for(int col=0; col<MAX_BOARD_SIZE; col++) {
            this->shotMap[row][col] = 0;
        }
    }
    this->shotGridSeed2 = rand() % 2;
    this->shotGridSeed = rand() % 3;
}

/**
 * @brief Destructor placeholder.
 * If your code does anything that requires cleanup when the object is
 * destroyed, do it here in the destructor.
 */
myPlayer::~myPlayer( ) {
}

/*
 * Private internal function that initializes a MAX_BOARD_SIZE 2D array of char to water.
 */
void myPlayer::initializeBoard() {
    for(int row=0; row<MAX_BOARD_SIZE; row++) {
        for(int col=0; col<MAX_BOARD_SIZE; col++) {
            this->board[row][col] = WATER;
            myBoard[row][col] = WATER;
        }
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
Message myPlayer::getMove() {
    for(int row = 0; row < boardSize; row++){
        for(int col = 0; col < boardSize; col++){
            if(board[row][col] == HIT){
                if(row+1 < boardSize && board[row+1][col] == HIT){
                    if(row+2 < boardSize && board[row+2][col] == WATER){
                        Message result( SHOT, row+2, col, "Bang", None, 1 );
                        return result;
                    }
                    else if (row-1 >=0 && board[row-1][col] == WATER){
                        Message result( SHOT, row-1, col, "Bang", None, 1 );
                        return result;
                    }
                }
                else if(col+1 < boardSize && board[row][col+1] == HIT){
                    if(col+2 < boardSize && board[row][col+2] == WATER){
                        Message result( SHOT, row, col+2, "Bang", None, 1 );
                        return result;
                    }
                    else if (col-1 >=0 && board[row][col-1] == WATER){
                        Message result( SHOT, row, col-1, "Bang", None, 1 );
                        return result;
                    }
                }
                else {
                    int neighbors[4][2] = {{row+1, col}, {row-1, col}, {row, col+1}, {row, col-1}};
                    for(int i = 0; i<4; i++){
                        if(neighbors[i][0] < boardSize && neighbors[i][1] < boardSize && neighbors[i][0] >=0 && neighbors[i][1] >=0){
                            if(board[neighbors[i][0]][neighbors[i][1]] == WATER){
                                Message result( SHOT, neighbors[i][0], neighbors[i][1], "Bang", None, 1 );
                                return result;
                            }
                        }
                    }
                }
            }
        }
    }
    int shotRow = rand() % boardSize;
    int shotCol = rand() % boardSize;
    
    while(board[shotRow][shotCol] != WATER || (shotGridSeed2 == 0 ? shotRow+shotCol: shotRow-shotCol+boardSize*3) % 3 != shotGridSeed){
        shotRow = rand() % boardSize;
        shotCol = rand() % boardSize;
    }
    Message result( SHOT, shotRow, shotCol, "Bang", None, 1 );
    return result;
}

/**
 * @brief Tells the AI that a new round is beginning.
 * The AI show reinitialize any intra-round data structures.
 */
void myPlayer::newRound() {
    /* DumbPlayer is too simple to do any inter-round learning. Smarter players 
     * reinitialize any round-specific data structures here.
     */
    this->shotGridSeed2 = rand() % 2;
    this->shotGridSeed = rand() % 3;
    this->initializeBoard();
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
Message myPlayer::placeShip(int length) {
    int spotCount = 0;
    int spots[MAX_BOARD_SIZE*MAX_BOARD_SIZE*2][3];
    int minValue = -1;
    int total = 0;

    for(int x=0; x<boardSize; x++){
        for (int y=0; y<boardSize; y++){
            for (int dir = 2; dir != 0; dir-=1){
                for (int d=0; d<length; d++){
                    if (total!=-1 && (dir==1? (x+d<boardSize && myBoard[y][x+d] != SHIP) : (y+d<boardSize && myBoard[y+d][x] != SHIP))){
                        total+= (dir==1 ? shotMap[y][x+d] : shotMap[y+d][x]);
                    } else {
                    total = -1;
                    }
                }
                if (total!=-1 && (total < minValue || minValue == -1)){
                    minValue = total;
                    spotCount = 0;
                }
                if (total == minValue){
                    spots[spotCount][0] = x;
                    spots[spotCount][1] = y;
                    spots[spotCount][2] = dir;
                    spotCount += 1;
                }
                total = 0;
            }
        }
    }
    
    int seed = rand() % spotCount;
    for (int d = 0; d<length; d++){
        if (spots[seed][2] == 1) { //Horizontal
            myBoard[spots[seed][1]][spots[seed][0]+d] = SHIP;
        } else { //Veritical
            myBoard[spots[seed][1]+d][spots[seed][0]] = SHIP;
        }
    }
    return Message(PLACE_SHIP, spots[seed][1], spots[seed][0], "",  spots[seed][2]==1 ? Horizontal:Vertical, length); 
}


/**
 * @brief Updates the AI with the results of its shots and where the opponent is shooting.
 * @param msg Message specifying what happened + row/col as appropriate.
 */
void myPlayer::update(Message msg) {
    switch(msg.getMessageType()) {
	case HIT:
	case KILL:
	case MISS:
	    board[msg.getRow()][msg.getCol()] = msg.getMessageType();
	    break;
	case WIN:
	    break;
	case LOSE:
	    break;
	case TIE:
	    break;
	case OPPONENT_SHOT:
        if (msg.getRow()<boardSize && msg.getCol() < boardSize && msg.getRow() >=0 && msg.getCol()>=0){
            shotMap[msg.getRow()][msg.getCol()]+=1;
        }
	    break;
    }
}

