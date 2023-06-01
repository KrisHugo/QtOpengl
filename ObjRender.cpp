#include "ObjRender.h"
#include "ObjLoader.h"
#include <QDebug>
#include <QOpenGLTexture>
void ObjRender::initsize(QString filename/*, QImage &textureImg*/)
{

    ObjLoader objModelLoader;
    objModelLoader.Load(filename, vertPoints_, texturePoints_, normalPoints_);
    if(vertPoints_.size() <= 0 ||texturePoints_.size() <= 0 || normalPoints_.size() <= 0){
        qDebug("Load failed");
        return;
    }
    QVector<float> points;
    points << vertPoints_ << normalPoints_ << texturePoints_ ;

//    objVao.create();
//    QOpenGLVertexArrayObject::Binder vaoBinder(&objVao);

    vbo_.create();
    vbo_.bind();
    vbo_.allocate(points.data(), points.size() * sizeof(float)); // save in buffer

    objTexture = new QOpenGLTexture(QImage(":/container2.png").mirrored());

    // Set nearest filtering mode for texture minification
    objTexture->setMinificationFilter(QOpenGLTexture::Nearest);

    // Set bilinear filtering mode for texture magnification
    objTexture->setMagnificationFilter(QOpenGLTexture::Linear);

    // Wrap texture coordinates by repeating
    // f.ex. texture coordinate (1.1, 1.2) is same as (0.1, 0.2)
    objTexture->setWrapMode(QOpenGLTexture::Repeat);
}

void ObjRender::render(QOpenGLExtraFunctions *f, QMatrix4x4 &projectionM, QMatrix4x4 &viewM, QMatrix4x4 &modelM, QVector3D cameraLoc, QVector3D lightCation)
{
//    f->glEnable(GL_DEPTH_TEST);
    objProgram.bind();//shader selected
    vbo_.bind();// buffer selected
    // qDebug() << vbo_.size();
    // shader
    objProgram.setUniformValue("projection", projectionM);
    objProgram.setUniformValue("view", viewM);
    objProgram.setUniformValue("model", modelM);
    objProgram.setUniformValue("viewPos", cameraLoc);
//    objProgram.setUniformValue("uLightLocation", lightCation);
//    objProgram.setUniformValue("sTexture", 0);

    f->glActiveTexture(GL_TEXTURE0);
    objTexture->bind();

    objProgram.setUniformValue("material.ambient",  1.0f, 0.5f, 0.31f);
    objProgram.setUniformValue("material.diffuse",  0);
    objProgram.setUniformValue("material.specular", 1);
    objProgram.setUniformValue("material.shininess", 32.0f);

    objProgram.setUniformValue("light.direction", -0.2f, -1.0f, -0.3f);
    objProgram.setUniformValue("light.ambient", 0.2f, 0.2f, 0.2f);
    objProgram.setUniformValue("light.diffuse", 0.5f, 0.5f, 0.5f); // darken the light a bit to fit the scene
    objProgram.setUniformValue("light.specular", 1.0f, 1.0f, 1.0f);

    objProgram.enableAttributeArray(0);
    objProgram.enableAttributeArray(1);
    objProgram.enableAttributeArray(2);
    objProgram.setAttributeBuffer(0, GL_FLOAT, 0, 3, 3 * sizeof(GLfloat));//vpoints
    objProgram.setAttributeBuffer(1, GL_FLOAT, vertPoints_.count() * sizeof(GLfloat), 3, 3 * sizeof(GLfloat));//npoints
    objProgram.setAttributeBuffer(2, GL_FLOAT, (vertPoints_.count() + texturePoints_.count()) * sizeof(GLfloat), 2, 2 * sizeof(GLfloat));//tpoints
//    objTexture->bind(0);

//    QOpenGLVertexArrayObject::Binder vaoBinder(&objVao);

    f->glDrawArrays(GL_TRIANGLES, 0, vertPoints_.count() / 3);

    //end here
    objProgram.disableAttributeArray(0);
    objProgram.disableAttributeArray(1);
    objProgram.disableAttributeArray(2);

    objTexture->release();
    vbo_.release();
    objProgram.release();
//    f->glDisable(GL_DEPTH_TEST);
}
