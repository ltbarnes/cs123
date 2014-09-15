#ifndef BOMBBRUSH_H
#define BOMBBRUSH_H

#include "Brush.h"

/**
 * @brief The blob struct - tracks the locations and movement of
 * the 'blobs' thrown out by the bomb.
 */
struct blob {
    float x;    // x-location
    float y;    // y-location
    float h;    // height off the screen
    float dx;   // rate of change for x
    float dy;   // rate of change for y
    float dh;   //
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
    blob* m_bomb; // array of blobs
    bool m_blown;

    void makeMask(); // Constructs the mask for this brush.

    void blowUp(int x, int y, Canvas2D* canvas);

    virtual void update();

    virtual void renderAnimation(Canvas2D *canvas);

    virtual void paintOnce(int mouse_x, int mouse_y, Canvas2D* canvas);

private:
    int m_bombRadius;   // relationship between blob amount and brush radius
    int m_bombWidth;    // width of blob array
    int m_bombSize;     // total size of blob array

    float m_centerX;    // previous mouse location
    float m_centerY;

    float m_accel;      // rate of acceleration
    float m_vel;        // initial velocity
    float m_time;       // pseudo time since making brush
};

#endif // BOMBBRUSH_H
