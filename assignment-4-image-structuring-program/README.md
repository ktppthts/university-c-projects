Assignment 4 – Image Structuring Program (ISP TU Graz)

# Description

This is Assignment 4 (A4-CSF) from Introduction to Structured Programming (TU Graz).
It implements a command-line image editor that allows users to load BMP images, manipulate them, and compose layered canvases.

Although this project was challenging and not fully optimized, it demonstrates key programming skills and understanding of memory management, data structures, and file I/O.

# Features

Load BMP images into memory

Crop images to create new BMPs

Place BMPs onto a canvas as layers

Maintain a tree structure of layers for undo/switch functionality

Blend layers using three modes: Normal (n), Multiply (m), Subtract (s)

Render the final image (console-based representation)

Save rendered images to BMP files

Proper heap memory management for all layers and images

# Learning Goals & Concepts

Dynamic memory allocation (malloc, realloc, free)

Structs and Enums in C

File I/O and BMP format handling

Trees and parent/child relationships (layer history)

# Usage

Run the program from the command line:

./a4-csf <CANVAS_WIDTH> <CANVAS_HEIGHT>


# Commands include:

load <PATH> – Load BMP image

crop <BMP_ID> <TOP_X> <TOP_Y> <BOTTOM_X> <BOTTOM_Y> – Crop BMP

place <BMP_ID> <CANVAS_X> <CANVAS_Y> <BLEND_MODE> – Place BMP on canvas

undo – Move to previous layer

switch <LAYER_ID> – Jump to a specific layer

print – Display current canvas in console

save <FILE_PATH> – Save current canvas as BMP

quit – Exit program and free memory

Note: Example outputs are not shown since the program renders BMPs.

# Technologies & Skills

C (main language, with dynamic memory management and structs)

Data structures: Trees for layers, arrays for BMPs

Understanding of BMP file format (BGRA pixels, little-endian storage)


# Notes

This assignment includes some unused code and parts that could be optimized

Focus was on completing the functionality, memory safety, and correctness

Demonstrates ability to work with complex data structures and file I/O in C
