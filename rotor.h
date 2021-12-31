/*
File: rotor.h
Owner: Edward Jenks 
Username: ej3718
Last edited: 21/11/21
Last editor: Edward Jenks
Description: header file for the rotor for 'Intro to C++'
                coursework submission. The assignment is to program an
                enigma machine.
Comments: Commenting style taken from 'Problem Solving with C++ - Savitch'.
*/
#include "enum.h"
#include "config.h"
#include <fstream>

#ifndef ROTOR_H
#define ROTOR_H

/* a reference error code value is used throughout this calss and passed to all function. when functions are called this value should be checked. anything other than 0 represents and error code as found in errors.h */

/* class for rotor objects */
class Rotor
{
    /* config struct to hold the mappings of the rotor */
    Config mappings;

    /* config struct to hold the notches of the rotor */
    Config notches;

    /* variable to hold the relative position of the rotor (letter facing up) */
    Alphabet rel_pos = A;

    /* helper function to check the format for the rotor file is correct */
    void check_file_format(char const *filename, int &error_code) const;
    // precondition: filename passed to function.
    // postcondition: error code set and message printed if wrong format.

    /* helper function to check the ifstream is working properly */
    void check_ifstream(std::ifstream &in, char const *filename, int &error_code) const;
    // precondition: in stream and filename passed to function.
    // postcondition: error code set and message printed if ifstream.fail().

    /* helper function to load the rotor configuration file */
    void load_config_file(std::ifstream &in, char const *filename, int &error_code) const;
    // precondition: filename and empty ifstream passed to function.
    // postcondition: file opened to ifstream.

    /* helper function to take mapping information from ifstream and put it into 'mappings' */
    void set_mapping_config(std::ifstream &in, char const *filename, int &error_code);
    // precondition: active ifstream and filename passed to function.
    // postcondition: mapping config set to data from ifstream.

    /* helper function to take notch information from ifstream and put it into 'notches' */
    void set_notch_config(std::ifstream &in, char const *filename, int &error_code);
    // precondition: active ifstream and filename passed to function.
    // postcondition: notch config set to data from ifstream.

    /* helper function to check for non-numeric characters; an error is returned if one is found */
    void non_numeric_char(std::ifstream &in, char const *filename, int &error_code) const;
    // precondition: active ifstream and filename passed to function.
    // postcondition: error code and message returned if either of the next two //                  entries are non-numeric.

    /* helper function to check for repeated output mapping */
    void check_outputs(int const map_number, int index, char const *filename, int &error_code) const;
    // precondition: output mapping passed to function.
    // postcondition: error code set and message printed if mapping invalid.

    /* helper function to check for repeated input mapping */
    void check_inputs(int const map_number, int index, char const *filename, int &error_code) const;
    // precondition: input mapping passed to function.
    // postcondition: error code set and message printed if mapping invalid.

    /* helper function to check a letter index or array index is within bounds; and error is returned if not */
    void check_bounds(int const map_number, char const *filename, int &error_code) const;
    // precondition: letter index and filename passed to function.
    // postcondition: error code set and message printed if letter index out of //                  bounds.

public:
    /* default constructor */
    Rotor();

    /* setup constructor for the rotor */
    Rotor(char const *filename, int &error_code);
    // precondition: rotor config file name passed to function.
    // postcondition: rotor object created with desired config.

    /* copy constructor for the rotor */
    Rotor(Rotor const &other);
    // precondition: other rotor object passed to function.
    // postcondition: deep copy of object created and returned.

    /* default destructor */
    ~Rotor();

    /* function to load config file and fill the mapping and notch array while checking for errors */
    void set_config(char const *filename, int &error_code);
    // precondition: rotor config file name passed to function.
    // postcondition: rotor object set to desired config.

    /* function to print the set mappings of the rotor */
    void print_mappings() const;
    // precondition:
    // postcondition: set mappings printed to console.

    /* function to print the set notches of the plugboard */
    void print_notches() const;
    // precondition:
    // postcondition: set notches printed to console.

    /* function to set the relative position of the rotor */
    void set_rel_pos(Alphabet const starting_pos, int &error_code);
    // precondition: desired position passed to function
    // postcondition: set realative position of rotor.

    /* function to return the relative position of the rotor */
    Alphabet get_rel_pos(int &error_code) const;
    // precondition:
    // postcondition: return relative position of rotor.

    /* function to rotate the rotor, returns true if a notch is triggered */
    bool rotate(int &error_code);
    // precondition:
    // postcondition: rotate rotor, if notch triggered return true.

    /* function to return the forward (right to left) rotor mapping of a given letter */
    Alphabet return_mapping(Alphabet const letter, int &error_code) const;
    // precondition: letter is passed to function.
    // postcondition: right-to-left letter mapping read and returned.

    /* function to return the backward (left to right) rotor mapping of a given letter */
    Alphabet return_reverse_mapping(Alphabet const letter, int &error_code) const;
    // precondition: letter is passed to function.
    // postcondition: left-to-right letter mapping read and returned.

    /* function to return the notches of the rotor */
    Alphabet return_notch(int const index, int &error_code) const;
    // precondition: position passed to function.
    // postcondition: notch in that position in array returned.

    /* assignment opporator for a rotor */
    Rotor &operator=(Rotor const &other);
    // precondition: initialised object set equal to other.
    // postcondition: this set to deep copy of other and returned.
};

#endif