#include "Scene.h"
#include "Camera.h"
#include "CS123SceneData.h"
#include "CS123ISceneParser.h"
#include <QtGlobal>
#include <QFile>
#include <QGLWidget>


Scene::Scene() //: m_camera(NULL)
{
}

Scene::~Scene()
{
    // Do not delete m_camera, it is owned by SupportCanvas3D

    // delete primitives
    int i;
    int num = m_shapes.size();
    for (i = 0; i < num; i++)
    {
        CS123ScenePrimitive *p = m_shapes.at(i);
        delete p->material.bumpMap;
        delete p->material.textureMap;
        delete p;
    }
    m_shapes.clear();

    // delete lights
    num = m_lights.size();
    for (i = 0; i < num; i++)
    {
        CS123SceneLightData *l = m_lights.at(i);
        delete l;
    }
    m_lights.clear();
}

void Scene::parse(Scene *sceneToFill, CS123ISceneParser *parser)
{
    // handle globals
    CS123SceneGlobalData data;
    parser->getGlobalData(data);
    sceneToFill->setGlobal(data);

    // handle light
    int light_num = parser->getNumLights();
    CS123SceneLightData lightData;
    for (int i = 0; i < light_num; i++)
    {
        parser->getLightData(i, lightData);
        sceneToFill->addLight(lightData);
    }

    // traverse tree to get primitives and transformations
    sceneToFill->nodecursion(sceneToFill, parser->getRootNode(), glm::mat4());
}

void Scene::nodecursion(Scene *scene, CS123SceneNode *node, glm::mat4 trans)
{
    // get transformation vector
    std::vector<CS123SceneTransformation*> transforms = node->transformations;
    int num_transforms = transforms.size();
    int i;

    // iterate through transformations and multiply with trans
    for (i = 0; i < num_transforms; i++) {

        CS123SceneTransformation *t = transforms[i];

        glm::vec3 v; // transformation coords
        float a, theta, phi; // rotation angles

        switch(t->type) {
        case TRANSFORMATION_TRANSLATE:
            v = t->translate;
            trans *= glm::mat4( 1,   0,   0,   0,
                                0,   1,   0,   0,
                                0,   0,   1,   0,
                               v.x, v.y, v.z,  1);
            break;
        case TRANSFORMATION_SCALE:
            v = t->scale;
            trans *= glm::mat4(v.x,  0,   0,   0,
                                0,  v.y,  0,   0,
                                0,   0,  v.z,  0,
                                0,   0,   0,   1);
            break;
        case TRANSFORMATION_ROTATE:
            v = t->rotate;
            a = t->angle;

            // angle between v and xy plane
            theta = atan2(v.z, v.x);
            theta = (theta > 0 ? theta : 2.f * M_PI + theta);

            // angle between v and xz plane
            phi = atan2(v.y, glm::distance(glm::vec2(), glm::vec2(v.x, v.z)));
            phi = -(phi > 0 ? phi : 2.f * M_PI + phi);

            // Rxz-1
            trans *= glm::mat4(cos(theta),   0,     sin(theta),    0,
                                   0,        1,         0,         0,
                              -sin(theta),   0,     cos(theta),    0,
                                   0,        0,         0,         1);
            // Rxy-1
            trans *= glm::mat4(cos(phi), -sin(phi),     0,         0,
                               sin(phi),  cos(phi),     0,         0,
                                  0,         0,         1,         0,
                                  0,         0,         0,         1);
            // Ryz
            trans *= glm::mat4(   1,         0,         0,         0,
                                  0,       cos(a),   sin(a),       0,
                                  0,      -sin(a),   cos(a),       0,
                                  0,         0,         0,         1);
            // Rxy
            trans *= glm::mat4(cos(phi),  sin(phi),     0,         0,
                              -sin(phi),  cos(phi),     0,         0,
                                  0,         0,         1,         0,
                                  0,         0,         0,         1);
            // Rxz
            trans *= glm::mat4(cos(theta),   0,   -sin(theta),     0,
                                  0,         1,         0,         0,
                              sin(theta),    0,    cos(theta),     0,
                                  0,         0,         0,         1);
            break;
        case TRANSFORMATION_MATRIX:
            // not sure what this does?
            break;
        default:
            break;
        }

    }

    // iterate through primitives
    std::vector<CS123ScenePrimitive*> prims = node->primitives;
    int num_prims = prims.size();
    for (i = 0; i < num_prims; i++) {
        CS123ScenePrimitive* prim = prims[i];
        scene->addPrimitive(*prim, trans);
    }

    // recur through all child nodes
    std::vector<CS123SceneNode*> kids = node->children;
    int num_kids = kids.size();
    for (i = 0; i < num_kids; i++) {
        CS123SceneNode* kid = kids[i];
        nodecursion(scene, kid, trans);
    }
}

