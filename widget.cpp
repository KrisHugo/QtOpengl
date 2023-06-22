#include "widget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QEvent>
#include <QOpenGLDebugLogger>
#include <QDebug>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    loadFileFlag = 0;
    this->resize(1200, 900);

    //2.0 MainMenuBar
    mainMenuBar = new QMenuBar();
    mainMenuBar->setMaximumHeight(20);

    //2.0.1 Menus
    mainMenu = new QMenu("菜单");
    mainMenuBar->addMenu(mainMenu);

    //2.0.1 Actions
    newAction = new QAction("新建");		//包括图标路径、长时间停留提示、父指针
//    newAction->setShortcut(QKeySequence::New);
    newAction->setStatusTip("新建文件");
    openAction = new QAction("打开");		//包括图标路径、长时间停留提示、父指针
//    openAction->setShortcut(QKeySequence::Open);
    openAction->setStatusTip("打开文件");
    connect(openAction, SIGNAL(triggered(bool)), this, SLOT(SelectFile()));

    mainMenu->addAction(newAction);
    mainMenu->addAction(openAction);


    //2.1 工具栏
    /*
    compsBoxToolBar = new QToolBar(tr("组件"), this);
    compsBoxToolBar->setIconSize(QSize(16,16));
    compsBoxToolBar->addAction(act);
    compsBoxToolBar->addAction(act1);
    */
    //2.2 主体
//    listComp = new QLabel();
    //2.2.1 文本提示框
    //2.3 OpenGL renderer
    openGL = new OpenGLWidget();
    openGL->setMinimumWidth(900);
    fileStatus = new QLabel("尚未打开文件");
    fileStatus->setMinimumWidth(350);
    fileStatus->setMaximumWidth(500);
    //下拉多级列表QTreeWidgetItem
    dataView = new TreeView();
    dataView->setMinimumWidth(350);
    dataView->setMaximumWidth(500);
    dataView->setWordWrap(true);
    dataView->setHeaderHidden(true);
    //2.4 布局
//    QVBoxLayout *leftLayout = new QVBoxLayout();
    //leftLayout->addWidget(compsBoxToolBar);//把工具栏作为一个widget添加到布局中
//    leftLayout->addWidget(listComp);
//    leftLayout->addWidget(lbl);
//    leftLayout->addWidget(txtTipComp);
//    QVBoxLayout *centerLayout = new QVBoxLayout();
//    centerLayout->addWidget(renderName);
//    centerLayout->addWidget(renderer);

//    centerLayout->setStretchFactor(renderName, 1);
//    centerLayout->setStretchFactor(renderer, 5);
//    renderer->move(250,250);
    QVBoxLayout *rightLayout = new QVBoxLayout();
    QHBoxLayout *mainLayout = new QHBoxLayout();
//    mainLayout->addLayout(leftLayout);
//    mainLayout->addLayout(centerLayout);
//    mainLayout->setStretchFactor(leftLayout, 1);
//    mainLayout->setStretchFactor(centerLayout, 5);
//    mainLayout->setStretchFactor(rightLayout, 1);


    QVBoxLayout *baseLayout = new QVBoxLayout(this);
    baseLayout->addWidget(mainMenuBar);
    baseLayout->addLayout(mainLayout);
    mainLayout->addWidget(openGL);
    mainLayout->addLayout(rightLayout);
    rightLayout->addWidget(fileStatus);
    rightLayout->addWidget(dataView, 10000);

//    baseLayout->setAlignment(Qt::AlignLeft | Qt::AlignCenter);
    //2.5 应用布局
    openGL->show();
//    renderer->installEventFilter(renderer);


    QOpenGLDebugLogger logger;
    logger.initialize(); // 初始化日志记录器
    logger.startLogging(); // 开始记录日志
}



Widget::~Widget()
{
}

//bool Widget::eventFilter(QObject *obj, QEvent *event)
//{
//    qDebug() << event->type();
//    return QWidget::eventFilter(obj, event);

//}


