#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QToolBar>
#include <QLabel>
#include <QTextEdit>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QFileDialog>
#include "MainRenderer.h"
class Widget : public QWidget
{
    Q_OBJECT

public slots:
    void SelectFile();

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();


//    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    int loadFileFlag;
    QMenuBar *mainMenuBar;
    QMenu *mainMenu;
    QAction *openAction;
    QAction *newAction;
//    QToolBar *compsBoxToolBar;
    QLabel *listComp;
    QLabel *lbl;
    QLabel *renderDetail;
    QLabel *renderStatus;
    RendererWidget *renderer;






};
#endif // WIDGET_H
