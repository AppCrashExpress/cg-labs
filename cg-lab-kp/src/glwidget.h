#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShader>
#include <QMatrix4x4>

#include <vector>

#include "light.h"
#include "surface.h"

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    GLWidget(QWidget* parent = nullptr);
    ~GLWidget();

    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;

public slots:
    void setXRotation(float x);
    void setYRotation(float y);
    void setZoom     (float z);
    void setLight    (Light l);

signals:
    void xRotationChanged(float x);
    void yRotationChanged(float y);
    void zoomChanged     (float z);
    void lightChanged    (Light l);

private:
    float normalizeAngle(float angle);

    void initializeGL() override;
    void paintGL() override;
    void resizeGL(GLint width, GLint height) override;

    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent (QMouseEvent* event) override;
    void wheelEvent     (QWheelEvent* event) override;

    std::vector< std::vector<QVector3D> >
    copyAroundVector(const std::vector<QVector3D>& points,
                     float x, float y, float z);

    std::vector<GLfloat> getData(const BSurface& surface);
    void setupVaoAttribs();

    QOpenGLShaderProgram     shader_;
    QOpenGLVertexArrayObject vao_;
    QOpenGLBuffer            vbo_;

    QMatrix4x4 model_matrix_;
    QMatrix4x4 view_matrix_;
    QMatrix4x4 proj_matrix_;
    QMatrix3x3 rot_look_at_;

    std::vector< std::vector<QVector3D> > ctrl_points_;
    int   data_count_;
    Light light_;

    float x_rot_;
    float y_rot_;
    float zoom_;

    // For mouse
    QPoint last_pos_;
};

#endif // GLWIDGET_H
