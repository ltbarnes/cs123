#include "ScaleFilter.h"
#include "Canvas2D.h"

ScaleFilter::ScaleFilter(float xScale, float yScale)
    : Filter()
{
    setXScale(xScale);
    setYScale(yScale);
}


ScaleFilter::~ScaleFilter()
{
}


void ScaleFilter::setXScale(float xScale)
{
    m_xscale = xScale;
    if (m_xscale < 1)
        m_width = 1.f / m_xscale;
    else
        m_width = 1.f;
}


void ScaleFilter::setYScale(float yScale)
{
    m_yscale = yScale;
    if ((m_yscale) < 1)
        m_height = 1.f / m_yscale;
    else
        m_height = 1.f;
}


void ScaleFilter::filter(Canvas2D *canvas)
{
    int pad = (int)(std::max(m_width, m_height) + 0.5f);

    setBounds(canvas);
    makeFilterCanvas(canvas, pad, false);

    int width = (m_r - m_l) + 2*pad;
    int height = (m_b - m_t) + 2*pad;

    int w = (int)((m_r - m_l) * m_xscale + 0.5f);

    int size = w * height;
    BGRA *temp = new BGRA[size];

    dostuff(width, w, height, m_xscale, true, pad, m_filter_canvas, temp);

    int h = (int)((m_b - m_t) * m_yscale + 0.5f);

    canvas->resize(w, h);
    BGRA *pix = canvas->data();

    dostuff(w, w, h, m_yscale, false, pad, temp, pix);

    delete[] temp;
}


void ScaleFilter::dostuff(int srcW, int destW, int destH, float a, bool x, int pad, BGRA *src, BGRA *dest)
{
    int ci, fi;
    for (int r = 0; r < destH; r++) {
        for (int c = 0; c < destW; c++) {
            ci = r * destW + c;

            float center = F(c * x + r * !x, a) + pad;
            int pix1 = (int)(center - m_width + 1);
            int pix2 = (int)(center + m_width);

            int red = 0; int green = 0; int blue = 0;
            float gee;
            float totes = 0;
            for (int p = pix1; p <= pix2; p++) {
                fi = (r * x + p * !x) * srcW + (c * !x + p * x);
                gee = G(p - center);
                red += gee * src[fi].r;
                green += gee * src[fi].g;
                blue += gee * src[fi].b;
                totes += gee;
            }

            dest[ci].r = (unsigned char)(red / totes);
            dest[ci].g = (unsigned char)(green / totes);
            dest[ci].b = (unsigned char)(blue / totes);
        }
    }
}


float ScaleFilter::F(int x, float a)
{
    return (x / a) + (1 - a) / (2 * a);
}


float ScaleFilter::G(float x)
{
    return (1 - fabs(x) / m_width) / m_width;
}
