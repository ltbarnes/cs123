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
#include <QtSerialPort>
#include <QColor>

#define RECURSION_LIMIT 5           // number of reflection rays
#define BLOCK_HEIGHT 10             // number of rows in each thread
#define SAMPLE_THRESHOLD 0.0002f    // variance threshold for super sampling

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

    // need both because m_tree stores repeat elements if shapes overlap an AABB
    delete m_tree;

    for (int i = 0; i < m_tasks.size(); i++) {
        delete m_tasks.at(i);
    }
    m_tasks.clear();

//    QList<QString> keys = m_textures.keys();
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
        if (e->primitive->material.textureMap->isUsed) {
            if (!addTexture(QString::fromStdString(e->primitive->material.textureMap->filename)))
                e->primitive->material.textureMap->isUsed = false;
        }
        m_primShapes.value(e->primitive->type)->getBounds(&min, &max, e->trans);

        m_kdes.append(new KDElement(min, max, pos, e));

        miniest = glm::min(min, miniest);
        maximus = glm::max(max, maximus);
    }
    m_elements.clear();


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


bool RayScene::addTexture(const QString &filename)
{
    // make sure file exists
    QFile file(filename);
    if (!file.exists())
        return false;

    if (m_textures.contains(filename))
        return true;

    // load file into memory
    QImage image;
    image.load(file.fileName());
    image = image.mirrored(false, true);
    m_textures.insert(filename, image);
    return true;
}


RayTaskBlock::RayTaskBlock(RayScene *scene, Canvas2D *canvas, int x, int y, int bw, int bh, int iw, int ih, glm::vec4 p_eye, glm::mat4 M_ftw)
{
    m_scene = scene;
    m_canvas = canvas;
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
    glm::vec3 color, tl, tr, bl, br, variance;
    BGRA *pix = m_canvas->data();

    int i;
    for (int y = 0; y < m_blockHeight; y++) {
        i = (m_y + y) * m_imageWidth + m_x;

        for (int x = 0; x < m_blockWidth; x++) {

            if (m_scene->m_stopRendering)
                break;

            // take four samples from the pixel and calculate the variance. If the variance is above
            // the threshold take more samples depending on the settings.numSuperSamples variable.
            if (settings.useSuperSampling) {
                // 4 initial samples
                tl = rayTrace(x + m_x + 0.2f, y + m_y + 0.3f, m_imageWidth, m_imageHeight, m_p_eye, m_M_ftw);
                tr = rayTrace(x + m_x + 0.7f, y + m_y + 0.2f, m_imageWidth, m_imageHeight, m_p_eye, m_M_ftw);
                bl = rayTrace(x + m_x + 0.3f, y + m_y + 0.8f, m_imageWidth, m_imageHeight, m_p_eye, m_M_ftw);
                br = rayTrace(x + m_x + 0.8f, y + m_y + 0.7f, m_imageWidth, m_imageHeight, m_p_eye, m_M_ftw);
                color = (tl + tr + bl + br) / 4.f;

                // variance calculation
                variance = (tl - color) * (tl - color);
                variance += (tr - color) * (tr - color);
                variance += (bl - color) * (bl - color);
                variance += (br - color) * (br - color);
                variance /= 4.f;

                // take more samples if necessary
                int samples2 = settings.numSuperSamples * settings.numSuperSamples - 4;
                if (samples2 > 4 && (variance.r > SAMPLE_THRESHOLD || variance.g > SAMPLE_THRESHOLD || variance.b > SAMPLE_THRESHOLD)) {

                    // this highlights the pixels that would be supersampled based on variance
                    if (settings.useAntiAliasing)
                        color = glm::vec3(1.f);

                    else {
                        color = (tl + tr + bl + br);

                        // uniform randomness
                        default_random_engine gen;
                        uniform_int_distribution<int> dist(0, 10);

                        float u, v;
                        for (int i = 0; i < samples2; i++) {
                            u = dist(gen) / 10.f;
                            v = dist(gen) / 10.f;
                            color += rayTrace(x + m_x + u, y + m_y + v, m_imageWidth, m_imageHeight, m_p_eye, m_M_ftw);
                        }
                        color /= glm::max(samples2 + 4, 4);
                    }
                }

            } else { // (no super sampling) sample the center point of the pixel
                color = rayTrace(x + m_x + 0.5f, y + m_y + 0.5f, m_imageWidth, m_imageHeight, m_p_eye, m_M_ftw);
            }

            pix[i].r = (unsigned char)(color.r * 255.f + 0.5f);
            pix[i].g = (unsigned char)(color.g * 255.f + 0.5f);
            pix[i].b = (unsigned char)(color.b * 255.f + 0.5f);

            i++;
        }
    }
    emit doneDrawing(m_canvas);
}


