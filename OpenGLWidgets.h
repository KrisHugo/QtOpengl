#ifndef OPENGLWIDGETS_H
#define OPENGLWIDGETS_H
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QScopedPointer>
#include <QTimer>
#include "ObjRender.h"
#include "camera.h"
class QOpenGLTexture;

class OpenGLWidget : public QOpenGLWidget, QOpenGLFunctions_3_3_Core
{
    Q_OBJECT

public:
    OpenGLWidget(QWidget *parent = nullptr);
    ~OpenGLWidget();
    void loadModel(ObjData &objdata);
    void unLoadModel();
    bool IsLoad();
protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    bool eventFilter(QObject *obj, QEvent *event) override;


private:
    bool loadingFlag;
    ObjRender objRender;
    QTimer tm_;
    QVector3D cameraLocation_, lightLocation_;
    QMatrix4x4 pMatrix_;
    qreal angleX_,angleY_,angleZ_;

    Camera camera;

    void initializeCubeGeometry();
    void initializeLampGeometry();
    void initializeShaders();
    QOpenGLTexture *initializeTexture(const QString &path);

    void drawCube(const QVector3D &position, float angle);
    void drawLamp();
    void drawModel();

    QOpenGLBuffer cubeBuffer;
    QOpenGLBuffer lampBuffer;
    QOpenGLVertexArrayObject cubeVao;
    QOpenGLVertexArrayObject lampVao;
    QOpenGLShaderProgram cubeProgram;
    QOpenGLShaderProgram lampProgram;
    QOpenGLTexture *container = nullptr;
    QOpenGLTexture *container_specular = nullptr;

    const QVector3D lightPos = {1.2f, 1.0f, 2.0f};

private slots:
    void slotTimeout();

};
#endif // OPENGLWIDGETS_H
