/*
File: plugboard.cpp
Owner: Edward Jenks 
Username: ej3718
Last edited: 21/11/21
Last editor: Edward Jenks
Description: Enigma file to conatin the plugboard class. 
                The assignment is to program an
                enigma machine.
Comments: Commenting style taken from 'Problem Solving with C++ - Savitch'.
*/
#include <iostream>
#include <fstream>
#include <cstring>
#include <cassert>
#include "plugboard.h"
#include "enum.h"
#include "config.h"
#include "errors.h"

/* the plugboard and reflector should use inheritence but that is beyond the scope of this coursework */

/* ------------------------------------------------- */
/* --------------------PLUGBOARD-------------------- */
/* ------------------------------------------------- */

/* default constructor for a plugboard */
Plugboard::Plugboard()
{
}

/* setup constructor for the plugboard */
Plugboard::Plugboard(char const *filename, int &error_code) { set_config(filename, error_code); }

/* copy constructor for the plugboard */
Plugboard::Plugboard(Plugboard const &other) { *this = other; }

/* default destructor */
Plugboard::~Plugboard() {}

/* helper function to check the format for the plugboard file is correct */
void Plugboard::check_file_format(char const *filename, int &error_code) const
{ /* check that a nullptr hasn't been passed to function */
    if (filename != nullptr)
    {
        int str_length = strlen(filename); // find string length
        /* check file is correct format for plugboard */
        if (filename[str_length - 2] != 'p' || filename[str_length - 1] != 'b')
        {
            /* return error message */
            std::cerr << filename << " has an invlaid rotor file format (files should be .pb)." << std::endl;
            error_code = ERROR_OPENING_CONFIGURATION_FILE;
            return;
        }
    }
}

/* helper function to check the ifstream is working properly */
void Plugboard::check_ifstream(std::ifstream &in, char const *filename, int &error_code) const
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

/* helper function to load the plugboard configuration file */
void Plugboard::load_config_file(std::ifstream &in, char const *filename, int &error_code) const
{
    /* check that a nullptr hasn't been passed to function */
    if (filename != nullptr)
    {
        check_file_format(filename, error_code);
        if (error_code != 0)
            return;
        /* open file */
        in.open(filename, std::ifstream::in);
        /* check ifstream for errors */
        check_ifstream(in, filename, error_code);
        if (error_code != 0)
            return;
        assert(in); // assert the in flow
    }
    /* catch nullptr filenames */
    else
    {
        /* return error message */
        std::cerr << "No file to load " << std::endl;
        error_code = ERROR_OPENING_CONFIGURATION_FILE;
        return;
    }
}

/* helper function to check for non-numeric characters; an error is returned if one is found. while this could be made a non-member function and used for all objects, this code is repeated to comply with labts testing. */
void Plugboard::non_numeric_char(std::ifstream &in, char const *filename, int &error_code) const
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
            std::cerr << "Non-numeric character in plugboard file " << filename << std::endl;
            error_code = NON_NUMERIC_CHARACTER;
            return;
        }
    }
    in.seekg(current_pos); // return to previous spot in file
}

/* helper function to check a letter index or array index is within bounds; and error is returned if not */
void Plugboard::check_bounds(int const letter_index, char const *filename, int &error_code) const
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
void Plugboard::check_valid(int const first_mapping_value, int const second_mapping_value, char const *filename, int &error_code) const
{
    /* if mapping exists or function attempts to self-map */
    if (mappings.get_value(first_mapping_value, error_code) != Empty || mappings.get_value(first_mapping_value, error_code) != Empty || first_mapping_value == second_mapping_value)
    {
        /* if error has been raised */
        if (error_code != 0)
            return;
        /* raise error message */
        std::cerr << "Impossible configuration in plugboard file " << filename << std::endl;
        error_code = IMPOSSIBLE_PLUGBOARD_CONFIGURATION;
        return;
    }
}

/* helper function to take mapping information from ifstream and put it into 'mappings' */
void Plugboard::set_mapping_config(std::ifstream &in, char const *filename, int &error_code)
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
            std::cerr << "Incorrect number of parameters in plugboard file " << filename << std::endl;
            error_code = INCORRECT_NUMBER_OF_PLUGBOARD_PARAMETERS;
            return;
        }
        /* check for a non-numeric character */
        non_numeric_char(in, filename, error_code);
        /* if error has been raised */
        if (error_code != 0)
            return;
        /* read second number */
        in >> second_mapping_value >> std::ws;
        /* check that number is within alphabet bounds */
        check_bounds(first_mapping_value, filename, error_code);
        check_bounds(second_mapping_value, filename, error_code);
        /* if error has been raised */
        if (error_code != 0)
            return;
        /* check that number is a valid mapping */
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
    /* if there is too many mapping numbers */
    if (index > 13)
    {
        /* return error message */
        std::cerr << "Incorrect number of parameters in plugboard file " << filename << std::endl;
        error_code = INCORRECT_NUMBER_OF_PLUGBOARD_PARAMETERS;
        return;
    }
    /* set empty mappings to self */
    for (index = 0u; index < 26; index++)
    {
        if (mappings.get_value(index, error_code) == Empty)
            mappings.set_value(index, Alphabet(index), error_code);
        if (error_code != 0)
            return;
    }
}

/* function to load config file and fill the mapping array while checking for errors */
void Plugboard::set_config(char const *filename, int &error_code)
{
    std::ifstream in;
    /* load the configuration file */
    load_config_file(in, filename, error_code);
    /* if error has been raised */
    if (error_code != 0)
        return;
    /* set the plugboard mappings */
    set_mapping_config(in, filename, error_code);
    /* close in stream */
    in.close();
    /* check for failure on close */
    check_ifstream(in, filename, error_code);
    if (error_code != 0)
        return;
}

/* function to print the set mappings of the rotor */
void Plugboard::print_mappings() const
{
    mappings.print();
};

/* function to return the forward (right to left) rotor mapping of a given letter */
Alphabet Plugboard::return_mapping(Alphabet const letter, int &error_code) const
{
    /* chek that letter is within alphabet bounds */
    check_bounds(static_cast<int>(letter), nullptr, error_code);
    /* if error has been raised */
    if (error_code != 0)
        return Empty;
    return mappings.get_value(letter, error_code);
}

/* assigment opporator for a plugboard */
Plugboard &Plugboard::operator=(Plugboard const &other)
{
    /* guard self assignment */
    if (this == &other)
        return *this;
    mappings = other.mappings;
    return *this;
}