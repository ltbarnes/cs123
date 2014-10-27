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
    m_numVerts = ((((2 * w) - 1) * m_p1) + 1); // for normals
    int size = m_numVerts * 8;
    m_vertexData = new GLfloat[size];

    double spacing = m_radius * 2.0 / m_p1;

    glm::vec3 v1, v2;
    v1 = glm::vec3(-m_radius, -m_radius, f(-m_radius, -m_radius));

    int index = 0;
    addVertexT(&index, v1, calcNormal(v1), glm::vec2(v1.x + 0.5f, 0.5f - v1.y));

    for (int i = 0; i < m_p1; i++) {

        v1.x = i * spacing - m_radius;

        bool skip = false;
        // draw up and down the columns depending on whether it's odd or even
        if (i % 2 == 0) {
            for (int j = 0; j < w; j++) {
                v1.y = j * spacing - m_radius;
                v1.z = f(v1.x, v1.y);
                v2 = v1;
                v2.x = v1.x + spacing;
                v2.z = f(v2.x, v2.y);

                if (skip)
                    addVertexT(&index, v1, calcNormal(v1), glm::vec2(v1.x + 0.5f, 0.5f - v1.y));
                else
                    skip = true;
                addVertexT(&index, v2, calcNormal(v2), glm::vec2(v2.x + 0.5f, 0.5f - v2.y));
            }

        } else {
            for (int j = m_p1; j >= 0; j--) {
                v1.y = j * spacing - m_radius;
                v1.z = f(v1.x, v1.y);
                v2 = v1;
                v2.x = v1.x + spacing;
                v2.z = f(v2.x, v2.y);

                if (skip)
                    addVertexT(&index, v1, calcNormal(v1), glm::vec2(v1.x + 0.5f, 0.5f - v1.y));
                else
                    skip = true;
                addVertexT(&index, v2, calcNormal(v2), glm::vec2(v2.x + 0.5f, 0.5f - v2.y));
            }
        }
    }
}


bool Ripple::animate()
{
    // update where on the sine function starts
    if (--m_rate < 0)
        m_rate = 100;
    return true;
}


glm::vec3 Ripple::calcNormal(glm::vec3 p)
{
    // kind of a hack to make sure the center normal is vertical
    if (EQ(p.x, 0) && EQ(p.y, 0))
        return glm::vec3(0, 0, 1);

    // calculates the gradient to the curve
    float x = p.x * 20.f;
    float y = p.y * 20.f;
    return glm::normalize(glm::vec3(
                -x * df(x, y),
                -y * df(x, y),
                .1f));
}

/**
 * @brief Ripple::f the sine function used to draw the shape:
 *                  f(x,y) = sin(sqrt(x^2 + y^2)
 * @param x - the x location
 * @param y - the y location
 * @return the z location based on the function
 */
float Ripple::f(float x, float y)
{
    x = x * 20.f;
    y = y * 20.f;
    return (float) sin(sqrt(x * x + y * y) + (m_rate * M_PI * 2.f / 100.f)) * .1f;
}

/**
 * @brief Ripple::df - the majority of the partial derivative of the sine fuction used to draw the shape
 * @param x - the x location
 * @param y - the y location
 * @return the value at point (x, y)
 */
float Ripple::df(float x, float y)
{
    double sqroot = sqrt(x * x + y * y);
    return (float) (cos(sqroot + (m_rate * M_PI * 2.f / 100.f)) * .1f / sqroot);
}
