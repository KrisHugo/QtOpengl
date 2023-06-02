#include "objdata.h"

ObjData::ObjData()
{
    vPoints = QVector<QVector3D>();
    nPoints = QVector<QVector3D>();
    tPoints = QVector<QVector2D>();
    objects = QVector<Obj>();
    mtls = QSet<QString>();
    position = QVector3D(0.0, 0.0, 0.0);
}

ObjData::~ObjData(){
}

Group::Group(){
    facets = QVector<QVector<std::tuple<int,int,int>>>();
}
Group::Group(QString _matrial){
    uid = QUuid::createUuid();
    matrial = _matrial;
    facets = QVector<QVector<std::tuple<int,int,int>>>();
}
Group::Group(QUuid _uid, QString _matrial, QVector<QVector<std::tuple<int,int,int>>> &_facets){
    uid = _uid;
    matrial = _matrial;
    facets = _facets;
}

Obj::Obj(){
    uid = QUuid::createUuid();
    groups = QVector<Group>();
}
Obj::Obj(QString _objName){
    uid = QUuid::createUuid();
    objName = _objName;
    groups = QVector<Group>();
}
Obj::Obj(QUuid _uid, QString _objName, QVector<Group> &_groups){
    uid = _uid;
    objName = _objName;
    groups = _groups;
}
