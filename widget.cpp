#include "widget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QEvent>
#include <QDebug>
#include <QCheckBox>
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
    connect(openAction, SIGNAL(triggered(bool)), this, SLOT(openFileBrowser()));

    mainMenu->addAction(newAction);
    mainMenu->addAction(openAction);


    //2.1.1 Function Menus
    editMenu = new QMenu("编辑");

    chAction = new QAction("创建凸面");
    connect(chAction, SIGNAL(triggered(bool)), this, SLOT(loadModelCH()));
    editMenu->addAction(chAction);

    hashMenu = new QMenu("哈希操作");
    shaAction = new QAction("SHA256");
    connect(shaAction, SIGNAL(triggered(bool)), this, SLOT(openSHADialog()));
    hashMenu->addAction(shaAction);

    extractMenu = new QMenu("哈希提取操作");
    extractHashAction = new QAction("SHA256");
    connect(extractHashAction, SIGNAL(triggered(bool)), this, SLOT(openExtractSHADialog()));
    extractMenu->addAction(extractHashAction);

    mainMenuBar->addMenu(mainMenu);
    mainMenuBar->addMenu(editMenu);
    mainMenuBar->addMenu(hashMenu);
    mainMenuBar->addMenu(extractMenu);
    //2.1 工具栏

    //2.2 主体
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

    //2.3.2 控制面板
    QCheckBox *modeCheck = new QCheckBox("线框模式");
    connect(modeCheck, SIGNAL(stateChanged(int)), openGL, SLOT(SwitchMode(int)));

    //2.4 布局
    QVBoxLayout *rightLayout = new QVBoxLayout();
    QHBoxLayout *mainLayout = new QHBoxLayout();
    QVBoxLayout *baseLayout = new QVBoxLayout(this);
    baseLayout->addWidget(mainMenuBar);
    baseLayout->addLayout(mainLayout);
    mainLayout->addWidget(openGL);
    mainLayout->addLayout(rightLayout);
    rightLayout->addWidget(fileStatus);
    rightLayout->addWidget(modeCheck);
    rightLayout->addWidget(dataView, 10000);
    //2.5 应用布局
    openGL->show();



    //报错处理
//    connect(fileHashGenerator, SIGNAL(errorOccurred(QString&)),
//                     this, SLOT(showError(QString&)));

}



Widget::~Widget()
{
}


void Widget::openFileBrowser(){
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

void Widget::openSHADialog()
{
    if(loadedObj == nullptr) {
        QMessageBox::critical(this,
                              tr("警告"),
                              tr("还未加载模型文件"),
                              QMessageBox::Apply);
        return;
    }
    QString objStr = loadedObj->file;
    hashDialog = new HashDialog(this, objStr, HashType::SHA256);
    connect(hashDialog, SIGNAL(hashStringGenerated(QString&)),
            this, SLOT(saveHashString(QString&)));
    connect(hashDialog, SIGNAL(HashStringInsert()), this, SLOT(LoadWaterMarkedModel()));
    hashDialog->show();
    qDebug("生成Hash窗口");
}

void Widget::openExtractSHADialog(){
    if(loadedObj == nullptr) {
        QMessageBox::critical(this,
                              tr("警告"),
                              tr("还未加载模型文件"),
                              QMessageBox::Apply);
        return;
    }
    QString objStr = loadedObj->file;
    extractHashDialog = new ExtractHashDialog(this, HashType::SHA256, loadedObj);
//    connect(hashDialog, SIGNAL(hashStringGenerated(QString&)),
//            this, SLOT(saveHashString(QString&)));
    connect(extractHashDialog, SIGNAL(HashStringExtract()), this, SLOT(ExtractWaterMark()));
    extractHashDialog->show();
    qDebug("生成提取Hash窗口");
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
    //loadedObj->generateCHData;
    loadedObj->InitializeCHData();
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

void Widget::LoadWaterMarkedModel(){
    if(loadedObj == nullptr) {
        QMessageBox::critical(this,
                              tr("警告"),
                              tr("还未加载模型文件"),
                              QMessageBox::Apply);
        return;
    }
    if(loadedObj->chData.vertices == nullptr) {
        QMessageBox::critical(this,
                              tr("警告"),
                              tr("还未生成简化模型"),
                              QMessageBox::Apply);
        return;
    }
    std::string watermark = loadedObj->fileHash.toStdString();
    loadedObj->InsertWatermarks(loadedObj->chData.vertices, loadedObj->chData.nVertices, watermark);
    openGL->loadModel(*loadedObj, true);
}

void Widget::ExtractWaterMark(){
    if(loadedObj == nullptr) {
        QMessageBox::critical(this,
                              tr("警告"),
                              tr("还未加载模型文件"),
                              QMessageBox::Apply);
        return;
    }
    if(loadedObj->chData.vertices == nullptr) {
        QMessageBox::critical(this,
                              tr("警告"),
                              tr("还未生成简化模型"),
                              QMessageBox::Apply);
        return;
    }
    std::string watermark;
    loadedObj->ExtractWatermark(loadedObj->chData.vertices, loadedObj->chData.nVertices, watermark);

}

//for display
QString Widget::Vector3D2String(QVector<float> &points, int index){
    return QString("(%1,%2,%3)").arg(QString::number(points[index]), QString::number(points[index + 1]), QString::number(points[index + 2]));
}
QString Widget::Vector2D2String(QVector<float> &points, int index){
    return QString("(%1,%2)").arg(QString::number(points[index]), QString::number(points[index + 1]));
}





