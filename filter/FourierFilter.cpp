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
    glm::vec3 *realz = new glm::vec3[size];
    glm::vec3 *imagz = new glm::vec3[size];
//    glm::vec2 *comp = new glm::vec2[size];

    double theta, cosine, sine;
    int ci, fi;

    for (int v = 0; v < h; v++) {
        for (int u = 0; u < w; u++) {
            ci = v * w + u;

            realz[ci].x = 0.0;
            realz[ci].y = 0.0;
            realz[ci].z = 0.0;
            imagz[ci].x = 0.0;
            imagz[ci].y = 0.0;
            imagz[ci].z = 0.0;
            for (int y = 0; y < h; y++) {
                for (int x = 0; x < w; x++) {
                    fi = y * w + x;
                    theta = -2.0 * M_PI * (((u * x * 1.0) / w) + ((v * y * 1.0) / h));
                    cosine = cos(theta) / sqrt(size);
                    sine = sin(theta) / sqrt(size);
                    realz[ci].x += (pix[fi].r / 255.0) * cosine;
                    imagz[ci].x += (pix[fi].r / 255.0) * sine;
                    realz[ci].y += (pix[fi].g / 255.0) * cosine;
                    imagz[ci].y += (pix[fi].g / 255.0) * sine;
                    realz[ci].z += (pix[fi].b / 255.0) * cosine;
                    imagz[ci].z += (pix[fi].b / 255.0) * sine;
                }
            }
        }
    }
    double valr, valg, valb;
    int r2, c2;
    for (int r = 0; r < h; r++) {
        for (int c = 0; c < w; c++) {
            ci = r * w + c;
            r < h / 2 ? r2 = r + h / 2 : r2 = r - h / 2;
            c < w / 2 ? c2 = c + w / 2 : c2 = c - w / 2;
            fi = r2 * w + c2;

            valr = log(glm::distance(glm::vec2(0.0, 0.0), glm::vec2(realz[fi].x, imagz[fi].x)) * 1.0 + 1);
            valg = log(glm::distance(glm::vec2(0.0, 0.0), glm::vec2(realz[fi].y, imagz[fi].y)) * 1.0 + 1);
            valb = log(glm::distance(glm::vec2(0.0, 0.0), glm::vec2(realz[fi].z, imagz[fi].z)) * 1.0 + 1);
//            cout << val << ", ";
            pix[ci].r = (unsigned char) floor(valr * 255.0 + 0.5f);
            pix[ci].g = (unsigned char) floor(valg * 255.0 + 0.5f);
            pix[ci].b = (unsigned char) floor(valb * 255.0 + 0.5f);
        }
//        cout << endl;
    }

    delete[] realz;
    delete[] imagz;

    canvas->repaint();
}
