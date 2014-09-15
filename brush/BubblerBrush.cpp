/**
  * @file   BubblerBrush.cpp
  *
  * Implementation of a brush which 'throws' paint accross the canvas
  */

#include <CS123Common.h>

#include "BubblerBrush.h"
#include "Canvas2D.h"

BubblerBrush::BubblerBrush(BGRA color, int flow, int radius)
    : Brush(color, flow, radius)
{
    makeMask();
    m_used = false;
}


BubblerBrush::~BubblerBrush()
{

}

void BubblerBrush::makeMask()
{
    int w = 2 * m_radius + 1;

    // iterate through mask from -radius to +radius
    for (int r = - m_radius; r <= m_radius; r++) {
        for (int c = - m_radius; c <= m_radius; c++) {

            // calculate the array index
            int i = w * (r + m_radius) + (c + m_radius);

            // create a weird mask
            float dist = sqrt(r * r + c * c);

            if (dist< m_radius) {
                float linear = (m_radius - dist) / m_radius;
                m_mask[i] = (linear * linear) - (2 * linear);
            } else {
                m_mask[i] = 0.f;
            }
        }
    }
}


void BubblerBrush::paintOnce(int mouse_x, int mouse_y, Canvas2D* canvas)
{

    if (m_used) { // one time use brush
        return;
    }
    m_used = true;

    int width = canvas->width();
    int height = canvas->height();

    BGRA* pix = canvas->data();

    // set the brush bounds
    int rowStart = std::max(0, mouse_y - m_radius);
    int rowEnd = std::min(height - 1, mouse_y + m_radius);
    int colStart = std::max(0, mouse_x - m_radius);
    int colEnd = std::min(width - 1, mouse_x + m_radius);
    int maskWidth = 2 * m_radius + 1;

    // iterate through the brush area applying the mask
    for (int r = rowStart; r <= rowEnd; ++r) {
        for (int c = colStart; c <= colEnd; ++c) {

            // canvas index
            int ci = width * r + c;
            // mask index
            int mi = maskWidth * (r - mouse_y + m_radius) + (c - mouse_x + m_radius);

            pix[ci].r = (unsigned char) (pix[ci].r * (1.f - m_mask[mi]) + 0.5f);
            pix[ci].g = (unsigned char) (pix[ci].g * (1.f - m_mask[mi]) + 0.5f);
            pix[ci].b = (unsigned char) (pix[ci].b * (1.f - m_mask[mi]) + 0.5f);
            pix[ci].a = 255;
        }
    }
}
