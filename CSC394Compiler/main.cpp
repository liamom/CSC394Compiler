#include <iostream>
#include "lex.h"

#include <algorithm>

int main() 
{
  std::vector<std::string> file_list{ 
    "../CSC394Compiler/front.in",
    "../CSC394Compiler/program1.in",
    "../CSC394Compiler/program2.in",
    "../CSC394Compiler/program3.in" };

  LexAnalyzer::Analyzer lex;

  for (auto &file : file_list)
  {
    std::cout << file << std::endl;
    std::vector<LexAnalyzer::Lexeme> symbol_table
      = lex.Analyze(file);

    LexAnalyzer::PrintLexemeVector(symbol_table, std::cout);

    std::cout << "symbol table" << std::endl;
    symbol_table.erase(std::remove_if(symbol_table.begin(), symbol_table.end(), [](LexAnalyzer::Lexeme &a) {return a.token != LexAnalyzer::Token::kId; }), symbol_table.end());
    std::sort(symbol_table.begin(), symbol_table.end());
    auto it = std::unique(symbol_table.begin(), symbol_table.end());
    symbol_table.resize(std::distance(symbol_table.begin(), it));
    LexAnalyzer::PrintLexemeVector(symbol_table, std::cout);
  }

  char a;
  std::cin >> a;
}