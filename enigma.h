/*
File: enigma.h
Owner: Edward Jenks
Username: ej3718
Last edited: 21/11/21
Last editor: Edward Jenks
Description: header file for the enigma for 'Intro to C++'
                coursework submission. The assignment is to program an
                enigma machine.
Comments: Commenting style taken from 'Problem Solving with C++ - Savitch'.
*/

#include "enum.h"
#include "config.h"
#include "plugboard.h"
#include "rotor.h"
#include "reflector.h"
#include <fstream>
#include <vector>

#ifndef ENIGMA_H
#define ENIGMA_H

/* a reference error code value is used throughout this calss and passed to all function. when functions are called this value should be checked. anything other than 0 represents and error code as found in errors.h */

/* class for the whole enigma machine */
class Enigma
{
    /* vector to hold rotors so we can have a flexible number in the machine */
    std::vector<Rotor> rotors;

    /* the plugboard for the machine */
    Plugboard plugboard;

    /* the reflector for the machine */
    Reflector reflector;

    /* variable to hold the number of rotors in the machine */
    int num_rotors = 0;

    /* helper function to check the format for the enigma file is correct */
    void check_file_format(char const *filename, int &error_code) const;
    // precondition: filename passed to function.
    // postcondition: error code set and message printed if wrong format.

    /* helper function to check the ifstream is working properly */
    void check_ifstream(std::ifstream &in, char const *filename, int &error_code) const;
    // precondition: in stream and filename passed to function.
    // postcondition: error code set and message printed if ifstream.fail().

    /* helper function to load the starting position configuration file */
    void load_starting_positions(std::ifstream &in, char const *filename, int &error_code) const;
    // precondition: filename and empty ifstream passed to function.
    // postcondition: file opened to ifstream.

    /* helper function to create the rotors and put them in the rotor array */
    void make_rotors(char *argv[], int const argc, int &error_code);
    // precondition: config args passed to function.
    // postcondition: rotors created and configured.

    /* helper function to read the starting position configuration file and put values in an array */
    void read_starting_positions(std::ifstream &in, char const *filename, int starting_positions[], int &error_code) const;
    // precondition: pass filename, instream and array to hold positions to //                  function.
    // postcondition: rotor positions read to array.

    /* helper function to encrypt a character */
    void encrypt(Alphabet &working_character, int &error_code);
    // precondition: letter passed to function.
    // postcondition: letter changed to encrypted character based on config.

    /* helper function to check for non-numeric characters; an error is returned if one is found */
    void non_numeric_char(std::ifstream &in, char const *filename, int &error_code) const;
    // precondition: active ifstream and filename passed to function.
    // postcondition: error code and message returned if either of the next two //                  entries are non-numeric.

    /* helper function to check a letter index or array index is within bounds; and error is returned if not */
    void check_bounds(int const letter_index, char const *filename, int &error_code) const;
    // precondition: letter index and filename passed to function.
    // postcondition: error code set and message printed if letter index out of //                  bounds.

public:
    /* default constructor; saves space for rotor vector */
    Enigma();

    /* setup constructor for the enigma */
    Enigma(int const argc, char *argv[], int &error_code);
    // precondition: enigma config file name passed to function.
    // postcondition: enigma object created with desired config.

    /* copy constructor for the enigma */
    Enigma(Enigma const &other);
    // precondition: other enigma object passed to function.
    // postcondition: deep copy of object created and returned.

    /* default destructor */
    ~Enigma();

    /* set the whole configuration of the machine */
    void configure_machine(int const argc, char *argv[], int &error_code);
    // precondition: config arguments passed to function.
    // postcondition: enigma machine is fully configured.

    /* function to run the enigma machine and start taking inputs */
    void run_machine(int &error_code);
    // precondition:
    // postcondition: machine runs and takes inputs.

    /* assignement opporator for an enigma */
    Enigma &operator=(Enigma const &other);
    // precondition: initialised object set equal to other.
    // postcondition: this set to deep copy of other and returned.
};

#endif // !ENIGMA_H