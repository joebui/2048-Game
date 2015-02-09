/** @file menu.c
 * @brief This file contains the functions to 
 * display and control player input for the main menu.
 */

#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "menu.h"

/** @brief This function controls player's input.
 * @return none 
 */
void print_menu() {
	initscr();  /* start ncurses */
	cbreak();  /* take input chars one at a time, no wait for \n */
	keypad(stdscr, TRUE);  /* enable keyboard mapping */	
	noecho();  /* do not echo input */	
	refresh();  /* refresh terminal's UI */			
	
	int choice = 0;	/* User chosen option */
	
	print_option(0); /* Print options */

	while(1) {
		int input = getch();

		if (input == KEY_DOWN) { /* Move selection marks down */
			choice++;
			if (choice > 3) {
				choice = 0;
			}
			print_option(choice);	
		} else if (input == KEY_UP) {  /* Move selection marks up */
			choice--;
			if (choice < 0) {
				choice = 3;
			}
			print_option(choice);
		}		
		
		if (input == 10) {  /* press "ENTER" button */	

			if (choice == 0) { /* Start game */
				clear();			
				break;	
			} else if (choice == 1) { /* Print scores */	
				clear();	
				print_score(0);
				break;
			} else if (choice == 2) { /* Print credit */
				clear();
				print_credit();
				break;
			} else if (choice == 3) { /* Quit game */
				clear();
				quit_game();
				break;									
			}
		}		
	}
}

/** @brief This function prints the options for player to choose. 
 * @param choice current position of the option in the menu
 * @return none
 */
void print_option(int choice) {
	
	char newGame[] = "New Game";
	char highScore[] = "High Score";
	char credits[] = "Credits";
	char quit[] = "Quit";

	clear();
	print_mess(3, newGame);	/* Print text in the center of screen */
	print_mess(2, highScore);
	print_mess(1, credits);
	print_mess(0, quit);		
	
	if (choice == 0) { /* Print selection marks next to the elected option */
		print_selection(3, newGame); 
	} else if (choice == 1) {
		print_selection(2, highScore);
	} else if (choice == 2) {
		print_selection(1, credits);
	} else if (choice == 3) {
		print_selection(0, quit);
	}

	refresh();			
}

/** @brief Print the Top 10 scores
 * @param current_screen current situation get the function is called
 * @return none
 */
void print_score(int current_screen) {

    FILE *f = fopen("score.sav", "r"); /* Open file to read data */  

    if (!f) { 
        print_mess(0, "Data is not existed!"); /* File is not existed */        
    } else {    	
    	int maxx, maxy;
	    getmaxyx(stdscr, maxy, maxx); /* Get size of screen */

	    clear();
	    for (int i = 0; i < 10; i++) { /* Print 10 scores */       
	        
	        if (feof(f)) { /* If there is not enough scores, print empty line*/
	        	mvprintw(maxy/4 + i, maxx/3, "%2u.", i + 1); /* Print in the center of screen */ 

	        } else {
	        	char name[10];
	        	int score;
	        	char date[20];
	        	fscanf(f,"%s %u %s\n", name, &score, date); /* Scan for data */        	
	        	mvprintw(maxy/4 + i, maxx/3, "%2u. %s\t%u\t%s", i + 1, name, score, date); /* Print scores in the center of screen */       	        	
	        }       
	    }	    	
	    fclose(f);
    }
               
    print_mess(9, "-- Top 10 Score --"); /* Print text in the center of screen */
	print_mess(-11, "*Press Enter to close"); 
	refresh(); 

	/* Press Enter to return to main menu*/
	while(1) { 
		int input = getch();

		if (input == 10) {
			if (current_screen == 0) {
				print_menu();
			}

			clear();	
			break;
		}
	}    
}		

/** @brief This function prints the game credit. 
 * @return none
 */
void print_credit() {
			
	char mess[] = "This game was created by";
	char student1[] = "Bui Quang Dien - s3445846";
	char student2[] = "Truong Nguyen Quoc Dat - s3412243";
	char note[] = "*Press Enter to return to Main Menu";

	print_mess(3, mess); /* Print text in the center of screen */
	print_mess(2, student1); 
	print_mess(1, student2); 
	print_mess(-11, note); 

	/* Press Enter to return to main menu*/
	while(1) {
		int input = getch();

		if (input == 10) {
			clear();
			print_menu();	
			break;
		}
	}	
}

/** @brief This function handle game exit. 
 * @return none
 */
void quit_game() {
			
	char mess[] = "Are you sure you want to exit? (Y/N)";
	print_mess(0, mess); /* Print text in the center of screen */

	while(1) {
		int input = getch();

		if (input == 'y') { /* Press 'y', exit game */
			struct timespec delay = {0, 999999999L}, rem;

			clear();
			int row, col;
			getmaxyx(stdscr,row,col);  /* get the number of rows and columns */
			/* print good bye message */
		 	mvprintw(row/2,(col-strlen("Good bye. See you again!"))/2,"%s", 
		 		"Good bye. See you again!");	
			refresh();
			nanosleep(&delay, &rem);  /* display for a while before exit */

			endwin();
			exit(1);
			break;					
		} else if (input == 'n') { /* Press 'n', return to main menu */
			clear();
			print_menu();			
			break;
		}
	}	
}

/** @brief This function prints the marking symbol showing
 * the current option player is on. 
 * @param num define the position of the option
 * @param choice chosen option
 * @return none 
 */
void print_selection(int num, char choice[]) {
	int maxx, maxy;
	getmaxyx(stdscr, maxy, maxx);
	
	mvprintw(maxy/2 - num, (maxx-strlen(choice))/2 - 2, "%s", ">"); /* Print ">" before text */
	mvprintw(maxy/2 - num, (maxx+strlen(choice))/2 + 1, "%s", "<");	/* Print "<" after text */
}

/** @brief This function prints all available text in the menu.
 * @param num define the position of the option
 * @param mess array to store all available texts
 * @return none 
 */
void print_mess(int num, char mess[]) {
	int maxx, maxy;
	getmaxyx(stdscr, maxy, maxx);

	mvprintw(maxy/2 - num, (maxx-strlen(mess))/2, "%s", mess);	/* Print text */
	refresh();	
}
