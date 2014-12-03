#ifndef MUSICTRACKER_H
#define MUSICTRACKER_H

#include <QObject>
#include <QList>
#include "Shape.h"
#include "udphandler.h"
#include "OpenGLScene.h"

class MusicTracker : public QObject, public Shape
{
    Q_OBJECT
public:
    explicit MusicTracker(int p1, int p2, float radius,
                          GLuint shader, NormalRenderer *nr, QObject *parent = 0);
    virtual ~MusicTracker();

    virtual void calcVerts();
    virtual bool animate();

public slots:
    void setFunction(QList<float> function);

protected:
    float m_radius;

private:
    glm::vec3 calcNormal(glm::vec3 p);
    glm::vec4 f(float x, float y);
    glm::vec4 fv(glm::vec2 v);
    float df(float x, float y);

    GLuint m_shader;
    NormalRenderer *m_normalRenderer;

    int m_rate;
    UDPHandler *m_udp;
    QList<float> m_function;

};

#endif // MUSICTRACKER_H
