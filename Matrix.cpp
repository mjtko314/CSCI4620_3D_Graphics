#include "Matrix.hpp"
#include <vector>

Matrix Matrix::multiply(const std::vector<float> &rhs)
{
    std::vector<float> reuslt;
    float row1 = (p1 * rhs[0]) +(p2 * rhs[1]) +(p3 * rhs[2]) +(p4 * rhs[3]); 
    float row2 = (p5 * rhs[0]) +(p6 * rhs[1]) +(p7 * rhs[2]) +(p8 * rhs[3]); 
    float row3 = (p9 * rhs[0]) +(p10 * rhs[1]) +(p11 * rhs[2]) +(p12 * rhs[3]);
    float row4 = (p13 * rhs[0]) +(p14 * rhs[1]) +(p15 * rhs[2]) +(p16 * rhs[3]); 
}
