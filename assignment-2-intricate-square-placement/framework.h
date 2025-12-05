//----------------------------------------------------------------------------------------------------------------------
/// Header file for the framework for assignment 2 in the course ISP.
/// You may use this framework to reduce the workload in your second assignment.
///
/// Author: Tutors
//----------------------------------------------------------------------------------------------------------------------

// Header Guard
#ifndef FRAMEWORK_H
#define FRAMEWORK_H

// Includes
#include <stdbool.h>

//----------------------------------------------------------------------------------------------------------------------
/// @brief Prints the current state of the game board
/// @param size The size of the board
/// @param board A 2D integer array representing the game board
/// @param color_status A boolean indicating whether color output is enabled
/// @param player_1 The integer value representing Player 1's pieces on the board
/// @param player_2 The integer value representing Player 2's pieces on the board
void printBoard(int size, int board[size][size], bool color_status, int player_1, int player_2);

#endif // FRAMEWORK_H
