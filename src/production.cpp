#include "production.h"

void production(int argc, char** argv)
{

	system ("cls");
	remove(LOG_FILE_NAME);
	Stats players[2] = {{0, 0, 0, 0.0}, {0, 0, 0, 0.0}};

	Cell playerOneGameBoard[ROWS][COLS];                                                                                                                                                                                                                                                                       /* Player one game board */
	Cell playerTwoGameBoard[ROWS][COLS];                                                                                                                                                                                                                                                                       /* Player two game board */

	Coordinate target;                                                                                                                                                                                                                                                                             /* x, y value of a target */
	Coordinate targetTemp;                                                                                                                                                                                                                                                                         /* x, y value that holds a temporary value*/
	Coordinate targetOrigin;                                                                                                                                                                                                                                                                       /* x, y value of the original target */
	Coordinate targetAI;                                                                                                                                                                                                                                                                           /* x, y value of the targets using AI technique */

	WaterCraft ship[NUM_OF_SHIPS] = {{'c', 5, (char*) "Carrier"},
		{'b', 4, (char*) "Battleship"},
		{'r', 3, (char*) "Cruiser"},
		{'s', 3, (char*) "Submarine"},
		{'d', 2, (char*) "Destroyer"}};

	Boolean huntMode       = TRUE;                                                                                                                                                                                                                                                                                        /* mode of randomly selecting a target */
	Boolean targetMode     = FALSE;                                                                                                                                                                                                                                                                                       /* mode when there is a hit */
	Boolean flipper        = TRUE;                                                                                                                                                                                                                                                                                            /* flips values of boolean */
	Boolean cardinals[4]   = {TRUE, TRUE, TRUE, TRUE};                                                                                                                                                                                                                                                                    /* represents the 4 cardinals, N, S, W, E */
	Boolean hasAShipSunked = FALSE;                                                                                                                                                                                                                                                                                       /* if a ship has sank */


	short sunkShip[2][NUM_OF_SHIPS] = {{5, 4, 3, 3, 2},
		{5, 4, 3, 3, 2}};                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             /* tracks parts of the ship destroyed */

	short player  = 0;                                                                                                                                                                                                                                                                                 /* 0 -> player1, 1 -> player2 */
	short shot    = 0;                                                                                                                                                                                                                                                                             /* holds temp value if ship has been shot */                                                                                                                 /* option for player to place ship manually or randomly */
	int north   = 0,                                                                                                                                                                                                                                                                               /* holds change of value when going north */
	    south   = 0,                                                                                                                                                                                                                                                                                   /* holds change of value when going south */
	    east    = 0,                                                                                                                                                                                                                                                                                   /* holds change of value when going east */
	    west    = 0;                                                                                                                                                                                                                                                                                   /* holds change of value when going west */
	int i       = 0,
	    counter = 1;                                                                                                                                                                                                                                                                                   /* i and counter are used as counters */

	char shipSymbol = '\0';                                                                                                                                                                                                                                                                        /* temporary value to save character symbol of the ship */

	FILE *outStream = NULL;                                                                                                                                                                                                                                                                        /* stream to output file battleship.log */

	/* Start of Program */
	outStream = fopen (LOG_FILE_NAME, "w");

	srand ((unsigned int) time (NULL));

	/**
	 * Print welcome screen
	 */
	welcomeScreen ();
	systemMessage ((char*) "                            Hit <ENTER> to continue!\n");
	system ("cls");


	initializeGameBoard (playerOneGameBoard);
	initializeGameBoard (playerTwoGameBoard);



	randomlyPlaceShipsOnGameBoard (playerOneGameBoard, ship);
	printf ("> Player 1 (Human's) board has been generated.\n");

	randomlyPlaceShipsOnGameBoard (playerTwoGameBoard, ship);
	printf ("> Player 2 (Computer's) board has been generated.\n");




	player = getRandomNumber (0, 1);
	printf ("> Player %d has been randomly selected to go first.\n", player + 1);
	systemMessage ((char*) "> Hit <ENTER> to continue!\n");
	system ("cls");


	while (TRUE) {

		/* Write to battleship.log */
		fprintf (outStream, "Player %d's turn.\n", player + 1);

		/* switches back and forth between player 1 and player 2 */
		switch (player) {

		case PLAYER_ONE:
			/* Print player 2's game board*/
			printf ("> Player 2's Board:\n");
			printGameBoard (playerTwoGameBoard, FALSE, outStream);
			printf ("> PLAYER 1'S TURN\n");

			/* Prompts user to enter target */
			do {
				target = getTarget ();
				shot = checkShot (playerTwoGameBoard, target);

				/* prompts user that input is invalid or either a hit or miss cell */
				if (shot == -1)
					printf ("> Try inputting another target!\n");

			} while (shot == -1);

			/* saves the character of the targeted cell */
			shipSymbol = playerTwoGameBoard[target.row][target.column].symbol;
			break;

		case PLAYER_TWO:

			/**** IMPLEMENTATION OF ARTIFICIAL INTELLIGENCE ****/

			/* Print player 1's game board */
			printf ("> Player 1's Board:\n");
			printGameBoard (playerOneGameBoard, TRUE, outStream);
			printf ("> COMPUTER'S TURN\n");

			/**
			 * check from a previous cycle if a ship has sunk,
			 * if yes, it reinitializes values to go back to hunt mode
			 */
			if (hasAShipSunked) {
				hasAShipSunked = FALSE;
				targetMode = FALSE;
				huntMode = TRUE;
			}

			/**
			 * TARGET MODE
			 * This mode is true when the computer randomly selects a target,
			 * and it happens to be a hit, this mode is set to true.
			 */
			if (targetMode) {
				/* retains value of targetAI */
				target = targetAI;

				do {
					if (cardinals[NORTH]) {                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                /* NORTH */
						target.row = north;
					} else if (cardinals[SOUTH]) {                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         /* SOUTH */
						target.row = south;
					} else if (cardinals[WEST]) {                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          /* WEST */
						target.column = west;
					} else if (cardinals[EAST]) {                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          /* EAST */
						target.column = east;
					} else if (!cardinals[NORTH] && !cardinals[SOUTH] &&
					           !cardinals[WEST]  && !cardinals[EAST]  &&
					           !hasAShipSunked) {
						/* All cardinals are FALSE but has not sunk a ship */

						/* reinitiliazes target to the original target value */
						target = targetOrigin;
						targetTemp = target;

						/**
						 * Counter increments by one, when the loop cycles again and
						 * all cardinals are still FALSE. This ensures that we are checking
						 * one cell over the adjacent cells
						 */
						north = target.row - counter;
						targetTemp.row = north;

						/* checks if the value NORTH of the target is a hit or miss */
						if (checkShot (playerOneGameBoard, targetTemp) != -1 && north >= 0) {
							cardinals[NORTH] = TRUE;
						}

						targetTemp = target;
						south = target.row + counter;
						targetTemp.row = south;

						/* checks if the value SOUTH of the target is a hit or miss */
						if (checkShot (playerOneGameBoard, targetTemp) != -1 && south <= 9) {
							cardinals[SOUTH] = TRUE;
						}

						targetTemp = target;
						west = target.column - counter;
						targetTemp.column = west;

						/* checks if the value WEST of the target is a hit or miss */
						if (checkShot (playerOneGameBoard, targetTemp) != -1 && west >= 0) {
							cardinals[WEST] = TRUE;
						}

						targetTemp = target;
						east = target.column + counter;
						targetTemp.column = east;

						/* checks if the value EAST of the target is a hit or miss */
						if (checkShot (playerOneGameBoard, targetTemp) != -1 && east <= 9) {
							cardinals[EAST] = TRUE;
						}

						/**
						 * increments counter every cycle, serves as a addend to how
						 * many cells to check from the target cell
						 */
						counter++;

					} else  {
						/* when all conditions are not met, it reverts back to Hunt mode */
						targetMode = FALSE;
						huntMode = TRUE;
						break;
					}

					/* checks if the shot is a hit or miss */
					shot = checkShot (playerOneGameBoard, target);

				} while (shot == -1 && targetMode == TRUE);

				/**
				 * this loop flips the values of the cardinals when it is not needed
				 * takes the cardinal off the stack for directions to check
				 */
				if (shot == 1 && huntMode == FALSE) {
					for (i = 0; i < 4; i++) {
						if (flipper == FALSE)
							cardinals[i] = FALSE;

						if (cardinals[i] == flipper)
							flipper = FALSE;
					}
				} else {
					for (i = 0; i < 4; i++) {
						if (flipper == TRUE && cardinals[i] != FALSE) {
							cardinals[i] = FALSE;
							break;
						}
					}
				}

				/* reinitializes the value of flipper */
				flipper = TRUE;
			}

			/**
			 * HUNT MODE
			 * This is starting mode of Player 2's turn. This pertains to the
			 * computer randomly selecting cells as targets and checking if they are
			 * just plain water. When a cell equates to a Hit, then hunt mode is
			 * set to false and target mode is set to true
			 */
			if (huntMode) {

				/* reinitializes important values */
				counter = 1;
				flipper = TRUE;
				for (i = 0; i < 4; i++)
					cardinals[i] = TRUE;

				/* loop that gets a random number of a cell as target */
				do {
					target.row = getRandomNumber (0, 9);
					target.column = getRandomNumber (0, 9);

					/* checks target if it is a miss or hit cell */
					shot = checkShot (playerOneGameBoard, target);
				} while (shot == -1);

				/* if it is a Hit, this saves the value of the original target */
				if (shot == 1) targetOrigin = target;
			}

			/**
			 * shot values (1 means craft is hit, 0 means missed and -1 means
			 * it is already a hit or miss cell
			 */
			if (shot == 1) {

				/**
				 * if all cardinals are false and no ship was sunk,
				 * reintializes target from value of original target
				 */
				if (!cardinals[NORTH] && !cardinals[SOUTH] &&
				    !cardinals[WEST]  && !cardinals[EAST]  &&
				    !hasAShipSunked) { target = targetOrigin; }

				/* hunt mode is false during a target mode */
				huntMode = FALSE;
				targetMode = TRUE;
				targetAI = target;

				/**
				 * get values of adjacent cells and ensuring that
				 * that it is withing the bounds of gameboard for
				 * NORTH, SOUTH, WEST & EAST
				 */
				if (cardinals[NORTH] == TRUE) {                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          /* NORTH */
					north = (targetAI.row - 1);
					checkBoundsOfCardinal (cardinals, north, NORTH);
					targetTemp = target;
					targetTemp.row = north;
					if (checkShot (playerOneGameBoard, targetTemp) == -1)
						cardinals[NORTH] = FALSE;
				}

				if (cardinals[SOUTH] == TRUE) {                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          /* SOUTH */
					south = targetAI.row + 1;
					checkBoundsOfCardinal (cardinals, south, SOUTH);
					targetTemp = target;
					targetTemp.row = south;
					if (checkShot (playerOneGameBoard, targetTemp) == -1)
						cardinals[SOUTH] = FALSE;
				}

				if (cardinals[WEST] == TRUE) {                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           /* WEST */
					west  = targetAI.column - 1;
					checkBoundsOfCardinal (cardinals, west, WEST);
					targetTemp = target;
					targetTemp.column = west;
					if (checkShot (playerOneGameBoard, targetTemp) == -1)
						cardinals[WEST] = FALSE;
				}

				if (cardinals[EAST] == TRUE) {                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           /* EAST */
					east  = targetAI.column + 1;
					checkBoundsOfCardinal (cardinals, east, EAST);
					targetTemp = target;
					targetTemp.column = east;
					if (checkShot (playerOneGameBoard, targetTemp) == -1)
						cardinals[EAST] = FALSE;
				}
			}

			/* saves the character of the targeted cell */
			shipSymbol = playerOneGameBoard[target.row][target.column].symbol;
			break;
		}

		/**
		 * Prompts player if it's a hit or miss
		 */
		if (shot == 1) {                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 /* HIT */
			printf ("> %d, %d is a hit!\n", target.row, target.column);

			/* Write to battleship.log */
			fprintf (outStream, "%d, %d is a hit!\n", target.row, target.column);

			/* Keeps track so number of hits for stats */
			players[player].numHits++;

			/* Checks if the ship has sank */
			if (player == 1)
				hasAShipSunked = checkSunkShip (sunkShip, !player, shipSymbol, outStream);
			else
				checkSunkShip (sunkShip, !player, shipSymbol, outStream);

		} else {                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        /* MISS */
			printf ("> %d, %d is a miss!\n", target.row, target.column);

			/* Write to battleship.log */
			fprintf (outStream, "%d, %d is a miss!\n", target.row, target.column);
			players[player].numMisses++;
		}

		if (player == 0)
			/* If Player 1 then update player 2's gameboard */
			updateGameBoard (playerTwoGameBoard, target);
		else                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             /* If Player 2 then update player 1's gameboard */
			updateGameBoard (playerOneGameBoard, target);

		/* Determins if there is a winner based on number of hits */
		if (isWinner (players, player)) {
			printf ("\n> Player %d wins!\n", player + 1);

			/* Write to battleship.log */
			fprintf (outStream, "\n>>>>> Player %d wins! <<<<<\n", player + 1);
			break;
		}

		systemMessage ((char*) "> Hit <ENTER> to continue!\n");

		/* switches from player 1 to player 2 */
		player = !player;

		system ("cls");
	}

	/**
	 * At the end of the game, Player1's and Player2's statistics should be
	 * written to "battleship.log". The stats include total number of hits,
	 * total number of misses, total number of shots, hits to misses ratio
	 * (as a percentage), and won or lost the game. Note that the statistics
	 * should be placed into a structure called Stats. You need two variables
	 * of type Stats, one for Player1 and one for Player2. Once the game has
	 * ended you should write the contents of each struct variable to the
	 * "battleship.log" file.
	 */
	players[0].totalShots = players[0].numHits + players[0].numMisses;
	players[0].hitMissRatio = ((double) players[0].numHits/(double) players[0].numMisses) * 100;
	players[1].totalShots = players[1].numHits + players[1].numMisses;
	players[1].hitMissRatio = ((double) players[1].numHits/(double) players[1].numMisses) * 100;
	fprintf (outStream, "+===================================================\n");
	fprintf (outStream, "|                    PLAYER STATS                   \n");
	fprintf (outStream, "+---------------------------------------------------\n");
	fprintf (outStream, "| PLAYER 1 : %d hits                                \n", players[0].numHits);
	fprintf (outStream, "|            %d misses                              \n", players[0].numMisses);
	fprintf (outStream, "|            %d total shots                         \n", players[0].totalShots);
	fprintf (outStream, "|            %.2lf%% hit/miss ratio                 \n", players[0].hitMissRatio);
	fprintf (outStream, "| PLAYER 2 : %d hits                                \n", players[1].numHits);
	fprintf (outStream, "|            %d misses                              \n", players[1].numMisses);
	fprintf (outStream, "|            %d total shots                         \n", players[1].totalShots);
	fprintf (outStream, "|            %.2lf%% hit/miss ratio                 \n", players[1].hitMissRatio);
	fprintf (outStream, "+===================================================");

	fclose (outStream);
}
