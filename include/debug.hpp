#ifndef DEBUG_HPP_INCLUDED
#define DEBUG_HPP_INCLUDED
#include <iostream>
#include <string>
#include "include/vectormath.hpp"

void write(mat3 t, std::string name);
void write(vec3 t, std::string name);
template<class T>
void dbg(T t)
{
    std::cout << "Debug: " << t << std::endl;
}

#endif // DEBUG_HPP_INCLUDED
