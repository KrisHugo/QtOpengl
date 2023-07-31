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

#include "stb_image.h"

class QOpenGLTexture;

class OpenGLWidget : public QOpenGLWidget, QOpenGLFunctions_3_3_Core
{
    Q_OBJECT

public:
    OpenGLWidget(QWidget *parent = nullptr);
    ~OpenGLWidget();
    void loadModel(ObjData &objdata, bool isCH);
    void unLoadModel();
    bool IsLoad();
public slots:
    void SwitchMode(int newMode);
protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    bool eventFilter(QObject *obj, QEvent *event) override;

    bool lineMode = false;

private:
    //camera
    Camera camera;
    QMatrix4x4 view;
    QMatrix4x4 projection;

    bool loadingFlag;
    ObjRender objRender;
    QTimer tm_;
    //useless
    QVector3D cameraLocation_, lightLocation_;
    qreal angleX_,angleY_,angleZ_;

    void initializeCubeGeometry();
    void initializeLampGeometry();
    void initializeObjShaders();
    QOpenGLTexture *loadTexture(const QString &path, bool isRepeat = false);

    void drawCube(const QVector3D &position, float angle);
    void drawLamp();
    void drawModel();

    // obj render
    QOpenGLBuffer cubeBuffer;
    QOpenGLBuffer lampBuffer;
    QOpenGLVertexArrayObject cubeVao;
    QOpenGLVertexArrayObject lampVao;
    QOpenGLShaderProgram cubeProgram;
    QOpenGLShaderProgram lampProgram;
    QOpenGLTexture *container = nullptr;
    QOpenGLTexture *container_specular = nullptr;
    // lighting
    const QVector3D lightPos = {1.2f, 1.0f, 2.0f};

    // background render
//    GLuint skyboxVAO;
    QOpenGLBuffer skyboxVBO;
    QOpenGLTexture* skyboxTexture;
    QOpenGLShaderProgram skyboxProgram;
    QMatrix4x4 skyboxProjection;
//    QOpenGLBuffer skyboxVBO;

    void initSkybox()
    {

        float vertices[] = {
            // positions
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            -1.0f,  1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f,  1.0f
        };
        skyboxVBO.create();
        skyboxVBO.bind();
        skyboxVBO.setUsagePattern(QOpenGLBuffer::StaticDraw);
        skyboxVBO.allocate(vertices, sizeof(vertices));

        QImage skyboxs[6];
        QVector<QString> faces
            {
            ":/right.jpg",
            ":/left.jpg",
            ":/top.jpg",
            ":/bottom.jpg",
            ":/back.jpg",
            ":/front.jpg",
            };
        // Load the skybox textures
        for(int i = 0; i < 6; i++){
            skyboxs[i] = QImage(faces[i]).convertToFormat(QImage::Format_RGB888);
        }

        //注释变量类型，QOpenGLTexture *texture;
        skyboxTexture = new QOpenGLTexture(QOpenGLTexture::TargetCubeMap);

        skyboxTexture->setSize(skyboxs[0].width(), skyboxs[0].height(), skyboxs[0].depth()); //这个我猜测 是确定一面纹理的尺寸，然后allocate分配函数，根据TargeCubeMap，分配六面纹理的空间
        skyboxTexture->setFormat(QOpenGLTexture::RGBFormat);  //Vries设置的就是GL_RGB,这里同步
        skyboxTexture->allocateStorage(QOpenGLTexture::RGB, QOpenGLTexture::UInt8); //分配内存 ，UInt8等价于GL_UNSIGNED_BYTE

        skyboxTexture->setData(0, 0, QOpenGLTexture::CubeMapPositiveX, QOpenGLTexture::RGB, QOpenGLTexture::UInt8, (const void*)skyboxs[0].bits());
        skyboxTexture->setData(0, 0, QOpenGLTexture::CubeMapNegativeX, QOpenGLTexture::RGB, QOpenGLTexture::UInt8, (const void*)skyboxs[1].bits());
        skyboxTexture->setData(0, 0, QOpenGLTexture::CubeMapPositiveY, QOpenGLTexture::RGB, QOpenGLTexture::UInt8, (const void*)skyboxs[2].bits());
        skyboxTexture->setData(0, 0, QOpenGLTexture::CubeMapNegativeY, QOpenGLTexture::RGB, QOpenGLTexture::UInt8, (const void*)skyboxs[3].bits());
        skyboxTexture->setData(0, 0, QOpenGLTexture::CubeMapPositiveZ, QOpenGLTexture::RGB, QOpenGLTexture::UInt8, (const void*)skyboxs[4].bits());
        skyboxTexture->setData(0, 0, QOpenGLTexture::CubeMapNegativeZ, QOpenGLTexture::RGB, QOpenGLTexture::UInt8, (const void*)skyboxs[5].bits());

        skyboxTexture->setMinificationFilter(QOpenGLTexture::Linear);     //纹理放大或缩小时，像素的取值方法 ，线性或就近抉择
        skyboxTexture->setMagnificationFilter(QOpenGLTexture::Linear);
        skyboxTexture->setWrapMode(QOpenGLTexture::DirectionS, QOpenGLTexture::ClampToEdge);   //设置纹理边缘的扩展方法
        skyboxTexture->setWrapMode(QOpenGLTexture::DirectionT, QOpenGLTexture::ClampToEdge);


        // Initialize the shader program
        if (!skyboxProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vsrc.vsh")
            || !skyboxProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fsrc.fsh")
            || !skyboxProgram.link()) {
            qCritical() << "SkyBox Shader add error:" << lampProgram.log();
            close();
        }
    }

    // Draw the skybox
    void drawSkybox()
    {
        glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
        skyboxProgram.bind();

        //bind texture
        glActiveTexture(GL_TEXTURE0);
        skyboxTexture->bind();

        QMatrix4x4 skyView;
        skyView.setRow(0, QVector4D(view(0, 0), view(0, 1), view(0, 2), 0.0f));
        skyView.setRow(1, QVector4D(view(1, 0), view(1, 1), view(1, 2), 0.0f));
        skyView.setRow(2, QVector4D(view(2, 0), view(2, 1), view(2, 2), 0.0f));
        skyView.setRow(3, QVector4D(0.0f,       0.0f,       0.0f,       1.0f));
        QMatrix4x4 projection = camera.projection();

        skyboxProgram.setUniformValue("view", skyView);
        skyboxProgram.setUniformValue("projection", projection);
        skyboxProgram.setUniformValue("skybox", 0);

        skyboxVBO.bind();
        skyboxProgram.enableAttributeArray(0);
        skyboxProgram.setAttributeBuffer(0, GL_FLOAT, 0, 3, 3 * sizeof(GLfloat)); //vpoints
        glDrawArrays(GL_TRIANGLES, 0, 36);

        skyboxProgram.disableAttributeArray(0);
        skyboxVBO.release();
        skyboxProgram.release();
        glDisable(GL_LESS); // set depth function back to default
    }

private slots:
    void slotTimeout();

};
#endif // OPENGLWIDGETS_H
