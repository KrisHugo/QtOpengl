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

void ObjData::LoadOnOpenGL(QVector<float> &vertPoints_, QVector<float> &texturePoints_, QVector<float> &normalPoints_){
    vertPoints_.clear();
    texturePoints_.clear();
    normalPoints_.clear();
    //only extract the points which in one of the faces, and throw the others.
    //but only available for triangle facet
    for(auto &verFaceInfo : facets){
        for(int i = 0; i < verFaceInfo.vpointsIndex.size(); i++){
            unsigned int vIndex = verFaceInfo.vpointsIndex[i];
            //            qDebug() << "v" << QString::number(vIndex);
            unsigned int tIndex = verFaceInfo.tpointsIndex[i];
            //            qDebug() << "t" << QString::number(tIndex);
            unsigned int nIndex = verFaceInfo.npointsIndex[i];
            //            qDebug() << "n" << QString::number(nIndex);
            //            int vPointSizes = objData.vPoints.size() / 3;
            //            int tPointSizes = objData.tPoints.size() / 2;
            //            int nPointSizes = objData.nPoints.size() / 3;
            vertPoints_ << vPoints.at((vIndex) * 3);
            vertPoints_ << vPoints.at((vIndex) * 3 + 1);
            vertPoints_ << vPoints.at((vIndex) * 3 + 2);

            texturePoints_ << tPoints.at((tIndex) * 2);
            texturePoints_ << tPoints.at((tIndex) * 2 + 1);

            normalPoints_ << nPoints.at((nIndex) * 3);
            normalPoints_ << nPoints.at((nIndex) * 3 + 1);
            normalPoints_ << nPoints.at((nIndex) * 3 + 2);
            //            qDebug() << "test";
        }
    }
}

void ObjData::Rotate(float angleX_ = 0.0, float angleY_ = 0.0, float angleZ_ = 0.0){
    transform.rotate(angleX_,1,0,0);
    transform.rotate(angleY_,0,1,0);
    transform.rotate(angleZ_,0,0,1);
}


//Group::Group(){
//    facets = QVector<QVector<std::tuple<int,int,int>>>();
//}
//Group::Group(QString _matrial){
//    uid = QUuid::createUuid();
//    matrial = _matrial;
//    facets = QVector<QVector<std::tuple<int,int,int>>>();
//}
//Group::Group(QUuid _uid, QString _matrial, QVector<QVector<std::tuple<int,int,int>>> &_facets){
//    uid = _uid;
//    matrial = _matrial;
//    facets = _facets;
//}

//Obj::Obj(){
//    uid = QUuid::createUuid();
//    groups = QVector<Group>();
//}
//Obj::Obj(QString _objName){
//    uid = QUuid::createUuid();
//    objName = _objName;
//    groups = QVector<Group>();
//}
//Obj::Obj(QUuid _uid, QString _objName, QVector<Group> &_groups){
//    uid = _uid;
//    objName = _objName;
//    groups = _groups;
//}
