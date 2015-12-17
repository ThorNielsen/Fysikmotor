#include "debug.hpp"

void write(mat3 t, std::string name)
{
    std::cout << "3×3 Matrix: " << name << '\n';
    std::cout << t[0][0] << '\t' << t[1][0] << '\t' << t[2][0] << '\n';
    std::cout << t[0][1] << '\t' << t[1][1] << '\t' << t[2][1] << '\n';
    std::cout << t[0][2] << '\t' << t[1][2] << '\t' << t[2][2] << '\n';
    std::cout << "End " << name << std::endl;
}

void write(vec3 t, std::string name)
{
    std::cout << name << ": {" << t[0] << ", " << t[1] << ", " << t[2] << "}" << std::endl;
}
