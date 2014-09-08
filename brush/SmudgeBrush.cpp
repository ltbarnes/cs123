/**
 * @file   Brush.cpp
 *
 * Implementation of a brush that smudges the image on the canvas as the mouse is dragged.
 *
 * You should fill this file in for the Brush assignment.
 */

#include <CS123Common.h>

#include "SmudgeBrush.h"
#include "Canvas2D.h"


SmudgeBrush::SmudgeBrush(BGRA color, int flow, int radius) : QuadraticBrush(color, flow, radius)
{
    // @TODO: [BRUSH] Initialize any memory you are going to use here. Hint - you are going to
    //       need to store temporary image data in memory. This might be a good place to
    //       allocate that memory.


    makeMask();
}


SmudgeBrush::~SmudgeBrush()
{
    // @TODO: [BRUSH] Be sure not to leak memory!

}



//! create a mask with a distribution of your choice (probably want to use quadratic for best results)
void SmudgeBrush::makeMask()
{
    // @TODO: [BRUSH] Set up the mask for your brush here. For this brush you will probably want
    //        to use a quadratic distribution for the best results. Linear or Gaussian would
    //        work too, however. Feel free to paste your code from the Linear or Quadratic brushes
    //        or modify the class inheritance to be able to take advantage of one of those class's
    //        existing implementations. The choice is yours!
    //
}

//! Picks up paint from the canvas before drawing begins.
void SmudgeBrush::pickUpPaint(int x, int y, Canvas2D* canvas)
{
    int width = canvas->width();
    int height = canvas->height();

    // @TODO: [BRUSH] Perform the "pick up paint" step described in the assignment handout here.
    //        In other words, you should store the "paint" under the brush mask in a temporary
    //        buffer (which you'll also have to figure out where and how to allocate). Then,
    //        in the paintOnce() method, you'll paste down the paint that you picked up here.
    //

}

void SmudgeBrush::paintOnce(int mouse_x, int mouse_y, Canvas2D* canvas)
{
    // @TODO: [BRUSH] Here, you'll need to paste down the paint you picked up in
    //        the previous method. Be sure to take the mask into account! You can
    //        ignore the alpha parameter, but you can also use it (smartly) if you
    //        would like to.

    // now pick up paint again...
    pickUpPaint(mouse_x, mouse_y, canvas);

}


