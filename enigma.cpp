/*
File: enigma.cpp
Owner: Edward Jenks 
Username: ej3718
Last edited: 21/11/21
Last editor: Edward Jenks
Description: Enigma file to conatin the enigma class. 
                The assignment is to program an
                enigma machine.
Comments: Commenting style taken from 'Problem Solving with C++ - Savitch'.
*/
#include <iostream>
#include <fstream>
#include <cstring>
#include <cassert>
#include <vector>
#include "enigma.h"
#include "errors.h"

/* ------------------------------------------------- */
/* ---------------------ENIGMA---------------------- */
/* ------------------------------------------------- */

/* default constructor for the enigma machine */
Enigma::Enigma()
{
    rotors.reserve(50); //reserve space for 50 rotors for speed
}

/* setup constructor for the rotor */
Enigma::Enigma(int const argc, char *argv[], int &error_code) { configure_machine(argc, argv, error_code); }

/* copy constructor for the rotor */
Enigma::Enigma(Enigma const &other) { *this = other; }

/* default destructor */
Enigma::~Enigma() {}

/* helper function to check the format for the rotor file is correct */
void Enigma::check_file_format(char const *filename, int &error_code) const
{ /* check that a nullptr hasn't been passed to function */
    if (filename != nullptr)
    {
        int str_length = strlen(filename);
        /* check file is correct format for reflector */
        if (filename[str_length - 3] != 'p' || filename[str_length - 2] != 'o' || filename[str_length - 1] != 's')
        {
            /* return error message */
            std::cerr << filename << " has an invlaid enigma file format (files should be .pos)." << std::endl;
            error_code = ERROR_OPENING_CONFIGURATION_FILE;
            return;
        }
    }
}

/* helper function to check the ifstream is working properly */
void Enigma::check_ifstream(std::ifstream &in, char const *filename, int &error_code) const
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

/* helper function to load the starting position configuration file */
void Enigma::load_starting_positions(std::ifstream &in, char const *filename, int &error_code) const
{
    /* check that a nullptr hasn't been passed to function */
    if (filename != nullptr)
    {
        /* check file is correct format for position */
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
        std::cerr << "Error opening " << filename << std::endl;
        error_code = ERROR_OPENING_CONFIGURATION_FILE;
        return;
    }
}

/* helper function to check for non-numeric characters; an error is returned if one is found. while this could be made a non-member function and used for all objects, this code is repeated to comply with labts testing. */
void Enigma::non_numeric_char(std::ifstream &in, char const *filename, int &error_code) const
{
    int current_pos;
    char characters[100];
    current_pos = in.tellg(); // store the current position in the file
    in >> characters;         // read the file until the next whitespace
    /* check read characters */
    for (size_t index = 0u; index < strlen(characters); index++)
    {
        /* if not a number */
        if (characters[index] < '0' || characters[index] > '9')
        {
            /* return error message */
            std::cerr << "Non-numeric character in rotor positions file " << filename << std::endl;
            error_code = NON_NUMERIC_CHARACTER;
            return;
        }
    }
    in.seekg(current_pos); // return to previous spot in file
}

/* helper function to check a letter index or array index is within bounds; and error is returned if not */
void Enigma::check_bounds(int letter_index, char const *filename, int &error_code) const
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

/* helper function to read the starting position configuration file and put values in an array */
void Enigma::read_starting_positions(std::ifstream &in, char const *filename, int starting_positions[], int &error_code) const
{
    int index = 0u;
    in >> std::ws; // remove leading whitespace in file
    /* if there is no rotors, leave */
    if (num_rotors == 0)
        return;
    /* while there is still text in the file */
    for (; !in.eof(); index++)
    {
        /* if we have more starting positions than rotors */
        if (index == num_rotors)
        {
            /* return error message */
            std::cerr << "Too many rotor starting positions in " << filename << std::endl;
            error_code = NO_ROTOR_STARTING_POSITION;
            return;
        }
        /* check for non-numeric characters */
        non_numeric_char(in, filename, error_code);
        /* if an error has been returned */
        if (error_code != 0)
            return;
        /* read number from file and remove whitespace */
        in >> starting_positions[index] >> std::ws;
        /* check the bounds of the starting position read */
        check_bounds(starting_positions[index], filename, error_code);
        /* if an error has been returned */
        if (error_code != 0)
            return;
    }
    /* check for corruption */
    check_ifstream(in, filename, error_code);
    if (error_code != 0)
        return;
    /* if we have too few starting positions */
    if (index < num_rotors)
    {
        /* return error message */
        std::cerr << "No starting position for rotor " << index << " in rotor position file: " << filename << std::endl;
        error_code = NO_ROTOR_STARTING_POSITION;
        return;
    }
    /* close the file */
    in.close();
    /* check for failure on close */
    check_ifstream(in, filename, error_code);
    if (error_code != 0)
        return;
}

