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
#include <qcoreapplication.h>


RayScene::RayScene()
{
    m_primShapes.insert(PRIMITIVE_CONE, new RayCone());
    m_primShapes.insert(PRIMITIVE_CUBE, new RayCube());
    m_primShapes.insert(PRIMITIVE_CYLINDER, new RayCylinder());
    m_primShapes.insert(PRIMITIVE_SPHERE, new RaySphere());
//    m_cone = new RayCone();
//    m_cube = new RayCube();
//    m_cylinder = new RayCylinder();
//    m_sphere = new RaySphere();
}

RayScene::~RayScene()
{
    m_primShapes.value(PRIMITIVE_CONE);
    m_primShapes.value(PRIMITIVE_CUBE);
    m_primShapes.value(PRIMITIVE_CYLINDER);
    m_primShapes.value(PRIMITIVE_SPHERE);
//    delete m_cone;
//    delete m_cube;
//    delete m_cylinder;
//    delete m_sphere;
}


void RayScene::transferSceneData(Scene *scene)
{
    this->setGlobal(scene->getGlobalData());

    int n = scene->getNumShapes();
    for (int i = 0; i < n; i++) {
        this->addPrimitive(*(scene->getPrimitive(i)), scene->getMatrix(i), false);
    }

    n = scene->getNumLights();
    for (int i = 0; i < n; i++) {
        this->addLight(*(scene->getLightData(i)));
    }
}


void RayScene::render(Canvas2D *canvas, Camera *camera, int width, int height)
{
    canvas->resize(width, height);
    BGRA* pix = canvas->data();

    glm::mat4 M_ftw = glm::inverse(camera->getViewMatrix()) * glm::inverse(camera->getScaleMatrix());

//    cout << "VIEW: " << endl;
//    cout << glm::to_string(camera->getViewMatrix()) << endl;
//    cout << "SCALE: " << endl;
//    cout << glm::to_string(camera->getScaleMatrix()) << endl;
//    cout << "BOTH: " << endl;
//    cout << glm::to_string(camera->getProjectionMatrix() * camera->getViewMatrix()) << endl;

    glm::vec3 color;
    int ymax = height - 1;
    int xmax = width - 1;

    glm::vec4 p_eye = glm::inverse(camera->getViewMatrix()) * glm::vec4(0,0,0,1);
//    cout << canvas->width() << ", " << canvas->height() << endl;
//    cout << width << ", " << height << endl;

//    cout << "EYE: " << glm::to_string(p_eye) << endl;

//    rayTrace(width / 2, height / 2, xmax, ymax, p_eye, M_ftw);

    int i;
    for (int y = 0; y < height; y++) {
        i = y * width;
        for (int x = 0; x < width; x++) {

            color = rayTrace(x, y, xmax, ymax, p_eye, M_ftw);
//            assert(color.r <= 1.f);
            pix[i].r = (unsigned char)(color.r * 255.f + 0.5f);
            pix[i].g = (unsigned char)(color.g * 255.f + 0.5f);
            pix[i].b = (unsigned char)(color.b * 255.f + 0.5f);
//            assert(color.r * 255.f + 0.5 <= 255.5f);

            i++;
        }
//        QCoreApplication::processEvents();
    }

}


glm::vec3 RayScene::rayTrace(int x, int y, int xmax, int ymax, glm::vec4 p_eye, glm::mat4 M_ftw)
{
    glm::vec4 farFilm = glm::vec4(x * 2.0 / xmax - 1.f, 1.f - y * 2.0 / ymax, -1.f, 1);
    glm::vec4 farWorld = M_ftw * farFilm;
    glm::vec4 d_world = glm::normalize(farWorld - p_eye);

//    cout << "FILM: " << glm::to_string(farFilm) << endl;
//    cout << "WORLD: " << glm::to_string(farWorld) << endl;
//    cout << "D_WORLD: " << glm::to_string(d_world) << endl;

    glm::vec4 p, d;
    glm::mat4 M_inv;
    int bestIndex = -1;
    glm::vec4 bestT = glm::vec4(0.f, 0.f, 0.f, std::numeric_limits<float>::infinity());
    glm::vec4 nt;
    RayShape *shape;

    int num_shapes = m_shapes.size();
    for (int i = 0; i < num_shapes; i++) {

        M_inv = glm::inverse(m_trans.at(i));
        p = M_inv * p_eye;
        d = M_inv * d_world;

//        cout << "P: " << glm::to_string(p) << endl;
//        cout << "D: " << glm::to_string(d) << endl;


        shape = m_primShapes.value(m_shapes.at(i)->type);
        if (shape) {
            nt = shape->intersects(p, d);

            assert(nt.w >= 0);
            if (nt.w < bestT.w) {
                bestT = nt;
                bestIndex = i;
            }
        }
    }

    if (bestT.w < std::numeric_limits<float>::infinity()) {
        shape = m_primShapes.value(m_shapes.at(bestIndex)->type);

        glm::vec4 point = p + bestT.w * d;
        point = m_trans.at(bestIndex) * point;

        glm::vec4 n = glm::vec4(glm::normalize(glm::transpose(glm::inverse(glm::mat3(m_trans.at(bestIndex)))) * glm::vec3(bestT)), 0);

        CS123SceneLightData *light;
        CS123SceneMaterial &mat = m_shapes.at(bestIndex)->material;

        int num_lights = m_lights.size();

        glm::vec3 color = glm::vec3();
        glm::vec3 amb = glm::vec3(mat.cAmbient.r, mat.cAmbient.g, mat.cAmbient.b);
        glm::vec3 diff = glm::vec3(mat.cDiffuse.r, mat.cDiffuse.g, mat.cDiffuse.b);

        glm::vec3 coeff;
        glm::vec4 pToL;
        float nDotL;

        for (int i = 0; i < num_lights; i++) {
            light = m_lights.at(i);

            if (light->type == LIGHT_POINT)
                cout << glm::to_string(light->pos) << endl;
                pToL = glm::normalize(light->pos - point);
            if (light->type == LIGHT_DIRECTIONAL)
                pToL = -glm::normalize(light->dir);

            nDotL = std::max(0.f, glm::dot(n, pToL));

            coeff.r = diff.r * nDotL;
            coeff.g = diff.g * nDotL;
            coeff.b = diff.b * nDotL;

            color.r += light->color.r * coeff.r;
            color.g += light->color.g * coeff.g;
            color.b += light->color.b * coeff.b;
        }
        color.r = std::min(1.f, color.r + amb.r);
        color.g = std::min(1.f, color.g + amb.g);
        color.b = std::min(1.f, color.b + amb.b);
        return color;
//        return glm::vec3(1);
    }

    return glm::vec3();
}


