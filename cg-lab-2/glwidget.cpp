#include <QOpenGLFunctions>
#include <QMouseEvent>

#include <iostream>
#include <math.h>

#include "glwidget.h"

namespace {
    const char kVShaderSrc[] = "#version 420 core\n"
            "layout (location = 0) in vec3 inPos;\n"
            "void main()\n"
            "{\n"
            "   gl_Position = vec4(inPos.x, inPos.y, inPos.z, 1.0);\n"
            "}\0";
    const char kFShaderSrc[] = "#version 420 core\n"
            "out vec4 fragColor;\n"
            "uniform vec3 inColor;\n"
            "void main()\n"
            "{\n"
            "   fragColor = vec4(inColor, 1.0);\n"
            "}\0";
}

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
    polygons_ = getPolygons(side_count_, radius_, height_);

    initializeOpenGLFunctions();
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    shader_.addShaderFromSourceCode(QOpenGLShader::Vertex,   kVShaderSrc);
    shader_.addShaderFromSourceCode(QOpenGLShader::Fragment, kFShaderSrc);
    shader_.link();
    if (!shader_.isLinked())
    {
        std::cerr << shader_.log().toStdString() << std::endl;
    }
    color_pos_ = shader_.uniformLocation("inColor");

    vao_.create();
    vao_.bind();

    vbo_.create();
    vbo_.bind();
    vbo_.allocate(0, side_count_ * 36 * sizeof(GLfloat));

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
    std::vector<GLfloat> invis_vert;
    for (const auto& poly : polygons_) {
        Polygon transPoly = poly.transform(model_matrix_, view_matrix_, proj_matrix_);
        std::vector<GLfloat> polyVert = transPoly.getVertices();

        if (transPoly.isVisible(QVector3D(0, 0, 1))) {
            vert.insert(vert.end(), polyVert.begin(), polyVert.end());
        } else {
            invis_vert.insert(invis_vert.end(), polyVert.begin(), polyVert.end());
        }
    }

    vao_.bind();
    shader_.bind();

    vbo_.write(0, invis_vert.data(), invis_vert.size() * sizeof(GLfloat));
    shader_.setUniformValue(color_pos_, 0.5f, 0.0f, 0.0f);
    glDrawArrays(GL_TRIANGLES, 0, invis_vert.size() / 3);

    vbo_.write(0, vert.data(), vert.size() * sizeof(GLfloat));
    shader_.setUniformValue(color_pos_, 0.0f, 1.0f, 0.0f);
    glDrawArrays(GL_TRIANGLES, 0, vert.size() / 3);

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

std::vector<GLfloat> GLwidget::getVertices(uint16_t side_count, float r, float h)
{
    // Pre-condition: side_count > 2

    std::vector<GLfloat> result;
    result.reserve((side_count + 1) * 2 * 3);

    result.push_back(0);
    result.push_back(- h * r / 2);
    result.push_back(0);
    for (size_t i = 0; i < side_count; ++i) {
        result.push_back(r * cos(2 * M_PI * i / side_count));
        result.push_back(- h * r / 2);
        result.push_back(r * sin(2 * M_PI * i / side_count));
    }

    float reduction = (1.0f - h);
    for (size_t i = 0; i < side_count; ++i) {
        result.push_back(reduction * r * cos(2 * M_PI * i / side_count));
        result.push_back(h * r / 2);
        result.push_back(reduction * r * sin(2 * M_PI * i / side_count));
    }
    result.push_back(0);
    result.push_back(h * r / 2);
    result.push_back(0);

    return result;
}

std::vector<GLuint> GLwidget::getIndecies(uint16_t side_count)
{
    std::vector<GLuint> result;
                   // top           // bottom        // sides
    result.reserve(3 * side_count + 3 * side_count + 6 * side_count);

    for (size_t i = 1; i <= side_count; ++i) {
        result.push_back(i);
        result.push_back(0);
        result.push_back(i % side_count + 1);
    }

    for (size_t i = 1; i <= side_count; ++i) {
        result.push_back(side_count + i % side_count + 1);
        result.push_back(side_count * 2 + 1);
        result.push_back(side_count + i);
    }

    for (size_t i = 1; i <= side_count; ++i) {
        result.push_back(i);
        result.push_back(i % side_count + 1);
        result.push_back(side_count + i % side_count + 1);

        result.push_back(i);
        result.push_back(side_count + i % side_count + 1);
        result.push_back(side_count + i);
    }

    return result;
}

std::vector<Polygon> GLwidget::getPolygons(uint16_t side_count, float r, float h)
{
    std::vector<GLfloat> vertices = getVertices(side_count, r, h);
    std::vector<GLuint>  indecies = getIndecies(side_count);

    std::vector<Polygon> polygons;
    polygons.reserve(side_count * 4);
    for (size_t i = 0; i < indecies.size(); i += 3) {
        polygons.push_back(
            Polygon(indecies[i], indecies[i + 1], indecies[i + 2], vertices)
        );
    }

    return polygons;
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
    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    f->glEnableVertexAttribArray(0);
    vao_.release();
}
