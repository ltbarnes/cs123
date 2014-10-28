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

#define SHAPE_RADIUS 0.5f

glm::vec4 lightDirection = glm::normalize(glm::vec4(1.f, -1.f, -1.f, 0.f));

ShapesScene::ShapesScene()
{
    // Use a shiny orange material
    memset(&m_material, 0, sizeof(m_material));
    m_material.cAmbient.r = 0.2f;
    m_material.cAmbient.g = 0.1f;
    m_material.cDiffuse.r = 1.0f;
    m_material.cDiffuse.g = 0.5f;
    m_material.cSpecular.r = m_material.cSpecular.g = m_material.cSpecular.b = 1;
    m_material.shininess = 64;

    // Use snow texture
    m_material.textureMap = new CS123SceneFileMap();
    m_material.textureMap->filename = "/course/cs123/data/image/terrain/snow.JPG";
    m_material.textureMap->isUsed = 1;
    m_material.textureMap->repeatU = 1;
    m_material.textureMap->repeatV = 1;

    // Use a white directional light from the upper left corner
    memset(&m_light, 0, sizeof(m_light));
    m_light.type = LIGHT_DIRECTIONAL;
    m_light.dir = lightDirection;
    m_light.color.r = m_light.color.g = m_light.color.b = 1;
    m_light.id = 0;

    // Store old settings and set shape pointer
    m_shape = NULL;
    m_oldSettings = glm::vec4(
                settings.shapeType,
                settings.shapeParameter1,
                settings.shapeParameter2,
                settings.shapeParameter3
                );

    m_initialized = false;
}

ShapesScene::~ShapesScene()
{
    // delete texture
    if (m_material.textureMap->isUsed) {
        GLuint id = m_material.textureMap->texid;
        glDeleteTextures(1, &id);
    }

    // delete texture map and shape
    delete m_material.textureMap;
    if (m_shape)
        delete m_shape;
}

void ShapesScene::init()
{

    OpenGLScene::init(); // Call the superclass's init()

    this->setShape();
    this->updateShape();

    int texId = loadTexture(QString::fromStdString(m_material.textureMap->filename));
    if (texId == -1) {
        cout << "Texture '" << m_material.textureMap->filename << "' does not exist" << endl;
        m_material.textureMap->isUsed = 0;
    } else
        m_material.textureMap->texid = texId;

    m_initialized = true;
}


void ShapesScene::setShape()
{
    if (m_shape) {
        delete m_shape;
        m_shape = NULL;
    }


    switch (settings.shapeType) {
    case SHAPE_CUBE:
        m_shape = new Cube(settings.shapeParameter1, SHAPE_RADIUS);
        break;
    case SHAPE_CONE:
        m_shape = new Cone(settings.shapeParameter1, settings.shapeParameter2, SHAPE_RADIUS, SHAPE_RADIUS);
        break;
    case SHAPE_SPHERE:
        m_shape = new Sphere(settings.shapeParameter1, settings.shapeParameter2, SHAPE_RADIUS);
        break;
    case SHAPE_CYLINDER:
        m_shape = new Cylinder(settings.shapeParameter1, settings.shapeParameter2, SHAPE_RADIUS, SHAPE_RADIUS);
        break;
    case SHAPE_TORUS:
        m_shape = new Torus(settings.shapeParameter1, settings.shapeParameter2, settings.shapeParameter3, SHAPE_RADIUS);
        break;
    case SHAPE_SPECIAL_1:
        m_shape = new Shape();
        break;
    case SHAPE_SPECIAL_2:
        m_shape = new Ripple(settings.shapeParameter1, settings.shapeParameter2, SHAPE_RADIUS);
        break;
    case SHAPE_SPECIAL_3:
        m_shape = new RippleSphere(settings.shapeParameter1, settings.shapeParameter2, settings.shapeParameter3, SHAPE_RADIUS);
        break;
    default: // basic square shape
        m_shape = new Shape();
        break;
    }
}


void ShapesScene::updateShape()
{
    if (!m_shape)
        return;
    m_shape->calcVerts();
    m_shape->updateGL(m_shader);
    m_shape->updateNormals(m_normalRenderer);
    m_shape->cleanUp();
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
        updateShape();
    }
    else if (m_oldSettings[1] != settings.shapeParameter1 &&
             m_shape->usesParam(1)) {

        m_oldSettings[1] = settings.shapeParameter1;
        m_shape->setParam1(settings.shapeParameter1);
        this->updateShape();

    }
    else if (m_oldSettings[2] != settings.shapeParameter2 &&
               m_shape->usesParam(2)) {

        m_oldSettings[2] = settings.shapeParameter2;
        m_shape->setParam2(settings.shapeParameter2);
        this->updateShape();

    }
    else if (m_oldSettings[3] != settings.shapeParameter3 &&
               m_shape->usesParam(3)) {

        m_oldSettings[3] = settings.shapeParameter3;
        m_shape->setParam3(settings.shapeParameter3);
        this->updateShape();
    }
}

void ShapesScene::renderGeometry()
{

    if (!m_initialized)
        return;

    applyMaterial(m_material);

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

    m_light.dir = glm::inverse(viewMatrix) * lightDirection;

    clearLights();
    setLight(m_light);
}

