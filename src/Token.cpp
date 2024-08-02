#include <Token.hpp>
#include <unordered_map>
#include <Util.hpp>

static const std::unordered_map<std::string, TokenType> strToTokenType = {
        { "IDENTIFICATION", IDENTIFICATION },
        { "DIVISION", DIVISION },
        { ".", END_OF_STATEMENT },
        { "PROGRAM-ID", PROGRAM_ID },
        { "DATA", DATA },
        { "WORKING-STORAGE", WORKING_STORAGE },
        { "SECTION", SECTION },
        { "PIC", PIC },
        { "VALUE", VALUE },
        { "PROCEDURE", PROCEDURE },
        { "COMPUTE", COMPUTE },
        { "=", ASSIGNMENT },
        { "+", ADDITION },
        { "-", SUBTRACTION },
        { "*", MULTIPLICATION },
        { "/", DIVIDE },
        { "STOP", STOP },
        { "DISPLAY", DISPLAY },
        { "RUN", RUN },
};

TokenType get_type_from_string(std::string src) {
    std::unordered_map<std::string, TokenType>::const_iterator iterator = strToTokenType.find(src);
    if(iterator != strToTokenType.end()) {
        return iterator->second;
    }

    if(is_number(src)) {
        return NUMBER;
    }

    if(src[0] == '"' && src.back() == '"'){
        return STRING;
    }

    return UNKNOWN;
}

Token::Token(std::string src)
{
    this->src = src;
    this->token_type=get_type_from_string(src);
    if(this->token_type == STRING){
        this->src = src.substr(1,src.length()-2);
    }

}

Token::~Token()
{
}


const std::string TypeString[] = {     
    "UNKNOWN",
    "IDENTIFICATION",
    "DIVISION",
    "END_OF_STATEMENT",
    "PROGRAM_ID",
    "DATA",
    "WORKING_STORAGE",
    "SECTION",
    "PIC",
    "VALUE",
    "PROCEDURE",
    "COMPUTE",
    "ASSIGNMENT",
    "ADDITION",
    "SUBTRACTION",
    "MULTIPLICATION",
    "DIVIDE",
    "STOP",
    "DISPLAY",
    "RUN", 
    "NUMBER",
    "STRING",
};

std::string Token::to_string() {
    if(this->token_type >= (sizeof(TypeString) / sizeof(std::string))) {
        //ERROR
        exit(-1);
    }

    if(this->token_type == UNKNOWN || this->token_type == NUMBER || this->token_type == STRING) {
        return "[ " + TypeString[this->token_type] + ": " + this->src + " ]";
    }

    return "[ " + TypeString[this->token_type] + " ]";
}

TokenType Token::type() {
    return this->token_type;
}

bool Token::operator==(TokenType t) const {
    return this->token_type == t;
}

bool Token::operator!=(TokenType t) const {
    return this->token_type != t;
}

std::string Token::get_src() {
    return this->src;
}