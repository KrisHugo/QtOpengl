#include "widget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QEvent>
#include <QOpenGLDebugLogger>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
{



    loadFileFlag = 0;
    this->resize(1080, 900);

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
    renderer = new RendererWidget();
    renderer->setMinimumWidth(800);
    renderStatus = new QLabel("尚未打开文件");
    renderStatus->setMaximumWidth(200);
    renderDetail = new QLabel();
    renderDetail->setMaximumWidth(200);
    renderDetail->setWordWrap(true);
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
    mainLayout->addWidget(renderer);
    mainLayout->addLayout(rightLayout);
    rightLayout->addWidget(renderStatus);
    rightLayout->addWidget(renderDetail);

//    baseLayout->setAlignment(Qt::AlignLeft | Qt::AlignCenter);
    //2.5 应用布局
    renderer->show();
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
            renderStatus->setText("现在已打开：" + objPaths[0]);
            loadFileFlag = 1;
            renderer->loadModel(objPaths[0]);
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
            renderStatus->setText("未获取到文件路径");
        }
    }
    else{
        renderStatus->setText("文件获取失败");
    }
}
