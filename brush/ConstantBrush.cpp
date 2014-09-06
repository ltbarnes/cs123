/**
 * @file   ConstantBrush.cpp
 *
 * Implementation of a brush with a constant mask distribution.
 *
 * You should fill this file in while completing the Brush assignment.
 */

#include "ConstantBrush.h"

ConstantBrush::ConstantBrush(BGRA color, int flow, int radius)
    : Brush(color, flow, radius)
{
    // @TODO: [BRUSH] You'll probably want to set up the mask right away.
    makeMask();
}


ConstantBrush::~ConstantBrush()
{
    // @TODO: [BRUSH] Delete any resources owned by this brush, so you don't leak memory.
}

void ConstantBrush::makeMask()
{
    // @TODO: [BRUSH] Set up the mask for your Constant brush here...

    int rad2 = m_radius * m_radius;
    int w = 2 * m_radius + 1;

    // iterate through mask from -radius to +radius
    for (int r = - m_radius; r <= m_radius; r++) {
        for (int c = - m_radius; c <= m_radius; c++) {

            // calculate the array index
            int i = w * (r + m_radius) + (c + m_radius);

            // if the mask coordinate is within the radius set it to 1, otherwise 0
            if ((r * r + c * c) <= rad2)
                m_mask[i] = 1.f;
            else
                m_mask[i] = 0.f;
        }
    }
}


