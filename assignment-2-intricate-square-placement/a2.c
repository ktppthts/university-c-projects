//----------------------------------------------------------------------------------------------------------------------
/// The program is called Intricate Square Placement and is very similar to the game Teeko. It is a PvP, two player,
/// 2D game where each player gets 4 pieces and has to put them on the board in a row to win. The win can be straight
/// along the diagonal or a 2x2 square.
///
/// Author: 12326821
//----------------------------------------------------------------------------------------------------------------------

#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include "framework.h"

#define SIZE 5
#define PLAYER_1 1
#define PLAYER_2 2
#define MAX_NUMBER_OF_MOVES 4
#define ALL_MOVES_USED 0

#define ERROR_UNKNOWN_COMMAND "\n[ERROR] Unknown command.\n\n"
#define ERROR_MUST_SHIFT "\n[ERROR] You must specify a piece to shift and a target field.\n\n"
#define ERROR_PLACE_ALL_PIECES "\n[ERROR] You must first place all your remaining pieces.\n\n"
#define ERROR_OUT_OF_BOUNDS "\n[ERROR] Coordinates not within board.\n\n"
#define ERROR_NO_SHIFTABLE_PIECE "\n[ERROR] No shiftable piece at coordinates.\n\n"
#define ERROR_FIELD_OCCUPIED "\n[ERROR] Field at coordinates occupied.\n\n"
#define ERROR_NOT_ADJACENT "\n[ERROR] Field at coordinates not adjacent to chosen piece.\n\n"
#define ERROR_WRONG_ARGUMENT "\n[ERROR] (Optional) color output argument can either be \"disabled\" or \"enabled\".\n\n"
#define ERROR_WRONG_USAGE "\n[ERROR] Usage: ./a2 [optional: color_enabled]\n\n"

typedef struct _Player_
{
  int move_number_;
  int remaining_pieces_;
  int index_;
}Player;

void printWelcomeMessage(void);
void makeLowerCase(char *argument);
void moveMessage(int current_player, Player *player1, Player *player2);
void remainingPiecesMessage(Player *player1, Player *player2);
void finalGameStateMessage(void);
int parseArguments(int argc, char *argv[], bool *color_status);
int turnTaking(bool color_status);
int placingPieces(int current_player, int board[SIZE][SIZE], bool color_status, Player *player1, Player *player2);
int isStriaghtWin(int current_player, int board[SIZE][SIZE]);
int isOppositeDiagonalWin(int current_player, int board[SIZE][SIZE]);
int isNormalDiagonalWin(int current_player, int board[SIZE][SIZE]);
int isSquareWin(int row_index, int column_index, int current_player, int board[SIZE][SIZE]);
int checkWinCondition(int current_player, int board[SIZE][SIZE]);
int getInput(char array[6]);
int checkValidity(char array[6], bool is_shifting_phase);
int isShiftAttempt(char array[6], bool is_shifting_phase);
int isOutOfBounds(char array[6], bool is_shifting_phase);
int isCurrentPlayersPiece(char array[6], bool is_shifting_phase, int board[SIZE][SIZE], int current_player);
int isOccupied(char array[6], bool is_shifting_phase, int board[SIZE][SIZE]);
int isAdjacent(char array[6], bool is_shifting_phase, int board[SIZE][SIZE]);
int isQuit(char array[6], bool had_space_after_index1, int *character);

