#ifndef FILTER_H
#define FILTER_H

class Canvas2D;

class Filter
{
public:
    Filter();
    virtual ~Filter();

    virtual void filter(Canvas2D *canvas);
};

#endif // FILTER_H
