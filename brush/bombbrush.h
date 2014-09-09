#ifndef BOMBBRUSH_H
#define BOMBBRUSH_H

#include "Brush.h"

/**
 * @class BombBrush
 *
 * Brush that takes any paint at the current location and
 * throws it to other parts of the canvas.
 */
class BombBrush
        : public Brush
{

public:
    BombBrush(BGRA color, int flow, int radius);
    virtual ~BombBrush();

protected:
    BGRA* m_bomb;
    int m_blown;

    void makeMask(); // Constructs the mask for this brush.

    void pickUpPaint(int x, int y, Canvas2D* canvas);

    virtual void paintOnce(int x, int y, Canvas2D* canvas);
};

#endif // BOMBBRUSH_H
