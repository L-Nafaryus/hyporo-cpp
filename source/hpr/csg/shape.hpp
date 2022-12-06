#pragma once

#include "../containers/array.hpp"
#include "../math/scalar.hpp"
#include "../math/vector.hpp"

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
#include <BRep_Builder.hxx>

#include <BRepBndLib.hxx>

#include <BRep_Tool.hxx>
#include <BRepTools.hxx>
#include <BRepGProp.hxx>
#include <GProp_GProps.hxx>
#include <ShapeUpgrade_UnifySameDomain.hxx>
#include <BRepExtrema_DistShapeShape.hxx>

#include <BRepBuilderAPI_Transform.hxx>
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

// Forward declaration of friend functions

double distance(const Shape& lhs, const Shape& rhs);

Shape fuse(const Shape& lhs, const Shape& rhs);

Shape fuse(const darray<Shape>& args, const darray<Shape>& tools);

Shape common(const Shape& lhs, const Shape& rhs);

Shape cut(const Shape& lhs, const Shape& rhs);


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

        return vec3 {center.X(), center.Y(), center.Z()};
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

        return sarray<vec3, 2> {{p1.X(), p1.Y(), p1.Z()}, {p2.X(), p2.Y(), p2.Z()}};
    }

    void incrementalMesh(double deflection)
    {
        BRepTools::Clean(p_shape);
        BRepMesh_IncrementalMesh(p_shape, deflection, true);
    }

    darray<Shape> subShapes(Type type) const
    {
        darray<Shape> subshapes;
        for (TopExp_Explorer exp(p_shape, static_cast<TopAbs_ShapeEnum>(type)); exp.More(); exp.Next())
            subshapes.push(Shape(exp.Current()));
        return subshapes;
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

    // Member functions: transformations

    Shape translate(const vec3& dir)
    {
        gp_Trsf transform;
        transform.SetTranslation(gp_Vec(dir[0], dir[1], dir[2]));
        BRepBuilderAPI_Transform builder {p_shape, transform, true};

        return builder.Shape();
    }

    Shape rotate(const vec3& pos, const vec3& axis, double angle)
    {
        gp_Trsf transform;
        transform.SetRotation(gp_Ax1({pos[0], pos[1], pos[2]}, {axis[0], axis[1], axis[2]}), radians(angle));
        BRepBuilderAPI_Transform builder {p_shape, transform, true};

        return builder.Shape();
    }

    Shape scale(const vec3& center, double scale)
    {
        gp_Trsf transform;
        transform.SetScale(gp_Pnt(center[0], center[1], center[2]), scale);
        BRepBuilderAPI_Transform builder {p_shape, transform, true};

        return builder.Shape();
    }

    Shape& scaled(const vec3& center, double scale)
    {
        p_shape = this->scale(center, scale).p_shape;
        return *this;
    }

    Shape extrude(const vec3& dir, double length)
    {
        BRepPrimAPI_MakePrism builder {p_shape, length * gp_Vec(dir[0], dir[1], dir[2]), true};

        for (auto& type : { Type::Solid, Type::Face, Type::Edge, Type::Vertex })
            for (TopExp_Explorer exp {p_shape, static_cast<TopAbs_ShapeEnum>(type)}; exp.More(); exp.Next())
            {
                auto data = metadata[Shape(exp.Current())];

                for (auto& mod : builder.Generated(exp.Current()))
                    metadata[Shape(mod)].merge(data);
            }

        return builder.Shape();
    }

    Shape fillet(darray<Shape> edges, double radius)
    {
        BRepFilletAPI_MakeFillet fillet {p_shape};

        for (auto& e : edges)
            fillet.Add(radius, TopoDS::Edge(e.p_shape));
        fillet.Build();

        return fillet.Shape();
    }

    // Friend functions

    friend
    double distance(const Shape& lhs, const Shape& rhs)
    {
        return BRepExtrema_DistShapeShape(lhs.tshape(), rhs.tshape()).Value();
    }

    friend
    Shape fuse(const Shape& lhs, const Shape& rhs)
    {
        BRepAlgoAPI_Fuse builder {lhs.p_shape, rhs.p_shape};
        builder.Build();
        return Shape {builder.Shape()};
    }

    friend
    Shape fuse(const darray<Shape>& args, const darray<Shape>& tools)
    {
        BRepAlgoAPI_Fuse builder;
        NCollection_List<TopoDS_Shape> args_, tools_;
        for (auto& arg : args)
            args_.Append(arg.tshape());
        for (auto& tool : tools)
            tools_.Append(tool.tshape());
        builder.SetArguments(args_);
        builder.SetTools(tools_);
        builder.Build();
        return Shape {builder.Shape()};
    }

    friend
    Shape common(const Shape& lhs, const Shape& rhs)
    {
        BRepAlgoAPI_Common builder {lhs.p_shape, rhs.p_shape};
        builder.Build();
        return Shape {builder.Shape()};
    }

    friend
    Shape cut(const Shape& lhs, const Shape& rhs)
    {
        BRepAlgoAPI_Cut builder {lhs.p_shape, rhs.p_shape};
        builder.Build();
        for (auto& type : { Type::Solid, Type::Face, Type::Edge, Type::Vertex })
            for (TopExp_Explorer exp {lhs.p_shape, static_cast<TopAbs_ShapeEnum>(type)}; exp.More(); exp.Next())
            {
                auto data = metadata[Shape(exp.Current())];

                for (auto& mod : builder.Modified(exp.Current()))
                    metadata[Shape(mod)].merge(data);
            }
        for (auto& type : { Type::Solid, Type::Face, Type::Edge, Type::Vertex })
            for (TopExp_Explorer exp {rhs.p_shape, static_cast<TopAbs_ShapeEnum>(type)}; exp.More(); exp.Next())
            {
                auto data = metadata[Shape(exp.Current())];

                for (auto& mod : builder.Modified(exp.Current()))
                    metadata[Shape(mod)].merge(data);
            }
        return Shape {builder.Shape()};
    }

};

// Global functions: primitives

Shape sphere(vec3 center, double radius);

Shape box(vec3 corner, double dx, double dy, double dz);

}




