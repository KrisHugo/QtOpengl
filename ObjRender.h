#ifndef OBJRENDER_H
#define OBJRENDER_H


#include <QOpenGLShaderProgram>
#include <QOpenGLExtraFunctions>
#include <QOpenGLTexture>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include "ObjData.h"
class ObjRender
{
public:
    ObjRender() = default;
    ~ObjRender();
    void initsize(ObjData &objData);
    void render(QOpenGLExtraFunctions *f, QMatrix4x4 &pMatrix, QMatrix4x4 &vMatrix, QMatrix4x4 &mMatrix, QVector3D cameraLocation, QVector3D lightCation);
    QOpenGLShaderProgram objProgram;
private:
    QOpenGLTexture *objTexture = nullptr;
    QOpenGLVertexArrayObject objVao;
    QOpenGLBuffer vbo_;
    ObjData *data = nullptr;
    QVector<float> vertPoints_, texturePoints_, normalPoints_;

};

#endif // OBJRENDER_H
