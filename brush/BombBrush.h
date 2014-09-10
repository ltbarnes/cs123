#ifndef BOMBBRUSH_H
#define BOMBBRUSH_H

#include "Brush.h"


struct frag {
    float x;
    float y;
    float h;
    float dx;
    float dy;
    float dh;
    BGRA color;
};

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
    frag* m_bomb;
    int m_blown;

    void makeMask(); // Constructs the mask for this brush.

    void blowUp(int x, int y, Canvas2D* canvas);

    virtual void update(bool lastUpdate);

    virtual void renderAnimation(Canvas2D *canvas);

    virtual void paintOnce(int mouse_x, int mouse_y, Canvas2D* canvas);

private:
    int m_bombRadius;
    int m_bombWidth;
    int m_bombSize;

    int m_maxHeight;

    float m_accel;
    float m_vel;
    float m_time;
};

#endif // BOMBBRUSH_H
