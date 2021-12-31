/*
File: rotor.cpp
Owner: Edward Jenks 
Username: ej3718
Last edited: 21/11/21
Last editor: Edward Jenks
Description: Enigma file to conatin the rotor class. 
                The assignment is to program an
                enigma machine.
Comments: Commenting style taken from 'Problem Solving with C++ - Savitch'.
*/
#include <iostream>
#include <fstream>
#include <cstring>
#include <cassert>
#include "rotor.h"
#include "enum.h"
#include "config.h"
#include "errors.h"

/* ------------------------------------------------- */
/* ----------------------ROTOR---------------------- */
/* ------------------------------------------------- */

/* default constructor for a rotor */
Rotor::Rotor()
{
}

/* setup constructor for the rotor */
Rotor::Rotor(char const *filename, int &error_code) { set_config(filename, error_code); }

/* copy constructor for the rotor */
Rotor::Rotor(Rotor const &other) { *this = other; }

/* default destructor */
Rotor::~Rotor() {}

/* helper function to check the format for the rotor file is correct */
void Rotor::check_file_format(char const *filename, int &error_code) const
{ /* check that a nullptr hasn't been passed to function */
    if (filename != nullptr)
    {
        int str_length = strlen(filename);
        /* check file is correct format for reflector */
        if (filename[str_length - 3] != 'r' || filename[str_length - 2] != 'o' || filename[str_length - 1] != 't')
        {
            /* return error message */
            std::cerr << filename << " has an invlaid rotor file format (files should be .rot)." << std::endl;
            error_code = ERROR_OPENING_CONFIGURATION_FILE;
            return;
        }
    }
}

/* helper function to check the ifstream is working properly */
void Rotor::check_ifstream(std::ifstream &in, char const *filename, int &error_code) const
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

/* helper function to load the rotor configuration file */
void Rotor::load_config_file(std::ifstream &in, char const *filename, int &error_code) const
{
    /* check that a nullptr hasn't been passed to function */
    if (filename != nullptr)
    {
        check_file_format(filename, error_code);
        if (error_code != 0)
            return;
        /* open file */
        in.open(filename, std::ifstream::in);
        /* check file opened successfully */
        check_ifstream(in, filename, error_code);
        if (error_code != 0)
            return;

        assert(in);
        // assert the in flow
    }
    /* catch nullptr filenames */
    else
    {
        /* return error message */
        std::cerr << "Error opening" << filename << std::endl;
        error_code = ERROR_OPENING_CONFIGURATION_FILE;
        return;
    }
}

/* helper function to check for non-numeric characters; an error is returned if one is found. while this could be made a non-member function and used for all objects, this code is repeated to comply with labts testing. */
void Rotor::non_numeric_char(std::ifstream &in, char const *filename, int &error_code) const
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
            std::cerr << "Non-numeric character for mapping in rotor file " << filename << std::endl;
            error_code = NON_NUMERIC_CHARACTER;
            return;
        }
    }
    in.seekg(current_pos); // return to previous spot in file
}

/* helper function to check the current output mappings for a clash */
void Rotor::check_outputs(int const map_number, int const index, char const *filename, int &error_code) const
{
    for (int secondary_index = 0u; secondary_index < index; secondary_index++)
    {
        if (mappings.get_value(secondary_index, error_code) == map_number)
        {
            /* if error raised */
            if (error_code != 0)
                return;
            /* return error message */
            std::cerr << "Invalid mapping of input " << index << " to output " << map_number << " (output " << map_number << " is already mapped to from input " << secondary_index << ") in rotor file: " << filename << std::endl;
            error_code = INVALID_ROTOR_MAPPING;
            return;
        }
    }
}

/* helper function to check the current input mappings for a clash */
void Rotor::check_inputs(int const map_number, int const index, char const *filename, int &error_code) const
{
    if (mappings.get_value(index, error_code) != Empty)
    {
        /* if error raised */
        if (error_code != 0)
            return;
        /* return error message */
        std::cerr << "Invalid mapping of output " << map_number << " to input " << index << " (input " << index << " is already mapped to from output " << mappings.get_value(index, error_code) << ") in rotor file: " << filename << std::endl;
        error_code = INVALID_ROTOR_MAPPING;
        return;
    }
}

/* helper function to check a letter index or array index is within bounds; and error is returned if not */
void Rotor::check_bounds(int const letter_index, char const *filename, int &error_code) const
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

