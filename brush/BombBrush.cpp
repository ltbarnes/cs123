/**
  * @file   BombBrush.cpp
  *
  * Implementation of a brush which 'throws' paint accross the canvas
  */

#include <CS123Common.h>
#include <math.h>
#include <cmath>
#include <stdlib.h>

#include "BombBrush.h"
#include "Canvas2D.h"


BombBrush::BombBrush(BGRA color, int flow, int radius)
    : Brush(color, flow, radius)
{
    makeMask();
    m_bombRadius = m_radius / 24 + 2;
    m_bombWidth = 2 * m_bombRadius + 1;
    m_bombSize = m_bombWidth * m_bombWidth;
    m_bomb = new blob[m_bombSize];

    m_blown = false;

    m_accel = -10;
    m_vel = 7;
    m_time = 0;
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

    m_centerX = x;
    m_centerY = y;

    int width = canvas->width();
    int height = canvas->height();

    BGRA* pix = canvas->data();

    // create brush bounds
    int rowStart = std::max(0, y - m_radius);
    int rowEnd = std::min(height - 1, y + m_radius);
    int colStart = std::max(0, x - m_radius);
    int colEnd = std::min(width - 1, x + m_radius);
    int maskWidth = 2 * m_radius + 1;

    // color bomb droplets
    for (int i = 0; i < m_bombSize; i++) {
        // set evenly spaced directions in a 360
        float horizAngle = (2.f * M_PI * i) / m_bombSize;

        // choose random vertical angle
        float vertAngle = rand() % 40 * 0.05f;

        // get the vectors of the angle
        float vertVel = sin(vertAngle) * m_vel;
        float horizVel = cos(vertAngle) * m_vel;

        m_bomb[i].x = x;
        m_bomb[i].y = y;
        m_bomb[i].h = 0;

        // set the vectors of the horizontal angle
        m_bomb[i].dx = cos(horizAngle) * horizVel;
        m_bomb[i].dy = sin(horizAngle) * horizVel;
        m_bomb[i].dh = vertVel;

        // choose random color from within the radial area
        int rowMod = rowEnd - rowStart;
        int colMod = colEnd - colStart;
        int r, c;
        if (rowMod == 0)
            r = rowStart;
        else
            r = rand() % (rowEnd - rowStart) + rowStart;

        if (colMod == 0)
            c = colStart;
        else
            c = rand() % (colEnd - colStart) + colStart;

        int ci = width * r + c;

        m_bomb[i].color.r = pix[ci].r;
        m_bomb[i].color.g = pix[ci].g;
        m_bomb[i].color.b = pix[ci].b;
        m_bomb[i].color.a = 255;
    }

    // make quadradic crater
    for (int r = rowStart; r <= rowEnd; ++r) {
        for (int c = colStart; c <= colEnd; ++c) {
            // canvas index
            int ci = width * r + c;
            // mask index
            int mi = maskWidth * (r - y + m_radius) + (c - x + m_radius);

            pix[ci].r = (unsigned char) (pix[ci].r * (1.f - m_mask[mi]));
            pix[ci].g = (unsigned char) (pix[ci].g * (1.f - m_mask[mi]));
            pix[ci].b = (unsigned char) (pix[ci].b * (1.f - m_mask[mi]));
            pix[ci].a = 255;
        }
    }

}

void BombBrush::update()
{
    // increase time
    m_time += 0.02;

    // recalculate blob positions
    for (int i = 0; i < m_bombSize; i++) {
        m_bomb[i].x += m_bomb[i].dx;
        m_bomb[i].y += m_bomb[i].dy;
        m_bomb[i].h = m_bomb[i].dh * m_time + 0.5f * m_accel * m_time * m_time;
        if (m_bomb[i].h < 0) {
            m_bomb[i].h = 0;
            m_bomb[i].dx = 0;
            m_bomb[i].dy = 0;
        }
    }
}

void BombBrush::renderAnimation(Canvas2D *canvas)
{
    BGRA *pix = canvas->data();
    int width = canvas->width();
    int height = canvas->height();
    int size = width * height;

    // reset the canvas image without blobs
    for (int i = 0; i < size; i++) {
        pix[i].r = m_drawLayer[i].r;
        pix[i].g = m_drawLayer[i].g;
        pix[i].b = m_drawLayer[i].b;
    }

    // draw shadows
    for (int i = 0; i < m_bombSize; i++) {
        int r = (int)(m_bomb[i].y + m_bomb[i].h * 5.f + 1.5f);
        int c = (int)(m_bomb[i].x + m_bomb[i].h * 5.f + 1.5f);
        int h = 5;

        for (int row = r - h; row <= r + h; row++) {
            for (int col = c - h; col <= c + h; col++) {

                if ((row - r) * (row - r) + (col - c) * (col - c) < h * h) {

                    if (row >= width || col >= height || row < 0 || col < 0)
                        continue;
                    int ci = width * row + col;
                    pix[ci].r = 0;
                    pix[ci].g = 0;
                    pix[ci].b = 0;
                }
            }
        }
    }

    // draw paint blob
    for (int i = 0; i < m_bombSize; i++) {
        int r = (int)(m_bomb[i].y + 0.5f);
        int c = (int)(m_bomb[i].x + 0.5f);
        float h = (m_bomb[i].h + 2.f) * 2.0f + 0.5f;
        int intH = (int)(h + 0.5f);

        for (int row = r - intH; row <= r + intH; row++) {
            for (int col = c - intH; col <= c + intH; col++) {

                if ((row - r) * (row - r) + (col - c) * (col - c) < h * h) {

                    if (row >= width || col >= height || row < 0 || col < 0)
                        continue;
                    int ci = width * row + col;
                    pix[ci].r = m_bomb[i].color.r;
                    pix[ci].g = m_bomb[i].color.g;
                    pix[ci].b = m_bomb[i].color.b;
                }
            }
        }
        // make it pretty (add white reflection dot)
        int lr = r - (h / 3.f + 0.5f);
        int lc = c - (h / 3.f + 0.5f);
        if (lr >= width || lc >= height || lr < 0 || lc < 0)
            continue;
        int ci = width * lr + lc;
        pix[ci].r = 255;
        pix[ci].g = 255;
        pix[ci].b = 255;
    }


}


void BombBrush::paintOnce(int mouse_x, int mouse_y, Canvas2D* canvas)
{
    if (!m_blown) { // initiate bomb blowing
        blowUp(mouse_x, mouse_y, canvas);
        m_blown = true;
    } else { // have blobs follow general mouse direction
            float x = (mouse_x - m_centerX) * 0.025f;
            float y = (mouse_y - m_centerY) * 0.025f;

            for (int i = 0; i < m_bombSize; i++) {
                if (m_bomb[i].h > 0) {
                    m_bomb[i].dx += x;
                    m_bomb[i].dy += y;
                }
        }
        m_centerX = mouse_x;
        m_centerY = mouse_y;
    }
}
