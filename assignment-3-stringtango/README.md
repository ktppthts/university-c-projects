Stringtango – Interactive Text Editor

# Description

This is Assignment 3 from Introduction to Structured Programming (TU Graz).
It implements a basic interactive text editor that allows the user to modify a single text stored on the heap using a small set of predefined commands.

# The program focuses on:

-Memory management (malloc, realloc, free)

-Handling strings of arbitrary length

# Features

Append new text

Search and replace substrings

Split text into words, sort alphabetically

Remove duplicate words (case-insensitive)

Interactive command loop

Proper memory management with heap allocation

# Technologies & Languages

C (main language, used for memory management, pointers, and string handling)

Concepts used: Heap allocation, dynamic memory, string manipulation, input validation


# Example Output
Welcome to the
Stringtango Text Editor!

Enter a text:
 > The weather today is very nice!

Choose a command:
 a: append text
 r: search and replace
 s: split and sort
 u: unique
 q: quit

 > r

Please enter the substring to search for:
 > nice

Please enter the new substring:
 > bad

Current text:
The weather today is very bad!

Choose a command:
 a: append text
 r: search and replace
 s: split and sort
 u: unique
 q: quit


