#ifndef MUSICSPHERE_H
#define MUSICSPHERE_H

#include <QObject>
#include <QList>
#include "Shape.h"
#include "udphandler.h"
#include "OpenGLScene.h"

class MusicSphere : public QObject, public Shape
{
    Q_OBJECT
public:
    explicit MusicSphere(int p1, int p2, float radius,
                          GLuint shader, NormalRenderer *nr, QObject *parent = 0);
    virtual ~MusicSphere();

    virtual void calcVerts();
    virtual bool animate();

public slots:
    void setFunction(QList<float> function);

protected:
    float m_radius;

private:
    glm::vec4 make3DRing(int *index, float outer, float inner, glm::vec4 prev, bool last);
    glm::vec4 f(float z);

    GLuint m_shader;
    NormalRenderer *m_normalRenderer;

    float m_depth;

    int m_rate;
    float m_ratef;

    UDPHandler *m_udp;
    QList<float> m_function;

};

#endif // MUSICSPHERE_H
