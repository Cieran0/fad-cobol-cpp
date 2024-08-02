#pragma once
#include <Token.hpp>
#include <Util.hpp>
#include <unordered_map>

struct Identification
{
    std::string program_id;
};

enum VariableType {
    VT_UNKNOWN,
    VT_NUMERIC,
    VT_STRING,
    VT_ALPHA_NUMERIC,
};

struct Variable
{
    std::string magic_number; //The one at the start cant remember what it does, something about the layout of the data.
    std::string name;
    VariableType type;
    int size;
    std::string value;
};



class ExecutionEngine
{
private:
    vector_2d<Token> program;
    Identification id;
    int line = 0;
    TokenType division = UNKNOWN;
    TokenType section = UNKNOWN;
    bool waiting_on_name = false;
    std::unordered_map<std::string, Variable> variables;
    std::string current_procedure = "";
    bool finished = false;
public:
    ExecutionEngine(vector_2d<Token> program);
    void step();
    ~ExecutionEngine();
    bool try_define_variable(std::vector<Token> variable_line);
    std::string compute(std::vector<Token> expression);
    Variable& get_variable(const std::string& name);
    bool execution_complete();
};
