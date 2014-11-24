#ifndef RAYSCENE_H
#define RAYSCENE_H

#include "Scene.h"
#include "kdtree/KDTree.h"
#include "shapes/RayShape.h"
#include <QHash>
#include <QRunnable>
#include <QObject>
#include <QFutureWatcher>
#include <QList>
#include <QtConcurrent>

class Canvas2D;
class RayTaskBlock;
class MainWindow;


/**
 * @class RayScene
 *
 *  Students will implement this class as necessary in the Ray project.
 */
class RayScene : public Scene
{
public:
    friend class RayTaskBlock;
    RayScene();
    virtual ~RayScene();

    // Transfers the data from the given scene to this scene and stores the
    // objects in a KDTree for faster spacial intersection tests.
    void transferSceneData(Scene *scene);

    // Iterates through every pixel in the range (width, height), calculates the color
    // based on intersections at that point and paints the provided canvas.
    void render(MainWindow *window, Canvas2D *canvas, Camera *camera, int width, int height);

    bool addTexture(const QString &filename);

    // stops the rendering after the current canvas pixel row is finished.
    void stopRendering();

protected:

    // maps shape types to actual shape instances so multiple shapes of the same type aren't stored
    QHash<PrimitiveType, RayShape*> m_primShapes;
    QHash<QString, QImage> m_textures;
    QList<KDElement *> m_kdes;// doesn't need to be global after kdtree is working

    // kd tree to speed up search times.
    KDTree *m_tree;

    // flag to prevent further rendering
    bool m_stopRendering;


private:
    // Thread stuffs
    QFutureWatcher<void> m_futureWatcher;
    QList<RayTaskBlock *> m_tasks;
};


class RayTaskBlock : public QObject
{
    Q_OBJECT
public:
    RayTaskBlock(RayScene *scene, Canvas2D *pix, int x, int y, int bw, int bh,
                 int iw, int ih, glm::vec4 p_eye, glm::mat4 M_ftw);
    virtual ~RayTaskBlock();

    void compute();

signals:
    void doneDrawing(Canvas2D *canvas);

private:
    // performs a ray tracing algorithm at the specified point.
    glm::vec3 rayTrace(float x, float y, float xmax, float ymax, glm::vec4 p_eye, glm::mat4 M_ftw);

    // recursively shoots rays into the scene to better estimate lighting
    glm::vec3 raycursion(glm::vec4 p_world, glm::vec4 d_world, int depth);

    // calculates the color of a point based on the lighting and materials of the shape
    glm::vec3 calcColor(CS123ScenePrimitive *prim, glm::vec4 point, glm::vec4 n, glm::vec4 eye, glm::vec2 uv);

    RayScene *m_scene;
    Canvas2D *m_pix;
    QRect rect;
    int m_x, m_y;
    int m_blockWidth, m_blockHeight;
    int m_imageWidth, m_imageHeight;
    glm::vec4 m_p_eye;
    glm::mat4 m_M_ftw;
};

#endif // RAYSCENE_H
