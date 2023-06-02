#ifndef OBJDATA_H
#define OBJDATA_H
#include <QString>
#include <QVector>
#include <QVector3D>
#include <QSet>
#include <QUuid>

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

//// 对象信息结构体
//class ObjObject
//{
//public:
//    int nMaterialID; // 纹理ID
//    bool bHasTexture; // 是否具有纹理映射
//    bool bHasNormal; // 是否具有法线
//    QVector<QVector3D> PosVerts; // 对象的顶点
//    QVector<QVector3D> Normals; // 对象的法向量
//    QVector<QVector2D> Texcoords; // 纹理UV坐标
//    QVector<unsigned short> Indexes; // 对象的顶点索引
//    unsigned int nNumIndexes; // 索引数目
//    uint nPosVBO;
//    uint nNormVBO;
//    uint nTexcoordVBO;
//    uint nIndexVBO;
//};

////模型信息结构体
//class ObjModel
//{
//public:
//    bool bIsTextured; //是否使用纹理
//    QVector<MaterialInfo> material; // 材质信息
//    QVector<ObjObject> group; // 模型中对象信息
//};

class Group{
public:
    Group();
    Group(QString matrial);
    Group(QString matrial, QVector<QVector<std::tuple<int,int,int>>> &facets);
    Group(QUuid uid, QString matrial, QVector<QVector<std::tuple<int,int,int>>> &facets);
    QUuid uid;
    QString matrial;//用于提取mtl数据的matrial信息
    QVector<QVector<std::tuple<int,int,int>>> facets;//通过facets的索引来确定这个group的facets有哪些, 每个faces的顶点index是什么
};

class Obj{
public:
    Obj();
    Obj(QString objName);
    Obj(QUuid uid, QString objName, QVector<Group> &groups);
    QUuid uid;
    QString objName;
    QVector<Group> groups;
};


//读入内存后的状态, 除了原本的模型信息, 还需要对模型整体进行调整, 如位移、旋转、缩放等.
//未来可能需要对顶点本身进行调整, 这本身还会影响面的法线,uv信息等
class ObjData
{
public:
    ObjData();
    ~ObjData();
    //    void load(QString file, QVector<float> &vPoints, QVector<float> &tPoints, QVector<float> &nPoints);
    QUuid uid;
    QString file;

    QVector<QVector3D> vPoints, nPoints;
    QVector<QVector2D> tPoints;
    //tuple: v t n indexs
//    QVector<std::tuple<int,int,int>> facetIndexs;
    QVector<Obj> objects;
    QSet<QString> mtls;

    QVector3D position;
};

#endif // OBJDATA_H
