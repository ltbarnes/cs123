#ifndef SCENEVIEWSCENE_H
#define SCENEVIEWSCENE_H

#include "OpenGLScene.h"
#include "SelectionRecorder.h"
#include "shapes/Shape.h"
#include "shapes/Cone.h"
#include "shapes/Cube.h"
#include "shapes/Cylinder.h"
#include "shapes/Sphere.h"
#include "shapes/Torus.h"


/**
 *
 * @class SceneviewScene
 *
 * A complex scene consisting of multiple objects. Students will implement this class in the
 * Sceneview assignment.
 *
 * Here you will implement your scene graph. The structure is up to you - feel free to create new
 * classes and data structures as you see fit. We are providing this SceneviewScene class for you
 * to use as a stencil if you like.
 *
 * Keep in mind that you'll also be rendering entire scenes in the next two assignments, Intersect
 * and Ray. The difference between this assignment and those that follow is here, we are using
 * OpenGL to do the rendering. In Intersect and Ray, you will be responsible for that.
 */
class SceneviewScene : public OpenGLScene
{
public:
    SceneviewScene();
    virtual ~SceneviewScene();

    // Initialize the scene.
    virtual void init();

protected:
    // Set the light uniforms for the lights in the scene. (View matrix is used in cases where a
    // light follows the camera, as in ShapesScene.)
    virtual void setLights(const glm::mat4 viewMatrix);

    // Render geometry for Shapes and Sceneview.
    virtual void renderGeometry();

public:
    // Use this method to set an internal selection, based on the (x, y) position of the mouse
    // pointer.  This will be used during the "modeler" lab, so don't worry about it for now.
    void setSelection(int x, int y);

private:
    void setShapeParams(int p1, int p2, float p3);
    void updateShape(Shape *shape);

    Shape *m_cone;
    Shape *m_cube;
    Shape *m_cylinder;
    Shape *m_sphere;
    Shape *m_torus;

    // This will help you during the "modeler" lab, so don't worry about it for now.
    SelectionRecorder m_selectionRecorder;
    int m_selectionIndex;

};

#endif // SCENEVIEWSCENE_H
