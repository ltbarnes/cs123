#ifndef SHAPE_H
#define SHAPE_H

#include "OpenGLScene.h"

class Shape
{
public:
    Shape();
    virtual ~Shape();

    void setParam1(int param1);
    void setParam2(int param2);
    void updateGL(GLuint shader);
    void updateNormals(NormalRenderer *normRenderer);
    void cleanUp();

    virtual void calcVerts();
    virtual void render();

protected:

    void addVertex(int *i, glm::vec3 v, glm::vec3 norm);

    GLuint m_vaoID; // The vertex array object ID, which is passed to glBindVertexArray.
    GLfloat *m_vertexData;

    int m_p1;
    int m_p2;
    float m_radius;
    int m_numVerts;
};

#endif // SHAPE_H
