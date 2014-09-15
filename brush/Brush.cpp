/**
 * @file   Brush.cpp
 *
 * Implementation of common functionality of bitmap brushes.
 *
 * You should fill this file in while scompleting the Brush assignment.
 */

#include <cmath>
#include "Brush.h"
#include "Canvas2D.h"

unsigned char lerp(unsigned char a, unsigned char b, float percent)
{
    float fa = (float)a / 255.0f;
    float fb = (float)b / 255.0f;
    return (unsigned char)((fa + (fb - fa) * percent) * 255.0f + 0.5f);
}


Brush::Brush(BGRA color, int flow, int radius) {

    m_color = color;
    m_flow = flow;
    m_radius = radius;
    int width = 2 * radius + 1;
    m_mask = new float[width * width];
    m_drawLayer = NULL;
}


Brush::~Brush()
{
    delete[] m_mask;
    if (m_drawLayer)
        delete[] m_drawLayer;
}


void Brush::setGreen(int green)
{
    m_color.g = green;
}


void Brush::setRed(int red)
{
    m_color.r = red;
}


void Brush::setBlue(int blue)
{
    m_color.b = blue;
}


void Brush::setFlow(int flow)
{
    m_flow = flow;
    makeMask();
}


void Brush::setRadius(int radius)
{
    m_radius = radius;
    makeMask();
}


void Brush:: setDrawLayer(Canvas2D *canvas)
{
    // if m_drawLayer is already in use delete it
    if (m_drawLayer) {
        delete[] m_drawLayer;
        m_drawLayer = NULL;
    }
    // copy the canvas to m_drawLayer
    if (canvas) {
        BGRA* pix = canvas->data();
        int size = canvas->width() * canvas->height();
        m_drawLayer = new BGRA[size];
        std::copy(pix, pix+size, m_drawLayer);
    }
}


void Brush::paintOnce(int mouse_x, int mouse_y, Canvas2D* canvas)
{
    // get the canvas array
    BGRA* pix = canvas->data();

    // set the starting and ending points for drawing
    int w = canvas->width();
    int rowStart = std::max(0, mouse_y - m_radius);
    int rowEnd = std::min(canvas->height() - 1, mouse_y + m_radius);
    int colStart = std::max(0, mouse_x - m_radius);
    int colEnd = std::min(w - 1, mouse_x + m_radius);
    int maskWidth = 2 * m_radius + 1;

    // iterate through the brush area and modify the canvas accordingly
    for (int r = rowStart; r <= rowEnd; ++r) {
        for (int c = colStart; c <= colEnd; ++c) {

            // canvas index
            int ci = w * r + c;
            // mask index
            int mi = maskWidth * (r - mouse_y + m_radius) + (c - mouse_x + m_radius);

            // alpha value * mask value
            float p = (m_flow / 255.f) * m_mask[mi];

            if (m_drawLayer) { // get values from the copy to avoid alpha blending
                if (p) { // only draw if p < 0
                    pix[ci].r = (unsigned char) (m_drawLayer[ci].r * (1.f - p) + (m_color.r * p) + 0.5f);
                    pix[ci].g = (unsigned char) (m_drawLayer[ci].g * (1.f - p) + (m_color.g * p) + 0.5f);
                    pix[ci].b = (unsigned char) (m_drawLayer[ci].b * (1.f - p) + (m_color.b * p) + 0.5f);
                    pix[ci].a = 255;
                }
            } else { // alpha blending
                pix[ci].r = (unsigned char) (pix[ci].r * (1.f - p) + (m_color.r * p) + 0.5f);
                pix[ci].g = (unsigned char) (pix[ci].g * (1.f - p) + (m_color.g * p) + 0.5f);
                pix[ci].b = (unsigned char) (pix[ci].b * (1.f - p) + (m_color.b * p) + 0.5f);
                pix[ci].a = 255;
            }
        }
    }

}

void Brush::update()
{

}

void Brush::renderAnimation(Canvas2D *)
{

}


