#ifndef OBJLOADER_H
#define OBJLOADER_H


#include <QVector>
#include <QHash>

class ObjLoader{
public:
    bool Load(QString fileName, QVector<float> &vPoints,QVector<float> &tPoints,QVector<float> &nPoints);

private:
    QHash<int, int> polyfacescount;
};

#endif // OBJLOADER_H
