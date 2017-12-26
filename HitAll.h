/**
 * @author Stefan Brandle, Jonathan Geisler, Scotti Bozarth, Jordan Rishardson
 * @date Oct, 2016
 *
 * 
 *
 */

/** 
 *@brief Strategic Planning Initilizations
 * initilize random number generator at some point outside of individual rounds and keep pulling from it. 
 */

#ifndef HITALL_H		// Double inclusion protection
#define HITALL_H

using namespace std;

#include "PlayerV2.h"
#include "Message.h"
#include "defines.h"

// DumbPlayer inherits from/extends PlayerV2

class HitAll: public PlayerV2 {
    public:
	HitAll( int boardSize );
	~HitAll();
	void newRound();
	Message placeShip(int length);
	Message getMove();
	void update(Message msg);

    private:
	void initializeBoard();
        void HitDefaultLU(int locRow, int locCol);
        void HitDefaultRU(int locRow, int locCol);
        void HitDefaultLD(int locRow, int locCol);
        void HitDefaultRD(int locRow, int locCol);
        void HitRight(int locRow, int locCol);
        void HitLeft(int locRow, int locCol);
        void HitUp(int locRow, int locCol);
        void HitDown(int locRow, int locCol);
        int fireRow;
        int fireCol;
        int solTemp;
        int solMax;
        int solMin;
	int numShipsPlaced;
        int isTrue;
        bool haveShot;
        Direction placeDirection;
        int numShots;
        char board[MAX_BOARD_SIZE][MAX_BOARD_SIZE];
        int HitHeatmap[MAX_BOARD_SIZE][MAX_BOARD_SIZE];
        int ShipHeatmap[MAX_BOARD_SIZE][MAX_BOARD_SIZE];
        char opponentBoard[MAX_BOARD_SIZE][MAX_BOARD_SIZE];
        bool myBoard[MAX_BOARD_SIZE][MAX_BOARD_SIZE];
        bool firingSolution[MAX_BOARD_SIZE][MAX_BOARD_SIZE];
};

#endif
