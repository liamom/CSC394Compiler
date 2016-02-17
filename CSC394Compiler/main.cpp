#include <iostream>
#include <iomanip>

#include "lex.h"

int main() 
{
	Lex lex;
  std::vector<Lex::Lexeme> symbol_table 
    = lex.Analyze("../CSC394Compiler/front.in");

  if(symbol_table.empty())
    std::cout << ("ERROR - cannot open front.in \n");
  else {
    for (auto &lexeme : symbol_table) {
      if (lexeme.in_error_state)
        std::cout << "Error - lexeme is too long \n";
      else
        std::cout << "Next token is: " << std::setw(4)
                  << (int)lexeme.token
                  << ", Next lexeme is " << lexeme << "\n";
    }
  }

	char a;
	std::cin >> a;
}