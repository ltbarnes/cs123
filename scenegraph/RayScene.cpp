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
    m_cone = new RayCone();
    m_cube = new RayCube();
    m_cylinder = new RayCylinder();
    m_sphere = new RaySphere();
}

RayScene::~RayScene()
{
    delete m_cone;
    delete m_cube;
    delete m_cylinder;
    delete m_sphere;
}


void RayScene::transferSceneData(Scene *scene)
{
    this->setGlobal(scene->getGlobalData());

    int n = scene->getNumShapes();
    for (int i = 0; i < n; i++) {
        this->addPrimitive(*(scene->getPrimitive(i)), scene->getMatrix(i));
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
            pix[i].r = (unsigned char)(color.r * 255.f + 0.5f);
            pix[i].g = (unsigned char)(color.g * 255.f + 0.5f);
            pix[i].b = (unsigned char)(color.b * 255.f + 0.5f);

            i++;
        }
//        QCoreApplication::processEvents();
    }

}


glm::vec3 RayScene::rayTrace(int x, int y, int xmax, int ymax, glm::vec4 p_eye, glm::mat4 M_ftw)
{
    // GET RID OF THE " - 0.5" AFTER DEBUGGING!!!
    glm::vec4 farFilm = glm::vec4((x - .0) * 2.0 / xmax - 1.f, 1.f - y * 2.0 / ymax, -1.f, 1);
    glm::vec4 farWorld = M_ftw * farFilm;
    glm::vec4 d_world = glm::normalize(farWorld - p_eye);

//    cout << "FILM: " << glm::to_string(farFilm) << endl;
//    cout << "WORLD: " << glm::to_string(farWorld) << endl;
//    cout << "D_WORLD: " << glm::to_string(d_world) << endl;

    glm::vec4 p, d;
    glm::mat4 M_inv;
    int bestIndex;
    float bestT = std::numeric_limits<float>::infinity();
    float currT = std::numeric_limits<float>::infinity();
    CS123ScenePrimitive *currShape;

    int num_shapes = m_shapes.size();
    for (int i = 0; i < num_shapes; i++) {

        M_inv = glm::inverse(m_trans.at(i));
        p = M_inv * p_eye;
        d = M_inv * d_world;

//        cout << "P: " << glm::to_string(p) << endl;
//        cout << "D: " << glm::to_string(d) << endl;


        currShape = m_shapes.at(i);
        switch(currShape->type) {
        case PRIMITIVE_CONE:
            currT = m_cone->intersects(p, d);
            break;
        case PRIMITIVE_CUBE:
            currT = m_cube->intersects(p, d);
            break;
        case PRIMITIVE_CYLINDER:
            currT = m_cylinder->intersects(p, d);
            break;
        case PRIMITIVE_MESH:
            currT = std::numeric_limits<float>::infinity();
            break;
        case PRIMITIVE_SPHERE:
            currT = m_sphere->intersects(p, d);
            break;
        case PRIMITIVE_TORUS:
            currT = std::numeric_limits<float>::infinity();
            break;
        default:
            currT = std::numeric_limits<float>::infinity();
            break;
        }

        assert(currT >= 0);
        if (currT < bestT) {
            bestT = currT;
            bestIndex = i;
        }
    }

    if (bestT < std::numeric_limits<float>::infinity())
        return glm::vec3(1, 1, 1);

//    if ((x == 0 && y == 0) || (x == xmax && y == ymax)) {
//        cout << glm::to_string(p_film) << endl;
//        cout << glm::to_string(p_world) << endl;
//    }

    return glm::vec3();
}