glm::vec3 RayTaskBlock::rayTrace(float x, float y, float xmax, float ymax, glm::vec4 p_eye, glm::mat4 M_ftw)
{
    // set the film and world vectors
    glm::vec4 farFilm = glm::vec4(x * 2.0 / xmax - 1.f, 1.f - y * 2.0 / ymax, -1.f, 1);
    glm::vec4 farWorld = M_ftw * farFilm;
    glm::vec4 d_world = glm::normalize(farWorld - p_eye);

    // recursively calculate colors
    return raycursion(p_eye, d_world, 0);
}


glm::vec3 RayTaskBlock::raycursion(glm::vec4 p_world, glm::vec4 d_world, int depth)
{
    // check depth then increment
    if (depth >= RECURSION_LIMIT)
        return glm::vec3();

    glm::vec4 p, d;
    glm::mat4 M_inv;
    int bestIndex = -1;
    glm::vec4 bestT = glm::vec4(0.f, 0.f, 0.f, std::numeric_limits<float>::infinity());
    glm::vec4 nt;
    RayShape *shape;

    //    QList<KDElement *> kdes = m_kdes;
    QList<KDElement *> kdes = m_scene->m_tree->getIntersections(p_world, d_world);

    // iterate through all necessary shapes and check for the shortest intersection distance
    int num_kdelements = kdes.size();
    for (int i = 0; i < num_kdelements; i++) {

        // get p and d in object space
        M_inv = kdes.at(i)->getInv();
        p = M_inv * p_world;
        d = M_inv * d_world;

        // check for intersections in shape space
        shape = m_scene->m_primShapes.value(kdes.at(i)->getPrimitive()->type);
        if (shape) {
            nt = shape->intersects(p, d);

            assert(nt.w >= 0);
            if (nt.w < bestT.w && !EQ(nt.w, 0.f)) {
                bestT = nt;
                bestIndex = i;
            }
        }
    }

    // if an intersection occurred calculate the lighting based on the intersection point and normal
    if (bestT.w < std::numeric_limits<float>::infinity()) {
        CS123ScenePrimitive *prim = kdes.at(bestIndex)->getPrimitive();
        shape = m_scene->m_primShapes.value(prim->type);

        glm::vec4 point = p_world + bestT.w * d_world;

        // compute world normal
        glm::vec4 n = glm::vec4(glm::normalize(
                        glm::transpose(glm::inverse(
                            glm::mat3(kdes.at(bestIndex)->getTrans())))
                                    * glm::vec3(bestT)), 0);

        // calculate texture uv coordinates
        glm::vec2 uv = glm::vec2(-1); // no texture

        if (settings.useTextureMapping) {
            CS123SceneMaterial &mat = prim->material;
            if (mat.textureMap->isUsed && mat.blend) {
                bestT.w = 0.f;
                uv = shape->getUV(kdes.at(bestIndex)->getInv() * point, bestT);
            }
        }

        if (settings.useReflection) {
            glm::vec4 reflection = glm::normalize(2.f * n * glm::dot(n, -d_world) + d_world);
            CS123SceneMaterial &mat = prim->material;
            glm::vec3 refl = glm::vec3(mat.cReflective.r, mat.cReflective.g, mat.cReflective.b);
            return glm::min(glm::vec3(1.f), calcColor(prim, point, n, p_world, uv) + refl * raycursion(point, reflection, depth + 1));
        }

        return calcColor(prim, point, n, p_world, uv);
    }

    return glm::vec3();

}


