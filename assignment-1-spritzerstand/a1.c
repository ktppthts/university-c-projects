//---------------------------------------------------------------------------------------------------------------------
/// This is a Spritzerstand simulation, where you can choose between returning glasses or ordering drinks.
/// 
///
/// Author: 12326821
//---------------------------------------------------------------------------------------------------------------------
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define SMALL_GLASS_SIZE 300
#define BIG_GLASS_SIZE 500

typedef struct _GlassReturn_
{
  int number_of_glasses_;
  int glass_size_;
  float deposit_refund_, total_refund_;
} GlassReturn;

typedef struct _DrinkOrder_
{
  int drink_number_;
  int glass_size_;
  int tip_;
  float drink_cost_;
  char drink_name_[50];
  float deposit_fee_;
} DrinkOrder;

//---------------------------------------------------------------------------------------------------------------------
/// @brief This function prints the welcome message at the beginning of the program.
void printWelcomeMessage(void)
{
    printf("Welcome to the Spritzerstand Simulator!!!\n");
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief This function shows the glasses wished to be returned dialogue.
/// @param glass_return_ Pointer to the GlassReturn structure. Used to pass it to other functions.
void numberOfGlassesToReturnDialogue(GlassReturn *glass_return_)
{
  bool valid_input = false;
  while (!valid_input)
  { 
    printf("\nHow many glasses do you want to return?\n > ");
    scanf("%d", &glass_return_->number_of_glasses_);
    if (glass_return_->number_of_glasses_ <= 0)
    {
      printf("[Error]: How do you plan on returning that?\n");
    }
    else
    {
      valid_input = true;
    }

  }
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief This function shows the glass size dialogue when returning glasses.
/// @param glass_return_ Pointer to the GlassReturn structure. Used to pass it to other functions.
void glassSizeReturnDialogue(GlassReturn *glass_return_)
{
  bool valid_input = false;
  while (!valid_input)
  { 
    printf("\nSelect a glass size (in ml)\n");
    printf("- 300\n");
    printf("- 500\n");
    printf(" > ");
    scanf("%d", &glass_return_->glass_size_);
    if (glass_return_->glass_size_ != SMALL_GLASS_SIZE && glass_return_->glass_size_ != BIG_GLASS_SIZE)
    {
      printf("[Error]: Sorry that glass size is invalid.\n"); 
    }
    else
    {
      if(glass_return_->glass_size_ == SMALL_GLASS_SIZE)
      {
        glass_return_->deposit_refund_ = 1.5;
      }
      else
      {
        glass_return_->deposit_refund_ = 2;
      } 
      valid_input = true;
    }
 }
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief This function shows the glass size dialogue when ordering drinks.
/// @param drink_order Pointer to the GlassReturn structure. Used to pass it to other functions.
void glassSizeOrderDialogue(DrinkOrder *drink_order_)
{
  bool valid_input = false;
  while (!valid_input)
  { 
    printf("\nSelect a glass size (in ml)\n");
    printf("- 300\n");
    printf("- 500\n");
    printf(" > ");
    scanf("%d", &drink_order_->glass_size_);
    if (drink_order_->glass_size_ != SMALL_GLASS_SIZE && drink_order_->glass_size_ != BIG_GLASS_SIZE)
    {
      printf("[Error]: Sorry that glass size is invalid.\n");
    }
    else
    {
      if(drink_order_->glass_size_ == SMALL_GLASS_SIZE)
      {
        drink_order_->deposit_fee_ = 1.5;
      }
      else
      {
        drink_order_->deposit_fee_ = 2;
      } 
      valid_input = true;
    }
 }
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief This function shows the receipt of returned glasses, including the total cashback.
/// @param glass_return_ Pointer to the GlassReturn structure. Used to pass it to other functions.
void returnedGlassesMessage(GlassReturn *glass_return_)
{
  glass_return_->total_refund_ = glass_return_->number_of_glasses_ * glass_return_->deposit_refund_;
  printf("\n--------------------------------\n");
  printf("You returned %d glasses at %.2f € each.\n", glass_return_->number_of_glasses_, glass_return_->deposit_refund_);
  printf("Your total cashback is: %.2f €\n", glass_return_->total_refund_);
  printf("--------------------------------\nHere you go... have a pleasant evening!\n");
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief This function shows the drink menu.
/// @param drink_order Pointer to the GlassReturn structure. Used to pass it to other functions.
void drinkMenu(DrinkOrder *drink_order_)
{
  bool valid_input = false;
  while (!valid_input)
  { 
    printf("\n");
    printf("What are you feeling today?\n");
    printf("1. Virgin Cuba Libre\n");
    printf("2. Carbonas rhoncus bibendum\n");
    printf("3. Fizzy water\n");
    printf(" > ");
    scanf("%d", &drink_order_->drink_number_);
    switch (drink_order_->drink_number_)
    {
      case 1:
        drink_order_->drink_cost_ = 2.5;
        strcpy(drink_order_->drink_name_, "Virgin Cuba Libre");
        valid_input = true;
        break;
      case 2:
        drink_order_->drink_cost_ = 3;
        strcpy(drink_order_->drink_name_, "Carbonas rhoncus bibendum");
        valid_input = true;
        break;
      case 3:
        drink_order_->drink_cost_ = 1;
        strcpy(drink_order_->drink_name_, "Fizzy water");
        valid_input = true;
        break;
      default:
        printf("[Error]: Hmm I don't seem to have that drink.\n");
        break;
    }
  }
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief This function is used to ask how much you want to tip_ when ordering drinks.
/// @param drink_order_ Pointer to the GlassReturn structure. Used to pass it to other functions.
void tipDialogue(DrinkOrder *drink_order_)
{
  bool valid_input = false;
  while (!valid_input)
  { 
    printf("\nHow much would you like to tip? (in %%)\n > ");
    scanf("%d", &drink_order_->tip_);
    if (drink_order_->tip_ < 0 || drink_order_->tip_ > 100)
    {
      printf("[Error]: I cannot accept that. Please insert an amount from 0 to 100.\n");
    }
    else
    {
      valid_input = true;
    }
  }
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief This function shows the receipt when ordering a drink.
/// @param drink_order_ Pointer to the GlassReturn structure. Used to pass it to other functions.
void printReceipt(DrinkOrder *drink_order_)
{
  float total_without_tip = drink_order_->drink_cost_ + drink_order_->deposit_fee_;
  float tip_amount = total_without_tip * drink_order_->tip_ / 100.0;
  float total_with_tip = total_without_tip + tip_amount;
  printf("\n--------------------------------\n");
  printf("- %d ml glass of %s\n", drink_order_->glass_size_,drink_order_->drink_name_);
  printf("- Drink price: %.2f €\n", drink_order_->drink_cost_);
  printf("- Deposit: %.2f €\n", drink_order_->deposit_fee_);
  printf("- Total without tip: %.2f €\n", total_without_tip);
  printf("- Tip: %d %%\n", drink_order_->tip_);
  printf("- Total: %.2f €", total_with_tip);
  printf("\n--------------------------------\n");
  printf("Here you go... have a pleasant evening!\n");
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief This function is the main function for the first option of the main menu, returning glasses.
void glassReturn(void)
{
  GlassReturn glass_return_;
  numberOfGlassesToReturnDialogue(&glass_return_);
  glassSizeReturnDialogue(&glass_return_);
  returnedGlassesMessage(&glass_return_);
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief This is the main function for the second option of the main menu, ordering a drink.
void drinkOrder(void)
{
  DrinkOrder drink_order_;
  drinkMenu(&drink_order_);
  glassSizeOrderDialogue(&drink_order_);
  tipDialogue(&drink_order_);
  printReceipt(&drink_order_);
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief This function shows the main menu and handles user input.
int mainMenu(void)
{
  while (true)
  {
    int user_input;
    printf("\nHow can I assist you today?\n");
    printf("1. I'd like to return my glasses.\n");
    printf("2. I'd like to order.\n");
    printf("3. Quit\n");
    printf(" > ");
    scanf("%d", &user_input);
    switch (user_input)
    {
      case 1:
        glassReturn();
        break;
      case 2:
        drinkOrder();
        break;
      case 3:
        return 0;
        break;
      default:
        printf("[Error]: Please choose a valid option!\n");
        break;
    }
  }
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief This is the main function.
int main(void)
{
  printWelcomeMessage();
  mainMenu();
  return 0;
}
