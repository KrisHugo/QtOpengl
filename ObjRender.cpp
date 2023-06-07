#include "ObjRender.h"
#include <QDebug>
#include <QOpenGLTexture>
ObjRender::ObjRender()
    : vbo(QOpenGLBuffer::VertexBuffer),
    ebo(QOpenGLBuffer::IndexBuffer)
{
}
ObjRender::~ObjRender(){
    vbo.destroy();
    ebo.destroy();
}

void ObjRender::initsize(ObjData &objData)
{
//    qDebug() << "test1";
    if(objData.vPoints.size() <= 0 || objData.nPoints.size() <= 0 || objData.tPoints.size() <= 0){
        qDebug("Load failed");
        return;
    }
    //program
//    objProgram.bind();

    QVector<float> points;
    vertPoints_ << objData.vertPoints_;
    texturePoints_ << objData.texturePoints_;
    normalPoints_ << objData.normalPoints_;
    points << vertPoints_ << texturePoints_ << normalPoints_ ;
//    qDebug() << "test2";

//    vao.create();
    vbo.create();
    ebo.create();
    //VAO
//    vao.bind();
    //VBO
    vbo.bind();
    vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    vbo.allocate(points.data(), points.size() * sizeof(float)); // save in buffer
//    qDebug() << "test3";

    //EBO
    QVector<GLuint> indices;
    foreach(facet face, objData.facets){
        vindices << face.vpointsIndex;
        tindices << face.tpointsIndex;
        nindices << face.npointsIndex;
    }
    //    qDebug() << "test5";
    indices << vindices << tindices << nindices;
//        qDebug() << "test6";
    //创建IBO
    ebo.bind();
    ebo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    // 初始化IBO
    ebo.allocate(indices.data(), indices.size() * sizeof(GLuint));

    //Texture
    objTexture = new QOpenGLTexture(QImage(":/container2.png").mirrored());
    // Set nearest filtering mode for texture minification
    objTexture->setMinificationFilter(QOpenGLTexture::Nearest);
    // Set bilinear filtering mode for texture magnification
    objTexture->setMagnificationFilter(QOpenGLTexture::Linear);
    // Wrap texture coordinates by repeating
    // f.ex. texture coordinate (1.1, 1.2) is same as (0.1, 0.2)
    objTexture->setWrapMode(QOpenGLTexture::Repeat);

    vbo.release();
//    vao.release();
//    ebo.release();
//    objProgram.release();
}

void ObjRender::render(QOpenGLExtraFunctions *f, QMatrix4x4 &projectionM, QMatrix4x4 &viewM, QMatrix4x4 &modelM, QVector3D cameraLoc)
{
    f->glEnable(GL_DEPTH_TEST);
    objProgram.bind();//shader selected
    vbo.bind();// buffer selected
//    ebo.bind();// indices buffer selected;

//    qDebug() << vbo.size();
//    qDebug() << ebo.size();
//    qDebug() << projectionM;
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
//    f->glEnableVertexAttribArray(0);
//    f->glEnableVertexAttribArray(1); // Attribute indexes were received from calls to glGetAttribLocation, or passed into glBindAttribLocation.
//    f->glEnableVertexAttribArray(2);
//    //triangle
//    // offset
//    GLintptr vertex_position_offset = 0 * sizeof(float);
//    GLintptr vertex_texcoord_offset = 3 * sizeof(float) * vindices.size();
//    GLintptr vertex_normal_offset = 3 * sizeof(float) *  vindices.size() + 2 * sizeof(float) * tindices.size();
//    f->glVertexAttribPointer(0, 3, GL_FLOAT, false, 3 * sizeof(GLfloat), (GLvoid*)vertex_position_offset);
//    f->glVertexAttribPointer(1, 2, GL_FLOAT, false, 2 * sizeof(GLfloat), (GLvoid*)vertex_texcoord_offset);
//    f->glVertexAttribPointer(2, 3, GL_FLOAT, false, 3 * sizeof(GLfloat), (GLvoid*)vertex_normal_offset);

    f->glDrawArrays(GL_TRIANGLES, 0, vertPoints_.count() / 3);
//    QOpenGLVertexArrayObject::Binder bind(&vao);
//    vao.bind();
//    f->glDrawElements(GL_TRIANGLES, vindices.size(), GL_UNSIGNED_INT, (GLvoid *)0);

    //end here
    objProgram.disableAttributeArray(0);
    objProgram.disableAttributeArray(1);
    objProgram.disableAttributeArray(2);

    objTexture->release();
    vbo.release();
    vao.release();
    ebo.release();
    objProgram.release();
    f->glDisable(GL_DEPTH_TEST);
}
