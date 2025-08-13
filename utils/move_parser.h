#ifndef MOVE_PARSER_H
#define MOVE_PARSER_H

#include <string_view>
#include <regex>

//const char mvRegex[] = "^([KQRBN]?)([a-h1-8]{0,2})(x?)([a-h][1-8])(?:=([QRBN]))?([+#])?$";
const char mvRegex[] = "^(?:([KQRBN]?)([a-h1-8]{0,2})(x?)([a-h][1-8])(?:=([QRBN]))?([+#])?|(O-O(?:-O)?))$";

bool parse_move(const std::string& move, std::smatch& match);

#endif