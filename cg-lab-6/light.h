#ifndef LIGHT_H
#define LIGHT_H

#include <QVector3D>

#include <type_traits>

enum LightOptions {
    L_AMBIENT  = 1,
    L_DIFFUSE  = 1 << 1,
    L_SPECULAR = 1 << 2,

    L_ALL = (1 << 3) - 1
};

inline LightOptions operator | (LightOptions a, LightOptions b) {
    using EnumT = std::underlying_type<LightOptions>::type;
    return static_cast<LightOptions>( static_cast<EnumT>( a ) | b );
}

inline LightOptions operator & (LightOptions a, LightOptions b) {
    using EnumT = std::underlying_type<LightOptions>::type;
    return static_cast<LightOptions>( static_cast<EnumT>( a ) & b );
}

inline LightOptions operator ~ (LightOptions a) {
    using EnumT = std::underlying_type<LightOptions>::type;
    return static_cast<LightOptions>( ~static_cast<EnumT>( a ) );
}

class Light
{
public:

    Light(QVector3D pos = QVector3D(0.0f, 0.0f, 0.0f),
          QVector3D color = QVector3D(1.0f, 1.0f, 1.0f),
          LightOptions opts = L_ALL);

    void setPos(QVector3D pos);
    void setColor(QVector3D color);

    void setOptions(LightOptions opts);
    void addOptions(LightOptions opts);
    void removeOptions(LightOptions opts);

    QVector3D    getPos()     const;
    QVector3D    getColor()   const;
    LightOptions getOptions() const;

private:
    QVector3D pos_;
    QVector3D color_;

    LightOptions options_;
};

#endif // LIGHT_H
