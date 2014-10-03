#include "EdgeFilter.h"
#include "Canvas2D.h"

EdgeFilter::EdgeFilter(float threshold)
    : Filter()
{
    setThreshold(threshold);
}


EdgeFilter::~EdgeFilter()
{
}


void EdgeFilter::setThreshold(float threshold)
{
    m_threshold = threshold;
}


void EdgeFilter::filter(Canvas2D *canvas)
{
    int pad = 1;

    setBounds(canvas);
    grayscale(canvas);
    makeFilterCanvas(canvas, pad);


    BGRA *pix = canvas->data();
    int width = canvas->width();

    int padw = (m_r - m_l) + 2 * pad;
    int padh = (m_b - m_t) + 2 * pad;

    int nwidth = padw-2;
    int size = nwidth * padh * 2;
    float *nums = new float[size];

    float sobel1[3] = {-1.f, 0.f, 1.f};
    float sobel2[3] = {1.f, 2.f, 1.f};

    int ci, fi;
    for (int row = 0; row < padh; row++) {
        for (int col = 1; col < padw - 1; col++) {
            fi = (row) * padw + (col);
            ci = 2 * (row * nwidth + (col - 1));

            nums[ci] = sobel1[0] * (m_filter_canvas[fi-1].b / 255.f) +
                             sobel1[1] * (m_filter_canvas[fi].b / 255.f) +
                             sobel1[2] * (m_filter_canvas[fi+1].b / 255.f);
            nums[ci+1] = sobel2[0] * (m_filter_canvas[fi-1].b / 255.f) +
                             sobel2[1] * (m_filter_canvas[fi].b / 255.f) +
                             sobel2[2] * (m_filter_canvas[fi+1].b / 255.f);
        }
    }

    int offset;
    for (int row = m_t; row < m_b; row++) {
        for (int col = m_l; col < m_r; col++) {
            ci = width * row + col;
            fi = 2*((row - m_t + pad) * nwidth + (col - m_l));
            offset = 2*(padw-2);

            float x = sobel2[0] * nums[fi-offset] +
                      sobel2[1] * nums[fi] +
                      sobel2[2] * nums[fi+offset];
            float y = sobel1[2] * nums[fi-offset+1] +
                      sobel1[1] * nums[fi+1] +
                      sobel1[0] * nums[fi+offset+1];

            float temp = sqrt(x*x + y*y) * m_threshold;

            pix[ci].b = (unsigned char)(std::min(1.f, std::max(temp, 0.f)) * 255.f);
            pix[ci].r = pix[ci].b;
            pix[ci].g = pix[ci].b;

        }
    }
    delete[] nums;
    canvas->repaint();
}
