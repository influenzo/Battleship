
#include "battleship.h"

void welcomeScreen (void) {
	printf ("XXXXX   XXXX  XXXXXX XXXXXX XX     XXXXXX  XXXXX XX  XX XX XXXX\n");
	printf ("XX  XX XX  XX   XX     XX   XX     XX     XX     XX  XX XX XX  XX\n");
	printf ("XXXXX  XX  XX   XX     XX   XX     XXXX    XXXX  XXXXXX XX XXXX\n");
	printf ("XX  XX XXXXXX   XX     XX   XX     XX         XX XX  XX XX XX\n");
	printf ("XXXXX  XX  XX   XX     XX   XXXXXX XXXXXX XXXXX  XX  XX XX XX\n");
	printf ("\n\n");
	printf ("RULES OF THE GAME:\n");
	printf ("1. This is a two player game.\n");
	printf ("2. Player 1 is you and Player 2 is the computer.\n");
	printf ("3. Player 1 will be prompted if user wants to manually input coordinates\n");
	printf ("   for the game board or have the computer randomly generate a game board\n");
	printf ("4. There are five types of ships to be placed by longest length to the\n");
	printf ("   shortest; [c] Carrier has 5 cells, [b] Battleship has 4 cells, [r] Cruiser\n");
	printf ("   has 3 cells, [s] Submarine has 3 cells, [d] Destroyer has 2 cells\n");
	printf ("5. The computer randomly selects which player goes first\n");
	printf ("6. The game begins as each player tries to guess the location of the ships\n");
	printf ("   of the opposing player's game board; [*] hit and [m] miss\n");
	printf ("7. First player to guess the location of all ships wins\n\n");
}

/**
 * Function name : initializeGameBoard ()
 * Date Created  : 17 October 2012
 * Usage         : initializeGameBoard ();
 * Definition    : This function displays initializes values of the multi-
 *                 dimensional array called gameBoard.
 * Pre-condition : ROWS & COLS are defined as constant macros
 * Post-condition: Array initialize to (~) tilde representing water
 */
void initializeGameBoard (Cell gameBoard[][COLS]) {
	srand(time(0));
	int i = 0, j = 0;

	for (i = 0; i < ROWS; i++)
		for (j = 0; j < COLS; j++) {
			gameBoard[i][j].symbol          = WATER;
			gameBoard[i][j].position.row    = i;
			gameBoard[i][j].position.column = j;
		}
}


void printGameBoard (Cell gameBoard [][COLS], Boolean showPegs, FILE *oFile) {
	int i = 0, j = 0;

	if(showPegs) {
		fprintf(oFile, ">> PLAYER 1's BOARD << \n");
	}
	else {
		fprintf(oFile, ">> PLAYER 2's BOARD << \n");
	}

	printf ("  0 1 2 3 4 5 6 7 8 9\n");
	fprintf (oFile, "  0 1 2 3 4 5 6 7 8 9\n");

	for (i = 0; i < ROWS; i++) {
		printf ("%d ", i);
		fprintf (oFile, "%d ", i);

		for (j = 0; j < COLS; j++) {
			fprintf (oFile, "%c ", gameBoard [i][j].symbol);
			if (showPegs == TRUE) {
				printf ("%c ", gameBoard [i][j].symbol);
			}
			else {
				switch (gameBoard [i][j].symbol) {
				case HIT:
					printf ("%c ", HIT);
					break;
				case MISS:
					printf ("%c ", MISS);
					break;
				case WATER:
				default:
					printf ("%c ", WATER);
					break;
				}
			}
		}
		fprintf(oFile, "%c", '\n');
		putchar ('\n');
	}

	fflush(oFile);
}


void putShipOnGameBoard (Cell gameBoard[][COLS], WaterCraft ship,
                         Coordinate position, int direction) {
	int i = ship.length - 1;

	for (i = 0; i < ship.length; i++) {
		if (direction == HORIZONTAL)
			gameBoard [position.row][position.column + i].symbol = ship.symbol;
		else                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         /* VERTICAL */
			gameBoard [position.row + i][position.column].symbol = ship.symbol;
	}
}


void randomlyPlaceShipsOnGameBoard (Cell gameBoard[][COLS], WaterCraft ship[]) {
	Coordinate position;
	int direction = -1;
	int i = 0;

	for (i = 0; i < NUM_OF_SHIPS; i++) {
		while (TRUE) {
			direction = getRandomNumber (0, 1);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     /* 0 -> horizontal, 1 -> vertical */
			position = generatePosition (direction, ship[i].length);

			if (isValidLocation (gameBoard, position, direction, ship[i].length)) break;
		}

		putShipOnGameBoard (gameBoard, ship[i], position, direction);
	}
}


