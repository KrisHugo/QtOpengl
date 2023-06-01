#ifndef OBJLOADER_H
#define OBJLOADER_H


#include <QVector>


class ObjLoader{
public:
    bool Load(QString fileName, QVector<float> &vPoints,QVector<float> &tPoints,QVector<float> &nPoints);
};

#endif // OBJLOADER_H
