#ifndef CANVAS3D_H
#define CANVAS3D_H

#include "SupportCanvas3D.h"
#include "CS123Common.h"

/**
 * @class Canvas3D
 *
 * 3D canvas that uses OpenGL. Students will implement this canvas in the OpenGL labs and in the
 * Shapes, Camtrans, and Sceneview assignments.
 */
class Canvas3D : public SupportCanvas3D
{
    Q_OBJECT
public:
    Canvas3D(QGLFormat format, QWidget *parent);
    virtual ~Canvas3D();

    virtual void initializeGL();

    virtual void paintGL();

    // Called when the settings have changed.
    virtual void settingsChanged();

protected:
    // Gets called at an interval specified by the timer
    virtual void timerEvent(QTimerEvent *);

private:
    int m_timer;
    bool m_useSceneviewSceneOld;
};

#endif // CANVAS3D_H
