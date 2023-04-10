#include <hpr/csg/shape.hpp>
#include <hpr/csg/compound.hpp>


bool std::less<hpr::csg::Shape>::operator()(const hpr::csg::Shape& s1, const hpr::csg::Shape& s2) const
{
    return s1.tshape().HashCode(std::numeric_limits<Standard_Integer>::max()) <
           s2.tshape().HashCode(std::numeric_limits<Standard_Integer>::max());
}

namespace hpr::csg
{

std::map<Shape, Shape::Metadata> Shape::metadata;

}
