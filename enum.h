/*
File: enum.h
Owner: Edward Jenks 
Username: ej3718
Last edited: 21/11/21
Last editor: Edward Jenks
Description: header file for the enumerator for 'Intro to C++'
                coursework submission. The assignment is to program an
                enigma machine.
Comments: Commenting style taken from 'Problem Solving with C++ - Savitch'.
*/

#ifndef ENUM_H
#define ENUM_H

/* alphabet enumerator to represent letters, mark empty array cells and make code more readable. while chars can be used, this reduces the need for conversions to printing results only. */
enum Alphabet
{
    A,
    B,
    C,
    D,
    E,
    F,
    G,
    H,
    I,
    J,
    K,
    L,
    M,
    N,
    O,
    P,
    Q,
    R,
    S,
    T,
    U,
    V,
    W,
    X,
    Y,
    Z,
    Empty
};

/* ++Alphabet oporator overload; Z wraps to A */
Alphabet &operator++(Alphabet &value);
// precondition: alphabet value passed to function.
// postcondition: value returned then increased by one.

/* Alphabet++ oporator overload; Z wraps to A */
Alphabet operator++(Alphabet &value, int);
// precondition: alphabet value passed to function.
// postcondition: value increased by one then returned.

/* --Alphabet oporator overload; A wraps to Z */
Alphabet &operator--(Alphabet &value);
// precondition: alphabet value passed to function.
// postcondition: value returned then decreased by one.

/* Alphabet-- oporator overload; A wraps to Z */
Alphabet operator--(Alphabet &value, int);
// precondition: alphabet value passed to function.
// postcondition: value increased by one then returned.

#endif // !ENUM_H