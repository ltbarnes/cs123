#include "GrayscaleFilter.h"
#include "Canvas2D.h"

GrayscaleFilter::GrayscaleFilter()
    : Filter()
{
}


GrayscaleFilter::~GrayscaleFilter()
{
}


void GrayscaleFilter::filter(Canvas2D *canvas)
{
    setBounds(canvas);
    grayscale(canvas);
    canvas->repaint();
}
