#include "BlurFilter.h"
#include "Canvas2D.h"

BlurFilter::BlurFilter(int radius) :
    Filter()
{
    m_f = NULL;
    setRadius(radius);
}


BlurFilter::~BlurFilter()
{
    if (m_f)
        delete[] m_f;
}


void BlurFilter::filter(Canvas2D *canvas)
{
    setBounds(canvas);
    makeFilterCanvas(canvas, m_radius, false);

    BGRA *pix = canvas->data();
    int width = canvas->width();

    int padw = (m_r - m_l) + 2 * m_radius;
    int padh = (m_b - m_t) + 2 * m_radius;

    int nwidth = padw - (m_radius * 2);
    int size = nwidth * padh * 3;
    float *nums = new float[size];

    BGRA pixel;
    int ci, fi;
    float r, g, b;
    for (int row = 0; row < padh; row++) {
        for (int col = m_radius; col < padw - m_radius; col++) {
            fi = row * padw + col;
            ci = 3 * (row * nwidth + (col - m_radius));

            r = 0; g = 0; b = 0;
            for (int i = 0; i < m_radius * 2; i++) {
                pixel = m_filter_canvas[fi - m_radius + i];
                r += (pixel.r / 255.f) * m_f[i];
                g += (pixel.g / 255.f) * m_f[i];
                b += (pixel.b / 255.f) * m_f[i];
            }
            nums[ci] = r;
            nums[ci+1] = g;
            nums[ci+2] = b;
        }
    }

    int offset;
    for (int row = m_t; row < m_b; row++) {
        for (int col = m_l; col < m_r; col++) {
            ci = width * row + col;
            fi = 3*((row - m_t + m_radius) * nwidth + (col - m_l));
            offset = 3*nwidth;

            r = 0; g = 0; b = 0;
            for (int i = 0; i < m_radius * 2; i++) {
                r += nums[fi - (m_radius - i)*offset] * m_f[i];
                g += nums[fi - (m_radius - i)*offset + 1] * m_f[i];
                b += nums[fi - (m_radius - i)*offset + 2] * m_f[i];
            }
            pix[ci].r = (unsigned char)(r * 255.f);
            pix[ci].g = (unsigned char)(g * 255.f);
            pix[ci].b = (unsigned char)(b * 255.f);

        }
    }
    delete[] nums;
    canvas->repaint();
}


void BlurFilter::setRadius(int radius)
{
    m_radius = radius + 1;
    buildFilter();
}


void BlurFilter::buildFilter()
{
    if (m_f)
        delete[] m_f;

    int size = m_radius * 2 + 1;
    m_f = new float[size];

    if (m_radius > 5) {
        // gaussian
        float sigma = m_radius / 6.f;
        float coeff = 1.f / (sqrt(2.f * M_PI) * sigma);
        float denom = -2.f * sigma * sigma;

        for (int x = -m_radius; x <= m_radius; x++) {

            m_f[x + m_radius] = coeff * exp((x * x) / denom);
        }
    } else {
        // triangle
        float b = 1.f / (m_radius);
        float m = b / -(m_radius);
        for (int x = -m_radius; x <= m_radius; x++) {
            m_f[x + m_radius] = m*abs(x) + b;
        }
    }

}
