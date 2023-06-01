#ifndef OBJDATA_H
#define OBJDATA_H
#include <QString>
#include <QVector>
class ObjData
{
public:
    ObjData(QString file, QVector<float> &vPoints, QVector<float> &tPoints,QVector<float> &nPoints);
    ~ObjData();
    int uid;
    QString file;
    QVector<float> vPoints, tPoints, nPoints;
};

#endif // OBJDATA_H
