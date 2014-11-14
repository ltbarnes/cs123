#include "RayScene.h"
#include "Settings.h"
#include "CS123SceneData.h"
#include "Canvas2D.h"
#include "Camera.h"
#include "shapes/RayShape.h"
#include "shapes/RayCone.h"
#include "shapes/RayCube.h"
#include "shapes/RayCylinder.h"
#include "shapes/RaySphere.h"
#include "kdtree/KDTree.h"
#include <qcoreapplication.h>
#include <QThread>
#include <QtConcurrent>
#include <QObject>
#include "moc_RayScene.cpp"
#include "mainwindow.h"

#define BLOCK_HEIGHT 10

using namespace std;

void executeTask(RayTaskBlock *task) {
    task->compute();
    QCoreApplication::processEvents();
}

RayScene::RayScene()
{
    m_primShapes.insert(PRIMITIVE_CONE, new RayCone());
    m_primShapes.insert(PRIMITIVE_CUBE, new RayCube());
    m_primShapes.insert(PRIMITIVE_CYLINDER, new RayCylinder());
    m_primShapes.insert(PRIMITIVE_SPHERE, new RaySphere());

    m_tree = NULL;
    m_tasks.clear();
}

RayScene::~RayScene()
{
    delete m_primShapes.value(PRIMITIVE_CONE);
    delete m_primShapes.value(PRIMITIVE_CUBE);
    delete m_primShapes.value(PRIMITIVE_CYLINDER);
    delete m_primShapes.value(PRIMITIVE_SPHERE);

    int num_kdelements = m_kdes.size();
    for (int i = 0; i < num_kdelements; i++) {
        delete m_kdes.at(i);
    }
    m_kdes.clear();

    // need both because m_tree stores repeat elements if they
    // overlap an AABB

     delete m_tree;

    for (int i = 0; i < m_tasks.size(); i++) {
        delete m_tasks.at(i);
    }
    m_tasks.clear();
}


void RayScene::stopRendering()
{
    m_stopRendering = true;
    if (!m_futureWatcher.isFinished()) {
        m_futureWatcher.cancel();
        m_futureWatcher.waitForFinished();
    }
}


void RayScene::transferSceneData(Scene *scene)
{
    cout << "Transfering scene data." << endl;

    // transfer global data
    this->setGlobal(scene->getGlobalData());

    // transfer light data
    int i;
    int n = scene->getNumLights();
    for (i = 0; i < n; i++) {
        this->addLight(*(scene->getLightData(i)));
    }

    // transfer object data and create kdelements based on each object's world location
    SceneElement *e;
    glm::vec3 miniest = glm::vec3(std::numeric_limits<float>::infinity());
    glm::vec3 maximus = glm::vec3(-std::numeric_limits<float>::infinity());
    glm::vec3 min = glm::vec3(1.f);
    glm::vec3 max = glm::vec3(1.f);
    glm::vec4 pos;

    n = scene->getNumElements();
    for (i = 0; i < n; i++) {
        this->addPrimitive(*(scene->getPrimitive(i)), scene->getMatrix(i), false);
        e = m_elements.at(i);

        m_primShapes.value(e->primitive->type)->getBounds(&min, &max, e->trans);

        m_kdes.append(new KDElement(min, max, pos, e));

        miniest = glm::min(min, miniest);
        maximus = glm::max(max, maximus);
    }
    m_elements.clear();

////     buffer on the sides of the scene
//    miniest += glm::vec3(-1.f);
//    maximus += glm::vec3(1.f);


    if (m_kdes.size() > 0) {
        cout << "Building KD Tree" << endl;

        // create new tree defined by the smallest and largest bounding points in the scene
        m_tree = new KDTree(m_kdes, miniest, maximus);
    }
}


