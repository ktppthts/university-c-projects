# Assignment 2: Intricate Square Placement

A C program simulating a two-player strategy game where players compete to arrange their pieces in winning patterns on a 5×5 board.

## Features

- Two-phase gameplay: piece placement followed by strategic movement
- 5×5 game board with coordinate-based positioning (columns A-E, rows 1-5)
- Multiple winning conditions: four in a row (any direction) or 2×2 square formation
- Turn-based system with move counter tracking per player
- Command-line argument parsing for optional color mode
- Input validation and error handling
- Case-insensitive command processing
- Real-time board updates and remaining pieces tracker

## Technical Concepts Demonstrated

- Command-line argument parsing and validation
- Pointers and array manipulation
- String processing
- Multi-dimensional arrays for board representation
- Input validation with error handling
- Unicode and ANSI escape sequences for formatted output
- Win condition detection algorithms
- Modular function design

## Compilation and Execution
```bash
clang -o a2 a2.c framework.c
./a2

# Or with color enabled
./a2 enabled
```

## Sample Output
```
===| Welcome to       |===
===| [I]ntricate      |===
===| [S]quare         |===
===| [P]lacement      |===


===| Move 1: Player 1 |===

     A  B  C  D  E
   + -------------
 1 | .  .  .  .  .
 2 | .  .  .  .  .
 3 | .  .  .  .  .
 4 | .  .  .  .  .
 5 | .  .  .  .  .

---| Remaining Pieces |---

Player 1: 4
Player 2: 4

P1 > b1

===| Move 1: Player 2 |===

     A  B  C  D  E
   + -------------
 1 | .  X  .  .  .
 2 | .  .  .  .  .
 3 | .  .  .  .  .
 4 | .  .  .  .  .
 5 | .  .  .  .  .

---| Remaining Pieces |---

Player 1: 3
Player 2: 4

P2 > d5

===| Move 2: Player 1 |===

     A  B  C  D  E
   + -------------
 1 | .  X  .  .  .
 2 | .  .  .  .  .
 3 | .  .  .  .  .
 4 | .  .  .  .  .
 5 | .  .  .  O  .

---| Remaining Pieces |---

Player 1: 3
Player 2: 3

P1 > C2

===| Move 2: Player 2 |===

     A  B  C  D  E
   + -------------
 1 | .  X  .  .  .
 2 | .  .  X  .  .
 3 | .  .  .  .  .
 4 | .  .  .  .  .
 5 | .  .  .  O  .

---| Remaining Pieces |---

Player 1: 2
Player 2: 3

P2 > D4

===| Move 3: Player 1 |===

     A  B  C  D  E
   + -------------
 1 | .  X  .  .  .
 2 | .  .  X  .  .
 3 | .  .  .  .  .
 4 | .  .  .  O  .
 5 | .  .  .  O  .

---| Remaining Pieces |---

Player 1: 2
Player 2: 2

P1 > b3

===| Move 3: Player 2 |===

     A  B  C  D  E
   + -------------
 1 | .  X  .  .  .
 2 | .  .  X  .  .
 3 | .  X  .  .  .
 4 | .  .  .  O  .
 5 | .  .  .  O  .

---| Remaining Pieces |---

Player 1: 1
Player 2: 2

P2 > B4

===| Move 4: Player 1 |===

     A  B  C  D  E
   + -------------
 1 | .  X  .  .  .
 2 | .  .  X  .  .
 3 | .  X  .  .  .
 4 | .  O  .  O  .
 5 | .  .  .  O  .

---| Remaining Pieces |---

Player 1: 1
Player 2: 1

P1 > C4

===| Move 4: Player 2 |===

     A  B  C  D  E
   + -------------
 1 | .  X  .  .  .
 2 | .  .  X  .  .
 3 | .  X  .  .  .
 4 | .  O  X  O  .
 5 | .  .  .  O  .

---| Remaining Pieces |---

Player 1: 0
Player 2: 1

P2 > c5

===| Move 5: Player 1 |===

     A  B  C  D  E
   + -------------
 1 | .  X  .  .  .
 2 | .  .  X  .  .
 3 | .  X  .  .  .
 4 | .  O  X  O  .
 5 | .  .  O  O  .

P1 > b1 B2

===| Move 5: Player 2 |===

     A  B  C  D  E
   + -------------
 1 | .  .  .  .  .
 2 | .  X  X  .  .
 3 | .  X  .  .  .
 4 | .  O  X  O  .
 5 | .  .  O  O  .

P2 > d4 C3

===| Move 6: Player 1 |===

     A  B  C  D  E
   + -------------
 1 | .  .  .  .  .
 2 | .  X  X  .  .
 3 | .  X  O  .  .
 4 | .  O  X  .  .
 5 | .  .  O  O  .

P1 > B3 a2

===| Move 6: Player 2 |===

     A  B  C  D  E
   + -------------
 1 | .  .  .  .  .
 2 | X  X  X  .  .
 3 | .  .  O  .  .
 4 | .  O  X  .  .
 5 | .  .  O  O  .

P2 > c3 d4

===| Move 7: Player 1 |===

     A  B  C  D  E
   + -------------
 1 | .  .  .  .  .
 2 | X  X  X  .  .
 3 | .  .  .  .  .
 4 | .  O  X  O  .
 5 | .  .  O  O  .

P1 > c4 d3

===| Move 7: Player 2 |===

     A  B  C  D  E
   + -------------
 1 | .  .  .  .  .
 2 | X  X  X  .  .
 3 | .  .  .  X  .
 4 | .  O  .  O  .
 5 | .  .  O  O  .

P2 > b4 c4

===| Final Game State |===

     A  B  C  D  E
   + -------------
 1 | .  .  .  .  .
 2 | X  X  X  .  .
 3 | .  .  .  X  .
 4 | .  .  O  O  .
 5 | .  .  O  O  .

===| Winner: Player 2 |===
