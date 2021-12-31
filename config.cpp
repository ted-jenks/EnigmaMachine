/*
File: structs.cpp
Owner: Edward Jenks 
Username: ej3718
Last edited: 19/11/21
Last editor: Edward Jenks
Description: Enigma file to conatin the config class. 
                The assignment is to program an
                enigma machine.
Comments: Commenting style taken from 'Problem Solving with C++ - Savitch'.
*/

#include <iostream>
#include <fstream>
#include <cstring>
#include <cassert>
#include "config.h"
#include "errors.h"

/* default constructor for a config struct */
Config::Config()
{
    /* initialise the array empty */
    for (int i = 0u; i < 26; i++)
        config[i] = Empty;
}

/* copy constructor for the config */
Config::Config(Config const &other) { *this = other; }

/* default destructor */
Config::~Config() {}

/* function to set a cell value */
void Config::set_value(int const index, Alphabet const letter, int &error_code)
{
    /* check bounds */
    if (letter < A || letter > Z || index < 0 || index > 25)
    {
        /* return error message */
        std::cerr << "Index out of bounds\n";
        error_code = INVALID_INDEX;
        return;
    }
    /* set value */
    config[index] = letter;
}

/* function to set a cell value */
Alphabet Config::get_value(int const index, int &error_code) const
{
    /* check bounds */
    if (index < 0 || index > 25)
    {
        /* return error message */
        std::cerr << "Index out of bounds\n";
        error_code = INVALID_INDEX;
        return Empty;
    }
    /* set value */
    return config[index];
}

/* function to return the index of a specified value */
int Config::get_index(Alphabet const letter, int &error_code) const
{
    /* check bounds in alphabet */
    if (letter < A || letter > Z)
    {
        /* return error message */
        std::cerr << "Index out of bounds\n";
        error_code = INVALID_INDEX;
        return Empty;
    }
    /* search for value */
    for (int index = 0u; index < 26; index++)
    {
        if (config[index] == letter)
            return index;
    }
    /* if not found */
    std::cerr << "Index out of bounds\n";
    error_code = INVALID_INDEX;
    return Empty;
}

/* function to print the config array */
void Config::print() const
{
    for (int index = 0u; index < 26; index++)
        std::cout << config[index] << " ";
    std::cout << std::endl;
}

/* function to return true if a value is foun */
bool Config::check_value(Alphabet letter, int &error_code) const
{
    /* check bounds in alphabet */
    if (letter < A || letter > Z)
    {
        std::cerr << "Index out of bounds\n";
        error_code = INVALID_INDEX;
        return false;
    }
    /* search for value */
    for (int i = 0u; i < 26; i++)
    {
        if (config[i] == letter)
            return true;
    }
    /* if not found */
    return false;
}

/* assigment opporator for a config */
Config &Config::operator=(Config const &other)
{
    /* guard self assignment */
    if (this == &other)
        return *this;

    /* equality opporator for a plugboard */
    for (int index = 0u; index < 26; index++)
    {
        /* copy mapping array */
        if (config[index] != other.config[index])
            config[index] = other.config[index];
    }
    return *this;
}