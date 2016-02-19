#include "lex.h"
#include <iomanip>

using namespace LexAnalyzer;

std::vector<Lexeme> Analyzer::Analyze(const std::string &file_path) {
  file_.open(file_path, std::fstream::in);
  std::vector<Lexeme> symbol_table;

  if (file_.is_open()){
    Lexeme lexeme;
		do {
      lexeme = GetNextLexeme();
      //move empties lexeme so the value of its token 
      //needs to be checked in the array
      symbol_table.emplace_back(std::move(lexeme));
		} while (symbol_table.back().token != Token::kEof);
	}

  file_.close();
  return symbol_table;
}

Lexeme Analyzer::GetNextLexeme() {
  char c = GetNextNonBlankChar();
  CharClass char_class = CharClassOf(c);

  Lexeme lexeme;
  if(char_class != CharClass::kEof)
    lexeme.addChar(c);

	switch (char_class) {
  case CharClass::kUnderscore:
	case CharClass::kLetter:
    char_class = CharClassOf(file_.peek());
    while (char_class == CharClass::kLetter 
      || char_class == CharClass::kDidit
      || char_class == CharClass::kUnderscore)
		{
			c = GetNextChar();
      lexeme.addChar(c);
      char_class = CharClassOf(file_.peek());
    }

    lexeme.token = StringToToken(lexeme);
		break;
	case CharClass::kDidit:
    char_class = CharClassOf(file_.peek());
    while (char_class == CharClass::kDidit) 
    {
			c = GetNextChar();
      lexeme.addChar(c);
      char_class = CharClassOf(file_.peek());
    } 
    lexeme.token = Token::kIntLiteral;
		break;
	case CharClass::kUnknown: //Parentheses and operators
    lexeme.token = TokenOf(c);
    if (lexeme.token == Token::kDeclarer || lexeme.token == Token::kAssignOp) {
      c = GetNextChar();
      lexeme.addChar(c);
    }
		break;
	case CharClass::kEof:
    lexeme.token = Token::kEof;
    lexeme.push_back('E');
    lexeme.push_back('O');
    lexeme.push_back('F');
		break;
	}

	return lexeme;
}

char Analyzer::GetNextNonBlankChar() {
  char c = '\0';

  do
  {
    c = GetNextChar();
  } while (c != '\0' && isspace(c));

  return c;
}

char Analyzer::GetNextChar() {
  //read next char
  char c = '\0';

  if (!file_.eof())
    file_ >> std::noskipws >> c;

  return c;
}

CharClass Analyzer::CharClassOf(const char c)
{
  //check if end of file
  if (c == '\0') 
    return CharClass::kEof;

  if (isalpha(c))
    return CharClass::kLetter;
  else if (isdigit(c))
    return CharClass::kDidit;
  else if (c == '_')
    return CharClass::kUnderscore;
  else 
    return CharClass::kUnknown;
}

Token Analyzer::TokenOf(const char ch) {
  switch (ch) {
  case '<':
    if (file_.peek() == '=')
      return Token::kAssignOp;
  case '-':
    if (file_.peek() == '>') 
      return Token::kDeclarer;
    else
      return Token::kSubOp;
  case '+':
  case '*':
  case '/':
  case '(':
  case ')':
  case ';':
  case '.':
  case ',':
    return static_cast<Token>(ch);
  default:
    return Token::kEof;
  }

  return Token::kEof;
}

Token Analyzer::StringToToken(const Lexeme &lexeme)
{
  if (lexeme == "program")
    return Token::kProgram;
  else if (lexeme == "begin")
    return Token::kBegin;
  else if (lexeme == "end")
    return Token::kEnd;
  else if (lexeme == "integer")
    return Token::kInteger;
  else
    return Token::kId;
}

void LexAnalyzer::PrintLexemeVector(const std::vector<Lexeme> &symbol_table, std::ostream &stream)
{
  if (symbol_table.empty())
    stream << ("ERROR - cannot open front.in \n");
  else {
    for (auto &lexeme : symbol_table) {
      if (lexeme.in_error_state)
        stream << "Error - lexeme is too long \n";
      else
        stream << "Next token is: " << std::setw(4)
        << (int)lexeme.token
        << ", Next lexeme is " << lexeme << "\n";
    }
  }
}