void updateGameBoard (Cell gameBoard[][COLS], Coordinate target) {
	switch (gameBoard[target.row][target.column].symbol) {
	/* miss */
	case WATER:
		gameBoard[target.row][target.column].symbol = MISS;
		break;

	/* hit */
	case CARRIER:
	case BATTLESHIP:
	case CRUISER:
	case SUBMARINE:
	case DESTROYER:
		gameBoard[target.row][target.column].symbol = HIT;
		break;

	case HIT:
	case MISS:
	default:
		break;
	}
}


void checkBoundsOfCardinal (Boolean cardinals[], int bound, int direction) {
	switch (direction) {
	case NORTH:
		if (bound < 0)
			cardinals[0] = FALSE;
		else
			cardinals[0] = TRUE;
		break;

	case SOUTH:
		if (bound > 9)
			cardinals[1] = FALSE;
		else
			cardinals[1] = TRUE;
		break;

	case WEST:
		if (bound < 0)
			cardinals[2] = FALSE;
		else
			cardinals[2] = TRUE;
		break;

	case EAST:
		if (bound > 9)
			cardinals[3] = FALSE;
		else
			cardinals[3] = TRUE;
		break;
	}
}


void systemMessage (char *message) {
	char ch = '\0';

	do {
		printf ("%s", message);
	} while ((ch = getch()) != '\r');
}

Boolean checkSunkShip (short sunkShip[][NUM_OF_SHIPS], short player, char shipSymbol, FILE *stream) {
	Boolean sunked = FALSE;

	switch (shipSymbol) {
	case CARRIER:
		if (--sunkShip[player][0] == 0) {
			printf ("> Player %d's Carrier sunked!\n", player + 1);

			/* Write to battleship.log */
			fprintf (stream, "Player %d's Carrier sunked!\n", player + 1);

			sunked = TRUE;
		}
		break;

	case BATTLESHIP:
		if (--sunkShip[player][1] == 0) {
			printf ("> Player %d's Battleship sunked!\n", player + 1);

			/* Write to battleship.log */
			fprintf (stream, "Player %d's Battleship sunked!\n", player + 1);

			sunked = TRUE;
		}
		break;

	case CRUISER:
		if (--sunkShip[player][2] == 0) {
			printf ("> Player %d's Cruiser sunked!\n", player + 1);

			/* Write to battleship.log */
			fprintf (stream, "Player %d's Cruiser sunked!\n", player + 1);

			sunked = TRUE;
		}
		break;

	case SUBMARINE:
		if (--sunkShip[player][3] == 0) {
			printf ("> Player %d's Submarine sunked!\n", player + 1);

			/* Write to battleship.log */
			fprintf (stream, "Player %d's Submarine sunked!\n", player + 1);

			sunked = TRUE;
		}
		break;

	case DESTROYER:
		if (--sunkShip[player][4] == 0) {
			printf ("> Player %d's Destroyer sunked!\n", player + 1);

			/* Write to battleship.log */
			fprintf (stream, "Player %d's Destroyer sunked!\n", player + 1);

			sunked = TRUE;
		}
		break;
	}

	return sunked;
}


Boolean isValidLocation (Cell gameBoard[][COLS], Coordinate position,
                         int direction, int length) {
	int i = length - 1;
	Boolean isValid = TRUE;

	for (i = 0; isValid && i < length; i++) {
		if (direction == HORIZONTAL) {
			if (gameBoard [position.row][position.column + i].symbol != WATER &&
			    (position.column + i) < COLS)
				isValid = FALSE;
		} else {                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         /* VERTICAL */
			if (gameBoard [position.row + i][position.column].symbol != WATER &&
			    (position.row + i) < ROWS)
				isValid = FALSE;
		}
	}

	return isValid;
}


Boolean isWinner (Stats players[], int player) {
	Boolean isWin = FALSE;

	if (players[player].numHits == 17)
		isWin = TRUE;

	return isWin;
}


short checkShot (Cell gameBoard[][COLS], Coordinate target) {
	int hit = -2;

	switch (gameBoard[target.row][target.column].symbol) {
	/* miss */
	case WATER:
		hit = 0;
		break;

	/* hit */
	case CARRIER:
	case BATTLESHIP:
	case CRUISER:
	case SUBMARINE:
	case DESTROYER:
		hit = 1;
		break;

	case HIT:
	case MISS:
	default:
		hit = -1;
		break;
	}

	return hit;
}
