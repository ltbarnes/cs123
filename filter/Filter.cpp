#include "Filter.h"
#include "Canvas2D.h"

using namespace std;

Filter::Filter()
{
    m_filter_canvas = NULL;
}


Filter::~Filter()
{
    if (m_filter_canvas) {
        delete[] m_filter_canvas;
        m_filter_canvas = NULL;
    }
}


void Filter::setBounds(Canvas2D *canvas)
{
    int width = canvas->width();
    int height = canvas->height();

    QPoint start = canvas->marqueeStart();
    QPoint stop = canvas->marqueeStop();

    if (start.x() == stop.x() && start.y() == stop.y()) {
        m_l = 0; m_r = width; m_t = 0; m_b = height;
    } else {
        m_l = std::max(0, std::min(start.x(), stop.x()));
        m_r = std::min(width, std::max(start.x(), stop.x()));
        m_t = std::max(0, std::min(start.y(), stop.y()));
        m_b = std::min(height, std::max(start.y(), stop.y()));
    }
}


void Filter::makeFilterCanvas(Canvas2D *canvas, int padding, bool black)
{
    int width = canvas->width();

    int fw = m_r - m_l;
    int padw = fw + 2 * padding;
    int fh = m_b - m_t;
    int padh = fh + 2 * padding;

    // if m_filter_canvas is already in use delete it
    if (m_filter_canvas) {
        delete[] m_filter_canvas;
        m_filter_canvas = NULL;
    }

    BGRA* pix = canvas->data();
    int size = padw * padh;
    m_filter_canvas = new BGRA[size];

    BGRA blk = m_filter_canvas[0];
    blk.r = 0; blk.g = 0; blk.b = 0;
    blk.a = 255;

    int ci, fi;
    BGRA *start;
    for (int row = m_t; row < m_b; row++) {
        ci = width * row + m_l;
        fi = (row + padding - m_t) * padw;

        if (padding <= fw) {
            for (int p = 0; p < padding; p++) {
                if (black) {
                    m_filter_canvas[fi + p] = blk;
                    m_filter_canvas[fi + padding + fw + p] = blk;
                } else {
                    m_filter_canvas[fi + p] = pix[ci + padding - p - 1];
                    m_filter_canvas[fi + padding + fw + p] =
                            pix[ci + fw - p - 1];
                }
            }
        } else {
            // TODO: SET PADDING TO ONE COLOR!!!
        }

        start = pix + ci;
        std::copy(start, start + fw, m_filter_canvas + fi + padding);
    }
    BGRA *blkarray;
    if (black) {
        blkarray = new BGRA[padw];
        for (int i = 0; i < padw; i++)
            blkarray[i] = blk;
    }

    for (int p = 0; p < padding; p++) {
        fi = (padding + p) * padw;

        start = m_filter_canvas + fi;
        if (black)
            std::copy(blkarray, blkarray + padw, start - padw * (2*p + 1));
        else
            std::copy(start, start + padw, start - padw * (2*p + 1));

        fi = (padding + fh - 1 - p) * padw;

        start = m_filter_canvas + fi;
        if (black)
            std::copy(blkarray, blkarray + padw, start + padw * (2*p + 1));
        else
            std::copy(start, start + padw, start + padw * (2*p + 1));
    }

    if (black)
        delete[] blkarray;

    for (int row = m_t; row < m_b; row++) {
        for (int col = m_l; col < m_r; col++) {
            ci = width * row + col;
            fi = (row + padding - m_t) * padw + (col + padding - m_l);

            assert(pix[ci].r == m_filter_canvas[fi].r);
            assert(pix[ci].g == m_filter_canvas[fi].g);
            assert(pix[ci].b == m_filter_canvas[fi].b);
        }
    }
}


void Filter::filter1DTwice(Canvas2D *canvas, int radius, float *horizf, float *vertf)
{

}


void Filter::grayscale(Canvas2D *canvas)
{
    BGRA *pix = canvas->data();

    int width = canvas->width();

    int i;
    for (int row = m_t; row < m_b; row++) {
        for (int col = m_l; col < m_r; col++) {
            i = width * row + col;

            unsigned char gray = (unsigned char) (pix[i].r * .299f +
                                                  pix[i].g * .587f +
                                                  pix[i].b * .114f);
            pix[i].r = gray;
            pix[i].g = gray;
            pix[i].b = gray;
        }
    }
}


void Filter::printPoint(QPoint p)
{
    cout << "(" << p.x();
    cout << ", " << p.y();
    cout << ")" << endl;
}


void Filter::printPoint3(glm::vec3 p)
{
    cout << "(" << p.x;
    cout << ", " << p.y;
    cout << ", " << p.z;
    cout << ")" << endl;
}
