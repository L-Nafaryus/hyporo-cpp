#pragma once

#include <hpr/containers/array.hpp>
#include <hpr/math/scalar.hpp>
#include <hpr/math/vector.hpp>

#include <map>
#include <string>

#include <Geom_Surface.hxx>
#include <GeomLProp_SLProps.hxx>
#include <Poly_Triangle.hxx>

#include <TopoDS.hxx>
#include <TopoDS_Vertex.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Wire.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Compound.hxx>
#include <TopoDS_CompSolid.hxx>
#include <TopoDS_Shell.hxx>
#include <TopoDS_Solid.hxx>
#include <TopExp_Explorer.hxx>

#include <BRepBuilderAPI_MakeVertex.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepBuilderAPI_MakeShell.hxx>
#include <BRepBuilderAPI_MakeSolid.hxx>
#include <BRepBuilderAPI_Sewing.hxx>
#include <BRep_Builder.hxx>

#include <BRepBndLib.hxx>

#include <BRep_Tool.hxx>
#include <BRepTools.hxx>
#include <BRepGProp.hxx>
#include <GProp_GProps.hxx>
#include <ShapeUpgrade_UnifySameDomain.hxx>
#include <BRepExtrema_DistShapeShape.hxx>

#include <BRepBuilderAPI_Transform.hxx>
#include <BRepBuilderAPI_GTransform.hxx>
#include <BRepFilletAPI_MakeFillet.hxx>

#include <BRepAlgoAPI_Fuse.hxx>
#include <BRepAlgoAPI_Common.hxx>
#include <BRepAlgoAPI_Cut.hxx>

#include <BRepPrimAPI_MakeSphere.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepPrimAPI_MakePrism.hxx>

#include <STEPControl_Writer.hxx>
#include <Interface_Static.hxx>

#include <BRepMesh_IncrementalMesh.hxx>
#include <Poly_PolygonOnTriangulation.hxx>
#include <Poly_Triangulation.hxx>

#include <TopAbs_ShapeEnum.hxx>
#include <TopLoc_Location.hxx>

#include <NCollection_List.hxx>

#include <ShapeFix_Solid.hxx>
#include <ShapeFix_Shell.hxx>
#include <ShapeFix_Face.hxx>



namespace hpr::csg
{
    class Shape;
}

namespace std
{
    template<>
    struct less<hpr::csg::Shape>
    {
        bool operator() (const hpr::csg::Shape& s1, const hpr::csg::Shape& s2) const;
    };
}

namespace hpr::csg
{

// Class declaration

class Shape
{

public:

    enum class Type
    {
        Compound,
        Compsolid,
        Solid,
        Shell,
        Face,
        Wire,
        Edge,
        Vertex,
        Shape,
        Unknown
    };

    enum class Format
    {
        Unknown,
        STEP
    };

    class Metadata
    {
    public:
        std::string label;
    public:
        Metadata() :
            label {"default"}
        {}
        void merge(const Metadata& data)
        {
            if (label == "default" && data.label != "default")
                label = data.label;
        }
    };

public:

    // TODO: clean up map
    static
    std::map<Shape, Shape::Metadata> metadata;

protected:

    TopoDS_Shape p_shape;

public:

    [[nodiscard]]
    TopoDS_Shape tshape() const { return p_shape; }

    Shape(const TopoDS_Shape& s) :
        p_shape {s}
    {}

    Shape(TopoDS_Shape&& s) noexcept:
        p_shape {std::forward<TopoDS_Shape>(s)}
    {}

public:

    Shape() :
        p_shape {}
    {}

    virtual
    ~Shape() = default;
    //{
        //if (metadata.contains(*this))
        //    metadata.erase(*this);
    //}

    [[nodiscard]]
    Type type() const
    {
        switch (p_shape.ShapeType())
        {
            case TopAbs_VERTEX:
                return Type::Vertex;
            case TopAbs_EDGE:
                return Type::Edge;
            case TopAbs_FACE:
                return Type::Face;
            case TopAbs_WIRE:
                return Type::Wire;
            case TopAbs_SHELL:
                return Type::Shell;
            case TopAbs_SOLID:
                return Type::Solid;
            case TopAbs_COMPOUND:
                return Type::Compound;
            case TopAbs_COMPSOLID:
                return Type::Compsolid;
            case TopAbs_SHAPE:
                return Type::Shape;
            default:
                return Type::Unknown;
        }
    }

