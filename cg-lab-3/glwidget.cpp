﻿#include <QOpenGLFunctions>
#include <QMouseEvent>

#include <iostream>
#include <math.h>

#include "glwidget.h"
#include "horseshoe.h"
#include "light.h"

GLwidget::GLwidget(QWidget* parent) :
    vbo_(QOpenGLBuffer::VertexBuffer)
{
    setParent(parent);
}

GLwidget::~GLwidget()
{
    vao_.destroy();
    vbo_.destroy();
}

QSize GLwidget::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize GLwidget::sizeHint() const
{
    return QSize(400, 400);
}

void GLwidget::setXRotation(float x)
{
    x = normalizeAngle(x);
    if (x != x_rot_)
    {
        x_rot_ = x;
        emit xRotationChanged(x);
        update();
    }
}

void GLwidget::setYRotation(float y)
{
    y = normalizeAngle(y);
    if (y != y_rot_)
    {
        y_rot_ = y;
        emit yRotationChanged(y);
        update();
    }
}

void GLwidget::setZoom(float z)
{
    if (z != zoom_)
    {
        zoom_ = z;
        emit zoomChanged(z);
        view_matrix_.setToIdentity();
        view_matrix_ = createTranslation(0, 0, -zoom_);
        update();
    }
}

void GLwidget::setSideCount(int n)
{
    if (n != horseshoe_mesh_.getSideCount())
    {
        horseshoe_mesh_.createHorseshoe(n);
        polygons_ = horseshoe_mesh_.getPolygons();
        emit sideCountChanged(n);
        update();
    }
}

void GLwidget::setLight(Light l)
{
    light_ = l;
    emit lightChanged(l);
    update();
}

float GLwidget::normalizeAngle(float angle)
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

void GLwidget::initializeGL()
{
    setSideCount(10);
    setLight(Light(QVector3D(0.0f, 1.0f, 0.0f)));

    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);

    shader_.addShaderFromSourceFile(QOpenGLShader::Vertex,   ":/main_vertex_shader.vsh");
    shader_.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/main_fragment_shader.fsh");
    shader_.link();
    if (!shader_.isLinked())
    {
        std::cerr << shader_.log().toStdString() << std::endl;
    }

    vao_.create();
    vao_.bind();

    vbo_.create();
    vbo_.bind();
    vbo_.allocate(0, 18 * sizeof(GLfloat));

    setupVaoAttribs();

    vao_.release();

    view_matrix_.setToIdentity();
    view_matrix_ *= createTranslation(0, 0, -zoom_);
}

void GLwidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    model_matrix_.setToIdentity();
    model_matrix_ *= createRotationX(x_rot_ / 16);
    model_matrix_ *= createRotationY(y_rot_ / 16);

    std::vector<GLfloat> vert;
    for (const auto& poly : polygons_) {
        Polygon transPoly = poly.transformAndColor(model_matrix_, view_matrix_, proj_matrix_, light_, 0.1, 256, QVector3D(0, 0, 1));

        if (transPoly.isVisible(QVector3D(0, 0, 1))) {
            std::vector<GLfloat> polyVert = transPoly.getVerticesFloat();
            vert.insert(vert.end(), polyVert.begin(), polyVert.end());
        }
    }

    vao_.bind();
    shader_.bind();

    for (size_t i = 0; i < vert.size(); i += 18) {
        vbo_.write(0, &vert[i], 18 * sizeof(GLfloat));
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }

    shader_.release();
    vao_.release();
}

void GLwidget::resizeGL(GLint w, GLint h)
{
    proj_matrix_ = createPerspective(45.0f, GLfloat(w)/GLfloat(h), 0.01f, 100.0f);
}

void GLwidget::mousePressEvent(QMouseEvent* event)
{
    last_pos_ = event->pos();
}

void GLwidget::mouseMoveEvent(QMouseEvent* event)
{
    int x_change = event->x() - last_pos_.x();
    int y_change = event->y() - last_pos_.y();

    setXRotation(x_rot_ + y_change * 8);
    setYRotation(y_rot_ + x_change * 8);

    last_pos_ = event->pos();
}

void GLwidget::wheelEvent(QWheelEvent* event)
{
    float step_count = event->angleDelta().y() / 120.0f;
    if (zoom_ + step_count > 3.0f && zoom_ + step_count < 31.0f)
    {
        setZoom(zoom_ + step_count);
    }
    event->accept();
}

QMatrix4x4 GLwidget::createPerspective(GLfloat angle, GLfloat ratio, GLfloat near_plane, GLfloat far_plane)
{
    float scale  = tan(angle * M_PI / 360.0f) * near_plane;
    float right  = ratio * scale;
    float left   = -right;
    float top    = scale;
    float bottom = -top;

    QMatrix4x4 result;
    result(0, 0) = 2 * near_plane / (right - left);
    result(0, 2) = (right + left) / (right - left);
    result(1, 1) = 2 * near_plane / (top - bottom);
    result(1, 2) = (top + bottom) / (top - bottom);
    result(2, 2) = -(far_plane + near_plane) / (far_plane - near_plane);
    result(2, 3) = -(2 * far_plane * near_plane) / (far_plane - near_plane);
    result(3, 2) = -1;

    return result;
}

QMatrix4x4 GLwidget::createTranslation(GLfloat x, GLfloat y, GLfloat z)
{
    return QMatrix4x4(
        1, 0, 0, x,
        0, 1, 0, y,
        0, 0, 1, z,
        0, 0, 0, 1
    );
}

QMatrix4x4 GLwidget::createRotationX(GLfloat angle)
{
    float rad = angle * M_PI / 180;

    return QMatrix4x4(
        1,    0,         0,     0,
        0, cos(rad), -sin(rad), 0,
        0, sin(rad),  cos(rad), 0,
        0,    0,         0,     1
    );
}

QMatrix4x4 GLwidget::createRotationY(GLfloat angle)
{
    float rad = angle * M_PI / 180;

    return QMatrix4x4(
         cos(rad), 0, sin(rad), 0,
            0,     1,    0,     0,
        -sin(rad), 0, cos(rad), 0,
            0,     0,    0,     1
    );
}

QMatrix4x4 GLwidget::createRotationZ(GLfloat angle)
{
    float rad = angle * M_PI / 180;

    return QMatrix4x4(
        cos(rad), -sin(rad), 0, 0,
        sin(rad),  cos(rad), 0, 0,
           0,         0,     1, 0,
           0,         0,     0, 1
    );
}

void GLwidget::setupVaoAttribs()
{
    vao_.bind();
    QOpenGLFunctions* f = QOpenGLContext::currentContext()->functions();
    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
    f->glEnableVertexAttribArray(0);
    f->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    f->glEnableVertexAttribArray(1);
    vao_.release();
}
