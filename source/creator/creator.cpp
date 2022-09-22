#include "scalar.hpp"
#include "vector.hpp"
#include <algorithm>
#include <iostream>
#include "matrix.hpp"

int main(void)
{
    hyporo::vec3 v1 {1, 3, 2};
    hyporo::vec3 v2 {7, 4, 5};
    std::cout << -v1 << std::endl;
    std::cout << ((v1 + v2) == hyporo::vec3(8, 7, 7)) << std::endl;
    std::cout << v1 - v2 << std::endl;
    std::cout << v1 * 2 << std::endl;
    std::cout << v1 / 2 << std::endl;
    std::cout << (v1 == v2) << std::endl;
    std::cout << (v1 == v1) << std::endl;
    hyporo::mat3 m1 {1, 3, 2, 5, 3, 6, 7, 2, 0};
    hyporo::mat3 m2 {7, 4, 5, 5, 7, 3, 1, 4, 6};
    std::cout << m1 + m2 << std::endl;
    return 0;
}
