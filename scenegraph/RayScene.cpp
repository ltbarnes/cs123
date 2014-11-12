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


RayScene::RayScene()
{
    m_primShapes.insert(PRIMITIVE_CONE, new RayCone());
    m_primShapes.insert(PRIMITIVE_CUBE, new RayCube());
    m_primShapes.insert(PRIMITIVE_CYLINDER, new RayCylinder());
    m_primShapes.insert(PRIMITIVE_SPHERE, new RaySphere());

    m_tree = NULL;
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

    // this references the same pointers and should be used
    // instead of m_kdes when the kdtree is functional
     delete m_tree;
}


void RayScene::stopRendering()
{
    m_stopRendering = true;
}


void RayScene::transferSceneData(Scene *scene)
{
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
    glm::vec4 min = glm::vec4(1.f);
    glm::vec4 max = glm::vec4(1.f);
    glm::vec4 pos, minTemp, maxTemp;

    n = scene->getNumElements();
    for (i = 0; i < n; i++) {
        this->addPrimitive(*(scene->getPrimitive(i)), scene->getMatrix(i), false);
        e = m_elements.at(i);
        pos = e->trans * glm::vec4(glm::vec3(0.f), 1); // world space pos
        minTemp = e->trans * glm::vec4(glm::vec3(-0.5f), 1); // world space bounding box
        maxTemp = e->trans * glm::vec4(glm::vec3(0.5f), 1);

        // in case there are rotations
        min = glm::min(minTemp, maxTemp);
        max = glm::max(minTemp, maxTemp);

        m_kdes.append(new KDElement(min, max, pos, e));

        miniest = glm::min(glm::vec3(min), miniest);
        maximus = glm::max(glm::vec3(max), maximus);
    }
    m_elements.clear();

    // buffer on the sides of the scene
    miniest += glm::vec3(-1.f);
    maximus += glm::vec3(1.f);

    // create new tree defined by the smallest and largest bounding points in the scene
    m_tree = new KDTree(m_kdes, miniest, maximus);

//    m_tree->printTree(m_tree->getRoot());
}


void RayScene::render(Canvas2D *canvas, Camera *camera, int width, int height)
{
    m_stopRendering = false;

    // reset the canvas size and get the pixel array
    canvas->resize(width, height);
    BGRA* pix = canvas->data();

    // get the film to world matrix and eye point
    glm::mat4 M_ftw = glm::inverse(camera->getViewMatrix()) * glm::inverse(camera->getScaleMatrix());
    glm::vec4 p_eye = glm::inverse(camera->getViewMatrix()) * glm::vec4(0,0,0,1);

    glm::vec3 color, tl, tr, bl, br;

    // iterate through all pixels
    int i;
    for (int y = 0; y < height; y++) {
        i = y * width;
        for (int x = 0; x < width; x++) {

            // sample each corner of the pixel and the center then average
            if (settings.useSuperSampling) {
                tl = rayTrace(x + 0.0f, y + 0.0f, width, height, p_eye, M_ftw);
                tr = rayTrace(x + 1.0f, y + 0.0f, width, height, p_eye, M_ftw);
                bl = rayTrace(x + 0.0f, y + 1.0f, width, height, p_eye, M_ftw);
                br = rayTrace(x + 1.0f, y + 1.0f, width, height, p_eye, M_ftw);
                color = rayTrace(x + 0.5f, y + 0.5f, width, height, p_eye, M_ftw);
                color = (tl + tr + bl + br + color) / 5.f;

            } else { // sample the center point of the pixel
                color = rayTrace(x + 0.5f, y + 0.5f, width, height, p_eye, M_ftw);
            }
            pix[i].r = (unsigned char)(color.r * 255.f + 0.5f);
            pix[i].g = (unsigned char)(color.g * 255.f + 0.5f);
            pix[i].b = (unsigned char)(color.b * 255.f + 0.5f);

            i++;
        }
        // repaint and check the gui
        canvas->repaint();
        QCoreApplication::processEvents();
        if (m_stopRendering)
            break;
    }

}


glm::vec3 RayScene::rayTrace(float x, float y, float xmax, float ymax, glm::vec4 p_eye, glm::mat4 M_ftw)
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

    QList<KDElement *> kdes = m_kdes;
//    QList<KDElement *> kdes = m_tree->getIntersections(p_eye, d_world);

    // iterate through all necessary shapes and check for the shortest intersection distance
    int num_kdelements = kdes.size();
    for (int i = 0; i < num_kdelements; i++) {

        // get p and d in object space
        M_inv = glm::inverse(kdes.at(i)->getTrans());
        p = M_inv * p_eye;
        d = M_inv * d_world;

        // check for intersections in shape space
        shape = m_primShapes.value(kdes.at(i)->getPrimitive()->type);
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
        shape = m_primShapes.value(prim->type);

        glm::vec4 point = p_eye + bestT.w * d_world;


        glm::vec4 n = glm::vec4(glm::normalize(
                        glm::transpose(glm::inverse(
                            glm::mat3(kdes.at(bestIndex)->getTrans())))
                                    * glm::vec3(bestT)), 0);

        return calcColor(prim, point, n);
    }

    return glm::vec3();
}


glm::vec3 RayScene::calcColor(CS123ScenePrimitive *prim, glm::vec4 point, glm::vec4 n)
{
    CS123SceneLightData *light;
    CS123SceneMaterial &mat = prim->material;

    int num_lights = m_lights.size();

    // variables for the lighting equation
    glm::vec3 color = glm::vec3();
    glm::vec3 amb = glm::vec3(mat.cAmbient.r, mat.cAmbient.g, mat.cAmbient.b);
    glm::vec3 diff = glm::vec3(mat.cDiffuse.r, mat.cDiffuse.g, mat.cDiffuse.b);

    glm::vec3 coeff;
    glm::vec4 pToL;
    float nDotL;

    // sum from the lighting equation
    for (int i = 0; i < num_lights; i++) {
        light = m_lights.at(i);

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


