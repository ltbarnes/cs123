/**
  * @file   BombBrush.cpp
  *
  * Implementation of a brush which 'throws' paint accross the canvas
  */

#include <CS123Common.h>

#include "BombBrush.h"
#include "Canvas2D.h"

BombBrush::BombBrush(BGRA color, int flow, int radius)
    : Brush(color, flow, radius)
{
    makeMask();
    int width = 2 * m_radius + 1;
    m_bomb = new BGRA[width * width];
    m_blown = 0;
    m_background = NULL;
}


BombBrush::~BombBrush()
{
    delete[] m_bomb;
    if (m_background)
        delete[] m_background;
}

void BombBrush::makeMask()
{
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
                float quad = linear * linear;
                m_mask[i] = 2 * quad - quad * quad;
            } else {
                m_mask[i] = 0.f;
            }
        }
    }
}


void BombBrush::pickUpPaint(int x, int y, Canvas2D* canvas)
{
    int width = canvas->width();
    int height = canvas->height();

    // @TODO: [BRUSH] Perform the "pick up paint" step described in the assignment handout here.
    //        In other words, you should store the "paint" under the brush mask in a temporary
    //        buffer (which you'll also have to figure out where and how to allocate). Then,
    //        in the paintOnce() method, you'll paste down the paint that you picked up here.
    //
    BGRA* pix = canvas->data();

    int rowStart = std::max(0, y - m_radius);
    int rowEnd = std::min(height - 1, y + m_radius);
    int colStart = std::max(0, x - m_radius);
    int colEnd = std::min(width - 1, x + m_radius);
    int maskWidth = 2 * m_radius + 1;

    for (int r = rowStart; r <= rowEnd; ++r) {
        for (int c = colStart; c <= colEnd; ++c) {
            // canvas index
            int ci = width * r + c;
            // mask index
            int mi = maskWidth * (r - y + m_radius) + (c - x + m_radius);

            m_bomb[mi].r = pix[ci].r;
            m_bomb[mi].g = pix[ci].g;
            m_bomb[mi].b = pix[ci].b;
            m_bomb[mi].a = 255;

            pix[ci].r = (unsigned char) (pix[ci].r * (1.f - m_mask[mi]));
            pix[ci].g = (unsigned char) (pix[ci].g * (1.f - m_mask[mi]));
            pix[ci].b = (unsigned char) (pix[ci].b * (1.f - m_mask[mi]));
            pix[ci].a = 255;
        }
    }
}

void BombBrush::update()
{

}


void BombBrush::paintOnce(int mouse_x, int mouse_y, Canvas2D* canvas)
{

    // now pick up paint again...
    if (!m_blown) {
        pickUpPaint(mouse_x, mouse_y, canvas);
        m_blown = 1;
    }
}
