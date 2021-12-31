/*
File: config.h
Owner: Edward Jenks 
Username: ej3718
Last edited: 21/11/21
Last editor: Edward Jenks
Description: header file for the config structs for 'Intro to C++'
                coursework submission. The assignment is to program an
                enigma machine.
Comments: Commenting style taken from 'Problem Solving with C++ - Savitch'.
*/
#include "enum.h"

#ifndef CONFIG_H
#define CONFIG_H

/* a reference error code value is used throughout this calss and passed to all function. when functions are called this value should be checked. anything other than 0 represents and error code as found in errors.h */

/* this struct is built to hold mappings and notch configurations */
struct Config
{
    /* default constructor */
    Config();

    /* copy constructor for the config */
    Config(Config const &other);
    // precondition: other config object passed to function.
    // postcondition: deep copy of object created and returned.

    /* default destructor */
    ~Config();

    /* function to set the value of config array at desired index */
    void set_value(int const index, Alphabet const letter, int &error_code);
    // precondition: desired letter and position is passed to function.
    // postcondition: letter at index changed to desired value.

    /* function to get the value of config array at desired index */
    Alphabet get_value(int const index, int &error_code) const;
    // precondition: position is passed to function.
    // postcondition: letter at index read and returned.

    /* function to get the index of config array for a desired value */
    int get_index(Alphabet const letter, int &error_code) const;
    // precondition: letter is passed to function.
    // postcondition: index of letter read and returned.

    /* function to print the config array */
    void print() const;
    // precondtion:
    // postcondition: config array printed to console.

    /* function to check if a value appears in config array */
    bool check_value(Alphabet const letter, int &error_code) const;
    // precondition: letter passed to function.
    // postcondition: true returned if value is present, false if not.

    /* assigment operator for a config */
    Config &operator=(Config const &other);
    // precondition: initialised object set equal to other.
    // postcondition: this set to deep copy of other and returned.

private:
    /* configuration array initialised at known max length (26) */
    Alphabet config[26];
};

#endif // !CONFIG_H