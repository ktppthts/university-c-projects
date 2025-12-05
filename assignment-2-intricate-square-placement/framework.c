//----------------------------------------------------------------------------------------------------------------------
/// Framework for assignment 2 in the course ISP.
/// You may use this framework to reduce the workload in your second assignment.
///
/// Author: Tutors
//----------------------------------------------------------------------------------------------------------------------

#include "framework.h"

#include <stdio.h>

#define UNICODE_MIDDLE_DOT "\u00b7"
#define UNICODE_SQUARE_FILLED "\u25a0"
#define UNICODE_SQUARE_EMPTY "\u25a1"

#define PIECE_PLAYER_ONE UNICODE_SQUARE_FILLED
#define PIECE_PLAYER_TWO UNICODE_SQUARE_EMPTY
#define COLORED_PIECE UNICODE_SQUARE_FILLED

#define ANSI_COLOR_RESET "\x1b[0m"
#define ANSI_COLOR_RED "\x1b[91m"
#define ANSI_COLOR_YELLOW "\x1b[93m"

#define COLOR_PLAYER_ONE ANSI_COLOR_RED
#define COLOR_PLAYER_TWO ANSI_COLOR_YELLOW

void printBoard(int size, int board[size][size], bool color_status, int player_1, int player_2)
{
  printf("   ");

  for (int letter = 0; letter < size; letter++)
  {
    printf("  %c", 'A' + letter);
  }

  printf("\n");

  printf("   + ");

  for (int dash = 0; dash < 3 * size - 2; dash++)
  {
    printf("-");
  }

  printf("\n");

  for (int row = 0; row < size; row++)
  {
    printf(" %d |", row + 1);

    for (int col = 0; col < size; col++)
    {
      if (col != 0)
        printf(" ");

      if (board[row][col] == player_1)
      {
        if (color_status == true)
          printf(" " COLOR_PLAYER_ONE COLORED_PIECE ANSI_COLOR_RESET);
        else
          printf(" " PIECE_PLAYER_ONE);
      }
      else if (board[row][col] == player_2)
      {
        if (color_status == true)
          printf(" " COLOR_PLAYER_TWO COLORED_PIECE ANSI_COLOR_RESET);
        else
          printf(" " PIECE_PLAYER_TWO);
      }
      else
      {
        printf(" " UNICODE_MIDDLE_DOT);
      }
    }

    printf("\n");
  }

  printf("\n");
}
