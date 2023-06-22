#ifndef OBJLOADER_H
#define OBJLOADER_H


#include <QVector>
#include <QHash>
#include "ObjData.h"
class ObjLoader{
public:
    bool Load(QString fileName, ObjData &objdata);
private:
    void push_back_vertexIndex(ObjData &objData, int currentFacet, QVector<int> vert);

};

#endif // OBJLOADER_H
