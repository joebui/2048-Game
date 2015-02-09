/** @file 2048.c
 * @brief This is the main file of the program,
 * which is used to run the program. 
 */

#include <ncurses.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include "key_algorithm.h"
#include "menu.h"
#include "score.h"

/** @struct Structure containing the properties of 2 players.
 */
struct player {
	int *table1; int *table2; int y;  int inp;  int score1; int score2; int timer; 	
	bool isStuck1; bool isMatch1; bool isFail1; bool isFail2; bool isStuck2; bool isMatch2; 
};

/** @brief Initialise the game board and setup the 
 * color pairs when starting the game.
 * @param a the array containing the numbers of the game board
 * @param s the current score 
 * @param f game status (fail or not)
 * @param y colum length of game board
 * @return none
 */
void init_table(int *a, int *s, bool *f, int *y);

/** @brief Initialize the game boards in 2-player game mode and 
 * setup the color pairs when starting the game.
 * @param table1 the array containing the numbers of player 1's game board
 * @param score1 the current score of player 1
 * @param isFail1 player 1's status (fail or not)
 * @param table2 the array containing the numbers of player 2's game board
 * @param score2 the current score of player 2
 * @param isFail2 player 2's status (fail or not)
 * @param y number of columns of game boards
 * @param inp input from the player
 * @return none
 */
void init_2_table(int *table1, int *score1, bool *isFail1, int *table2, 
	int *score2, bool *isFail2, int *y, int *timer);

/** @brief Print all the current numbers of the game board
 * on the terminal.
 * @param a the array containing the numbers of the game board
 * @param s the current score 
 * @param f game status (fail or not)
 * @param y column length of game board
 * @return none
 */
void print_table(int *a, int *s, bool *f, int *y);

/** @brief Print all the current numbers of the 2 game boards.
 * @param table1 the array containing the numbers of player 1's game board
 * @param score1 the current score of player 1
 * @param isFail1 player 1's status (fail or not)
 * @param table2 the array containing the numbers of player 2's game board
 * @param score2 the current score of player 2
 * @param isFail2 player 2's status (fail or not)
 * @param y number of columns of game boards
 * @param inp input from the player
 * @return none
 */
void print_2_table(int *table1, int *score1, bool *isFail1, int *table2, 
	int *score2, bool *isFail2, int *y, int *timer);

/** @brief Add a new random number to the board (2 or 4).
 * @param a the array containing the numbers of the game board
 * @param f game status (game over or not)
 * @param y colum length of game board
 * @return none
 */
void add_value(int *a, bool *f, int *y);

/** @brief Display game board's sizes for players to choose.
 * @return none
 */
void table_choices();

/** @brief Display game board's sizes for players to choose.
 * @param inp input from player
 */
void game_choices(int *inp, int *timer);

/** @brief Display the menu for player to choose time mode.
 */
void time_choice();

/** @brief Reset all numbers on the board back to 0 
 * (including game status and current score).
 * @param a the array containing the numbers of the game board
 * @param y colum length of game board
 * @return none
 */
void reset_table(int *a, int *y);

/** @brief Generate random AI (easy level).
 * @param param pointer to the "struct player"
 * @return none
 */
void *random_AI(void *param);

/** @brief Generate the move of human player 1.
 * @param param pointer to the "struct player"
 * @return none
 */
void *first_player_move(void* param);

/** @brief Generate the move of 2 players (human vs human).
 * @param param pointer to the "struct player"
 * @return none
 */
void *hvh_player_move(void* param);

/** @brief Generate a countdown clock
 * @param param pointer to the "struct player"
 * @return none
 */
void *count_down(void* param);

/** @brief Generate medium AI (AI vs AI Mode).
 * @param param pointer to the "struct player"
 * @return none
 */
void *medium_AI(void* param);

/** @brief Generate smart AI (hard Mode).
 * @param param pointer to the "struct player"
 * @return none
 */
void *smart_AI(void* param);

/** @brief check if the table is stuck (not change after trying to move)
 * @param a the array containing the numbers of the game board
 * @param a_clone the clone array of a
 * @param length board size
 * @return bool
 */
bool check_stuck(int *a, int *a_clone, int length);

/** @brief Count the number of matching pairs in the board
 * @param a the array containing the numbers of the game board
 * @param y colum length of game board
 * @return int
 */
int check_matchingPair(int *a, int *y);

/**
 * @brief Main function.
 * @return integer
 */
