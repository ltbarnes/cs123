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
    setBounds(canvas);
    makeFilterCanvas(canvas, 0, false);
    BGRA *pix = m_filter_canvas;

    int w = m_r - m_l;
    int h = m_b - m_t;
    int size = w * h;

    std::complex<double> *Fr = new std::complex<double>[size];
    std::complex<double> *Fg = new std::complex<double>[size];
    std::complex<double> *Fb = new std::complex<double>[size];

    dft(pix, Fr, Fg, Fb, w, h);
    applyThreshold(Fr, Fg, Fb, w, h, 0.05, std::numeric_limits<double>::infinity());
    idft(Fr, Fg, Fb, w, h);
    img2Canvas(canvas->data(), Fr, Fg, Fb, w, h);

    canvas->repaint();

    delete[] Fr;
    delete[] Fg;
    delete[] Fb;

}
