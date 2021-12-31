/*
File: main.cpp
Owner: Edward Jenks 
Username: ej3718
Last edited: 21/11/21
Last editor: Edward Jenks
Description: Main file to contain all work for 'Intro to C++'
                coursework submission. The assignment is to program an
                enigma machine.
Comments: Commenting style taken from 'Problem Solving with C++ - Savitch'.
*/

#include <iostream>
#include "errors.h"
#include "enigma.h"

int main(int argc, char **argv)
{
    /* variable to hold the error_code */
    int error_code = 0;
    /* initialise the enigma */
    Enigma enigma;
    /* set up the enigma machine */
    enigma.configure_machine(argc, argv, error_code);
    /* check an error has not been returned */
    if (error_code != 0)
        return error_code;
    /* run the enigma machine */
    enigma.run_machine(error_code);
    /* check an error has not been returned */
    if (error_code != 0)
        return error_code;
    /* return successfuly */
    return 0;
}