/* helper function to take mapping information from ifstream and put it into 'mappings' */
void Rotor::set_mapping_config(std::ifstream &in, char const *filename, int &error_code)
{
    int map_number;

    in >> std::ws; // remove leading whitespace

    /* while there is still characters in the file */
    for (int index = 0u; index < 26; index++)
    {
        /* check if we've ran out of characters */
        if (in.eof())
        {
            /* return error message */
            std::cerr << "Not all inputs mapped in rotor file: " << filename << std::endl;
            error_code = INVALID_ROTOR_MAPPING;
            return;
        }
        /* check for a non-numeric character */
        non_numeric_char(in, filename, error_code);
        /* if error has been raised */
        if (error_code != 0)
            return;
        /* read first number */
        in >> map_number >> std::ws;
        /* check the output isn't already mapped */
        check_outputs(map_number, index, filename, error_code);
        check_inputs(map_number, index, filename, error_code);
        /* if error has been raised */
        if (error_code != 0)
            return;
        check_bounds(map_number, filename, error_code);
        /* if error has been raised */
        if (error_code != 0)
            return;
        /* commit the mapping */
        mappings.set_value(index, Alphabet(map_number), error_code);
    }
    /* check for corruption */
    check_ifstream(in, filename, error_code);
    if (error_code != 0)
        return;
}

/* helper function to take notch information from ifstream and put it into 'notch' */
void Rotor::set_notch_config(std::ifstream &in, char const *filename, int &error_code)
{
    /* while there is still characters in the file */
    for (int index = 0u; !in.eof(); index++)
    {
        int notch_number;

        /* check for non-numeric character */
        non_numeric_char(in, filename, error_code);
        /* if error has been raised */
        if (error_code != 0)
            return;
        /* read character */
        in >> notch_number >> std::ws;
        /* check letter is within alphabet bounds */
        check_bounds(notch_number, filename, error_code);
        /* if error has been raised */
        if (error_code != 0)
            return;

        /* commit notches */
        notches.set_value(index, Alphabet(notch_number), error_code);
    }
}

/* function to load config file and fill the mapping and notch array while checking for errors */
void Rotor::set_config(char const *filename, int &error_code)
{
    std::ifstream in;

    /* load the configuration file */
    load_config_file(in, filename, error_code);
    /* if error has been raised */
    if (error_code != 0)
        return;
    /* set the rotor mappings */
    set_mapping_config(in, filename, error_code);
    /* if error has been raised */
    if (error_code != 0)
        return;
    /* set the rotor notch */
    set_notch_config(in, filename, error_code);
    /* if error has been raised */
    if (error_code != 0)
        return;
    /* close in stream */
    in.close();
    /* check for failure on close */
    check_ifstream(in, filename, error_code);
    if (error_code != 0)
        return;
}

/* function to return the relative position of the rotor */
void Rotor::set_rel_pos(Alphabet const starting_pos, int &error_code)
{
    /* check the starting positions are within alphabet bounds */
    check_bounds(static_cast<int>(starting_pos), nullptr, error_code);
    /* if error has been raised */
    if (error_code != 0)
        return;
    /* set relative position */
    rel_pos = starting_pos;
}

/* function to print the set mappings of the rotor */
void Rotor::print_mappings() const
{
    mappings.print();
};

/* function to print the set notches of the rotor */
void Rotor::print_notches() const
{
    notches.print();
};

/* function to return the relative position of the rotor */
Alphabet Rotor::get_rel_pos(int &error_code) const
{
    return rel_pos;
}

/* function to rotatethe rotor; returns true if notch is triggered */
bool Rotor::rotate(int &error_code)
{
    rel_pos++; // oporator deals with wrap around
    return notches.check_value(rel_pos, error_code);
}

/* function to return the mapped value of a letter right to left */
Alphabet Rotor::return_mapping(Alphabet const letter, int &error_code) const
{
    /* check the letter is in alphabet bounds */
    check_bounds(static_cast<int>(letter), nullptr, error_code);
    /* if error has been raised */
    if (error_code != 0)
        return Empty;
    /* account for relative position */
    Alphabet adjusted_letter = Alphabet((letter + rel_pos + 26) % 26);
    Alphabet mapped_value = mappings.get_value(adjusted_letter, error_code);
    return Alphabet((mapped_value - rel_pos + 26) % 26);
}

/* function to return the mapped value of a letter left to right */
Alphabet Rotor::return_reverse_mapping(Alphabet const letter, int &error_code) const
{
    /* check the letter is in alphabet bounds */
    check_bounds(static_cast<int>(letter), nullptr, error_code);
    /* if error has been raised */
    if (error_code != 0)
        return Empty;
    /* account for relative position */
    Alphabet adjusted_letter = Alphabet((letter + rel_pos + 26) % 26);
    Alphabet mapped_value = Alphabet(mappings.get_index(adjusted_letter, error_code));
    return Alphabet((mapped_value - rel_pos + 26) % 26);
}

/* function to return the notch at a given position */
Alphabet Rotor::return_notch(int const index, int &error_code) const
{
    /* check the letter is in alphabet bounds */
    check_bounds(static_cast<int>(index), nullptr, error_code);
    /* if error has been raised */
    if (error_code != 0)
        return Empty;
    return notches.get_value(index, error_code);
}

/* assignment opporator for a rotor */
Rotor &Rotor::operator=(Rotor const &other)
{
    /* guard self assignment */
    if (this == &other)
        return *this;

    if (rel_pos != other.rel_pos)
        rel_pos = other.rel_pos;

    mappings = other.mappings;
    notches = other.notches;

    return *this;
}
