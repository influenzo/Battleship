#include "tests.h"

using namespace std;

bool tests()
{
	bool ok = testWelcomeScreen() && testInitGameBoard() && testPutShipOnBoard(3,4) && testAutomaticBoardPlace() && testHit(2,3) && testSunkShip(4,5);
	return ok;
}

bool testWelcomeScreen() {
	bool ok = false;
	welcomeScreen();
	cout << "Did you see the welcome screen? (y/n)" << endl;
	char c = getchar();
	getchar();
	if(c == 'y') {
		ok = true;
	}
	else {
		ok = false;
	}
	return ok;
}

bool testInitGameBoard() {
	bool ok = false;
	Cell playerOneGameBoard[ROWS][COLS];
	initializeGameBoard (playerOneGameBoard);
	printGameBoard(playerOneGameBoard, TRUE, NULL);

	cout << "Is this the initialized game board? (y/n)" << endl;
	char c = getchar();
	getchar();
	if(c == 'y') {
		ok = true;
	}
	else {
		ok = false;
	}
	return ok;
}

bool testPutShipOnBoard(int row, int col) {
	bool ok = false;
	Cell gameBoard[ROWS][COLS];
	initializeGameBoard(gameBoard);

	WaterCraft ship = {'c', 5, (char*) "Carrier"};

	Coordinate position = {row, col};

	putShipOnGameBoard (gameBoard, ship, position, 1);
	printGameBoard(gameBoard, TRUE, NULL);

	cout << "Was a ship placed on the board? (y/n)"<< endl;
	char c = getchar();
	getchar();
	if(c == 'y') {
		ok = true;
	}
	else {
		ok = false;
	}
	return ok;
}

bool testAutomaticBoardPlace() {
	bool ok = false;
	WaterCraft ship[NUM_OF_SHIPS] = {{'c', 5, (char*) "Carrier"},
		{'b', 4, (char*) "Battleship"},
		{'r', 3, (char*) "Cruiser"},
		{'s', 3, (char*) "Submarine"},
		{'d', 2, (char*) "Destroyer"}};

	Cell playerOneGameBoard[ROWS][COLS];

	initializeGameBoard(playerOneGameBoard);
	randomlyPlaceShipsOnGameBoard(playerOneGameBoard, ship);
	printGameBoard(playerOneGameBoard, TRUE, NULL);

	cout << "Was a board randomly generated? (y/n)" << endl;
	char c = getchar();
	getchar();
	if(c == 'y') {
		ok = true;
	}
	else {
		ok = false;
	}
	return ok;
}

bool testHit(int row, int col) {
	bool ok = false;
	Cell gameBoard[ROWS][COLS];
	initializeGameBoard(gameBoard);

	WaterCraft ship = {'c', 5, (char*) "Carrier"};

	Coordinate position = {row, col};

	putShipOnGameBoard (gameBoard, ship, position, 1);
	updateGameBoard(gameBoard, position);
	printGameBoard(gameBoard, TRUE, NULL);

	cout << "Was the ship hit by an attack? (y/n) \n" << endl;
	char c = getchar();
	getchar();
	if(c == 'y') {
		ok = true;
	}
	else {
		ok = false;
	}
	return ok;
}

bool testSunkShip(int row, int col) {
	bool ok = false;
	Cell gameBoard[ROWS][COLS];

	short sunkShip[2][NUM_OF_SHIPS] = {{5, 4, 3, 3, 2}, {5, 4, 3, 3, 2}};

	initializeGameBoard(gameBoard);

	WaterCraft ship = {'s', 3, (char*) "Submarine"};

	Coordinate position = {row, col};

	FILE *outStream = NULL;

	outStream = fopen (LOG_FILE_NAME, "w");

	putShipOnGameBoard (gameBoard, ship, position, 1);

	for(int i = 0; i < 3; i++) {
		updateGameBoard(gameBoard, position);
		checkSunkShip(sunkShip, 0, 's', outStream);
		position.row += 1;
	}

	printGameBoard(gameBoard, TRUE, outStream);

	fclose(outStream);

	cout << "Was the ship sunk and printed to a file? (y/n) \n" << endl;
	char c = getchar();
	getchar();
	if(c == 'y') {
		ok = true;
	}
	else {
		ok = false;
	}
	return ok;
}
