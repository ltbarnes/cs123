#include "InvertFilter.h"
#include "Canvas2D.h"

InvertFilter::InvertFilter()
    : Filter()
{
}


InvertFilter::~InvertFilter()
{
}


void InvertFilter::filter(Canvas2D *canvas)
{
    setBounds(canvas);

    BGRA *pix = canvas->data();

    int width = canvas->width();

    int i;
    for (int row = m_t; row < m_b; row++) {
        for (int col = m_l; col < m_r; col++) {
            i = width * row + col;

            pix[i].r = (unsigned char) (255 - pix[i].r);
            pix[i].g = (unsigned char) (255 - pix[i].g);
            pix[i].b = (unsigned char) (255 - pix[i].b);
        }
    }
    canvas->repaint();
}
