#include "Shape.h"

Shape::Shape()
{
    m_p1 = 15;
    m_p2 = 15;
    m_radius = 0.5f;
    m_numVerts = 6;
    m_vertexData = NULL;
}

Shape::~Shape()
{
    cleanUp();
}


void Shape::setParam1(int param1)
{
    if (param1 == m_p1)
        return;
    m_p1 = param1;
}

void Shape::setParam2(int param2)
{
    if (param2 == m_p2)
        return;
    m_p2 = param2;
}


void Shape::calcVerts()
{
    int size = m_numVerts * 3;
    m_vertexData = new GLfloat[size];

    int index = 0;
    glm::vec3 norm = glm::vec3(0, 0, 1);

    addVertex(&index, glm::vec3(-1, -1, 0), norm);
    addVertex(&index, glm::vec3(1, -1, 0), norm);
    addVertex(&index, glm::vec3(0, 1, 0), norm);
}


void Shape::addVertex(int *i, glm::vec3 v, glm::vec3 norm)
{
    m_vertexData[(*i)++] = v.x;
    m_vertexData[(*i)++] = v.y;
    m_vertexData[(*i)++] = v.z;
    m_vertexData[(*i)++] = norm.x;
    m_vertexData[(*i)++] = norm.y;
    m_vertexData[(*i)++] = norm.z;
}


void Shape::updateGL(GLuint shader)
{
    // Initialize the vertex array object.
    glGenVertexArrays(1, &m_vaoID);
    glBindVertexArray(m_vaoID);

    // Initialize the vertex buffer object.
    GLuint vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

    glBufferData(GL_ARRAY_BUFFER, 3 * m_numVerts * sizeof(GLfloat), m_vertexData, GL_STATIC_DRAW);
    glEnableVertexAttribArray(glGetAttribLocation(shader, "position"));
    glVertexAttribPointer(
        glGetAttribLocation(shader, "position"),
        3,                   // Num coordinates per position
        GL_FLOAT,            // Type
        GL_FALSE,            // Normalized
        sizeof(GLfloat) * 6, // Stride
        (void*) 0            // Array buffer offset
    );
    glEnableVertexAttribArray(glGetAttribLocation(shader, "normal"));
    glVertexAttribPointer(
        glGetAttribLocation(shader, "normal"),
        3,                              // Num coordinates per normal
        GL_FLOAT,                       // Type
        GL_TRUE,                        // Normalized
        sizeof(GLfloat) * 6,            // Stride
        (void*) (sizeof(GLfloat) * 3)   // Array buffer offset
    );

    // Unbind buffers.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Shape::updateNormals(NormalRenderer *normRenderer)
{
    normRenderer->generateArrays(
                m_vertexData,             // Pointer to vertex data
                6 * sizeof(GLfloat),    // Stride (distance between consecutive vertices/normals in BYTES
                0,                      // Offset of first position in BYTES
                3 * sizeof(GLfloat),    // Offset of first normal in BYTES
                3);
}

void Shape::cleanUp()
{
    if (m_vertexData) {
        delete[] m_vertexData;
        m_vertexData = NULL;
    }
}


void Shape::render()
{
        glBindVertexArray(m_vaoID);
        glDrawArrays(GL_TRIANGLES, 0, m_numVerts / 2 /* Number of vertices to draw */);
        glBindVertexArray(0);
}


