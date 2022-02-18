#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int roll(int *die1, int *die2);
int setupBoard(int *board);
int printBoard(int *board, int *bar, int die1, int die2, int playerGo);
int checkEndgame(int *board, int *endgame1, int *endgame2);
int checkWin(int *board, int endgame1, int endgame2);
int nextPlayer(int *playerGo);
int checkBar(int *board, int *bar, int die1, int die2, int playerGo);


int main(){
	#define POINT 24
	#define PIECES 5

	int die1, die2;
	int playerGo = 1;
	int endgame1 = 0, endgame2 = 0, win = 0; 

	srand (time(NULL));

	
	// initialise game board and bar
	int board[POINT][PIECES] = {0};
	int bar[PIECES] = {0};

	// place initial setup pieces 
	setupBoard(&board);

	// get dice roll
	roll(&die1, &die2);

	while(win !=1){

		// print game board
		printBoard(&board, &bar, die1, die2, playerGo);

		// check bar
		checkBar(&board, &bar, die1, die2, playerGo);

		// get move 
		int curPos = 0, newPos = 0, valid = 1;
		do{
			printf("Enter a move (From Point No : To Point No): ");
			scanf("%d%d", &curPos, &newPos);
			printf("move from %d to %d\n", curPos, newPos);
			
			// check move is valid
			valid = checkValidMove(&board, &bar, die1, die2, curPos, newPos, playerGo, endgame1, endgame2);
		}
		while(valid != 0);

		// move piece 
		movePiece(&board, &bar, curPos, newPos, &die1, &die2, playerGo);

		//check endgame
		checkEndgame(&board, &endgame1, &endgame2);

		// check win
		win = checkWin(&board, endgame1, endgame2);

		//swap player if not doubles 
		if((die1 == -1) && (die2 == -1)){
			nextPlayer(&playerGo);
			// get dice roll
			roll(&die1, &die2);
			die1 = 5;
			die2 = 4;
		}

		//win = 1;
	}

	return 0;
}


int movePiece(int *board, int bar[], int curPos, int newPos, int *die1, int *die2, int playerGo){
	for (int i = PIECES -1; i >= 0; i--){
		if( *((board+curPos*PIECES) + i) == playerGo){
			*((board+curPos*PIECES) + i) = 0;

			for (int i = 0; i < PIECES; i++){
				if( *((board+newPos*PIECES) + i) != playerGo){
					if (*((board+newPos*PIECES) + i) != 0){
						printf("piece has been moved to bar\n");
						bar[0] = *((board+newPos*PIECES) + i);
					}
					*((board+newPos*PIECES) + i) = playerGo;

					if( abs(newPos - curPos) == *die1){
						*die1 = -1;
						return 0;
					}
					if( abs(newPos - curPos) == *die2){
						*die2 = -1;
						return 0;
					}
				}
			}

		}
	}
}

int checkBar(int *board, int *bar, int die1, int die2, int playerGo){

	for(int i = 0; i < PIECES; i++){
		if(bar[i] == playerGo){
			printf("\nMust move from bar! \n");

			int oppPieces = 0;

			if(playerGo == 1){
				for(int i = 0; i < PIECES; i++){
					if(*((board+(die1-1)*PIECES) + i) == 2){
						oppPieces++;
					}
				}
				if (oppPieces >= 2){
					printf("unable to move to point %d\n", die1-1);
				}
				oppPieces = 0;
				for(int i = 0; i < PIECES; i++){
					if(*((board+ (die2-1)*PIECES) + i)== 2){
						oppPieces++;
					}
				}
				if (oppPieces >= 2){
					printf("unable to move to point %d\n", die2-1);
				}
			}

			if(playerGo == 2){
				for(int i = 0; i < PIECES; i++){
					if(*((board+(POINT - die1)*PIECES) + i) == 1){
						oppPieces++;
					}
				}
				if (oppPieces >= 2){
					printf("unable to move to point %d\n", POINT - die1);
				}
				oppPieces = 0;
				for(int i = 0; i < PIECES; i++){
					if(*((board+ (POINT - die2)*PIECES) + i)== 1){
						oppPieces++;
					}
				}
				if (oppPieces >= 2){
					printf("unable to move to point %d\n", POINT - die2);
				}
			}

		}
	}

	return 0;
}

int checkValidMove(int *board, int *bar, int die1, int die2, int curPos, int newPos, int playerGo, int endgame1, int endgame2){	
	// check valid endgame play  
	if(playerGo == 1 && endgame1 == 1){
		if( (*((board+curPos*PIECES) + 0) == playerGo) && (curPos >= 0 && curPos <= 5) && newPos == -1){
			return 0;
		}
		printf("To move piece off of board, set new position to -1\n");
		return 1;
	}

	//
	if(playerGo == 2 && endgame2 == 1){
		if( (*((board+curPos*PIECES) + 0) == playerGo) && (curPos >= 18 && curPos <= 23) && newPos == -1){
			return 0;
		}
		printf("To move piece off of board, set new position to -1\n");
		return 1;
	}


	// check on board
	if(curPos > 23 || curPos < 0 || newPos > 23 || newPos < 0){
		printf("Input out of range!\n");
		return 1;
	}
	// check move forwards
	if( ((playerGo == 1) && (newPos>curPos))  || ((playerGo == 2) && (newPos<curPos)) ){
		printf("You can only move forwards!\n");
		return 1;
	}
	// check distance == die value
	if((abs(newPos - curPos) != die1) && (abs(newPos - curPos) != die2)){
		printf("distance between positions must be equal to one of the die rolls\n");
		return 1;
	}
	// check new position contents
	int oppPieces = 0;
	for(int i = 0; i < PIECES; i++){
		if((*((board+newPos*PIECES) + i) != playerGo) && (*((board+newPos*PIECES) + i) != 0)){
			oppPieces++;
		}
	}
	if(oppPieces >= 2){
		printf("Can't place ontop of oponent pieces!\n");
		return 1;
	}

	// check piece exists
	for (int i = 0; i < PIECES; i++){
		if( *((board+curPos*PIECES) + i) == playerGo){
			printf("piece exists\n");
			if(oppPieces == 1){
				printf("opponents piece has been moved to bar!\n");
			}
			
			return 0;
		}
	}
}

