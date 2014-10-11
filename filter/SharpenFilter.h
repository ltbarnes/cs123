#ifndef SHARPENFILTER_H
#define SHARPENFILTER_H

#include "FourierFilter.h"

class SharpenFilter : public FourierFilter
{
public:
    SharpenFilter();
    virtual ~SharpenFilter();

    virtual void filter(Canvas2D *canvas);

};

#endif // SHARPENFILTER_H
