#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QMatrix4x4>

#include <vector>

#include "polygon.h"

class GLwidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    GLwidget(QWidget* parent = nullptr);
    ~GLwidget();

    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;

public slots:
    void setXRotation(float x);
    void setYRotation(float y);
    void setZoom(float z);

signals:
    void xRotationChanged(float x);
    void yRotationChanged(float y);
    void zoomChanged(float z);

private:
    float normalizeAngle(float angle);

    void initializeGL() override;
    void paintGL() override;
    void resizeGL(GLint width, GLint height) override;

    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent (QMouseEvent* event) override;
    void wheelEvent     (QWheelEvent* event) override;

    std::vector<GLfloat> getVertices(uint16_t side_count, float r, float h);
    std::vector<GLuint>  getIndecies(uint16_t side_count);
    std::vector<Polygon> getPolygons(uint16_t side_count, float r, float h);

    QMatrix4x4 createPerspective(GLfloat angle, GLfloat ratio, GLfloat near_plane, GLfloat far_plane);
    QMatrix4x4 createTranslation(GLfloat x,     GLfloat y,     GLfloat z);
    QMatrix4x4 createRotationX  (GLfloat angle);
    QMatrix4x4 createRotationY  (GLfloat angle);
    QMatrix4x4 createRotationZ  (GLfloat angle);

    void setupVaoAttribs();

    QOpenGLShaderProgram     shader_;
    QOpenGLVertexArrayObject vao_;
    QOpenGLBuffer            vbo_;

    GLuint color_pos_;

    std::vector<Polygon> polygons_;

    QMatrix4x4 model_matrix_;
    QMatrix4x4 view_matrix_;
    QMatrix4x4 proj_matrix_;

    uint16_t side_count_ = 10;
    uint16_t radius_     = 3;
    float    height_     = 0.5f;

    float x_rot_;
    float y_rot_;
    float zoom_;

    QPoint last_pos_;
};

#endif // GLWIDGET_H
