#include "ShapesScene.h"
#include "Camera.h"
#include <SupportCanvas3D.h>
#include <QFileDialog>

#define SHAPE_RADIUS 0.5f

glm::vec4 lightDirection = glm::normalize(glm::vec4(1.f, -1.f, -1.f, 0.f));

ShapesScene::ShapesScene()
{
    // Use a shiny orange material
    memset(&m_material, 0, sizeof(m_material));
    m_material.cAmbient.r = 0.2f;
    m_material.cAmbient.g = 0.1f;
    m_material.cDiffuse.r = 1;
    m_material.cDiffuse.g = 0.5f;
    m_material.cSpecular.r = m_material.cSpecular.g = m_material.cSpecular.b = 1;
    m_material.shininess = 64;

    // Use a white directional light from the upper left corner
    memset(&m_light, 0, sizeof(m_light));
    m_light.type = LIGHT_DIRECTIONAL;
    m_light.dir = lightDirection;
    m_light.color.r = m_light.color.g = m_light.color.b = 1;
    m_light.id = 0;

    //TODO: [SHAPES] Allocate any additional memory you need...
    m_shape = NULL;
}

ShapesScene::~ShapesScene()
{
    // TODO: [SHAPES] Don't leak memory!
    if (m_shape)
        delete m_shape;
}

void ShapesScene::init()
{
    // TODO: [SHAPES] Initialize the shape based on settings.shapeType. The sample code provided
    //       initializes a single triangle using OpenGL. Refer to the labs for more information
    //       about these OpenGL functions.
    //
    //       If you've chosen to implement subclasses for your shapes, this would be a good time to
    //       call upon the subclass to initialize itself.
    //

    OpenGLScene::init(); // Call the superclass's init()

    switch (settings.shapeType) {
    case SHAPE_CUBE:
//        m_shape = new Cube(settings.shapeParameter1, settings.shapeParameter2, SHAPE_RADIUS);
//        break;
    case SHAPE_CONE:
//        m_shape = new Cone(settings.shapeParameter1, settings.shapeParameter2, SHAPE_RADIUS);
//        break;
    case SHAPE_SPHERE:
//        m_shape = new Sphere(settings.shapeParameter1, settings.shapeParameter2, SHAPE_RADIUS);
//        break;
    case SHAPE_CYLINDER:
//        m_shape = new Cylinder(settings.shapeParameter1, settings.shapeParameter2, SHAPE_RADIUS);
//        break;
    case SHAPE_SPECIAL_1:
//        m_shape = new (settings.shapeParameter1, settings.shapeParameter2, SHAPE_RADIUS);
//        break;
    case SHAPE_SPECIAL_2:
//        m_shape = new (settings.shapeParameter1, settings.shapeParameter2, SHAPE_RADIUS);
//        break;
    case SHAPE_SPECIAL_3:
//        m_shape = new (settings.shapeParameter1, settings.shapeParameter2, SHAPE_RADIUS);
//        break;
    default: // basic triangle shape
        m_shape = new Shape();
        break;
    }

    m_shape->calcVerts();
    m_shape->updateGL(m_shader);

//   glEnableVertexAttribArray(glGetAttribLocation(m_shader, "position"));
//   glVertexAttribPointer(
//       glGetAttribLocation(m_shader, "position"),
//       3,                   // Num coordinates per position
//       GL_FLOAT,            // Type
//       GL_FALSE,            // Normalized
//       sizeof(GLfloat) * 6, // Stride
//       (void*) 0            // Array buffer offset
//   );
//   glEnableVertexAttribArray(glGetAttribLocation(m_shader, "normal"));
//   glVertexAttribPointer(
//       glGetAttribLocation(m_shader, "normal"),
//       3,           // Num coordinates per normal
//       GL_FLOAT,    // Type
//       GL_TRUE,     // Normalized
//       sizeof(GLfloat) * 6,           // Stride
//       (void*) (sizeof(GLfloat) * 3)    // Array buffer offset
//   );

//   // Unbind buffers.
//   glBindBuffer(GL_ARRAY_BUFFER, 0);
//   glBindVertexArray(0);

    m_shape->updateNormals(m_normalRenderer);
    m_shape->cleanUp();
}

void ShapesScene::renderGeometry()
{

    applyMaterial(m_material);

    // Draw the shape.
    m_shape->render();

}

void ShapesScene::setLights(const glm::mat4 viewMatrix)
{
    // YOU DON'T NEED TO TOUCH THIS METHOD, unless you want to do fancy lighting...

    m_light.dir = glm::inverse(viewMatrix) * lightDirection;

    clearLights();
    setLight(m_light);
}