void Widget::SelectFile(){
    QFileDialog *objFile = new QFileDialog(this);
    objFile->setWindowTitle("选择三维文件");
    objFile->setNameFilter("*.obj");
    objFile->setViewMode(QFileDialog::Detail);

    QStringList objPaths;
    int objLoadStats;
    objLoadStats = objFile->exec();
    if(objLoadStats){
        objPaths = objFile->selectedFiles();
        if(objPaths.size() > 0)
        {
//            txtTipComp->append(QString::number(loadStats));
            fileStatus->setText("现在已打开: " + objPaths[0]);
            loadFileFlag = 1;
            ObjLoader objLoader;
            if(openGL->IsLoad()){
                openGL->unLoadModel();
            }
            loadedObj = ObjData();
            if(objLoader.Load(objPaths[0], loadedObj))
            {
                //load on detail
                loadOnDetail(loadedObj);
                openGL->loadModel(loadedObj);
            }
            else{
                qCritical("Load Error");
                close();
            }
//            QFileDialog *imgfile = new QFileDialog(this);
//            imgfile->setWindowTitle("选择贴图文件");
//            imgfile->setNameFilter("*.png");
//            imgfile->setViewMode(QFileDialog::Detail);
//            QStringList imgPaths;
//            int imgLoadStats;
//            imgLoadStats = imgfile->exec();
//            if(imgLoadStats){
//                imgPaths = imgfile->selectedFiles();
//                if(imgPaths.size() > 0)
//                {
//                    renderer->loadModel(objPaths[0], "...");
//                }
//                else
//                {
//                    renderStatus->setText("未获取到文件路径");
//                }
//            }
//            else{
//                renderStatus->setText("文件获取失败");
//            }

        }
        else
        {
            fileStatus->setText("未获取到文件路径");
        }
    }
    else{
        fileStatus->setText("文件获取失败");
    }
}

