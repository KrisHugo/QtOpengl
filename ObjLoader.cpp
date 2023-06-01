#include "ObjLoader.h"

#include <QDebug>
#include <QFile>

bool ObjLoader::Load(QString fileName, QVector<float> &vPoints, QVector<float> &tPoints, QVector<float> &nPoints)
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

    QVector<float> vertextPoints, texturePoints, normalPoints;
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
            polyfacescount[strValues.size()]++;
            std::transform(strValues.begin(), strValues.end(), std::back_inserter(facesIndexs), [](QByteArray &str) {
                QList<QByteArray> intStr = str.split('/');
                // f 的存储格式有 v, v/t, v/t/n, v//n
                // 先临时把索引存成-1, 当遇到的时候将之换算成默认的数值可能会比较好
                if(intStr.size() > 3 || intStr.size() <= 0){
                    return std::make_tuple(intStr.first().toInt(), intStr.last().toInt(), -1);
                }else if(intStr.size() == 2){
                    return std::make_tuple(intStr.first().toInt(), intStr.last().toInt(), -1);
                }else if(intStr.size() == 1){
                    return std::make_tuple(intStr.first().toInt(), -1, -1);
                }else{
                    if(intStr.at(1) == "") {
                        return std::make_tuple(intStr.first().toInt(), -1, intStr.last().toInt());
                    }
                }
                return std::make_tuple(intStr.first().toInt(), intStr.at(1).toInt(), intStr.last().toInt());

            });
        }
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

    //only extract the points which  in one of the faces, and throw the others.
    for(auto &verFaceInfo : facesIndexs){
        int vIndex = std::get<0>(verFaceInfo);
        int tIndex = std::get<1>(verFaceInfo);
        int nIndex = std::get<2>(verFaceInfo);
        if(vIndex < 0 || tIndex < 0 || nIndex < 0){
//            qDebug() << "0:" <<  vertextPoints.at((vIndex) * 3) << texturePoints.at((tIndex) * 2) << normalPoints.at((nIndex) * 3);
            continue;
        }
        int vPointSizes = vertextPoints.count() / 3;
        int tPointSizes = texturePoints.count() / 2;
        int nPointSizes = normalPoints.count() / 3;
        if(vIndex * 3 + 2 > vPointSizes || tIndex * 2 + 1 > tPointSizes || nIndex * 3 + 2 > nPointSizes){
//            qDebug() << "outofbounds" << vertextPoints.at((vIndex) * 3) << texturePoints.at((tIndex) * 2) << normalPoints.at((nIndex) * 3);
            continue;
        }
        vPoints << vertextPoints.at((vIndex) * 3);
        vPoints << vertextPoints.at((vIndex) * 3 + 1);
        vPoints << vertextPoints.at((vIndex) * 3 + 2);
        tPoints << texturePoints.at((tIndex) * 2);
        tPoints << texturePoints.at((tIndex) * 2 + 1);
        nPoints << normalPoints.at((nIndex) * 3);
        nPoints << normalPoints.at((nIndex) * 3 + 1);
        nPoints << normalPoints.at((nIndex) * 3 + 2);
    }
    //debug
    qDebug() << polyfacescount;

    vertextPoints.clear();
    texturePoints.clear();
    normalPoints.clear();
    facesIndexs.clear();

    objFile.close();
    return true;
}
