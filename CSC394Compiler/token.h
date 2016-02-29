#pragma once

#include <ostream>
#include <string>

namespace LexAnalyzer{

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

}

std::ostream& operator<<(std::ostream& o, LexAnalyzer::Token t);