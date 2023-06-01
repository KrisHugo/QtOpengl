#include "objdata.h"

ObjData::ObjData()
{
}



void ObjData::load(QString _file, QVector<float> &_vPoints, QVector<float> &_tPoints,QVector<float> &_nPoints){

    file = _file;
    vPoints = &_vPoints;
    tPoints = &_tPoints;
    nPoints = &_nPoints;
}
