#include "renderer.h"

#include <QImage>
#include <QOpenGLTexture>
#include <QMatrix4x4>
#include <QDateTime>
#include <QKeyEvent>
#include <QtMath>
#include <QElapsedTimer>


static const float vertices[] = {
    // positions          // normals           // texture coords
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
    0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
    0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
    0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
    0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
    0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};


RendererWidget::RendererWidget(QWidget *parent)
    : QOpenGLWidget(parent)
    , cubeBuffer(QOpenGLBuffer::VertexBuffer)
    , lampBuffer(QOpenGLBuffer::VertexBuffer)
{
    //构成主widget的子窗口
    setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    hide();
    isLoad = false;
}

RendererWidget::~RendererWidget()
{
    cubeBuffer.destroy();
    lampBuffer.destroy();
}

void RendererWidget::initializeGL()
{
//    initializeOpenGLFunctions();
//    showNormal();
//    setGeometry(0, 0, 800, 600);

    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    initializeCubeGeometry();
    initializeLampGeometry();
    initializeShaders();

    container = initializeTexture(":/container2.png");
    container_specular = initializeTexture(":/container2_specular.png");
}

void RendererWidget::resizeGL(int w, int h)
{
    camera.setViewport(w, h);
    glViewport(0, 0, w, h);
}

void RendererWidget::paintGL()
{
//    QOpenGLExtraFunctions *f = QOpenGLContext::currentContext()->extraFunctions();
//    f->glClearColor(0.0f,0.0f,0.0f,1.0f);
//    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//    QMatrix4x4 vMatrix;
//    vMatrix.lookAt(cameraLocation_, QVector3D(0,0,0), QVector3D(0,1,0));

//    QMatrix4x4 mMatrix;
//    mMatrix.translate(0, -0.8);
//    //    mMatrix.rotate(angleX_,1,0,0);
//    mMatrix.rotate(angleY, 0, 1, 0);
//    //    mMatrix.rotate(anglZ_,0,0,1);
//    render_.render(f,pMatrix_,vMatrix,mMatrix,cameraLocation_,lightLocation_);
//    update();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    camera.update();

    static constexpr QVector3D cubePositions[] = {
        QVector3D( 0.0f,  0.0f,  0.0f),
        QVector3D( 2.0f,  5.0f, -15.0f),
        QVector3D(-1.5f, -2.2f, -2.5f),
        QVector3D(-3.8f, -2.0f, -12.3f),
        QVector3D( 2.4f, -0.4f, -3.5f),
        QVector3D(-1.7f,  3.0f, -7.5f),
        QVector3D( 1.3f, -2.0f, -2.5f),
        QVector3D( 1.5f,  2.0f, -2.5f),
        QVector3D( 1.5f,  0.2f, -1.5f),
        QVector3D(-1.3f,  1.0f, -1.5f)
    };
    int i = 0;
    for (const auto &position : cubePositions) {
        const float angle =  20.0f * i;
        ++i;
        drawCube(position, angle);
    }
    drawLamp();

    update();
}

void RendererWidget::loadModel(QString filePath){
    QImage *img = new QImage(":/awesomeface.png");
    render_.initsize(filePath, *img);
    cameraLocation_.setX(0);
    cameraLocation_.setY(0);
    cameraLocation_.setZ(3);
    lightLocation_.setX(10);
    lightLocation_.setY(10);
    lightLocation_.setZ(1);
    isLoad = true;
    connect(&tm_,SIGNAL(timeout()),this,SLOT(slotTimeout()));
    tm_.start(30);
    update();
}

void RendererWidget::slotTimeout()
{
    angleX_ += 5;
    angleY += 5;
    anglZ_ += 5;
    update();
}

void RendererWidget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_W:
        camera.move(Camera::MoveDirection::Direction_Forward);
        break;
    case Qt::Key_S:
        camera.move(Camera::MoveDirection::Direction_Backward);
        break;
    case Qt::Key_A:
        camera.move(Camera::MoveDirection::Direction_Left);
        break;
    case Qt::Key_D:
        camera.move(Camera::MoveDirection::Direction_Right);
        break;
    default:
        break;
    }
    QWidget::keyPressEvent(event);
}

void RendererWidget::keyReleaseEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_W:
        camera.move(Camera::MoveDirection::Direction_Forward, false);
        break;
    case Qt::Key_S:
        camera.move(Camera::MoveDirection::Direction_Backward, false);
        break;
    case Qt::Key_A:
        camera.move(Camera::MoveDirection::Direction_Left, false);
        break;
    case Qt::Key_D:
        camera.move(Camera::MoveDirection::Direction_Right, false);
        break;
    default:
        break;
    }
    QWidget::keyPressEvent(event);
}

