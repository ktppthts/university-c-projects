//----------------------------------------------------------------------------------------------------------------------
/// The program is called Image Structuring Program, it can load, edit and save BMPs.
///
/// Author: 12326821
//----------------------------------------------------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include "bmp.h"

#define SIZE 8
#define BYTE 4
#define FIRST_MAGIC_NUMBER 'B'
#define SECOND_MAGIC_NUMBER 'M'

#define BLEND_MODE_N 'n'
#define BLEND_MODE_M 'm'
#define BLEND_MODE_S 's'

#define ARGS_COUNT_INDEX 2
#define ARGS_COUNT 3
#define PLACE_ARGS_COUNT 5
#define CROP_ARGS_COUNT 6
#define ARGC_ONE 1
#define ARGC_TWO 2
#define ARGC_FIVE 5
#define ARGC_SIX 6

#define ROOT_LAYER_ID 0
#define LIBRARY_CAPACITY 1
#define LAYER_TREE_CAPACITY 1
#define START_NUMBER_OF_CHILDREN 0
#define START_ID 0

#define COMMAND_HELP   "help"
#define COMMAND_LOAD   "load"
#define COMMAND_CROP   "crop"
#define COMMAND_PLACE  "place"
#define COMMAND_UNDO   "undo"
#define COMMAND_PRINT  "print"
#define COMMAND_SWITCH "switch"
#define COMMAND_TREE   "tree"
#define COMMAND_BMPS   "bmps"
#define COMMAND_SAVE   "save"

typedef enum _Error_Codes_
{
  OK,
  QUIT,
  ERROR_MALLOC_FAILED,
  ERROR_INVALID_AMOUNT,
  ERROR_INVALID_SIZE,
  ERROR_COMMAND_UNKNOWN,
  ERROR_ARGUMENTS_AMOUNT,
  ERROR_BMP_ID_NOT_FOUND,
  ERROR_CANNOT_OPEN_FILE,
  ERROR_INVALID_FILE,
  ERROR_OUT_OF_RANGE,
  ERROR_INVALID_RECTANGLE,
  ERROR_INVALID_COORDINATES,
  ERROR_BMP_DOESNT_FIT,
  ERROR_INVALID_BLEND_MODE,
  ERROR_ALREADY_ROOT,
  ERROR_LAYER_ID_NOT_FOUND,
  ERROR_INVALID_FILE_PATH
} ErrorCodes;

typedef enum 
{
  HELP,
  LOAD,
  CROP,
  PLACE,
  UNDO,
  PRINT,
  SWITCH,
  TREE,
  BMPS,
  SAVE,
  CMD_COUNT
} CommandCodes;

typedef struct _BMP_
{
  int width_;
  int height_;
  int bmp_id_;
  char *pixels_;
  char *path_;
} BMP;

typedef struct _BMP_Library_
{
  int capacity_;
  BMP **bmps_;
  int next_id_;
} BmpLibrary;

typedef struct _Layer_
{
  int layer_id_;
  BMP* bmp_;
  int coordinate_x_;
  int coordinate_y_;
  int width_;
  int height_;
  char blend_mode_;
  struct _Layer_* parent_layer_;
  struct _Layer_** children_list_;
  int number_of_children_;
  int capacity_;
} Layer;

typedef struct _Tree_Node_
{
  int next_id_;
  Layer* current_active_layer_;
} TreeNode;

typedef struct _Command_
{
  char* name_;
  int argc_;
} Command;

void printWelcomeMessage(char* argv[]);
int handleArguments(int argc, char* argv[]);
int printErrorMessage(ErrorCodes error_code);
int commandLoop(BmpLibrary* library, int width, int height);
int isValid(char* input, BmpLibrary* library, TreeNode* layers);
ErrorCodes loadBmp(char* path, BmpLibrary* library);
void freeLibrary(BmpLibrary* library);
void freeLayerTree(Layer* layer);
ErrorCodes initializeLibrary(BmpLibrary* library);
void initializeCommands(Command commands[CMD_COUNT]);
ErrorCodes resizeCapacity(BmpLibrary* library);
int isWhiteSpace(char* input_string);
int isEmpty(char* input_string);
void removeNewLine(char* string);
char* isInputDone(char* input_string, char last_character);
char* sizeUp(char* input_string, int* size);
char* sizeDown(char* input_string);
char* readInput(void);
char** splitString(char* input);
void printCropMessage(int bmp_id, int new_bmp_id, int width, int height);
void printUndoMessage(int id);
void printPlaceMessage(int layer_id);
void printSwitchMessage(int layer_id);
void printCommandList(void);
int isQuit(char* input);
int countArguments(char** words);
ErrorCodes resizeLayerCapacity(TreeNode* layers);
TreeNode* createRootLayer(int width, int height);
ErrorCodes flipBmp(BMP* bmp);
ErrorCodes loadBmp(char* path, BmpLibrary* library);
void printBmps(BmpLibrary* library);
ErrorCodes checkBmpId(int id, BmpLibrary* library);
ErrorCodes cropCommand(char** words, BmpLibrary* library);
ErrorCodes checkBlendMode(char mode);
ErrorCodes placeBmp(int id, int canvas_x, int canvas_y, char blend_mode, BmpLibrary* library, TreeNode* layers_tree);
ErrorCodes placeCommand(BmpLibrary* library, char** words, TreeNode* layers_tree);
ErrorCodes undoCommand(TreeNode* layers_tree);
int getLayers(TreeNode* layers_tree, Layer** layers_to_print);
void blendLayer(Layer* layer, char* canvas, int canvas_width);
void printCanvas(char* canvas, int canvas_height, int canvas_width);
ErrorCodes printCommand(TreeNode* layers_tree);
Layer* getRootLayer(TreeNode* layers_tree);
void printTree(Layer* root, int depth);
ErrorCodes treeCommand(TreeNode* layers_tree);
Layer* switchLayer(Layer* layer, int new_layer_id);
ErrorCodes switchCommand(TreeNode* layers_tree, char* new_id);
ErrorCodes saveCommand(TreeNode* layers_tree, char* path);
ErrorCodes executeCommand(char** words, BmpLibrary* library, TreeNode* layers_tree);
ErrorCodes dispatchCommand(char** words, int argc, BmpLibrary* library, TreeNode* layers_tree);
int isValid(char* input, BmpLibrary* library, TreeNode* layers_tree);
int commandLoop(BmpLibrary* library, int width, int height);

