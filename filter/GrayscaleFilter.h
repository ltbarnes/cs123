#ifndef GRAYSCALEFILTER_H
#define GRAYSCALEFILTER_H

#include "Filter.h"

class GrayscaleFilter : public Filter
{
public:
    GrayscaleFilter();
    virtual ~GrayscaleFilter();

    virtual void filter(Canvas2D *canvas);
};

#endif // GRAYSCALEFILTER_H
