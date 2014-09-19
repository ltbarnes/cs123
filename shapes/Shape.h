#ifndef SHAPE_H
#define SHAPE_H

#include "OpenGLScene.h"

struct Rect {
    glm::vec3 tr, trNorm;
    glm::vec3 tl, tlNorm;
    glm::vec3 bl, blNorm;
    glm::vec3 br, brNorm;
};

class Shape
{
public:
    Shape();
    virtual ~Shape();

    void setParam1(int param1);
    void setParam2(int param2);
    void setParam3(float param3);

    void updateGL(GLuint shader);
    void updateNormals(NormalRenderer *normRenderer);
    void cleanUp();

    virtual bool usesParam(int num);
    virtual void calcVerts();
    void render();

protected:

    void addVertex(int *i, glm::vec3 v, glm::vec3 norm);
    void makeRect(int *i, struct Rect *rect);

    void printVert(glm::vec3 v);
    void setParamMax(int p1, int p2, float p3);

    GLuint m_vaoID; // The vertex array object ID, which is passed to glBindVertexArray.
    GLfloat *m_vertexData;

    int m_p1;
    int m_p2;
    float m_p3;

    int m_numVerts;

private:
    int m_p1max;
    int m_p2max;
    float m_p3max;
};

#endif // SHAPE_H
