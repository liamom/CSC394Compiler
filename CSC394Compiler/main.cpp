#include <iostream>
#include "lex.h"
#include "parser.h"

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

    Parser parser;
    parser.Parse(symbol_table, &std::cout);

    //LexAnalyzer::PrintLexemeVector(symbol_table, std::cout);
  }

  char a;
  std::cin >> a;
}