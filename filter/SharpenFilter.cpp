#include "SharpenFilter.h"
#include "Canvas2D.h"

SharpenFilter::SharpenFilter()
{
}


SharpenFilter::~SharpenFilter()
{
}

void SharpenFilter::filter(Canvas2D *canvas)
{
    int w = canvas->width();
    int h = canvas->height();
    int size = w * h;

    m_Fr = new std::complex<double>[size];
    m_Fg = new std::complex<double>[size];
    m_Fb = new std::complex<double>[size];

    BGRA *pix = canvas->data();
    dft(pix, m_Fr, m_Fg, m_Fb, w, h);
    applyThreshold(m_Fr, m_Fg, m_Fb, w, h, 0.05, std::numeric_limits<double>::infinity());
    idft(m_Fr, m_Fg, m_Fb, w, h);
    img2Canvas(pix, m_Fr, m_Fg, m_Fb, w, h);

}
