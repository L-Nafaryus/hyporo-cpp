
namespace hyporo
{

// Constructors

template <class C, sizet Mrows, sizet Ncols>
inline MatrixSpace<C, Mrows, Ncols>::MatrixSpace()
{}

template <class C, sizet Mrows, sizet Ncols>
template <class... Args>
inline MatrixSpace<C, Mrows, Ncols>::MatrixSpace(const Args... components)
    //: row { static_cast<C>(components)... }
{
    this->row = { static_cast<C>(components)... };
}

// Member operators

template <class C, sizet Mrows, sizet Ncols>
inline const C& MatrixSpace<C, Mrows, Ncols>::operator()(const sizet& n, const sizet& m)
{
    return this->row[n * Ncols + m];
}

} // end namespace hyporo
