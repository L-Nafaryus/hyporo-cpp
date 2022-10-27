#include "../scalar/scalar.hpp"
#include "matrix_space.hpp"


namespace hpr
{

template <typename Type, size_t Row, size_t Col>
using mat = MatrixSpace<Type, Row, Col>;

using mat2 = MatrixSpace<scalar, 2, 2>;
using mat3 = MatrixSpace<scalar, 3, 3>;
using mat4 = MatrixSpace<scalar, 4, 4>;

}