void RayScene::render(MainWindow *window, Canvas2D *canvas, Camera *camera, int width, int height)
{
    m_stopRendering = false;

    // reset the canvas size
    canvas->resize(width, height);

    // get the film to world matrix and eye point
    glm::mat4 M_ftw = glm::inverse(camera->getViewMatrix()) * glm::inverse(camera->getScaleMatrix());
    glm::vec4 p_eye = glm::inverse(camera->getViewMatrix()) * glm::vec4(0,0,0,1);

    // delete previous tasks if any
    for (int i = 0; i < m_tasks.size(); i++) {
        delete m_tasks.at(i);
    }
    m_tasks.clear();

    RayTaskBlock *task;
    int y;

    // iterate through all pixels
    int subH = height - BLOCK_HEIGHT;
    for (y = 0; y < subH; y += BLOCK_HEIGHT) {

        task = new RayTaskBlock(this, canvas, 0, y, width, BLOCK_HEIGHT, width, height, p_eye, M_ftw);

        if (settings.useMultiThreading) {
            m_tasks.append(task);

        } else {
            task->compute();
            canvas->repaint();
            QCoreApplication::processEvents();
            delete task;
        }
    }

    task = new RayTaskBlock(this, canvas, 0, y, width, height - y, width, height, p_eye, M_ftw);

    if (settings.useMultiThreading) {
        m_tasks.append(task);

        QObject::connect(&m_futureWatcher, SIGNAL(progressValueChanged(int)), canvas, SLOT(repaint()));
        QObject::connect(&m_futureWatcher, SIGNAL(finished()), canvas, SLOT(repaint()));
        QObject::connect(&m_futureWatcher, SIGNAL(finished()), window, SLOT(changeToRenderButton()));

        m_futureWatcher.setFuture(QtConcurrent::map(m_tasks, &executeTask));

    } else {
        task->compute();
        canvas->repaint();
        delete task;
        window->changeToRenderButton();
    }

    QCoreApplication::processEvents();
}


RayTaskBlock::RayTaskBlock(RayScene *scene, Canvas2D *pix, int x, int y, int bw, int bh, int iw, int ih, glm::vec4 p_eye, glm::mat4 M_ftw)
{
    m_scene = scene;
    m_pix = pix;
    rect = QRect(x, y, bw, bh);
    m_x = x;
    m_y = y;
    m_blockWidth = bw;
    m_blockHeight = bh;
    m_imageWidth = iw;
    m_imageHeight = ih;
    m_p_eye = p_eye;
    m_M_ftw = M_ftw;
}


RayTaskBlock::~RayTaskBlock()
{
}


void RayTaskBlock::compute()
{
    glm::vec3 color, tl, tr, bl, br;
    BGRA *pix = m_pix->data();

    int i;
    for (int y = 0; y < m_blockHeight; y++) {
        i = (m_y + y) * m_imageWidth;

        for (int x = 0; x < m_blockWidth; x++) {

            if (m_scene->m_stopRendering)
                break;

            // sample each corner of the pixel and the center then average
            if (settings.useSuperSampling) {
                tl = rayTrace(x + m_x + 0.0f, y + m_y + 0.0f, m_imageWidth, m_imageHeight, m_p_eye, m_M_ftw);
                tr = rayTrace(x + m_x + 1.0f, y + m_y + 0.0f, m_imageWidth, m_imageHeight, m_p_eye, m_M_ftw);
                bl = rayTrace(x + m_x + 0.0f, y + m_y + 1.0f, m_imageWidth, m_imageHeight, m_p_eye, m_M_ftw);
                br = rayTrace(x + m_x + 1.0f, y + m_y + 1.0f, m_imageWidth, m_imageHeight, m_p_eye, m_M_ftw);
                color = rayTrace(x + m_x + 0.5f, y + m_y + 0.5f, m_imageWidth, m_imageHeight, m_p_eye, m_M_ftw);
                color = (tl + tr + bl + br + color) / 5.f;

            } else { // sample the center point of the pixel
                color = rayTrace(x + m_x + 0.5f, y + m_y + 0.5f, m_imageWidth, m_imageHeight, m_p_eye, m_M_ftw);
            }

            pix[i].r = (unsigned char)(color.r * 255.f + 0.5f);
            pix[i].g = (unsigned char)(color.g * 255.f + 0.5f);
            pix[i].b = (unsigned char)(color.b * 255.f + 0.5f);

            i++;
        }
//        assert(y < 3);
//        cout << i << endl;
    }
    emit doneDrawing(m_pix);
}


