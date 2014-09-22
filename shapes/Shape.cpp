#include "Shape.h"

Shape::Shape()
{
    m_numVerts = 6;
    m_vertexData = NULL;
    m_vaoID = 0;

    setParamMax(-1, -1, -1.f);
}

Shape::~Shape()
{
    cleanUp();
    glDeleteVertexArrays(1, &m_vaoID);
}


void Shape::setParamMax(int p1, int p2, float p3)
{
    m_p1max = p1;
    m_p2max = p2;
    m_p3max = p3;
}

void Shape::setParam1(int param1)
{
    m_p1 = param1;
    if (m_p1 < m_p1max)
        m_p1 = m_p1max;
}

void Shape::setParam2(int param2)
{
    m_p2 = param2;
    if (m_p2 < m_p2max)
        m_p2 = m_p2max;
}

void Shape::setParam3(float param3)
{
    m_p3 = param3;
    if (m_p3 < m_p3max)
        m_p3 = m_p3max;
}

bool Shape::usesParam(int num)
{
    if (num == 1)
        return m_p1max > -1;
    if (num == 2)
        return m_p2max > -1;
    if (num == 3)
        return m_p3max > -1.f;
    return false;
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


void Shape::updateGL(GLuint shader)
{
    if (m_vaoID)
        glDeleteVertexArrays(1, &m_vaoID);

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

    // Clean up
    glDeleteBuffers(1, &vertexBuffer);
}

void Shape::updateNormals(NormalRenderer *normRenderer)
{
    normRenderer->generateArrays(
                m_vertexData,             // Pointer to vertex data
                6 * sizeof(GLfloat),    // Stride (distance between consecutive vertices/normals in BYTES
                0,                      // Offset of first position in BYTES
                3 * sizeof(GLfloat),    // Offset of first normal in BYTES
                m_numVerts / 2);
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


void Shape::makeRect(int *i, Rect *rect)
{
    addVertex(i, rect->tr, rect->trNorm);
    addVertex(i, rect->tl, rect->tlNorm);
    addVertex(i, rect->bl, rect->blNorm);
    addVertex(i, rect->bl, rect->blNorm);
    addVertex(i, rect->br, rect->brNorm);
    addVertex(i, rect->tr, rect->trNorm);
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


void Shape::printVert(glm::vec3 v)
{
    cout << "(" << v.x;
    cout << ", " << v.y;
    cout << ", " << v.z;
    cout << ")" << endl;
}


