#pragma once
#include <string>
#include <ostream>
#include <set>

#include "Lex.h"

class Parser
{
public:
  bool Parse(const std::vector<LexAnalyzer::Lexeme> &file, std::ostream *output);

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

private:
  int file_position = 0;
  const std::vector<LexAnalyzer::Lexeme> *file_;
  std::ostream *log_;
  std::set<LexAnalyzer::Lexeme> symbols_;
  bool has_error_ = false;
};

