#ifndef OBJLOADER_H
#define OBJLOADER_H


#include <QVector>
#include <QHash>
#include "ObjData.h"
class ObjLoader{
public:
    bool Load(QString fileName, ObjData &objdata);

private:
    QHash<int, int> polyfacescount;
};

#endif // OBJLOADER_H
