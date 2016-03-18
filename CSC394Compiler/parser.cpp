#include "parser.h"

using LexAnalyzer::Token;

std::string Parser::Parse(const std::vector<LexAnalyzer::Lexeme> &file, std::ostream *output)
{
  log_ = output;
  file_ = &file;

  prog();

  if (has_error_)
    return "";


  std::stringstream symbol_table;
  symbol_table << "****************************\n"
              << "***Compilation Successful***\n"
              << "****************************\n"
              << "symbol_table:" << std::endl;
  for (auto &lex : symbol_table_) {
    switch (lex.token) {
    case Token::kBegin:
    case Token::kEnd:
    case Token::kInteger:
      symbol_table << lex << " keyword" << std::endl; break;
    case Token::kProgram:
      symbol_table << lex << " program" << std::endl; break;
    case Token::kId:
      symbol_table << lex << " integer" << std::endl; break;
    }
  }

  for (auto &lex : var_table_) {
    symbol_table << lex.first << " integer " << lex.second << std::endl;
  }

  return "*******Code Generation*******\n" + assembly_output_.str() + symbol_table.str();
}

LexAnalyzer::Lexeme Parser::GetNextLexeme()
{
  file_position++;
  return (*file_)[file_position];
}

LexAnalyzer::Lexeme Parser::peekNextLexeme()
{
  return (*file_)[file_position + 1 ];
}

bool Parser::prog()
{
  log("entering <program>");
  bool isCorrect = false;
  LexAnalyzer::Lexeme lex = file_->at(0);
  
  if (lex.token == Token::kProgram) {
    symbol_table_.insert(lex);
    isCorrect = id_keyword() && op(Token::kSemiColon) && declaration() && body() && op(Token::kPeriod);
  }

  log("exit <program>");
  return isCorrect;
}

bool Parser::body()
{
  log("entering <body>");
  bool isCorrect = op(Token::kBegin) && stat_list() && op(Token::kEnd);
  log("exit <body>");
  return isCorrect;
}

bool Parser::declaration()
{
  log("entering <declaration>");

  bool isCorrect = true;
  if(!null(Token::kInteger))
    isCorrect = op(Token::kInteger) && op(Token::kDeclarer) && id_list() && op(Token::kSemiColon);

  log("exit <declaration>");
  return isCorrect;
}

bool Parser::id_list()
{
  log("entering <id_list>");
  bool isCorrect =  id_declare() && id_list_prime();
  log("exit <id_list>");
  return isCorrect;
}

bool Parser::id_list_prime()
{
  log("entering <id_list_prime>");

  bool isCorrect = true;
  if (!null(Token::kComma))
    isCorrect = op(Token::kComma) && id_declare() && id_list_prime();

  log("exit <id_list_prime>");
  return isCorrect;
}

bool Parser::stat_list()
{
  log("entering <stat_list>");
  bool isCorrect = assign() && stat_list_prime();
  log("exit <stat_list>");
  return isCorrect;
}

bool Parser::stat_list_prime()
{
  log("entering <stat_list_prime>");

  bool isCorrect = true;
  if(!null(Token::kSemiColon))
    isCorrect =  op(Token::kSemiColon) && assign() && stat_list_prime();

  log("exit <stat_list_prime>");
  return isCorrect;
}

bool Parser::assign()
{
  log("entering <assign>");
  LexAnalyzer::Lexeme id = peekNextLexeme();
  bool isCorrect = id_use() && op(Token::kAssignOp) && expression();

  if(isCorrect)
    add_assembly("POP", id);

  log("exit <assign>");
  return isCorrect;
}

bool Parser::expression()
{
  log("entering <expression>");
  bool isCorrect = term() && expression_prime();
  log("exit <expression>");
  return isCorrect;
}

bool Parser::expression_prime()
{
  log("entering <expression_prime>");
  bool isCorrect = false;

  const auto &token = peekNextLexeme().token;
  switch (token)
  {
  case Token::kAddOp:
    isCorrect = true;
    op(Token::kAddOp) && /*term() &&*/ expression();
    assembly_output_ << "ADD" << std::endl;
    break;
  case Token::kSubOp:
    isCorrect = true;
    op(Token::kSubOp) && /*term() &&*/ expression();
    assembly_output_ << "SUB" << std::endl;
    break;
  default://null
    isCorrect = true;
    break;
  }

  log("exit <expression_prime>");
  return isCorrect;
}

bool Parser::term()
{
  log("entering <term>");
  bool isCorrect = false;

  const auto &token = peekNextLexeme().token;
  switch (token)
  {
  case Token::kIntLiteral: {
    auto int_lit = peekNextLexeme();
    isCorrect = op(Token::kIntLiteral);

    if (isCorrect)
      add_assembly("PUSH", int_lit);

    break; }
  case Token::kId: {
    auto id = peekNextLexeme();
    isCorrect = op(Token::kId);

    if (isCorrect)
      add_assembly("PUSH", id);
    break; }
  case Token::kLeftParen:
    isCorrect = op(Token::kLeftParen) && id_use() && op(Token::kRightParen);
    break;
  }

  log("exit <term>");
  return isCorrect;
}

bool Parser::id_declare()
{
  LexAnalyzer::Lexeme lex = GetNextLexeme();
  if (lex.token == Token::kId) {
    auto it = var_table_.find(lex);
    //if found, already declared
    if (it == var_table_.end()) {
      assembly_output_ << "INT <" << lex << "> " << id_counter << std::endl;
      var_table_.insert({ lex, id_counter++ });
      return true;
    }
    else {
      has_error_ = true;
      *log_ << "ERROR -- identifier " << lex << " already declared.\n";
      return false;
    }
  }
  else
    return false;
}

bool Parser::id_use() 
{
  LexAnalyzer::Lexeme lex = GetNextLexeme();
  if (lex.token == Token::kId) {
    auto it = var_table_.find(lex);
    //if found, already declared
    if (it == var_table_.end()) {
      has_error_ = true;
      *log_ << "ERROR -- identifier " << lex << " used before declaration.\n";
      return false;
    }
    else {
      return true;
    }
  }
  else
    return false;
}

bool Parser::id_keyword()
{
  return GetNextLexeme().token == Token::kId;
}

bool Parser::op(Token token)
{
  LexAnalyzer::Lexeme lex = GetNextLexeme();
  if (lex.token == token) {
    switch (token) {
    case Token::kBegin:
    case Token::kEnd:
    case Token::kInteger:
    case Token::kProgram:
    case Token::kId:
      symbol_table_.insert(lex);
    }

    return true;
  }
  else {
    has_error_ = true;
    *log_ << "ERROR -- expected token " << token << " found: " << lex.token << std::endl;
    return false;
  }
}

bool Parser::null(Token expected_token)
{
  return peekNextLexeme().token != expected_token;
}

void Parser::log(std::string str)
{
  *log_ << str << std::endl;
}

void Parser::add_assembly(std::string str, LexAnalyzer::Lexeme lex)
{
  if (lex.token == Token::kId) {
    auto symbol = var_table_.find(lex);
    assembly_output_ << str << " " << symbol->first << " " << symbol->second << std::endl;
  }
  else
    assembly_output_ << str << " " << lex << std::endl;
}
