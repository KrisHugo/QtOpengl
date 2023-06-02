#include "ObjLoader.h"

#include <QDebug>
#include <QFile>

bool ObjLoader::Load(QString fileName, ObjData &objData)
{
    if (fileName.mid(fileName.lastIndexOf('.')) != ".obj" && fileName.mid(fileName.lastIndexOf('.')) != ".OBJ")
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
    objData.file = fileName;

//    QVector<float> vertextPoints, texturePoints, normalPoints;
    int currentObj = -1, currentGroup = -1, currentFacet = -1;
//    QVector<std::tuple<int,int,int>> facesIndexs;
    while (!objFile.atEnd()) {
        QByteArray lineData = objFile.readLine();
        lineData = lineData.remove(lineData.size() - 1, 1);
        if(lineData.isEmpty()){
            continue;
        }
        QList<QByteArray> strValues = lineData.split(' ');

        QString dataType = strValues.takeFirst();

//        qDebug()<< dataType;

        if (dataType == "o"){
            QString objName = strValues.takeFirst();
            objData.objects.push_back(Obj(objName));
            currentObj = objData.objects.size() - 1;
        }
        //其实不应该用usemtl而是用g来识别, 不过文件里没有g, 暂时先这样
        else if (dataType == "usemtl"){
            QString matrial = strValues.takeFirst();
            objData.mtls.insert(matrial);//这里实际上应该当读取mtl文件时再记录一个与mtl属性连接的map
            objData.objects[currentObj].groups.push_back(Group(matrial));
            currentGroup = objData.objects[currentObj].groups.size() - 1;
        }
        else if (dataType == "v"){
            if(strValues.size() == 3){
                objData.vPoints.push_back(QVector3D(strValues[0].toFloat(), strValues[1].toFloat(), strValues[2].toFloat()));
            }
//            std::transform(strValues.begin(),strValues.end(),std::back_inserter(objData.vPoints),[](QByteArray &str){
//                return str.toFloat();
//            });
        }else if (dataType == "vt"){
            if(strValues.size() == 2){
                objData.tPoints.push_back(QVector2D(strValues[0].toFloat(), strValues[1].toFloat()));
            }
//            std::transform(strValues.begin(),strValues.end(),std::back_inserter(objData.tPoints),[](QByteArray &str){
//                return str.toFloat();
//            });
        }else if (dataType == "vn"){
            if(strValues.size() == 3){
                objData.nPoints.push_back(QVector3D(strValues[0].toFloat(), strValues[1].toFloat(), strValues[2].toFloat()));
            }
//            std::transform(strValues.begin(),strValues.end(),std::back_inserter(objData.nPoints),[](QByteArray &str){
//                return str.toFloat();
//            });
        }else if (dataType == "f"){
//            qDebug() << lineData;
            objData.objects[currentObj].groups[currentGroup].facets.push_back(QVector<std::tuple<int,int,int>>());
            currentFacet = objData.objects[currentObj].groups[currentGroup].facets.size() - 1;
            polyfacescount[strValues.size()]++;
//            qDebug() << currentObj << "," << currentGroup << "," << currentFacet;
            std::transform(strValues.begin(), strValues.end(),
                           std::back_inserter(objData.objects[currentObj].groups[currentGroup].facets[currentFacet]), [](QByteArray &str) {

                QList<QByteArray> intStr = str.split('/');
                // f 的存储格式有 v, v/t, v/t/n, v//n
                if(intStr.at(1) == "") {
                    return std::make_tuple(intStr.first().toInt(), 1, intStr.last().toInt());
                }
                return std::make_tuple(intStr.first().toInt(), intStr.at(1).toInt(), intStr.last().toInt());

            });

        }
    }

    if (objData.vPoints.size() != 0 && objData.tPoints.size() != 0 && objData.nPoints.size() != 0 && objData.objects.size() != 0)
    {
        qDebug()<< objData.vPoints.size() << "," << objData.tPoints.size()  << ","  << objData.nPoints.size() << ","  << objData.objects.size();
    }
    else
    {
        qDebug() << "none vert points";
        return false;
    }

    //only extract the points which in one of the faces, and throw the others.
    //but only available for triangle facet
//    for(auto &verFaceInfo : facesIndexs){
//        int vIndex = std::get<0>(verFaceInfo);
//        int tIndex = std::get<1>(verFaceInfo);
//        int nIndex = std::get<2>(verFaceInfo);
//        if(vIndex < 0 || tIndex < 0 || nIndex < 0){
//            continue;
//        }
//        int vPointSizes = vertextPoints.count() / 3;
//        int tPointSizes = texturePoints.count() / 2;
//        int nPointSizes = normalPoints.count() / 3;
//        if(vIndex * 3 + 2 > vPointSizes || tIndex * 2 + 1 > tPointSizes || nIndex * 3 + 2 > nPointSizes){
////            qDebug() << "outofbounds" << vertextPoints.at((vIndex) * 3) << texturePoints.at((tIndex) * 2) << normalPoints.at((nIndex) * 3);
//            continue;
//        }
//        objData.vPoints << vertextPoints.at((vIndex) * 3);
//        objData.vPoints << vertextPoints.at((vIndex) * 3 + 1);
//        objData.vPoints << vertextPoints.at((vIndex) * 3 + 2);
//        objData.tPoints << texturePoints.at((tIndex) * 2);
//        objData.tPoints << texturePoints.at((tIndex) * 2 + 1);
//        objData.nPoints << normalPoints.at((nIndex) * 3);
//        objData.nPoints << normalPoints.at((nIndex) * 3 + 1);
//        objData.nPoints << normalPoints.at((nIndex) * 3 + 2);
//    }
    //debug
    qDebug() << polyfacescount;

//    vertextPoints.clear();
//    texturePoints.clear();
//    normalPoints.clear();
//    facesIndexs.clear();

    objFile.close();
    return true;
}