bool Widget::loadOnDetail(ObjData &objData){
    //Clear All
    dataView->clear();
    //Top
    QTreeWidgetItem *rootItem = new QTreeWidgetItem();
    rootItem->setText(0, "file:" + objData.file);
    rootItem->setCheckState(0, Qt::Unchecked);
    rootItem->setData(0, m_STypeRole, NT_RootObj);
    dataView->addTopLevelItem(rootItem);
    //Obj
//    qDebug() << "OBJ:" << objData.objects.size();
    foreach (QString obj, objData.objects)
    {
        QTreeWidgetItem *objItem = new QTreeWidgetItem();
        QVector<int> facetIndexes = objData.facetIndexesInObj[obj];
        objItem->setText(0, "o:" + obj + "(" + QString::number(facetIndexes.size()) + ")");
        objItem->setFlags(objItem->flags() | Qt::ItemIsUserCheckable);
        objItem->setCheckState(0, Qt::Unchecked);
        objItem->setData(0, m_STypeRole, NT_Obj);
        rootItem->addChild(objItem);
        for (int i = 0; i < facetIndexes.size(); i++){
            facet facet = objData.facets[facetIndexes[i]];
            QTreeWidgetItem *facetItem = new QTreeWidgetItem();
            facetItem->setText(0, "f:" + QString::number(i));
            facetItem->setFlags(objItem->flags() | Qt::ItemIsUserCheckable);
            facetItem->setCheckState(0, Qt::Unchecked);
            facetItem->setData(0, m_STypeRole, NT_Facets);
            objItem->addChild(facetItem);
            //Node: vertex, normal vertex, texture vectex
            //                qDebug() << i << ":" << facet.size();
            for(int j = 0; j < facet.vpointsIndex.size(); j++){
                unsigned int vindex = facet.vpointsIndex[j];
                unsigned int tindex = facet.tpointsIndex[j];
                unsigned int nindex = facet.npointsIndex[j];
                QTreeWidgetItem *nodeItem = new QTreeWidgetItem();
                nodeItem->setText(0, "Node:" + QString::number(vindex));
                nodeItem->setFlags(facetItem->flags() | Qt::ItemIsUserCheckable);
                nodeItem->setCheckState(0, Qt::Unchecked);
                nodeItem->setData(0, m_STypeRole, NT_Node);
                facetItem->addChild(nodeItem);

                //                    qDebug() << vindex << "," << nindex << "," << tindex;
                //                    qDebug() << "test1:" << objData.vPoints.size() << ":" << vindex;
                QTreeWidgetItem *vItem = new QTreeWidgetItem();
                vItem->setText(0,  "v:" + Vector3D2String(objData.vPoints, 3 * (vindex)));
                vItem->setFlags(nodeItem->flags());
                vItem->setData(0, m_STypeRole, NT_ENode);
                nodeItem->addChild(vItem);
                //                    qDebug() << "test2:" << objData.tPoints.size() << ":" << tindex;
                QTreeWidgetItem *tItem = new QTreeWidgetItem();
                tItem->setText(0, "t:" + Vector2D2String(objData.tPoints, 2 * (tindex)));
                tItem->setFlags(nodeItem->flags());
                tItem->setData(0, m_STypeRole, NT_ENode);
                nodeItem->addChild(tItem);
                //                    qDebug() << "test3:" << objData.nPoints.size() << ":" << nindex;
                QTreeWidgetItem *nItem = new QTreeWidgetItem();
                nItem->setText(0, "n:" + Vector3D2String(objData.nPoints, 3 * (nindex)));
                nItem->setFlags(nodeItem->flags());
                nItem->setData(0, m_STypeRole, NT_ENode);
                nodeItem->addChild(nItem);
            }
        }

        //Group
//        qDebug() << obj.objName << ":" << obj.groups.size();
//        foreach (Group group, obj.groups)
//        {
//            QTreeWidgetItem *groupItem = new QTreeWidgetItem();
//            groupItem->setText(0, "g:" + group.uid.toString() + "[" + group.matrial + "](" + QString::number(group.facets.size()) + ")");
//            groupItem->setFlags(groupItem->flags() | Qt::ItemIsUserCheckable);
//            groupItem->setCheckState(0, Qt::Unchecked);
//            groupItem->setData(0, m_STypeRole, NT_Group);
//            objItem->addChild(groupItem);
//            //Facets
////            qDebug() << group.matrial << ":" << group.facets.size();
//            for (int i = 0; i < group.facets.size(); i++){
//                QVector<std::tuple<int, int, int>> facet = group.facets[i];
//                QTreeWidgetItem *facetItem = new QTreeWidgetItem();
//                facetItem->setText(0, "f:" + QString::number(i));
//                facetItem->setFlags(groupItem->flags() | Qt::ItemIsUserCheckable);
//                facetItem->setCheckState(0, Qt::Unchecked);
//                facetItem->setData(0, m_STypeRole, NT_Facets);
//                groupItem->addChild(facetItem);
//                //Node: vertex, normal vertex, texture vectex
////                qDebug() << i << ":" << facet.size();
//                for(int j = 0; j < facet.size(); j++){
//                    std::tuple<int, int, int> node = facet[j];
//                    QTreeWidgetItem *nodeItem = new QTreeWidgetItem();
//                    nodeItem->setText(0, "N:" + QString::number(j));
//                    nodeItem->setFlags(groupItem->flags() | Qt::ItemIsUserCheckable);
//                    nodeItem->setCheckState(0, Qt::Unchecked);
//                    nodeItem->setData(0, m_STypeRole, NT_Node);
//                    facetItem->addChild(nodeItem);

//                    int vindex = std::get<0>(node);
//                    int tindex = std::get<1>(node);
//                    int nindex = std::get<2>(node);
////                    qDebug() << vindex << "," << nindex << "," << tindex;
////                    qDebug() << "test1:" << objData.vPoints.size() << ":" << vindex;
//                    QTreeWidgetItem *vItem = new QTreeWidgetItem();
//                    vItem->setText(0,  "v:" + Vector3D2String(objData.vPoints[vindex - 1]));
//                    vItem->setFlags(groupItem->flags());
//                    vItem->setData(0, m_STypeRole, NT_ENode);
//                    nodeItem->addChild(vItem);
////                    qDebug() << "test2:" << objData.tPoints.size() << ":" << tindex;
//                    QTreeWidgetItem *tItem = new QTreeWidgetItem();
//                    tItem->setText(0, "t:" + Vector2D2String(objData.tPoints[tindex - 1]));
//                    tItem->setFlags(groupItem->flags());
//                    tItem->setData(0, m_STypeRole, NT_ENode);
//                    nodeItem->addChild(tItem);
////                    qDebug() << "test3:" << objData.nPoints.size() << ":" << nindex;
//                    QTreeWidgetItem *nItem = new QTreeWidgetItem();
//                    nItem->setText(0, "n:" + Vector3D2String(objData.nPoints[nindex - 1]));
//                    nItem->setFlags(groupItem->flags());
//                    nItem->setData(0, m_STypeRole, NT_ENode);
//                    nodeItem->addChild(nItem);
//                }
//            }
//        }
    }
//    dataView->expandAll();  //展开所有节点

//    dataDetail->clear();
//    QVector<QTreeWidgetItem> structures();
//    QTreeWidgetItem *item = new QTreeWidgetItem(dataDetail);
//    item->setText(0, tr("Cities"));
//    structures->append(&item);
//    renderDetail->update();
    return true;
}

QString Widget::Vector3D2String(QVector<float> &points, int index){
    return QString("(%1,%2,%3)").arg(QString::number(points[index]), QString::number(points[index + 1]), QString::number(points[index + 2]));
}
QString Widget::Vector2D2String(QVector<float> &points, int index){
    return QString("(%1,%2)").arg(QString::number(points[index]), QString::number(points[index + 1]));
}













