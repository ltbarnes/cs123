/**
  * @file   BombBrush.cpp
  *
  * Implementation of a brush which 'throws' paint accross the canvas
  */

#include "BombBrush.h"

BombBrush::BombBrush(BGRA color, int flow, int radius)
    : Brush(color, flow, radius)
{
    makeMask();
}


BombBrush::~BombBrush()
{

}

void BombBrush::makeMask()
{

}


void BombBrush::pickUpPaint(int x, int y, Canvas2D* canvas)
{

}

void BombBrush::paintOnce(int x, int y, Canvas2D* canvas)
{

}
