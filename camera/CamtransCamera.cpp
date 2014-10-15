/**
 * @file   CamtransCamera.cpp
 *
 * This is the perspective camera class you will need to fill in for the Camtrans lab.  See the
 * lab handout for more details.
 */

#include "CamtransCamera.h"
#include <Settings.h>

CamtransCamera::CamtransCamera()
{
    // View Defaults
    glm::vec4 eye = glm::vec4(2, 2, 2, 0);
    glm::vec4 look = -eye;
    glm::vec4 up = glm::vec4(0, 1, 0, 0);
    orientLook(eye, look, up);

    // Projection Defaults
    m_heightDegrees = 60;
    m_aspectRatio = 1.0;
    m_near = 1.0;
    m_far = 30.0;
    setProjectionMatrices();

}

void CamtransCamera::setAspectRatio(float a)
{
    m_aspectRatio = a;
    setProjectionMatrices();
}

glm::mat4x4 CamtransCamera::getProjectionMatrix() const
{
    return m_perspective * m_scale;
}

glm::mat4x4 CamtransCamera::getViewMatrix() const
{
    return m_rot * m_trans;
}

glm::mat4x4 CamtransCamera::getScaleMatrix() const
{
    return m_scale;
}

glm::mat4x4 CamtransCamera::getPerspectiveMatrix() const
{
    return m_perspective;
}

glm::vec4 CamtransCamera::getPosition() const
{
    return m_eye;
}

glm::vec4 CamtransCamera::getLook() const
{
    return glm::normalize(m_look);
}

glm::vec4 CamtransCamera::getUp() const
{
    return glm::normalize(m_up);
}

float CamtransCamera::getAspectRatio() const
{
    return m_aspectRatio;
}

float CamtransCamera::getHeightAngle() const
{
    return m_heightDegrees;
}

void CamtransCamera::orientLook(const glm::vec4 &eye, const glm::vec4 &look, const glm::vec4 &up)
{
    // Camera vecs
    m_eye = eye;
    m_look = look;
    m_up = up;

    setCameraSpace();
    setViewMatrices();
}

void CamtransCamera::setHeightAngle(float h)
{
    m_heightDegrees = h;
    setProjectionMatrices();
}

void CamtransCamera::translate(const glm::vec4 &v)
{
    m_eye += v;
    setViewMatrices();
}

void CamtransCamera::rotateU(float degrees)
{
    float cosine = glm::cos(glm::radians(degrees));
    float sine = glm::sin(glm::radians(degrees));

    glm::vec4 oldv = m_v;
    m_v = cosine * m_v + sine * m_w;
    m_w = cosine * m_w - sine * oldv;
    m_look = -m_w;
    m_up = m_v;
    setViewMatrices();
}

void CamtransCamera::rotateV(float degrees)
{
    float cosine = glm::cos(glm::radians(degrees));
    float sine = glm::sin(glm::radians(degrees));

    glm::vec4 oldw = m_w;
    m_w = cosine * m_w + sine * m_u;
    m_u = cosine * m_u - sine * oldw;
    m_look = -m_w;
    m_up = m_v;
    setViewMatrices();
}

void CamtransCamera::rotateW(float degrees)
{
    float cosine = glm::cos(glm::radians(degrees));
    float sine = glm::sin(glm::radians(degrees));

    glm::vec4 oldu = m_u;
    m_u = cosine * m_u + sine * m_v;
    m_v = cosine * m_v - sine * oldu;
    m_look = -m_w;
    m_up = m_v;
    setViewMatrices();
}

void CamtransCamera::setClip(float nearPlane, float farPlane)
{
    m_near = nearPlane;
    m_far = farPlane;
    setProjectionMatrices();
}

void CamtransCamera::setCameraSpace()
{
    // Camera coordinate space
    m_look = glm::normalize(m_look);
    m_w = -m_look;
    m_v = glm::normalize(m_up - (glm::dot(m_up, m_w) * m_w));
    m_u = glm::vec4(
                glm::normalize(
                        glm::cross(glm::vec3(m_v.x, m_v.y, m_v.z),
                                   glm::vec3(m_w.x, m_w.y, m_w.z))),
                    0);
    m_up = m_v;
}

void CamtransCamera::setViewMatrices()
{
    // View Matrix
    m_trans = glm::mat4x4();
    m_trans[3][0] = -m_eye[0];
    m_trans[3][1] = -m_eye[1];
    m_trans[3][2] = -m_eye[2];

    m_rot = glm::mat4x4(m_u.x, m_u.y, m_u.z, 0.0,
                        m_v.x, m_v.y, m_v.z, 0.0,
                        m_w.x, m_w.y, m_w.z, 0.0,
                         0.0,   0.0,   0.0,  1.0);
    m_rot = glm::transpose(m_rot);
}

void CamtransCamera::setProjectionMatrices()
{
    // Projection Matrices
    float h = 2.0 * m_far * glm::tan(glm::radians(m_heightDegrees / 2.0));
    float w = m_aspectRatio * h;

    m_scale = glm::mat4x4(1.0 / (w / 2.0),       0.0,          0.0,     0.0,
                               0.0,        1.0 / (h / 2.0),    0.0,     0.0,
                               0.0,              0.0,       1.0 / m_far,  0.0,
                               0.0,              0.0,          0.0,     1.0);
    m_scale = glm::transpose(m_scale);

    float c = -m_near / m_far;
    m_perspective = glm::mat4x4(1.0,   0.0,      0.0,         0.0,
                                0.0,   1.0,      0.0,         0.0,
                                0.0,   0.0, -1.0/(1.0+c),  c/(1.0+c),
                                0.0,   0.0,     -1.0,         0.0);
    m_perspective = glm::transpose(m_perspective);
}


void CamtransCamera::printVec(glm::vec4 v)
{
    cout << v[0] << ", ";
    cout << v[1] << ", ";
    cout << v[2] << ", ";
    cout << v[3] << ", ";
    cout << endl;
}

