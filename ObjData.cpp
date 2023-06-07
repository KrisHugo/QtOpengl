#include "objdata.h"

ObjData::ObjData():uid(), vPoints(), tPoints(), nPoints(),
    objects(), facets(), facetIndexesInObj(), facetIndexesInSize(),
    mtls(), vertPoints_(), texturePoints_(), normalPoints_()
{
    position = QVector3D(0.0, 0.0, 0.0);
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
