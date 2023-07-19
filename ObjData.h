#ifndef OBJDATA_H
#define OBJDATA_H
#include <QString>
#include <QVector>
#include <QMatrix4x4>
#include <QSet>
#include <QUuid>
#include <QOpenGLBuffer>

#include "convhull_3d.h"
//// 材质信息结构体
//class MaterialInfo
//{
//public:
//    QString strName[256]; // 纹理名称
//    float crAmbient[4];
//    float crDiffuse[4];
//    float crSpecular[4];
//    float fShiness;
//    uint nDiffuseMap;
//    uint nSpecularMap;
//    uint nBumpMap;
//    uint TexObjDiffuseMap;
//    uint TexObjSpecularMap;
//    uint TexObjBumpMap;
//};
class ConvhullData{
public:
    ch_vertex* vertices;
    int nVertices;
    int* faceIndices;
    int nFaces;
};

class facet{
public:
    facet();
    facet(QUuid _uid,
           QVector<unsigned int> &_vpiontsIndex,
           QVector<unsigned int> &_tpointsIndex,
           QVector<unsigned int> &_npointsIndex);

    QUuid uid;
    // v v/t/n v/t v//n
    QVector<unsigned int> vpointsIndex;//we haven't know the size of a facets until we loaded it.
    QVector<unsigned int> tpointsIndex;//there might be null while we are loading, so we might check if it exists.
    QVector<unsigned int> npointsIndex;//there might be null while we are loading, so we might check if it exists.
};


class PolarCoord{
public:
    unsigned int uid;
    double r;
    double theta;
    double phi;
    PolarCoord(unsigned int _uid, double _r, double _theta, double _phi);
};

/*
class StatusInRender{
position, rotation, scale
};
*/

//读入内存后的状态, 除了原本的模型信息, 还需要对模型整体进行调整, 如位移、旋转、缩放等.
//未来可能需要对顶点本身进行调整, 这本身还会影响面的法线,uv信息等
//实际渲染的指针也保存在这里？
class ObjData
{
public:
    ObjData();
    ~ObjData();

    void Rotate(float angleX_, float angleY_, float angleZ_);
    void LoadOnOpenGL(QVector<float> &vertPoints, QVector<float> &texturePoints, QVector<float> &normalPoints, bool isCH);
    void Transform_CH_Vertices(QVector<float> &vertPoints, ch_vertex *&vertices);
    QUuid uid;
    QString file;
    QString fileHash;
    QVector<float> vPoints, tPoints, nPoints;
    QVector<QString> objects;
    QVector<facet> facets;
    QHash<QString, QVector<int>> facetIndexesInObj; //for modifing a whole part of a objModel.
    QHash<int, QVector<int>> facetIndexesInSize; //for rendering the whole objModel on Screen by opengl functions correctly.
    //... a obj model might be more complex, such as: smooth, mtl, etc. and I will and be willing to fix all of them to build this project after I finish my current emergent works.
    // texture for rendering correct and comfortable for viewers.
    QSet<QString> mtls;

    QMatrix4x4 transform;

    QOpenGLBuffer vbo;
    QOpenGLBuffer ebo;
    ConvhullData chData;
    void InsertWatermarks(ch_vertex *vertices, int nVertex, std::string watermark, int _alpha = 13);
    void InitializeCHData();
};


#endif // OBJDATA_H
