#include "BlurFilter.h"
#include "Canvas2D.h"

BlurFilter::BlurFilter() :
    Filter()
{
}


BlurFilter::~BlurFilter()
{
}


void BlurFilter::filter(Canvas2D *canvas)
{
    setBounds(canvas);
    makeFilterCanvas(canvas, 0);
}
