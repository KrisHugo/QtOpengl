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
    ObjRender();
    ~ObjRender();
    void initsize(ObjData &objData);
    void render(QOpenGLExtraFunctions *f, QMatrix4x4 &pMatrix, QMatrix4x4 &vMatrix, QMatrix4x4 &mMatrix, QVector3D cameraLocation);
    QOpenGLShaderProgram objProgram;
private:
    QOpenGLTexture *objTexture = nullptr;
//    QOpenGLVertexArrayObject objVao;
    QOpenGLBuffer vbo;
    QOpenGLBuffer ebo;
    ObjData *data = nullptr;
    QVector<unsigned int> vindices, tindices, nindices;

    QVector<float> vertPoints_, texturePoints_, normalPoints_;
    QOpenGLVertexArrayObject vao;

};

#endif // OBJRENDER_H
