#include "objdata.h"

ObjData::ObjData()
{
    uid = QUuid();
    vPoints = QVector<float>();
    nPoints = QVector<float>();
    tPoints = QVector<float>();
    objects = QVector<QString>();
    facetIndexesInObj = QHash<QString, QVector<int>>();
    facetIndexesInSize = QHash<int, QVector<int>>();
    mtls = QSet<QString>();
    position = QVector3D(0.0, 0.0, 0.0);
}

ObjData::~ObjData(){
}

facets::facets()
{
    uid = QUuid();
    vexIndex = QVector<std::tuple<int,int,int>>();
}

facets::facets(QUuid _uid, QVector<std::tuple<int,int,int>> &_vexIndex)
{
    uid = _uid;
    vexIndex = _vexIndex;
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
