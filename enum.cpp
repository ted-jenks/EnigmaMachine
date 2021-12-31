/*
File: enum.cpp
Owner: Edward Jenks 
Username: ej3718
Last edited: 21/11/21
Last editor: Edward Jenks
Description: Enigma file to conatin the enumerator opporators. 
                The assignment is to program an
                enigma machine.
Comments: Commenting style taken from 'Problem Solving with C++ - Savitch'.
*/
#include "enum.h"
#include "errors.h"

/* ------------------------------------------------- */
/* --------------------ALPHABET--------------------- */
/* ------------------------------------------------- */

/* ++Alphabet */
Alphabet &operator++(Alphabet &value)
{
    /* add one to value */
    value = static_cast<Alphabet>(value + 1);
    /* wrap around if > Z */
    if (value == Empty)
        value = A;
    return value;
}

/* Alphabet++ */
Alphabet operator++(Alphabet &value, int)
{
    Alphabet return_value = value;
    ++value; // use already defined function
    return return_value;
}

/* --Alphabet */
Alphabet &operator--(Alphabet &value)
{
    /* minus one from value */
    value = static_cast<Alphabet>(value - 1);
    /* wrap around if < A */
    if (value < A)
        value = Z;
    return value;
}

/* Alphabet-- */
Alphabet operator--(Alphabet &value, int)
{
    Alphabet return_value = value;
    --value; // use already defined function
    return return_value;
}