void RendererWidget::mouseMoveEvent(QMouseEvent *event)
{
    static QPointF lastPos = event->pos();
    QPointF currentPos = event->pos();

    float xoffset = currentPos.x() - lastPos.x();
    float yoffset = -(currentPos.y() - lastPos.y());

    lastPos = currentPos;

    static constexpr float sensitivity = 0.2f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    camera.rotate(xoffset, yoffset);
    QWidget::mouseMoveEvent(event);
}

void RendererWidget::wheelEvent(QWheelEvent *event)
{
    camera.zoom(event->angleDelta().y()/100.0f);
    QWidget::wheelEvent(event);
}

void RendererWidget::initializeCubeGeometry()
{
    cubeVao.create();
    QOpenGLVertexArrayObject::Binder vaoBinder(&cubeVao);

    cubeBuffer.create();
    cubeBuffer.bind();
    cubeBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    cubeBuffer.allocate(vertices, sizeof(vertices));

    //  location, size(vec3), type, nomalize, stride(step), start position (offset)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*) 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
}


void RendererWidget::initializeLampGeometry()
{
    lampVao.create();
    QOpenGLVertexArrayObject::Binder vaoBinder(&lampVao);

    lampBuffer.create();
    lampBuffer.bind();
    lampBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    lampBuffer.allocate(vertices, sizeof(vertices));

    //  location, size(vec3), type, nomalize, stride(step), start position (offset)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*) 0);
    glEnableVertexAttribArray(0);
}

void RendererWidget::initializeShaders()
{
    if (!cubeProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vcube.vsh")
        || !cubeProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fcube.fsh")
        || !cubeProgram.link()) {
        qCritical() << "Shader add error:" << cubeProgram.log();
        close();
    }
    if (!lampProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vlamp.vsh")
        || !lampProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/flamp.fsh")
        || !lampProgram.link()) {
        qCritical() << "Shader add error:" << lampProgram.log();
        close();
    }
}

QOpenGLTexture *RendererWidget::initializeTexture(const QString &path)
{
    QOpenGLTexture *texture = new QOpenGLTexture(QImage(path).mirrored());

    // Set nearest filtering mode for texture minification
    texture->setMinificationFilter(QOpenGLTexture::Nearest);

    // Set bilinear filtering mode for texture magnification
    texture->setMagnificationFilter(QOpenGLTexture::Linear);

    // Wrap texture coordinates by repeating
    // f.ex. texture coordinate (1.1, 1.2) is same as (0.1, 0.2)
    texture->setWrapMode(QOpenGLTexture::Repeat);

    return texture;
}


void RendererWidget::drawCube(const QVector3D &position, float angle)
{
    cubeProgram.bind();

    QMatrix4x4 model;
    model.translate(position);
    model.rotate(angle, QVector3D(1.0f, 0.3f, 0.5f));
    QMatrix4x4 view = camera.view();
    QMatrix4x4 projection = camera.projection();

    cubeProgram.setUniformValue("model", model);
    cubeProgram.setUniformValue("view", view);
    cubeProgram.setUniformValue("projection", projection);
    cubeProgram.setUniformValue("viewPos", camera.position());

    glActiveTexture(GL_TEXTURE0);
    container->bind();
    glActiveTexture(GL_TEXTURE1);
    container_specular->bind();

    cubeProgram.setUniformValue("material.ambient",  1.0f, 0.5f, 0.31f);
    cubeProgram.setUniformValue("material.diffuse",  0);
    cubeProgram.setUniformValue("material.specular", 1);
    cubeProgram.setUniformValue("material.shininess", 32.0f);

    cubeProgram.setUniformValue("light.direction", -0.2f, -1.0f, -0.3f);
    cubeProgram.setUniformValue("light.ambient", 0.2f, 0.2f, 0.2f);
    cubeProgram.setUniformValue("light.diffuse", 0.5f, 0.5f, 0.5f); // darken the light a bit to fit the scene
    cubeProgram.setUniformValue("light.specular", 1.0f, 1.0f, 1.0f);

    QOpenGLVertexArrayObject::Binder vaoBinder(&cubeVao);

    glDrawArrays(GL_TRIANGLES, 0, 36);
    cubeProgram.release();
}

void RendererWidget::drawLamp()
{
    lampProgram.bind();

    QMatrix4x4 model;
    model.translate(lightPos);
    model.scale({0.2f, 0.2f, 0.2f});

    QMatrix4x4 view = camera.view();
    QMatrix4x4 projection = camera.projection();

    lampProgram.setUniformValue("model", model);
    lampProgram.setUniformValue("view", view);
    lampProgram.setUniformValue("projection", projection);

    QOpenGLVertexArrayObject::Binder vaoBinder(&lampVao);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    lampProgram.release();
}


