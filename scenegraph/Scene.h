#ifndef SCENE_H
#define SCENE_H

#include <CS123Common.h>
#include "CS123SceneData.h"

class Camera;
class CS123ISceneParser;

struct SceneElement
{
    CS123ScenePrimitive *primitive;
    glm::mat4 trans;
    glm::mat4 inv;
};

/**
 * @class Scene
 *
 * @brief This is the base class for all scenes. Modify this class if you want to provide
 * common functionality to all your scenes.
 */
class Scene
{
public:
    Scene();
    virtual ~Scene();

    static void parse(Scene *sceneToFill, CS123ISceneParser *parser);

    virtual CS123SceneGlobalData getGlobalData();

    virtual CS123ScenePrimitive *getPrimitive(int i);

    virtual glm::mat4 getMatrix(int i);

    virtual CS123SceneLightData *getLightData(int i);

    virtual int getNumElements();

    virtual int getNumLights();

protected:

    // Adds a primitive to the scene.
    virtual void addPrimitive(const CS123ScenePrimitive &scenePrimitive, const glm::mat4 &matrix, bool useCoeff);

    // Adds a light to the scene.
    virtual void addLight(const CS123SceneLightData &sceneLight);

    // Sets the global data for the scene.
    virtual void setGlobal(const CS123SceneGlobalData &global);

    int loadTexture(const QString &filename);

    CS123SceneGlobalData m_global;
    QList<CS123SceneLightData*> m_lights;
    QList<SceneElement *> m_elements;

private:
    void nodecursion(Scene *scene, CS123SceneNode *node, glm::mat4 trans);
    void copyFileMap(CS123SceneFileMap *fm1, CS123SceneFileMap *fm2);
    void setColor(CS123SceneColor *dest, CS123SceneColor *src, float constant);
};

#endif // SCENE_H
