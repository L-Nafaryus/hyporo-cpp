#pragma once

#include "geometry.hpp"
#include "face.hpp"


namespace hpr::csg
{

    class Surface : public Geometry
    {

    protected:

        Handle(Geom_Surface) p_surface;

    public:

        Surface() = default;

        ~Surface() override = default;

        explicit
        Surface(const Face& face) :
            Geometry {},
            p_surface {BRep_Tool::Surface(face.tcast())}
        {}

        [[nodiscard]]
        Handle(Geom_Surface) tcast() const
        {
            return p_surface;
        }

        [[nodiscard]]
        vec3 value(double u, double v) const
        {
            gp_Pnt p {p_surface->Value(u, v)};

            return vec3 {p.X(), p.Y(), p.Z()};
        }

        [[nodiscard]]
        vec3 normal(double u, double v) const
        {
            GeomLProp_SLProps props {p_surface, u, v, 1, 1e-8};
            gp_Dir dir {props.Normal()};

            return vec3 {dir.X(), dir.Y(), dir.Z()};
        }

        vec3 normal()
        {
            gp_Vec du, dv;
            gp_Pnt p;
            p_surface->D1(0, 0, p, du, dv);
            gp_Vec dir {du ^ dv};
            return vec3 {dir.X(), dir.Y(), dir.Z()};
        }
    };

}


