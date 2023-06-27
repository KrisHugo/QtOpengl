#include "ObjRender.h"
#include <QDebug>
#include <QOpenGLTexture>
ObjRender::ObjRender()
    : vertPoints_(), texturePoints_(), normalPoints_()
{

}
ObjRender::~ObjRender()
{

}

void ObjRender::initsize(ObjData &objData, bool isCH)
{
    curData = &objData;
    if(curData->vPoints.size() <= 0 || curData->nPoints.size() <= 0 || curData->tPoints.size() <= 0){
        qDebug("Load failed");
        return;
    }
    QVector<float> points;
    curData->LoadOnOpenGL(vertPoints_, texturePoints_, normalPoints_, isCH);
    points << vertPoints_ << texturePoints_ << normalPoints_ ;

    curData->vbo.create();
    curData->ebo.create();
    //VBO
    curData->vbo.bind();
    curData->vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    curData->vbo.allocate(points.data(), points.size() * sizeof(float)); // save in buffer

    //actually doesn't work.
    //Texture
    objTexture = new QOpenGLTexture(QImage(":/container2.png").mirrored());
    // Set nearest filtering mode for texture minification
    objTexture->setMinificationFilter(QOpenGLTexture::Nearest);
    // Set bilinear filtering mode for texture magnification
    objTexture->setMagnificationFilter(QOpenGLTexture::Linear);
    // Wrap texture coordinates by repeating
    // f.ex. texture coordinate (1.1, 1.2) is same as (0.1, 0.2)
    objTexture->setWrapMode(QOpenGLTexture::Repeat);

    curData->vbo.release();
    curData->transform.translate(QVector3D(0.0f,  0.0f,  0.0f));
}

void ObjRender::render(QOpenGLExtraFunctions *f, QMatrix4x4 &projectionM, QMatrix4x4 &viewM, QVector3D cameraLoc, bool mode)
{
    //    qDebug() << "test1";
    if(curData == nullptr){
        qDebug("Unloaded");
        return;
    }

    f->glEnable(GL_DEPTH_TEST);
    objProgram.bind();//shader selected
    curData->vbo.bind();// buffer selected
//    curData->ebo.bind();// indices buffer selected;
    // shader
    objProgram.setUniformValue("projection", projectionM);
    objProgram.setUniformValue("view", viewM);
    objProgram.setUniformValue("model", curData->transform);
    objProgram.setUniformValue("viewPos", cameraLoc);

    f->glActiveTexture(GL_TEXTURE0);
    objTexture->bind();

    objProgram.setUniformValue("material.ambient",  1.0f, 0.5f, 0.31f);
    objProgram.setUniformValue("material.diffuse",  1.0f, 0.5f, 0.31f);
    objProgram.setUniformValue("material.specular", 0.5f, 0.5f, 0.5f);
    objProgram.setUniformValue("material.shininess", 32.0f);

    objProgram.setUniformValue("light.direction", -0.2f, -1.0f, -0.3f);
    objProgram.setUniformValue("light.ambient", 0.2f, 0.2f, 0.2f);
    objProgram.setUniformValue("light.diffuse", 0.5f, 0.5f, 0.5f); // darken the light a bit to fit the scene
    objProgram.setUniformValue("light.specular", 1.0f, 1.0f, 1.0f);

    //position, normal, texture
    objProgram.enableAttributeArray(0);
    objProgram.enableAttributeArray(1);
    objProgram.enableAttributeArray(2);
    //vertex buffer
    objProgram.setAttributeBuffer(0, GL_FLOAT, 0, 3, 3 * sizeof(GLfloat)); //vpoints
    objProgram.setAttributeBuffer(1, GL_FLOAT, vertPoints_.size() * sizeof(GLfloat), 2, 2 * sizeof(GLfloat)); //tpoints
    objProgram.setAttributeBuffer(2, GL_FLOAT, (vertPoints_.size() + texturePoints_.size()) * sizeof(GLfloat), 3, 3 * sizeof(GLfloat)); //npoints
    qDebug() << mode;
    f->glDrawArrays((mode? GL_LINE_LOOP : GL_TRIANGLES), 0, vertPoints_.count() / 3);
    //end here
    objProgram.disableAttributeArray(0);
    objProgram.disableAttributeArray(1);
    objProgram.disableAttributeArray(2);

    objTexture->release();
    curData->vbo.release();
    //curData->ebo.release();
    objProgram.release();
    f->glDisable(GL_DEPTH_TEST);
}

void ObjRender::unLoad()
{
//    curData->vbo.bind();
    curData->vbo.destroy();
    qDebug() << curData->vbo.size();
//    curData->ebo.destroy();
}

