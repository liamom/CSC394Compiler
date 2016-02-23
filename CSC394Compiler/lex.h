#ifndef CSCCOMPILER_LEX_H_
#define CSCCOMPILER_LEX_H_

#include <ostream>
#include <fstream>
#include <string>
#include <vector>

namespace LexAnalyzer
{

enum class CharClass {
  kLetter,
  kDidit,
  kUnderscore,
  kUnknown,
  kEof = EOF //-1
};

enum class Token : char {
  kProgram = 0,
  kBegin = 1,
  kEnd = 2,
  kIntLiteral = 3,
  kId = 4,
  kInteger = 5,
  kDeclarer = 6,    // '->'
  kAssignOp = '<',  // '<='   
  kAddOp = '+',
  kSubOp = '-',
  kMultOp = '*',
  kDivOp = '/',
  kLeftParen = '(',
  kRightParen = ')',
  kSemiColon = ';',
  kPeriod = '.',
  kComma = ',',
  kEof = EOF  //-1
};

class Lexeme : public std::string
{
public:
  void addChar(char c) {
    if (this->size() <= 98) {
      this->push_back(c);
    }
    else
      in_error_state = true;
  }

  bool in_error_state = false;
  Token token;
};

class Analyzer
{
public:

  bool open(const std::string &file_path);

  /// <summary>
  /// Reads a file and outputs each lexeme in order to an ostream.
  /// </summary>
  /// <returns>Symbol table</returns>
  std::vector<Lexeme> Analyze();

  /// <summary>
  /// Reads a file and outputs each lexeme in order to an ostream.
  /// </summary>
  /// <returns>Symbol table</returns>
  std::vector<Lexeme> Analyze(const std::string &file_path);

  /// <summary>
  /// Get next lexeme from file
  /// </summary>
  /// <returns></returns>
  Lexeme GetNextLexeme();

private:
  /// <summary>
  /// Get next character that isn't whitespace
  /// </summary>
  /// <returns>The next whitespace character in file_ and move file to that point</returns>
  char GetNextNonBlankChar();
  /// <summary>
  /// Get next character from File
  /// </summary>
  /// <returns>Next character from file</returns>
  char GetNextChar();

  CharClass CharClassOf(const char c);
  Token TokenOf(const char ch);
  Token StringToToken(const Lexeme &lexeme);

private:
  std::fstream file_;
};

void PrintLexemeVector(const std::vector<Lexeme> &symbol_table, std::ostream &stream);

}

#endif  // CSCCOMPILER_LEX_H_