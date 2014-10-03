#ifndef FILTER_H
#define FILTER_H

#include <CS123Common.h>

class Canvas2D;

class Filter
{
public:
    Filter();
    virtual ~Filter();

    virtual void filter(Canvas2D *canvas) = 0;
    void printPoint(QPoint p);

protected:
    virtual void setBounds(Canvas2D *canvas);
    void makeFilterCanvas(Canvas2D *canvas, int padding);
    void grayscale(Canvas2D *canvas);

    BGRA *m_filter_canvas;
    int m_t, m_l, m_b, m_r;

};

#endif // FILTER_H
