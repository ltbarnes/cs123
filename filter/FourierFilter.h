#ifndef FOURIERFILTER_H
#define FOURIERFILTER_H

#include "Filter.h"

class FourierFilter : public Filter
{
public:
    FourierFilter();
    virtual ~FourierFilter();

    virtual void filter(Canvas2D *canvas);
};

#endif // FOURIERFILTER_H
