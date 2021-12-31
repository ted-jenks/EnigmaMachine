/*
File: reflector.cpp
Owner: Edward Jenks 
Username: ej3718
Last edited: 21/11/21
Last editor: Edward Jenks
Description: Enigma file to conatin the reflector class. 
                The assignment is to program an
                enigma machine.
Comments: Commenting style taken from 'Problem Solving with C++ - Savitch'.
*/
#include <iostream>
#include <fstream>
#include <cstring>
#include <cassert>
#include "reflector.h"
#include "enum.h"
#include "config.h"
#include "errors.h"

/* the plugboard and reflector should use inheritence but that is beyond the scope of this coursework */

/* ------------------------------------------------- */
/* --------------------REFLECTOR-------------------- */
/* ------------------------------------------------- */

/* default constructor for a reflector */
Reflector::Reflector()
{
}

/* setup constructor for the reflector */
Reflector::Reflector(char const *filename, int &error_code) { set_config(filename, error_code); }

/* copy constructor for the reflector */
Reflector::Reflector(Reflector const &other) { *this = other; }

/* default destructor */
Reflector::~Reflector() {}

/* helper function to check the format for the reflector file is correct */
void Reflector::check_file_format(char const *filename, int &error_code) const
{ /* check that a nullptr hasn't been passed to function */
    if (filename != nullptr)
    {
        int str_length = strlen(filename); // find string length
        /* check file is correct format for reflector */
        if (filename[str_length - 2] != 'r' || filename[str_length - 1] != 'f')
        {
            /* return error message */
            std::cerr << filename << " has an invlaid rotor file format (files should be .rf)." << std::endl;
            error_code = ERROR_OPENING_CONFIGURATION_FILE;
            return;
        }
    }
}

/* helper function to check the ifstream is working properly */
void Reflector::check_ifstream(std::ifstream &in, char const *filename, int &error_code) const
{
    /* catch EOF errors */
    if (in.eof())
        in.clear();
    /* check file stream is ok */
    if (in.fail())
    {
        /* return error message */
        std::cerr << "Error opening/closing " << filename << std::endl;
        error_code = ERROR_OPENING_CONFIGURATION_FILE;
        return;
    }
}

/* helper function to load the reflector configuration file */
void Reflector::load_config_file(std::ifstream &in, char const *filename, int &error_code) const
{
    /* check that a nullptr hasn't been passed to function */
    if (filename != nullptr)
    {
        /* check file is correct format for reflector */
        check_file_format(filename, error_code);
        if (error_code != 0)
            return;
        /* open file */
        in.open(filename, std::ifstream::in);
        /* check file opened successfully */
        check_ifstream(in, filename, error_code);
        if (error_code != 0)
            return;
        assert(in); // assert the in flow
    }
    /* catch nullptr filenames */
    else
    {
        /* return error message */
        std::cerr << "No file to load" << std::endl;
        error_code = ERROR_OPENING_CONFIGURATION_FILE;
        return;
    }
}

/* helper function to check for non-numeric characters; an error is returned if one is found. while this could be made a non-member function and used for all objects, this code is repeated to comply with labts testing. */
void Reflector::non_numeric_char(std::ifstream &in, char const *filename, int &error_code) const
{
    int current_pos;
    char characters[20];

    current_pos = in.tellg(); // store the current position in the file
    in >> characters;         // read the file until the next whitespace

    /* check read characters */
    for (size_t index = 0u; index < strlen(characters); index++)
    {
        /* if not a number */
        if (characters[index] < '0' || characters[index] > '9')
        {
            /* return error message */
            std::cerr << "Non-numeric character in reflector file " << filename << std::endl;
            error_code = NON_NUMERIC_CHARACTER;
            return;
        }
    }
    in.seekg(current_pos); // return to previous spot in file
}

/* helper function to check a letter index or array index is within bounds; and error is returned if not */
void Reflector::check_bounds(int const letter_index, char const *filename, int &error_code) const
{
    /* if out of alphabet bounds */
    if (letter_index < A || letter_index > Z)
    {
        /* if a filename is passed to function, print message */
        if (filename != nullptr)
            std::cerr << "Index out of bounds in " << filename << std::endl;
        /* else print alt message */
        else
            std::cerr << "Invalid index: No possible mapping" << std::endl;
        error_code = INVALID_INDEX;
        return;
    }
}

