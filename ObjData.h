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

//class Group{
//public:
//    Group();
//    Group(QString matrial);
//    Group(QString matrial, QVector<QVector<std::tuple<int,int,int>>> &facets);
//    Group(QUuid uid, QString matrial, QVector<QVector<std::tuple<int,int,int>>> &facets);
//    QUuid uid;
//    QString matrial;//用于提取mtl数据的matrial信息
//    QVector<QVector<std::tuple<int,int,int>>> facets;//通过facets的索引来确定这个group的facets有哪些, 每个faces的顶点index是什么
//};

//class Obj{
//public:
//    Obj();
//    Obj(QString objName);
//    Obj(QUuid uid, QString objName, QVector<Group> &groups);
//    QUuid uid;
//    QString objName;
//    QVector<Group> groups;
//};

class facet{
public:
    facet();
    facet(QUuid _uid,
           QVector<unsigned int> &_vpiontsIndex,
           QVector<unsigned int> &_tpointsIndex,
           QVector<unsigned int> &_npointsIndex);
//    facets(QUuid _uid);
    QUuid uid;
    // v v/t/n v/t v//n
    QVector<unsigned int> vpointsIndex;//we haven't know the size of a facets until we loaded it.
    QVector<unsigned int> tpointsIndex;//there might be null while we are loading, so we might check if it exists.
    QVector<unsigned int> npointsIndex;//there might be null while we are loading, so we might check if it exists.
};


/*
class StatusInRender{
position, rotation, scale
};
*/

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
    QVector<float> vPoints;
    QVector<float> tPoints;
    QVector<float> nPoints;
    QVector<QString> objects;
    QVector<facet> facets;
    QHash<QString, QVector<int>> facetIndexesInObj; //for modifing a whole part of a objModel.
    QHash<int, QVector<int>> facetIndexesInSize; //for rendering the whole objModel on Screen by opengl functions correctly.
    //... a obj model might be more complex, such as: smooth, mtl, etc. and I will and be willing to fix all of them to build this project after I finish my current emergent works.
    // texture for rendering correct and comfortable for viewers.
    QSet<QString> mtls;

    QVector3D position;
    QVector<float> vertPoints_, texturePoints_, normalPoints_;
};

#endif // OBJDATA_H
