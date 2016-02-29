#include "token.h"

using namespace LexAnalyzer;

std::ostream& operator<<(std::ostream& o, Token t){
   switch (t)
   {
   case Token::kProgram:
     o << "Program";
     break;
   case Token::kBegin:
     o << "Begin";
     break;
   case Token::kEnd:
     o << "End";
     break;
   case Token::kIntLiteral:
     o << "Int";
     break;
   case Token::kId:
     o << "ID";
     break;
   case Token::kInteger:
     o << "Integer";
     break;
   case Token::kDeclarer:
     o << "->";
     break;
   case Token::kAssignOp:
     o << "<=";
     break;
   case Token::kAddOp:
   case Token::kSubOp:
   case Token::kMultOp:
   case Token::kDivOp:
   case Token::kLeftParen:
   case Token::kRightParen:
   case Token::kSemiColon:
   case Token::kPeriod:
   case Token::kComma:
   case Token::kEof:
     o << static_cast<char>(t);
     break;
   default:
     o << "Unknown token";
   }

   return o;
}