//----------------------------------------------------------------------------
// Anti-Grain Geometry (AGG) - Version 2.5
// A high quality rendering engine for C++
// Copyright (C) 2002-2006 Maxim Shemanarev
// Contact: mcseem@antigrain.com
//          mcseemagg@yahoo.com
//          http://antigrain.com
// 
// AGG is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
// 
// AGG is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with AGG; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, 
// MA 02110-1301, USA.
//----------------------------------------------------------------------------

#ifndef AGG_VCGEN_CONTOUR_INCLUDED
#define AGG_VCGEN_CONTOUR_INCLUDED

#include "agg_math_stroke.h"

namespace agg
{

    //----------------------------------------------------------vcgen_contour
    //
    // See Implementation agg_vcgen_contour.cpp
    //
    class vcgen_contour
    {
        enum status_e
        {
            initial,
            ready,
            outline,
            out_vertices,
            end_poly,
            stop
        };

    public:
        typedef vertex_sequence<vertex_dist, 6> vertex_storage;
        typedef pod_bvector<point_d, 6>         coord_storage;

        vcgen_contour();

        void line_cap(line_cap_e lc)     { m_stroker.line_cap(lc); }
        void line_join(line_join_e lj)   { m_stroker.line_join(lj); }
        void inner_join(inner_join_e ij) { m_stroker.inner_join(ij); }

        line_cap_e   line_cap()   const { return m_stroker.line_cap(); }
        line_join_e  line_join()  const { return m_stroker.line_join(); }
        inner_join_e inner_join() const { return m_stroker.inner_join(); }

        void width(float_t w) { m_stroker.width(m_width = w); }
        void miter_limit(float_t ml) { m_stroker.miter_limit(ml); }
        void miter_limit_theta(float_t t) { m_stroker.miter_limit_theta(t); }
        void inner_miter_limit(float_t ml) { m_stroker.inner_miter_limit(ml); }
        void approximation_scale(float_t as) { m_stroker.approximation_scale(as); }

        float_t width() const { return m_width; }
        float_t miter_limit() const { return m_stroker.miter_limit(); }
        float_t inner_miter_limit() const { return m_stroker.inner_miter_limit(); }
        float_t approximation_scale() const { return m_stroker.approximation_scale(); }

        void auto_detect_orientation(bool v) { m_auto_detect = v; }
        bool auto_detect_orientation() const { return m_auto_detect; }

        // Generator interface
        void remove_all();
        void add_vertex(float_t x, float_t y, unsigned cmd);

        // Vertex Source Interface
        void     rewind(unsigned path_id);
        unsigned vertex(float_t* x, float_t* y);

    private:
        vcgen_contour(const vcgen_contour&);
        const vcgen_contour& operator = (const vcgen_contour&);

        math_stroke<coord_storage> m_stroker;
        float_t                     m_width;
        vertex_storage             m_src_vertices;
        coord_storage              m_out_vertices;
        status_e                   m_status;
        unsigned                   m_src_vertex;
        unsigned                   m_out_vertex;
        unsigned                   m_closed;
        unsigned                   m_orientation;
        bool                       m_auto_detect;
    };

}

#endif
