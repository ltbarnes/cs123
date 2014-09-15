#ifndef BUBBLERBRUSH_H
#define BUBBLERBRUSH_H


#include "Brush.h"

/**
 * @class BubblerBrush
 *
 * Brush that creates bubble like forms on the canvas
 */
class BubblerBrush
        : public Brush
{

public:
    BubblerBrush(BGRA color, int flow, int radius);
    virtual ~BubblerBrush();

protected:
    bool m_used; // checks if the brush has already been painted

    void makeMask(); // Constructs the mask for this brush.

    virtual void paintOnce(int mouse_x, int mouse_y, Canvas2D* canvas);
};

#endif // BUBBLERBRUSH_H
