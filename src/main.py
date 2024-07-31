#!/usr/bin/python

import sys

keywords = [
    "IDENTIFICATION", "DIVISION", "PROGRAM-ID", 
    "DATA", "WORKING-STORAGE", "SECTION", 
    "PIC", "VALUE", "PROCEDURE", "COMPUTE", 
    "DISPLAY", "STOP", "RUN"
]

division_types = ['IDENTIFICATION', 'DATA', 'PROCEDURE']

key_symbols = [
   ">=", "<=", "<>", "*>", ">>", "==", "**", "=", "+", "*", "/", ".",
]

def extract_string_literal(line, start):
    looking_for = line[start]
    if start+1 >= len(line):
        raise Exception("string literal broken")
    end = start+1
    
    while line[end] != looking_for:
        end+=1

    return line[start:end+1], end-start + 1


def parse_line(line):
    program_line = []

    if line in ['\n', '\r\n']:
        return

    if len(line) > 72:
        line = line[:72]
        line += '\n'
    
    line = line[7::]
    line = line[:-1]
    
    
    i = 0
    while i < len(line):
        finished = False
        string_builder = ""
        while i < len(line) and not finished:
            if(line[i] in ["\"", "\'"]):
                if(len(string_builder) != 0):
                    raise Exception("string literal broken")
                string_builder, size = extract_string_literal(line, i)
                i += size
                break

            if(line[i] != " ") and not (line[i] in key_symbols):
                string_builder += line[i]
            elif len(string_builder) > 0 and line[i] == " ":
                finished = True
            elif line[i] in key_symbols:
                if(len(string_builder) > 0):
                    program_line.append(string_builder)
                string_builder = line[i]
                finished = True
            i+=1
        program_line.append(string_builder)  

    return program_line

def parse_program():
    program = []
    filename = sys.argv[1]
    with open(filename) as f:
        for line in f:
            program_line = parse_line(line)

            if(type(program_line) is list):
                for snippet in program_line:
                    program.append(snippet)
    return program

def extract_cobol_divisions(lst):
    divisions = {}
    current_division = None
    current_section = []

    for item in lst:
        if item in division_types:
            division_name = item + ' DIVISION'

            if current_division:
                divisions[current_division] = current_section

            if division_name in divisions:
                raise Exception(f"Duplicate division: {item}")

            current_division = division_name
            current_section = [item]
        else:
            if current_section:
                current_section.append(item)
            else:
                current_section = [item]

    if current_division:
        divisions[current_division] = current_section

    return divisions

divisions = extract_cobol_divisions(parse_program())

print(divisions)

