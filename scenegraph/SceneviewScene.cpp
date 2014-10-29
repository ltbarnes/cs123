#include "SceneviewScene.h"
#include "GL/glew.h"
#include <qgl.h>
#include <QGLWidget>
#include <QFile>
#include <QSet>
#include "Camera.h"

SceneviewScene::SceneviewScene()
{
    int param = 3; // will be changed later based on scene complexity

    // create shapes
    m_cone = new Cone(param, param, 0.5f, 0.5f);
    m_cube = new Cube(param, 0.5f);
    m_cylinder = new Cylinder(param, param, 0.5f, 0.5f);
    m_sphere = new Sphere(param, param, 0.5f);
    m_torus = new Torus(param, param, 25, 0.5);

    m_initialized = false;
}

SceneviewScene::~SceneviewScene()
{
    // ~Scene() will handle deleting m_shapes and m_lights

    // delete shapes
    delete m_cone;
    delete m_cube;
    delete m_cylinder;
    delete m_sphere;
    delete m_torus;

    // delete textures
    QSet<GLuint> textureIDs;
    GLuint id;
    int num_shapes = m_shapes.size();

    // iterate through materials
    for (int i = 0; i < num_shapes; i++) {
        CS123SceneMaterial& mat = m_shapes.at(i)->material;

        // if the texture id was assigned
        if (mat.textureMap->isUsed) {
            id = mat.textureMap->texid;

            // if the texture hasn't been deleted yet
            if (!textureIDs.contains(id)) {
                glDeleteTextures(1, &id);
                textureIDs.insert(id);
            }
        }
    }
    // done with texture ids
    textureIDs.clear();
}


void SceneviewScene::setShapeParams(int p1, int p2, float p3)
{
    m_cone->setParam1(p1);
    m_cone->setParam2(p2);
    m_cone->setParam3(p3);

    m_cube->setParam1(p1);
    m_cube->setParam2(p2);
    m_cube->setParam3(p3);

    m_cylinder->setParam1(p1);
    m_cylinder->setParam2(p2);
    m_cylinder->setParam3(p3);

    m_sphere->setParam1(p1);
    m_sphere->setParam2(p2);
    m_sphere->setParam3(p3);

    m_torus->setParam1(p1);
    m_torus->setParam2(p2);
    m_torus->setParam3(p3);
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

    int num_shapes = m_shapes.size();

    // adjust tessellation based on scene complexity
    if (num_shapes <= 1)
        setShapeParams(100, 100, 20);
    else if (num_shapes <= 50)
        setShapeParams(70, 70, 20);
    else if (num_shapes <= 100)
        setShapeParams(50, 50, 20);
    else if (num_shapes <= 200)
        setShapeParams(25, 25, 20);
    else if (num_shapes <= 1000)
        setShapeParams(15, 15, 20);
    else if (num_shapes <= 10000)
        setShapeParams(7, 7, 20);
    else
        setShapeParams(3, 3, 20);

    // set shape vertices and make gl calls
    updateShape(m_cone);
    updateShape(m_cube);
    updateShape(m_cylinder);
    updateShape(m_sphere);
    updateShape(m_torus);

    // load and set texture ids
    QString filename;
    int texId;
    // use hash table so different textures are only loaded once
    QHash<QString, int> textures;

    // iterate through materials
    for (int i = 0; i < num_shapes; i++)
    {

        CS123SceneMaterial& mat = m_shapes.at(i)->material;

        // if there is a filename
        if (mat.textureMap->isUsed) {
            filename = QString::fromStdString(mat.textureMap->filename);

            // if the texture was already loaded
            if (textures.contains(filename)) {
                if (textures.value(filename) != -1)
                    mat.textureMap->texid = textures.value(filename);
                else
                    mat.textureMap->isUsed = 0;

            } else { // if the texture hasn't been loaded
                texId = loadTexture(filename);
                if (texId == -1) {
                    cout << "Texture '" << mat.textureMap->filename << "' does not exist" << endl;
                    mat.textureMap->isUsed = 0;
                } else {
                    mat.textureMap->texid = texId;
                }
                 // add texture id to the hash table
                textures.insert(filename, texId);
            }
        }
    }
    // done with texture hash table
    textures.clear();

    m_initialized = true;
}

void SceneviewScene::setLights(const glm::mat4 viewMatrix)
{
    CS123SceneLightData *light;

    int num_lights = m_lights.size();
    int i;

    for (i = 0; i < num_lights; i++) {
        light = m_lights.at(i);
        setLight(*light);
    }
}

void SceneviewScene::renderGeometry()
{
    if (!m_initialized)
        return;

    int num_shapes = m_shapes.size();
    int i;

    // iterate through shapes
    for (i = 0; i < num_shapes; i++) {
        CS123ScenePrimitive *sp = m_shapes.at(i);

        applyMaterial(sp->material);

        // render shape based on corresponding transformation
        switch (sp->type) {
        case PRIMITIVE_CUBE:
            m_cube->transformAndRender(m_shader, m_trans.at(i));
            break;
        case PRIMITIVE_CONE:
            m_cone->transformAndRender(m_shader, m_trans.at(i));
            break;
        case PRIMITIVE_CYLINDER:
            m_cylinder->transformAndRender(m_shader, m_trans.at(i));
            break;
        case PRIMITIVE_SPHERE:
            m_sphere->transformAndRender(m_shader, m_trans.at(i));
            break;
        case PRIMITIVE_TORUS:
            m_torus->transformAndRender(m_shader, m_trans.at(i));
            break;
        case PRIMITIVE_MESH:
            m_sphere->transformAndRender(m_shader, m_trans.at(i));
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

