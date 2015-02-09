/** @file 2048.c
 * @brief This is the main file of the program,
 * which is used to run the program. 
 */

#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "key_algorithm.h"
#include "menu.h"
#include "score.h"

/** @brief Initialise the game board and setup the 
 * color pairs when starting the game.
 * @param a the array containing the numbers of the game board
 * @param s the current score 
 * @param f game status (game over or not)
 * @param y colum length of game board
 * @return none
 */
void init_table(int *a, int *s, bool *f, int *y);

/** @brief Print all the current numbers of the game board
 * on the terminal.
 * @param a the array containing the numbers of the game board
 * @param s the current score 
 * @param f game status (game over or not)
 * @param y colum length of game board
 * @return none
 */
void print_table(int *a, int *s, bool *f, int *y);

/** @brief Add a new random number to the board (2 or 4)
 * @param a the array containing the numbers of the game board
 * @param f game status (game over or not)
 * @param y colum length of game board
 * @return none
 */
void add_value(int *a, bool *f, int *y);

/** @brief Display game board's sizes for players to choose
 * @return none
 */
void table_choices();

/** @brief Reset all numbers on the board back to 0 
 * (including game status and current score).
 * @param a the array containing the numbers of the game board
 * @param y colum length of game board
 * @return none
 */
void reset_table(int *a, int *y);

/**
 * @brief Main function.
 * @return integer
 */
int main(int argc, char const *argv[]) {	
	initscr();  /* Initialize the ncurses library. */
	cbreak();  /* Take input chars one at a time, no wait for \n. */
	keypad(stdscr, TRUE);  /* Enable keyboard mapping. */	
	noecho();  /* Do not echo input. */	
	refresh();  /* Refresh terminal's UI. */
	curs_set(0);  /* hide the terminal cursor */

	struct timespec delay = {0, 999999999L}, 
                     rem;

	int size = 0;  /* column length of game board */
	int *y = &size;	
											// of the game board.
	int *inp = malloc(sizeof(int));  /* store the input's value in the HEAP. */
	int *score = malloc(sizeof(int));  /* store the score in the HEAP */
	*score = 0;  /* instantiate value */				

	/* store value to check if the player wins or fails the game */
	bool *isFail = malloc(sizeof(bool)); 	
	bool *isStuck = malloc(sizeof(bool)); 
	bool *isMatch = malloc(sizeof(bool)); 
	
	print_menu();  /* display main menu */
	table_choices();  /* display game board's sizes */
	/* get input about the size from users */	
	while(*inp != '1' && *inp != '2' && *inp != '3') {
		*inp = getch();  

		if (*inp == '1') {
			*y = 3;
		} else if (*inp == '2') {
			*y = 4;
		} else if (*inp == '3')	{
			*y = 5;
		}
	}
	clear();
	int *table = malloc(sizeof(int) * (*y * *y));  // array to store the numbers

	init_table(table, score, isFail, y);		

	/* keep playing until the user presses q on the keyboard */
	while(*inp != 'q' && *inp != 'Q') {  				
		if (*isFail == false) {  // accept up, down, right, left keys 
								 // as long as player doesn't fail								  
			*inp = getch();  /* get input from keyboard */						
			switch(*inp) {			
				case KEY_DOWN:  /* press "DOWN" button */
					down(table, score, isStuck, isMatch, y);					

					if (*isStuck == false || *isMatch == true) {
						add_value(table, isFail, y);	/* print the table again */											
					}					

					/* clear all UI elements displayed on the terminal */
					clear();  	
					print_table(table, score, isFail, y);								
					break;
				case KEY_UP:  /* press "UP" button */
					up(table, score, isStuck, isMatch, y);					
					
					if (*isStuck == false || *isMatch == true) {
						add_value(table, isFail, y);
					}					
					
					clear();	
					print_table(table, score, isFail, y);	
					break;
				case KEY_RIGHT:  /* press "RIGHT" button */
					right(table, score, isStuck, isMatch, y);					
					
					if (*isStuck == false || *isMatch == true) {
						add_value(table, isFail, y);
					}					
					
					clear();		
					print_table(table, score, isFail, y);	
					break;
				case KEY_LEFT:  /* press "LEFT" button */
					left(table, score, isStuck, isMatch, y);					
					
					if (*isStuck == false || *isMatch == true) {
						add_value(table, isFail, y);
					}					
					
					clear();									
					print_table(table, score, isFail, y);	
					break;
				case 'r':  /* press 'R' to restart game */
					*score = 0;
					reset_table(table, y);
					clear();
					init_table(table, score, isFail, y);		
					break;	
				default:				
					break;
			}
			check_failing(table, isFail, y);  // check if there's any available 
										   // move left on the board.
		} else {  /* player fails */

			clear(); 
			print_table(table, score, isFail, y); /* Print table when fail */

            clear();
            store_score(score); /* Store current score of player */
            print_score(1); /* Print Top 10 scores */
            print_table(table, score, isFail, y); /* Print the game table again */

			while(*inp != 'q' && *inp != 'Q') {
				*inp = getch();  /* get input from keyboard */

				if (*inp == 'r') {  /* player wants to restart game */
					*isFail = false;  
					*score = 0;
					reset_table(table, y);  /* reset value of the table */
					clear();
					init_table(table, score, isFail, y);		
					break;		
				}
			}			
		}				
	}		

	/* free all values in the HEAP */
	free(table);  free(inp);  free(score);
	free(isFail);  free(isStuck);  free(isMatch);

	clear();
	int row, col;
	getmaxyx(stdscr,row,col);  /* get the number of rows and columns */
	/* print good bye message */
 	mvprintw(row/2,(col-strlen("Good bye. See you again!"))/2,"%s", 
 		"Good bye. See you again!");	
	refresh();
	nanosleep(&delay, &rem);  /* display for a while before exit */

	endwin();
	return 0;
}

