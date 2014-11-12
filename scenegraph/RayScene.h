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

    void transferSceneData(Scene *scene);

    void render(Canvas2D *canvas, Camera *camera, int width, int height);
    void stopRendering();

private:

    glm::vec3 rayTrace(float x, float y, float xmax, float ymax, glm::vec4 p_eye, glm::mat4 M_ftw);

    QHash<PrimitiveType, RayShape*> m_primShapes;
    QList<KDElement *> m_kdes;

    KDTree *m_tree;

    bool m_stopRendering;
};

#endif // RAYSCENE_H
