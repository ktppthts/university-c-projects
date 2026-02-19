//----------------------------------------------------------------------------------------------------------------------
/// The program is called Stringtango. It is an editor for basic string operations.
///
/// Author: 12326821
//----------------------------------------------------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define SIZE 8
#define ERROR_NULL "[ERROR] Memory allocation failed!\n"
#define ERROR_INVALID_COMMAND "[ERROR] Command unknown!\n"
#define ERROR_EMPTY_STRING "[ERROR] String can't be empty!\n"
#define ERROR_NOT_FOUND "[ERROR] Substring not found!\n"
#define ERROR_SORT_EMPTY "[ERROR] No words to sort!\n"
#define ERROR_ALL_WORDS_UNIQUE "[ERROR] All words are already unique\n"

#define NULL_TERMINATOR "\0"
#define END_OF_LINE "\n"

#define SIZE_OF_COMMAND 4
#define COMMAND_APPEND "a"
#define COMMAND_SEARCH "r"
#define COMMAND_SPLIT "s"
#define COMMAND_UNIQUE "u"
#define COMMAND_QUIT_Q "q"
#define COMMAND_QUIT "quit"

void printWelcomeMessage(void);
char* isInputDone(char* input_string, char last_character);
void inputMessage(void);
void commandList(void);
void appendMessage(void);
void searchMessage(void);
void replaceMessage(void);
void currentTextMessage(char* input_string);
void removeNewLine(char* string);
int isWhiteSpace(char* input_string);
int isQuit(char* input_string);
int isEmpty(char* input_string);
int isFound(char* input_string, char* search_string);
char* sizeUp(char* input_string, int* size);
char* sizeDown(char* input_string);
char* getSentence(void);
void commandList(void);
char* commandAppend(char* input_string);
char* replace(char* input_string, char* search_string, char* replacement_string);
char* search(char* input_string);
char* commandSearchAndReplace(char* input_string);
char** splitString(char* input_string);
char* putWordsIntoString(char** words,char* input_string);
char* sortString(char** words, char* input_string);
char* commandSplitAndSort(char* input_string);
int hasDuplicates(char* input_string);
char* removeDuplicates(char** words, char* input_string, int number_of_words);
char* commandUnique(char* input_string);
char* commandQuit(char* input_string);
char* handleCommands(char* input_string);
int handleStart(void);