void init_table(int *a, int *s, bool *f, int *y) {				
	if (has_colors()) {  /* check if Terminal supports colors */
		start_color();
		/* initialize color pairs for the game */		 
		init_pair(1, COLOR_YELLOW, COLOR_BLACK);
		init_pair(2, COLOR_RED, COLOR_BLACK);
		init_pair(3, COLOR_GREEN, COLOR_BLACK);
		init_pair(4, COLOR_BLUE, COLOR_BLACK);
		init_pair(5, COLOR_CYAN, COLOR_BLACK);	
		init_pair(6, COLOR_MAGENTA, COLOR_BLACK);	
		init_pair(7, COLOR_WHITE, COLOR_BLACK);	
	}	

	add_value(a, f, y);
	add_value(a, f, y);
	print_table(a, s, f, y);
}

void add_value(int *a, bool *f, int *y) {
	int slots = *y * *y;

	int init_values[] = {2, 4};
	/* array to store location of empty slot of the table */	 
	int avail_space[slots];  

	/* check if the table has at least 1 empty slot */	 
	bool check_value = false;  
	int count = 0;
	for (int i = 0; i < slots; ++i) {
		if (*(a + i) == 0) {			
			avail_space[count] = i;	 /* store the location */		
			count++;
			check_value = true;			
		} 
	}	

	if (check_value == true) {  /* the table has at least 1 empty slot */
		/* generate a random empty slot */
		srand(time(NULL));  		
		int random_space = rand() % count;  
		srand(1);

		/* generate a random number (2 or 4) */
		srand(time(NULL));
		int random_value = rand() % 2;
		srand(1);

		/* add random value to that random slot */
		a[avail_space[random_space]] = init_values[random_value];	
	} else {
		*f = true;  /* change game status to "game over" */
	}	
}

void print_table(int *a, int *s, bool *f, int *y) {  	
	printw("\n"); 	
	int winning_score = 0;
	if (*y == 3) {
		winning_score = 1024;
	} else if (*y == 4)	{
		winning_score = 2048;
	} else if (*y == 5)	{
		winning_score = 4096;
	}

	for (int i = 0; i < *y; ++i)	{
		for (int j = 0; j < *y; ++j)	{
			if (a[i * *y + j] == 0)	{
				attron(COLOR_PAIR(7));
				printw("%10d", a[i * *y + j], "");					
				attroff(COLOR_PAIR(7));
			} else if (a[i * *y + j] == 2)	{
				attron(COLOR_PAIR(1));
				printw("%10d", a[i * *y + j], "");					
				attroff(COLOR_PAIR(1));
			} else if (a[i * *y + j] == 4)	{
				attron(COLOR_PAIR(2));
				printw("%10d", a[i * *y + j], "");					
				attroff(COLOR_PAIR(2));
			} else if (a[i * *y + j] == 8)	{
				attron(COLOR_PAIR(3));
				printw("%10d", a[i * *y + j], "");					
				attroff(COLOR_PAIR(3));
			} else if (a[i * *y + j] == 16)	{
				attron(COLOR_PAIR(4));
				printw("%10d", a[i * *y + j], "");					
				attroff(COLOR_PAIR(4));
			} else if (a[i * *y + j] == 32)	{
				attron(COLOR_PAIR(5));
				printw("%10d", a[i * *y + j], "");					
				attroff(COLOR_PAIR(5));
			} else {
				attron(COLOR_PAIR(6));
				printw("%10d", a[i * *y + j], "");					
				attroff(COLOR_PAIR(6));
			}			
		}
		printw("\n\n\n\n");
	}
	
	if (*f == false) {  /* game isn't over yet */
		attron(COLOR_PAIR(2));
		printw("SCORE: %5d", *s);
		attroff(COLOR_PAIR(2));
	/* game over and player's score is >= winning score */		
	} else if (*f == true && *s >= winning_score) {  
		printw("SCORE: %5d\n\n", *s);		
		attron(COLOR_PAIR(2));
		printw("GAME OVER! YOU WON THE GAME :)");		
		attroff(COLOR_PAIR(2));
	} else {  /* game over and player's score is < winning score */
		printw("SCORE: %5d\n\n", *s);		
		attron(COLOR_PAIR(2));
		printw("GAME OVER! YOU FAILED THE GAME :(");
		attroff(COLOR_PAIR(2));
	}		
	
	printw("\nPress 'Q' to exit the game OR 'R' to restart the game");

	refresh();
}

void reset_table(int *a, int *y) {  
	int size = *y * *y;
	for (int i = 0; i < size; ++i) {
		*(a + i) = 0;
	}	
}

void table_choices() {
	printw("Game board's sizes\n\n");
	printw("\t1. 3 x 3 (You need at least 1024 to win)\n");
	printw("\t2. 4 x 4 (You need at least 2048 to win)\n");
	printw("\t3. 5 x 5 (You need at least 4096 to win)\n\n");
	printw("Press '1', '2' or '3' to choose the board size.\n");
	refresh();
}


