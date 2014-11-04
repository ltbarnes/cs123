#include "ShapesScene.h"
#include "Camera.h"
#include <SupportCanvas3D.h>
#include <QFileDialog>
#include "shapes/Cube.h"
#include "shapes/Cone.h"
#include "shapes/Sphere.h"
#include "shapes/Cylinder.h"
#include "shapes/Torus.h"
#include "shapes/Ripple.h"
#include "shapes/RippleSphere.h"

glm::vec4 lightDirection = glm::normalize(glm::vec4(1.f, -1.f, -1.f, 0.f));

ShapesScene::ShapesScene()
{
    m_global.ka = 1.f;
    m_global.kd = 1.f;
    m_global.ks = 1.f;
    m_global.kt = 1.f;

    CS123ScenePrimitive *prim = new CS123ScenePrimitive();
    CS123SceneMaterial& mat = prim->material;

    prim->meshfile = "";
    prim->type = PRIMITIVE_CUBE;

    // Use a shiny orange material
    memset(&mat, 0, sizeof(CS123SceneMaterial));
    mat.cAmbient.r = 0.2f;
    mat.cAmbient.g = 0.1f;
    mat.cDiffuse.r = 1.0f;
    mat.cDiffuse.g = 0.5f;
    mat.cSpecular.r = mat.cSpecular.g = mat.cSpecular.b = 1;
    mat.shininess = 64;

    // Use snow texture
    mat.textureMap = new CS123SceneFileMap();
    mat.textureMap->filename = "/course/cs123/data/image/terrain/snow.JPG";
    mat.textureMap->isUsed = false;
    mat.textureMap->repeatU = 1;
    mat.textureMap->repeatV = 1;

    mat.bumpMap = new CS123SceneFileMap();
    mat.bumpMap->filename = "";
    mat.bumpMap->isUsed = false;
    mat.bumpMap->repeatU = 1;
    mat.bumpMap->repeatV = 1;
    mat.bumpMap->texid = 0;

    CS123SceneLightData *light = new CS123SceneLightData();
    // Use a white directional light from the upper left corner
    memset(light, 0, sizeof(CS123SceneLightData));
    light->type = LIGHT_DIRECTIONAL;
    light->dir = lightDirection;
    light->color.r = light->color.g = light->color.b = 1;
    light->id = 0;

    // Store old settings and set shape pointer
    m_shape = NULL;
    m_oldSettings = glm::vec4(
                settings.shapeType,
                settings.shapeParameter1,
                settings.shapeParameter2,
                settings.shapeParameter3
                );

    m_shapes.clear();
    m_lights.clear();
    m_trans.clear();

    m_shapes.append(prim);
    m_lights.append(light);
    m_trans.append(glm::mat4());


    m_initialized = false;
}

ShapesScene::~ShapesScene()
{
    if (m_square)
        delete m_square;
    if (m_ripplePlane)
        delete m_ripplePlane;
    if (m_rippleSphere)
        delete m_rippleSphere;
}


void ShapesScene::updateCurrentShape() {
    this->updateShape(m_shape);
}

void ShapesScene::init()
{
    if (m_initialized)
        return;

    OpenGLScene::init(); // Call the superclass's init()

    this->initShapes(settings.shapeParameter1,
                     settings.shapeParameter2,
                     settings.shapeParameter3);
    m_square = new Shape();
    m_ripplePlane = new Ripple(settings.shapeParameter1,
                               settings.shapeParameter2,
                               0.5f);
    m_rippleSphere = new RippleSphere(settings.shapeParameter1,
                                      settings.shapeParameter2,
                                      settings.shapeParameter3,
                                      0.5f);

    updateShapes();
    this->updateShape(m_square);
    this->updateShape(m_ripplePlane);
    this->updateShape(m_rippleSphere);

    this->setShape();

    CS123SceneMaterial& mat = m_shapes.at(0)->material;
    int texId = loadTexture(QString::fromStdString(mat.textureMap->filename));
    if (texId == -1) {
        cout << "Texture '" << mat.textureMap->filename << "' does not exist" << endl;
        mat.textureMap->isUsed = 0;
    } else {
        mat.textureMap->texid = texId;
        mat.textureMap->isUsed = true;
    }

    m_initialized = true;
}