//----------------------------------------------------------------------------------------------------------------------
/// @brief Main function of the program.
/// @param argc Count of cmd line arguments.
/// @param argv Holds the cmd line arguments.
/// @return Either 0 for quit or 1 if memory allocation failed.
int main(int argc, char* argv[])
{
  int result = handleArguments(argc, argv);
  if (result != 0)
  {
    return result;
  }
  BmpLibrary *library = calloc(1, sizeof(BmpLibrary));
  if (library == NULL)
  {
    return printErrorMessage(ERROR_MALLOC_FAILED);
  }
  if (initializeLibrary(library) != OK)
  {
    return printErrorMessage(ERROR_MALLOC_FAILED);
  }
  printWelcomeMessage(argv);
  int width = atoi(argv[1]);
  int height = atoi(argv[2]);
  result = commandLoop(library, width, height);
  freeLibrary(library);
  return result;
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief Initializes the bmp library.
/// @param library Pointer to the new library.
/// @return OK if memory allocated succesfully, error if not.
ErrorCodes initializeLibrary(BmpLibrary* library)
{
  library->capacity_ = LIBRARY_CAPACITY;
  library->bmps_ = calloc(library->capacity_, sizeof(BMP*));
  if (library->bmps_ == NULL)
  {
    freeLibrary(library);
    return ERROR_MALLOC_FAILED;
  }
  library->next_id_ = START_ID;
  return OK;
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief Initializes the commands.
/// @param commands contains the command names.
void initializeCommands(Command commands[CMD_COUNT])
{
    commands[HELP].name_ = COMMAND_HELP;
    commands[HELP].argc_ = ARGC_ONE;

    commands[LOAD].name_ = COMMAND_LOAD;
    commands[LOAD].argc_ = ARGC_TWO;

    commands[CROP].name_ = COMMAND_CROP;
    commands[CROP].argc_ = ARGC_SIX;

    commands[PLACE].name_ = COMMAND_PLACE;
    commands[PLACE].argc_ = ARGC_FIVE;

    commands[UNDO].name_ = COMMAND_UNDO;
    commands[UNDO].argc_ = ARGC_ONE;

    commands[PRINT].name_ = COMMAND_PRINT;
    commands[PRINT].argc_ = ARGC_ONE;

    commands[SWITCH].name_ = COMMAND_SWITCH;
    commands[SWITCH].argc_ = ARGC_TWO;

    commands[TREE].name_ = COMMAND_TREE;
    commands[TREE].argc_ = ARGC_ONE;

    commands[BMPS].name_ = COMMAND_BMPS;
    commands[BMPS].argc_ = ARGC_ONE;

    commands[SAVE].name_ = COMMAND_SAVE;
    commands[SAVE].argc_ = ARGC_TWO;
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief Prints the welcome message.
/// @param argv Holds the canvas width and length.
void printWelcomeMessage(char* argv[])
{
  printf("\n"
          "Welcome to Image Structuring Program!\n"
          "The canvas is %s x %s pixels.\n"
          "\n", argv[1], argv[2]);
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief Frees the BMP library.
/// @param library pointer to the library.
void freeLibrary(BmpLibrary* library)
{
  if (library == NULL)
  {
    return;
  }
  for (int index = 0; index < library->next_id_; index++)
  {
    if (library->bmps_[index] != NULL)
    {
      free(library->bmps_[index]->pixels_);
      free(library->bmps_[index]->path_);
      free(library->bmps_[index]);
    }
  }
  free(library->bmps_);
  free(library);
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief Frees the layer tree;
/// @param layer pointer to the tree.
void freeLayerTree(Layer* layer)
{
  if (layer == NULL)
  {
    return;
  }
  for (int index = 0; index < layer->number_of_children_; index++)
  {
    freeLayerTree(layer->children_list_[index]);
  }
  free(layer->children_list_);
  free(layer);
  return;
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief Resizes the library capacity.
/// @param library pointer to the library
/// @return -1 if malloc failed, 0 if OK
ErrorCodes resizeCapacity(BmpLibrary* library)
{
  if (library->next_id_ >= library->capacity_)
  {
    int new_capacity = library->capacity_ + 1;
    BMP** temporary = realloc(library->bmps_, new_capacity * sizeof(BMP*));
    if (temporary == NULL)
    {
      printErrorMessage(ERROR_MALLOC_FAILED);
      return ERROR_MALLOC_FAILED;
    }
    library->bmps_ = temporary;
    library->capacity_ = new_capacity;
    temporary = NULL;
  }
  return OK;
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief This function validates the command line arguments.
/// @param argc Count of command line arguments.
/// @param argv Holds the command line arguments.
/// @return 0 if everything is correct, -1 if not.
int handleArguments(int argc, char *argv[])
{
  if (argc != ARGS_COUNT)
  {
    return printErrorMessage(ERROR_INVALID_AMOUNT);
  }
  int argument_index = 1;
  while (argument_index <= ARGS_COUNT_INDEX)
  {
    int arguments_length = (int)strlen(argv[argument_index]);
    for (int index = 0; index < arguments_length; index++)
    {
      if (!isdigit(argv[argument_index][index]))
      {
        return printErrorMessage(ERROR_INVALID_SIZE);
      }
    }
    argument_index++;
  }
  int first_argument = atoi(argv[1]);
  int second_argument = atoi(argv[2]);
  if (first_argument < 1 || second_argument < 1)
  {
    return printErrorMessage(ERROR_INVALID_SIZE);
  }
  return 0;
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief Prints the corresponding error message.
/// @param error_code Holds the error code.
/// @return Returns the correct return value.
int printErrorMessage(ErrorCodes error_code)
{
  switch (error_code)
  {
    case ERROR_MALLOC_FAILED:
      printf("[ERROR] Memory allocation failed!\n");
      return 1;
    case ERROR_INVALID_AMOUNT:
      printf("[ERROR] Invalid amount of command line parameters!\n");
      return 2;
    case ERROR_INVALID_SIZE:
      printf("[ERROR] Invalid canvas size specified!\n");
      return 3;
    case ERROR_COMMAND_UNKNOWN:
      printf("[ERROR] Command unknown!\n");
      return -1;
    case ERROR_ARGUMENTS_AMOUNT:
      printf("[ERROR] Wrong number of arguments!\n");
      return -1;
    case ERROR_BMP_ID_NOT_FOUND:
      printf("[ERROR] BMP ID not found!\n");
      return -1;
    case ERROR_CANNOT_OPEN_FILE:
      printf("[ERROR] Cannot open file!\n");
      return -1;
    case ERROR_INVALID_FILE:
      printf("[ERROR] Invalid file!\n");
      return -1;
    case ERROR_OUT_OF_RANGE:
      printf("[ERROR] Crop coordinates are outside the BMP!\n");
      return -1;
    case ERROR_INVALID_RECTANGLE:
      printf("[ERROR] Crop coordinates do not form a valid rectangle!\n");
      return -1;
    case ERROR_INVALID_COORDINATES:
      printf("[ERROR] Canvas coordinates are invalid!\n");
      return -1;
    case ERROR_BMP_DOESNT_FIT:
      printf("[ERROR] BMP does not fit on the canvas!\n");
      return -1;
    case ERROR_INVALID_BLEND_MODE:
      printf("[ERROR] Invalid blend mode!\n");
      return -1;
    case ERROR_ALREADY_ROOT:
      printf("[ERROR] Already at root layer!\n");
      return -1;
    case ERROR_LAYER_ID_NOT_FOUND:
      printf("[ERROR] Layer ID not found!\n");
      return -1;
    case ERROR_INVALID_FILE_PATH:
      printf("[ERROR] Invalid file path!\n");
      return -1;
    default:
      return 0;
  }
}

//The following code was taken from my own Assignment 3
//begin
//----------------------------------------------------------------------------------------------------------------------
/// @brief Checks if the user input was just white spaces.
/// @param input_string Stores the user input.
/// @return Returns 1 if it is just white spaces, 0 if it is not.
int isWhiteSpace(char* input_string)
{
  int index = 0;
  int is_space = 1;
  while (input_string[index] != '\0')
  {
    if (!isspace(input_string[index]))
    {
      is_space = 0;
      break;
    }
    index++;
  }
  return (is_space == 1);
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief Checks if the user input was empty. (Just newline character or null terminator)
/// @param input_string Stores the user input.
/// @return Returns 1 if it is \n or \0, returns 0 if it is not.
int isEmpty(char* input_string)
{
  return ((strcmp(input_string, "\n") == 0) || (input_string[0] == '\0'));
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief Removes the new line character from a string.
/// @param string String we remove the new line character from.
void removeNewLine(char* string)
{
  int length = strlen(string);
  if (length > 0 && string[length - 1] == '\n')
  {
    string[length - 1] = '\0';
  }
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief This function checks if all of the input was read from the buffer.
/// @param input_string Stores the user input.
/// @param last_character Stores the character we want to find in the string.
/// @return Returns NULL if last_character was not found and the iterator if it was.
char* isInputDone(char* input_string, char last_character)
{
  for (char* iterator = input_string; *iterator != '\0'; iterator++)
  {
    if (*iterator == last_character)
    {
      return iterator;
    }
  }
  return NULL;
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief Sizes up (allocates more memory) for user input if needed.
/// @param input_string Stores the user input.
/// @param size Stores the size of the input string.
/// @return Returns the resized input string.
char* sizeUp(char* input_string, int* size)
{
  while (isInputDone(input_string, '\n') == NULL)
  {
    *size += SIZE;
    char* temporary_string = realloc(input_string, *size * sizeof(char));
    if (temporary_string == NULL)
    {
      free(input_string);
      input_string = NULL;
      return NULL;
    }
    input_string = temporary_string;
    temporary_string = NULL;
    int already_read = strlen(input_string);
    int read_until = *size - already_read;
    fgets(input_string + already_read, read_until, stdin);
  }
  return input_string;
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief Sizes down (reallocates less memory than needed) the user input.
/// @param input_string Stores the user input.
/// @return Returns the resized input string.
char* sizeDown(char* input_string)
{
  int new_size = strlen(input_string) + 1;
  char* temporary_string = realloc(input_string, new_size * sizeof(char));
  if (temporary_string == NULL)
  {
    return NULL;
  }
  return temporary_string;
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief Reads and validates user input.
/// @return Returns the input string.
char* readInput(void)
{
  int size = SIZE;
  char* input_string = calloc(size, sizeof(char));
  if (input_string == NULL)
  {
    return NULL;
  }

  char c;
  while ((c = getchar()) != EOF && isspace(c) && c != '\n'){}
  if (c != EOF)
  {
      ungetc(c, stdin);
  }

  fgets(input_string, size, stdin);

  if (strcmp(input_string, "\n") == 0)
  {
    printErrorMessage(ERROR_COMMAND_UNKNOWN);
  }
  if (isInputDone(input_string, '\n') == NULL)
  {
    input_string = sizeUp(input_string, &size);
    if (input_string == NULL)
    {
      return NULL;
    }
  }
  if ((int)strlen(input_string) < size)
  {
    input_string = sizeDown(input_string);
    if (input_string == NULL)
    {
      return NULL;
    }
  }
  return input_string; 
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief Splits string into words.
/// @param input Input string.
/// @return  Returns the words.
char** splitString(char* input)
{
  int index = 0;
  int input_length = strlen(input);
  char** words = calloc(input_length + 1, sizeof(char*));
  if (words == NULL)
  {
    free(input);
    return NULL;
  }
  char* word = strtok(input, " ");
  while (word != NULL)
  {
    words[index++] = word;
    word = strtok(NULL, " ");
  }
  words[index] = NULL;
  return words;
}
//end

//----------------------------------------------------------------------------------------------------------------------
/// @brief Prints the successful crop message.
/// @param bmp_id Id of the bmp.
/// @param new_bmp_id Id of the new cropped bmp.
/// @param width Width of the new bmp.
/// @param height Height of the new bmp.
void printCropMessage(int bmp_id, int new_bmp_id, int width, int height)
{
  printf("Cropped ID %d to new ID %d with dimensions %d x %d\n",
          bmp_id, new_bmp_id, width, height);
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief Prints the successful undo message.
/// @param id Id of the layer we switched to after undo.
void printUndoMessage(int id)
{
  printf("Switched to layer %d\n", id);
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief Prints successful place message.
/// @param layer_id Id of the layer we switched to after placing.
void printPlaceMessage(int layer_id)
{
  printf("Switched to layer %d\n", layer_id);
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief Prints successful switch message.
/// @param layer_id Id of the layer we switched to.
void printSwitchMessage(int layer_id)
{
  printf("Switched to layer %d\n", layer_id);
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief Prints list of commands.
void printCommandList(void)
{
  printf("\n"
         "Available commands:\n"
         " help\n"
         " load <PATH>\n"
         " crop <BMP_ID> <TOP_X> <TOP_Y> <BOTTOM_X> <BOTTOM_Y>\n"
         " place <BMP_ID> <CANVAS_X> <CANVAS_Y> <BLEND_MODE>\n"
         " undo\n"
         " print\n"
         " switch <LAYER_ID>\n"
         " tree\n"
         " bmps\n"
         " save <FILE_PATH>\n"
         " quit\n"
         "\n");
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief Checks if input was the quit command and trims trailing spaces.
/// @param input User input.
/// @return Returns true if it was, false if not.
int isQuit(char* input)
{
  removeNewLine(input);
  int len = strlen(input);
  while (len > 0 && isspace((unsigned char)input[len - 1]))
  {
    input[len - 1] = '\0';
    len--;
  }
  return strcmp("quit", input) == 0;
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief Counts the number of arguments, including the command itself.
/// @param words User input split into words.
/// @return Returns number of arguments.
int countArguments(char** words)
{
  int number_of_arguments = 0;
  while (words[number_of_arguments] != NULL)
  {
    number_of_arguments++;
  }
  return number_of_arguments;
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief Resizes the capacity of the layer tree.
/// @param layers Layer tree.
/// @return Returns O if everything is OK, -1 if memory allocation failed.
ErrorCodes resizeLayerCapacity(TreeNode* layers)
{
  if (layers->current_active_layer_->number_of_children_ >= layers->current_active_layer_->capacity_)
  {
    layers->current_active_layer_->capacity_ += 1;
    Layer** temporary = realloc(
    layers->current_active_layer_->children_list_, 
    layers->current_active_layer_->capacity_ * sizeof(Layer*));

    if (temporary == NULL)
    {
      printErrorMessage(ERROR_MALLOC_FAILED);
      return ERROR_MALLOC_FAILED;
    }
    layers->current_active_layer_->children_list_ = temporary;
    temporary = NULL;
  }
  return OK;
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief Creates the root layer and sets it as canvas.
/// @param width Width of the canvas.
/// @param height Height of the canvas.
/// @return  layers tree if everything passed, NULL for memory allocation fail.
TreeNode* createRootLayer(int width, int height)
{
  TreeNode* layers = calloc(1, sizeof(TreeNode));
  if (layers == NULL)
  {
    printErrorMessage(ERROR_MALLOC_FAILED);
    return NULL;
  }
  Layer* root = calloc(1, sizeof(Layer));
  if (root == NULL)
  {
    free(layers);
    printErrorMessage(ERROR_MALLOC_FAILED);
    return NULL;
  }

  root->layer_id_ = ROOT_LAYER_ID;
  root->width_ = width;
  root->height_ = height;
  root->parent_layer_ = NULL;

  layers->next_id_ = root->layer_id_ + 1;
  layers->current_active_layer_ = root;
  layers->current_active_layer_->capacity_ = LAYER_TREE_CAPACITY;
  layers->current_active_layer_->children_list_ = calloc(layers->current_active_layer_->capacity_, sizeof(Layer*));
  if (layers->current_active_layer_->children_list_ == NULL)
  {
    free(root);
    free(layers);
    printErrorMessage(ERROR_MALLOC_FAILED);
    return NULL;
  }
  layers->current_active_layer_->number_of_children_ = START_NUMBER_OF_CHILDREN;

  return layers;
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief Flips the bmp upside down so it can be read from top left.
/// @param bmp BMP that we flip.
/// @return OK (0) if everything passed, ERROR_MALLOC_FAILED (1) if memory allocation failed.
ErrorCodes flipBmp(BMP* bmp)
{
  int bytes_in_row = bmp->width_ * BYTE;
  int total_rows = bmp->height_;
  char* flipped = calloc(bytes_in_row * total_rows, sizeof(char));
  if (flipped == NULL)
  {
    return ERROR_MALLOC_FAILED;
  }
  for (int row = 0; row < total_rows; row++)
  {
    int source_row = total_rows - 1 - row;
    for (int column = 0; column < bytes_in_row; column++)
    {
      flipped[row * bytes_in_row + column] = bmp->pixels_[source_row * bytes_in_row + column];
    }
  }

  free(bmp->pixels_);
  bmp->pixels_ = flipped;
  return OK;
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief Loads the and stores it in the library.
/// @param path Path to the bmp.
/// @param library The bmp library of the program.
/// @return OK (0) if everything passed, ERROR_MALLOC_FAILED (1) if memory allocation failed, -1 for other errors
ErrorCodes loadBmp(char* path, BmpLibrary* library)
{
  FILE* file = fopen(path, "rb");
  if (file == NULL)
  {
    return ERROR_CANNOT_OPEN_FILE;
  }
  char magic_number[2] = {0};
  fread(magic_number, sizeof(char), 2, file);
  if (magic_number[0] != FIRST_MAGIC_NUMBER || magic_number[1] != SECOND_MAGIC_NUMBER)
  {
    fclose(file);
    return ERROR_INVALID_FILE;
  }
  
  BMP* new_bmp = calloc(1, sizeof(BMP));
  if (new_bmp == NULL)
  {
    fclose(file);
    return ERROR_MALLOC_FAILED;
  }

  uint32_t pixel_offset;
  fseek(file, 0x0A, SEEK_SET);
  fread(&pixel_offset, sizeof(uint32_t), 1, file);

  int32_t  width;
  fseek(file, 0x12, SEEK_SET);
  fread(&width, sizeof(int32_t), 1, file);

  int32_t  height;
  fseek(file, 0x16, SEEK_SET);
  fread(&height, sizeof(int32_t), 1, file);

  new_bmp->width_ = width;
  new_bmp->height_ = height;

  int number_of_pixels = width * height * BYTE;
  new_bmp->width_ = width;
  new_bmp->height_ = height;
  new_bmp->bmp_id_ = library->next_id_;
  
  new_bmp->pixels_ = calloc(number_of_pixels, sizeof(char));
  if (new_bmp->pixels_ == NULL)
  {
    free(new_bmp);
    fclose(file);
    return ERROR_MALLOC_FAILED;
  }

  fseek(file, pixel_offset, SEEK_SET);  
  fread(new_bmp->pixels_, sizeof(char), number_of_pixels, file);
  
  ErrorCodes result = flipBmp(new_bmp);
  if (result != OK)
  {
    return result;
  }

  new_bmp->path_ = calloc((strlen(path) + 1), sizeof(char));
  if (new_bmp->path_ == NULL)
  {
    free(new_bmp->pixels_);
    free(new_bmp);
    fclose(file);
    return ERROR_MALLOC_FAILED;
  }
  strcpy(new_bmp->path_, path);
  fclose(file);

  if (library->next_id_ >= library->capacity_)
  {
    library->capacity_ *= 2;
    BMP **temporary = realloc(library->bmps_, library->capacity_ * sizeof(BMP*));
    if (temporary == NULL)
    {
      free(new_bmp->pixels_);
      free(new_bmp->path_);
      free(new_bmp);
      return ERROR_MALLOC_FAILED;
    }
    library->bmps_ = temporary;
  }
  library->bmps_[library->next_id_] = new_bmp;
  library->next_id_++;

  printf("Loaded %s with ID %d and dimensions %d %d\n",
  path, new_bmp->bmp_id_, new_bmp->width_, new_bmp->height_);

  return OK;
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief Prints the bmps.
/// @param library The bmp library of the program.
void printBmps(BmpLibrary* library)
{
  if (library == NULL)
  {
    return;
  }

  for (int index = 0; index < library->next_id_; index++)
  {
    if (library->bmps_[index] != NULL)
    {
      printf("BMP %d has dimensions %d x %d\n", 
      library->bmps_[index]->bmp_id_, library->bmps_[index]->width_, library->bmps_[index]->height_);
    }
  }
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief Validates the bmp ID.
/// @param id Id we are looking for.
/// @param library The bmp library of the program.
/// @return OK (0) if it was found, ERROR_BMP_ID_NOT_FOUND (-1) if it was not.
ErrorCodes checkBmpId(int id, BmpLibrary* library)
{
  for (int index = 0; index < library->next_id_; index++)
  {
    if (id == library->bmps_[index]->bmp_id_)
    {
      return OK;
    }
  }
  return ERROR_BMP_ID_NOT_FOUND;
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief Validates the arguments and crops the bmp.
/// @param words User input split into words.
/// @param library The bmp library of the program.
/// @return OK (0) if everything passed, ERROR_MALLOC_FAILED (1) if malloc failed, -1 for everything else.
ErrorCodes cropCommand(char** words, BmpLibrary* library)
{
  int id = atoi(words[1]);
  int top_x = atoi(words[2]);
  int top_y = atoi(words[3]);
  int bottom_x = atoi(words[4]);
  int bottom_y = atoi(words[5]);

  if (checkBmpId(id, library) != OK)
  {
    return ERROR_BMP_ID_NOT_FOUND;
  }
  for (int index_words = 1; index_words < CROP_ARGS_COUNT; index_words++)
  {
    for (int index_letters = 0; words[index_words][index_letters] != 0; index_letters++)
    {
      if (!isdigit(words[index_words][index_letters]))
      {
        return ERROR_OUT_OF_RANGE;
      }
    }
  }
  BMP *old_bmp = library->bmps_[id];
  if (top_x <= 0 || top_y <= 0 ||
      bottom_x > old_bmp->width_ ||
      bottom_y > old_bmp->height_)
  {
    return ERROR_OUT_OF_RANGE;
  }
  if (bottom_x < top_x || bottom_y < top_y )
  {
    return ERROR_INVALID_RECTANGLE;
  }
  int crop_width  = bottom_x - top_x + 1;
  int crop_height = bottom_y - top_y + 1;
  BMP *new_bmp = calloc(1, sizeof(BMP));
  if (new_bmp == NULL)
  {
    return ERROR_MALLOC_FAILED;
  }
  int new_size = crop_width * crop_height * BYTE;
  new_bmp->pixels_ = calloc(new_size, sizeof(char));
  if (new_bmp->pixels_ == NULL)
  {
    free(new_bmp);
    return ERROR_MALLOC_FAILED;
  }
  new_bmp->width_ = crop_width;
  new_bmp->height_ = crop_height;
  new_bmp->bmp_id_ = library->next_id_;
  new_bmp->path_ = NULL;

  for (int y_coordinate = 0;  y_coordinate < crop_height;  y_coordinate++)
  {
    for (int x_coordinate = 0; x_coordinate < crop_width; x_coordinate++)
    {
      int old_x = top_x - 1 + x_coordinate;
      int old_y = top_y - 1 + y_coordinate;
      int old_index = (old_y * old_bmp->width_ + old_x) * BYTE;
      int new_index = (y_coordinate * crop_width + x_coordinate) * BYTE;

      for (int c = 0; c < 4; c++)
      {
        new_bmp->pixels_[new_index + c] = old_bmp->pixels_[old_index + c];
      }
    }
  }
  int result = resizeCapacity(library);
  if (result != OK)
  {
      free(new_bmp->pixels_);
      free(new_bmp);
      return result;
  }
  library->bmps_[library->next_id_] = new_bmp;
  library->next_id_++;
  printCropMessage(id, new_bmp->bmp_id_, crop_width, crop_height);
  return OK;
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief Validates the blend mode.
/// @param mode Blend mode from user input.
/// @return OK (0) if everything passed, ERROR_INVALID_BLEND_MODE (-1) if blend mode was incorrect.
ErrorCodes checkBlendMode(char mode)
{
  if ((mode != BLEND_MODE_N) && (mode != BLEND_MODE_M) && (mode != BLEND_MODE_S))
  {
    return ERROR_INVALID_BLEND_MODE;
  }
  return OK;
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief Places bmp on the canvas by storing bmps in different layers on the layer tree.
/// @param id Id of the bmp.
/// @param canvas_x X coordinate where bmp is placed on the canvas.
/// @param canvas_y Y coordinate where bmp is placed on the canvas.
/// @param blend_mode Blend mode with which bmp should be placed.
/// @param library The bmp library of the program.
/// @param layers_tree The layer tree of the program.
/// @return OK (0) if everything passed, ERROR_MALLOC_FAILED (1) if malloc failed, (-1,2,3) for every other error.
ErrorCodes placeBmp(int id, int canvas_x, int canvas_y, char blend_mode, BmpLibrary* library, TreeNode* layers_tree)
{
  Layer* new_layer = calloc(1, sizeof(Layer));
  if (new_layer == NULL)
  {
    return ERROR_MALLOC_FAILED;
  }
  Layer* parent = layers_tree->current_active_layer_;
  new_layer->width_ = parent->width_;
  new_layer->height_ = parent->height_;

  new_layer->bmp_ = library->bmps_[id];
  new_layer->coordinate_x_ = canvas_x - 1;
  new_layer->coordinate_y_ = canvas_y - 1;
  new_layer->blend_mode_ = blend_mode;
  new_layer->layer_id_ = layers_tree->next_id_++;
  new_layer->parent_layer_ = parent;

  new_layer->children_list_ = calloc(5, sizeof(Layer*));
  if (new_layer->children_list_ == NULL) 
  {
    free(new_layer);
    return ERROR_MALLOC_FAILED;
  }
  new_layer->number_of_children_ = START_NUMBER_OF_CHILDREN;
  new_layer->capacity_ = LAYER_TREE_CAPACITY;

  if (parent->number_of_children_ >= parent->capacity_)
  {
    parent->capacity_ += 1;
    Layer** temporary = realloc(parent->children_list_, parent->capacity_ * sizeof(Layer*));
    if (temporary == NULL)
    {
      free(new_layer->children_list_);
      free(new_layer);
      return ERROR_MALLOC_FAILED;
    }
    parent->children_list_ = temporary;
    temporary = NULL;
  }
  parent->children_list_[parent->number_of_children_++] = new_layer;
  layers_tree->current_active_layer_ = new_layer;

  printPlaceMessage(new_layer->layer_id_);
  return OK;
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief Validates the place command and calls placeBmp() to actually place the bmp.
/// @param library The bmp library of the program.
/// @param words User input split into words.
/// @param layers_tree The layer tree of the program.
/// @return OK (0) if everything passed, ERROR_MALLOC_FAILED (1) if malloc failed, (-1,2,3) for every other error.
ErrorCodes placeCommand(BmpLibrary* library, char** words, TreeNode* layers_tree)
{
  int id = atoi(words[1]);
  int canvas_x = atoi(words[2]);
  int canvas_y = atoi(words[3]);
  char blend_mode = words[4][0];
  ErrorCodes result = checkBmpId(id, library);
  if (result != OK)
  {
    return result;
  }
  for (int index_words = 1; index_words < PLACE_ARGS_COUNT - 1; index_words++)
  {
    for (int index_letters = 0; words[index_words][index_letters] != 0; index_letters++)
    {
      if (!isdigit((unsigned char)words[index_words][index_letters]))
      {
        return ERROR_INVALID_COORDINATES;
      }
    }
  }
  Layer* layer = layers_tree->current_active_layer_;
  BMP* bmp = library->bmps_[id];
  
  if (canvas_x <= 0 || canvas_y <= 0)
  {
    return ERROR_INVALID_COORDINATES;
  }

  if ((canvas_x - 1 + bmp->width_) > layer->width_ ||
    (canvas_y - 1 + bmp->height_) > layer->height_)
  {
    return ERROR_BMP_DOESNT_FIT;
  }

  if (words[4][1] != '\0') 
  {
      return ERROR_INVALID_BLEND_MODE;
  }
  result = checkBlendMode(blend_mode);
  if (result != OK) 
  {
      return result;
  }
  result = placeBmp(id, canvas_x, canvas_y, blend_mode, library, layers_tree);
  if (result != 0)
  {
    return result;
  }
  return OK;
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief Switches user to the previous layer.
/// @param layers_tree The layer tree of the program.
/// @return OK (0) if everything passed, ERROR_ALREADY_ROOT (-1) if already at root layer.
ErrorCodes undoCommand(TreeNode* layers_tree)
{
  if (layers_tree->current_active_layer_->layer_id_ == ROOT_LAYER_ID)
  {
    return ERROR_ALREADY_ROOT;
  }
  layers_tree->current_active_layer_ = layers_tree->current_active_layer_->parent_layer_;
  printUndoMessage(layers_tree->current_active_layer_->layer_id_);
  return OK;
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief Starts at leaf, goes up to root by parent layer to get the array of layers to print.
/// @param layers_tree The layer tree of the program.
/// @param layers_to_print Array of the layers we want to print.
/// @return last_index which is actually the count of all layers we want to print excluding the root layer.
int getLayers(TreeNode* layers_tree, Layer** layers_to_print)
{
  Layer* current_layer = layers_tree->current_active_layer_;
  int last_index = 0;
  while (current_layer->layer_id_ != 0)
  {
    layers_to_print[last_index++] = current_layer;
    current_layer = current_layer->parent_layer_;
  }

  for (int index = 0; index < last_index / 2; index++)
  {
    Layer* temporary = layers_to_print[index];
    layers_to_print[index] = layers_to_print[last_index - 1 - index];
    layers_to_print[last_index - 1 - index] = temporary;
  }

  return last_index;
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief Blends the layer pixels together.
/// @param layer Current layer.
/// @param canvas The root layer.
/// @param canvas_width Width of the canvas.
void blendLayer(Layer* layer, char* canvas, int canvas_width)
{
  BMP* bmp = layer->bmp_;
  double alpha = 0;
  for (int y = 0; y < bmp->height_; y++)
  {
    for (int x = 0; x < bmp->width_; x++)
    {
      int bmp_index = (y * bmp->width_ + x) * BYTE;
      int canvas_x = layer->coordinate_x_ + x;
      int canvas_y = layer->coordinate_y_ + y;
      int canvas_index = (canvas_y * canvas_width + canvas_x) * BYTE;

      switch (layer->blend_mode_)
      {
        case BLEND_MODE_N:
          alpha = (unsigned char)bmp->pixels_[bmp_index + 3] / 255.0;
          for (int c = 0; c < 3; c++) {
              unsigned char A = (unsigned char)canvas[canvas_index + c];
              unsigned char B = (unsigned char)bmp->pixels_[bmp_index + c];
              canvas[canvas_index + c] = (unsigned char)(alpha * B + (1.0 - alpha) * A);
          }
          canvas[canvas_index + 3] = (unsigned char)255;
          break;
        case BLEND_MODE_M:
          canvas[canvas_index] = ((unsigned char)canvas[canvas_index] * (unsigned char)bmp->pixels_[bmp_index]) / 255;
          canvas[canvas_index + 1] = 
          ((unsigned char)canvas[canvas_index + 1] * (unsigned char)bmp->pixels_[bmp_index + 1]) / 255;
          canvas[canvas_index + 2] = 
          ((unsigned char)canvas[canvas_index + 2] * (unsigned char)bmp->pixels_[bmp_index + 2]) / 255;
          canvas[canvas_index + 3] = (unsigned char)255;
          break;
        case BLEND_MODE_S:
          canvas[canvas_index] = 
          (unsigned char)abs((unsigned char)canvas[canvas_index] - (unsigned char)bmp->pixels_[bmp_index]);
          canvas[canvas_index + 1] = 
          (unsigned char)abs((unsigned char)canvas[canvas_index + 1] - (unsigned char)bmp->pixels_[bmp_index + 1]);
          canvas[canvas_index + 2] = 
          (unsigned char)abs((unsigned char)canvas[canvas_index + 2] - (unsigned char)bmp->pixels_[bmp_index + 2]);
          canvas[canvas_index + 3] = (unsigned char)255;
          break;
        default:
          break;
      }
    }
  }
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief Prints the canvas.
/// @param canvas The root layer.
/// @param canvas_height Height of the canvas.
/// @param canvas_width Width of the canvas.
void printCanvas(char* canvas, int canvas_height, int canvas_width)
{
  printf("   ");
  for (int column_indices = 1; column_indices <= canvas_width; column_indices++)
  {
    if (column_indices < 10)
    {
      printf(" 0%d", column_indices);
    }
    else
    {
      printf(" %d", column_indices);
    }
  }
  printf("\n");

  for (int y = 0; y < canvas_height; y++)
  {
    int row_indices = y + 1;
    if (row_indices < 10)
    {
      printf("0");
    }
    printf("%d|", row_indices);

    for (int x = 0; x < canvas_width; x++)
    {
        int index = (y * canvas_width + x) * BYTE;
        unsigned char B = canvas[index];
        unsigned char G = canvas[index + 1];
        unsigned char R = canvas[index + 2];
        printf("\033[38;2;%d;%d;%dm███\033[0m", R, G, B);
    }
    printf("|\n");
  }

  printf("  ");
  for (int bottom = 0; bottom < canvas_width * 3 + 2; bottom++)
  {
    printf("-");
  }
  printf("\n");
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief Initializes the canvas, blends the layers and prints the canvas.
/// @param layers_tree The layer tree of the program.
/// @return OK (0) if everything passed, ERROR_MALLOC_FAILED (1) if memory allocation failed
ErrorCodes printCommand(TreeNode* layers_tree)
{
  Layer* layer = layers_tree->current_active_layer_;
  int canvas_height = layer->height_;
  int canvas_width = layer->width_;
  int canvas_size = canvas_height * canvas_width * BYTE;
  char* canvas = calloc(canvas_size, sizeof(char));
  if (canvas == NULL)
  {
    return ERROR_MALLOC_FAILED;
  }

  for (int pixel_index = 0; pixel_index < canvas_size; pixel_index += BYTE)
  {
    canvas[pixel_index] = (unsigned char)255;
    canvas[pixel_index + 1] = (unsigned char)255;
    canvas[pixel_index + 2] = (unsigned char)255;
    canvas[pixel_index + 3] = (unsigned char)255;
  }
  
  if (layers_tree->current_active_layer_->layer_id_ == 0)
  {
    printCanvas(canvas, canvas_height, canvas_width);
    free(canvas);
    return OK;
  }
  Layer** layers_to_print = calloc(layers_tree->next_id_, sizeof(Layer*));
  if (layers_to_print == NULL)
  {
    free(canvas);
    return ERROR_MALLOC_FAILED;
  }
  int layers_count = getLayers(layers_tree, layers_to_print);

  for (int index = 0; index < layers_count; index++)
  {
    blendLayer(layers_to_print[index], canvas, canvas_width);
  }

  printCanvas(canvas, canvas_height, canvas_width);
  free(layers_to_print);
  free(canvas);
  return OK;
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief Gets the pointer to the root layer of the tree.
/// @param layers_tree The layer tree of the program.
/// @return The root of the tree.
Layer* getRootLayer(TreeNode* layers_tree)
{
  Layer* root = layers_tree->current_active_layer_;
  while (root->parent_layer_ != NULL)
  {
    root = root->parent_layer_;
  }
  return root;
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief Prints the layer tree.
/// @param root Root of the layer tree.
/// @param depth Depth of the layer tree.
void printTree(Layer* root, int depth)
{
  for (int space_count = 0; space_count < depth; space_count++)
  {
    printf("   ");
  }
  printf("Layer %d renders BMP %d at %d %d\n",
          root->layer_id_, root->bmp_->bmp_id_, root->coordinate_x_ + 1, root->coordinate_y_ + 1);
  if (root->number_of_children_ != 0)
  {
    for (int index = 0; index < root->number_of_children_; index++)
    {
      printTree(root->children_list_[index], depth + 1);
    }
  }
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief Gets the root of the tree and prints it.
/// @param layers_tree The layer tree of the program.
/// @return OK (0) if everything passes.
ErrorCodes treeCommand(TreeNode* layers_tree)
{
  int depth = 0;
  Layer* root = getRootLayer(layers_tree);
  printf("Layer %d\n", root->layer_id_);

  if (root->number_of_children_ != 0)
  {
    for (int index = 0; index < root->number_of_children_; index++)
    {
      printTree(root->children_list_[index], depth + 1);
    }
  }
  return OK;
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief Switches from current to the chosen layer.
/// @param layer Pointer to the layer we are on.
/// @param new_layer_id Pointer to the layer we want to be on.
/// @return Returns current layer if we are already on the layer we want to switch to, returns the switched layer if
///         we switched, NULL otherwise.
Layer* switchLayer(Layer* layer, int new_layer_id)
{
  Layer* switched_layer = NULL;

  if (layer->layer_id_ == new_layer_id)
  {
    return layer;
  }
  else if (layer->number_of_children_ != 0 && layer->layer_id_ != new_layer_id)
  {
    for (int index = 0; index < layer->number_of_children_; index++)
    {
      switched_layer = switchLayer(layer->children_list_[index], new_layer_id);
      if (switched_layer != NULL)
      {
        return switched_layer;
      }
    }
  }
  return NULL;
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief Validates the switch command and executes it.
/// @param layers_tree The tree of the program.
/// @param new_id Id of the layer we want to switch to.
/// @return OK (0) if everything passes, ERROR_INVALID_COORDINATES (-1) if coordinates are invalid.
ErrorCodes switchCommand(TreeNode* layers_tree, char* new_id)
{
  Layer* root = getRootLayer(layers_tree);
  Layer* switched_layer = NULL;

  for (int index_letters = 0; new_id[index_letters] != 0; index_letters++)
  {
    if (!isdigit(new_id[index_letters]))
    {
      return ERROR_INVALID_COORDINATES;
    }
  }
  
  int new_layer_id = atoi(new_id);

  if (root->layer_id_ == new_layer_id)
  {
    layers_tree->current_active_layer_ = root;
    printSwitchMessage(new_layer_id);
    return OK;
  }

  for (int index = 0; index < root->number_of_children_; index++)
  {
    switched_layer = switchLayer(root->children_list_[index], new_layer_id);
    if (switched_layer != NULL)
    {
      layers_tree->current_active_layer_ = switched_layer;
      printSwitchMessage(new_layer_id);
      return OK;
    }
  }
  return ERROR_LAYER_ID_NOT_FOUND;
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief Validates the save command and executes it.
/// @param layers_tree The layer tree of the program.
/// @param path Path we want to save the bmp to.
/// @return OK (0) if everything passes, ERROR_MALLOC_FAILED (1) if malloc fails, (-1, 2, 3) for other types of errors.
ErrorCodes saveCommand(TreeNode* layers_tree, char* path)
{
  Layer** layers_to_print = calloc(layers_tree->next_id_, sizeof(Layer*));
  if (layers_to_print == NULL)
  {
    return ERROR_MALLOC_FAILED;
  }
  int layers_count = getLayers(layers_tree, layers_to_print);

  char* canvas = 
    calloc(layers_tree->current_active_layer_->width_ * layers_tree->current_active_layer_->height_ * BYTE,
    sizeof(char));
    
  if (canvas == NULL)
  {
    free(layers_to_print);
    free(canvas);
    return ERROR_MALLOC_FAILED;
  }

  for (int index = 0; 
       index < layers_tree->current_active_layer_->width_ * layers_tree->current_active_layer_->height_ * BYTE; 
       index += BYTE)
  {
    canvas[index] = (unsigned char)255;
    canvas[index + 1] = (unsigned char)255;
    canvas[index + 2] = (unsigned char)255;
    canvas[index + 3] = (unsigned char)255;
  }

  for (int counter = 0; counter < layers_count; counter++)
  {
    blendLayer(layers_to_print[counter],canvas, layers_tree->current_active_layer_->width_);
  }

  FILE* file = fopen(path, "wb");
  if (file == NULL)
  {
    free(layers_to_print);
    free(canvas);
    return ERROR_INVALID_FILE_PATH;
  }
  
  BmpHeader* header = calloc(1, sizeof(BmpHeader));
  if (header == NULL)
  {
    free(header);
    free(layers_to_print);
    free(canvas);
    fclose(file);
    return ERROR_MALLOC_FAILED;
  }
  fillBmpHeaderDefaultValues(header, 
                             layers_tree->current_active_layer_->width_, 
                             layers_tree->current_active_layer_->height_);
  fwrite(header, sizeof(BmpHeader), 1, file);

  int row_size = layers_tree->current_active_layer_->width_ * BYTE;
  for (int row = layers_tree->current_active_layer_->height_ - 1; row >= 0; row--)
  {
    fwrite(canvas + row * row_size, row_size, 1, file);
  }

  free(header);
  free(layers_to_print);
  free(canvas);
  fclose(file);
  printf("Successfully saved image to %s\n", path);
  return OK;
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief Executes the command from user input.
/// @param words User input split into words.
/// @param library The bmp library of the program.
/// @param layers_tree The layer tree of the program.
/// @return OK (0) if everything passes, ERROR_MALLOC_FAILED (1) if malloc fails, (-1, 2, 3) for other types of errors.
ErrorCodes executeCommand(char** words, BmpLibrary* library, TreeNode* layers_tree)
{
  char* command = words[0];
  if (strcmp(COMMAND_HELP, command) == 0)
  {
    printCommandList();
    return OK;
  }
  else if (strcmp(COMMAND_LOAD, command) == 0)
  {
    return loadBmp(words[1], library);
  }
  else if (strcmp(COMMAND_BMPS, command) == 0)
  {
    printBmps(library);
    return OK;
  }
  else if (strcmp(COMMAND_CROP, command) == 0)
  {
    return cropCommand(words, library);
  }
  else if (strcmp(COMMAND_PRINT, command) == 0)
  {
    return printCommand(layers_tree);
  }
  else if (strcmp(COMMAND_TREE, command) == 0)
  {
    return treeCommand(layers_tree);
  }
  else if (strcmp(COMMAND_SWITCH, command) == 0)
  {
    return switchCommand(layers_tree, words[1]);
  }
  else if (strcmp(COMMAND_PLACE, command) == 0)
  {
    return placeCommand(library, words, layers_tree);
  }
  else if (strcmp(COMMAND_UNDO, command) == 0)
  {
    return undoCommand(layers_tree);
  }
  else
  {
    return saveCommand(layers_tree, words[1]);
  }
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief Validates the command and dispatches it to be executed.
/// @param words User input split into words.
/// @param argc Count of arguments the user put in.
/// @param library The bmp library of the program.
/// @param layers_tree The layer tree of the program.
/// @return OK (0) if everything passes, ERROR_MALLOC_FAILED (1) if malloc fails, (-1, 2, 3) for other types of errors. 
ErrorCodes dispatchCommand(char** words, int argc, BmpLibrary* library, TreeNode* layers_tree)
{
  char* command = words[0];
  Command commands[CMD_COUNT];
  initializeCommands(commands);
  for (int index = 0; index < CMD_COUNT; index++)
  {
    if (strcmp(command, commands[index].name_) == 0)
    {
      if (argc != commands[index].argc_)
      {
        return ERROR_ARGUMENTS_AMOUNT;
      }
      return executeCommand(words, library, layers_tree);
    }
  }
  return ERROR_COMMAND_UNKNOWN;
}

/// @brief Starts the command validation and execution process.
/// @param input User input.
/// @param library The bmp library of the program. 
/// @param layers_tree The layer tree of the program.
/// @return OK (0) if everything passes, ERROR_MALLOC_FAILED (1) if malloc fails
int isValid(char* input, BmpLibrary* library, TreeNode* layers_tree)
{
  removeNewLine(input);
  char** words = splitString(input);
  if (words == NULL)
  {
      free(input);
      return printErrorMessage(ERROR_MALLOC_FAILED);
  }
  int argc = countArguments(words);
  ErrorCodes result = dispatchCommand(words, argc, library, layers_tree);
  free(words);
  
  if (result == OK || result == ERROR_MALLOC_FAILED)
  {
    return result;
  }
  else
  {
    return printErrorMessage(result);
  }
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief Starts the command loop.
/// @param library The bmp library of the program.
/// @param width Width of the canvas.
/// @param height Height of the canvas.
/// @return OK (0) if everything passes, ERROR_MALLOC_FAILED (1) if malloc fails, (-1, 2, 3) for other types of errors.
int commandLoop(BmpLibrary* library, int width, int height)
{
  TreeNode* layers_tree = createRootLayer(width, height);
  if (layers_tree == NULL)
  {
    return 1;
  }
  while (1)
  {
    printf(" > ");
    char* input = readInput();
    if (input == NULL)
    {
      return printErrorMessage(ERROR_MALLOC_FAILED);
    }
    if (isQuit(input))
    {
      free(input);
      Layer* root = getRootLayer(layers_tree);
      freeLayerTree(root);
      free(layers_tree);
      return 0;
    }
    if (isEmpty(input) || isWhiteSpace(input))
    {
      free(input);
      continue;
    }
    int result = isValid(input, library, layers_tree);
    if (result == 1)
    {
      free(input);
      return result;
    }
    free(input);
  }
  return 0;
}
