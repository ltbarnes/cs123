#ifndef ROTATEFILTER_H
#define ROTATEFILTER_H

#include "Filter.h"

class RotateFilter : public Filter
{
public:
    RotateFilter(float angle);
    virtual ~RotateFilter();

    virtual void filter(Canvas2D *canvas);

    void setAngle(float angle);

private:
    bool inBounds(glm::vec3 p, int w, int h);
    void fillPixel(BGRA *pix, int i, int width, float wxr, float wyb);

    double m_angle;
    glm::mat2 m_R;
};

#endif // ROTATEFILTER_H
