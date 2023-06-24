#define CONVHULL_3D_ENABLE
#include "objdata.h"



ObjData::ObjData(): uid(),
    vPoints(), tPoints(), nPoints(), objects(), facets(),
    facetIndexesInObj(), facetIndexesInSize(), mtls(),
    vbo(QOpenGLBuffer::VertexBuffer),
    ebo(QOpenGLBuffer::IndexBuffer)
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

void ObjData::LoadOnOpenGL(QVector<float> &vertPoints, QVector<float> &texturePoints, QVector<float> &normalPoints, bool isCH = false){
    vertPoints.clear();
    texturePoints.clear();
    normalPoints.clear();
    if(isCH){
        int* faceIndices = nullptr;
        int nFaces = 0;
        vertices = nullptr;
        Transform_CH_Vertices(vPoints, vertices);
        if(vertices == nullptr) qDebug() << "test";
        convhull_3d_build(vertices, vPoints.size() / 3, &faceIndices, &nFaces);
        if(vertices == nullptr) return;
        qDebug() << "convhull build over :" << nFaces;
        for(int i = 0; i < nFaces; i++){
            for(int j = 0; j < 3; j++){
                vertPoints << vertices[faceIndices[i * 3 + j]].x;
                vertPoints << vertices[faceIndices[i * 3 + j]].y;
                vertPoints << vertices[faceIndices[i * 3 + j]].z;

                texturePoints << 0;
                texturePoints << 1;

                normalPoints << 0;
                normalPoints << 0;
                normalPoints << 1;
            }
        }
        qDebug() << vertPoints.size();

        free(vertices);
        free(faceIndices);
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

                texturePoints << tPoints.at((tIndex) * 2);
                texturePoints << tPoints.at((tIndex) * 2 + 1);

                normalPoints << nPoints.at((nIndex) * 3);
                normalPoints << nPoints.at((nIndex) * 3 + 1);
                normalPoints << nPoints.at((nIndex) * 3 + 2);
            }
        }
    }
}

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
