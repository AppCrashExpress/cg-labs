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
#include "horseshoe.h"
#include "light.h"

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
    void setZoom     (float z);
    void setSideCount(int   n);
    void setLight    (Light l);

signals:
    void xRotationChanged(float x);
    void yRotationChanged(float y);
    void zoomChanged     (float z);
    void sideCountChanged(int   n);
    void lightChanged    (Light l);

private:
    float normalizeAngle(float angle);

    void initializeGL() override;
    void paintGL() override;
    void resizeGL(GLint width, GLint height) override;

    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent (QMouseEvent* event) override;
    void wheelEvent     (QWheelEvent* event) override;

    QMatrix4x4 createPerspective(GLfloat angle, GLfloat ratio, GLfloat near_plane, GLfloat far_plane);
    QMatrix4x4 createTranslation(GLfloat x,     GLfloat y,     GLfloat z);
    QMatrix4x4 createRotationX  (GLfloat angle);
    QMatrix4x4 createRotationY  (GLfloat angle);
    QMatrix4x4 createRotationZ  (GLfloat angle);

    void setupVaoAttribs();

    QOpenGLShaderProgram     shader_;
    QOpenGLVertexArrayObject vao_;
    QOpenGLBuffer            vbo_;

    HorseshoeMesh horseshoe_mesh_;
    std::vector<Polygon> polygons_;
    Light light_;

    QMatrix4x4 model_matrix_;
    QMatrix4x4 view_matrix_;
    QMatrix4x4 proj_matrix_;

    float x_rot_;
    float y_rot_;
    float zoom_;

    // For mouse
    QPoint last_pos_;
};

#endif // GLWIDGET_H
