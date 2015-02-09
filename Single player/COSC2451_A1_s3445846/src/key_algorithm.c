/** @file key_algorithm.c
 * @brief This is the file to store the functions
 * to control the game board based on player's input.
 */

#include <stdbool.h>

/** @brief This function will change and re-arrange 
 * the values of the table when users press 
 * the "DOWN" button on the keyboard.
 * @param a the array containing the numbers of the game board
 * @param s the current score 
 * @param is boolean to check if the numbers are able to move down
 * @param im boolean to check if there is at least a matching pair of numbers
 * @param y colum length of game board
 * @return none
 */
void down(int *a, int *s, bool *is, bool *im, int *y) {
	bool isStuck = true;  /* check if the slot can be moved */
	bool isMatch = false;  /* check if there are matching slots */

	for (int i = (*y * *y) - 1; i > (*y * *y) - (*y + 1); --i)	{  /* start from the bottom of table */
		for (int j = 0; j < (*y * *y) - (*y - 1);) {					
			int temp = a[i - j];

			if (temp != 0) {  /* if the key value is not 0 */
				bool check = false;

				/* search through other rows of a column */
				for (int k = j + *y; k < (*y * *y) - (*y - 1); k += *y)	{  
					if (a[i - k] == temp) {  /* if the 2 values are match */
						a[i - k] = temp * 2;  /* combine them and double it */
						*s += temp * 2;  /* add to the total score */
						a[i - j] = 0;  /* change data of key value to 0 */
						j = k + *y;  // change key value to the row next to 
									// the matching value of a column.									
						check = true;	
						isMatch = true;																
						break;																						

						// if there is a different number 
						// in front of the key value 						 
					} else if (a[i - k] != temp && a[i - k] != 0) {  
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


	/* move all values down (to the bottom of the table) */
	/* start from the botton of the table */
	for (int i = (*y * *y) - 1; i > (*y * *y) - (*y + 1); --i)	{  
		for (int j = 0; j < (*y * *y) - (*y - 1); j += *y) {					
			if (a[i - j] == 0) {  /* key value is 0 */
				for (int k = j + *y; k < (*y * *y) - (*y - 1); k += *y) {
					if (a[i - k] != 0) {  // the value is not 0 in the same 
										  // column with the key value 										   
						a[i - j] = a[i - k];  /* change key value to the targeted value */
						a[i - k] = 0;  /* set targeted value to 0 */	
						isStuck = false;															   					
						break;  /* move to next row of the column */
					}
				}
			}
		}
	}

	*is = isStuck;		
	*im = isMatch;
}

/** @brief This function will change and re-arrange 
 * the values of the table when users press 
 * the "UP" button on the keyboard.
 * @param a the array containing the numbers of the game board
 * @param s the current score 
 * @param is boolean to check if the numbers are able to move up
 * @param im boolean to check if there is at least a matching pair of numbers
 * @param y colum length of game board
 * @return none
 */
void up(int *a, int *s, bool *is, bool *im, int *y) {
	bool isStuck = true;  /* check if the slot can be moved */
	bool isMatch = false;  /* check if there are matching slots */

	for (int i = 0; i < *y; ++i)	{  /* start from the top of table */
		for (int j = 0; j < (*y * *y) - (*y - 1);)	{					
			int temp = a[i + j];

			if (temp != 0) {  /* if the key value is not 0 */
				bool check = false;

				/* search through other rows of a column */
				for (int k = j + *y; k < (*y * *y) - (*y - 1); k += *y)	{  
					if (a[i + k] == temp) {  /* if the 2 values are match */
						a[i + k] = temp * 2;  /* combine them and double it */
						*s += temp * 2;  /* add to the total score */
						a[i + j] = 0;  /* change data of key value to 0 */
						j = k + *y;	// change key value to the row next to 
									// the matching value of a column 									 
						check = true;			
						isMatch = true;													
						break;

						// if there is a different number 
						// in front of the key value 						
					} else if (a[i + k] != temp && a[i + k] != 0) {
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


	// move all values up
	// (to the top of the table)	  
	for (int i = 0; i < *y; ++i) {  /* start from the top of table */
		for (int j = 0; j < (*y * *y) - (*y - 1); j += *y)	{					
			if (a[i + j] == 0) {  /* key value is 0 */
				for (int k = j + *y; k < (*y * *y) - (*y - 1); k += *y) {
					if (a[i + k] != 0) {  /** the value is not 0 in the same 
										   * column with the key value 
										   */
						a[i + j] = a[i + k];  /* change key value to the targeted value */
						a[i + k] = 0;  /* set targeted value to 0 */	
						isStuck = false;					
						break;  /* move to next row of the column */
					} 
				}
			}
		}
	}

	*is = isStuck;		
	*im = isMatch;
}

/** @brief This function will change and re-arrange 
 * the values of the table when users press 
 * the "RIGHT" button on the keyboard.
 * @param a the array containing the numbers of the game board
 * @param s the current score 
 * @param is boolean to check if the numbers are able to move right
 * @param im boolean to check if there is at least a matching pair of numbers
 * @param y colum length of game board
 * @return none
 */
void right(int *a, int *s, bool *is, bool *im, int *y) {
	bool isStuck = true;  /* check if the slot can be moved */
	bool isMatch = false;  /* check if there are matching slots */

	for (int i = *y - 1; i < (*y * *y); i += *y)	{  /* start from the right of table */
		for (int j = 0; j < *y;) {					
			int temp = a[i - j];

			if (temp != 0) {  /* if the key value is not 0 */
				bool check = false;

				for (int k = j + 1; k < *y; ++k)	{  /* search through other columns of a row */
					if (a[i - k] == temp) {  /* if the 2 values are match */
						a[i - k] = temp * 2;  /* combine them and double it */
						*s += temp * 2;  /* add to the total score */
						a[i - j] = 0;  /* change data of key value to 0 */
						j = k + 1;  // change key value to the row next to 
									// the matching value of a column 									
						check = true;	
						isMatch = true;
						break;																						

						// if there is a different number 
						// in front of the key value 						 
					} else if (a[i - k] != temp && a[i - k] != 0) {  
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


	// move all values to 
	// the right of the table	  
	for (int i = *y - 1; i < (*y * *y); i += *y) {  /* start from the right of the table */
		for (int j = 0; j < *y; j++) {
			if (a[i - j] == 0) {  /* key value is 0 */
				for (int k = j + 1; k < *y; k++) {
					if (a[i - k] != 0) {  // the value is not 0 in the same 
										  // row with the key value 										    
						a[i - j] = a[i - k];  /* change key value to the targeted value */
						a[i - k] = 0;  /* set targeted value to 0 */
					    isStuck = false;
						break;  /* move to next column of the row */
					}
				}
			}
		}
	}

	*is = isStuck;		
	*im = isMatch;
}

/** @brief This function will change and re-arrange 
 * the values of the table when users press 
 * the "LEFT" button on the keyboard.
 * @param a the array containing the numbers of the game board
 * @param s the current score 
 * @param is boolean to check if the numbers are able to move left
 * @param im boolean to check if there is at least a matching pair of numbers
 * @param y colum length of game board
 * @return none
 */
void left(int *a, int *s, bool *is, bool *im, int *y) {
	bool isStuck = true;  /* check if the slot can be moved */
	bool isMatch = false;  /* check if there are matching slots */

	for (int i = 0; i < (*y * *y) - (*y - 1); i += *y)	{  /* start from the right of table */
		for (int j = 0; j < *y;)	{					
			int temp = a[i + j];

			if (temp != 0) {  /* if the key value is not 0 */
				bool check = false;

				for (int k = j + 1; k < *y; ++k)	{  /* search through other rows of a column */
					if (a[i + k] == temp) {  /* if the 2 values are match */
						a[i + k] = temp * 2;  /* combine them and double it */
						*s += temp * 2;  /* add to the total score */
						a[i + j] = 0;  /* change data of key value to 0 */
						j = k + 1;	// change key value to the row next to 
									// the matching value of a column 									 
						check = true;	
						isMatch = true;															
						break;

						// if there is a different number 
						// in front of the key value 						 
					} else if (a[i + k] != temp && a[i + k] != 0) {
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


	// move all values up
	// (to the top of the table)	  
	/* start from the right of table */
	for (int i = 0; i < (*y * *y) - (*y - 1); i += *y)	{  
		for (int j = 0; j < *y; j++) {					
			if (a[i + j] == 0) {  /* key value is 0 */
				for (int k = j + 1; k < *y; k++) {
					if (a[i + k] != 0) {  // the value is not 0 in the same 
										  // column with the key value 										    
						a[i + j] = a[i + k];  /* change key value to the targeted value */
						a[i + k] = 0;  /* set targeted value to 0 */
						isStuck = false;
						break;  /* move to next row of the column */
					} 
				}
			}
		}
	}

	*is = isStuck;		
	*im = isMatch;
}

/** @brief This function checks if the game board is full
 * and there is no matching pairs left.
 * @param a the array containing the numbers of the game board
 * @param fail boolean to check if the game is over
 * @param y colum length of game board
 * @return none
 */
void check_failing(int *a, bool *fail, int *y) {  
	bool isFull = true;
	bool isFail = true;

	/* check if the game board is full */
	for (int i = 0; i < (*y * *y); ++i) {
		if (*(a + i) == 0) {
			isFull = false;
			break;
		}
	}

	/* check if there's matching values */
	if (isFull == true) {
		for (int i = (*y * *y) - 1; i > (*y * *y) - (*y + 1); --i)	{  /* start from the bottom of table */
			for (int j = 0; j < (*y * *y) - (*y - 1);) {					
				int temp = a[i - j];

				if (temp != 0) {  /* if the key value is not 0 */
					bool check = false;

					for (int k = j + *y; k < (*y * *y) - (*y - 1); k += *y)	{  /* search through other rows of a column */
						if (a[i - k] == temp) {  /* if the 2 values are match */
							isFail = false;														
							break;																						

							// if there is a different number 
							// in front of the key value 							 
						} else if (a[i - k] != temp && a[i - k] != 0) {  
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

		for (int i = 0; i < *y; ++i)	{  /* start from the top of table */
			for (int j = 0; j < (*y * *y) - (*y - 1);)	{					
				int temp = a[i + j];

				if (temp != 0) {  /* if the key value is not 0 */
					bool check = false;

					/* search through other rows of a column */
					for (int k = j + *y; k < (*y * *y) - (*y - 1); k += *y)	{  
						if (a[i + k] == temp) {  /* if the 2 values are match */
							isFail = false;												
							break;

							// if there is a different number 
							// in front of the key value 							
						} else if (a[i + k] != temp && a[i + k] != 0) {
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

		for (int i = *y - 1; i < (*y * *y); i += *y)	{  /* start from the left of table */
			for (int j = 0; j < *y;)	{					
				int temp = a[i - j];

				if (temp != 0) {  /* if the key value is not 0 */
					bool check = false;

					for (int k = j + 1; k < *y; ++k)	{  /* search through other columns of a row */
						if (a[i - k] == temp) {  /* if the 2 values are match */
							isFail = false;
							break;																						

							// if there is a different number 
							// in front of the key value 							
						} else if (a[i - k] != temp && a[i - k] != 0) {  
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

		for (int i = 0; i < (*y * *y) - (*y - 1); i += *y)	{  /* start from the right of table */
			for (int j = 0; j < *y;)	{					
				int temp = a[i + j];

				if (temp != 0) {  /* if the key value is not 0 */
					bool check = false;

					for (int k = j + 1; k < *y; ++k)	{  /* search through other rows of a column */
						if (a[i + k] == temp) {  /* if the 2 values are match */
							isFail = false;														
							break;

							// if there is a different number 
							// in front of the key value 							
						} else if (a[i + k] != temp && a[i + k] != 0) {
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
	} else {
		isFail = false;
	}

	*fail = isFail;
}