/* helper function to ensure that a mapping pair is valid in the current mapping array */
void Reflector::check_valid(int const first_mapping_value, int const second_mapping_value, char const *filename, int &error_code) const
{
    /* if mapping exists or function attempts to self-map */
    if (mappings.get_value(first_mapping_value, error_code) != Empty || mappings.get_value(first_mapping_value, error_code) != Empty || first_mapping_value == second_mapping_value)
    {
        /* if error has been raised */
        if (error_code != 0)
            return;
        /* raise error message */
        std::cerr << "Impossible configuration in reflector file " << filename << std::endl;
        error_code = INVALID_REFLECTOR_MAPPING;
        return;
    }
}

/* helper function to take mapping information from ifstream and put it into 'mappings' */
void Reflector::set_mapping_config(std::ifstream &in, char const *filename, int &error_code)
{
    int index = 0u;
    in >> std::ws; // remove leading whitespace
    /* while there is still characters in the file */
    for (; !in.eof(); index++)
    {
        int first_mapping_value, second_mapping_value;
        /* check for a non-numeric character */
        non_numeric_char(in, filename, error_code);
        /* if error has been raised */
        if (error_code != 0)
            return;
        /* read first number */
        in >> first_mapping_value >> std::ws;
        /* if the file has ended (odd number of numbers in file) */
        if (in.eof())
        {
            /* raise error message */
            std::cerr << "Incorrect (odd) number of parameters in reflector file " << filename << std::endl; //FIX THIS WITH FILE NAME
            error_code = INCORRECT_NUMBER_OF_REFLECTOR_PARAMETERS;
            return;
        }
        /* check for non-numeric character */
        non_numeric_char(in, filename, error_code);
        /* if error has been raised */
        if (error_code != 0)
            return;
        /* read second nnumber */
        in >> second_mapping_value >> std::ws;
        /* check that number is within alphabet bounds */
        check_bounds(first_mapping_value, filename, error_code);
        /* if error has been raised */
        if (error_code != 0)
            return;
        /* check that number is within alphabet bounds */
        check_bounds(second_mapping_value, filename, error_code);
        check_valid(first_mapping_value, second_mapping_value, filename, error_code);
        /* if error has been raised */
        if (error_code != 0)
            return;
        /* commit the mapping */
        mappings.set_value(first_mapping_value, Alphabet(second_mapping_value), error_code);
        mappings.set_value(second_mapping_value, Alphabet(first_mapping_value), error_code);
        /* if error has been raised */
        if (error_code != 0)
            return;
    }
    /* check for corruption */
    check_ifstream(in, filename, error_code);
    if (error_code != 0)
        return;
    /* if there is too few mapping numbers */
    if (index < 13)
    {
        /* return error message */
        std::cerr << "Insufficient number of mappings in reflector file: " << filename << std::endl;
        error_code = INCORRECT_NUMBER_OF_REFLECTOR_PARAMETERS;
        return;
    }
    /* if there is too many mapping numbers */
    if (index > 13)
    {
        /* return error message */
        std::cerr << "Too many mappings in reflector file: " << filename << std::endl;
        error_code = INCORRECT_NUMBER_OF_REFLECTOR_PARAMETERS;
        return;
    }
}

/* function to load config file and fill the mapping array while checking for errors */
void Reflector::set_config(char const *filename, int &error_code)
{
    std::ifstream in;
    /* load the configuration file */
    load_config_file(in, filename, error_code);
    /* if error has been raised */
    if (error_code != 0)
        return;
    /* set the reflector mappings */
    set_mapping_config(in, filename, error_code);
    /* close in stream */
    in.close();
    /* check for failure on close */
    check_ifstream(in, filename, error_code);
}

/* function to print the set mappings of the reflector */
void Reflector::print_mappings() const
{
    mappings.print();
};

/* function to return the mapped value of a letter */
Alphabet Reflector::return_mapping(Alphabet const letter, int &error_code) const
{
    /* check the letter is in alphabet bounds */
    check_bounds(static_cast<int>(letter), nullptr, error_code);
    /* if error has been raised */
    if (error_code != 0)
        return Empty;
    return mappings.get_value(letter, error_code);
}

/* assigment opporator for a reflector */
Reflector &Reflector::operator=(Reflector const &other)
{
    /* guard self assignment */
    if (this == &other)
        return *this;

    mappings = other.mappings;

    return *this;
}