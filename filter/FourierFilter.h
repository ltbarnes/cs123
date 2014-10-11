#ifndef FOURIERFILTER_H
#define FOURIERFILTER_H

#include <complex>
#include "Filter.h"

class FourierFilter : public Filter
{
public:
    FourierFilter();
    virtual ~FourierFilter();

    virtual void filter(Canvas2D *canvas);
    void calcInverse(bool inverse);

protected:
    // discrete Fourier transform
    void dft(BGRA *pix, std::complex<double> *F, std::complex<double> *Fg, std::complex<double> *Fb, int w, int h);

    // inverse discrete Fourier transform
    void idft(std::complex<double> *Fr, std::complex<double> *Fg, std::complex<double> *Fb, int w, int h);
    
    // display the magnitued of the complex values on the canvas on a log10 scale
    void trans2Canvas(BGRA *pix, std::complex<double> *Fr, std::complex<double> *Fg, std::complex<double> *Fb, int w, int h);
    
    // display the magnitude of the complex values on the canvas
    void img2Canvas(BGRA *pix, std::complex<double> *Fr, std::complex<double> *Fg, std::complex<double> *Fb, int w, int h);
    
    // filter the results of the transform
    void applyThreshold(std::complex<double> *Fr, std::complex<double> *Fg, std::complex<double> *Fb, int w, int h, float min, float max);


private:
    std::complex<double> *m_Fr;
    std::complex<double> *m_Fg;
    std::complex<double> *m_Fb;

    bool m_inverse;
    bool m_hasInverse;
};

#endif // FOURIERFILTER_H
