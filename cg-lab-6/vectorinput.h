#ifndef VECTORINPUT_H
#define VECTORINPUT_H

#include <QWidget>
#include <QDoubleSpinBox>
#include <QString>
#include <QVector3D>
#include <QGroupBox>
#include <QLabel>

class VectorInput : public QWidget
{
    Q_OBJECT
public:
    VectorInput(QWidget *parent = nullptr);

    void setLimits(double min, double max, double step = 1.0);

    void setGroupName(const QString& name);
    void setLabels   (const QString& x_name,
                      const QString& y_name,
                      const QString& z_name);
public slots:
    void changeVector(const QVector3D& new_vector);

private slots:
    void changeX(double);
    void changeY(double);
    void changeZ(double);

signals:
    void vectorChanged(QVector3D vector);

private:
    QDoubleSpinBox* x_;
    QDoubleSpinBox* y_;
    QDoubleSpinBox* z_;

    QGroupBox* group_;
    QLabel* x_label_;
    QLabel* y_label_;
    QLabel* z_label_;

    QVector3D value_;
};

#endif // VECTORINPUT_H
