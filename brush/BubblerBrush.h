#ifndef BUBBLERBRUSH_H
#define BUBBLERBRUSH_H


#include "Brush.h"

/**
 * @class BubblerBrush
 *
 * Brush that takes any paint at the current location and
 * throws it to other parts of the canvas.
 */
class BubblerBrush
        : public Brush
{

public:
    BubblerBrush(BGRA color, int flow, int radius);
    virtual ~BubblerBrush();

protected:
    int m_used;

    void makeMask(); // Constructs the mask for this brush.

    virtual void paintOnce(int mouse_x, int mouse_y, Canvas2D* canvas);
};

#endif // BUBBLERBRUSH_H
