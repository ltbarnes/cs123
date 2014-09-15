/**
 * @file   QuadraticBrush.cpp
 *
 * Implementation of a brush with a quadratic mask distribution.
 *
 * You should fill this file in while completing the Brush assignment.
 */

#include "QuadraticBrush.h"

QuadraticBrush::QuadraticBrush(BGRA color, int flow, int radius)
    : Brush(color, flow, radius)
{
    makeMask();
}


QuadraticBrush::~QuadraticBrush()
{

}


void QuadraticBrush::makeMask()
{
    // width
    int w = 2 * m_radius + 1;

    // iterate through mask from -radius to +radius
    for (int r = - m_radius; r <= m_radius; r++) {
        for (int c = - m_radius; c <= m_radius; c++) {

            // calculate the array index
            int i = w * (r + m_radius) + (c + m_radius);

            // create a quadratic mask
            float dist = sqrt(r * r + c * c);

            if (dist< m_radius) {
                float linear = (m_radius - dist) / m_radius;
                m_mask[i] = linear * linear;
            } else {
                m_mask[i] = 0;
            }
        }
    }
}


