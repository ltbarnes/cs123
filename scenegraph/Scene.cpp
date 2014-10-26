#include "Scene.h"
#include "Camera.h"
#include "CS123SceneData.h"
#include "CS123ISceneParser.h"
#include <QtGlobal>


Scene::Scene() //: m_camera(NULL)
{

}

Scene::~Scene()
{
    // Do not delete m_camera, it is owned by SupportCanvas3D

}

void Scene::parse(Scene *sceneToFill, CS123ISceneParser *parser)
{
    // TODO: load scene into sceneToFill using setGlobal(), addLight(), addPrimitive(), and
    //   finishParsing()
    CS123SceneGlobalData data;
    parser->getGlobalData(data);
    sceneToFill->setGlobal(data);

    int light_num = parser->getNumLights();
    for (int i = 0; i < light_num; i++) {
        CS123SceneLightData lightData;
        parser->getLightData(i, lightData);
        sceneToFill->addLight(lightData);
    }

    sceneToFill->nodecursion(sceneToFill, parser->getRootNode(), glm::mat4());
}

void Scene::nodecursion(Scene *scene, CS123SceneNode *node, glm::mat4 trans)
{
    std::vector<CS123SceneTransformation*> transforms = node->transformations;
    int num_transforms = transforms.size();
    int i;

    for (i = 0; i < num_transforms; i++) {

        CS123SceneTransformation *t = transforms[i];

        glm::vec3 v;
        float a, theta, phi;

        switch(t->type) {
        case TRANSFORMATION_TRANSLATE:
            v = t->translate;
            trans *= glm::mat4(1, 0, 0, 0,
                               0, 1, 0, 0,
                               0, 0, 1, 0,
                               v.x, v.y, v.z, 1);
            break;
        case TRANSFORMATION_SCALE:
            v = t->scale;
            trans *= glm::mat4(v.x, 0, 0, 0,
                               0, v.y, 0, 0,
                               0, 0, v.z, 0,
                               0, 0, 0, 1);
            break;
        case TRANSFORMATION_ROTATE:
            v = t->rotate;
            a = t->angle;
            theta = atan(v.z / v.x);
            phi = -atan(v.y / glm::distance(glm::vec2(), glm::vec2(v.x, v.z)));

            trans *= glm::mat4(cos(theta), 0, sin(theta), 0,
                               0, 1, 0, 0,
                              -sin(theta), 0, cos(theta), 0,
                               0, 0, 0, 1);
            trans *= glm::mat4(cos(phi), -sin(phi), 0, 0,
                               sin(phi), cos(phi), 0, 0,
                               0, 0, 1, 0,
                               0, 0, 0, 1);
            trans *= glm::mat4(1, 0, 0, 0,
                               0, cos(a), sin(a), 0,
                               0, -sin(a), cos(a), 0,
                               0, 0, 0, 1);
            trans *= glm::mat4(cos(phi), sin(phi), 0, 0,
                              -sin(phi), cos(phi), 0, 0,
                               0, 0, 1, 0,
                               0, 0, 0, 1);
            trans *= glm::mat4(cos(theta), 0, -sin(theta), 0,
                              0, 1, 0, 0,
                              sin(theta), 0, cos(theta), 0,
                              0, 0, 0, 1);
//            cout << "TRANS" << endl;
//            cout << glm::to_string(t->matrix).c_str() << endl;
            break;
        case TRANSFORMATION_MATRIX:
            cout << "Matrix" << endl;
            break;
        default:
            break;
        }

    }

    std::vector<CS123ScenePrimitive*> prims = node->primitives;
    int num_prims = prims.size();
    for (i = 0; i < num_prims; i++) {
        CS123ScenePrimitive* prim = prims[i];
        scene->addPrimitive(*prim, trans);
    }

    std::vector<CS123SceneNode*> kids = node->children;
    int num_kids = kids.size();
    for (i = 0; i < num_kids; i++) {
        CS123SceneNode* kid = kids[i];
        nodecursion(scene, kid, trans);
    }
}

void Scene::addPrimitive(const CS123ScenePrimitive &scenePrimitive, const glm::mat4x4 &matrix)
{
    CS123ScenePrimitive sp = scenePrimitive;

    CS123SceneColor *ca = &(sp.material.cAmbient);
    ca->r *= m_global.ka;
    ca->g *= m_global.ka;
    ca->b *= m_global.ka;

    CS123SceneColor *cd = &(sp.material.cDiffuse);
    cd->r *= m_global.kd;
    cd->g *= m_global.kd;
    cd->b *= m_global.kd;

    m_shapes.append(sp);
    m_trans.append(matrix);

}

void Scene::addLight(const CS123SceneLightData &sceneLight)
{
    CS123SceneLightData light = sceneLight;
    m_lights.append(light);
}

void Scene::setGlobal(const CS123SceneGlobalData &global)
{
    m_global = global;
}

