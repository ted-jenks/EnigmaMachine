/*
File: plugboard.h
Owner: Edward Jenks 
Username: ej3718
Last edited: 21/11/21
Last editor: Edward Jenks
Description: header file for the plugboard for 'Intro to C++'
                coursework submission. The assignment is to program an
                enigma machine.
Comments: Commenting style taken from 'Problem Solving with C++ - Savitch'.
*/
#include "enum.h"
#include "config.h"
#include <fstream>

#ifndef PLUGBOARD_H
#define PLUGBOARD_H

/* a reference error code value is used throughout this calss and passed to all function. when functions are called this value should be checked. anything other than 0 represents and error code as found in errors.h */

/* class for plugboard objects. while this would be a good place to use inheritence, that is beyond the scope of this coursework so code has been repeated. */
class Plugboard
{
    /* config struct to hold the mappings of the plugboard */
    Config mappings;

    /* helper function to check the format for the plugboard file is correct */
    void check_file_format(char const *filename, int &error_code) const;
    // precondition: filename passed to function.
    // postcondition: error code set and message printed if wrong format.

    /* helper function to check the ifstream is working properly */
    void check_ifstream(std::ifstream &in, char const *filename, int &error_code) const;
    // precondition: in stream and filename passed to function.
    // postcondition: error code set and message printed if ifstream.fail().

    /* helper function to load the plugboard configuration file */
    void load_config_file(std::ifstream &in, char const *filename, int &error_code) const;
    // precondition: filename and empty ifstream passed to function.
    // postcondition: file opened to ifstream.

    /* helper function to take mapping information from ifstream and put it into 'mappings' */
    void set_mapping_config(std::ifstream &in, char const *filename, int &error_code);
    // precondition: active ifstream and filename passed to function.
    // postcondition: mapping config set to data from ifstream.

    /* helper function to check for non-numeric characters; an error is returned if one is found */
    void non_numeric_char(std::ifstream &in, char const *filename, int &error_code) const;
    // precondition: active ifstream and filename passed to function.
    // postcondition: error code and message returned if either of the next two //                  entries are non-numeric.

    /* helper function to check a letter index or array index is within bounds; and error is returned if not */
    void check_bounds(int const letter_index, char const *filename, int &error_code) const;
    // precondition: letter index and filename passed to function.
    // postcondition: error code set and message printed if letter index out of //                  bounds.

    /* helper function to ensure that a mapping pair is valid in the curretn mapping array */
    void check_valid(int const first_mapping_value, int const second_mapping_value, char const *filename, int &error_code) const;
    // precondition: mapping index pair passed to function.
    // postcondition: error code set and message printed if mapping invalid.

public:
    /* default constructor */
    Plugboard();

    /* setup constructor for the plugboard */
    Plugboard(char const *filename, int &error_code);
    // precondition: plugboard config file name passed to function.
    // postcondition: plugboard object created with desired config.

    /* copy constructor for the plugboard */
    Plugboard(Plugboard const &other);
    // precondition: other plugboard object passed to function.
    // postcondition: deep copy of object created and returned.

    /* default destructor */
    ~Plugboard();

    /* function to load config file and fill the mapping array while checking for errors */
    void set_config(char const *filename, int &error_code);
    // precondition: plugboard config file name passed to function.
    // postcondition: plugboard object set to desired config.

    /* function to print the set mappings of the plugboard */
    void print_mappings() const;
    // precondition:
    // postcondition: set mappings printed to console.

    /* function to return the plugboard mapping for a given letter */
    Alphabet return_mapping(Alphabet const letter, int &error_code) const;
    // precondition: letter is passed to function.
    // postcondition: letter mapping read and returned.

    /* assignment opporator for a plugboard */
    Plugboard &operator=(Plugboard const &other);
    // precondition: initialised object set equal to other.
    // postcondition: this set to deep copy of other and returned.
};

#endif