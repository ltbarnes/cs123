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
#include <QFuture>

class Canvas2D;
class RayTaskBlock;
class MainWindow;

//struct ImageBlock {

//    ImageBlock(BGRA *pixels, int ex, int why, int bw, int bh,
//               int fw, int fh, glm::vec4 eye, glm::mat4 ftw)
//    {
//        pix = pixels;
//        x = ex;
//        y = why;
//        blockWidth = bw;
//        blockHeight = bh;
//        fullWidth = fw;
//        fullHeight = fh;
//        p_eye = eye;
//        M_ftw = ftw;
//    }

//    BGRA *pix;
//    int x, y;
//    int blockWidth, blockHeight;
//    int fullWidth, fullHeight;
//    glm::vec4 p_eye;
//    glm::mat4 M_ftw;
//    glm::vec3 color;
//};

/**
 * @class RayScene
 *
 *  Students will implement this class as necessary in the Ray project.
 */
class RayScene : public QObject, Scene
{
    Q_OBJECT
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

    // stops the rendering after the current canvas pixel row is finished.
    void stopRendering();

public slots:
    void updateCanvas();

protected:

    // performs a ray tracing algorithm at the specified point.
//    glm::vec3 rayTrace(float x, float y, float xmax, float ymax, glm::vec4 p_eye, glm::mat4 M_ftw);

    // calculates the color of a point based on the lighting and materials of the shape
//    glm::vec3 calcColor(CS123ScenePrimitive *prim, glm::vec4 point, glm::vec4 n);

    // maps shape types to actual shape instances so multiple shapes of the same type aren't stored
    QHash<PrimitiveType, RayShape*> m_primShapes;
    QList<KDElement *> m_kdes;// doesn't need to be global after kdtree is working

    // kd tree to speed up search times.
    KDTree *m_tree;

    // flag to prevent further rendering
    bool m_stopRendering;


private:
    QFutureWatcher<void> m_futureWatcher;
    Canvas2D *m_canvas;
    QList<RayTaskBlock *> tasks;
    QFuture<void> future;
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

    // calculates the color of a point based on the lighting and materials of the shape
    glm::vec3 calcColor(CS123ScenePrimitive *prim, glm::vec4 point, glm::vec4 n);

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
