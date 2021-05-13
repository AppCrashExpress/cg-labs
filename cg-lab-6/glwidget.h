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

    QMatrix3x3 calculatePlaneRotation(const QVector3D& norm1, const QVector3D& norm2);

    void setupVaoAttribs();

    QOpenGLShaderProgram     shader_;
    QOpenGLShaderProgram     light_shader_;
    QOpenGLVertexArrayObject vao_;
    QOpenGLBuffer            vbo_;

    int side_count_;
    std::vector<GLfloat> data_;
    std::vector<GLfloat> light_data_;
    Light light_;
    float angle_; // to calculate rotation of object on arbitrary plane

    QMatrix4x4 model_matrix_;
    QMatrix4x4 view_matrix_;
    QMatrix4x4 proj_matrix_;
    QMatrix3x3 rot_look_at_;

    float x_rot_;
    float y_rot_;
    float zoom_;

    // For mouse
    QPoint last_pos_;
};

#endif // GLWIDGET_H
