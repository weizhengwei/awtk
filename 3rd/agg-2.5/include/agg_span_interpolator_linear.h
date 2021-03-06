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

#ifndef AGG_SPAN_INTERPOLATOR_LINEAR_INCLUDED
#define AGG_SPAN_INTERPOLATOR_LINEAR_INCLUDED

#include "agg_basics.h"
#include "agg_dda_line.h"
#include "agg_trans_affine.h"

namespace agg
{

    //================================================span_interpolator_linear
    template<class Transformer = trans_affine, unsigned SubpixelShift = 8> 
    class span_interpolator_linear
    {
    public:
        typedef Transformer trans_type;

        enum subpixel_scale_e
        {
            subpixel_shift = SubpixelShift,
            subpixel_scale  = 1 << subpixel_shift
        };

        //--------------------------------------------------------------------
        span_interpolator_linear() {}
        span_interpolator_linear(const trans_type& trans) : m_trans(&trans) {}
        span_interpolator_linear(const trans_type& trans,
                                 float_t x, float_t y, unsigned len) :
            m_trans(&trans)
        {
            begin(x, y, len);
        }

        //----------------------------------------------------------------
        const trans_type& transformer() const { return *m_trans; }
        void transformer(const trans_type& trans) { m_trans = &trans; }

        //----------------------------------------------------------------
        void begin(float_t x, float_t y, unsigned len)
        {
            float_t tx;
            float_t ty;

            tx = x;
            ty = y;
            m_trans->transform(&tx, &ty);
            int x1 = iround(tx * subpixel_scale);
            int y1 = iround(ty * subpixel_scale);

            tx = x + len;
            ty = y;
            m_trans->transform(&tx, &ty);
            int x2 = iround(tx * subpixel_scale);
            int y2 = iround(ty * subpixel_scale);

            m_li_x = dda2_line_interpolator(x1, x2, len);
            m_li_y = dda2_line_interpolator(y1, y2, len);
        }

        //----------------------------------------------------------------
        void resynchronize(float_t xe, float_t ye, unsigned len)
        {
            m_trans->transform(&xe, &ye);
            m_li_x = dda2_line_interpolator(m_li_x.y(), iround(xe * subpixel_scale), len);
            m_li_y = dda2_line_interpolator(m_li_y.y(), iround(ye * subpixel_scale), len);
        }
    
        //----------------------------------------------------------------
        void operator++()
        {
            ++m_li_x;
            ++m_li_y;
        }

        //----------------------------------------------------------------
        void coordinates(int* x, int* y) const
        {
            *x = m_li_x.y();
            *y = m_li_y.y();
        }

    private:
        const trans_type* m_trans;
        dda2_line_interpolator m_li_x;
        dda2_line_interpolator m_li_y;
    };






    //=====================================span_interpolator_linear_subdiv
    template<class Transformer = trans_affine, unsigned SubpixelShift = 8> 
    class span_interpolator_linear_subdiv
    {
    public:
        typedef Transformer trans_type;

        enum subpixel_scale_e
        {
            subpixel_shift = SubpixelShift,
            subpixel_scale = 1 << subpixel_shift
        };


        //----------------------------------------------------------------
        span_interpolator_linear_subdiv() :
            m_subdiv_shift(4),
            m_subdiv_size(1 << m_subdiv_shift),
            m_subdiv_mask(m_subdiv_size - 1) {}

        span_interpolator_linear_subdiv(const trans_type& trans, 
                                        unsigned subdiv_shift = 4) : 
            m_subdiv_shift(subdiv_shift),
            m_subdiv_size(1 << m_subdiv_shift),
            m_subdiv_mask(m_subdiv_size - 1),
            m_trans(&trans) {}

        span_interpolator_linear_subdiv(const trans_type& trans,
                                        float_t x, float_t y, unsigned len,
                                        unsigned subdiv_shift = 4) :
            m_subdiv_shift(subdiv_shift),
            m_subdiv_size(1 << m_subdiv_shift),
            m_subdiv_mask(m_subdiv_size - 1),
            m_trans(&trans)
        {
            begin(x, y, len);
        }

        //----------------------------------------------------------------
        const trans_type& transformer() const { return *m_trans; }
        void transformer(const trans_type& trans) { m_trans = &trans; }

        //----------------------------------------------------------------
        unsigned subdiv_shift() const { return m_subdiv_shift; }
        void subdiv_shift(unsigned shift) 
        {
            m_subdiv_shift = shift;
            m_subdiv_size = 1 << m_subdiv_shift;
            m_subdiv_mask = m_subdiv_size - 1;
        }

        //----------------------------------------------------------------
        void begin(float_t x, float_t y, unsigned len)
        {
            float_t tx;
            float_t ty;
            m_pos   = 1;
            m_src_x = iround(x * subpixel_scale) + subpixel_scale;
            m_src_y = y;
            m_len   = len;

            if(len > m_subdiv_size) len = m_subdiv_size;
            tx = x;
            ty = y;
            m_trans->transform(&tx, &ty);
            int x1 = iround(tx * subpixel_scale);
            int y1 = iround(ty * subpixel_scale);

            tx = x + len;
            ty = y;
            m_trans->transform(&tx, &ty);

            m_li_x = dda2_line_interpolator(x1, iround(tx * subpixel_scale), len);
            m_li_y = dda2_line_interpolator(y1, iround(ty * subpixel_scale), len);
        }

        //----------------------------------------------------------------
        void operator++()
        {
            ++m_li_x;
            ++m_li_y;
            if(m_pos >= m_subdiv_size)
            {
                unsigned len = m_len;
                if(len > m_subdiv_size) len = m_subdiv_size;
                float_t tx = float_t(m_src_x) / float_t(subpixel_scale) + len;
                float_t ty = m_src_y;
                m_trans->transform(&tx, &ty);
                m_li_x = dda2_line_interpolator(m_li_x.y(), iround(tx * subpixel_scale), len);
                m_li_y = dda2_line_interpolator(m_li_y.y(), iround(ty * subpixel_scale), len);
                m_pos = 0;
            }
            m_src_x += subpixel_scale;
            ++m_pos;
            --m_len;
        }

        //----------------------------------------------------------------
        void coordinates(int* x, int* y) const
        {
            *x = m_li_x.y();
            *y = m_li_y.y();
        }

    private:
        unsigned m_subdiv_shift;
        unsigned m_subdiv_size;
        unsigned m_subdiv_mask;
        const trans_type* m_trans;
        dda2_line_interpolator m_li_x;
        dda2_line_interpolator m_li_y;
        int      m_src_x;
        float_t   m_src_y;
        unsigned m_pos;
        unsigned m_len;
    };


}



#endif


