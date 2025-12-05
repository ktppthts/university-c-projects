# Assignment 2: Intricate Square Placement

A C-based strategy game implementation inspired by Teeko, featuring a 5×5 board where two players compete to form winning patterns through strategic piece placement and movement.

## Features
Core Gameplay

-Two-phase game mechanics: Initial piece placement followed by strategic movement
5×5 game board with coordinate-based positioning (A-E columns, 1-5 rows)
-Multiple winning conditions: Four in a row (any direction) or 2×2 square formation
-Turn-based system with move counter tracking per player

Technical Features

-Command-line argument parsing for optional color mode
-Input validation and error handling for all user inputs
-Case-insensitive command processing
-EOF handling for graceful program termination
-Arbitrary-length input handling without crashes

Visual Options

-Standard mode: Text-based board with Unicode symbols (■ for Player 1, □ for Player 2)
-Color mode: Enhanced visualization with colored pieces (red for Player 1, yellow for Player 2)
-Real-time board updates after each move
-Remaining pieces tracker during placement phase

Game Rules
Phase 1: Placing Pieces (Moves 1-4 per player)

-Each player places their 4 pieces on any empty field
-Input format: <column><row> (e.g., b2, d4)
-Pieces cannot be moved until all 8 pieces are on the board

Phase 2: Shifting Pieces (Move 5 onwards)

-Players move one piece at a time to an adjacent empty field
-Input format: <from_column><from_row> <to_column><to_row> (e.g., b2 c3)
-Adjacent means directly next to the piece (horizontally, vertically, or diagonally)


## Compilation and Execution
```bash
# Compile
clang -o a2 a2.c framework.c

# Run without color
./a2

# Run with color disabled (explicit)
./a2 disabled

# Run with color enabled
./a2 enabled

```

## Standard Gameplay Loop
===| Welcome to       |===
===| [I]ntricate      |===
===| [S]quare         |===
===| [P]lacement      |===


===| Move 1: Player 1 |===

     A  B  C  D  E
   + -------------
 1 | ·  ·  ·  ·  ·
 2 | ·  ·  ·  ·  ·
 3 | ·  ·  ·  ·  ·
 4 | ·  ·  ·  ·  ·
 5 | ·  ·  ·  ·  ·

---| Remaining Pieces |---

Player 1: 4
Player 2: 4

P1 > b1

===| Move 1: Player 2 |===

     A  B  C  D  E
   + -------------
 1 | ·  ■  ·  ·  ·
 2 | ·  ·  ·  ·  ·
 3 | ·  ·  ·  ·  ·
 4 | ·  ·  ·  ·  ·
 5 | ·  ·  ·  ·  ·

---| Remaining Pieces |---

Player 1: 3
Player 2: 4

P2 > d5

===| Move 2: Player 1 |===

     A  B  C  D  E
   + -------------
 1 | ·  ■  ·  ·  ·
 2 | ·  ·  ·  ·  ·
 3 | ·  ·  ·  ·  ·
 4 | ·  ·  ·  ·  ·
 5 | ·  ·  ·  □  ·

---| Remaining Pieces |---

Player 1: 3
Player 2: 3

P1 > C2

===| Move 2: Player 2 |===

     A  B  C  D  E
   + -------------
 1 | ·  ■  ·  ·  ·
 2 | ·  ·  ■  ·  ·
 3 | ·  ·  ·  ·  ·
 4 | ·  ·  ·  ·  ·
 5 | ·  ·  ·  □  ·

---| Remaining Pieces |---

Player 1: 2
Player 2: 3

P2 > D4

===| Move 3: Player 1 |===

     A  B  C  D  E
   + -------------
 1 | ·  ■  ·  ·  ·
 2 | ·  ·  ■  ·  ·
 3 | ·  ·  ·  ·  ·
 4 | ·  ·  ·  □  ·
 5 | ·  ·  ·  □  ·

---| Remaining Pieces |---

Player 1: 2
Player 2: 2

P1 > b3

===| Move 3: Player 2 |===

     A  B  C  D  E
   + -------------
 1 | ·  ■  ·  ·  ·
 2 | ·  ·  ■  ·  ·
 3 | ·  ■  ·  ·  ·
 4 | ·  ·  ·  □  ·
 5 | ·  ·  ·  □  ·

---| Remaining Pieces |---

Player 1: 1
Player 2: 2

P2 > B4

===| Move 4: Player 1 |===

     A  B  C  D  E
   + -------------
 1 | ·  ■  ·  ·  ·
 2 | ·  ·  ■  ·  ·
 3 | ·  ■  ·  ·  ·
 4 | ·  □  ·  □  ·
 5 | ·  ·  ·  □  ·

---| Remaining Pieces |---

Player 1: 1
Player 2: 1

P1 > C4

===| Move 4: Player 2 |===

     A  B  C  D  E
   + -------------
 1 | ·  ■  ·  ·  ·
 2 | ·  ·  ■  ·  ·
 3 | ·  ■  ·  ·  ·
 4 | ·  □  ■  □  ·
 5 | ·  ·  ·  □  ·

---| Remaining Pieces |---

Player 1: 0
Player 2: 1

P2 > c5

===| Move 5: Player 1 |===

     A  B  C  D  E
   + -------------
 1 | ·  ■  ·  ·  ·
 2 | ·  ·  ■  ·  ·
 3 | ·  ■  ·  ·  ·
 4 | ·  □  ■  □  ·
 5 | ·  ·  □  □  ·

P1 > b1 B2

===| Move 5: Player 2 |===

     A  B  C  D  E
   + -------------
 1 | ·  ·  ·  ·  ·
 2 | ·  ■  ■  ·  ·
 3 | ·  ■  ·  ·  ·
 4 | ·  □  ■  □  ·
 5 | ·  ·  □  □  ·

P2 > d4 C3

===| Move 6: Player 1 |===

     A  B  C  D  E
   + -------------
 1 | ·  ·  ·  ·  ·
 2 | ·  ■  ■  ·  ·
 3 | ·  ■  □  ·  ·
 4 | ·  □  ■  ·  ·
 5 | ·  ·  □  □  ·

P1 > B3 a2

===| Move 6: Player 2 |===

     A  B  C  D  E
   + -------------
 1 | ·  ·  ·  ·  ·
 2 | ■  ■  ■  ·  ·
 3 | ·  ·  □  ·  ·
 4 | ·  □  ■  ·  ·
 5 | ·  ·  □  □  ·

P2 > c3 d4

===| Move 7: Player 1 |===

     A  B  C  D  E
   + -------------
 1 | ·  ·  ·  ·  ·
 2 | ■  ■  ■  ·  ·
 3 | ·  ·  ·  ·  ·
 4 | ·  □  ■  □  ·
 5 | ·  ·  □  □  ·

P1 > c4 d3

===| Move 7: Player 2 |===

     A  B  C  D  E
   + -------------
 1 | ·  ·  ·  ·  ·
 2 | ■  ■  ■  ·  ·
 3 | ·  ·  ·  ■  ·
 4 | ·  □  ·  □  ·
 5 | ·  ·  □  □  ·

P2 > b4 c4

===| Final Game State |===

     A  B  C  D  E
   + -------------
 1 | ·  ·  ·  ·  ·
 2 | ■  ■  ■  ·  ·
 3 | ·  ·  ·  ■  ·
 4 | ·  ·  □  □  ·
 5 | ·  ·  □  □  ·

===| Winner: Player 2 |===

