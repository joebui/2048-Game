/** @file score.c
 * @brief This file get player's information and store
 * in the text file.
 */

#include <ncurses.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include "score.h"

/** @brief Write the current score of player into file
 * @param score current score  
 * @return none
 */
void store_score(int *score) {
    FILE *f = fopen("score.sav", "a");  /* Open file to write (append mode)*/

    char name[6]; /* The name of user */
    get_name(name); /* Ask player to input name */

    /* Write player name, score, and playtime*/
    fprintf(f, "%s %d %d-%d-%d\n", name, *score, get_time(0), get_time(1), get_time(2));      

    fclose(f); /* Close file */
    
    sort_score(); /* Soft scores in descending order*/
}

/** @brief Soft scores in descending order
 * @return none
 */
void sort_score() {
        
    FILE *f = fopen("score.sav", "r"); /* Open file to read data */
    
    int counter = 0; /* Number of line (scores) in save file */
    int score[11]; /* Array of scores */
    char data[11][30]; /* Array of string (name, score, date) */ 

    /* Get number of line in file */
    for (int i = 0; i < 11; i++) {
        char test[30];
        if (fgets(test, 30, f) == NULL) { /* If end of file, break the loop */
            break;
        } else { /* If not end of file, increase number of line */
            counter++;                            
        }
    } 

    rewind(f); /* Go back to the top of file */

    for (int i = 0; i < counter; i++) { /* Get the score from file and store in array */
        char name[6];
        char date[10];
        fscanf(f,"%s %u %s\n", name, &score[i], date); /* Scan the file for score */
    }

    rewind(f); /* Go back to the top of file */

    for (int i = 0; i < counter; i++) { /* Get the string (name, score, date) from file and store in array */
        fgets(data[i], 30, f); /* Scan the file for string */
    }
    
    /* Insertion Sort
     * Soft array of scores in descending order 
     * Soft array of data depend on the result of scores
     */
    for (int i = 1; i < counter; i++) {
        int key = score[i]; /* Take the next value in array */
        int position = i;

        char holder[30]; 
        strncpy(holder, data[i], 30);

        /* Move all values greater that greater than current value to the right */ 
        while (position > 0 && score[position-1] < key) { 
            score[position] = score[position-1];
            strncpy(data[position], data[position -1], 30);
            position--;
        }

        /* Move the current value to the empty postion on the left */
        score[position] = key;
        strncpy(data[position], holder, 30);
    }

    fclose(f); /* Close the file */

    f = fopen("score.sav", "w"); /* Open the file to write (overwrite mode) */

    /* If the number of line is greater than 10 (more than 10 scores)
     * change the counter to 10 (10 scores)
     */
    if (counter > 10) { 
        counter = 10;
    }

    /* Print top 10 scores in the data arrays to file */
    for (int i = 0; i < counter; i++) {                 
        fprintf(f, "%s", data[i]);
    }
    
    fclose(f); /* Close the file */
}

/** @brief Ask player to input name
 * @param name the name of player     
 * @return none
 */
void get_name(char *name) {
    char mess[] = "Enter name (5 letters): ";
    
    clear(); /* Clear elements on the UI*/
    
    int maxx, maxy; /* Size of screen */
    getmaxyx(stdscr, maxy, maxx); /* Get size of screen */
    mvprintw(maxy/2, (maxx-strlen(mess))/2, "%s", mess); /* Print message in the center of screen */
    refresh(); /* Refresh the UI */

    fgets(name, 6, stdin); /* Ask player to input name, limited to 5 letters */
}  

/** @brief Get playtime of player
 * @param choice the option to get date, month, or year    
 * @return none
 */
int get_time(int choice) {

    time_t t = time(NULL); /* Get the current time */
    struct tm tm = *localtime(&t); /* Get the current time in structure */

    switch(choice) {
        case 0: 
            return tm.tm_mday; /* Get date */
            break;
        case 1: 
            return tm.tm_mon + 1; /* Get month */
            break;
        case 2: 
            return tm.tm_year + 1900; /* Get year */
            break;
    }
    return -1; 
}

