#include "musicsphere.h"
#include "udphandler.h"
#include <glm/gtx/rotate_vector.hpp>

MusicSphere::MusicSphere(int p1, int p2, float radius, GLuint shader, NormalRenderer *nr, QObject *parent) :
    QObject(parent)
{
    setParamMax(2, 3, -1);
    setParam1(p1);
    setParam2(p2);

    m_radius = radius;
    m_udp = new UDPHandler(this);
    m_shader = shader;
    m_normalRenderer = nr;

    m_depth = radius * .2f;
    m_rate = 1;
    m_ratef = M_PI * 2.f / 45.f;
}

MusicSphere::~MusicSphere()
{
}


void MusicSphere::calcVerts()
{
    // ((slices * verts per slice) - first and last vert) * 2 for normals
    m_numVerts = (((m_p2 + 2) * 2 * (m_p1)));
    int size = m_numVerts * 8; // 3 points per vertex
    m_vertexData = new GLfloat[size];

    float inner = -m_radius;
    float outer;
    glm::vec4 prev = glm::vec4(0, 0, -1, 0);

    int index = 0;
    float mult = (2.f * m_radius) / m_p1;

    for (int i = 1; i <= m_p1; i++) {
        outer = (i * mult) - m_radius;

        prev = make3DRing(&index, outer, inner, prev, i == m_p1);

        inner = outer;
    }
}


glm::vec4 MusicSphere::make3DRing(int *index, float outer, float inner, glm::vec4 prev, bool last)
{
    glm::vec4 curr;
    if (last)
        curr = glm::vec4(0, 0, 1, 0);
    else
        curr = f(outer);
    glm::vec3 currV = glm::vec3(0, curr.w, outer);
    glm::vec3 currN = glm::vec3(curr);
    glm::vec3 prevV = glm::vec3(0, prev.w, inner);
    glm::vec3 prevN = glm::vec3(prev);
    glm::vec3 rotationVector = glm::vec3(0, 0, -1);

    addVertex(index, currV, currN);

    float angle = (2.0 * M_PI) / m_p2;
    float a;
    glm::vec3 v1, v2;
    glm::vec3 n1, n2;
    for (int i = 0; i <= m_p2; i++) {
        a = angle * i;
        v1 = glm::rotate(currV, a, rotationVector);
        n1 = glm::rotate(currN, a, rotationVector);
        v2 = glm::rotate(prevV, a, rotationVector);
        n2 = glm::rotate(prevN, a, rotationVector);

        addVertex(index, v1, n1);
        addVertex(index, v2, n2);
    }
    addVertex(index, v2, n2);

    return curr;
}


void MusicSphere::setFunction(QList<float> function)
{
    m_function = function;
    calcVerts();
    updateGL(m_shader);
    updateNormals(m_normalRenderer);
    cleanUp();
}


bool MusicSphere::animate()
{
    if (--m_rate < 0)
        m_rate = 45;
    m_ratef = m_rate * M_PI * 2.f / 45.f;
    return true;
}

/**
 * @brief MusicSphere::f the sine function used to draw the shape:
 *                  f(x,y) = sin(sqrt(x^2 + y^2)
 * @param x - the x location
 * @param y - the y location
 * @return the z location based on the function
 */
glm::vec4 MusicSphere::f(float z)
{
    if (m_function.isEmpty())
        return glm::vec4(0.f);

    double sizeMinus = m_function.size() - 1.f;
    double di = (glm::abs(z) * 2.f/*+ m_radius*/) * (sizeMinus);
    float f = modf(di, &di);
    int li, ri;
    float mid, t, left, right;
    if (f < 0.5f) {
        t = f + 0.5f;
        li = (int)glm::max(0.0, di - 1);
        ri = (int) glm::min(di + 1, sizeMinus);
    } else {
        t = f - 0.5f;
        li = (int)glm::max(0.0, di);
        ri = (int) glm::min(di + 2.0, sizeMinus);
        di = glm::min(di + 1.0, sizeMinus);
    }
    mid = m_function.value((int)di);
    left = (m_function.value(li) + mid) / 2.f;
    right = (mid + m_function.value(ri)) / 2.f;

    float mag = (((mid - left) * t + left) - ((mid - right) * ((1.f - t)) + right));
    mag *= (z > 0 ? 1.f : -1.f);
    glm::vec3 norm = glm::normalize(glm::vec3(0.f, 1.f / (m_function.size() - 1.f), mag));

    float t_1 = 1.f - t;
    float curve = t_1 * (t_1 * left + t * mid) + t * (t_1 * mid + t * right);
//    glm::vec3 norm = glm::vec3(0, 1, 0);
//    float curve = 0.5f;

    return glm::vec4(norm.x, norm.y, norm.z, curve * .6f);

}
