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
    void initsize(ObjData &objData, bool isCH = false);
    void render(QOpenGLExtraFunctions *f, QMatrix4x4 &pMatrix, QMatrix4x4 &vMatrix, QVector3D cameraLocation);
    void unLoad();
    QOpenGLShaderProgram objProgram;
private:
    QOpenGLTexture *objTexture = nullptr;
    ObjData *curData = nullptr;
    QVector<unsigned int> vindices, tindices, nindices;

    QVector<float> vertPoints_, texturePoints_, normalPoints_;
//    QOpenGLVertexArrayObject vao;
};

#endif // OBJRENDER_H
