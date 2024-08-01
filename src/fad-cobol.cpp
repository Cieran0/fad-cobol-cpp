#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <cctype>

std::vector<char> key_symbols_chars = {
   //">=", "<=", "<>", "*>", ">>", "==", "**", 
   '=', '+', '*', '/', '.',
};

template <typename T>
bool contains(const std::vector<T>& vec, const T& value) {
    return std::find(vec.begin(), vec.end(), value) != vec.end();
}

std::vector<std::string> read_file(char* filepath) {
    std::vector<std::string> output;
    std::string line;

    std::ifstream file(filepath);
    
    // Check if the file was opened successfully
    if (!file) {
        std::cerr << "Unable to open file: " << filepath << std::endl;
        exit(-1);
    }

    while (std::getline(file, line)) {
        line += '\n';
        output.push_back(line);
    }

    file.close();

    return output;
}

bool is_fully_whitespace(const std::string& str) {
    return std::all_of(str.begin(), str.end(), [](unsigned char c) {
        return std::isspace(c);
    });
}

std::string extract_string_literal(std::string line, unsigned int start, int* size_buffer) {
    char looking_for = line[start];
    if (start+1 >= line.length()) {
        //raise Exception("string literal broken")
    }

    int end = start+1;
    
    while (line[end] != looking_for){
        end+=1;
    }

    int size = end-start + 1;
    *size_buffer = size;

    return line.substr(start,size);
    //return line[start:end+1], end-start + 1
}

std::vector<std::string> parse_line(std::string line) {
    std::vector<std::string> program_line;

    if(line.length() < 8 || is_fully_whitespace(line)) {
        return program_line;
    }

    if(line.length() > 72) {
        line = line.substr(0,72);
        line += "\n";
    }

    line = line.substr(7);
    line.pop_back();

    unsigned int i = 0;
    int size = 0;
    while (i < line.length())
    {
        bool finished = false;
        std::string string_builder = "";
        while (i < line.length() && !finished)
        {
            if(line[i] == '\"' || line[i] == '\'') {
                if((string_builder.length()) != 0) {
                    //raise Exception("string literal broken")
                }
                string_builder = extract_string_literal(line, i, &size);
                i += size;
                break;
            }

            if(line[i] != ' ' && !contains(key_symbols_chars, line[i])) {
                string_builder += line[i];
            }
            else if ((string_builder.length()) > 0 && line[i] == ' ') {
                finished = true;
            }
            else if(contains(key_symbols_chars, line[i])){
                if(string_builder.length() > 0){
                        program_line.push_back(string_builder);
                }
                string_builder = line[i];
                finished = true;
            }
            i++;
        }
        program_line.push_back(string_builder);
        
    }
    
    return program_line;
}

std::vector<std::string> parse_program(std::vector<std::string> file) {
    std::vector<std::string> program;

    for (const std::string& line : file) {
        for(const std::string& snippet : parse_line(line)) {
            program.push_back(snippet);    
        }
    }

    return program;
}

int main(int argc, char** argv) {
    if(argc < 2){
        return -1;
    }

    char* filepath = argv[1];

    std::vector<std::string> file = read_file(filepath);

    std::vector<std::string> program = parse_program(file);

    for (const std::string& code : program) { 
        std::cout << code << std::endl;
    }


    return 0;
}