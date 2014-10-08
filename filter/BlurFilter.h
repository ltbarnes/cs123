#ifndef BLURFILTER_H
#define BLURFILTER_H

#include "Filter.h"

class BlurFilter : public Filter
{
public:
    BlurFilter(int radius);
    virtual ~BlurFilter();

    virtual void filter(Canvas2D *canvas);
    virtual void setRadius(int radius);

private:
    void buildFilter();

    int m_radius;
    float *m_f;

};

#endif // BLURFILTER_H
