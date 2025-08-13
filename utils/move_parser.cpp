#include "move_parser.h"

bool parse_move(const std::string& move, std::smatch& match)
{
    static const std::regex move_regex(mvRegex);
    return std::regex_match(move, match, move_regex);
}