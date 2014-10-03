#ifndef BLURFILTER_H
#define BLURFILTER_H

#include "Filter.h"

class BlurFilter : public Filter
{
public:
    BlurFilter();
    virtual ~BlurFilter();

    virtual void filter(Canvas2D *canvas);
};

#endif // BLURFILTER_H
