#include "widget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QEvent>
#include <QOpenGLDebugLogger>
#include <QMessageBox>
#include <QDebug>
#include "ObjLoader.h"

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

    //2.0.2 Actions
    newAction = new QAction("新建");		//包括图标路径、长时间停留提示、父指针
//    newAction->setShortcut(QKeySequence::New);
    newAction->setStatusTip("新建文件");
    openAction = new QAction("打开");		//包括图标路径、长时间停留提示、父指针
//    openAction->setShortcut(QKeySequence::Open);
    openAction->setStatusTip("打开文件");
    connect(openAction, SIGNAL(triggered(bool)), this, SLOT(SelectFile()));

    mainMenu->addAction(newAction);
    mainMenu->addAction(openAction);


    //2.1.1 Function Menus
    editMenu = new QMenu("编辑");

    chAction = new QAction("创建凸面");
    connect(chAction, SIGNAL(triggered(bool)), this, SLOT(loadModelCH()));

    editMenu->addAction(chAction);

    mainMenuBar->addMenu(mainMenu);
    mainMenuBar->addMenu(editMenu);

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
            fileStatus->setText("现在已打开: " + objPaths[0]);
            loadFileFlag = 1;
            ObjLoader objLoader;
            if(openGL->IsLoad()){
                openGL->unLoadModel();
            }
            loadedObj = new ObjData();
            if(objLoader.Load(objPaths[0], *loadedObj))
            {
                //load on detail
                loadOnDetail(*loadedObj);
                openGL->loadModel(*loadedObj, false);
            }
            else{
                qCritical("Load Error");
                close();
            }

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


void Widget::loadModelCH(){
    if(loadedObj == nullptr) {
        QMessageBox::critical(this,
                              tr("警告"),
                              tr("还未加载模型文件"),
                              QMessageBox::Apply);
        return;
    }
    if(openGL->IsLoad()){
        openGL->unLoadModel();
    }
    openGL->loadModel(*loadedObj, true);
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
                QTreeWidgetItem *vItem = new QTreeWidgetItem();
                vItem->setText(0,  "v:" + Vector3D2String(objData.vPoints, 3 * (vindex)));
                vItem->setFlags(nodeItem->flags());
                vItem->setData(0, m_STypeRole, NT_ENode);
                nodeItem->addChild(vItem);
                QTreeWidgetItem *tItem = new QTreeWidgetItem();
                tItem->setText(0, "t:" + Vector2D2String(objData.tPoints, 2 * (tindex)));
                tItem->setFlags(nodeItem->flags());
                tItem->setData(0, m_STypeRole, NT_ENode);
                nodeItem->addChild(tItem);
                QTreeWidgetItem *nItem = new QTreeWidgetItem();
                nItem->setText(0, "n:" + Vector3D2String(objData.nPoints, 3 * (nindex)));
                nItem->setFlags(nodeItem->flags());
                nItem->setData(0, m_STypeRole, NT_ENode);
                nodeItem->addChild(nItem);
            }
        }
    }
    return true;
}

QString Widget::Vector3D2String(QVector<float> &points, int index){
    return QString("(%1,%2,%3)").arg(QString::number(points[index]), QString::number(points[index + 1]), QString::number(points[index + 2]));
}
QString Widget::Vector2D2String(QVector<float> &points, int index){
    return QString("(%1,%2)").arg(QString::number(points[index]), QString::number(points[index + 1]));
}













