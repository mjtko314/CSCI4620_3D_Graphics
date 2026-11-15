#include <vector>

struct Matrix{
    public:
    float p1, p2, p3, p4,
          p5, p6, p7, p8,
          p9, p10, p11, p12,
          p13, p14, p15, p16;

    std::vector<float> multiply(const std::vector<float> &rhs);
};