/* helper function to create the rotors and put them in the rotor array */
void Enigma::make_rotors(char *argv[], int const argc, int &error_code)
{
    int starting_positions[num_rotors];
    std::ifstream in;
    for (int index = 0u; index < num_rotors; index++)
    {
        auto temp_rotor = Rotor();                          // initialise an emptry rotor
        temp_rotor.set_config(argv[index + 3], error_code); // configure the rotor
        /* if error has been returned */
        if (error_code != 0)
            return;
        rotors.push_back(temp_rotor); // store rotor in vector
    }
    /* load the starting positions file */
    load_starting_positions(in, argv[argc - 1], error_code);
    if (error_code != 0)
        return;
    /* read the starting positions file to array */
    read_starting_positions(in, argv[argc - 1], starting_positions, error_code);
    if (error_code != 0)
        return;
    /* loop to set starting positions of the rotors */
    for (int index = 0u; index < num_rotors; index++) // this loop is seperate from the one above to comply with LabTS tests
    {
        /* set the starting positions of the rotors */
        rotors[index].set_rel_pos(Alphabet(starting_positions[index]), error_code);
        if (error_code != 0)
            return;
    }
}

/* set the whole configuration of the machine */
void Enigma::configure_machine(int const argc, char *argv[], int &error_code)
{
    /* check enough arguements have been passed to the program */
    if (argc < 4)
    {
        /* return error message */
        std::cerr << "usage: enigma plugboard-file reflector-file (<rotor-file>)* rotor-positions" << std::endl;
        error_code = INSUFFICIENT_NUMBER_OF_PARAMETERS;
        return;
    }

    num_rotors = argc - 4; // calculate the number of rotors in the machine
    /* set the plugboard configuration */
    plugboard.set_config(argv[1], error_code);
    /* if error has been raised */
    if (error_code != 0)
        return;

    /* set the reflector configuration */
    reflector.set_config(argv[2], error_code);
    /* if error has been raised */
    if (error_code != 0)
        return;
    /* set up the rotors */
    make_rotors(argv, argc, error_code);
    /* if error has been raised */
    if (error_code != 0)
        return;
}

/* helper function to encrypt a character */
void Enigma::encrypt(Alphabet &working_character, int &error_code)
{
    /* check the input character is valid */
    if (working_character < A || working_character > Z)
    {
        /* return error message */
        std::cerr << static_cast<char>(working_character + 'A') << " is not a valid input character (input characters must be upper case letters A-Z)!" << std::endl;
        error_code = INVALID_INPUT_CHARACTER;
        return;
    }
    /* if there are any rotors */
    if (num_rotors > 0)
    {
        /* cascade rotation as neccesary */
        for (int index = num_rotors - 1; rotors[index].rotate(error_code); index--)
        {
            if (error_code != 0)
                return;
            if (index == num_rotors)
                break;
        }
    }
    /* get plugboard mapping */
    working_character = plugboard.return_mapping(working_character, error_code);
    /* if error has been raised */
    if (error_code != 0)
        return;
    /* move through rotors right to left */
    for (int index = (num_rotors - 1); index >= 0; index--)
    {
        working_character = rotors[index].return_mapping(working_character, error_code);
        /* if error has been raised */
        if (error_code != 0)
            return;
    }
    /* get reflector mapping */
    working_character = reflector.return_mapping(working_character, error_code);
    /* if error has been raised */
    if (error_code != 0)
        return;
    /* move through rotors left to right */
    for (int index = 0u; index < num_rotors; index++)
    {
        working_character = rotors[index].return_reverse_mapping(working_character, error_code);
        /* if error has been raised */
        if (error_code != 0)
            return;
    }
    /* get final plugboard mapping */
    working_character = plugboard.return_mapping(working_character, error_code);
    /* if error has been raised */
    if (error_code != 0)
        return;
    /* check that the final solution is within alphabet bounds */
    check_bounds(static_cast<int>(working_character), nullptr, error_code);
}

/* function to run the enigma machine and start taking inputs */
void Enigma::run_machine(int &error_code)
{
    char character_in;
    Alphabet working_character;
    std::cin >> std::ws;        // remove leading whitespace
    std::cin.get(character_in); // read character
    while (std::cin)
    {
        working_character = Alphabet(static_cast<int>(character_in) - 'A'); // change character to alphabet enum type
        encrypt(working_character, error_code);                             // encrypt the algorithm
        /* if error has been raised */
        if (error_code != 0)
            return;
        std::cout << static_cast<char>(working_character + 'A'); // cast back to character
        std::cin >> std::ws;                                     // clear whitespace
        std::cin.get(character_in);                              // read next character
    }
}

/* assignment opporator for an enigma */
Enigma &Enigma::operator=(Enigma const &other)
{
    /* guard self assignment */
    if (this == &other)
        return *this;
    plugboard = other.plugboard;
    rotors = other.rotors;
    reflector = other.reflector;
    return *this;
}
