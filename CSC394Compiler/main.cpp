#include <sstream>
#include <iostream>
#include "lex.h"
#include "parser.h"

#include <algorithm>

int main() 
{
  std::vector<std::string> file_list{
    //"../CSC394Compiler/error1.in",
    //"../CSC394Compiler/error2.in",
    //"../CSC394Compiler/error3.in",
    "../CSC394Compiler/program1.in",
    "../CSC394Compiler/program2.in",
    "../CSC394Compiler/program3.in" };

  LexAnalyzer::Analyzer lex;

  for (auto &file : file_list)
  {
    std::cout << file << std::endl;
    std::vector<LexAnalyzer::Lexeme> symbol_table
      = lex.Analyze(file);

    //std::stringstream log;

    Parser parser;
    std::string code = parser.Parse(symbol_table, &std::cout);

    std::cout << code;
  }

  char a;
  std::cin >> a;
}