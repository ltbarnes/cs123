#ifndef INVERTFILTER_H
#define INVERTFILTER_H

#include "Filter.h"

class InvertFilter : public Filter
{
public:
    InvertFilter();
    virtual ~InvertFilter();

    virtual void filter(Canvas2D *canvas);
};

#endif // INVERTFILTER_H
