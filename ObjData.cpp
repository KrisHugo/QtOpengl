#define CONVHULL_3D_ENABLE
#include "objdata.h"
#include <bitset>
#include <QtMath>
#include <binarymask.cpp>

ObjData::ObjData(): uid(),
    vPoints(), tPoints(), nPoints(), objects(), facets(),
    facetIndexesInObj(), facetIndexesInSize(), mtls(),
    vbo(QOpenGLBuffer::VertexBuffer),
    ebo(QOpenGLBuffer::IndexBuffer),
    chData()
{
}

ObjData::~ObjData(){

}

facet::facet():uid(), vpointsIndex(), tpointsIndex(), npointsIndex()
{
}

facet::facet(QUuid _uid,
               QVector<unsigned int> &_vpointsIndex,
               QVector<unsigned int> &_tpointsIndex,
               QVector<unsigned int> &_npointsIndex)
{
    uid = _uid;
    vpointsIndex = _vpointsIndex;
    tpointsIndex = _tpointsIndex;
    npointsIndex = _npointsIndex;
}

void ObjData::InitializeCHData(){
    if(chData.faceIndices != nullptr){
        free(chData.faceIndices);
    }
    if(chData.vertices != nullptr){
        free(chData.vertices);
    }

    //生成凸包的这个部分只在LoadOnOpenGL中做了，那意味着没有保存在本地，更加意味着破坏了整个程序应有的解耦性
    chData.faceIndices = nullptr;
    chData.nVertices = vPoints.size() / 3;
    chData.nFaces = 0;
    chData.vertices = nullptr;
    Transform_CH_Vertices(vPoints, chData.vertices);
    if(chData.vertices == nullptr) return;
    convhull_3d_build(chData.vertices, chData.nVertices, &chData.faceIndices, &chData.nFaces);
}

void ObjData::LoadOnOpenGL(QVector<float> &vertPoints, QVector<float> &texturePoints, QVector<float> &normalPoints, bool isCH = false){
    vertPoints.clear();
    texturePoints.clear();
    normalPoints.clear();
    if(isCH){
        if(chData.vertices == nullptr) {
            return;
        }
        qDebug() << "before CH check:" << vertPoints.size();
        qDebug() << "convhull build over :" << chData.nFaces;
        for(int i = 0; i < chData.nFaces; i++){
            for(int j = 0; j < 3; j++){
                vertPoints << chData.vertices[chData.faceIndices[i * 3 + j]].x;
                vertPoints << chData.vertices[chData.faceIndices[i * 3 + j]].y;
                vertPoints << chData.vertices[chData.faceIndices[i * 3 + j]].z;
                // 重新计算顶点法线
                normalPoints << 0;
                normalPoints << 0;
                normalPoints << 1;
                //
                texturePoints << 0;
                texturePoints << 1;

            }
        }
        qDebug() << "vertPoint Loaded Check:" << vertPoints.size();
    }
    else{
        //only extract the points which in one of the faces, and throw the others.
        //but only available for triangle facet
        for(auto &verFaceInfo : facets){
            for(int i = 0; i < verFaceInfo.vpointsIndex.size(); i++){
                unsigned int vIndex = verFaceInfo.vpointsIndex[i];
                unsigned int tIndex = verFaceInfo.tpointsIndex[i];
                unsigned int nIndex = verFaceInfo.npointsIndex[i];

                vertPoints << vPoints.at((vIndex) * 3);
                vertPoints << vPoints.at((vIndex) * 3 + 1);
                vertPoints << vPoints.at((vIndex) * 3 + 2);

                normalPoints << nPoints.at((nIndex) * 3);
                normalPoints << nPoints.at((nIndex) * 3 + 1);
                normalPoints << nPoints.at((nIndex) * 3 + 2);

                texturePoints << tPoints.at((tIndex) * 2);
                texturePoints << tPoints.at((tIndex) * 2 + 1);
            }
        }
    }
}


