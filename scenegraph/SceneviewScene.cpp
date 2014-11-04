#include "SceneviewScene.h"
#include "GL/glew.h"
#include <qgl.h>
#include <QGLWidget>
#include <QFile>
#include <QSet>
#include "Camera.h"

SceneviewScene::SceneviewScene()
{
    m_initialized = false;
}

SceneviewScene::~SceneviewScene()
{
    // ~Scene() will handle deleting m_shapes and m_lights

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


void SceneviewScene::init()
{
    OpenGLScene::init();

    int num_shapes = m_shapes.size();

    // adjust tessellation based on scene complexity
    if (num_shapes <= 1)
        initShapes(100, 100, 20.f);
    else if (num_shapes <= 50)
        initShapes(70, 70, 20.f);
    else if (num_shapes <= 100)
        initShapes(50, 50, 20.f);
    else if (num_shapes <= 200)
        initShapes(25, 25, 20.f);
    else if (num_shapes <= 1000)
        initShapes(15, 15, 20.f);
    else if (num_shapes <= 10000)
        initShapes(7, 7, 20.f);
    else
        initShapes(4, 4, 20.f);

    // set shape vertices and make gl calls
    updateShapes();

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