glm::vec3 RayTaskBlock::rayTrace(float x, float y, float xmax, float ymax, glm::vec4 p_eye, glm::mat4 M_ftw)
{
    // set the film and world vectors
    glm::vec4 farFilm = glm::vec4(x * 2.0 / xmax - 1.f, 1.f - y * 2.0 / ymax, -1.f, 1);
    glm::vec4 farWorld = M_ftw * farFilm;
    glm::vec4 d_world = glm::normalize(farWorld - p_eye);

    glm::vec4 p, d;
    glm::mat4 M_inv;
    int bestIndex = -1;
    glm::vec4 bestT = glm::vec4(0.f, 0.f, 0.f, std::numeric_limits<float>::infinity());
    glm::vec4 nt;
    RayShape *shape;

//    QList<KDElement *> kdes = m_kdes;
    QList<KDElement *> kdes = m_scene->m_tree->getIntersections(p_eye, d_world);

    // iterate through all necessary shapes and check for the shortest intersection distance
    int num_kdelements = kdes.size();
    for (int i = 0; i < num_kdelements; i++) {

        // get p and d in object space
        M_inv = glm::inverse(kdes.at(i)->getTrans());
        p = M_inv * p_eye;
        d = M_inv * d_world;

        // check for intersections in shape space
        shape = m_scene->m_primShapes.value(kdes.at(i)->getPrimitive()->type);
        if (shape) {
            nt = shape->intersects(p, d);

            assert(nt.w >= 0);
            if (nt.w < bestT.w) {
                bestT = nt;
                bestIndex = i;
            }
        }
    }

    // if an intersection occurred calculate the lighting based on the intersection point and normal
    if (bestT.w < std::numeric_limits<float>::infinity()) {
        CS123ScenePrimitive *prim = kdes.at(bestIndex)->getPrimitive();
        shape = m_scene->m_primShapes.value(prim->type);

        glm::vec4 point = p_eye + bestT.w * d_world;


        glm::vec4 n = glm::vec4(glm::normalize(
                        glm::transpose(glm::inverse(
                            glm::mat3(kdes.at(bestIndex)->getTrans())))
                                    * glm::vec3(bestT)), 0);

        return calcColor(prim, point, n);
    }

    return glm::vec3();
}


glm::vec3 RayTaskBlock::calcColor(CS123ScenePrimitive *prim, glm::vec4 point, glm::vec4 n)
{
    CS123SceneLightData *light;
    CS123SceneMaterial &mat = prim->material;

    int num_lights = m_scene->m_lights.size();

    // variables for the lighting equation
    glm::vec3 color = glm::vec3();
    glm::vec3 amb = glm::vec3(mat.cAmbient.r, mat.cAmbient.g, mat.cAmbient.b);
    glm::vec3 diff = glm::vec3(mat.cDiffuse.r, mat.cDiffuse.g, mat.cDiffuse.b);

    glm::vec3 coeff;
    glm::vec4 pToL;
    float nDotL;

    // sum from the lighting equation
    for (int i = 0; i < num_lights; i++) {
        light = m_scene->m_lights.at(i);

        // light direction vector
        if (light->type == LIGHT_POINT)
            pToL = glm::normalize(light->pos - point);
        else if (light->type == LIGHT_DIRECTIONAL)
            pToL = -glm::normalize(light->dir);

        // dot product of normal and light vector
        nDotL = std::max(0.f, glm::dot(n, pToL));

        // diffuse coefficient times dot product
        coeff.r = diff.r * nDotL;
        coeff.g = diff.g * nDotL;
        coeff.b = diff.b * nDotL;

        // color of the light
        color.r += light->color.r * coeff.r;
        color.g += light->color.g * coeff.g;
        color.b += light->color.b * coeff.b;
    }
    // add the ambient lighting
    color.r = std::min(1.f, color.r + amb.r);
    color.g = std::min(1.f, color.g + amb.g);
    color.b = std::min(1.f, color.b + amb.b);

    return color;
}
