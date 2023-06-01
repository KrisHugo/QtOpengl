#include "widget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    loadFileFlag = 0;
    this->resize(1080, 900);
    //2.0 MainMenuBar
    mainMenuBar = new QMenuBar();
    mainMenuBar->setMaximumHeight(25);


    //2.0.1 Menus
    mainMenu = new QMenu("菜单");

    mainMenuBar->addMenu(mainMenu);

    //2.0.1 Actions
    newAction = new QAction("新建");		//包括图标路径、长时间停留提示、父指针
    newAction->setShortcut(QKeySequence::New);
    newAction->setStatusTip("新建文件");
    openAction = new QAction("打开");		//包括图标路径、长时间停留提示、父指针
    openAction->setShortcut(QKeySequence::Open);
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
    listComp = new QLabel();
    //2.2.1 文本提示框
    lbl = new QLabel(this);
    lbl->setText(tr("组件说明："));
    //2.2.2 TextEdit
    txtTipComp = new QTextEdit();
    //2.3 OpenGL renderer
    renderName = new QLabel("尚未打开文件");
    renderName->setMaximumHeight(18);
    renderer = new RendererWidget();
    //2.4 布局
    QVBoxLayout *leftLayout = new QVBoxLayout();
    //leftLayout->addWidget(compsBoxToolBar);//把工具栏作为一个widget添加到布局中
    leftLayout->addWidget(listComp);
    leftLayout->addWidget(lbl);
//    leftLayout->addWidget(txtTipComp);
    QVBoxLayout *centerLayout = new QVBoxLayout();
    centerLayout->addWidget(renderName);
//    centerLayout->addWidget(renderer);
    centerLayout->setStretchFactor(renderName, 1);
    centerLayout->setStretchFactor(renderer, 5);
//    renderer->move(250,250);
    renderer->show();
    QVBoxLayout *rightLayout = new QVBoxLayout();
    QHBoxLayout *mainLayout = new QHBoxLayout();
    mainLayout->addLayout(leftLayout);
    mainLayout->addLayout(centerLayout);
    mainLayout->addLayout(rightLayout);
    mainLayout->setStretchFactor(leftLayout, 1);
    mainLayout->setStretchFactor(centerLayout, 3);
    mainLayout->setStretchFactor(rightLayout, 1);


    QVBoxLayout *baseLayout = new QVBoxLayout();
    baseLayout->addWidget(mainMenuBar);
    baseLayout->addLayout(mainLayout);


    //2.5 应用布局
    this->setLayout(baseLayout);

}

Widget::~Widget()
{
}

void Widget::SelectFile(){
    QFileDialog *f = new QFileDialog(this);
    f->setWindowTitle("选择三维文件");
    f->setNameFilter("*.obj");
    f->setViewMode(QFileDialog::Detail);

    QStringList filePaths;
    int loadStats;
    loadStats = f->exec();
    if(loadStats){
        filePaths = f->selectedFiles();
        if(filePaths.size() > 0)
        {
//            txtTipComp->append(QString::number(loadStats));
            renderName->setText("现在已打开：" + filePaths[0]);
            loadFileFlag = 1;
            renderer->loadModel(filePaths[0]);
        }
        else
        {
            renderName->setText("未获取到文件路径");
        }
    }
    else{
        renderName->setText("文件获取失败");
    }
}
