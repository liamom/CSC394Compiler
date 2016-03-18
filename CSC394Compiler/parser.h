#pragma once
#include <string>
#include <sstream>
#include <ostream>
#include <set>
#include <map>

#include "Lex.h"

class Parser
{
public:
  std::string Parse(const std::vector<LexAnalyzer::Lexeme> &file, std::ostream *output);

private:
  LexAnalyzer::Lexeme GetNextLexeme();
  LexAnalyzer::Lexeme peekNextLexeme();

  bool prog();
  bool body();
  bool declaration();
  bool id_list();
  bool id_list_prime();
  bool stat_list();
  bool stat_list_prime();
  bool assign();
  bool expression();
  bool expression_prime();
  bool term();
  bool id_declare();
  bool id_use();
  bool id_keyword();
  bool op(LexAnalyzer::Token token);
  bool null(LexAnalyzer::Token expected_token);

  void log(std::string str);
  void add_assembly(std::string str, LexAnalyzer::Lexeme lex);

private:
  int file_position = 0;
  const std::vector<LexAnalyzer::Lexeme> *file_;
  std::ostream *log_;
  std::map<LexAnalyzer::Lexeme, int> var_table_;
  std::set<LexAnalyzer::Lexeme> symbol_table_;
  int id_counter = 100;
  std::stringstream assembly_output_;
  bool has_error_ = false;
};