int main() {	
	initscr();  /* Initialize the ncurses library. */
	cbreak();  /* Take input chars one at a time, no wait for \n. */
	keypad(stdscr, TRUE);  /* Enable keyboard mapping. */	
	noecho();  /* Do not echo input. */	
	refresh();  /* Refresh terminal's UI. */
	curs_set(0);  /* hide the terminal cursor */
    pthread_t threads[3];

	/* column length of game board */
	int y = 0;
	int inp = 0;  /* store the input's value in the HEAP. */	

	print_menu();  /* display main menu */
	table_choices();  /* display game board's sizes */
	/* get input about the size from users */	
	while(inp != '1' && inp != '2' && inp != '3') {
		inp = getch();  

		if (inp == '1') {
			y = 3;
		} else if (inp == '2') {
			y = 4;
		} else if (inp == '3')	{
			y = 5;
		}
	}
	clear();	

	int timer = 0;
	game_choices(&inp, &timer); /* get the desired game mode of player */

	/* instantiate the values of the struct */	
	struct player p;	
	p.table1 = malloc(sizeof(int) * (y * y));  
	p.score1 = 0;
	p.isStuck1 = true; 
	p.isMatch1 = false;
	p.isFail1 = false;
	p.y = y;		
	p.inp = inp;	
	p.timer = timer;
	p.table2 = malloc(sizeof(int) * (y * y));  
	p.score2 = 0;
	p.isStuck2 = true; 
	p.isMatch2 = false;
	p.isFail2 = false;

	if (inp == 11) {  /* player chooses 1-player: Human */				
		pthread_create(&threads[0], NULL, first_player_move, &p); /* start thread */
	} else if (inp == 12) {  /* player chooses 1-player: AI */
		pthread_create(&threads[0], NULL, smart_AI, &p);
	} else if (inp == 211) {  /* player chooses 2-player: Human vs Human (unlimited) */				
		pthread_create(&threads[0], NULL, hvh_player_move, &p);		
	} else if (inp == 212) {  /* player chooses 2-player: Human vs Human (limited) */				
		pthread_create(&threads[1], NULL, count_down, &p);
		pthread_create(&threads[0], NULL, hvh_player_move, &p);		
	} else if (inp == 2211)	{  /* player chooses 2-player: Human vs AI (Easy - Unlimited) */		
		pthread_create(&threads[0], NULL, first_player_move, &p);
		pthread_create(&threads[1], NULL, random_AI, &p);
	} else if (inp == 2212)	{  /* player chooses 2-player: Human vs AI (Easy - Limited) */		
		pthread_create(&threads[0], NULL, count_down, &p);
		pthread_create(&threads[1], NULL, first_player_move, &p);
		pthread_create(&threads[2], NULL, random_AI, &p);		
	} else if (inp == 2221) {  /* player chooses 2-player: Human vs AI (Hard - Unlimited) */		
		pthread_create(&threads[0], NULL, first_player_move, &p);
		pthread_create(&threads[1], NULL, smart_AI, &p);
	} else if (inp == 2222) {  /* player chooses 2-player: Human vs AI (Hard - Limited) */		
		pthread_create(&threads[0], NULL, count_down, &p);
		pthread_create(&threads[1], NULL, first_player_move, &p);
		pthread_create(&threads[2], NULL, smart_AI, &p);		
	} else if (inp == 231) {  /* player chooses 2-player: AI vs AI (Unlimited) */		
		pthread_create(&threads[0], NULL, smart_AI, &p);
		pthread_create(&threads[1], NULL, medium_AI, &p);
	} else {  /* player chooses 2-player: AI vs AI (limited) */		
		pthread_create(&threads[0], NULL, count_down, &p);
		pthread_create(&threads[1], NULL, smart_AI, &p);
		pthread_create(&threads[2], NULL, medium_AI, &p);		
	}

	pthread_exit(NULL);  /* exit thread */			
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

void init_2_table(int *table1, int *score1, bool *isFail1, int *table2, 
	int *score2, bool *isFail2, int *y, int *timer) {
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

	/* add 2 initial numbers for 2 game boards */
	add_value(table1, isFail1, y);	add_value(table1, isFail1, y);
	add_value(table2, isFail2, y);  add_value(table2, isFail2, y);
	print_2_table(table1, score1, isFail1, table2, score2, isFail2, y, timer);
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
		printw("SCORE: %5d\n\n", *s);		
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
	
	printw("\nPress Esc to exit the game OR 'R' to restart the game\n");
	printw("For 1 player AI and AI vs AI, you can only press these keys after the game is over");

	refresh();
}

void print_2_table(int *table1, int *score1, bool *isFail1, int *table2, 
	int *score2, bool *isFail2, int *y, int *timer) {  	

	if (*timer > 0)  /* print countdown clock if user chooses limitied game mode */
		printw("%d seconds left\n\n", *timer);
	else
		printw("\n");

	for (int i = 0; i < *y; ++i)	{	
		for (int j = 0; j < *y; ++j)	{
			if (table1[i * *y + j] == 0)	{
				attron(COLOR_PAIR(7));
				printw("%10d", table1[i * *y + j], "");					
				attroff(COLOR_PAIR(7));
			} else if (table1[i * *y + j] == 2)	{
				attron(COLOR_PAIR(1));
				printw("%10d", table1[i * *y + j], "");					
				attroff(COLOR_PAIR(1));
			} else if (table1[i * *y + j] == 4)	{
				attron(COLOR_PAIR(2));
				printw("%10d", table1[i * *y + j], "");					
				attroff(COLOR_PAIR(2));
			} else if (table1[i * *y + j] == 8)	{
				attron(COLOR_PAIR(3));
				printw("%10d", table1[i * *y + j], "");					
				attroff(COLOR_PAIR(3));
			} else if (table1[i * *y + j] == 16)	{
				attron(COLOR_PAIR(4));
				printw("%10d", table1[i * *y + j], "");					
				attroff(COLOR_PAIR(4));
			} else if (table1[i * *y + j] == 32)	{
				attron(COLOR_PAIR(5));
				printw("%10d", table1[i * *y + j], "");					
				attroff(COLOR_PAIR(5));
			} else {
				attron(COLOR_PAIR(6));
				printw("%10d", table1[i * *y + j], "");					
				attroff(COLOR_PAIR(6));
			}				
		}

		printw("\t");

		for (int j = 0; j < *y; ++j)	{
			if (table2[i * *y + j] == 0)	{
				attron(COLOR_PAIR(7));
				printw("%10d", table2[i * *y + j], "");					
				attroff(COLOR_PAIR(7));
			} else if (table2[i * *y + j] == 2)	{
				attron(COLOR_PAIR(1));
				printw("%10d", table2[i * *y + j], "");					
				attroff(COLOR_PAIR(1));
			} else if (table2[i * *y + j] == 4)	{
				attron(COLOR_PAIR(2));
				printw("%10d", table2[i * *y + j], "");					
				attroff(COLOR_PAIR(2));
			} else if (table2[i * *y + j] == 8)	{
				attron(COLOR_PAIR(3));
				printw("%10d", table2[i * *y + j], "");					
				attroff(COLOR_PAIR(3));
			} else if (table2[i * *y + j] == 16)	{
				attron(COLOR_PAIR(4));
				printw("%10d", table2[i * *y + j], "");					
				attroff(COLOR_PAIR(4));
			} else if (table2[i * *y + j] == 32)	{
				attron(COLOR_PAIR(5));
				printw("%10d", table2[i * *y + j], "");					
				attroff(COLOR_PAIR(5));
			} else {
				attron(COLOR_PAIR(6));
				printw("%10d", table2[i * *y + j], "");					
				attroff(COLOR_PAIR(6));
			}						
		}
		printw("\n\n\n\n");
	}
		
	printw("\tSCORE (player 1): %d", *score1);
	printw("\t\t\tSCORE (player 2): %d\n\n", *score2);

	if (*timer > 0) {  /* the count down is still on */		
		if (*isFail1 == true) {   /* 1st player fails */
			attron(COLOR_PAIR(5));
			printw("Player 2 has won the game");					
			attroff(COLOR_PAIR(5));
		} else if (*isFail2 == true) {  /* 1st player fails */
			attron(COLOR_PAIR(5));
			printw("Player 1 has won the game");					
			attroff(COLOR_PAIR(5));
		} 
	} else {  /* the count down is over or the game's in unlimited mode */
		if (*isFail1 == true || *isFail2 == true) {  
			if (*score1 > *score2) {
				attron(COLOR_PAIR(5));
				printw("Player 1 has won the game");					
				attroff(COLOR_PAIR(5));
			} else if (*score1 < *score2)	{
				attron(COLOR_PAIR(5));
				printw("Player 2 has won the game");
				attroff(COLOR_PAIR(5));
			} else {
				attron(COLOR_PAIR(5));
				printw("No one wins the game");
				attroff(COLOR_PAIR(5));
			}
		}	
	}
	
	printw("\nPress Esc to exit the game OR 'R' to restart the game");
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

void game_choices(int *inp, int *timer) {
	*inp = '0';
	printw("Game mode\n\n");
	printw("\t1 - 1 player\n");
	printw("\t2 - 2 players\n\n");
	printw("Press '1' or '2' to choose game mode.\n");
	refresh();	

	while(*inp != '1' && *inp != '2') {
		*inp = getch();  

		if (*inp == '1') {
			clear();
			printw("1 - player mode\n\n");
			printw("\t1 - Human\n");
			printw("\t2 - AI\n\n");
			printw("Press '1' or '2' to choose game mode.\n");
			refresh();
			*inp = '0';

			while(*inp != '1' && *inp != '2') {
				*inp = getch();  

				if (*inp == '1') {
					*inp = 11;
					break;
				} else if (*inp == '2') {
					*inp = 12;
					break;
				}
			}
			clear();
			break;
		} else if (*inp == '2') {
			clear();
			printw("2 - player mode\n\n");
			printw("\t1 - Human vs Human\n");
			printw("\t2 - Human vs AI\n");
			printw("\t3 - AI vs AI\n\n");
			printw("Press '1', '2' or '3' to choose game mode.\n");
			refresh();
			*inp = 0;

			while(*inp != '1' && *inp != '2' && *inp != '3') {
				*inp = getch();  

				if (*inp == '1') {
					time_choice();
					*inp = 0;
					while(*inp != '1' && *inp != '2') {						
						*inp = getch();  						

						if (*inp == '1') {
							*inp = 211;
							break;
						} else if (*inp == '2') {
							*inp = 212;

							clear();
							printw("Enter the time range to play (in minute) then press \"Enter\": ");
							refresh();
							scanf("%d", timer);
							*timer = *timer * 60;
							break;
						}
					}

					clear();					
					break;
				} else if (*inp == '2') {					
					clear();
					printw("AI difficulty\n\n");
					printw("\t1 - Easy\n");
					printw("\t2 - Hard\n\n");
					printw("Press '1' or '2' to choose.\n");
					refresh();						
					*inp = 0;

					while(*inp != '1' && *inp != '2') {						
						*inp = getch();  						

						if (*inp == '1') {
							time_choice();
							*inp = 0;
							while(*inp != '1' && *inp != '2') {						
								*inp = getch();  						

								if (*inp == '1') {
									*inp = 2211;
									break;
								} else if (*inp == '2') {
									*inp = 2212;

									clear();
									printw("Enter the time range to play (in minute) then press \"Enter\": ");
									refresh();
									scanf("%d", timer);
									*timer = *timer * 60;
									break;
								}
							}

							clear();	
							break;
						} else if (*inp == '2') {
							time_choice();
							*inp = 0;
							while(*inp != '1' && *inp != '2') {						
								*inp = getch();  						

								if (*inp == '1') {
									*inp = 2221;
									break;
								} else if (*inp == '2') {
									*inp = 2222;

									clear();
									printw("Enter the time range to play (in minute) then press \"Enter\": ");
									refresh();
									scanf("%d", timer);
									*timer = *timer * 60;
									break;
								}
							}

							clear();	
							break;
						}
					}
					clear();
					break;
				} else if (*inp == '3') {
					time_choice();
					*inp = 0;
					while(*inp != '1' && *inp != '2') {						
						*inp = getch();  						

						if (*inp == '1') {
							*inp = 231;
							break;
						} else if (*inp == '2') {
							*inp = 232;

							clear();
							printw("Enter the time range to play (in minute) then press \"Enter\": ");
							refresh();
							scanf("%d", timer);
							*timer = *timer * 60;
							break;
						}
					}

					clear();	
					break;
				}			
			}
		}
		break;
	}
}

void time_choice() {
	clear();
	printw("Time choices\n\n");
	printw("\t1 - Unlimited\n");
	printw("\t2 - Limited\n\n");
	printw("Press '1' or '2' to choose.\n");
	refresh();	
}

void *random_AI(void* param) {		
	struct player *randomAI = (struct player*) param;	
	int *table1 = randomAI->table1;
	int *score1 = &(randomAI->score1);	
	bool *isStuck1 = &(randomAI->isStuck2);
	bool *isMatch1 = &(randomAI->isMatch2);	
	int *y = &(randomAI->y);
	bool *isFail1 = &(randomAI->isFail1);
	int *timer = &(randomAI->timer);
	int *inp = &(randomAI->inp);
	int temp = *inp;

	int *table2 = randomAI->table2;
	int *score2 = &(randomAI->score2);
	bool *isStuck2 = &(randomAI->isStuck2);
	bool *isMatch2 = &(randomAI->isMatch2);	
	bool *isFail2 = &(randomAI->isFail2);

	sleep(1);	
	while (*inp != 27) {  /* player(s) doesn't press Esc */
		if (*isFail2 == false && *isFail1 == false) {
			/* generate random */
			srand(time(NULL));  		
			int random_move = rand() % 4;  
			srand(1);
			
			if (temp == 231 || temp == 232) {  /* player is in AI vs AI mode */
				if (random_move == 0) {
					down(table1, score1, isStuck1, isMatch1, y);							
				} else if (random_move == 1) {
					up(table1, score1, isStuck1, isMatch1, y);
				} else if (random_move == 2) {
					right(table1, score1, isStuck1, isMatch1, y);
				} else {
					left(table1, score1, isStuck1, isMatch1, y);
				}

				check_failing(table1, isFail1, y);  // check if there's any available 
										   			// move left on the board.									

				if (*isStuck1 == false || *isMatch1 == true) {
					add_value(table1, isFail1, y);	/* print the table again */											
					/* clear all UI elements displayed on the terminal */						
					clear();  	
					print_2_table(table1, score1, isFail1, table2, 
						score2, isFail2, y, timer);	
					sleep(1);					
				} 
			} else {  /* in other mode */				
				if (random_move == 0) {
					down(table2, score2, isStuck2, isMatch2, y);							
				} else if (random_move == 1) {
					up(table2, score2, isStuck2, isMatch2, y);
				} else if (random_move == 2) {
					right(table2, score2, isStuck2, isMatch2, y);
				} else {
					left(table2, score2, isStuck2, isMatch2, y);
				}

				check_failing(table2, isFail2, y);  // check if there's any available 
										   // move left on the board.									

				if (*isStuck2 == false || *isMatch2 == true) {
					add_value(table2, isFail2, y);	/* print the table again */											
					/* clear all UI elements displayed on the terminal */						
					clear();  	

					if (temp != 2212) {
						print_2_table(table1, score1, isFail1, table2, 
							score2, isFail2, y, timer);	
					}					
					sleep(1);					
				} 	
			}											
		} else {
			clear();  	
			print_2_table(table1, score1, isFail1, table2, score2, isFail2, y, timer);				
			sleep(1);					
		} 		
	}	
	
	pthread_exit(NULL);
}

void *first_player_move(void* param) {
	struct player *player = (struct player*) param;
	int *table1 = player->table1;
	int *score1 = &(player->score1);
	bool *isStuck1 = &(player->isStuck1);
	bool *isMatch1 = &(player->isMatch1);
	int *y = &(player->y);
	bool *isFail1 = &(player->isFail1);
	int *inp = &(player->inp);
	int *timer = &(player->timer);

	int *table2 = player->table2;
	int *score2 = &(player->score2);
	bool *isFail2 = &(player->isFail2);

	int temp = *inp;	
	int temp_time = *timer;

	reset_table(table1, y);  /* reset value of the table */
	reset_table(table2, y);  /* reset value of the table */
	if (temp == 11) {
		init_table(table1, score1, isFail1, y);
	} else {
		init_2_table(table1, score1, isFail1, table2, score2, isFail2, y, timer);
	}	
	
	while(*inp != 27 && *inp != 27) {  						
		if (*isFail1 == false && *isFail2 == false) {  // accept up, down, right, left keys 
								 					   // as long as player(s) doesn't fail.
			*inp = getch();  /* get input from keyboard */							
			switch(*inp) {			
				case KEY_DOWN:  /* press "DOWN" button */
					if (*isFail1 == false && *isFail2 == false) {
						down(table1, score1, isStuck1, isMatch1, y);					

						if (*isStuck1 == false || *isMatch1 == true) {
							add_value(table1, isFail1, y);	/* print the table again */											
						}					

						/* clear all UI elements displayed on the terminal */
						clear();  	
						if (temp == 11) {
							print_table(table1, score1, isFail1, y);
						} else {
							print_2_table(table1, score1, isFail1, table2, score2, isFail2, y, timer);
						}							
					}					
					break;
				case KEY_UP:  /* press "UP" button */
					if (*isFail1 == false && *isFail2 == false) {
						up(table1, score1, isStuck1, isMatch1, y);					

						if (*isStuck1 == false || *isMatch1 == true) {
							add_value(table1, isFail1, y);	/* print the table again */											
						}					

						/* clear all UI elements displayed on the terminal */
						clear();  	
						if (temp == 11) {
							print_table(table1, score1, isFail1, y);
						} else {
							print_2_table(table1, score1, isFail1, table2, score2, isFail2, y, timer);
						}							
					}					

					break;
				case KEY_RIGHT:   // press "RIGHT" button 
					if (*isFail1 == false && *isFail2 == false) {
						right(table1, score1, isStuck1, isMatch1, y);					

						if (*isStuck1 == false || *isMatch1 == true) {
							add_value(table1, isFail1, y);	/* print the table again */											
						}					

						/* clear all UI elements displayed on the terminal */
						clear();  	
						if (temp == 11) {
							print_table(table1, score1, isFail1, y);
						} else {
							print_2_table(table1, score1, isFail1, table2, score2, isFail2, y, timer);
						}							
					}										

					break;
				case KEY_LEFT:  /* press "LEFT" button */
					if (*isFail1 == false && *isFail2 == false) {
						left(table1, score1, isStuck1, isMatch1, y);					

						if (*isStuck1 == false || *isMatch1 == true) {
							add_value(table1, isFail1, y);	/* print the table again */											
						}					

						/* clear all UI elements displayed on the terminal */
						clear();  	
						if (temp == 11) {
							print_table(table1, score1, isFail1, y);
						} else {
							print_2_table(table1, score1, isFail1, table2, score2, isFail2, y, timer);
						}							
					}											

					break;
				case 'r':  /* press 'R' to restart game */
					*isFail1 = false;  *isFail2 = false;  
					*score1 = 0;  *score2 = 0;
					*timer = temp_time;
					reset_table(table1, y);  /* reset value of the table */
					reset_table(table2, y);  /* reset value of the table */
					clear();
					if (temp == 11) {
						init_table(table1, score1, isFail1, y);
					} else {
						init_2_table(table1, score1, isFail1, table2, score2, isFail2, y, timer);
					}
					break;		
				default:				
					break;
			}		

			check_failing(table1, isFail1, y);  // check if there's any available 
											  				   // move left on the board.				    									
		} else {  /* player fails */
			clear(); 
			/* Print table when fail */
			if (temp == 11) {
				print_table(table1, score1, isFail1, y);
				clear();
	            store_score(score1); /* Store current score of player */
	            print_score(1); /* Print Top 10 scores */
	            /* Print the game table again */
	            if (temp == 11) {
					print_table(table1, score1, isFail1, y);
				} else {
					print_2_table(table1, score1, isFail1, table2, score2, isFail2, y, timer);
				}	
			} else {
				print_2_table(table1, score1, isFail1, table2, score2, isFail2, y, timer);
			}            

			while(*inp != 27 && *inp != 27) {
				*inp = getch();  /* get input from keyboard */

				if (*inp == 'r') {  /* player wants to restart game */
					*isFail1 = false;  *isFail2 = false;  
					*score1 = 0;  *score2 = 0;
					*timer = temp_time;

					reset_table(table1, y);  /* reset value of the table */
					reset_table(table2, y);  /* reset value of the table */
					clear();
					if (temp == 11) {
						init_table(table1, score1, isFail1, y);
					} else {
						init_2_table(table1, score1, isFail1, table2, score2, isFail2, y, timer);
					}
					break;		
				}
			}			
		}				
	}

	*isFail1 = true; *isFail2 = true;
	free(table1); free(table2);
	clear();
	int row, col;
	getmaxyx(stdscr,row,col);  /* get the number of rows and columns */
	/* print good bye message */
 	mvprintw(row/2,(col-strlen("Good bye. See you again!"))/2,"%s", 
 		"Good bye. See you again!");	
	refresh();
	sleep(1);  /* display for a while before exit */
	exit(-1);

	pthread_exit(NULL);
}

void *hvh_player_move(void* param) {
	struct player *player = (struct player*) param;
	int *table1 = player->table1;
	int *score1 = &(player->score1);
	bool *isStuck1 = &(player->isStuck1);
	bool *isMatch1 = &(player->isMatch1);
	int *y = &(player->y);
	bool *isFail1 = &(player->isFail1);	
	int *timer = &(player->timer);
	int temp = *timer;

	int *table2 = player->table2;
	int *score2 = &(player->score2);
	bool *isStuck2 = &(player->isStuck2);
	bool *isMatch2 = &(player->isMatch2);	
	bool *isFail2 = &(player->isFail2);

	reset_table(table1, y);  /* reset value of the table */
	reset_table(table2, y);  /* reset value of the table */
	init_2_table(table1, score1, isFail1, table2, score2, isFail2, y, timer);

	int key = '0';
	while(key != 27 && key != 27) {  						
		if (*isFail1 == false && *isFail2 == false) {  // accept up, down, right, left keys 
								 // as long as player doesn't fail						
			key = getch();  /* get input from keyboard */				
			switch(key) {
				/* SECOND PLAYER'S MOVES */			
				case KEY_DOWN:  /* press "DOWN" button */
					if (*isFail1 == false && *isFail2 == false) {
						down(table2, score2, isStuck2, isMatch2, y);					

						if (*isStuck2 == false || *isMatch2 == true) {
							add_value(table2, isFail2, y);	/* print the table again */											
						}					

						/* clear all UI elements displayed on the terminal */
						clear();  				
						print_2_table(table1, score1, isFail1, table2, score2, isFail2, y, timer);
					}

					break;
				case KEY_UP:  /* press "UP" button */
					if (*isFail1 == false && *isFail2 == false) {
						up(table2, score2, isStuck2, isMatch2, y);					

						if (*isStuck2 == false || *isMatch2 == true) {
							add_value(table2, isFail2, y);	/* print the table again */											
						}					

						/* clear all UI elements displayed on the terminal */
						clear();  				
						print_2_table(table1, score1, isFail1, table2, score2, isFail2, y, timer);
					}

					break;
				case KEY_RIGHT:   // press "RIGHT" button 
					if (*isFail1 == false && *isFail2 == false) {
						right(table2, score2, isStuck2, isMatch2, y);					

						if (*isStuck2 == false || *isMatch2 == true) {
							add_value(table2, isFail2, y);	/* print the table again */											
						}					

						/* clear all UI elements displayed on the terminal */
						clear();  				
						print_2_table(table1, score1, isFail1, table2, score2, isFail2, y, timer);
					}

					break;
				case KEY_LEFT:  /* press "LEFT" button */
					if (*isFail1 == false && *isFail2 == false) {
						left(table2, score2, isStuck2, isMatch2, y);					

						if (*isStuck2 == false || *isMatch2 == true) {
							add_value(table2, isFail2, y);	/* print the table again */											
						}					

						/* clear all UI elements displayed on the terminal */
						clear();  				
						print_2_table(table1, score1, isFail1, table2, score2, isFail2, y, timer);
					}

					break;

				/* FIRST PLAYER'S MOVES */
				case 's':  /* press "DOWN" button */
					if (*isFail1 == false && *isFail2 == false) {
						down(table1, score1, isStuck1, isMatch1, y);										

						if (*isStuck1 == false || *isMatch1 == true) {							
							add_value(table1, isFail1, y);	/* print the table again */							
						}					

						/* clear all UI elements displayed on the terminal */
						clear();  	
						print_2_table(table1, score1, isFail1, table2, score2, isFail2, y, timer);
					}

					break;
				case 'w':  /* press "UP" button */
					if (*isFail1 == false && *isFail2 == false) {
						up(table1, score1, isStuck1, isMatch1, y);										

						if (*isStuck1 == false || *isMatch1 == true) {							
							add_value(table1, isFail1, y);	/* print the table again */							
						}					

						/* clear all UI elements displayed on the terminal */
						clear();  	
						print_2_table(table1, score1, isFail1, table2, score2, isFail2, y, timer);
					}

					break;
				case 'd':   // press "RIGHT" button 
					if (*isFail1 == false && *isFail2 == false) {
						right(table1, score1, isStuck1, isMatch1, y);										

						if (*isStuck1 == false || *isMatch1 == true) {							
							add_value(table1, isFail1, y);	/* print the table again */							
						}					

						/* clear all UI elements displayed on the terminal */
						clear();  	
						print_2_table(table1, score1, isFail1, table2, score2, isFail2, y, timer);
					}

					break;
				case 'a':  /* press "LEFT" button */
					if (*isFail1 == false && *isFail2 == false) {
						left(table1, score1, isStuck1, isMatch1, y);										

						if (*isStuck1 == false || *isMatch1 == true) {							
							add_value(table1, isFail1, y);	/* print the table again */							
						}					

						/* clear all UI elements displayed on the terminal */
						clear();  	
						print_2_table(table1, score1, isFail1, table2, score2, isFail2, y, timer);
					}

					break;		
				case 'r':
					*isFail1 = false;  *isFail2 = false;  
					*score1 = 0;  *score2 = 0;
					*timer = temp;
					reset_table(table1, y);  /* reset value of the table */
					reset_table(table2, y);  /* reset value of the table */
					clear();
					if (temp == 11) {
						init_table(table1, score1, isFail1, y);
					} else {
						init_2_table(table1, score1, isFail1, table2, score2, isFail2, y, timer);
					}
					break;		
				default:				
					break;
			}		

			/* check if there's any available move left for both players */
			check_failing(table1, isFail1, y); 
			check_failing(table2, isFail2, y);  
		} else {  /* player fails */
			clear(); 
			/* Print table when fail */			
			print_2_table(table1, score1, isFail1, table2, score2, isFail2, y, timer);

			key = '0';
			while(key != 27 && key != 27) {
				key = getch();  /* get input from keyboard */

				if (key == 'r') {  /* player wants to restart game */
					*isFail1 = false;  *isFail2 = false;  
					*score1 = 0;  *score2 = 0;
					*timer = temp;
					/* reset value of the table */
					reset_table(table1, y);  
					reset_table(table2, y);  
					clear();					
					init_2_table(table1, score1, isFail1, table2, score2, isFail2, y, timer);
					break;			
				}
			}			
		}				
	}
	
	*isFail1 = true; *isFail2 = true;
	free(table1); free(table2);
	clear();
	int row, col;
	getmaxyx(stdscr,row,col);  /* get the number of rows and columns */
	/* print good bye message */
 	mvprintw(row/2,(col-strlen("Good bye. See you again!"))/2,"%s", 
 		"Good bye. See you again!");	
	refresh();
	sleep(1);  /* display for a while before exit */
	exit(-1);
	pthread_exit(NULL);
}

void *count_down(void* param) {
	struct player *counter = (struct player*) param;
	int *table1 = counter->table1;
	int *score1 = &(counter->score1);
	int *y = &(counter->y);
	bool *isFail1 = &(counter->isFail1);	
	int *timer = &(counter->timer);

	int *table2 = counter->table2;
	int *score2 = &(counter->score2);
	bool *isFail2 = &(counter->isFail2);
	
	while(true) {		
		sleep(1);			
		if (*isFail1 == false && *isFail2 == false && *timer > 0) {
			*timer = *timer - 1;
			clear();
			print_2_table(table1, score1, isFail1, table2, score2, isFail2, y, timer);	
		} else {
			*isFail1 = true; *isFail2 = true;
			if (*timer == 0) {
				clear();
				print_2_table(table1, score1, isFail1, table2, score2, isFail2, y, timer);	
			}	
		}		
	}	
	pthread_exit(NULL);	
}

void *medium_AI(void* param) {
	/*This AI is only used in AI vs AI mode and displayed in the left table
	 *The strategy of this AI is turn-based phase 
	 *If first turn is up then next turn is left
	 *If it is stuck, move right
	 *If it is still stuck, move down
	 *The strategy aims for keeping high value in the top row   	  		
	 */

	struct player *mediumAI = (struct player*) param;	

	int turn = 0; // Turn to perform variable action
	int *y = &(mediumAI->y); // Length of column 			

	int *table = mediumAI->table1; // The array containing numbers of table   
	int *score = &(mediumAI->score1); // Score of the play		 
	bool *isFail = &(mediumAI->isFail1); // Boolean to check if game is failed
	bool *isStuck = &(mediumAI->isStuck1); // Boolean to check if there is slots to move
	bool *isMatch = &(mediumAI->isMatch1); // Boolean to check if there is matching pairs
	
	int *table_clone = malloc((*y) * (*y) * sizeof(int)); // A copy of array containing numbers of table   

	sleep(1);
	while (true) {
		//Check if any player is failed
		if ((mediumAI->isFail1) == false && (mediumAI->isFail2) == false) {						
			
			//Copy the array of table into the clone 
			memcpy(table_clone, table, (*y) * (*y) * sizeof(int));

			if (turn == 0) { //Turn 0, move up
				up(table, score, isStuck, isMatch, y);
				turn = 1;			
			} else if (turn == 1) { //Turn 1, move left
				left(table, score, isStuck, isMatch, y);
				turn = 0;
			}

			//If the table is stuck (nothing is move) after first try, move right 
			if (check_stuck(table, table_clone, (*y) * (*y)) == true) {
				right(table, score, isStuck, isMatch, y);
			}

			//If the table is stuck (nothing is move) after second try, move down 
			if (check_stuck(table, table_clone, (*y) * (*y)) == true) {
				down(table, score, isStuck, isMatch, y);
			} 

			check_failing(table, isFail, y);  // check if there's any available move left on the board.

			if ((mediumAI->inp) == 231 && (*isFail == true)) {
				print_2_table(table, score, isFail, 
				mediumAI->table2, &(mediumAI->score2), &(mediumAI->isFail2), y, &(mediumAI->timer));		
			}

			if (*isStuck == false || *isMatch == true) {
				add_value(table, isFail, y); //	Add new random value											
										
				clear(); // clear all UI elements displayed on the terminal
				sleep(1);
													
			} else {
				continue;
			}									
		} 		
	}	

	free(table);
	free(table_clone); 	
	pthread_exit(NULL);
}

void *smart_AI(void* param) {		
	/*This AI can be used in both single mode and 2 player mode
	 *This AI is displayed in the rigth table in 2 player mode 	  		
	 *The strategy is to find the best move in 1 turn and move base on the result  
	 */

	struct player *mediumAI = (struct player*) param;	

	int *y = &(mediumAI->y); // Length of column	
	int temp_choice = (mediumAI->inp); // The game choice
	int temp_time = (mediumAI->timer); 	
	//int temp_choice = *inp;
	 
   	int *table; // The array containing numbers of table   
	int *score; // Score of the play		 
	bool *isFail; // Boolean to check if game is failed
	bool *isStuck; // Boolean to check if there is slots to move
	bool *isMatch; // Boolean to check if there is matching pairs

	//The below value is used to calculate the best move
	int score_clone; // A copy of score
	bool isStuck_clone; // A copy of boolean isStruck
	bool isMatch_clone; // A copy of boolean isMatch
   	int *table_clone = malloc((*y) * (*y) * sizeof(int)); // A copy of array containing numbers of table   	 

	if (temp_choice == 12) { //If player choose 1-player mode
		//Get value of the left table		
		table = mediumAI->table1;
		score = &(mediumAI->score1);		
		isFail = &(mediumAI->isFail1);
		isStuck = &(mediumAI->isStuck1);
		isMatch = &(mediumAI->isMatch1);

		reset_table(mediumAI->table1, y);  
		reset_table(mediumAI->table2, y);
		init_table(table, score, isFail, y);

	} else { //If player choose 2-player mode
		//Get value of the right table
		table = mediumAI->table2;
		score = &(mediumAI->score2);		
		isFail = &(mediumAI->isFail2);
		isStuck = &(mediumAI->isStuck2);
		isMatch = &(mediumAI->isMatch2);

		if ((temp_choice == 232) || (temp_choice == 231)) {			
			reset_table(mediumAI->table1, y);  
			reset_table(mediumAI->table2, y);
			init_2_table(mediumAI->table1, &(mediumAI->score1), &(mediumAI->isFail1), 
				 table, score, isFail, y, &(mediumAI->timer));		
		}		
	}

	//Copy the value to the clone
	score_clone = *score;
	isStuck_clone = *isStuck;
	isMatch_clone = *isMatch;
   	memcpy(table_clone, table, (*y) * (*y) * sizeof(int));

	sleep(1);
	while (true) {			
		//Check if any player is failed
		if ((mediumAI->isFail1) == false && (mediumAI->isFail2) == false) {

			//Array containing the "predicted score" after moving each direction
			int predict_score[4] = {0,0,0,0};

			//Array containing adresses of function
			void (* sort_funcs[4])(int*, int*, bool*, bool*, int*) = {&down, &up, &left, &right};

			//Move in each direction to find out the best move
			for (int i = 0; i < 4; i++) {				

				//Moving using clone values
				(* sort_funcs[i])(table_clone, &score_clone, &isStuck_clone, &isMatch_clone, y);	
  
				predict_score[i] = score_clone; // Get the score after the move
				predict_score[i] += check_matchingPair(table_clone, y); // Get the number of matching pairs
				for (int j = 0; j < (*y) * (*y); j++) { 
					if (table_clone[j] == 0) {
						predict_score[i] += 1; //Get the number of free space
					}
				}

				//Copy the value into the clone again
				score_clone = *score;
				isStuck_clone = *isStuck;
				isMatch_clone = *isMatch;
			   	memcpy(table_clone, table, (*y) * (*y) * sizeof(int));						
			}

			//Find the index of best move in function arrays 
			int index = 0;		
			int largest = predict_score[0];			

			for (int i = 0; i < 4; i++) {
				if (largest < predict_score[i]) {
					largest = predict_score[i];
					index = i;
				}
			}

			//Move the table base on the best move, actual value is use
			(* sort_funcs[index])(table, score, isStuck, isMatch, y);


			//If the table is stuck (nothing is move)
			//The situation occurs when there is more than 1 same highest score
			if (check_stuck(table, table_clone, (*y) * (*y)) == true) {
				for (int i = 0; i < 4; i++) {
					(* sort_funcs[i])(table, score, isStuck, isMatch, y);

					if (check_stuck(table, table_clone, (*y) * (*y)) == false) {
						break;
					} else {
						continue;
					}							
				}

			}
			
			check_failing(table, isFail, y); // check if there's any available move left on the board.				

			if (*isStuck == false || *isMatch == true) {
				add_value(table, isFail, y); //	Add new random value											
										
				clear(); // clear all UI elements displayed on the terminal  	

				/* print the table again */ 
				if (temp_choice == 12) {
					print_table(table, score, isFail, y);
				} else {
					if ((temp_choice == 231) || (temp_choice == 2221))  {
						print_2_table(mediumAI->table1, &(mediumAI->score1), &(mediumAI->isFail1), 
							  table, score, isFail, y, &(mediumAI->timer));		
					}					
				}

				sleep(1);
								
			} else {
				continue;
			}									
		} else {
			if ((temp_choice == 12) || (temp_choice == 231) || (temp_choice == 232)) {
				int key = 0;
				while(true) {
					clear();
					print_2_table(mediumAI->table1, &(mediumAI->score1), &(mediumAI->isFail1), 
							  table, score, isFail, y, &(mediumAI->timer));		
					key = getch();  /* get input from keyboard */

					if (key == 'r') {  /* player wants to restart game */
						(mediumAI->isFail1) = false;  
						(mediumAI->isFail2) = false;  
						(mediumAI->score1) = 0;  
						(mediumAI->score2) = 0;
						(mediumAI->timer) = temp_time;
						reset_table(mediumAI->table1, y);  /* reset value of the table */
						reset_table(mediumAI->table2, y);  /* reset value of the table */
						clear();

						if (temp_choice == 12) {
							init_table(table, score, isFail, y);
						} else {
							init_2_table(mediumAI->table1, &(mediumAI->score1), &(mediumAI->isFail1), 
				 						table, score, isFail, y, &(mediumAI->timer));
						}
						 	
						break;		
					} else if (key == 27) {
						clear();
						int row, col;
						getmaxyx(stdscr,row,col);  /* get the number of rows and columns */
						/* print good bye message */
					 	mvprintw(row/2,(col-strlen("Good bye. See you again!"))/2,"%s", 
					 		"Good bye. See you again!");	
						refresh();
						sleep(1);  /* display for a while before exit */					
						free(table);
						free(table_clone);
						exit(-1);
						pthread_exit(NULL);
					}	
				}
				continue;
			} else {
				clear();
				print_2_table(mediumAI->table1, &(mediumAI->score1), &(mediumAI->isFail1), 
						  table, score, isFail, y, &(mediumAI->timer));		
				sleep(1);
			}
		}		
	}		
}

bool check_stuck(int *table, int *table_clone, int length) {
	bool stuck = true; // Boolean to check if the table is stuck

	//If the value of table is the same as the clone, the table is stuck
	for (int i = 0; i < length; i++) {
		if (table[i] != table_clone[i]) {
			stuck = false;
			break;
		}
	}

	return stuck;
}

int check_matchingPair(int *table_clone, int *y) {
	//Find the matching pairs available in the table

	int matchingPair = 0;// Intger to store number of pairs

	/* start from the bottom of table */
	for (int i = (*y * *y) - 1; i > (*y * *y) - (*y + 1); --i)	{  
		for (int j = 0; j < (*y * *y) - (*y - 1);) {					
			int temp = table_clone[i - j];

			if (temp != 0) {  /* if the key value is not 0 */
				bool check = false;

				for (int k = j + *y; k < (*y * *y) - (*y - 1); k += *y)	{  /* search through other rows of a column */
					if (table_clone[i - k] == temp) {  /* if the 2 values are match */
						matchingPair++;													
						break;																						

						// if there is a different number 
						// in front of the key value 							 
					} else if (table_clone[i - k] != temp && table_clone[i - k] != 0) {  
						break;
					}
				}

				// if there's no matching value with the key value 
				// in the same column					 
				if (check == false) {  
					j += *y;  /* change key value to the next row of a column */
				}
			} else {
				j += *y;  /* change key value to the next row of a column */
			}					
		}					
	}

	/* start from the top of table */
	for (int i = 0; i < *y; ++i)	{  
		for (int j = 0; j < (*y * *y) - (*y - 1);)	{					
			int temp = table_clone[i + j];

			if (temp != 0) {  /* if the key value is not 0 */
				bool check = false;

				/* search through other rows of a column */
				for (int k = j + *y; k < (*y * *y) - (*y - 1); k += *y)	{  
					if (table_clone[i + k] == temp) {  /* if the 2 values are match */
						matchingPair++;

						// if there is a different number 
						// in front of the key value 							
					} else if (table_clone[i + k] != temp && table_clone[i + k] != 0) {
						break;
					}
				}

				// if there's no matching value with the key value 
				// in the same column					
				if (check == false) {
					j += *y;  /* change key value to the next row of a column */
				}
			} else {
				j += *y;  /* change key value to the next row of a column */
			}					
		}					
	}

	/* start from the left of table */
	for (int i = *y - 1; i < (*y * *y); i += *y)	{  
		for (int j = 0; j < *y;)	{					
			int temp = table_clone[i - j];

			if (temp != 0) {  /* if the key value is not 0 */
				bool check = false;

				for (int k = j + 1; k < *y; ++k)	{  /* search through other columns of a row */
					if (table_clone[i - k] == temp) {  /* if the 2 values are match */
						matchingPair++;																						

						// if there is a different number 
						// in front of the key value 							
					} else if (table_clone[i - k] != temp && table_clone[i - k] != 0) {  
						break;
					}
				}

				// if there's no matching value with the key value 
				// in the same column					
				if (check == false) {  
					j++;  /* change key value to the next column of a row */
				}
			} else {
				j++;  /* change key value to the next column of a row */
			}					
		}					
	}

	/* start from the right of table */
	for (int i = 0; i < (*y * *y) - (*y - 1); i += *y)	{  
		for (int j = 0; j < *y;)	{					
			int temp = table_clone[i + j];

			if (temp != 0) {  /* if the key value is not 0 */
				bool check = false;

				for (int k = j + 1; k < *y; ++k)	{  /* search through other rows of a column */
					if (table_clone[i + k] == temp) {  /* if the 2 values are match */
						matchingPair++;

						// if there is a different number 
						// in front of the key value 							
					} else if (table_clone[i + k] != temp && table_clone[i + k] != 0) {
						break;
					}
				}

				// if there's no matching value with the key value 
				// in the same column					
				if (check == false) {
					j++;  /* change key value to the next row of a column */
				}
			} else {
				j++;  /* change key value to the next row of a column */
			}					
		}
	}

	return matchingPair;
}
