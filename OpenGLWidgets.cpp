#include "OpenGLWidgets.h"

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


OpenGLWidget::OpenGLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
    , cubeBuffer(QOpenGLBuffer::VertexBuffer)
    , lampBuffer(QOpenGLBuffer::VertexBuffer)
{

    setFocusPolicy(Qt::StrongFocus);//强制focus opengl widgets
    //构成主widget的子窗口
    setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    hide();
    loadingFlag = false;
    installEventFilter(this);

//    connect(&tm_, SIGNAL(timeout()), this, SLOT(slotTimeout()));
//    tm_.start(30);
}

OpenGLWidget::~OpenGLWidget()
{
    cubeBuffer.destroy();
    lampBuffer.destroy();
}

void OpenGLWidget::initializeGL()
{

    initializeOpenGLFunctions();
    showNormal();
    setGeometry(0, 0, 800, 600);

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

//    initializeCubeGeometry();
//    initializeLampGeometry();
    initializeShaders();

    container = initializeTexture(":/container2.png");
    container_specular = initializeTexture(":/container2_specular.png");
}

void OpenGLWidget::resizeGL(int w, int h)
{
    camera.setViewport(w, h);
    glViewport(0, 0, w, h);

    pMatrix_.setToIdentity();
    pMatrix_.perspective(45, float(w)/h, 0.01f, 100.0f);
}

void OpenGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    camera.update();
//    static constexpr QVector3D cubePositions[] = {
////        QVector3D( 0.0f,  0.0f,  0.0f),
//        QVector3D( 2.0f,  5.0f, -15.0f),
//        QVector3D(-1.5f, -2.2f, -2.5f),
//        QVector3D(-3.8f, -2.0f, -12.3f),
//        QVector3D( 2.4f, -0.4f, -3.5f),
//        QVector3D(-1.7f,  3.0f, -7.5f),
//        QVector3D( 1.3f, -2.0f, -2.5f),
//        QVector3D( 1.5f,  2.0f, -2.5f),
//        QVector3D( 1.5f,  0.2f, -1.5f),
//        QVector3D(-1.3f,  1.0f, -1.5f)
//    };
//    int i = 0;
//    for (const auto &position : cubePositions) {
//        const float angle =  20.0f * i;
//        ++i;
//        drawCube(position, angle);
//    }
//    drawLamp();

    if(loadingFlag){
        drawModel();
    }
    update();
}

void OpenGLWidget::loadModel(ObjData &objData){
    objRender.initsize(objData);
    loadingFlag = true;
}

void OpenGLWidget::unLoadModel(){
    objRender.unLoad();
    loadingFlag = false;
}

bool OpenGLWidget::IsLoad()
{
    return loadingFlag;
}

void OpenGLWidget::drawModel(){
    QOpenGLExtraFunctions *f = QOpenGLContext::currentContext()->extraFunctions();
    QMatrix4x4 vMatrix = camera.view();

    objRender.render(f, pMatrix_, vMatrix, camera.position());
    update();
}


void OpenGLWidget::slotTimeout()
{
//    qDebug() << "test";
    // rotate the obj
//    angleX_ += 5;
//    angleY_ += 5;
//    angleZ_ += 5;
//    QVector3D position3 = QVector3D( angleX_,  angleY,  anglZ_);
//    drawCube(position3, 20);
    update();
}

bool OpenGLWidget::eventFilter(QObject *obj, QEvent *event)
{

    if (event->type() == QEvent::KeyPress)
    {
        qDebug() << event->type();
        return QWidget::eventFilter(obj, event);
    }
    else if(event->type() == QEvent::KeyRelease)
    {
        qDebug() << event->type();
        return QWidget::eventFilter(obj, event);
    }
    else
    {
        return QWidget::eventFilter(obj, event);
    }
//    return QWidget::eventFilter(obj, event);
}

void OpenGLWidget::keyPressEvent(QKeyEvent *event)
{
    qDebug() << event->key();

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

void OpenGLWidget::keyReleaseEvent(QKeyEvent *event)
{
    qDebug() << event->key();

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


void OpenGLWidget::mouseMoveEvent(QMouseEvent *event)
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

void OpenGLWidget::wheelEvent(QWheelEvent *event)
{
    camera.zoom(event->angleDelta().y()/100.0f);
    QWidget::wheelEvent(event);
}

void OpenGLWidget::initializeCubeGeometry()
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


void OpenGLWidget::initializeLampGeometry()
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

void OpenGLWidget::initializeShaders()
{
    if (!objRender.objProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vcube.vsh")
        || !objRender.objProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fcube.fsh")
        || !objRender.objProgram.link()) {
        qCritical() << "Shader add error:" << cubeProgram.log();
        close();
    }
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

QOpenGLTexture *OpenGLWidget::initializeTexture(const QString &path)
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


void OpenGLWidget::drawCube(const QVector3D &position, float angle)
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

void OpenGLWidget::drawLamp()
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


