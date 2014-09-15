/**
 * @file   Brush.cpp
 *
 * Implementation of a brush that smudges the image on the canvas as the mouse is dragged.
 *
 * You should fill this file in for the Brush assignment.
 */

#include <CS123Common.h>

#include "SmudgeBrush.h"
#include "Canvas2D.h"


SmudgeBrush::SmudgeBrush(BGRA color, int flow, int radius) : QuadraticBrush(color, flow, radius)
{
    makeMask(); // uses quadradic mask
    int width = 2 * m_radius + 1;
    m_finger = new BGRA[width * width];
    m_smudged = false;
}


SmudgeBrush::~SmudgeBrush()
{
    delete[] m_finger;
}


//! Picks up paint from the canvas before drawing begins.
void SmudgeBrush::pickUpPaint(int x, int y, Canvas2D* canvas)
{
    int width = canvas->width();
    int height = canvas->height();

    BGRA* pix = canvas->data();

    // set brush bounds
    int rowStart = std::max(0, y - m_radius);
    int rowEnd = std::min(height - 1, y + m_radius);
    int colStart = std::max(0, x - m_radius);
    int colEnd = std::min(width - 1, x + m_radius);
    int maskWidth = 2 * m_radius + 1;

    // iterate through brush area storing color data from the canvas
    for (int r = rowStart; r <= rowEnd; ++r) {
        for (int c = colStart; c <= colEnd; ++c) {

            // canvas index
            int ci = width * r + c;
            // mask index
            int mi = maskWidth * (r - y + m_radius) + (c - x + m_radius);

            m_finger[mi].r = pix[ci].r;
            m_finger[mi].g = pix[ci].g;
            m_finger[mi].b = pix[ci].b;
            m_finger[mi].a = 255;

        }
    }
}

void SmudgeBrush::paintOnce(int mouse_x, int mouse_y, Canvas2D* canvas)
{
    if (m_smudged) { // only paint after picking up paint the first time
        BGRA* pix = canvas->data();

        int w = canvas->width();
        int rowStart = std::max(0, mouse_y - m_radius);
        int rowEnd = std::min(canvas->height() - 1, mouse_y + m_radius);
        int colStart = std::max(0, mouse_x - m_radius);
        int colEnd = std::min(w - 1, mouse_x + m_radius);
        int maskWidth = 2 * m_radius + 1;

        for (int r = rowStart; r <= rowEnd; ++r) {
            for (int c = colStart; c <= colEnd; ++c) {

                // canvas index
                int ci = w * r + c;
                // mask index
                int mi = maskWidth * (r - mouse_y + m_radius) + (c - mouse_x + m_radius);
                float p = m_mask[mi];

                pix[ci].r = (unsigned char) (pix[ci].r * (1.f - p) + (m_finger[mi].r * p) + 0.5f);
                pix[ci].g = (unsigned char) (pix[ci].g * (1.f - p) + (m_finger[mi].g * p) + 0.5f);
                pix[ci].b = (unsigned char) (pix[ci].b * (1.f - p) + (m_finger[mi].b * p) + 0.5f);
                pix[ci].a = 255;

            }
        }
    }

    // now pick up paint again...
    pickUpPaint(mouse_x, mouse_y, canvas);
    m_smudged = true;
}