//----------------------------------------------------------------------------------------------------------------------
/// @brief This is the main function of the program.
/// @param argc represents the number of arguments in the command line.
/// @param argv holds the words from the input in the command line.
/// @return the winner (1/2) or 0 if the program is exited.
int main(int argc, char *argv[])
{
  bool color_status = false;
  if (parseArguments(argc, argv, &color_status) == -1)
  {
    return -1;
  }
  printWelcomeMessage();
  int winner = turnTaking(color_status);
  return winner;
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief This function prints the welcome message.
void printWelcomeMessage(void)
{
  printf("===| Welcome to       |===\n"
         "===| [I]ntricate      |===\n"
         "===| [S]quare         |===\n"
         "===| [P]lacement      |===\n"
         "\n");
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief This function makes the letters of arguments from the command line lowercase.
/// @param argument represents argv, as in holds the words from the input in the command line.
void makeLowerCase(char *argument)
{
  for (int counter = 0; argument[counter] != '\0'; counter++)
  {
    argument[counter] = tolower(argument[counter]);
  }
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief This function handles the command line arguments and their error checks.
/// @param argc Represents the number of arguments in the command line.
/// @param argv Holds the words from the input in the command line.
/// @param color_status If user wants the colors enabled this parameter is true, if not it is false by default.
/// @return 1 if successful, -1 if there was an error, to the main function.
int parseArguments(int argc, char *argv[], bool *color_status)
{
  *color_status = false;
  if (argc == 2)
  {
    makeLowerCase(argv[1]);
    if (strcmp(argv[1], "enabled") == 0)
    {
      *color_status = true; 
      return 1;
    }
    else if (strcmp(argv[1], "disabled") == 0)
    { 
      *color_status = false; 
      return 1;
    }
    else
    {
      printf(ERROR_WRONG_ARGUMENT);
      return -1;
    }
  }
  else if (argc > 2)
  {
    printf(ERROR_WRONG_USAGE);
    return -1;
  }
  return 1;
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief This function prints the message when a player makes a move.
/// @param current_player The number of the player who's turn it is.
/// @param player1 Member of the struct Player, holds player 1's remaining pieces, moves and index.
/// @param player2 Member of the struct Player, holds player 2's remaining pieces, moves and index.
void moveMessage(int current_player, Player *player1, Player *player2)
{
  player1->index_ = 1;
  player2->index_ = 2;
  if (current_player == 1)
  {
    printf("\n"
          "===| Move %d: Player %d |===\n"
          "\n",player1->move_number_, player1->index_);
  }
  else if (current_player == 2)
  {
    printf("\n"
           "===| Move %d: Player %d |===\n"
           "\n",player2->move_number_, player2->index_);
  }
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief Prints the "Final Game State" message when someone wins.
void finalGameStateMessage(void)
{
  printf("\n===| Final Game State |===\n\n");
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief Prints the "Remaining Pieces" message every turn until all pieces are placed.
/// @param player1 Member of the struct Player, holds player 1's remaining pieces, moves and index.
/// @param player2 Member of the struct Player, holds player 2's remaining pieces, moves and index.
void remainingPiecesMessage(Player *player1, Player *player2)
{
  if (player1->move_number_ <= MAX_NUMBER_OF_MOVES || player2->move_number_ <= MAX_NUMBER_OF_MOVES)
  {
    printf("---| Remaining Pieces |---\n"
           "\n"
           "Player 1: %d\n"
           "Player 2: %d\n"
           "\n", player1->remaining_pieces_, player2->remaining_pieces_);
  }
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief Checks if a player won by having 4 in a row/column.
/// @param current_player Represents the ID of the player who's turn it is.
/// @param board The board which the game is played on, which holds all positions of pieces.
/// @return 0 if nobody won or current players ID if somebody did.
int isStriaghtWin(int current_player, int board[SIZE][SIZE])
{
  for (int row = 0; row < SIZE; row++) 
  {
    for (int col = 0; col <= SIZE - 4; col++) 
    {
      if (board[row][col] == current_player &&
          board[row][col + 1] == current_player &&
          board[row][col + 2] == current_player &&
          board[row][col + 3] == current_player)
      {
        return current_player;
      }
    }
  }
  for (int row = 0; row <= SIZE - 4; row++) 
  {
    for (int col = 0; col < SIZE; col++) 
    {
      if (board[row][col] == current_player &&
          board[row + 1][col] == current_player &&
          board[row + 2][col] == current_player &&
          board[row + 3][col] == current_player)
      {
        return current_player;
      }
    }
  }
  return 0;
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief Checks if a player won by having 4 in a diagonal (\).
/// @param current_player Represents the ID of the player who's turn it is.
/// @param board The board which the game is played on, which holds all positions of pieces.
/// @return 0 if nobody won or current players ID if somebody did.
int isOppositeDiagonalWin(int current_player, int board[SIZE][SIZE])
{
  for (int row = 0; row <= SIZE - 4; row++) 
  {
    for (int col = 3; col < SIZE; col++) 
    {
      if (board[row][col] == current_player &&
          board[row + 1][col - 1] == current_player &&
          board[row + 2][col - 2] == current_player &&
          board[row + 3][col - 3] == current_player)
      {
        return current_player;
      }
    }
  }
  return 0;
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief Checks if a player won by having 4 in a diagonal (/).
/// @param current_player Represents the ID of the player who's turn it is.
/// @param board The board which the game is played on, which holds all positions of pieces.
/// @return 0 if nobody won or current players ID if somebody did.
int isNormalDiagonalWin(int current_player, int board[SIZE][SIZE])
{
  for (int row = 0; row <= SIZE - 4; row++) 
  {
    for (int col = 0; col <= SIZE - 4; col++) 
    {
      if (board[row][col] == current_player &&
          board[row + 1][col + 1] == current_player &&
          board[row + 2][col + 2] == current_player &&
          board[row + 3][col + 3] == current_player)
      {
        return current_player;
      }
    }
  }
  return 0;
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief Checks if a player won by having 4 next to each other as a 2x2 square.
/// @param row_index The row index of the current player's pieces.
/// @param column_index The column index of the current player's pieces.
/// @param current_player Represents the ID of the player who's turn it is.
/// @param board The board which the game is played on, which holds all positions of pieces.
/// @return 0 if nobody won or current players ID if somebody did.
int isSquareWin(int row_index, int column_index, int current_player, int board[SIZE][SIZE])
{
  for (row_index = 0; row_index <= 3; row_index++)
  {
    for (column_index = 0; column_index <= 3; column_index++)
    {
      if (board[row_index][column_index] == current_player &&
          board[row_index][column_index + 1] == current_player &&
          board[row_index + 1][column_index] == current_player &&
          board[row_index + 1][column_index + 1] == current_player)
      {
        return current_player;
      }
    }
  }
  return 0;
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief This function checks if any of the win conditions are fullfiled.
/// @param current_player Represents the ID of the player who's turn it is.
/// @param board The board which the game is played on, which holds all positions of pieces.
/// @return -1 if nobody won or current players ID if somebody did.
int checkWinCondition(int current_player, int board[SIZE][SIZE])
{
  int row_index = 0;
  int column_index = 0;

  if (isStriaghtWin(current_player, board) == current_player || 
      isNormalDiagonalWin(current_player, board) == current_player ||
      isOppositeDiagonalWin(current_player, board) == current_player ||
      isSquareWin(row_index, column_index,  current_player, board) == current_player) 
  {
    return current_player;
  }
  else
  {
    return -1;
  }
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief This function check if the quit command was entered.
/// @param array Input is stored into this array in the form <col><row><col><row>
/// @param had_space_after_index1 Returns true if there was a space in between coordinates.
/// @param character Character the program reads from input.
/// @return 1 if successful, -1 if error was met, 0 if 'quit' was typed or EOF was used.
int isQuit(char array[6], bool had_space_after_index1, int *character)
{
  if (strcmp(array, "quit") == 0 && !had_space_after_index1)
  {
    if (*character == '\n' || *character == EOF) 
    {
      return 0;
    }
    while (isspace(*character)) 
    {
      *character = getchar();
      if (*character == '\n' || *character == EOF)
      {
        return 0;
      }
      else
      {
        return -1;
      }
    }
  }
  if (*character == EOF) 
  {
    return 0;
  }

  return 1;
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief This function gets the input and does some error checks immediately.
/// @param array Input is stored into this array in the form <col><row><col><row>
/// @return 1 if successful, -1/-2 if error was met, 0 if 'quit' was typed or EOF was used.
int getInput(char array[6])
{
  int index = 0;
  int character;
  int had_space_after_index1 = 0;
  
  while ((character = getchar()) != EOF && character != '\n')
  {
    character = tolower(character);

    if (((index == 1 || index == 3) && isspace(character)))
    {
      return -1;
    }

    while (isspace(character))
    {
      if (index == 2)
      {
        had_space_after_index1 = 1;
      }
      character = getchar();
      if (index == 0 && character == '\n') 
      {
        return -2;
      }
      if (index != 0 && character == '\n') 
      {
        return 1;
      }
    }

    array[index++] = tolower(character);
    if (index >= 5) 
    {
      return -1;
    }
  }

  int quit_return_value = isQuit(array, had_space_after_index1, &character);
  if (quit_return_value != 1)
  {
    return quit_return_value;
  }

  if (index == 4 && !had_space_after_index1)
  {
    return -2;
  }

  return 1;
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief This function checks if the input is valid.
/// @param array Input is stored into this array in the form <col><row><col><row>
/// @param is_shifting_phase True if the game has gone into shifting pieces phase and all pieces were placed.
/// @return 1 if successful, -1 if error was met.
int checkValidity(char array[6], bool is_shifting_phase)
{
  if (!isalpha(array[0]) || !isdigit(array[1]))
  {
    printf(ERROR_UNKNOWN_COMMAND);
    return -1;
  }

  if (!is_shifting_phase && (array[2] != 0 || array[3] != 0))
  {
    if ((!isdigit(array[3]) || !isalpha(array[2])))
    {
      printf(ERROR_UNKNOWN_COMMAND);
      return -1;
    }
  }
  else if (is_shifting_phase && array[2] == 0 && array[3] == 0) 
  {
    printf(ERROR_MUST_SHIFT);
    return -1;
  }
  else if (is_shifting_phase && (!isdigit(array[3]) || !isalpha(array[2])))
  {
    printf(ERROR_UNKNOWN_COMMAND);
    return -1;
  }
  return 1;
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief This function checks if the player attempted to shift pieces before placing them all.
/// @param array Input is stored into this array in the form <col><row><col><row>
/// @param is_shifting_phase True if the game has gone into shifting pieces phase and all pieces were placed.
/// @return 1 if successful, -1 if error was met.
int isShiftAttempt(char array[6], bool is_shifting_phase)
{
  if (array[2] != 0 && array[3] != 0 && !is_shifting_phase)
  {
    printf(ERROR_PLACE_ALL_PIECES);
    return -1;
  }
  return 1;
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief This function checks if the entered coordinates were out of bounds.
/// @param array Input is stored into this array in the form <col><row><col><row>
/// @param is_shifting_phase True if the game has gone into shifting pieces phase and all pieces were placed.
/// @return 1 if successful, -1 if error was met.
int isOutOfBounds(char array[6], bool is_shifting_phase)
{
  if (array[0] < 'a' || array[0] > 'e' ||
      array[1] < '1' || array[1] > '5')
    {
      printf(ERROR_OUT_OF_BOUNDS);
      return -1;
    }
  if (is_shifting_phase)
  {
    if (array[2] < 'a' || array[2] > 'e' ||
        array[3] < '1' || array[3] > '5')
    {
      printf(ERROR_OUT_OF_BOUNDS);
      return -1;
    }
  }
  return 1;
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief This function checks if the player is trying to move their own piece.
/// @param array Input is stored into this array in the form <col><row><col><row>.
/// @param is_shifting_phase True if the game has gone into shifting pieces phase and all pieces were placed.
/// @param current_player Represents the ID of the player who's turn it is.
/// @param board The board which the game is played on, which holds all positions of pieces.
/// @return 1 if successful, -1 if error was met.
int isCurrentPlayersPiece(char array[6], bool is_shifting_phase, int board[SIZE][SIZE], int current_player)
{
  if (is_shifting_phase)
  {
    int row = array[1] - '1';
    int column = array[0] - 'a';
    if (board[row][column] != current_player)
    {
      printf(ERROR_NO_SHIFTABLE_PIECE);
      return -1;
    }
  }
  return 1;
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief This function checks if the coordinate is occupied.
/// @param array Input is stored into this array in the form <col><row><col><row>.
/// @param current_player Represents the ID of the player who's turn it is.
/// @param board The board which the game is played on, which holds all positions of pieces.
/// @return 1 if successful, -1 if error was met.
int isOccupied(char array[6], bool is_shifting_phase, int board[SIZE][SIZE])
{
  int row, column;
  if (!is_shifting_phase)
  {
    row = array[1] - '1';
    column = array[0] - 'a'; 
    if (board[row][column] != 0)
    {
      printf(ERROR_FIELD_OCCUPIED);
      return -1;
    }
  }
  else if (is_shifting_phase)
  {
    row = array[3] - '1';
    column = array[2] - 'a'; 
    if (board[row][column] != 0)
    {
      printf(ERROR_FIELD_OCCUPIED);
      return -1;
    }
  }
  return 1;
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief This function checks if the coordinate the player is trying to shift to is adjacent to the coordinate
/// they're trying to shift from and if it is, places it.
/// @param array Input is stored into this array in the form <col><row><col><row>.
/// @param current_player Represents the ID of the player who's turn it is.
/// @param board The board which the game is played on, which holds all positions of pieces.
/// @return 1 if successful and placed, -1 if error was met.
int isAdjacent(char array[6], bool is_shifting_phase, int board[SIZE][SIZE])
{
  if (is_shifting_phase)
  {
    int from_column = array[0] - 'a';
    int from_row = array[1] - '1';
    int to_column = array[2] - 'a';
    int to_row = array[3] - '1';
    if ((to_column == from_column - 1 || to_column == from_column + 1 || to_column == from_column) &&
        (to_row == from_row - 1 || to_row == from_row + 1 || to_row == from_row ) && 
        !(to_column == from_column && to_row == from_row))
    {
      board[to_row][to_column] = board[from_row][from_column];
      board[from_row][from_column] = 0;
      return 1;
    }
    else
    {
      printf(ERROR_NOT_ADJACENT);
      return -1;
    }
  }
  return 1;
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief This function combines the functions which gets the input, check for errors, check for wins, prints the 
/// dialogue again and handles the amount of pieces and moves used.
/// @param current_player Represents the ID of the player who's turn it is.
/// @param board The board which the game is played on, which holds all positions of pieces.
/// @param color_status If user wants the colors enabled this parameter is true, if not it is false by default.
/// @param player1 Member of the struct Player, holds player 1's remaining pieces, moves and index.
/// @param player2 Member of the struct Player, holds player 2's remaining pieces, moves and index.
/// @return current player's id if someone won, 0 if program is exitted.
int placingPieces(int current_player, int board[SIZE][SIZE], bool color_status, Player *player1, Player *player2)
{
  while (1)
  {
    bool is_shifting_phase = false;
    printf("P%d > ", current_player);
    char array[6] = {0};
    int input_result = getInput(array);
    if (player1->remaining_pieces_ == ALL_MOVES_USED && player2->remaining_pieces_ == ALL_MOVES_USED)
    {
      is_shifting_phase = true;
    }
    if (input_result == -2)
    {
      printf(ERROR_UNKNOWN_COMMAND);
      continue;
    }
    else if (input_result == -1)
    {
      printf(ERROR_UNKNOWN_COMMAND);
      while (getchar() != '\n');
      continue;
    }
    else if (input_result == 0)
    {
      return 0;
    }

    if ((checkValidity(array, is_shifting_phase) == -1) ||
        (isShiftAttempt(array, is_shifting_phase) == -1) ||
        (isOutOfBounds(array, is_shifting_phase) == -1)  ||
        (isCurrentPlayersPiece(array, is_shifting_phase, board, current_player) == -1) ||
        (isOccupied(array, is_shifting_phase, board) == -1) ||
        (isAdjacent(array, is_shifting_phase, board) == -1))
    {
      continue;
    }
    if (!is_shifting_phase)
    {
      int row = array[1] - '1';
      int column = array[0] - 'a'; 
      board[row][column] = current_player;
    }
    if (checkWinCondition(current_player, board) == current_player)
    {
      finalGameStateMessage();
      printBoard(SIZE, board, color_status, PLAYER_1, PLAYER_2);
      printf("===| Winner: Player %d |===\n", current_player);
      return current_player;
    }
    if (current_player == 1)
    {
      current_player = 2;
      player1->move_number_++;
      if (player1->remaining_pieces_ > ALL_MOVES_USED)
        player1->remaining_pieces_--;
    }
    else
    {
      current_player = 1;
      player2->move_number_++;
      if (player2->remaining_pieces_ > ALL_MOVES_USED)
        player2->remaining_pieces_--;
    }
    moveMessage(current_player, player1, player2);
    printBoard(SIZE, board, color_status, PLAYER_1, PLAYER_2);
    if (player1->remaining_pieces_ > ALL_MOVES_USED || player2->remaining_pieces_ > ALL_MOVES_USED) 
      remainingPiecesMessage(player1, player2);
  }
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief This function sets the start values and starts the program.
/// @param color_status If user wants the colors enabled this parameter is true, if not it is false by default.
/// @return Returns the return value of the function placingPieces. (winner/quit/EOF)
int turnTaking(bool color_status)
{
  Player player1;
  Player player2;
  int board[SIZE][SIZE] = {0};
  int current_player = 1;

  player1.move_number_ = 1;
  player2.move_number_ = 1;

  player1.remaining_pieces_ = 4;
  player2.remaining_pieces_ = 4;

  moveMessage(current_player, &player1, &player2);
  printBoard(SIZE, board, color_status, PLAYER_1, PLAYER_2);
  remainingPiecesMessage(&player1, &player2);
  int return_value = placingPieces(current_player, board, color_status, &player1, &player2);
  return return_value;
}

