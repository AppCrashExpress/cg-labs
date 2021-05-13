#ifndef LIGHTMENU_H
#define LIGHTMENU_H

#include <QWidget>
#include <QCheckBox>
#include <QVector3D>

#include "light.h"

class LightMenu : public QWidget
{
    Q_OBJECT
public:
    LightMenu(QWidget *parent = nullptr);

private slots:
    void setPos   (const QVector3D& pos);
    void setColor (const QVector3D& color);

    void setAmbient (int state);
    void setDiffuse (int state);
    void setSpecular(int state);

signals:
    void lightChanged(Light l);

private:
    QCheckBox* ambt_check_;
    QCheckBox* diff_check_;
    QCheckBox* spec_check_;

    Light light_;
};

#endif // LIGHTMENU_H
