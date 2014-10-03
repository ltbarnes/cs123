#ifndef EDGEFILTER_H
#define EDGEFILTER_H

#include "Filter.h"

class EdgeFilter : public Filter
{
public:
    EdgeFilter(float threshold);
    virtual ~EdgeFilter();

    virtual void filter(Canvas2D *canvas);
    void setThreshold(float threshold);

private:
    float m_threshold;
};

#endif // EDGEFILTER_H
