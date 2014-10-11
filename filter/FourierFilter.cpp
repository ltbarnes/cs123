#include "FourierFilter.h"
#include "Canvas2D.h"

FourierFilter::FourierFilter()
    : Filter()
{ 
    m_Fr = NULL;
    m_Fg = NULL;
    m_Fb = NULL;

    m_inverse = false;
    m_hasInverse = false;
}


FourierFilter::~FourierFilter()
{
    if (m_Fr) {
        delete[] m_Fr;
        delete[] m_Fg;
        delete[] m_Fb;
    }
}


void FourierFilter::calcInverse(bool inverse)
{
    m_inverse = inverse;
}


void FourierFilter::filter(Canvas2D *canvas)
{
    int w = canvas->width();
    int h = canvas->height();
    int size = w * h;

    if (!m_hasInverse) {
        if (m_inverse) {
            cout << "Fourier transform hasn't been performed yet." << endl;
            return;
        } else {
            m_Fr = new std::complex<double>[size];
            m_Fg = new std::complex<double>[size];
            m_Fb = new std::complex<double>[size];
        }
    }

    BGRA *pix = canvas->data();

    if (m_inverse) {
        idft(m_Fr, m_Fg, m_Fb, w, h);
        img2Canvas(pix, m_Fr, m_Fg, m_Fb, w, h);
    } else {
        dft(pix, m_Fr, m_Fg, m_Fb, w, h);
        m_hasInverse = true;
        trans2Canvas(pix, m_Fr, m_Fg, m_Fb, w, h);
    }
    canvas->repaint();

}


void FourierFilter::dft(BGRA *pix, std::complex<double> *Fr, std::complex<double> *Fg, std::complex<double> *Fb, int w, int h)
{
    int size = w * h;
    std::complex<double> *Pr = new std::complex<double>[size];
    std::complex<double> *Pg = new std::complex<double>[size];
    std::complex<double> *Pb = new std::complex<double>[size];

    double part, theta, cosine, sine;
    double realr, realg, realb, imagr, imagg, imagb;
    int ci, fi;

    double wsqrt = sqrt(w);
    double hsqrt = sqrt(h);

    for (int b = 0; b < h; b++) {
        for (int k = 0; k < w; k++) {
            ci = b * w + k;

            realr = 0.0;
            imagr = 0.0;
            realg = 0.0;
            imagg = 0.0;
            realb = 0.0;
            imagb = 0.0;
            part = (-2.0 * M_PI * k) / w;
            for (int a = 0; a < w; a++) {
                fi = b * w + a;

                theta = part * a;
                cosine = cos(theta);
                sine = sin(theta);

                realr += (pix[fi].r / 255.0) * cosine;
                imagr += (pix[fi].r / 255.0) * sine;
                realg += (pix[fi].g / 255.0) * cosine;
                imagg += (pix[fi].g / 255.0) * sine;
                realb += (pix[fi].b / 255.0) * cosine;
                imagb += (pix[fi].b / 255.0) * sine;
            }
            Pr[ci] = std::complex<double>(realr / wsqrt, imagr / wsqrt);
            Pg[ci] = std::complex<double>(realg / wsqrt, imagg / wsqrt);
            Pb[ci] = std::complex<double>(realb / wsqrt, imagb / wsqrt);
        }
    }

    double rr, rg, rb, ir, ig, ib;
    for (int l = 0; l < h; l++) {
        for (int k = 0; k < w; k++) {
            ci = l * w + k;

            realr = 0.0;
            imagr = 0.0;
            realg = 0.0;
            imagg = 0.0;
            realb = 0.0;
            imagb = 0.0;
            part = (-2.0 * M_PI * l) / h;
            for (int b = 0; b < h; b++) {
                fi = b * w + k;

                theta = part * b;

                cosine = cos(theta); // c
                sine = sin(theta);   // d
                rr = Pr[fi].real();  // a
                ir = Pr[fi].imag();  // b
                rg = Pg[fi].real();
                ig = Pg[fi].imag();
                rb = Pb[fi].real();
                ib = Pb[fi].imag();

                realr += rr * cosine - ir * sine;
                imagr += rr * sine + ir * cosine;
                realg += rg * cosine - ig * sine;
                imagg += rg * sine + ig * cosine;
                realb += rb * cosine - ib * sine;
                imagb += rb * sine + ib * cosine;
            }
            Fr[ci] = std::complex<double>(realr / hsqrt, imagr / hsqrt);
            Fg[ci] = std::complex<double>(realg / hsqrt, imagg / hsqrt);
            Fb[ci] = std::complex<double>(realb / hsqrt, imagb / hsqrt);
        }
    }
    delete[] Pr;
    delete[] Pg;
    delete[] Pb;
}