glm::vec3 RayTaskBlock::calcColor(CS123ScenePrimitive *prim, glm::vec4 point, glm::vec4 n, glm::vec4 eye, glm::vec2 uv)
{
    CS123SceneLightData *light;
    CS123SceneMaterial &mat = prim->material;

    int num_lights = m_scene->m_lights.size();

    // variables for the lighting equation
    glm::vec3 color = glm::vec3();
    glm::vec3 amb = glm::vec3(mat.cAmbient.r, mat.cAmbient.g, mat.cAmbient.b);
    glm::vec3 diff = glm::vec3(mat.cDiffuse.r, mat.cDiffuse.g, mat.cDiffuse.b);
    glm::vec3 spec = glm::vec3(mat.cSpecular.r, mat.cSpecular.g, mat.cSpecular.b);

    glm::vec3 diffCalc;
    glm::vec3 specCalc;
    glm::vec4 pToL;
    glm::vec4 reflection;
    glm::vec4 pToEye;
    float dist;
    float att;
    float nDotL;

    // sum from the lighting equation
    for (int i = 0; i < num_lights; i++) {
        light = m_scene->m_lights.at(i);

        pToEye = eye - point;

        // light direction vector
        switch (light->type) {
        case LIGHT_POINT:
            if (!settings.usePointLights)
                return glm::min(glm::vec3(1.f), amb);

            pToL = glm::normalize(light->pos - point);
            break;

        case LIGHT_DIRECTIONAL:
            if (!settings.useDirectionalLights)
                return glm::min(glm::vec3(1.f), amb);

            pToL = -glm::normalize(light->dir);
            break;

        case LIGHT_SPOT:
            if (!settings.useSpotLights)
                return glm::min(glm::vec3(1.f), amb);

            pToL = glm::normalize(light->pos - point);

            if (glm::dot(glm::normalize(light->dir), -pToL) <= glm::cos((light->angle + light->penumbra) * 2.f*M_PI / 360.f))
                continue;

            break;

        case LIGHT_AREA:
            break;
        }

        glm::vec4 bestT = glm::vec4(0.f, 0.f, 0.f, std::numeric_limits<float>::infinity());
        RayShape *shape;

        // check intersections
        if (settings.useShadows) {
            glm::vec4 p, d;
            glm::mat4 M_inv;
            glm::vec4 nt;

            QList<KDElement *> kdes = m_scene->m_tree->getIntersections(point, pToL);

            // iterate through all necessary shapes and check for the shortest intersection distance
            int num_kdelements = kdes.size();
            for (int i = 0; i < num_kdelements; i++) {

                // don't need to check current shape
                if (kdes.at(i)->getPrimitive() == prim)
                    continue;

                // get p and d in object space
                M_inv = kdes.at(i)->getInv();
                p = M_inv * point;
                d = M_inv * pToL;

                // check for intersections in shape space
                shape = m_scene->m_primShapes.value(kdes.at(i)->getPrimitive()->type);
                if (shape) {
                    nt = shape->intersects(p, d);

                    assert(nt.w >= 0);
                    if (nt.w < bestT.w && !EQ(nt.w, 0.f)) {
                        bestT = nt;
                    }
                }
            }
        }

        // if no intersection occurred calculate the non ambient lighting
        if (bestT.w == std::numeric_limits<float>::infinity()) {

            // dot product of normal and light vector
            nDotL = std::max(0.f, glm::dot(n, pToL));

            // calc texture
            if (uv.x != -1) {
                QImage image = m_scene->m_textures.value(QString::fromStdString(mat.textureMap->filename));
                glm::vec2 st = glm::vec2(uv.x * mat.textureMap->repeatU * image.width(), uv.y * mat.textureMap->repeatV * image.height());

                int r = ((int) st.y) % image.height();
                int c = ((int) st.x) % image.width();

                QColor clr = QColor::fromRgb(image.pixel(c, r));
                diffCalc = mat.blend * glm::vec3(clr.redF(), clr.greenF(), clr.blueF()) + (1.f - mat.blend) * diff;
                amb *= glm::vec3(clr.redF(), clr.greenF(), clr.blueF());
            } else {
                diffCalc = diff;
            }

            // diffuse coefficient times dot product
            diffCalc = diffCalc * nDotL;

            if (light->type == LIGHT_SPOT && settings.useSpotLights) {
                dist = glm::dot(glm::normalize(light->dir), -pToL);
                att = light->penumbra * 2.f*M_PI / 360.f;
                diffCalc *= glm::max(0.f, glm::pow((att - glm::acos(dist)) / att, 0.5f));
            }

            // reflection vector
            reflection = 2.f * glm::dot(pToL, n) * n - pToL;

            // specular calculation
            specCalc = glm::max(glm::vec3(), spec * std::pow(glm::max(0.f, glm::dot(glm::normalize(reflection), glm::normalize(pToEye))), mat.shininess));

            // attenuation
            if (light->type != LIGHT_DIRECTIONAL) {
                dist = glm::distance(light->pos, point);
                att = std::min(1.f, 1.f / (light->function.x + light->function.y * dist + light->function.z * dist * dist));
            } else {
                att = 1.f;
            }

            color.r += light->color.r * (diffCalc.r + specCalc.r) * att;
            color.g += light->color.g * (diffCalc.g + specCalc.g) * att;
            color.b += light->color.b * (diffCalc.b + specCalc.b) * att;
        }
    }

    // add the ambient lighting
    color = glm::min(glm::vec3(1.f), color + amb);

    return color;
}
