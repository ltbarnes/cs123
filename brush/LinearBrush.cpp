/**
 * @file   LinearBrush.cpp
 *
 * Implementation of a brush with a linear mask distribution.
 *
 * You should fill this file in while completing the Brush assignment.
 */

#include "LinearBrush.h"

LinearBrush::LinearBrush(BGRA color, int flow, int radius)
    : Brush(color, flow, radius)
{
    // @TODO: [BRUSH] You'll probably want to set up the mask right away.
    makeMask();
}


LinearBrush::~LinearBrush()
{
    // @TODO: [BRUSH] Delete any resources owned by this brush, so you don't leak memory.

}

void LinearBrush::makeMask()
{
    // @TODO: [BRUSH] Set up the mask for your Linear brush here...

    int w = 2 * m_radius + 1;

    // iterate through mask from -radius to +radius
    for (int r = - m_radius; r <= m_radius; r++) {
        for (int c = - m_radius; c <= m_radius; c++) {

            // calculate the array index
            int i = w * (r + m_radius) + (c + m_radius);

            // create a linear mask
            float dist = sqrt(r * r + c * c);

            if (dist< m_radius) {
                m_mask[i] = (m_radius - dist) / m_radius;
            } else {
                m_mask[i] = 0;
            }
        }
    }
}