void FourierFilter::idft(std::complex<double> *Fr, std::complex<double> *Fg, std::complex<double> *Fb, int w, int h)
{
    int size = w * h;
    std::complex<double> *Pr = new std::complex<double>[size];
    std::complex<double> *Pg = new std::complex<double>[size];
    std::complex<double> *Pb = new std::complex<double>[size];

    double part, theta, cosine, sine;
    double realr, realg, realb, imagr, imagg, imagb;
    double rr, rg, rb, ir, ig, ib;
    int ci, fi;

    double wsqrt = sqrt(w);
    double hsqrt = sqrt(h);

    for (int b = 0; b < h; b++) {
        for (int k = 0; k < w; k++) {
            ci = b * w + k;

            realr = 0.0;
            imagr = 0.0;
            realg = 0.0;
            imagg = 0.0;
            realb = 0.0;
            imagb = 0.0;
            part = (2.0 * M_PI * k) / w;
            for (int a = 0; a < w; a++) {
                fi = b * w + a;

                theta = part * a;

                cosine = cos(theta); // c
                sine = sin(theta);   // d
                rr = Fr[fi].real();  // a
                ir = Fr[fi].imag();  // b
                rg = Fg[fi].real();
                ig = Fg[fi].imag();
                rb = Fb[fi].real();
                ib = Fb[fi].imag();

                realr += rr * cosine - ir * sine;
                imagr += rr * sine + ir * cosine;
                realg += rg * cosine - ig * sine;
                imagg += rg * sine + ig * cosine;
                realb += rb * cosine - ib * sine;
                imagb += rb * sine + ib * cosine;
            }
            Pr[ci] = std::complex<double>(realr / wsqrt, imagr / wsqrt);
            Pg[ci] = std::complex<double>(realg / wsqrt, imagg / wsqrt);
            Pb[ci] = std::complex<double>(realb / wsqrt, imagb / wsqrt);
        }
    }

    for (int l = 0; l < h; l++) {
        for (int k = 0; k < w; k++) {
            ci = l * w + k;

            realr = 0.0;
            imagr = 0.0;
            realg = 0.0;
            imagg = 0.0;
            realb = 0.0;
            imagb = 0.0;
            part = (2.0 * M_PI * l) / h;
            for (int b = 0; b < h; b++) {
                fi = b * w + k;

                theta = part * b;

                cosine = cos(theta); // c
                sine = sin(theta);   // d
                rr = Pr[fi].real();  // a
                ir = Pr[fi].imag();  // b
                rg = Pg[fi].real();
                ig = Pg[fi].imag();
                rb = Pb[fi].real();
                ib = Pb[fi].imag();

                realr += rr * cosine - ir * sine;
                imagr += rr * sine + ir * cosine;
                realg += rg * cosine - ig * sine;
                imagg += rg * sine + ig * cosine;
                realb += rb * cosine - ib * sine;
                imagb += rb * sine + ib * cosine;
            }
            Fr[ci] = std::complex<double>(realr / hsqrt, imagr / hsqrt);
            Fg[ci] = std::complex<double>(realg / hsqrt, imagg / hsqrt);
            Fb[ci] = std::complex<double>(realb / hsqrt, imagb / hsqrt);
        }
    }
    delete[] Pr;
    delete[] Pg;
    delete[] Pb;
}

void FourierFilter::trans2Canvas(BGRA *pix, std::complex<double> *Fr, std::complex<double> *Fg, std::complex<double> *Fb, int w, int h)
{

    double wsqrt = sqrt(w);
    double hsqrt = sqrt(h);

    double valr, valg, valb;
    int r2, c2, ci, fi;

    for (int r = 0; r < h; r++) {
        for (int c = 0; c < w; c++) {
            ci = r * w + c;
            r < h / 2 ? r2 = r + h / 2 : r2 = r - h / 2;
            c < w / 2 ? c2 = c + w / 2 : c2 = c - w / 2;
            fi = r2 * w + c2;

            valr = std::abs(Fr[fi] * hsqrt * wsqrt);
            valg = std::abs(Fg[fi] * hsqrt * wsqrt);
            valb = std::abs(Fb[fi] * hsqrt * wsqrt);
            
            valr = log10(valr + 1.0);
            valg = log10(valg + 1.0);
            valb = log10(valb + 1.0);
            
            if (valr > 1) valr = 1.0;
            else if (valr < 0) valr = 0.0;
            if (valg > 1) valg = 1.0;
            else if (valg < 0) valg = 0.0;
            if (valb > 1) valb = 1.0;
            else if (valb < 0) valb = 0.0;

            pix[ci].r = (unsigned char) floor(valr * 255.0 + 0.5f);
            pix[ci].g = (unsigned char) floor(valg * 255.0 + 0.5f);
            pix[ci].b = (unsigned char) floor(valb * 255.0 + 0.5f);
        }
    }
}


void FourierFilter::img2Canvas(BGRA *pix, std::complex<double> *Fr, std::complex<double> *Fg, std::complex<double> *Fb, int w, int h)
{
    double valr, valg, valb;
    int i;
    for (int r = 0; r < h; r++) {
        for (int c = 0; c < w; c++) {
            i = r * w + c;

            valr = std::abs(Fr[i]);
            valg = std::abs(Fg[i]);
            valb = std::abs(Fb[i]);

            if (valr > 1) valr = 1.0;
            else if (valr < 0) valr = 0.0;
            if (valg > 1) valg = 1.0;
            else if (valg < 0) valg = 0.0;
            if (valb > 1) valb = 1.0;
            else if (valb < 0) valb = 0.0;

            pix[i].r = (unsigned char) floor(valr * 255.0 + 0.5f);
            pix[i].g = (unsigned char) floor(valg * 255.0 + 0.5f);
            pix[i].b = (unsigned char) floor(valb * 255.0 + 0.5f);

        }
    }
}


void FourierFilter::applyThreshold(std::complex<double> *Fr, std::complex<double> *Fg, std::complex<double> *Fb, int w, int h, float min, float max)
{
    double valr, valg, valb;
    int i;
    for (int r = 0; r < h; r++) {
        for (int c = 0; c < w; c++) {
            i = r * w + c;

            valr = std::abs(Fr[i]);
            valg = std::abs(Fg[i]);
            valb = std::abs(Fb[i]);

            if (valr < min || valr > max) {
                Fr[i] *= 0.0;
            }
            if (valg < min || valg > max) {
                Fg[i] *= 0.0;
            }
            if (valb < min || valb > max) {
                Fb[i] *= 0.0;
            }
        }
    }
}
