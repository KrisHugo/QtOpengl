#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QToolBar>
#include <QLabel>
#include <QTextEdit>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QFileDialog>
#include <QMessageBox>
#include "OpenGLWidgets.h"
#include "ObjData.h"
#include "filehashgenerator.h"
#include "hashWidget.h"

class TreeView :public QTreeWidget
{
public:
    TreeView(QWidget *pParent = nullptr) :QTreeWidget(pParent) {}
    ~TreeView() {};
};

class Widget : public QWidget
{
    Q_OBJECT

//public slots:

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

protected:
    FileHashGenerator *fileHashGenerator;
    bool loadOnDetail(ObjData &objData);
    QString Vector3D2String(QVector<float> &points, int index);
    QString Vector2D2String(QVector<float> &points, int index);
protected slots:
    void openFileBrowser();
    void loadModelCH();
    void openSHADialog();
    void LoadWaterMarkedModel();
    void showError(QString& errorMessage)
    {
        QMessageBox::critical(this, "Error", errorMessage);
    }
    void saveHashString(QString& hashString)
    {
        loadedObj->fileHash = hashString;
    }
private:
    const static int m_STypeRole = Qt::UserRole;

    int loadFileFlag;
    QMenuBar *mainMenuBar;
    QMenu *mainMenu;
    QMenu *editMenu;
    QMenu *hashMenu;

    QAction *openAction;
    QAction *newAction;

    QAction *chAction;

    QAction *shaAction;

    HashDialog *hashDialog;

    QLabel *listComp;
    QLabel *lbl;
    QLabel *fileStatus;
    TreeView *dataView;
    OpenGLWidget *openGL;
    ObjData *loadedObj = nullptr;

    //节点类型
    enum NodeType
    {
        NT_RootObj = 0,
        NT_Obj,
        NT_Group,
        NT_Facets,
        NT_Node,
        NT_ENode
    };


};
#endif // WIDGET_H
