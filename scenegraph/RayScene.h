#ifndef RAYSCENE_H
#define RAYSCENE_H

#include "Scene.h"
#include "kdtree/KDTree.h"
#include "shapes/RayShape.h"

class Canvas2D;

/**
 * @class RayScene
 *
 *  Students will implement this class as necessary in the Ray project.
 */
class RayScene : public Scene
{
public:
    RayScene();
    virtual ~RayScene();

    // Transfers the data from the given scene to this scene and stores the
    // objects in a KDTree for faster spacial intersection tests.
    void transferSceneData(Scene *scene);

    // Iterates through every pixel in the range (width, height), calculates the color
    // based on intersections at that point and paints the provided canvas.
    void render(Canvas2D *canvas, Camera *camera, int width, int height);

    // stops the rendering after the current canvas pixel row is finished.
    void stopRendering();

private:

    // performs a ray tracing algorithm at the specified point.
    glm::vec3 rayTrace(float x, float y, float xmax, float ymax, glm::vec4 p_eye, glm::mat4 M_ftw);

    // calculates the color of a point based on the lighting and materials of the shape
    glm::vec3 calcColor(CS123ScenePrimitive *prim, glm::vec4 point, glm::vec4 n);

    // maps shape types to actual shape instances so multiple shapes of the same type aren't stored
    QHash<PrimitiveType, RayShape*> m_primShapes;
    QList<KDElement *> m_kdes;// doesn't need to be global after kdtree is working

    // kd tree to speed up search times.
    KDTree *m_tree;

    // flag to prevent further rendering
    bool m_stopRendering;
};

#endif // RAYSCENE_H
