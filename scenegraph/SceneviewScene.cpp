#include "SceneviewScene.h"
#include "GL/glew.h"
#include <qgl.h>
#include <QGLWidget>
#include <QFile>
#include "Camera.h"

SceneviewScene::SceneviewScene()
{
    // TODO: [SCENEVIEW] Set up anything you need for your Sceneview scene here...
    int param = 25;
    m_cone = new Cone(param, param, 0.5f, 0.5f);
    m_cube = new Cube(param, 0.5f);
    m_cylinder = new Cylinder(param, param, 0.5f, 0.5f);
    m_sphere = new Sphere(param, param, 0.5f);
    m_torus = new Torus(param, param, 25, 0.5);

    m_initialized = false;
}

SceneviewScene::~SceneviewScene()
{
    // TODO: [SCENEVIEW] Don't leak memory!
    delete m_cone;
    delete m_cube;
    delete m_cylinder;
    delete m_sphere;
    delete m_torus;
}


void SceneviewScene::updateShape(Shape *shape)
{
    shape->calcVerts();
    shape->updateGL(m_shader);
//    shape->updateNormals(m_normalRenderer);
    shape->cleanUp();
}


void SceneviewScene::init()
{
    OpenGLScene::init();

    updateShape(m_cone);
    updateShape(m_cube);
    updateShape(m_cylinder);
    updateShape(m_sphere);
    updateShape(m_torus);

    QString filename;
    int texId;
    int num_shapes = m_shapes.size();

    for (int i = 0; i < num_shapes; i++)
    {

        CS123SceneMaterial& mat = m_shapes.at(i)->material;
        filename = QString::fromStdString(mat.textureMap->filename);

        if (mat.textureMap->isUsed) {
            if (m_textures.contains(filename)) {
                mat.textureMap->texid = m_textures.value(filename);
            } else {
                texId = loadTexture(filename);
                if (texId == -1) {
                    cout << "Texture '" << mat.textureMap->filename << "' does not exist" << endl;
                    mat.textureMap->isUsed = 0;
                } else {
                    m_textures.insert(filename, texId);
                    mat.textureMap->texid = texId;
                }
            }
        }
    }

    m_initialized = true;
}

void SceneviewScene::setLights(const glm::mat4 viewMatrix)
{
    // TODO: [SCENEVIEW] Fill this in...
    //
    // Use function(s) inherited from OpenGLScene to set up the lighting for your scene.
    // The lighting information will most likely be stored in CS123SceneLightData structures.
    //
    CS123SceneLightData *light;

    int num_lights = m_lights.size();
    int i;

    for (i = 0; i < num_lights; i++) {
        light = m_lights.at(i);

        light->dir = glm::inverse(viewMatrix) * light->dir;
        setLight(*light);
    }
}

void SceneviewScene::renderGeometry()
{
    // TODO: [SCENEVIEW] Fill this in...
    //
    // This is where you should render the geometry of the scene. Use what you
    // know about OpenGL and leverage your Shapes classes to get the job done.
    //
    if (!m_initialized)
        return;

    int num_shapes = m_shapes.size();
    int i;

    for (i = 0; i < num_shapes; i++) {
        CS123ScenePrimitive *sp = m_shapes.at(i);

        applyMaterial(sp->material);
//        cout << glm::to_string(m_trans.at(i)).c_str() << endl;

        switch (sp->type) {
        case PRIMITIVE_CUBE:
//            cout << "Cube: ";
            m_cube->renderTransform(m_shader, m_trans.at(i));
            break;
        case PRIMITIVE_CONE:
//            cout << "Cone: ";
            m_cone->renderTransform(m_shader, m_trans.at(i));
            break;
        case PRIMITIVE_CYLINDER:
//            cout << "Cylinder: ";
            m_cylinder->renderTransform(m_shader, m_trans.at(i));
            break;
        case PRIMITIVE_SPHERE:
//            cout << "Sphere: ";
            m_sphere->renderTransform(m_shader, m_trans.at(i));
            break;
        case PRIMITIVE_TORUS:
//            cout << "Torus: ";
            m_torus->renderTransform(m_shader, m_trans.at(i));
            break;
        case PRIMITIVE_MESH:
            m_sphere->renderTransform(m_shader, m_trans.at(i));
            break;
        default:
            break;
        }
    }
}


void SceneviewScene::setSelection(int x, int y)
{
    // TODO: [MODELER LAB] Fill this in...
    //
    // Using m_selectionRecorder, set m_selectionIndex to the index in your
    // flattened parse tree of the object under the mouse pointer.  The
    // selection recorder will pick the object under the mouse pointer with
    // some help from you, all you have to do is:

    // 1) Set this to the number of objects you will be drawing.
    int numObjects = 0;

    // 2) Start the selection process
    m_selectionRecorder.enterSelectionMode(x, y, numObjects);

    // 3) Draw your objects, calling m_selectionRecorder.setObjectIndex() before drawing each one.

    // 4) Find out which object you selected, if any (-1 means no selection).
    m_selectionIndex = m_selectionRecorder.exitSelectionMode();
}


