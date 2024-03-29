#include "OpenGLScene.h"
#include "Camera.h"
#include "ResourceLoader.h"
#include "Settings.h"
#include "SupportCanvas3D.h"
#include <string>
#include <sstream>
#include "QCoreApplication"
#include "shapes/Cone.h"
#include "shapes/Cube.h"
#include "shapes/Cylinder.h"
#include "shapes/Sphere.h"
#include "shapes/Torus.h"

#define SHAPE_RADIUS 0.5f

OpenGLScene::OpenGLScene()
{
    m_cone = NULL;
    m_cube = NULL;
    m_cylinder = NULL;
    m_sphere = NULL;
    m_torus = NULL;

    m_initializedShapes = false;

    m_normalRenderer = NULL;
}

OpenGLScene::~OpenGLScene()
{
    // delete shapes
    if (m_initializedShapes) {
        delete m_cone;
        delete m_cube;
        delete m_cylinder;
        delete m_sphere;
        delete m_torus;
    }

    if (m_normalRenderer)
        delete m_normalRenderer;
}


bool OpenGLScene::isInit()
{
    return m_initialized;
}


void OpenGLScene::initShapes(int p1, int p2, float p3)
{
    if (!m_initializedShapes) {
        m_cone = new Cone(p1, p2, SHAPE_RADIUS, SHAPE_RADIUS);
        m_cube = new Cube(p1, SHAPE_RADIUS);
        m_cylinder = new Cylinder(p1, p2, SHAPE_RADIUS, SHAPE_RADIUS);
        m_sphere = new Sphere(p1, p2, SHAPE_RADIUS);
        m_torus = new Torus(p1, p2, p3, SHAPE_RADIUS);

        m_initializedShapes = true;
    }
}


void OpenGLScene::setShapeParams(int p1, int p2, float p3)
{
    if (m_initializedShapes) {
        m_cone->setParam1(p1);
        m_cone->setParam2(p2);

        m_cube->setParam1(p1);

        m_cylinder->setParam1(p1);
        m_cylinder->setParam2(p2);

        m_sphere->setParam1(p1);
        m_sphere->setParam2(p2);

        m_torus->setParam1(p1);
        m_torus->setParam2(p2);
        m_torus->setParam3(p3);
    }
}


void OpenGLScene::updateShape(Shape *shape)
{
    if (shape) {
        shape->calcVerts();
        shape->updateGL(m_shader);
        shape->updateNormals(m_normalRenderer);
        shape->cleanUp();
    }
}


void OpenGLScene::updateShapes()
{
    updateShape(m_cone);
    updateShape(m_cube);
    updateShape(m_cylinder);
    updateShape(m_sphere);
    updateShape(m_torus);
}

void OpenGLScene::init()
{
    m_shader = ResourceLoader::loadShaders(
            ":/shaders/default.vert",
            ":/shaders/default.frag");
    if (m_normalRenderer)
        delete m_normalRenderer;
    m_normalRenderer = new NormalRenderer(m_shader);

    m_uniformLocs["p"]= glGetUniformLocation(m_shader, "p");
    m_uniformLocs["m"]= glGetUniformLocation(m_shader, "m");
    m_uniformLocs["v"]= glGetUniformLocation(m_shader, "v");
    m_uniformLocs["allBlack"]= glGetUniformLocation(m_shader, "allBlack");
    m_uniformLocs["useLighting"]= glGetUniformLocation(m_shader, "useLighting");
    m_uniformLocs["ambient_color"] = glGetUniformLocation(m_shader, "ambient_color");
    m_uniformLocs["diffuse_color"] = glGetUniformLocation(m_shader, "diffuse_color");
    m_uniformLocs["specular_color"] = glGetUniformLocation(m_shader, "specular_color");
    m_uniformLocs["shininess"] = glGetUniformLocation(m_shader, "shininess");
    m_uniformLocs["useTexture"] = glGetUniformLocation(m_shader, "useTexture");
    m_uniformLocs["tex"] = glGetUniformLocation(m_shader, "tex");
    m_uniformLocs["useArrowOffsets"] = glGetUniformLocation(m_shader, "useArrowOffsets");
    m_uniformLocs["blend"] = glGetUniformLocation(m_shader, "blend");
    m_uniformLocs["repeatU"] = glGetUniformLocation(m_shader, "repeatU");
    m_uniformLocs["repeatV"] = glGetUniformLocation(m_shader, "repeatV");
}

