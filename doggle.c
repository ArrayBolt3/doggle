/* ---------------------------------------------------------------------------
 * |Doggle - Boggle for DOS                                                  |
 * |Copyright (c) 2023 Aaron Rainbolt. Licensed under the MIT License.       |
 * ---------------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <ctype.h>
#include <time.h>
#include <string.h>
#include <i86.h>

/* all of the letters in a box of Boggle dice */
char dice[16][6] = { { 'R', 'H', 'Z', 'L', 'N', 'N' },
                     { 'G', 'E', 'W', 'N', 'E', 'H' },
                     { 'B', 'O', 'O', 'J', 'B', 'A' },
                     { 'F', 'P', 'S', 'A', 'K', 'F' },
                     { 'S', 'T', 'I', 'T', 'D', 'Y' },
                     { 'X', 'R', 'D', 'I', 'E', 'L' },
                     { 'H', 'N', 'U', 'I', 'M', '@' }, /* @ = Qu */
                     { 'R', 'E', 'T', 'W', 'V', 'H' },
                     { 'E', 'G', 'N', 'E', 'A', 'A' },
                     { 'Y', 'T', 'L', 'E', 'T', 'R' },
                     { 'S', 'H', 'A', 'P', 'C', 'O' },
                     { 'O', 'T', 'W', 'O', 'T', 'A' },
                     { 'I', 'E', 'N', 'U', 'S', 'E' },
                     { 'I', 'O', 'T', 'M', 'U', 'C' },
                     { 'E', 'Y', 'L', 'D', 'V', 'R' },
                     { 'T', 'O', 'E', 'S', 'S', 'I' } };

/* Searches for an integer in an int array.
 * Returns the index of the first matching element if found, or -1 if the
 * element was not found. */
int intidx(int *arr, int arrLen, int idx) {
  int i;
  for (i = 0;i < arrLen;i++) {
    if (arr[i] == idx) {
      return idx;
    }
  }
  return -1;
}

/* Returns a pseudorandom integer less than upperBound. This function avoids
 * modulo bias. */
int randLt(int upperBound) {
  int val;
  int rawUpperBound = 32767 - (32767 % upperBound);
  while (1) {
    val = rand();
    if (val <= rawUpperBound) {
      val = val % upperBound;
      return val;
    }
  }
}

/* Unsurprisingly, prints a help message. */
void printhelp() {
  printf("Doggle is a Boggle simulator for DOS.\n");
  printf("Press N to generate a new Boggle board.\n");
  printf("Press D to search through the dictionary.\n");
  printf("Press X to exit.\n");
  printf("Press H to display this help message.\n\n");
  printf("If you accept a Boggle board, and decide that was a bad idea, you can cancel\n");
  printf("the current round by pressing X during the game.\n");
}

/* Generates a new Boggle board and allows it to be played or discarded. */
void newboard() {
  char board[16];
  int remainingDice[16] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
  int selectedLetter;
  char *letterLine;
  char input;
  int i;
  int j;

  /* Generate the board - we randomly select each die **only once**, then
  randomly select a character from each die.*/

  for (i = 16;i >= 1;i--) {
    int selectedDie = randLt(i);
    selectedLetter = randLt(6);
    board[i - 1] = dice[remainingDice[selectedDie]][selectedLetter];
    remainingDice[selectedDie] = remainingDice[i - 1];
  }

  /* Print the board. */

  for (i = 0;i < 4;i++) {
    char currentChar;
    printf("+-----+-----+-----+-----+\n");
    printf("|     |     |     |     |\n");

    /* each line of the Boggle board is 27 characters long */
    letterLine = malloc(27 * sizeof(char));
    strcpy(letterLine, "|     |     |     |     |\n");
    for (j = 0;j < 4;j++) {
      currentChar = board[(i * 4) + j];
      if (currentChar != '@') {
        letterLine[(j * 6) + 3] = currentChar;
      } else {
        letterLine[(j * 6) + 3] = 'Q';
        letterLine[(j * 6) + 4] = 'u';
      }
    }
    printf(letterLine);
    printf("|     |     |     |     |\n");
  }

  printf("+-----+-----+-----+-----+\n"); /* The bottom side of the board. */

  /* Allow the user to reject or accept the board. */

  while (1) {
    printf("Accept board? Y/N "); fflush(stdout);
    input = getche();
    printf("\n");
    input = toupper(input);

    if (input == 'Y') {
      time_t start;
      time_t now;

      /* Start the timer, allowing the user to terminate the game early with
      the X key. */
      start = time(NULL);

      while (1) {
        delay(250);
        now = time(NULL);

        if (now - start == 180) { /* Three minutes has passed! */
          sound(2500);
          delay(1000);
          nosound();
          break;
        }

        if (kbhit()) {
          input = getch();
          input = toupper(input);
          if (input == 'X') {
            printf("Game stopped.\n");
            break;
          }
        }
      }

      break;
    } else if (input == 'N') {
      break;
    }

    free(letterLine);
  }
}

/* Allows the user to determine if a particular word exists in Doggle's
 * dict.txt file. */
void dictionary() {
  char *input = NULL;
  char *dictWord = NULL;
  size_t inputSize = 0;
  size_t dictWordSize = 0;
  FILE *dict = fopen("dict.txt", "r");
  int wordFound = 0;
  int i;

  if (dict == NULL) {
    printf("The dict.txt file could not be opened. The dictionary feature cannot function\n");
    printf("without it.\n");
    return;
  }

  printf("Type a word and press Enter to see if it exists in the dictionary.\n");
  printf("Type \"x\" and press Enter to exit.\n");

  while (1) {
    printf("Doggle:\\dict\\>"); fflush(stdout);
    getline(&input, &inputSize, stdin);
    
    for(i = 0;i < inputSize;i++) {
      if (input[i] == '\0') {
        break;
      }
      input[i] = tolower(input[i]);
    }

    if (strcmp(input, "x\n") == 0) {
      break;
    }
    
    while (getline(&dictWord, &dictWordSize, dict) != -1) {
      if(strcmp(input, dictWord) == 0) {
        printf("Found word in dictionary!\n");
        wordFound = 1;
        break;
      }
    }

    if (wordFound == 0) {
      printf("Did not find word in dictionary.\n");
    }

    wordFound = 0;
    fseek(dict, 0, SEEK_SET);
  }

  fclose(dict);
  free(input);
  free(dictWord);
}

int main(int argc, char** argv) {
  char input;

  srand((int)time(NULL));
  printf("Doggle - Boggle for DOS\n");
  printf("Doggle is in no way associated with Hasbro, the owners of the Boggle trademark.\n");
  printf("Press H for help\n\n");
  
  while (1) {
    printf("Doggle:\\> "); fflush(stdout);
    input = getche();
    printf("\n");
    input = toupper(input);

    switch (input) {
      case 'H':
        printhelp();
        break;
      case 'N':
        newboard();
        break;
      case 'D':
        dictionary();
        break;
      case 'X':
        return 0;
      default:
        printf("Illegal command: %c. Press H for help.\n", input);
        break;
    }
  }
}
