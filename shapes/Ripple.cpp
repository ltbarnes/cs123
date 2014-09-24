#include "Ripple.h"

Ripple::Ripple(int p1, int p2, float radius)
{
    setParamMax(1, 3, -1);
    setParam1(p1);
    setParam2(p2);

    m_radius = radius;
    m_rate = 1;
}

Ripple::~Ripple()
{
}


void Ripple::calcVerts()
{
    int w = m_p1 + 1;
    m_numVerts = ((((2 * w) - 1) * m_p1) + 1) * 2; // for normals
    int size = m_numVerts * 3;
    m_vertexData = new GLfloat[size];

    double spacing = m_radius * 2.0 / m_p1;

    glm::vec3 v1, v2;
    v1 = glm::vec3(-m_radius, -m_radius, f(-m_radius, -m_radius));

    int index = 0;
    addVertex(&index, v1, calcNormal(v1));

    for (int i = 0; i < m_p1; i++) {

        v1.x = i * spacing - m_radius;

        bool skip = false;
        if (i % 2 == 0) {
            for (int j = 0; j < w; j++) {
                v1.y = j * spacing - m_radius;
                v1.z = f(v1.x, v1.y);
                v2 = v1;
                v2.x = v1.x + spacing;
                v2.z = f(v2.x, v2.y);

                if (skip)
                    addVertex(&index, v1, calcNormal(v1));
                else
                    skip = true;
                addVertex(&index, v2, calcNormal(v2));
            }

        } else {
            for (int j = m_p1; j >= 0; j--) {
                v1.y = j * spacing - m_radius;
                v1.z = f(v1.x, v1.y);
                v2 = v1;
                v2.x = v1.x + spacing;
                v2.z = f(v2.x, v2.y);

                if (skip)
                    addVertex(&index, v1, calcNormal(v1));
                else
                    skip = true;
                addVertex(&index, v2, calcNormal(v2));
            }
        }
    }
}


bool Ripple::animate()
{
    if (--m_rate < 0)
        m_rate = 100;
    return true;
}


void Ripple::render()
{
        glBindVertexArray(m_vaoID);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, m_numVerts / 2 /* Number of vertices to draw */);
        glBindVertexArray(0);
}


glm::vec3 Ripple::calcNormal(glm::vec3 p)
{
    if (EQ(p.x, 0) && EQ(p.y, 0))
        return glm::vec3(0, 0, 1);
    float x = p.x * 20.f;
    float y = p.y * 20.f;
    return glm::normalize(glm::vec3(
                -x * df(x, y),
                -y * df(x, y),
                .1f));
}


float Ripple::f(float x, float y)
{
    x = x * 20.f;
    y = y * 20.f;
    return (float) sin(sqrt(x * x + y * y) + (m_rate * M_PI * 2.f / 100.f)) * .1f;
}


float Ripple::df(float x, float y)
{
    double sqroot = sqrt(x * x + y * y);
    return (float) (cos(sqroot + (m_rate * M_PI * 2.f / 100.f)) * .1f / sqroot);
}
