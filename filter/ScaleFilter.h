#ifndef SCALEFILTER_H
#define SCALEFILTER_H

#include "Filter.h"

class ScaleFilter : public Filter
{
public:
    ScaleFilter(float xScale, float yScale);
    virtual ~ScaleFilter();

    virtual void filter(Canvas2D *canvas);

    void setXScale(float xScale);
    void setYScale(float yScale);


private:
    float F(int x, float a);
    float G(float x);
    void dostuff(int srcW, int destW, int destH, float a, bool x, int pad, BGRA *src, BGRA *dest);

    float m_xscale;
    float m_yscale;
    int m_width;
    int m_height;

};

#endif // SCALEFILTER_H