    [[nodiscard]]
    vec3 center() const
    {
        GProp_GProps props;

        switch (type())
        {
            case Type::Solid:
            case Type::Compsolid:
            case Type::Compound:
                BRepGProp::VolumeProperties(p_shape, props);
            case Type::Shell:
            case Type::Face:
                BRepGProp::SurfaceProperties(p_shape, props);
            default:
                BRepGProp::LinearProperties(p_shape, props);
        }

        gp_Pnt center {props.CentreOfMass()};

        return vec3 {static_cast<scalar>(center.X()), static_cast<scalar>(center.Y()), static_cast<scalar>(center.Z())};
    }

    [[nodiscard]]
    double length() const
    {
        GProp_GProps props;

        switch (type())
        {
            case Type::Vertex:
                return 0;
            default:
                BRepGProp::LinearProperties(p_shape, props);
                return props.Mass();
        }
    }

    [[nodiscard]]
    double area() const
    {
        GProp_GProps props;

        switch (type())
        {
            case Type::Vertex:
            case Type::Edge:
            case Type::Wire:
                return 0;
            default:
                BRepGProp::SurfaceProperties(p_shape, props);
                return props.Mass();
        }
    }

    [[nodiscard]]
    double volume() const
    {
        GProp_GProps props;

        switch (type())
        {
            case Type::Compsolid:
            case Type::Solid:
                BRepGProp::VolumeProperties(p_shape, props);
                return props.Mass();

            default:
                return 0;
        }
    }

    void label(const std::string& label) const
    {
        metadata[*this].label = label;
    }

    [[nodiscard]]
    std::string label() const
    {
        return metadata[*this].label;
    }

    void dump(const std::string& filename, Format format) const
    {
        if (p_shape.IsNull())
            throw std::runtime_error("Trying to export null shape");

        switch (format)
        {
            case Format::STEP:
            {
                STEPControl_Writer writer;
                Interface_Static::SetCVal("xstep.cascade.unit", "MM");
                Interface_Static::SetCVal("write.step.unit", "MM");
                Interface_Static::SetIVal("write.step.nonmanifold", 1);

                writer.Transfer(p_shape, STEPControl_AsIs);
                writer.Write(filename.c_str());
                break;
            }
            case Format::Unknown:
            default:
                throw std::invalid_argument("Unknown export format");
        }
    }

    //

    [[nodiscard]]
    sarray<vec3, 2> boundingBox() const
    {
        Bnd_Box bbox;
        BRepBndLib::Add(p_shape, bbox, true);
        gp_Pnt p1 {bbox.CornerMin()};
        gp_Pnt p2 {bbox.CornerMax()};

        return sarray<vec3, 2> {
            {static_cast<scalar>(p1.X()), static_cast<scalar>(p1.Y()), static_cast<scalar>(p1.Z())},
            {static_cast<scalar>(p2.X()), static_cast<scalar>(p2.Y()), static_cast<scalar>(p2.Z())}
        };
    }

    void incrementalMesh(double deflection)
    {
        BRepTools::Clean(p_shape);
        BRepMesh_IncrementalMesh(p_shape, deflection, true);
    }

    darray<Shape> subShapes(Type type) const
    {
        darray<Shape> shapes;
        for (TopExp_Explorer exp(p_shape, static_cast<TopAbs_ShapeEnum>(type)); exp.More(); exp.Next())
            shapes.push(Shape(exp.Current()));
        return shapes;
    }

    darray<Shape> edges() const
    {
        return subShapes(Type::Edge);
    }

    darray<Shape> faces() const
    {
        return subShapes(Type::Face);
    }

    darray<Shape> shells() const
    {
        return subShapes(Type::Shell);
    }





};

double distance(const Shape& lhs, const Shape& rhs)
{
    return BRepExtrema_DistShapeShape(lhs.tshape(), rhs.tshape()).Value();
}

}






