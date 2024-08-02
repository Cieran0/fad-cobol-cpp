#pragma once

#include <string>
#include <vector>
#include <TokenType.hpp>

class Token
{
private:
    std::string src;
    TokenType token_type;
public:
    Token(std::string src);
    ~Token();
    std::string to_string();
    TokenType type();
    bool operator==(TokenType t) const;
    bool operator!=(TokenType t) const;
    std::string get_src();
};