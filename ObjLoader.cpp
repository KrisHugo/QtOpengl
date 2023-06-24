#include "ObjLoader.h"

#include <QDebug>
#include <QFile>
#include <regex>



void ObjLoader::push_back_vertexIndex(ObjData &objData, int currentFacet, QVector<int> vert)
{
    objData.facets[currentFacet].vpointsIndex.push_back(vert[0]);
    objData.facets[currentFacet].tpointsIndex.push_back(vert[1]);
    objData.facets[currentFacet].npointsIndex.push_back(vert[2]);
}

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
    int currentObj = -1, currentFacet = -1;
//    QVector<std::tuple<int,int,int>> facesIndexs;
    //插入初始t, n
    objData.tPoints.push_back(0);
    objData.tPoints.push_back(1);

    objData.nPoints.push_back(0);
    objData.nPoints.push_back(0);
    objData.nPoints.push_back(1);


    while (!objFile.atEnd()) {
        QByteArray lineData = objFile.readLine();
        std::regex pattern("\\r\\n+$");
        int len = std::regex_search(lineData.toStdString(), pattern) ? 2 : 1;
//        qDebug() << QString::number(len) << ":" << lineData.toStdString();
        lineData = lineData.remove(lineData.size() - len, len);
        if(lineData.isEmpty()){
            continue;
        }
        QList<QByteArray> strValues = lineData.split(' ');

        QString dataType = strValues.takeFirst();
        if (dataType == "o"){
            QString objName = strValues.takeFirst();
            objData.objects.push_back(objName);
            currentObj = objData.objects.size() - 1;
        }
        //其实不应该用usemtl而是用g来识别, 不过文件里没有g, 暂时先这样
        else if (dataType == "usemtl"){
            QString matrial = strValues.takeFirst();
            objData.mtls.insert(matrial);//这里实际上应该当读取mtl文件时再记录一个与mtl属性连接的map, 同一个mtl可能被使用多次
            //应该记录每个facet
//            objData.objects[currentObj].groups.push_back(Group(matrial));
//            currentGroup = objData.objects[currentObj].groups.size() - 1;
        }
        else if (dataType == "v"){
            if(strValues.size() == 3){
                std::transform(strValues.begin(), strValues.end(),
                               std::back_inserter(objData.vPoints), [](QByteArray &str) {
                    return str.toFloat();
                });
            }
            else{
                qDebug() << "vPoints Load Error";
            }
        }else if (dataType == "vt"){
//            qDebug() << strValues.size() << ":" << strValues;
//            if(strValues.size() == 2){
//                std::transform(strValues.begin(), strValues.end(),
//                               std::back_inserter(objData.tPoints), [](QByteArray &str) {
//                    return str.toFloat();
//                });
//            }
//            else{
//                qDebug() << "tPoints Load Error";
//            }
            std::transform(strValues.begin(), strValues.begin() + std::min(2, static_cast<int>(strValues.size())),
                           std::back_inserter(objData.tPoints),
                           [](QByteArray &str) {
                return str.toFloat();
            });
        }else if (dataType == "vn"){
            if(strValues.size() == 3){
                std::transform(strValues.begin(), strValues.end(),
                               std::back_inserter(objData.nPoints), [](QByteArray &str) {
                    return str.toFloat();
                });
            }
            else{
                qDebug() << "nPoints Load Error";
            }
        }else if (dataType == "f"){
            objData.facets.push_back(facet());
            currentFacet = objData.facets.size() - 1;
            objData.facetIndexesInObj[objData.objects[currentObj]].push_back(currentFacet);
            objData.facetIndexesInSize[strValues.size()].push_back(currentFacet);
//            polyfacescount[strValues.size()]++;
//            qDebug() << lineData.toStdString();
            QVector<int> firstVex, lastVex;
            for (int i = 0; i < strValues.size(); i++) {
//                qDebug() << "i:" << QString::number(i);
                QByteArray str = strValues[i];
                QList<QByteArray> intStr = str.split('/');
                int v, t, n;
                v = intStr.first().toInt();
                if (intStr.size() == 1){
                    t = 0;
                    n = 0;
                }
                else if(intStr.size() == 2){
                    t = intStr.last().toInt();
                    n = 0;
                }
                else if(intStr.at(1) == ""){
                    t = 0;
                    n = intStr.last().toInt();
                }
                else{
                    t = intStr.at(1).toInt();
                    n = intStr.last().toInt();
                }
                //修正vpoint索引
                v--;
//                qDebug() <<"t:"<< QString::number(t);
//                qDebug() <<"n:"<< QString::number(n);
                if(i == 0) firstVex = {v, t, n};
                //补三角面
                if(i >= 3){
                    // firstVex
                    push_back_vertexIndex(objData, currentFacet, firstVex);
                    // lastVex
                    push_back_vertexIndex(objData, currentFacet, lastVex);
                }
//                qDebug() << "add Facets";
                push_back_vertexIndex(objData, currentFacet, {v, t, n});
                lastVex = {v, t, n};
//                qDebug() << "lastVex:" << lastVex;
            }

        }
    }

    if (objData.vPoints.size() != 0 && objData.tPoints.size() != 0 && objData.nPoints.size() != 0 && objData.objects.size() != 0)
    {
        qDebug()<< objData.vPoints.size() << "," << objData.tPoints.size()  << ","  << objData.nPoints.size() << "," << objData.facets.size() << ","  << objData.objects.size();
    }
    else
    {
        qDebug() << "none vert points";
        return false;
    }

//    //only extract the points which in one of the faces, and throw the others.
//    //but only available for triangle facet
//    for(auto &verFaceInfo : objData.facets){
//        for(int i = 0; i < verFaceInfo.vpointsIndex.size(); i++){
//            unsigned int vIndex = verFaceInfo.vpointsIndex[i];
////            qDebug() << "v" << QString::number(vIndex);
//            unsigned int tIndex = verFaceInfo.tpointsIndex[i];
////            qDebug() << "t" << QString::number(tIndex);
//            unsigned int nIndex = verFaceInfo.npointsIndex[i];
////            qDebug() << "n" << QString::number(nIndex);
////            int vPointSizes = objData.vPoints.size() / 3;
////            int tPointSizes = objData.tPoints.size() / 2;
////            int nPointSizes = objData.nPoints.size() / 3;
//            objData.vertPoints_ << objData.vPoints.at((vIndex) * 3);
//            objData.vertPoints_ << objData.vPoints.at((vIndex) * 3 + 1);
//            objData.vertPoints_ << objData.vPoints.at((vIndex) * 3 + 2);

//            objData.texturePoints_ << objData.tPoints.at((tIndex) * 2);
//            objData.texturePoints_ << objData.tPoints.at((tIndex) * 2 + 1);

//            objData.normalPoints_ << objData.nPoints.at((nIndex) * 3);
//            objData.normalPoints_ << objData.nPoints.at((nIndex) * 3 + 1);
//            objData.normalPoints_ << objData.nPoints.at((nIndex) * 3 + 2);
////            qDebug() << "test";
//        }
//    }
    //debug
//    qDebug() << polyfacescount;
    objFile.close();
    return true;
}



