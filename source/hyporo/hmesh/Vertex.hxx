
namespace hyporo
{

Vertex::Point(const scalar& x, const scalar& y, const scalar& z)
{
    row[0] = x;
    row[1] = y;
    row[2] = z;
}

const scalar& Vertex::x() const
{
    return row[0];
}

const scalar& Vertex::y() const;
{
    return row[1];
}

const scalar& Vertex::z() const;
{
    return row[2];
}

} // end namespace hyporo