void ObjData::InsertWatermarks(ch_vertex *vertices, int nVertex, std::string watermark){

    //将watermark转化成二进制序列
    std::vector<int> wmvec;
    for (quint64 i = 0; i < watermark.length(); ++i) {
        int bs4(watermark[i]);
        wmvec.push_back(bs4);
    }
    QVector<PolarCoord> polarcoords;
    //读取顶点数据并转换成极坐标存储
    for(int i = 0; i < nVertex; i++){

        double r, theta, phi;
        //对x,y,z写入数据
        //先转换成二进制形式，然后确定尾数位所在位置，然后写入后两位
        //写入方式可以用二进制的移位删除后两位数据然后进行异或操作？
        //不能用二进制的方式来处理，而且需要保证水印强度
        double x = vertices[i].x;
        double y = vertices[i].y;
        double z = vertices[i].z;
//        qDebug() << x << " " << y << " " << z;
        r = qSqrt(qPow(x, 2) + qPow(y, 2) + qPow(z, 2));
        theta = qAcos(z/r);
        phi = qAtan2(y,x);//arctan 在 y 与 x 比较关系不同时是不同的函数;
        polarcoords.push_back(PolarCoord(i, r, theta, phi));

    }
    //排序
    //根据极坐标角度进行排序
    //保留原始序号,用于逆排序

    //依次将水印信息嵌入r中
    //计算纠错编码,保证水印数据能准确恢复？
    //确定嵌入位置和长度
    int wmLen = 2, start = 12, alpha = 5;
    int desination = sizeof(CH_FLOAT) - (start + alpha);//确定水印嵌入的起始位置
    quint64 rmask = maskGenerate(desination);//生成位掩码
    qDebug() << "rmask:" << rmask;
    // 用LSB实现, 后续应该单独写成函数
    // 通过强转指针的方式实现double数的位运算
    {
        int i = 0, j = 0;
        for(PolarCoord &pc : polarcoords){
            int wmPart = i % (sizeof(int)/wmLen);
            double *rp = &pc.r;
            qDebug() << "previous rp:" << *rp;
            quint64 rv = (*((quint64 *)rp) & rmask);
            quint64 lv = (*((quint64 *)rp) & ~rmask);
            qDebug() << "lv:         " << lv;
            qDebug() << "previous rv:" << rv;
            rv = rv>>wmLen; //右移留出嵌入水印的空位
            qDebug() << "moved rv:   " << rv;
            // 通过掩码的方式实现分段位移
            quint64 insert = initialWaterMark(wmvec[j], sizeof(int) - wmPart * wmLen, desination, wmLen);//选取出需要写入的部分, 并左移到需要嵌入的位置
            rv = rv | insert;
            qDebug() << "inserted rv:" << rv;
            if(wmPart == 0) j++;//水印位被写入完成, 写入下一个水印
            quint64 result = (lv | rv);
            quint64 *rp2 = &result;
            pc.r = *(double *)rp2;
            qDebug() << "modified rp:" << *rp;
            i++;
        }
    }
    //逆排序

    //坐标轴转换, 写回数据
    for(PolarCoord pc : polarcoords){
        int i = pc.uid;
        double r = pc.r;
        double theta = pc.theta;
        double phi = pc.phi;
        qDebug() << "before transverse:" << r;
        //对x,y,z写入数据
        //先转换成二进制形式，然后确定尾数位所在位置，然后写入后两位
        //写入方式可以用二进制的移位删除后两位数据然后进行异或操作？
        //不能用二进制的方式来处理，而且需要保证水印强度
        //float from double?
        vertices[i].x = r * qSin(theta) * qCos(phi);
        vertices[i].y = r * qSin(theta) * qSin(phi);
        vertices[i].z = r * qCos(theta);
    }
}

//void ObjData::ExtractWatermark(){

//}

void ObjData::Transform_CH_Vertices(QVector<float> &vertPoints, ch_vertex *&vertices)
{
    int num = vertPoints.size() / 3;
    vertices = (ch_vertex*)malloc(num * sizeof(ch_vertex));
    for (int i = 0; i < num; i++) {
        vertices[i].x = vertPoints[i * 3];
        vertices[i].y = vertPoints[i * 3 + 1];
        vertices[i].z = vertPoints[i * 3 + 2];
        qDebug() << i << ":" << num << ";" << vertices[i].x << "," << vertices[i].y << ","  << vertices[i].z ;
    }
    qDebug() << "Transform over";
}

void ObjData::Rotate(float angleX_ = 0.0, float angleY_ = 0.0, float angleZ_ = 0.0){
    transform.rotate(angleX_,1,0,0);
    transform.rotate(angleY_,0,1,0);
    transform.rotate(angleZ_,0,0,1);
}
//增加uid保证能还原回原本的顺序结构
PolarCoord::PolarCoord(unsigned int _uid, double _r, double _theta, double _phi)
{
    uid = _uid;
    r = _r;
    theta = _theta;
    phi = _phi;
}