void Scene::addPrimitive(const CS123ScenePrimitive &scenePrimitive, const glm::mat4 &matrix)
{
    // make new primitive
    CS123ScenePrimitive *sp = new CS123ScenePrimitive();

    // set type
    sp->type = scenePrimitive.type;

    // set filename
    sp->meshfile = scenePrimitive.meshfile;

    // set material
    CS123SceneMaterial material = scenePrimitive.material;

    CS123SceneMaterial& mat = sp->material;
    memset(&mat, 0, sizeof(CS123SceneMaterial));
    mat.textureMap = new CS123SceneFileMap();
    mat.bumpMap = new CS123SceneFileMap();

    copyFileMap(mat.textureMap, material.textureMap);
    copyFileMap(mat.bumpMap, material.bumpMap);

    setColor(&mat.cAmbient, &material.cAmbient, m_global.ka);
    setColor(&mat.cDiffuse, &material.cDiffuse, m_global.kd);
    setColor(&mat.cSpecular, &material.cSpecular, m_global.ks);
    setColor(&mat.cTransparent, &material.cTransparent, m_global.kt);

    mat.blend = material.blend;
    mat.ior = material.ior;
    mat.shininess = material.shininess;

    // add primitive and transformation to lists
    m_shapes.append(sp);
    m_trans.append(matrix);
}

void Scene::addLight(const CS123SceneLightData &sceneLight)
{
    // make new light
    CS123SceneLightData *light = new CS123SceneLightData();
    memset(light, 0, sizeof(CS123SceneLightData));

    // set light data
    light->angle = sceneLight.angle;
    light->color = sceneLight.color;
    light->dir = sceneLight.dir;
    light->function = sceneLight.function;
    light->height = sceneLight.height;
    light->id = sceneLight.id;
    light->penumbra = sceneLight.penumbra;
    light->pos = sceneLight.pos;
    light->radius = sceneLight.radius;
    light->type = sceneLight.type;

    // add light to list
    m_lights.append(light);
}

void Scene::setGlobal(const CS123SceneGlobalData &global)
{
    m_global = global;
}


void Scene::copyFileMap(CS123SceneFileMap *fm1, CS123SceneFileMap *fm2)
{
    fm1->filename = fm2->filename;
    fm1->isUsed = fm2->isUsed;
    fm1->repeatU = fm2->repeatU;
    fm1->repeatV = fm2->repeatV;
    fm1->texid = fm2->texid;
}


void Scene::setColor(CS123SceneColor *dest, CS123SceneColor *src, float constant)
{
    dest->r = src->r * constant;
    dest->g = src->g * constant;
    dest->b = src->b * constant;
}



// Copied from lab04
int Scene::loadTexture(const QString &filename)
{
    // make sure file exists
    QFile file(filename);
    if (!file.exists())
        return -1;

    // load file into memory
    QImage image;
    image.load(file.fileName());
    image = image.mirrored(false, true);
    QImage texture = QGLWidget::convertToGLFormat(image);

    // generate texture ID
    GLuint id = 0;
    glGenTextures(1, &id);

    // make the texture
    glBindTexture(GL_TEXTURE_2D, id);

    // copy image data into texture
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, texture.width(), texture.height(), GL_RGBA, GL_UNSIGNED_BYTE, texture.bits());

    // filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // coordinate wrapping options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    return id;
}


int Scene::getNumShapes()
{
    return m_shapes.size();
}


int Scene::getNumLights()
{
    return m_lights.size();
}


CS123SceneGlobalData Scene::getGlobalData()
{
    return m_global;
}


CS123ScenePrimitive* Scene::getPrimitive(int i)
{
    if (i < 0 || (unsigned int)i >= m_shapes.size())
    {
        cout << "invalid light index %d" << endl;
        return NULL;
    }
    return m_shapes[i];
}


glm::mat4 Scene::getMatrix(int i)
{
    if (i < 0 || (unsigned int)i >= m_trans.size())
    {
        cout << "invalid light index %d" << endl;
        return glm::mat4();
    }
    return m_trans[i];
}


CS123SceneLightData* Scene::getLightData(int i)
{
    if (i < 0 || (unsigned int)i >= m_lights.size())
    {
        cout << "invalid light index %d" << endl;
        return NULL;
    }
    return m_lights[i];
}

