#include "VectorSpace.hpp"


namespace hyporo
{

template <class C, sizet Mrows, sizet Ncols>
class MatrixSpace : public VectorSpace<C, Mrows * Ncols>
{

public:

    // Member constants

    static const sizet mRows = Mrows;
    static const sizet nColumns = Ncols;

    // Constructors

    inline MatrixSpace();

    template <class... Args>
    inline MatrixSpace(const Args... components);

    // Member operators

    inline const C& operator()(const sizet& n, const sizet& m);

};

} // end namespace hyporo

#include "MatrixSpace.hxx"
