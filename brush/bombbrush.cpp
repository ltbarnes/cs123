/**
  * @file   BombBrush.cpp
  *
  * Implementation of a brush which 'throws' paint accross the canvas
  */

#include <CS123Common.h>
#include <math.h>
#include <cmath>

#include "BombBrush.h"
#include "Canvas2D.h"


BombBrush::BombBrush(BGRA color, int flow, int radius)
    : Brush(color, flow, radius)
{
    makeMask();
    m_bombRadius = 2;
    m_bombWidth = 2 * m_bombRadius + 1;
    m_bombSize = m_bombWidth * m_bombWidth;
    m_bomb = new frag[m_bombSize];
    m_maxHeight = 60;
    m_blown = 0;
}


BombBrush::~BombBrush()
{
    delete[] m_bomb;
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


void BombBrush::blowUp(int x, int y, Canvas2D* canvas)
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

//            m_bomb[mi].color.r = 255;
//            m_bomb[mi].color.g = 255;
//            m_bomb[mi].color.b = 255;
//            m_bomb[mi].color.a = 255;

            pix[ci].r = (unsigned char) (pix[ci].r * (1.f - m_mask[mi]));
            pix[ci].g = (unsigned char) (pix[ci].g * (1.f - m_mask[mi]));
            pix[ci].b = (unsigned char) (pix[ci].b * (1.f - m_mask[mi]));
            pix[ci].a = 255;
        }
    }

    for (int i = 0; i < m_bombSize; i++) {
        float angle = (2.f * M_PI * i) / m_bombSize;

        m_bomb[i].x = x;
        m_bomb[i].y = y;
        m_bomb[i].h = 0;
        m_bomb[i].dx = cos(angle) * 1.5f;
        m_bomb[i].dy = sin(angle) * 1.5f;
        m_bomb[i].dh = 1;
        m_bomb[i].color.r = 255;
        m_bomb[i].color.g = 255;
        m_bomb[i].color.b = 255;
        m_bomb[i].color.a = 255;
    }
}

void BombBrush::update()
{

    for (int i = 0; i < m_bombSize; i++) {
        m_bomb[i].x += m_bomb[i].dx;
        m_bomb[i].y += m_bomb[i].dy;
        m_bomb[i].h += m_bomb[i].dh;
        if (m_bomb[i].h > m_maxHeight) {
            m_bomb[i].dh = -m_bomb[i].dh;
        }
        if (m_bomb[i].h < 0) {
            m_bomb[i].h = 0;
            m_bomb[i].dx = 0;
            m_bomb[i].dy = 0;
            m_bomb[i].dh = 0;
        }
    }
}

void BombBrush::renderAnimation(Canvas2D *canvas)
{
    BGRA *pix = canvas->data();
    int width = canvas->width();
    int height = canvas->height();
    int size = width * height;

    for (int i = 0; i < size; i++) {
        pix[i].r = m_drawLayer[i].r;
        pix[i].g = m_drawLayer[i].g;
        pix[i].b = m_drawLayer[i].b;
    }

    for (int i = 0; i < m_bombSize; i++) {
        int r = (int)(m_bomb[i].y + 0.5f);
        int c = (int)(m_bomb[i].x + 0.5f);
        if (r >= width || c >= height || r < 0 || c < 0)
            continue;
        int h = m_bomb[i].h / 6 + 2;
        for (int row = r - h; row <= r + h; row++) {
            for (int col = c - h; col <= c+h; col++) {
                if ((row - r) * (row - r) + (col - c) * (col - c) < h * h) {
                    int ci = width * row + col;
                    pix[ci].r = m_bomb[i].color.r;
                    pix[ci].g = m_bomb[i].color.g;
                    pix[ci].b = m_bomb[i].color.b;
                }
            }
        }
    }

}


void BombBrush::paintOnce(int mouse_x, int mouse_y, Canvas2D* canvas)
{
    // now pick up paint again...
    if (!m_blown) {
        blowUp(mouse_x, mouse_y, canvas);
        m_blown = 1;
    }
}