//----------------------------------------------------------------------------------------------------------------------
/// @brief This is the main function of the program. 
/// @return Returns the return value from the function handleStart().
int main(void)
{
  printWelcomeMessage();
  return handleStart();
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief This function prints the welcome message.
void printWelcomeMessage(void)
{
  printf("\n"
         "Welcome to the\n"
         "Stringtango Text Editor!\n"
         "\n");
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
/// @brief Prints the message for very first user input.
void inputMessage(void)
{
  printf("Enter a text:\n"
         " > ");
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief Prints the list of commands.
void commandList(void)
{
  printf("\n"
         "Choose a command:\n"
         " a: append text\n"
         " r: search and replace\n"
         " s: split and sort\n"
         " u: unique\n"
         " q: quit\n"
         "\n"
         " > ");
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief Prints the message asking for text to append.
void appendMessage(void)
{
  printf("\n"
        "Please enter the string that should be appended:\n"
        " > ");
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief Prints the message asking for a substring to search for.
void searchMessage(void)
{
  printf("\n"
         "Please enter the substring to search for:\n"
         " > ");
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief Prints the message asking for a substring to replace with.
void replaceMessage(void)
{
  printf("\n"
         "Please enter the new substring:\n"
         " > ");
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief Prints the current text.
void currentTextMessage(char* input_string)
{
  printf("\n"
        "Current text:\n"
        "%s\n", input_string);
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
/// @brief Checks if the user input was the quit command.
/// @param input_string Stores the user input.
/// @return Returns 1 if it is quit, 0 if it is not.
int isQuit(char* input_string)
{
  return (strcmp(input_string, COMMAND_QUIT) == 0);
}

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
  return ((strcmp(input_string, END_OF_LINE) == 0) || (input_string[0] == '\0'));
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief Checks if the substring we are looking for is found in the input string.
/// @param input_string Stores the user input.
/// @param search_string Stores the string we search for.
/// @return Returns 1 if one occurrence of the substring, 0 if it is not.
int isFound(char* input_string, char* search_string)
{
  return strstr(input_string, search_string) != NULL;
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
    int read_until =*size - already_read;
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
/// @brief Reads user input, allocates memory for it, resizes it if needed.
/// @return Returns NULL if allocation failed, the input string if the user input was stored in it and allocated.
char* getSentence(void)
{
  int size = SIZE;
  char* input_string = malloc(size * sizeof(char));
  if (input_string == NULL)
  {
    return NULL;
  }
  fgets(input_string, size, stdin);

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
/// @brief This function appends another string to the first one.
/// @param string1 The string we append to.
/// @return Returns NULL if memory allocation failed or the new string if appending succeded or the original input
///         string if there were any errors.
char* commandAppend(char* input_string)
{
  appendMessage();
  char* append_string = getSentence();
  if (append_string == NULL)
  {
    return NULL;
  }
  if (isEmpty(append_string))
  {
    printf(ERROR_EMPTY_STRING);
    free(append_string);
    append_string = NULL;
    return input_string;
  }
  removeNewLine(append_string);
  if (isQuit(append_string))
  {
    free(input_string);
    input_string = NULL;
    return append_string;
  }
  removeNewLine(input_string);
  int input_length = strlen(input_string);
  int append_length = strlen(append_string);
  int new_size = input_length + append_length + 1;

  char* temporary_string = realloc(input_string, new_size * sizeof(char));
  if (temporary_string == NULL)
  {
    free(append_string);
    append_string = NULL;
    return NULL;
  }
  input_string = temporary_string;
  temporary_string = NULL;
  strcat(input_string, append_string);
  currentTextMessage(input_string);
  free(append_string);
  append_string = NULL;
  return input_string;
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief Replaces the substring we are looking for with the replacement substring.
/// @param input_string Stores the user input.
/// @param search_string Stores the substring we search for.
/// @param replacement_string Stores the substring we want to replace with.
/// @return Returns NULL if memory allocaiton failed, the new string if the substring was succesfully replaced,
///         or returns the original input string if there were errors.
// The idea and way of how the following code works was taken from this video:
// https://www.youtube.com/watch?v=tGgl6EMZxLU&t=1042s
// The code was altered so it's not exactly the same as in the video because it caused timeouts.
// begin
char* replace(char* input_string, char* search_string, char* replacement_string)
{
  int input_length = strlen(input_string);
  int search_length  = strlen(search_string);
  int replacement_length  = strlen(replacement_string);
  if (search_length == 0)
  {
    printf(ERROR_EMPTY_STRING);
    return input_string;
  }

  int new_length = input_length;
  for (int input_index = 0; input_index <= input_length - search_length; input_index++)
  {
    int same_string = 1;
    for (int search_index = 0; search_index < search_length; search_index++)
    {
      if (input_string[input_index + search_index] != search_string[search_index])
      {
        same_string = 0;
        break;
      }
    }
    if (same_string)
    {
      new_length += (replacement_length - search_length);
    }
  }
  char* new_string = malloc(new_length + 1);
  if (new_string == NULL)
  {
    return NULL;
  }
  int new_index = 0;
  int input_index = 0;
  while (input_index < input_length)
  {
    int same_string = 0;
    if (input_index <= input_length - search_length)
    {
      same_string = 1;
      for (int search_index = 0; search_index < search_length; search_index++)
      {
        if (input_string[input_index + search_index] != search_string[search_index])
        {
          same_string = 0;
          break;
        }
      }
    }
    if (same_string)
    {
      for (int replacement_index = 0; replacement_index < replacement_length; replacement_index++)
      {
        new_string[new_index++] = replacement_string[replacement_index];
      }
      input_index += search_length;
    }
    else
    {
      new_string[new_index++] = input_string[input_index++];
    }
  }
  new_string[new_index] = '\0';
  free(input_string);
  input_string = NULL;
  currentTextMessage(new_string);
  return new_string;
}
//end

//----------------------------------------------------------------------------------------------------------------------
/// @brief Checks if the substring we are looking for is found in the input string.
/// @param input_string Stores the user input.
/// @return Returns NULL if memory allocation failed, the new string if it was replaced correctly or input string if 
///         there were any errors.
char* search(char* input_string)
{
  searchMessage();
  char* search_string = getSentence();
  if (search_string == NULL)
  {
    return NULL;
  }
  removeNewLine(search_string);
  if (isQuit(search_string))
  {
    free(input_string);
    input_string = NULL;
    return search_string;
  }
  if (!isFound(input_string, search_string))
  {
    printf(ERROR_NOT_FOUND);
    free(search_string);
    search_string = NULL;
    return input_string;
  }
  if (isEmpty(search_string))
  {
    printf(ERROR_EMPTY_STRING);
    free(search_string);
    search_string = NULL;
    return input_string;
  }
  replaceMessage();
  char* replacement_string = getSentence();
  if (replacement_string == NULL)
  {
    free(search_string);
    search_string = NULL;
    return NULL;
  }
  removeNewLine(replacement_string);
  if (isQuit(replacement_string))
  {
    free(search_string);
    free(input_string);
    search_string = NULL;
    input_string = NULL;
    return replacement_string;
  }
  char* new_string = replace(input_string, search_string, replacement_string);
  free(search_string);
  free(replacement_string);
  search_string = NULL;
  input_string = NULL;
  return new_string;
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief Handles errors before starting to search for the inputted substring.
/// @param input_string Stores the user input.
/// @return Returns the original input string if there were errors or whatever search() returns as it's called.
char* commandSearchAndReplace(char* input_string)
{
  if (isEmpty(input_string))
  {
    printf(ERROR_EMPTY_STRING);
    return input_string;
  }
  return search(input_string);
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief Splits a string into words.
/// @param input_string Stores the user input.
/// @return Returns NULL if memory allocation failed or the splitted words.
char** splitString(char* input_string)
{
  int index = 0;
  int input_length = strlen(input_string);
  char** words = malloc((input_length + 1) * sizeof(char*));
  if (words == NULL)
  {
    return NULL;
  }
  char* word = strtok(input_string, " ");
  while (word != NULL)
  {
    words[index++] = word;
    word = strtok(NULL, " ");
  }
  words[index] = NULL;
  return words;
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief Puts the splitted words back into the string.
/// @param words Array of pointers to words. (Array of words)
/// @param input_string Stores the user input.
/// @return Returns NULL if memory allocation failed or the new string after putting the words together.
char* putWordsIntoString(char** words,char* input_string)
{
  int new_size = 0;
  int index_of_words = 0;
  while (words[index_of_words] != NULL)
  {
    new_size += strlen(words[index_of_words]);
    index_of_words++;
  }
  new_size += index_of_words;

  char* temporary = realloc(input_string, new_size * sizeof(char));
  if (temporary == NULL) 
  {
    return NULL;
  }
  input_string = temporary;
  input_string[0] = '\0';

  index_of_words = 0;
  while (words[index_of_words] != NULL)
  {
    strcat(input_string, words[index_of_words]);
    if (words[index_of_words + 1] != NULL)
    {
      strcat(input_string, " ");
    }
    index_of_words++;
  }
  return input_string;
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief Sorts the splitted words in "ASCII" order.
/// @param words Array of pointers to words. (Array of words)
/// @param input_string Stores the user input.
/// @return Returns whatever putWordsIntoString() returns as it's called.
char* sortString(char** words, char* input_string)
{
  int number_of_elements = 0;
  while (words[number_of_elements] != NULL) 
  {
    number_of_elements++;
  }
  for (int index_of_words = 0; index_of_words < number_of_elements - 1; index_of_words++)
  {
    for (int index_of_letters = 0; index_of_letters < number_of_elements - index_of_words - 1; index_of_letters++)
    {
      if (strcmp(words[index_of_letters], words[index_of_letters + 1]) > 0)
      {
        char* temporary = words[index_of_letters];
        words[index_of_letters] = words[index_of_letters + 1];
        words[index_of_letters + 1] = temporary;
      }
    }
  }
  return putWordsIntoString(words, input_string);
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief Handles errors before starting the split and sort command.
/// @param input_string Stores the user input.
/// @return Returns NULL if memory allocation failed, otherwise the new sorted string.
char* commandSplitAndSort(char* input_string)
{
  if (isEmpty(input_string) || isWhiteSpace(input_string))
  {
    printf(ERROR_SORT_EMPTY);
    return input_string;
  }
  removeNewLine(input_string);
  int input_length = strlen(input_string);
  char* temporary = malloc((input_length + 1) * sizeof(char));
  if (temporary == NULL)
  {
    return NULL;
  }
  strcpy(temporary, input_string);
  char** words = splitString(temporary);
  if (words == NULL)
  {
    free(temporary);
    temporary = NULL;
    return NULL;
  }
  input_string = sortString(words, input_string);
  free(words);
  free(temporary);
  words = NULL;
  temporary = NULL;
  currentTextMessage(input_string);
  return input_string;
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief Checks if there are duplicate words in the string.
/// @param input_string Stores the user input.
/// @return Returns 1 if there were same words, 0 if all words were unique, -1 if memory allocation failed.
int hasDuplicates(char* input_string)
{
  char* temporary_string = malloc((strlen(input_string) + 1) * sizeof(char));
  if (temporary_string == NULL)
  {
    return -1;
  }
  strcpy(temporary_string, input_string);
  char** words = splitString(temporary_string);

  int number_of_elements = 0;
  while (words[number_of_elements] != NULL) 
  {
    number_of_elements++;
  }

  for (int first_word_index = 0; first_word_index < number_of_elements; first_word_index++)
  {
    for (int second_word_index = first_word_index + 1; second_word_index < number_of_elements; second_word_index++)
    {
      int is_equal = 1;
      char* word1 = words[first_word_index];
      char* word2 = words[second_word_index];
      int index = 0;
      while (word1[index] != '\0' || word2[index] != '\0')
      {
        if (tolower((char)word1[index]) != tolower((char)word2[index]))
        {
          is_equal = 0;
          break;
        }
        index++;
      }
      if (is_equal)
      {
        free(temporary_string);
        free(words);
        temporary_string = NULL;
        words = NULL;
        return 1;
      }
    }
  }
  free(temporary_string);
  free(words);
  temporary_string = NULL;
  words = NULL;
  return 0;
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief Removes duplicate words.
/// @param words Array of pointers to words. (Array of words)
/// @param input_string Stores the user input.
/// @return Returns the new string without duplicate words, if there were any.
char* removeDuplicates(char** words, char* input_string, int number_of_words)
{
  for (int first_word_index = 0; first_word_index < number_of_words; first_word_index++)
  {
    for (int second_word_index = first_word_index + 1; second_word_index < number_of_words; second_word_index++)
    {
      int is_equal = 1;
      char* first_word = words[first_word_index];
      char* second_word = words[second_word_index];
      int index = 0;
      while (first_word[index] != '\0' || second_word[index] != '\0')
      {
        if (tolower((char)first_word[index]) != tolower((char)second_word[index]))
        {
          is_equal = 0;
          break;
        }
        index++;
      }
      if (is_equal)
      {
        for (int shift_index = second_word_index; shift_index < number_of_words - 1; shift_index++)
        {
          words[shift_index] = words[shift_index + 1];
        }
        number_of_words--;
        second_word_index--;
        words[number_of_words] = NULL;
      }
    }
  }
  return putWordsIntoString(words,input_string);
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief Handles errors and the unique command.
/// @param input_string Stores the user input.
/// @return Returns the original input string if there was an error, NULL if memory allocation failed, the new string
///         without duplicate words otherwise.
char* commandUnique(char* input_string)
{
  int has_duplicates = hasDuplicates(input_string);
  if (isEmpty(input_string) || isWhiteSpace(input_string) || !has_duplicates)
  {
    printf(ERROR_ALL_WORDS_UNIQUE);
    return input_string;
  }
  else if (has_duplicates == -1)
  {
    free(input_string);
    input_string = NULL;
    return NULL;
  }
  int input_length = strlen(input_string);
  char* temporary = malloc((input_length + 1) * sizeof(char));
  if (temporary == NULL)
  {
    return NULL;
  }
  strcpy(temporary, input_string);
  char** words = splitString(temporary);
  if (words == NULL)
  {
    free(temporary);
    temporary = NULL;
    return NULL;
  }
  int number_of_words = 0;
  while (words[number_of_words] != NULL) 
  {
    number_of_words++;
  }
  input_string = removeDuplicates(words, input_string, number_of_words);
  free(words);
  free(temporary);
  words = NULL;
  temporary = NULL;
  currentTextMessage(input_string);
  return input_string;
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief Handles the command menu loop, commands and freeing any memory that isn't being used anymore.
/// @param input_string Stores the user input.
/// @return NULL if memory allocation failed, the input string either same as before if there were errors or the 
///         changed one after successfully completing the command.
char* handleCommands(char* input_string)
{
  removeNewLine(input_string);
  while (1)
  {
    commandList();
    char* command = getSentence();
    if (command == NULL)
    {
      return NULL;
    }
    removeNewLine(command);
    if (strcmp(command, COMMAND_APPEND) == 0)
    {
      input_string = commandAppend(input_string);
    }
    else if (strcmp(command,COMMAND_SEARCH) == 0)
    {
      input_string = commandSearchAndReplace(input_string);
    }
    else if (strcmp(command, COMMAND_QUIT_Q) == 0)
    {
      currentTextMessage(input_string);
      free(input_string);
      input_string = NULL;
      return command;
    }
    else if (strcmp(command, COMMAND_SPLIT) == 0)
    {
      input_string = commandSplitAndSort(input_string);
    }
    else if (strcmp(command, COMMAND_UNIQUE) == 0)
    {
      input_string = commandUnique(input_string);
    }
    else if (isQuit(command))
    {
      free(input_string);
      input_string = NULL;
      return command;
    }
    else
    {
      printf(ERROR_INVALID_COMMAND);
    }
    free(command);
    command = NULL;
    if (input_string == NULL)
    {
      return NULL;
    }
    if (isQuit(input_string))
    {
      return input_string;
    }
    input_string = sizeDown(input_string);
    if (input_string == NULL)
    {
      return NULL;
    }
  }
  return input_string;
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief Handles the start of the program with the first user input and errors.
/// @return 1 if memory allocation failed, 0 if quit was called, -1 if everything was done successfully.
int handleStart(void)
{
  inputMessage();
  char* input_string = getSentence();
  if (input_string == NULL)
  {
    printf(ERROR_NULL);
    return 1;
  }
  removeNewLine(input_string);
  if (isQuit(input_string))
  {
    free(input_string);
    input_string =  NULL;
    return 0;
  }
  input_string = handleCommands(input_string);
  if (input_string == NULL)
  {
    printf(ERROR_NULL);
    free(input_string);
    input_string = NULL;
    return 1;
  }
  removeNewLine(input_string);
  if (isQuit(input_string) || (strcmp(input_string, COMMAND_QUIT_Q) == 0))
  {
    free(input_string);
    input_string = NULL;
    return 0;
  }
  return -1;
}

