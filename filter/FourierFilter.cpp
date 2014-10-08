#include "FourierFilter.h"
#include "Canvas2D.h"

FourierFilter::FourierFilter()
{
}


FourierFilter::~FourierFilter()
{
}


void FourierFilter::filter(Canvas2D *canvas)
{
    setBounds(canvas);
//    makeFilterCanvas(canvas, 0, false);
    BGRA *pix = canvas->data();

    int w = canvas->width();
    int h = canvas->height();
    int size = w * h;
    BGRA *realz = new BGRA[size];
    BGRA *imagz = new BGRA[size];
//    glm::vec2 *comp = new glm::vec2[size];

    double real, imag, theta, sign, cosine, sine;
    int ci, fi;

    for (int v = 0; v < h; v++) {
        for (int u = 0; u < w; u++) {
            ci = v * w + u;

            real = 0.0;
            imag = 0.0;
            for (int y = 0; y < h; y++) {
                for (int x = 0; x < w; x++) {
                    fi = y * w + x;
                    theta = -2.0 * M_PI * (((u * x * 1.0) / w) + ((v * y * 1.0) / h));
                    cosine = cos(theta) / sqrt(size);
                    sine = sin(theta) / sqrt(size);
                    realz[ci].r += (pix[fi].r / 255.0) * cosine;
                    imagz[ci].r += (pix[fi].r / 255.0) * sine;
                    realz[ci].g += (pix[fi].g / 255.0) * cosine;
                    imagz[ci].g += (pix[fi].g / 255.0) * sine;
                    realz[ci].b += (pix[fi].b / 255.0) * cosine;
                    imagz[ci].b += (pix[fi].b / 255.0) * sine;
                }
            }
        }
    }
    double val;
    int r2, c2;
    for (int r = 0; r < h; r++) {
        for (int c = 0; c < w; c++) {
            ci = r * w + c;
            r < h / 2 ? r2 = r + h / 2 : r2 = r - h / 2;
            c < w / 2 ? c2 = c + w / 2 : c2 = c - w / 2;
            fi = r2 * w + c2;

            sign = pow(-1.0, r + c * 1.0);
            val = log(glm::distance(glm::vec2(0.0, 0.0), comp[fi]) * 1.0 + 1);
//            if (val < 0)
//                val = 0;
//            cout << val << ", ";
            pix[ci].r = (unsigned char) floor(val * 255.0 + 0.5f);
            pix[ci].g = (unsigned char) floor(val * 255.0 + 0.5f);
            pix[ci].b = (unsigned char) floor(val * 255.0 + 0.5f);
        }
//        cout << endl;
    }
    cout << ci << ", " << size << endl;

    canvas->repaint();
}
