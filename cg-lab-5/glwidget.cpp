#include <QOpenGLFunctions>
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
    return QSize(1000, 1000);
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
        view_matrix_.translate(0, 0, -zoom_);
        update();
    }
}

void GLwidget::setSideCount(int n)
{

    if (n != side_count_)
    {
        side_count_ = n;
        HorseshoeMesh horseshoe_mesh;
        horseshoe_mesh.createHorseshoe(n);
        data_ = horseshoe_mesh.getData();
        emit sideCountChanged(n);
        update();
    }
}

void GLwidget::setLight(Light l)
{
    light_ = l;
    emit lightChanged(l);

    rot_look_at_ = calculatePlaneRotation(
                QVector3D(0.0f, 1.0f, 0.0f),
                light_.getPos() != QVector3D(0, 0, 0) ?
                    light_.getPos() :
                    QVector3D(0.0f, 1.0f, 0.0f) );
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
    HorseshoeMesh light_mesh;
    light_mesh.createHorseshoe(3);
    light_data_ = light_mesh.getData();

    side_count_ = 0;
    setSideCount(10);
    setLight(Light(QVector3D(0.0f, 1.0f, 0.0f)));
    rot_look_at_ = calculatePlaneRotation(QVector3D(0.0f, 1.0f, 0.0f), light_.getPos());

    setXRotation(396 * 16);
    setYRotation(0);
    setZoom(4);

    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);

    light_shader_.addShaderFromSourceFile(QOpenGLShader::Vertex,   ":/light_vertex_shader.vsh");
    light_shader_.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/light_fragment_shader.fsh");
    light_shader_.link();
    if (!light_shader_.isLinked())
    {
        std::cerr << light_shader_.log().toStdString() << std::endl;
    }

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
    vbo_.allocate(0, 27 * sizeof(GLfloat));

    setupVaoAttribs();

    vao_.release();

    view_matrix_.setToIdentity();
    view_matrix_.translate(0, 0, -zoom_);
}

void GLwidget::paintGL()
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
    shader_.setUniformValue("rotation_angle",   angle_);
    shader_.setUniformValue("rotation_look_at", rot_look_at_);
    shader_.setUniformValue("light_pos",   light_.getPos());
    shader_.setUniformValue("light_color", light_.getColor());
    shader_.setUniformValue("ambt_bool",   light_.getOptions() & LightOptions::L_AMBIENT);
    shader_.setUniformValue("diff_bool",   light_.getOptions() & LightOptions::L_DIFFUSE);
    shader_.setUniformValue("spec_bool",   light_.getOptions() & LightOptions::L_SPECULAR);
    shader_.setUniformValue("view_pos",    QVector3D(0.0f, 0.0f, -1.0f));


    for (size_t i = 0; i < data_.size(); i += 27) {
        vbo_.write(0, &data_[i], 27 * sizeof(GLfloat));
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }

    shader_.release();
    light_shader_.bind();

    model_matrix_.translate(light_.getPos());
    model_matrix_.scale(0.2f, 0.2f, 0.2f);

    light_shader_.setUniformValue("model", model_matrix_);
    light_shader_.setUniformValue("view",  view_matrix_);
    light_shader_.setUniformValue("proj",  proj_matrix_);
    light_shader_.setUniformValue("color", light_.getColor());

    // Ideally, there would be a seperate VBO with a light cube
    // instead of "light horseshoe"
    // But Qt tried it's best to not work with multiple VAO's
    // And succeeded
    for (size_t i = 0; i < light_data_.size(); i += 27) {
        vbo_.write(0, &light_data_[i], 27 * sizeof(GLfloat));
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }

    light_shader_.release();
    vao_.release();

    constexpr float angle_increment = 2 * M_PI / 180;
    angle_ += angle_increment;
    update();
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

QMatrix3x3 GLwidget::calculatePlaneRotation(const QVector3D& vec1, const QVector3D& vec2)
{
    QVector3D norm1 = vec1.normalized();
    QVector3D norm2 = vec2.normalized();

    QMatrix3x3 rot;
    rot.setToIdentity();

    if (norm1 == norm2) { return rot; }
    else if (norm1 == -norm2) { return -rot; }

    QVector3D cross    = QVector3D::crossProduct(norm1, norm2);
    float     sin_val  = cross.length();
    float     cos_val  = QVector3D::dotProduct(norm1, norm2);
    float     nega_cos = 1 - cos_val;
    QVector3D axis     = cross / sin_val;

    float values[] = {
        (axis.x() * axis.x() * nega_cos) + cos_val,
        (axis.y() * axis.x() * nega_cos) - (sin_val * axis.z()),
        (axis.z() * axis.x() * nega_cos) + (sin_val * axis.y()),
        (axis.x() * axis.y() * nega_cos) + (sin_val * axis.z()),
        (axis.y() * axis.y() * nega_cos) + cos_val,
        (axis.z() * axis.y() * nega_cos) - (sin_val * axis.x()),
        (axis.x() * axis.z() * nega_cos) - (sin_val * axis.y()),
        (axis.y() * axis.z() * nega_cos) + (sin_val * axis.x()),
        (axis.z() * axis.z() * nega_cos) + cos_val
    };

    return QMatrix3x3(values);
}

void GLwidget::setupVaoAttribs()
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
