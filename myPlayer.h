/**
 * @author Stefan Brandle, Jonathan Geisler
 * @date September, 2004
 *
 * Please type in your name[s] here:
 *
 */

#ifndef MYPLAYER_H		// Double inclusion protection
#define MYPLAYER_H

using namespace std;

#include "PlayerV2.h"
#include "Message.h"
#include "defines.h"

// DumbPlayer inherits from/extends PlayerV2

class myPlayer: public PlayerV2 {
    public:
	myPlayer( int boardSize );
	~myPlayer();
	void newRound();
	Message placeShip(int length);
	Message getMove();
	void update(Message msg);

    private:
	void initializeBoard();
    char board[MAX_BOARD_SIZE][MAX_BOARD_SIZE];
    int shotMap[MAX_BOARD_SIZE][MAX_BOARD_SIZE];
    char myBoard[MAX_BOARD_SIZE][MAX_BOARD_SIZE];
    int shotGridSeed;
    int shotGridSeed2;
};

#endif
