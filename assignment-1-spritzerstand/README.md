# Assignment 1: Spritzerstand Simulator

A C program simulating a non-alcoholic beverage stand with drink ordering and deposit return functionality.

## Features
- Interactive menu system for ordering drinks or returning deposit glasses
- Three drink options with two glass sizes (300ml or 500ml)
- Tip calculation system
- Input validation and error handling
- Deposit refund calculations

## Technical Concepts Demonstrated
- Structs for data modeling (separate structs for orders and returns)
- Input validation with error handling
- Control flow (loops, conditionals)
- Formatted output
- Modular function design

## Compilation and Execution
```bash
clang -o a1 a1.c
./a1
```

## Sample Output 

Welcome to the Spritzerstand Simulator!!!

How can I assist you today?
1. I'd like to return my glasses.
2. I'd like to order.
3. Quit
 > 2

What are you feeling today?
1. Virgin Cuba Libre
2. Carbonas rhoncus bibendum
3. Fizzy water
 > 1

Select a glass size (in ml)
- 300
- 500
 > 300

How much would you like to tip? (in %)
 > 10

--------------------------------
- 300 ml glass of Virgin Cuba Libre
- Drink price: 2.50 €
- Deposit: 1.50 €
- Total without tip: 4.00 €
- Tip: 10 %
- Total: 4.40 €
--------------------------------
Here you go... have a pleasant evening!

How can I assist you today?
1. I'd like to return my glasses.
2. I'd like to order.
3. Quit
 > 3