int roll(int *die1, int *die2){

	*die1 = (rand() % 5)+1;
	*die2 = (rand() % 5)+1;

	return 0;
}

int setupBoard(int *board){

	// print player1 pieces
	*((board+23*PIECES) + 0) = 1;
	*((board+23*PIECES) + 1) = 1;

	*((board+12*PIECES) + 0) = 1;
	*((board+12*PIECES) + 1) = 1;
	*((board+12*PIECES) + 2) = 1;
	*((board+12*PIECES) + 3) = 1;
	*((board+12*PIECES) + 4) = 1;

	*((board+7*PIECES) + 0) = 1;
	*((board+7*PIECES) + 1) = 1;
	*((board+7*PIECES) + 2) = 1;

	*((board+5*PIECES) + 0) = 1;
	*((board+5*PIECES) + 1) = 1;
	*((board+5*PIECES) + 2) = 1;
	*((board+5*PIECES) + 3) = 1;
	*((board+5*PIECES) + 4) = 1;


	// print player2 pieces
	*((board+0) + 0) = 2;
	*((board+0) + 1) = 2;

	*((board+11*PIECES) + 0) = 2;
	*((board+11*PIECES) + 1) = 2;
	*((board+11*PIECES) + 2) = 2;
	*((board+11*PIECES) + 3) = 2;
	*((board+11*PIECES) + 4) = 2;

	*((board+16*PIECES) + 0) = 2;
	*((board+16*PIECES) + 1) = 2;
	*((board+16*PIECES) + 2) = 2;

	*((board+18*PIECES) + 0) = 2;
	*((board+18*PIECES) + 1) = 2;
	*((board+18*PIECES) + 2) = 2;
	*((board+18*PIECES) + 3) = 2;
	*((board+18*PIECES) + 4) = 2;

	return 0;
}

int printBoard(int *board, int *bar, int die1, int die2, int playerGo){

	printf("\n12  13  14  15  16  17  18  19  20  21  22  23\n");

	for(int i = 0; i < PIECES; i++){
		for(int j = (POINT/2); j < (POINT); j++){
			printf("%-4.0d", *((board+j*PIECES) + i));
		}
		printf("\n");
	}
	printf("\n");
	for(int i = PIECES-1; i >= 0; i--){
		for(int j = ((POINT/2)-1); j >= 0; j--){
			printf("%-4.0d", *((board+j*PIECES) + i));
		}
		printf("\n");
	}

	printf("11  10  9   8   7   6   5   4   3   2   1   0\n\n");

	printf("Bar = ");
	for(int i = 0; i < PIECES; i++){
		printf("%-4.0d", *(bar));
	}

	printf("\n\nroll 1: %d  roll 2: %d\n\n", die1, die2);

	printf("Player%d's turn \n", playerGo);

	return 0;
}

int checkEndgame(int *board, int *endgame1, int *endgame2){
	int piecesCount = 0;
	for(int i = 0; i <= 5; i++){
		for(int j = 0; j < PIECES; j++){
			if(*((board+i*PIECES) + j)== 1){
				piecesCount++;
			}
		}
	}
	if(piecesCount == 15){
		*endgame1 = 1;
		printf("\n\nPlayer 1 can begin removing pieces from the board! \n\n");
	}

	piecesCount = 0;
	for(int i = 18; i <= 23; i++){
		for(int j = 0; j < PIECES; j++){
			if(*((board+i*PIECES) + j) == 2){
				piecesCount++;
			}
		}
	}
	if(piecesCount == 15){
		*endgame2 = 1;
		printf("\n\nPlayer 2 can begin removing pieces from the board! \n\n");
	}

	return 0;
}

int checkWin(int *board, int endgame1, int endgame2){
	int piecesCount = 0;

	if(endgame1){
		for(int i = 0; i <= 5; i++){
			for(int j = 0; j < PIECES; j++){
				if(*((board+i*PIECES) + j)== 1){
					piecesCount++;
				}
			}
		}
		if(piecesCount == 0){
			printf("\n\nPlayer 1 win! \n\n");
			return 1;
		}
	}

	if(endgame2){
		for(int i = 18; i <= 23; i++){
			for(int j = 0; j < PIECES; j++){
				if(*((board+i*PIECES) + j) == 2){
					piecesCount++;
				}
			}
		}
		if(piecesCount == 0){
			printf("\n\nPlayer 2 win! \n\n");
			return 1; 
		}
	}
	return 0;
}

int nextPlayer(int *playerGo){
	if(*playerGo == 1){
		*playerGo = 2;
		return 0;
	}
	*playerGo = 1;
	return 0;
}