void ShapesScene::setShape()
{
    switch (settings.shapeType) {
    case SHAPE_CUBE:
        m_shape = m_cube;
        m_shapes.at(0)->type = PRIMITIVE_CUBE;
        break;
    case SHAPE_CONE:
        m_shape = m_cone;
        m_shapes.at(0)->type = PRIMITIVE_CONE;
        break;
    case SHAPE_SPHERE:
        m_shape = m_sphere;
        m_shapes.at(0)->type = PRIMITIVE_SPHERE;
        break;
    case SHAPE_CYLINDER:
        m_shape = m_cylinder;
        m_shapes.at(0)->type = PRIMITIVE_CYLINDER;
        break;
    case SHAPE_TORUS:
        m_shape = m_torus;
        m_shapes.at(0)->type = PRIMITIVE_TORUS;
        break;
    case SHAPE_SPECIAL_1:
        m_shape = m_square;
        m_shapes.at(0)->type = PRIMITIVE_CUBE;
        break;
    case SHAPE_SPECIAL_2:
        m_shape = m_ripplePlane;
        m_shapes.at(0)->type = PRIMITIVE_CUBE;
        break;
    case SHAPE_SPECIAL_3:
        m_shape = m_rippleSphere;
        m_shapes.at(0)->type = PRIMITIVE_CUBE;
        break;
    default: // basic square shape
        m_shape = m_square;
        m_shapes.at(0)->type = PRIMITIVE_CUBE;
        break;
    }
    if (m_shape) {
        m_shape->setParam1(settings.shapeParameter1);
        m_shape->setParam2(settings.shapeParameter2);
        m_shape->setParam3(settings.shapeParameter3);
    }
}

/**
 * @brief ShapesScene::update - checks for parameter or shape changes and updates
 * the appropriate objects.
 */
void ShapesScene::update()
{
    if (m_oldSettings[0] != settings.shapeType) {

        m_oldSettings[0] = settings.shapeType;
        setShape();
        updateShape(m_shape);
    }
    else if (m_oldSettings[1] != settings.shapeParameter1 &&
             m_shape->usesParam(1)) {

        m_oldSettings[1] = settings.shapeParameter1;
        m_shape->setParam1(settings.shapeParameter1);
        this->updateShape(m_shape);

    }
    else if (m_oldSettings[2] != settings.shapeParameter2 &&
               m_shape->usesParam(2)) {

        m_oldSettings[2] = settings.shapeParameter2;
        m_shape->setParam2(settings.shapeParameter2);
        this->updateShape(m_shape);

    }
    else if (m_oldSettings[3] != settings.shapeParameter3 &&
               m_shape->usesParam(3)) {

        m_oldSettings[3] = settings.shapeParameter3;
        m_shape->setParam3(settings.shapeParameter3);
        this->updateShape(m_shape);
    }
}

void ShapesScene::renderGeometry()
{

    if (!m_initialized)
        return;

    applyMaterial(m_shapes.at(0)->material);

    // Draw the shape.
    if (m_shape)
        m_shape->render();

}


bool ShapesScene::animate()
{
    // return whether or not the shape is capable of animation
    if (m_shape)
        return m_shape->animate();
    return false;
}

void ShapesScene::setLights(const glm::mat4 viewMatrix)
{
    // YOU DON'T NEED TO TOUCH THIS METHOD, unless you want to do fancy lighting...

    CS123SceneLightData &light = *(m_lights.at(0));
    light.dir = glm::inverse(viewMatrix) * lightDirection;

    clearLights();
    setLight(light);
}