void OpenGLScene::render(SupportCanvas3D *context)
{
    // Clear the screen in preparation for the next frame. (Use a gray background instead of a
    // black one for drawing wireframe or normals so they will show up against the background.)
    if (settings.drawWireframe || settings.drawNormals) glClearColor(0.5f, 0.5f, 0.5f, 0.5f);
    else glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Get the active camera
    Camera *camera = context->getCamera();
    assert(camera);
    glm::mat4 viewMatrix = camera->getViewMatrix();

    glUseProgram(m_shader);

    // Set scene uniforms.
    clearLights();
    setLights(viewMatrix);
    glUniform1i(m_uniformLocs["useLighting"], settings.useLighting);
    glUniform1i(m_uniformLocs["useArrowOffsets"], GL_FALSE);
    glUniformMatrix4fv(m_uniformLocs["p"], 1, GL_FALSE,
            glm::value_ptr(camera->getProjectionMatrix()));
    glUniformMatrix4fv(m_uniformLocs["v"], 1, GL_FALSE,
            glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(m_uniformLocs["m"], 1, GL_FALSE,
            glm::value_ptr(glm::mat4()));
    glUniform3f(m_uniformLocs["allBlack"], 1, 1, 1);


    renderGeometry();

    if (settings.drawWireframe)
    {
        glUniform3f(m_uniformLocs["allBlack"], 0, 0, 0);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        renderGeometry();

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    // Render normals if specified.
    if (settings.drawNormals)
    {
        glUseProgram(m_shader);
        glUniform3f(m_uniformLocs["allBlack"], 0, 0, 0);
        this->renderNormals();
    }

    glUseProgram(0);
}

void OpenGLScene::renderNormals()
{
    m_normalRenderer->draw();
}

void OpenGLScene::applyMaterial(const CS123SceneMaterial &material)
{
    // Make sure the members of CS123SceneColor are packed tightly
    COMPILE_TIME_ASSERT(sizeof(CS123SceneColor) == sizeof(float) * 4);

    glUniform3fv(m_uniformLocs["ambient_color"], 1, &material.cAmbient.r);
    glUniform3fv(m_uniformLocs["diffuse_color"], 1, &material.cDiffuse.r);
    glUniform3fv(m_uniformLocs["specular_color"], 1, &material.cSpecular.r);
    glUniform1f(m_uniformLocs["shininess"], material.shininess);
    if (material.textureMap && material.textureMap->isUsed && material.textureMap->texid) {
        glUniform1i(m_uniformLocs["useTexture"], 1);
        glUniform1i(m_uniformLocs["tex"], 1);
        glUniform1f(m_uniformLocs["blend"], material.blend);
        glUniform1f(m_uniformLocs["repeatU"], material.textureMap->repeatU);
        glUniform1f(m_uniformLocs["repeatV"], material.textureMap->repeatV);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, material.textureMap->texid);
        glActiveTexture(GL_TEXTURE0);
    } else {
        glUniform1i(m_uniformLocs["useTexture"], 0);
    }
}

void OpenGLScene::clearLights()
{
    for (int i = 0; i < MAX_NUM_LIGHTS; i++) {
        std::ostringstream os;
        os << i;
        std::string indexString = "[" + os.str() + "]"; // e.g. [0], [1], etc.
        glUniform3f(glGetUniformLocation(m_shader, ("lightColors" + indexString).c_str()), 0, 0, 0);
    }
}

void OpenGLScene::setLight(const CS123SceneLightData &light)
{
    std::ostringstream os;
    os << light.id;
    std::string indexString = "[" + os.str() + "]"; // e.g. [0], [1], etc.

    bool ignoreLight = false;

    GLint lightType;
    switch(light.type)
    {
    case LIGHT_POINT:
        lightType = 0;
        glUniform3fv(glGetUniformLocation(m_shader, ("lightPositions" + indexString).c_str()), 1,
                glm::value_ptr(light.pos));
        if (!settings.usePointLights) ignoreLight = true;
        break;
    case LIGHT_DIRECTIONAL:
        lightType = 1;
        glUniform3fv(glGetUniformLocation(m_shader, ("lightDirections" + indexString).c_str()), 1,
                glm::value_ptr(glm::normalize(light.dir)));
        if (!settings.useDirectionalLights) ignoreLight = true;
        break;
    default:
        lightType = -1;
        ignoreLight = true; // Light type not supported
        break;
    }

    CS123SceneColor color = light.color;
    if (ignoreLight) color.r = color.g = color.b = 0;

    glUniform1i(glGetUniformLocation(m_shader, ("lightTypes" + indexString).c_str()), lightType);
    glUniform3f(glGetUniformLocation(m_shader, ("lightColors" + indexString).c_str()),
                color.r, color.g, color.b);
    glUniform3f(glGetUniformLocation(m_shader, ("lightAttenuations" + indexString).c_str()),
            light.function.x, light.function.y, light.function.z);
}
