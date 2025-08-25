// BufferOverflow.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iomanip>
#include <iostream>
#include <limits> // For std::numeric_limits

int main()
{
  std::cout << "Buffer Overflow Example" << std::endl;

  // TODO: The user can type more than 20 characters and overflow the buffer, resulting in account_number being replaced -
  //  even though it is a constant and the compiler buffer overflow checks are on.
  //  You need to modify this method to prevent buffer overflow without changing the account_number
  //  variable, and its position in the declaration. It must always be directly before the variable used for input.
  //  You must notify the user if they entered too much data.

  const std::string account_number = "CharlieBrown42";
  char user_input[20];


  std::cout << "Enter a value (max 19 characters): ";

  // Using cin.get to restrict input to 19 characters + null terminator
  std::cin.get(user_input, sizeof(user_input));

  // Check if there are still characters left in the input stream (i.e., too much was entered)
  if (std::cin.peek() != '\n') {
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      std::cout << "\n[WARNING] Input was too long and has been truncated for security.\n";
  }
  else {
      // Discard the newline
      std::cin.get();
  }


  std::cout << "You entered: " << user_input << std::endl;
  std::cout << "Account Number = " << account_number << std::endl;

  return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu
