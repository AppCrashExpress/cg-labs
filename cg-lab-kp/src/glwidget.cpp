#include <QOpenGLFunctions>
#include <QMouseEvent>

#include <iostream>
#include <math.h>

#include "glwidget.h"
#include "light.h"

namespace {
    constexpr QVector3D kColor(0.5f, 0.5f, 0.5f);
    constexpr int kDegree = 3; // of closed line
}

GLWidget::GLWidget(QWidget* parent) :
    vbo_(QOpenGLBuffer::VertexBuffer)
{
    setParent(parent);
}

GLWidget::~GLWidget()
{
    vao_.destroy();
    vbo_.destroy();
}

QSize GLWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize GLWidget::sizeHint() const
{
    return QSize(600, 600);
}

void GLWidget::setXRotation(float x)
{
    x = normalizeAngle(x);
    if (x != x_rot_)
    {
        x_rot_ = x;
        emit xRotationChanged(x);
        update();
    }
}

void GLWidget::setYRotation(float y)
{
    y = normalizeAngle(y);
    if (y != y_rot_)
    {
        y_rot_ = y;
        emit yRotationChanged(y);
        update();
    }
}

void GLWidget::setZoom(float z)
{
    if (z != zoom_)
    {
        zoom_ = z;
        emit zoomChanged(z);
        view_matrix_.setToIdentity();
        view_matrix_.translate(0, 0, -zoom_);
        update();
    }
}

void GLWidget::setLight(Light l)
{
    light_ = l;
    emit lightChanged(l);
    update();
}

float GLWidget::normalizeAngle(float angle)
{
    constexpr float max_diff = 360.0f * 16 * 2;

    while (angle < 0)
    {
        angle += max_diff;
    }
    while (angle > max_diff)
    {
        angle -= max_diff;
    }
    return angle;
}

void GLWidget::initializeGL()
{
    setLight(Light(QVector3D(0.0f, 1.0f, 0.0f)));

    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);

    shader_.addShaderFromSourceFile(QOpenGLShader::Vertex,   ":/shaders/main.vsh");
    shader_.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/main.fsh");
    shader_.link();
    if (!shader_.isLinked())
    {
        std::cerr << shader_.log().toStdString() << std::endl;
    }

    ctrl_points_ = copyAroundVector({QVector3D(0, 1, 0.5), QVector3D(0, 0.5, 2), QVector3D(0, 0, 1), QVector3D(0, -0.5, 0.5), QVector3D(0, -1, 1)},
                                    0.0f, 1.0f, 0.0f);

    BSurface surface(ctrl_points_, 0.001f, 0.001f);

    std::vector<GLfloat> data = getData(surface);
    data_count_ = data.size();

    vao_.create();
    vao_.bind();

    vbo_.create();
    vbo_.bind();
    vbo_.allocate((void*)data.data(), data.size() * sizeof(GLfloat));

    setupVaoAttribs();

    vao_.release();

    view_matrix_.setToIdentity();
    zoom_ = 3.0f;
    view_matrix_.translate(0, 0, -zoom_);
}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    model_matrix_.setToIdentity();
    model_matrix_.rotate(x_rot_ / 16, 1.0f, 0.0f, 0.0f);
    model_matrix_.rotate(y_rot_ / 16, 0.0f, 1.0f, 0.0f);

    vao_.bind();
    shader_.bind();

    shader_.setUniformValue("model",       model_matrix_);
    shader_.setUniformValue("view",        view_matrix_);
    shader_.setUniformValue("proj",        proj_matrix_);
    shader_.setUniformValue("light_pos",   light_.getPos());
    shader_.setUniformValue("light_color", light_.getColor());
    shader_.setUniformValue("ambt_bool",   light_.getOptions() & LightOptions::L_AMBIENT);
    shader_.setUniformValue("diff_bool",   light_.getOptions() & LightOptions::L_DIFFUSE);
    shader_.setUniformValue("spec_bool",   light_.getOptions() & LightOptions::L_SPECULAR);
    shader_.setUniformValue("view_pos",    QVector3D(0.0f, 0.0f, -1.0f));

    glDrawArrays(GL_TRIANGLES, 0, data_count_ / 9);

    shader_.release();
    vao_.release();
}

void GLWidget::resizeGL(GLint w, GLint h)
{
    proj_matrix_.setToIdentity();
    proj_matrix_.perspective(45.0f, GLfloat(w)/GLfloat(h), 0.01f, 100.0f);
}

void GLWidget::mousePressEvent(QMouseEvent* event)
{
    last_pos_ = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent* event)
{
    int x_change = event->x() - last_pos_.x();
    int y_change = event->y() - last_pos_.y();

    setXRotation(x_rot_ + y_change * 8);
    setYRotation(y_rot_ + x_change * 8);

    last_pos_ = event->pos();
}

void GLWidget::wheelEvent(QWheelEvent* event)
{
    float step_count = event->angleDelta().y() / 120.0f;
    if (zoom_ + step_count > 3.0f && zoom_ + step_count < 31.0f)
    {
        setZoom(zoom_ + step_count);
    }
    event->accept();
}

std::vector< std::vector<QVector3D> >
GLWidget::copyAroundVector(const std::vector<QVector3D>& points,
                           float x, float y, float z)
{
    const int rotation_count = 4 + kDegree;

    std::vector< std::vector<QVector3D> > copied_points;
    copied_points.resize(points.size());
    std::for_each(copied_points.begin(),
                  copied_points.end(),
                  [](std::vector<QVector3D>& vec){
                      vec.resize(rotation_count);
                  }
    );


    QMatrix4x4 rotator;
    rotator.setToIdentity();

    for (short rot_num = 0; rot_num < rotation_count; ++rot_num)
    {
        for (size_t point_num = 0; point_num < points.size(); ++point_num)
        {
            copied_points[point_num][rot_num] = points[point_num] * rotator;
        }

        rotator.rotate(-90, x, y, z);
    }

    return copied_points;
}

std::vector<GLfloat> GLWidget::getData(const BSurface& surface)
{
    const std::vector< std::vector<QVector3D> >& points = surface.getPoints();    

    const size_t line_count  = points.size() - 1;
    const size_t point_count = points[0].size();

    std::vector<GLfloat> data;

    for (size_t line = 0; line < line_count; ++line)
    {
        for (size_t point = 0; point < point_count; ++point)
        {
            size_t nextPoint = (point + 1) % point_count;
            QVector3D a = points[line][nextPoint] - points[line][point];
            QVector3D b = points[line + 1][point] - points[line][point];
            QVector3D norm = QVector3D::crossProduct(a.normalized(), b.normalized());

            const float x[] = {
                points[line][point].x(),
                points[line][nextPoint].x(),
                points[line + 1][nextPoint].x(),
                points[line + 1][point].x()
            };

            const float y[] = {
                points[line][point].y(),
                points[line][nextPoint].y(),
                points[line + 1][nextPoint].y(),
                points[line + 1][point].y()
            };

            const float z[] = {
                points[line][point].z(),
                points[line][nextPoint].z(),
                points[line + 1][nextPoint].z(),
                points[line + 1][point].z()
            };

            /**  It looks something like this:
             *
             *       [line][nextPoint]          [line][point]
             *                     1 .          . 0
             *
             *                     2 .          . 3
             *   [line + 1][nextPoint]          [line + 1][point]
             **/

            data.insert(data.end(), {
                             x[0], y[0], z[0], kColor.x(), kColor.y(), kColor.z(), norm.x(), norm.y(), norm.z(),
                             x[1], y[1], z[1], kColor.x(), kColor.y(), kColor.z(), norm.x(), norm.y(), norm.z(),
                             x[2], y[2], z[2], kColor.x(), kColor.y(), kColor.z(), norm.x(), norm.y(), norm.z(),

                             x[0], y[0], z[0], kColor.x(), kColor.y(), kColor.z(), norm.x(), norm.y(), norm.z(),
                             x[2], y[2], z[2], kColor.x(), kColor.y(), kColor.z(), norm.x(), norm.y(), norm.z(),
                             x[3], y[3], z[3], kColor.x(), kColor.y(), kColor.z(), norm.x(), norm.y(), norm.z()
                         });
            // We could have different norms for each triangle
            // but here the triangles form rectangle, so
            // the norm should be the same

        }
    }

    return data;
}

void GLWidget::setupVaoAttribs()
{
    vao_.bind();
    QOpenGLFunctions* f = QOpenGLContext::currentContext()->functions();
    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (void*)0);
    f->glEnableVertexAttribArray(0);
    f->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    f->glEnableVertexAttribArray(1);
    f->glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
    f->glEnableVertexAttribArray(2);
    vao_.release();
}
