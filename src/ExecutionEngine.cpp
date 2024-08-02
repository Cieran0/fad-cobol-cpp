#include <ExecutionEngine.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <Util.hpp>

ExecutionEngine::ExecutionEngine(vector_2d<Token> program) : program(program)
{
}

void ExecutionEngine::step() {
    assert(!finished, "Execution has finished!");
    std::vector<Token> current_line = program[line];

    switch (current_line[0].type())
    {
    case IDENTIFICATION: {
        assert(current_line.size() == 3, "Wrong length");
        assert(current_line[1] == DIVISION && current_line[2] == END_OF_STATEMENT, "Unexpected error!");
        division = IDENTIFICATION;
        break;
    }
    case DATA: {
        assert(current_line.size() == 3, "Wrong length");
        assert(current_line[1] == DIVISION && current_line[2] == END_OF_STATEMENT, "Unexpected error!");
        division = DATA;
        break;
    }
    case PROCEDURE: {
        assert(current_line.size() == 3, "Wrong length");
        assert(current_line[1] == DIVISION && current_line[2] == END_OF_STATEMENT, "Unexpected error!");
        division = PROCEDURE;
        break;
    }
    case WORKING_STORAGE: {
        assert(division == DATA, "Wrong division!");
        assert(current_line.size() == 3, "Wrong length");
        assert(current_line[1] == SECTION && current_line[2] == END_OF_STATEMENT, "Unexpected error!");
        section = WORKING_STORAGE;
        break;
    }
    case PROGRAM_ID: {
        assert(division == IDENTIFICATION, "Wrong division!");
        assert(current_line.size() == 2, "Wrong length");
        assert(current_line[1] == END_OF_STATEMENT, "Unexpected error!");
        waiting_on_name = true;
        break;
    }
    case UNKNOWN: {
        if(waiting_on_name) {
            assert(current_line.size() == 2, "Wrong length");
            assert(current_line[1] == END_OF_STATEMENT, "Unexpected error!");
            id.program_id = current_line[0].get_src();
            waiting_on_name = false;
            logger << "Name set as: " << id.program_id << std::endl;
        } else if(division == PROCEDURE) {
            assert(current_line.size() == 2, "Wrong length");
            assert(current_line[1] == END_OF_STATEMENT, "Unexpected error!");
            current_procedure = current_line[0].get_src();
            logger << "Entering: " << current_procedure << std::endl;
        }
        else {
            assert(false, "Unexpected token: " + current_line[0].to_string());
        }
        break;
    }
    case NUMBER: {
        assert(division == DATA && section == WORKING_STORAGE, "Unexpected token: " + current_line[0].to_string());
        bool success = try_define_variable(current_line);
        assert(success, "Failed to make variable!");
        
        break;
    }
    case COMPUTE: {
        assert(current_line.size() >= 5, "Wrong length!");
        assert(current_line[1] == UNKNOWN);
        assert(current_line[2] == ASSIGNMENT);

        std::vector<Token> expression(current_line.begin() + 3, current_line.end() - 1);
        std::string result = compute(expression);


        Variable v = get_variable(current_line[1].get_src());
        variables[v.name].value = result;

        logger << "Result: " << variables[v.name].value << std::endl;
        break;
    }
    case DISPLAY: {
        assert(current_line.size() == 4, "Wrong length!");
        assert(current_line[1] == STRING);
        assert(current_line[2] == UNKNOWN);
        assert(current_line[3] == END_OF_STATEMENT);

        Variable v = get_variable(current_line[2].get_src());

        std::cout << current_line[1].get_src() << v.value << std::endl;
        break;
    }
    case STOP: {
        assert(current_line.size() == 3, "Wrong length!");
        assert(current_line[1] == RUN && current_line[2] == END_OF_STATEMENT);
        this->finished = true;
        break;
    }
    default:
        assert(false, "Unexpected token: " + current_line[0].to_string());
        break;
    }


    this->line++;
}

ExecutionEngine::~ExecutionEngine()
{
}

bool ExecutionEngine::execution_complete() {
    return this->finished;
}

Variable& ExecutionEngine::get_variable(const std::string& name) {
    std::unordered_map<std::string, Variable>::iterator iterator = variables.find(name);
    if(iterator != variables.end()) {
        return iterator->second;
    }

    assert(false, "Variable: " + name + " not found!");
    Variable* error = new Variable;
    return *error;
}

std::string ExecutionEngine::compute(std::vector<Token> expression) {

    logger << expression.size() << std::endl;
    assert(expression.size() == 3, "COMPUTE only implemented for single computation, e.g. x + x");

    Variable v1 = get_variable(expression[0].get_src());
    Variable v2 = get_variable(expression[2].get_src());

    std::string result = "";

    assert(v1.type == VT_NUMERIC && v2.type == VT_NUMERIC, "COMPUTE only implemented for numerics!");

    int i1 = std::stoi(v1.value);
    int i2 = std::stoi(v2.value);
    int ir = 0;

    switch (expression[1].type())
    {
    case ADDITION: {
        ir = i1 + i2;
        break;
    }
    case SUBTRACTION: {
        ir = i1 - i2;
        break;
    }
    case MULTIPLICATION: {
        ir = i1 * i2;
        break;
    }
    case DIVIDE: {
        ir = i1 / i2;
        break;
    }
    default:
        assert(false, "Unexpected operation!");
        break;
    }

    result = std::to_string(ir);
    return result;
}


bool ExecutionEngine::try_define_variable(std::vector<Token> variable_line) {
    if(variable_line.size() < 5) {
        return false;
    }

    std::string magic_number = variable_line[0].get_src();

    if(variable_line[1] != UNKNOWN) {
        return false;
    }

    std::string name = variable_line[1].get_src();

    if(variable_line[2] != PIC) {
        return false;
    }
    
    std::string type_and_size = variable_line[3].get_src();

    char type = type_and_size[0];
    VariableType variable_type = VT_UNKNOWN;
    switch (type)
    {
    case '9':
        variable_type = VT_NUMERIC;
        break;
    default:
        break;
    }

    const int size = 1;

    //TODO: actually check size!
    //TODO: let size have spaces e.g. 9 (5) instead of only 9(5)
    if(type_and_size.length() != 1) {

    }

    Variable v = {.magic_number = magic_number, .name = name, .type = variable_type, .size = size, .value = ""};
    if(variable_line[4] == END_OF_STATEMENT) {
        variables.insert({name, v});
        logger << "Made variable: " << v.name << ", value: " << v.value << std::endl;
        return true;
    }

    if(variable_line[4] != VALUE) {
        return false;
    }

    std::string value = variable_line[5].get_src();

    if(variable_line[6] != END_OF_STATEMENT) {
        return false;
    }
        
    v.value = value;
    variables.insert({name, v});
    logger << "Made variable: " << v.name << ", value: " << v.value << std::endl;
    return true;
}