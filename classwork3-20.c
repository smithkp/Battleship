/*
 ============================================================================
 Name        : BattleClass.c
 Author      : tms
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

/*==============================================================================
 | Filename: main.c
 | Programmer: Gabriel V. de a Cruz Jr.
 | Class: CptS 121, Fall 2012 ; Lab Section 7
 | Programming Assignment 4: Basic Game of Battleship
 | Date: October 17, 2012
 +------------------------------------------------------------------------------
 | Description: This program simulates the game of Battleship. The game will be
 |              completely text-based. Battleship is a two player Navy game.
 |              The objective of the game is to sink all ships in your enemy's
 |              fleet. The Player to sink his/her enemy's fleet first wins. Both
 |              players' fleets consist of 5 ships that are hidden from the
 |              enemy. Each ship may be differentiated by its "size" (besides
 |              the Cruiser and Submarine) or number of cells it expands on the
 |              game board. The Carrier has 5 cells, Battleship has 4 cells,
 |              Cruiser has 3 cells, Submarine has 3 cells, and the Destroyer
 |              has 2 cells.
 ==============================================================================*/

#include "battleship.h"

int main (void)
{
	/**
	 * Two boards exist within the game. Hint: each board should be
	 * implemented as a 2-dimensional array. Each 2-dimensional array
	 * should be 10 X 10. One represents Player1's board and one represents
	 * Player2's board.
	 */
	Stats players[2] = {{0, 0, 0, 0.0}, {0, 0, 0, 0.0}};

	Cell playerOneGameBoard[ROWS][COLS];       /* Player one game board */
	Cell playerTwoGameBoard[ROWS][COLS];       /* Player two game board */


	Coordinate_t * target = (Coordinate_t *)malloc(sizeof(Coordinate_t));             /* x, y value of a target */
	Coordinate_t * targetTemp  = (Coordinate_t *)malloc(sizeof(Coordinate_t));             /* x, y value that holds a temporary value*/
	Coordinate_t * targetOrigin  = (Coordinate_t *)malloc(sizeof(Coordinate_t));            /* x, y value of the original target */
	Coordinate_t * targetAI  = (Coordinate_t *)malloc(sizeof(Coordinate_t));              /* x, y value of the targets using AI technique */

	WaterCraft ship[NUM_OF_SHIPS] = {{'c', 5, "Carrier"},
	                                 {'b', 4, "Battleship"},
	                                 {'r', 3, "Cruiser"},
	                                 {'s', 3, "Submarine"},
	                                 {'d', 2, "Destroyer"}};

	Boolean    huntMode       = TRUE;                     /* mode of randomly selecting a target */
	Boolean    targetMode     = FALSE;                    /* mode when there is a hit */
	Boolean    flipper        = TRUE;	                  /* flips values of boolean */
	Boolean    cardinals[4]   = {TRUE, TRUE, TRUE, TRUE}; /* represents the 4 cardinals, N, S, W, E */
	Boolean    hasAShipSunked = FALSE;                    /* if a ship has sank */


	short sunkShip[2][NUM_OF_SHIPS] = {{5, 4, 3, 3, 2},
	                                   {5, 4, 3, 3, 2}};  /* tracks parts of the ship destroyed */

	short player  = 0;	           /* 0 -> player1, 1 -> player2 */
	short shot    = 0;             /* holds temp value if ship has been shot */
	int   option  = 0;             /* option for player to place ship manually or randomly */
	int   north   = 0,             /* holds change of value when going north */
		  south   = 0,             /* holds change of value when going south */
		  east    = 0,             /* holds change of value when going east */
		  west    = 0;             /* holds change of value when going west */
	int   i       = 0,
		  counter = 1;             /* i and counter are used as counters */

	char  shipSymbol = '\0';       /* temporary value to save character symbol of the ship */

	FILE *outStream = NULL;        /* stream to output file battleship.log */

	/* Start of Program */
	outStream = fopen (LOG_FILE_NAME, "w");

	puts("opened log file"); fflush(stdout);

	//seed for random numbers srand ((unsigned int) time (NULL));

	/**
	 * Print welcome screen
	 */
	welcomeScreen ();
	puts("called welcome screen"); fflush(stdout);

	//systemMessage ("Hit <ENTER> to continue!\n");fflush(stdout);
	//system ("cls"); //this clears the screen in Visual C

	/**
	 * Each Player's game board should be initialized to all '~' indicating
	 * no ships have been placed on either board.
	 */

	initializeGameBoard (playerOneGameBoard);
	initializeGameBoard (playerTwoGameBoard);
	puts("initialized boards"); fflush(stdout);

	/**
	 * Both players' fleets consist of 5 ships that are hidden from the enemy.
	 * Each ship may be differentiated by its "size" (besides the Cruiser and
	 * Submarine) or number of cells it expands on the game board.
	 *      The Carrier has 5 cells,
	 *      Battleship has 4 cells,
	 *      Cruiser has 3 cells,
	 *      Submarine has 3 cells, and
	 *      the Destroyer has 2 cells.
	 */

	/**
	 * The program should be built such that the user is Player1 and the
	 * computer is Player2.
	 */

	/**
	 * Before the game starts, Player1 should have the option to either
	 * manually place each of the 5 ships in his/her fleet or to have them
	 * randomly placed on the board. If Player1 decides to place the ships
	 * manually, then he/she should be prompted to place the Carrier first,
	 * Battleship second, Cruiser third, Submarine fourth, and the Destroyer
	 * last.
	 */
	printf ("> Please select from the following menu:\n");
	printf ("> [1] Manually\n");
	printf ("> [2] Randomly\n");
	printf ("> Enter Option: ");fflush(stdout);
	scanf ("%d", &option);

	switch (option) {
		case 1: manuallyPlaceShipsOnGameBoard (playerOneGameBoard, ship);
	            break;
		case 2: randomlyPlaceShipsOnGameBoard (playerOneGameBoard, ship);
				break;
	}

	/**
	 * Note that ships cannot be placed diagonally on the board, they can
	 * only be placed vertically or horizontally. You program must check to
	 * see if the user tries to place a ship outside the boundaries of the
	 * board or on top of a ship that has already been placed. Each cell on
	 * the board that contains part of the ship must be indicated by 'c' for
	 * Carrier, 'b' for Battleship, 'r' for Cruiser, 's' for Submarine, or
	 * 'd' for Destroyer.
	 */

	/**
	 * Player2's ships must be randomly placed. Note that the placement of
	 * Player2's ships must be unknown. Thus, Player2's board will only
	 * display '~' in each cell after the placement of each ship.
	 */
	randomlyPlaceShipsOnGameBoard (playerTwoGameBoard, ship);
	printf ("> Player 2 (Computer's) board has been generated.\n");fflush(stdout);

	/**
	 * The program should randomly select Player1 or Player2 to go first.
	 */
	player = getRandomNumber (0, 1);
	printf ("> Player %d has been randomly selected to go first.\n", player + 1);fflush(stdout);
	//systemMessage ("> Hit <ENTER> to continue!\n");fflush(stdout);
	//system ("cls");

	/**
	 * Once it has been decided on which player goes first, the game starts.
	 * Whenever it's Player1's turn, a prompt should be displayed asking for
	 * a position to target (specifying where to "shoot") on the enemy's
	 * (Player2's) board (2-dimensional array). The position should be
	 * specified in terms of a row and a column on the board. The row and
	 * column should always be displayed along with the board. If the
	 * position specified happens to hit a ship, then a '*' should replace
	 * the '-' on Player2's board. If the positioned specified misses any
	 * one of the ships in the fleet, then a 'm' should replace the '-' on
	 * Player2's board. Note that from turn-to-turn each player should NOT
	 * be allowed to enter the same position. Also, between turns clear the
	 * screen (system("cls")). In one turn, a player can only take one shot
	 * at the enemy's (other player's) fleet. When Player2 takes a shot at
	 * Player1's board, each hit should be specified with a '*' and each
	 * miss with a 'm' on Player1's board. The game is over win Player1 or
	 * Player2 has sunk all of the ships in the fleet of the enemy.
	 */

	/**
	 * For each move made by Player1 and Player2, the results should be
	 * echoed to a file called "battleship.log". In this file, you should
	 * log the targeted position by each player on each move and whether it
	 * was a hit on one of the ships in the fleet. Also, if one of the ships
	 * happens to sink, then note this in the log file.
	 */
	puts("about to write to log");fflush(stdout);
	while (TRUE) {

		/* Write to battleship.log */
		fprintf (outStream, "Player %d's turn.\n", player + 1);fflush(outStream);
		printf ( "Player %d's turn.\n", player + 1);fflush(stdout);

		/* switches back and forth between player 1 and player 2 */
		switch (player) {

			case PLAYER_ONE:
				/* Print player 2's game board*/
				printf ("> Player 2's Board:\n");fflush(stdout);
				printGameBoard (playerTwoGameBoard, FALSE);
				printf ("> PLAYER 1'S TURN\n");fflush(stdout);//works

				/* Prompts user to enter target */
				do
				{
					target = getTarget ();
					puts("215");fflush(stdout);
					shot = checkShot (playerTwoGameBoard, target);

					/* prompts user that input is invalid or either a hit or miss cell */
					if (shot == -1)
						printf ("> Try inputting another target!\n");fflush(stdout);

				} while (shot == -1);

				/* saves the character of the targeted cell */
				shipSymbol = playerTwoGameBoard[target->row][target->column].symbol;
				printf ("> end PLAYER 1'S TURN\n");fflush(stdout);
				break;

			case PLAYER_TWO:

				/**** IMPLEMENTATION OF ARTIFICIAL INTELLIGENCE ****/

				/* Print player 1's game board */
				printf ("> Player 1's Board:\n");fflush(stdout);
				printGameBoard (playerOneGameBoard, TRUE);
				printf ("> COMPUTER'S TURN\n");fflush(stdout);

				/**
				 * check from a previous cycle if a ship has sunk,
				 * if yes, it reinitializes values to go back to hunt mode
				 */
				if (hasAShipSunked) {
					puts("a ship has sunk"); fflush(stdout);
					hasAShipSunked = FALSE;
					targetMode = FALSE;
					huntMode = TRUE;
				}

				/**
				 * TARGET MODE
				 * This mode is true when the computer randomly selects a target,
				 * and it happens to be a hit, this mode is set to true.
				 */
				if (targetMode)
				{
					/* retains value of targetAI */
					target = targetAI;

					do
					{
						if (cardinals[NORTH])
						{        /* NORTH */
							puts("263");
							cardinals[NORTH] = FALSE; //changed
							target->row = north;
						}
						else if (cardinals[SOUTH])
						{ /* SOUTH */
							puts("264");
							target->row = south;
						}
						else if (cardinals[WEST])
						{  /* WEST */
							puts("265");
							target->column = west;
						}
						else if (cardinals[EAST])
						{  /* EAST */
							puts("266");
							target->column = east;
						}

						else if (!cardinals[NORTH] && !cardinals[SOUTH] &&
						           !cardinals[WEST]  && !cardinals[EAST]  &&
								   !hasAShipSunked)
						{
							/* All cardinals are FALSE but has not sunk a ship */

							/* reinitiliazes target to the original target value */
							puts("285");
							target = targetOrigin;
							targetTemp = target;

							/**
							 * Counter increments by one, when the loop cycles again and
							 * all cardinals are still FALSE. This ensures that we are checking
							 * one cell over the adjacent cells
							 */
							north = target->row - counter;
							targetTemp->row = north;

							/* checks if the value NORTH of the target is a hit or miss */
							puts("297");fflush(stdout);
							if (checkShot (playerOneGameBoard, targetTemp) != -1 && north >= 0)
							{
								cardinals[NORTH] = TRUE;
							}

							targetTemp = target;
							south = target->row + counter;
							targetTemp->row = south;

							/* checks if the value SOUTH of the target is a hit or miss */
							puts("308");fflush(stdout);
							if (checkShot (playerOneGameBoard, targetTemp) != -1 && south <= 9) {
								cardinals[SOUTH] = TRUE;
							}

							targetTemp = target;
							west = target->column - counter;
							targetTemp->column = west;

							/* checks if the value WEST of the target is a hit or miss */
							puts("318");fflush(stdout);
							if (checkShot (playerOneGameBoard, targetTemp) != -1 && west >= 0) {
								cardinals[WEST] = TRUE;
							}

							targetTemp = target;
							east = target->column + counter;
							targetTemp->column = east;

							/* checks if the value EAST of the target is a hit or miss */
							puts("328");fflush(stdout);
							if (checkShot (playerOneGameBoard, targetTemp) != -1 && east <= 9) {
								cardinals[EAST] = TRUE;
							}

							/**
							 * increments counter every cycle, serves as a addend to how
							 * many cells to check from the target cell
							 */
							counter++;

						}

						else
						{
							/* when all conditions are not met, it reverts back to Hunt mode */
							targetMode = FALSE;
							huntMode = TRUE;
							puts("going to hunt mode");fflush(stdout);
							break;
						}

						/* checks if the shot is a hit or miss */
						puts("351");fflush(stdout);
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
						target->row = getRandomNumber (0, 9);
						target->column = getRandomNumber (0, 9);

						/* checks target if it is a miss or hit cell */
						puts("402");fflush(stdout);
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
					puts("going to target mode");fflush(stdout);
					targetAI = target;

					/**
					 * get values of adjacent cells and ensuring that
					 * that it is within the bounds of gameboard for
					 * NORTH, SOUTH, WEST & EAST
					 */
					if (cardinals[NORTH] == TRUE) {  /* NORTH */
						north = (targetAI->row - 1);
						checkBoundsOfCardinal (cardinals, north, NORTH);
						targetTemp = target;
						targetTemp->row = north;
						puts("432");fflush(stdout);
						if (checkShot (playerOneGameBoard, targetTemp) == -1)
							cardinals[NORTH] = FALSE;
					}

					if (cardinals[SOUTH] == TRUE) {  /* SOUTH */
						south = targetAI->row + 1;
						checkBoundsOfCardinal (cardinals, south, SOUTH);
						targetTemp = target;
						targetTemp->row = south;
						puts("450");fflush(stdout);
						if (checkShot (playerOneGameBoard, targetTemp) == -1)
							cardinals[SOUTH] = FALSE;
					}

					if (cardinals[WEST] == TRUE) {   /* WEST */
						west  = targetAI->column - 1;
						checkBoundsOfCardinal (cardinals, west, WEST);
						targetTemp = target;
						targetTemp->column = west;
						puts("460");fflush(stdout);
						if (checkShot (playerOneGameBoard, targetTemp) == -1)
							cardinals[WEST] = FALSE;
					}

					if (cardinals[EAST] == TRUE) {   /* EAST */
						east  = targetAI->column + 1;
						checkBoundsOfCardinal (cardinals, east, EAST);
						targetTemp = target;
						targetTemp->column = east;
						puts("470");fflush(stdout);
						if (checkShot (playerOneGameBoard, targetTemp) == -1)
							cardinals[EAST] = FALSE;
					}
				}

				/* saves the character of the targeted cell */
				shipSymbol = playerOneGameBoard[target->row][target->column].symbol;
				break;
		} //end of switch on player

		/**
		 * Prompts player if it's a hit or miss
		 */
		puts("about hit or miss");fflush(stdout);
		if (shot == 1) { /* HIT */
			printf ("> %d, %d is a hit!\n", target->row, target->column);fflush(stdout);

			/* Write to battleship.log */
			fprintf (outStream, "%d, %d is a hit!\n", target->row, target->column);fflush(outStream);

			/* Keeps track so number of hits for stats */
			players[player].numHits++;

			/* Checks if the ship has sank */
			if (player == 1)
				hasAShipSunked = checkSunkShip (sunkShip, !player, shipSymbol, outStream);
			else
				checkSunkShip (sunkShip, !player, shipSymbol, outStream);

		} else {        /* MISS */
			printf ("> %d, %d is a miss!\n", target->row, target->column);fflush(stdout);

			/* Write to battleship.log */
			fprintf (outStream, "%d, %d is a miss!\n", target->row, target->column);fflush(outStream);
			players[player].numMisses++;
		}

		if (player == 0) /* If Player 1 then update player 2's gameboard */
			updateGameBoard (playerTwoGameBoard, target);
		else             /* If Player 2 then update player 1's gameboard */
			updateGameBoard (playerOneGameBoard, target);
        puts("boards updated");fflush(stdout);

		/* Determins if there is a winner based on number of hits */
		if (isWinner (players, player)) {
			printf ("\n> Player %d wins!\n", player + 1);fflush(stdout);

			/* Write to battleship.log */
			fprintf (outStream, "\n>>>>> Player %d wins! <<<<<\n", player + 1);fflush(outStream);
			break;
		}

		//systemMessage ("> Hit <ENTER> to continue!\n");
		myflush ( stdin );
		mypause();


		/* switches from player 1 to player 2 */
		player = !player;

		//system ("cls");
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
	return 0;
}


