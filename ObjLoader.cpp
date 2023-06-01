#include "ObjLoader.h"

#include <QDebug>
#include <QFile>

bool ObjLoader::Load(QString fileName, QVector<float> &vPoints,QVector<float> &tPoints, QVector<float> &nPoints)
{
    if (fileName.mid(fileName.lastIndexOf('.')) != ".obj"&&fileName.mid(fileName.lastIndexOf('.')) != ".OBJ")
    {
        qDebug() << "file is not a obj file!";
        return false;
    }
    else
    {
        qDebug() << "file loading";
    }

    QFile objFile(fileName);
    if(!objFile.open(QIODevice::ReadOnly)){
        qDebug() << "open" << fileName << "failed";
        return false;
    }

    QVector<float> vertextPoints,texturePoints,normalPoints;
    QVector<std::tuple<int,int,int>> facesIndexs;
    while (!objFile.atEnd()) {
        QByteArray lineData = objFile.readLine();
        lineData = lineData.remove(lineData.size() - 2, 2);//\r\n
        if(lineData.isEmpty()){
            continue;
        }

        QList<QByteArray> strValues = lineData.split(' ');
        QString dataType = strValues.takeFirst();
        if (dataType == "v"){
            std::transform(strValues.begin(),strValues.end(),std::back_inserter(vertextPoints),[](QByteArray &str){
                return str.toFloat();
            });
        }else if (dataType == "vt"){
            std::transform(strValues.begin(),strValues.end(),std::back_inserter(texturePoints),[](QByteArray &str){
                return str.toFloat();
            });
        }else if (dataType == "vn"){
            std::transform(strValues.begin(),strValues.end(),std::back_inserter(normalPoints),[](QByteArray &str){
                return str.toFloat();
            });
        }else if (dataType == "f"){
            std::transform(strValues.begin(), strValues.end(), std::back_inserter(facesIndexs), [](QByteArray &str) {
                QList<QByteArray> intStr = str.split('/');
                if(intStr.size() == 2){
                    return std::make_tuple(intStr.first().toInt(), intStr.last().toInt(), -1);
                }
                else if(intStr.size() == 1){
                    return std::make_tuple(intStr.first().toInt(), -1, -1);
                }
                else if(intStr.size() == 0){
                    return std::make_tuple(-1, -1, -1);
                }
                return std::make_tuple(intStr.first().toInt(), intStr.at(1).toInt(), intStr.last().toInt());

            });
        }
//        qDebug()<< vertextPoints.size() << "," << texturePoints.size() << ","  << normalPoints.size() << ","  << facesIndexs.size();
    }

    if (vertextPoints.size() != 0 && texturePoints.size() != 0 && normalPoints.size() != 0 && facesIndexs.size() != 0)
    {
        qDebug()<< vertextPoints.size() << "," << texturePoints.size() << ","  << normalPoints.size() << ","  << facesIndexs.size();
    }
    else
    {
        qDebug() << "none vert points";
        return false;
    }


    for(auto &verFaceInfo : facesIndexs){
        int vIndex = std::get<0>(verFaceInfo);
        int tIndex = std::get<1>(verFaceInfo);
        int nIndex = std::get<2>(verFaceInfo);
        if(vIndex < 0 || tIndex < 0 || nIndex < 0){
            continue;
        }
//        qDebug() << "test";
        int vPointSizes = vertextPoints.count() / 3;
        int tPointSizes = texturePoints.count() / 3;
        int nPointSizes = normalPoints.count() / 3;
        if(vIndex * 3 + 2 < vPointSizes){
            vPoints << vertextPoints.at((vIndex) * 3);
            vPoints << vertextPoints.at((vIndex) * 3 + 1);
            vPoints << vertextPoints.at((vIndex) * 3 + 2);
        }
        if(tIndex * 3 + 2 < tPointSizes){
            tPoints << texturePoints.at((tIndex) * 3);
            tPoints << texturePoints.at((tIndex) * 3 + 1);
            tPoints << texturePoints.at((tIndex) * 3 + 2);
        }
        if(nIndex * 3 + 2 < nPointSizes){
            nPoints << normalPoints.at((nIndex) * 3);
            nPoints << normalPoints.at((nIndex) * 3 + 1);
            nPoints << normalPoints.at((nIndex) * 3 + 2);
        }
    }
    vertextPoints.clear();
    texturePoints.clear();
    normalPoints.clear();
    facesIndexs.clear();

    objFile.close();
    return true;
}
