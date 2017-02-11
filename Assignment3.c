/* "I declare that the attached assignment is wholly my own work in accordance
with Seneca Academic Policy. No part of this assignment has been copied
manually or electronically from any other source (including web sites) or
distributed to other students."

Name Jacob Adach   Student ID  062271143*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define CAP 9
#define MISSILE 100

void playBattleship(void);
void initGame(char board[], char mask[]);
void drawGame(char mask[], char board[], char PM[], int* MF,int* MR,int* CS);
void loadShips(char board[], char ship[][CAP]);
int checkmove(char CM[]);
int updatedata(char CM[],char mask[],char board[], char PM[], int* MF,int* MR,int* CS);
void clear(void);

int main()
{
	playBattleship(); //Begins Game
	getchar();
	return 0;
}
void playBattleship(void)
{
	char ship[4][CAP] = { { "[CCCCC=>"}, { "[DDD=>" }, { "[SS=>" }, { "[F>" } }; //These are the ships
	char mask[391]; 
	char board[391];
	char CM[3], PM[4]= {"N/A"};//Current and previous moves
	int MF = 00, MR = MISSILE, CS = 000, CHECK; // MissilesFired, MissilesRemaining, CurrentScore.
initGame(mask,board); 
loadShips(board, ship); // Places the ships.
drawGame(mask, board, PM, &MF, &MR, &CS );
		for (CHECK = 0; CHECK == 0 ;)
		{
		CM[0]=0;
		CM[1]=0;//Resets the value of coordinates
			printf("Enter Target Coordinates--> ");
			scanf("%2s", CM);
			clear();
			CHECK = checkmove(CM);
			if ( CHECK == 0 )
				printf("Sorry...%s   is not a valid target\n",CM);
				else if ( CHECK == 1 )//If coordinates were valid
					{
					CHECK = updatedata(CM,mask,board,PM,&MF,&MR,&CS);
					drawGame(mask, board, PM, &MF, &MR, &CS);
					}
		}	
	if ( CHECK == 2 )
	{
	printf("Congratulations You Have Sunk All the Battleships!\nVictory is yours!! ");
	getchar();
	}
		else
		{
		printf("You have Failed, all missiles have been Squandered...You Lose!!");
		getchar();
		}
}

void initGame(char board[], char mask[]) // Creates the characters ~ for mask and board
{
int i;
for (i = 0; i<390; i++) 
{
mask[i] = '~';
}
mask[i] = '\0'; /* add the null byte to use as a string if required */
strcpy(board, mask);
}
void loadShips(char board[], char ship[][CAP]) // Randomly places ships.
{
srand(time(NULL));
//a is the ship type, b & c are the markers, d is the position on the board, e is the ship character
int a, b, c, d, e;
	for (a = 0; a < 4; a++)  // Loops for finding rando locations for ships
		{
		switch (a)
		{
		case 0:	// First ship location will be randomized
			do {
				b = rand() % 18 + 1;
				c = rand() % 15 + 0;
				d = c * 26 + b;
			} while (board[d] != '~' || board[d + 1] != '~' || board[d + 2] != '~' 
|| board[d + 3] != '~' || board[d + 4] != '~' || board[d + 5] != '~' || board[d + 6] != '~' || board[d + 7] != '~'); 
			//makes sure the second ship doesnt collide with the first
			break;
			case 1:
				do {
				b = rand() % 20 + 1;
				c = rand() % 15 + 0;
				d = c * 26 + b;
					} while (board[d] != '~' || board[d + 1] != '~' || board[d + 2] != '~' 
|| board[d + 3] != '~' || board[d + 4] != '~' || board[d + 5] != '~'); 
				//makes sure the second ship doesnt collide with the first
				break;
				case 2:
					do {
					b = rand() % 21 + 1;
					c = rand() % 15 + 0;
					d = c * 26 + b;
						} while (board[d] != '~' || board[d + 1] != '~' || board[d + 2] != '~'
|| board[d + 3] != '~' || board[d + 4] != '~'); 
				//makes sure the ships dont collide
				break;
				case 3:
					do {
						b = rand() % 23 + 1;
						c = rand() % 15 + 0;
						d = c * 26 + b;
					    } while (board[d] != '~' || board[d + 1] != '~' || board[d + 2] != '~'); 
						//makes sure the ships dont collide
						break;
		}
	for (e = 0; ship[a][e] != '\0'; e++) // Prints out the ships location according to 'a' the marker.
	{
	board[d] = ship[a][e];
	d++;
	}
}
}
void drawGame(char mask[], char board[], char PM[], int* MF, int* MR, int* CS) // Draws the game board and initializes data
{
int i, j;
printf("   C Battleship...\n");
printf("   ABCDEFGHIJKLMNOPQRSTUVWXYZ\n");
for (i = 0; i<15; i++) // Produces the Game Boarder
{               
	if (i < 9) 
		{
		printf(" %c|", i + 1 + '0');  
		}
		else 
		{
		printf(" %c|", i + 1 + 55);    
		}
		for (j = 0; j<26; j++) 
			{             // display all 26 columns in each row 
			printf("%c", mask[j + (i * 26)]);
			}
					printf("|\n");
}
	printf("\n\n");
	printf("Missiles Fired: %d   Missiles Remaining: %d\nCurrent Score:  %d   Previous Move: %s\n",*MF,*MR,*CS,PM);
	//Below is an option to show the enemy board and ship locations, by defualt it is disabled.
	/*for (i = 0; i < 15; i++)
	{
	if (i < 9)
	{
	printf(" %c|", i + 1 + '0');
	}
	else {
	printf(" %c|", i + 1 + 55);
	}
	for (j = 0; j < 26; j++) {
	printf("%c", board[j + (i * 26)]);
	}
	printf("|\n");
	}*/
}
void clear(void)
{
	while (getchar() != '\n');
}
int checkmove(char CM[])//Validate Coordinates
{
	int b; 
if (CM[0] < '1' || CM[0] > '9' && CM[0] < 'A' || CM[0] > 'F')//Makes sure input is within boundaries
	b = 0;
		else if (CM[1] < 'A' || CM[1] > 'Z')//Makes sure input of 2nd letter  is within boundaries
		b = 0;
			else
			b = 1;
				return b;
}
int updatedata(char CM[], char mask[], char board[], char PM[], int* MF, int* MR, int* CS)
{
	int a, b, c, CHECK;
	a = CM[0];
	b = CM[1];

if (a >= 49 && a <= 57) //Convert Input into coordinates
a = a - 49;//If first input is between 1-9
		else
		a = a - 56;//If first input is between A-F
b -= 65;
c = a * 26 + b;//c will calculate the user coordinates to a location on the map
		if (board[c] == '~')//Hit Detection on the board and player input
			{	
			printf("MISS!\n");
			board[c] = 'x';
			mask[c] = 'x';
			getchar();
			*MF += 1;// Adds missile fired and subtracts missile remaining
			*MR -= 1;
			PM[0]= CM[0];// Copies the coordintates to the previous move
			PM[2]= CM[1]; 
			CHECK = 0;
			}
	else if (board[c] == 'x')
	{
	printf("This Location Has Already Been Hit!\n");
	getchar();
	*MF += 1;// ^^same as above
	*MR -= 1;
	PM[0]= CM[0];
	PM[2]= CM[1];
	CHECK = 0;
	}
		else
		{
		printf("Enemy Ship Has Been Hit!\n");
		mask[c] = board[c];
		board[c] = 'x';
		*MF += 1;//^^Same as above
		*MR -= 1;
		*CS += 5;//Adds 5 points per hit
		PM[0]= CM[0];
		PM[2]= CM[1];
		getchar();
		CHECK = 0;
		}
if ( *CS == 110 )//Determines if the game will continue or end based on score and missiles remaining
return CHECK = 2;
	else if ( *MR == 0 )
	return CHECK = 3;
		else
		return CHECK;
}
