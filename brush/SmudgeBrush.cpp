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
    // @TODO: [BRUSH] Initialize any memory you are going to use here. Hint - you are going to
    //       need to store temporary image data in memory. This might be a good place to
    //       allocate that memory.


    makeMask();
    int width = 2 * m_radius + 1;
    m_finger = new BGRA[width * width];
    m_smudged = 0;
}


SmudgeBrush::~SmudgeBrush()
{
    // @TODO: [BRUSH] Be sure not to leak memory!
    delete[] m_finger;
}



//! create a mask with a distribution of your choice (probably want to use quadratic for best results)
void SmudgeBrush::makeMask()
{
    // @TODO: [BRUSH] Set up the mask for your brush here. For this brush you will probably want
    //        to use a quadratic distribution for the best results. Linear or Gaussian would
    //        work too, however. Feel free to paste your code from the Linear or Quadratic brushes
    //        or modify the class inheritance to be able to take advantage of one of those class's
    //        existing implementations. The choice is yours!
    //

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

//! Picks up paint from the canvas before drawing begins.
void SmudgeBrush::pickUpPaint(int x, int y, Canvas2D* canvas)
{
    int width = canvas->width();
    int height = canvas->height();

    // @TODO: [BRUSH] Perform the "pick up paint" step described in the assignment handout here.
    //        In other words, you should store the "paint" under the brush mask in a temporary
    //        buffer (which you'll also have to figure out where and how to allocate). Then,
    //        in the paintOnce() method, you'll paste down the paint that you picked up here.
    //
    BGRA* pix = canvas->data();

    int w = canvas->width();
    int rowStart = std::max(0, y - m_radius);
    int rowEnd = std::min(canvas->height(), y + m_radius);
    int colStart = std::max(0, x - m_radius);
    int colEnd = std::min(w, x + m_radius);
    int maskWidth = 2 * m_radius + 1;

    for (int r = rowStart; r <= rowEnd; ++r) {
        for (int c = colStart; c <= colEnd; ++c) {
            // canvas index
            int ci = w * r + c;
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
    // @TODO: [BRUSH] Here, you'll need to paste down the paint you picked up in
    //        the previous method. Be sure to take the mask into account! You can
    //        ignore the alpha parameter, but you can also use it (smartly) if you
    //        would like to.
    if (m_smudged) {
        BGRA* pix = canvas->data();

        int w = canvas->width();
        int rowStart = std::max(0, mouse_y - m_radius);
        int rowEnd = std::min(canvas->height(), mouse_y + m_radius);
        int colStart = std::max(0, mouse_x - m_radius);
        int colEnd = std::min(w, mouse_x + m_radius);
        int maskWidth = 2 * m_radius + 1;

        for (int r = rowStart; r <= rowEnd; ++r) {
            for (int c = colStart; c <= colEnd; ++c) {
                // canvas index
                int ci = w * r + c;
                // mask index
                int mi = maskWidth * (r - mouse_y + m_radius) + (c - mouse_x + m_radius);
                float p = m_mask[mi];

                // debug
                if (p == 0.25f) {
                    printf("display: %d, finger: %d\n", pix[ci].r, m_finger[mi].r);
                    printf("display: %f, finger: %f", pix[ci].r * (1 - p), m_finger[mi].r * p);
                    cout << endl;
                }

                pix[ci].r = pix[ci].r * (1.f - p) + (m_finger[mi].r * p);
                pix[ci].g = pix[ci].g * (1.f - p) + (m_finger[mi].g * p);
                pix[ci].b = pix[ci].b * (1.f - p) + (m_finger[mi].b * p);
                pix[ci].a = 255;

            }
        }
    }

    // now pick up paint again...
    pickUpPaint(mouse_x, mouse_y, canvas);
    m_smudged = 1;
}


