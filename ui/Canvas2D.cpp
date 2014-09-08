/**
 * @file Canvas2D.cpp
 *
 * CS123 2-dimensional canvas. Contains support code necessary for Brush, Filter, Intersect, and
 * Ray.
 *
 * YOU WILL NEED TO FILL THIS IN!
 *
 */

// For your convenience, a few headers are included for you.
#include <math.h>
#include <assert.h>
#include <unistd.h>
#include "Canvas2D.h"
#include "Canvas3D.h"
#include "Settings.h"
#include "RayScene.h"


#include <QPainter>
#include "brush/Brush.h"
#include "brush/ConstantBrush.h"
#include "brush/LinearBrush.h"
#include "brush/QuadraticBrush.h"
#include "brush/SmudgeBrush.h"

Canvas2D::Canvas2D()
{
    // @TODO: Initialize any pointers in this class here.
    m_scene = NULL;
    m_brush = NULL;

}

Canvas2D::~Canvas2D()
{
    // @TODO: Be sure to release all memory that you allocate.
    delete m_scene;
    if (m_brush)
        delete m_brush;

}

// This is called when the canvas size is changed. You can change the canvas size by calling
// resize(...). You probably won't need to fill this in, but you can if you want to.
void Canvas2D::notifySizeChanged(int w, int h) {

}

void Canvas2D::paintEvent(QPaintEvent *e) {
    // You probably won't need to fill this in, but you can if you want to override any painting
    // events for the 2D canvas. For now, we simply call the superclass.

    SupportCanvas2D::paintEvent(e);

}

// ********************************************************************************************
// ** BRUSH
// ********************************************************************************************

void Canvas2D::mouseDown(int x, int y)
{
    // @TODO: [BRUSH] Mouse interaction for brush. You will probably want to create a separate
    //        class for each of your brushes. Remember that you can use the static Settings
    //        object to get the currently selected brush and its parameters.

    int currentBrush = settings.brushType;
    int currentRadius = settings.brushRadius;

    BGRA currentColor;
    currentColor.b = settings.brushBlue;
    currentColor.g = settings.brushGreen;
    currentColor.r = settings.brushRed;
    currentColor.a = 255;

    int currentFlow = settings.brushAlpha;
    // You're going to need to leave the alpha value on the canvas itself at 255, but you will
    // need to use the actual alpha value to compute the new color of the pixel

    switch (currentBrush) {
    case BRUSH_SOLID:
        m_brush = new ConstantBrush(currentColor, currentFlow, currentRadius);
        break;
    case BRUSH_LINEAR:
        m_brush = new LinearBrush(currentColor, currentFlow, currentRadius);
        break;
    case BRUSH_QUADRATIC:
        m_brush = new QuadraticBrush(currentColor, currentFlow, currentRadius);
        break;
    case BRUSH_SMUDGE:
        m_brush = new SmudgeBrush(currentColor, currentFlow, currentRadius);
        m_brush->paintOnce(x, y, this);
        break;
    case BRUSH_SPECIAL_1:
//        m_brush = new (currentColor, currentFlow, currentRadius);
//        break;
    case BRUSH_SPECIAL_2:
//        m_brush = new (currentColor, currentFlow, currentRadius);
//        break;
    default:
        m_brush = new ConstantBrush(currentColor, currentFlow, currentRadius);
        break;
    }

    bool fixAlphaBlending = settings.fixAlphaBlending; // for extra/half credit

}

void Canvas2D::mouseDragged(int x, int y)
{ 
    // TODO: [BRUSH] Mouse interaction for Brush.

    m_brush->paintOnce(x, y, this);
}

void Canvas2D::mouseUp(int x, int y)
{
    // TODO: [BRUSH] Mouse interaction for Brush.
    delete m_brush;
    m_brush = NULL;
}



// ********************************************************************************************
// ** FILTER
// ********************************************************************************************

void Canvas2D::filterImage()
{
    // TODO: [FILTER] Filter the image. Some example code to get the filter type is provided below.

    switch (settings.filterType) {
    case FILTER_BLUR:
        // ...
        break;
        // fill in the rest
    }

}

void Canvas2D::setScene(RayScene *scene)
{
    delete m_scene;
    m_scene = scene;
}


void Canvas2D::renderImage(Camera *camera, int width, int height)
{
    if (m_scene)
    {
        // @TODO: raytrace the scene based on settings
        //        YOU MUST FILL THIS IN FOR INTERSECT/RAY

        // If you want the interface to stay responsive, make sure to call
        // QCoreApplication::processEvents() periodically during the rendering.

    }
}

void Canvas2D::cancelRender()
{
    // TODO: cancel the raytracer (optional)
}



void Canvas2D::settingsChanged() {

    // TODO: Process changes to the application settings.
}

