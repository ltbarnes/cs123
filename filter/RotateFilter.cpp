#include "RotateFilter.h"
#include "Canvas2D.h"

RotateFilter::RotateFilter(float angle)
    : Filter()
{
    m_R = glm::mat2();
    setAngle(angle);
}


RotateFilter::~RotateFilter()
{
}


void RotateFilter::setAngle(float angle)
{
    m_angle = (angle / 180.f) * M_PI;
    m_R[0][0] = cos(m_angle);
    m_R[0][1] = sin(m_angle);
    m_R[1][0] = -sin(m_angle);
    m_R[1][1] = cos(m_angle);
}


void RotateFilter::filter(Canvas2D *canvas)
{
    // no rotation
    if (!m_angle || EQ(fabs(m_angle), 2.f * M_PI))
        return;

    int pad = 2;

    setBounds(canvas);
    makeFilterCanvas(canvas, pad, true);

    int width = canvas->width();
    int height = canvas->height();

    glm::vec2 v1, v2, v3;
    v1 = m_R * glm::vec2(0, height);
    v2 = m_R * glm::vec2(width, height);
    v3 = m_R * glm::vec2(width, 0);

    float minX = std::min(0.f, std::min(v1.x, std::min(v2.x, v3.x)));
    float maxX = std::max(0.f, std::max(v1.x, std::max(v2.x, v3.x)));
    float minY = std::min(0.f, std::min(v1.y, std::min(v2.y, v3.y)));
    float maxY = std::max(0.f, std::max(v1.y, std::max(v2.y, v3.y)));

    width = width + 2*pad;
    height = height + 2*pad;
    int nw = (int) ceil(maxX - minX);
    int nh = (int) ceil(maxY - minY);

    canvas->resize(nw, nh);
    BGRA *pix = canvas->data();

    // column row for glm
    glm::mat3 T= glm::mat3();
    T[2][0] = -nw / 2.f;
    T[2][1] = -nh / 2.f;
    glm::mat3 trans = glm::mat3(m_R) * T;
    T[2][0] = width / 2.f;
    T[2][1] = height / 2.f;
    trans = T * trans;

    glm::vec3 np = glm::vec3(0.f, 0.f, 1.f);
    glm::vec3 op = glm::vec3(0.f, 0.f, 1.f);
    int ci, fi;
    for (int y = 0; y < nh; y++) {
        for (int x = 0; x < nw; x++) {
            op.x = x;
            op.y = y;

            np = trans * op;

            if (inBounds(np, width, height)) {
                ci = y * nw + x;

                double modx, mody;
                float weightx = modf(np.x, &modx);
                float weighty = modf(np.y, &mody);
                fi = ((int) mody) * width + ((int) modx);

                fillPixel(&pix[ci], fi, width, weightx, weighty);
            }
        }
    }
}


bool RotateFilter::inBounds(glm::vec3 p, int w, int h)
{
    return p.x >= 0 && p.x < w - 1 && p.y >= 0 && p.y < h - 1;
}


void RotateFilter::fillPixel(BGRA *pix, int i, int width, float wxr, float wyb)
{
    BGRA *tl = &m_filter_canvas[i];
    BGRA *tr = &m_filter_canvas[i+1];
    BGRA *bl = &m_filter_canvas[i+width];
    BGRA *br = &m_filter_canvas[i+width+1];

    float wxl = 1.f - wxr;
    float wyt = 1.f - wyb;

    pix->r = tl->r * wxl * wyt +
             bl->r * wxl * wyb +
             br->r * wxr * wyb +
             tr->r * wxr * wyt;
    pix->g = tl->g * wxl * wyt +
             bl->g * wxl * wyb +
             br->g * wxr * wyb +
             tr->g * wxr * wyt;
    pix->b = tl->b * wxl * wyt +
             bl->b * wxl * wyb +
             br->b * wxr * wyb +
             tr->b * wxr * wyt;
}
