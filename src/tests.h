#ifndef TESTS_H_
#define TESTS_H_

#include <iostream>
#include "battleship.h"

bool tests();

bool testWelcomeScreen();

bool testInitGameBoard();

bool testPutShipOnBoard(int, int);

bool testAutomaticBoardPlace();

bool testHit(int, int);

bool testSunkShip(int, int);

#endif
