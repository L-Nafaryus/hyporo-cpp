#include <iostream>
#include "../vector/vector_space.hpp"

template <typename Type, size_t Size>
using vs = hpr::VectorSpace<Type, Size>;

using std::cout, std::endl;

template <typename Type, size_t Size>
void print(const vs<Type, Size>& vec)
{
    cout << "Vector: ";
    for (auto n = vec.begin(); n != vec.end(); ++n)
        cout << *n << " ";
    cout << endl;
}
int main(void)
{

    vs<float, 3> v1 {3, 4, 7, 8};
    v1 += 3;
    print<float, 3>(v1);


    return 0;
}
