#ifndef RAYSCENE_H
#define RAYSCENE_H

#include "Scene.h"
#include "OpenGLScene.h"
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

private:

    glm::vec3 rayTrace(int x, int y, int xmax, int ymax, glm::vec4 p_eye, glm::mat4 M_ftw);

    QHash<PrimitiveType, RayShape*> m_primShapes;
//    RayShape *m_cone;
//    RayShape *m_cube;
//    RayShape *m_cylinder;
//    RayShape *m_sphere;
};

#endif // RAYSCENE_H
