#ifndef EXTRACTWIDGET_H
#define EXTRACTWIDGET_H


#include <QDialog>
#include <QTextEdit>
#include <QPushButton>
#include "HashType.h"
#include "objdata.h"

class ExtractHashDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ExtractHashDialog(QWidget *parent = nullptr);
    ExtractHashDialog(QWidget *parent, HashType hashType, ObjData *objData);

signals:
    void errorOccurred(QString& errorMessage);
//    void hashStringGenerated(QString& hashString);
    void HashStringExtract();

protected:
    QString fileStr;
    HashType type;
    QPushButton *extractButton;
    QTextEdit *outputTextEdit;
    ObjData *objData;
protected slots:
    void HashExtract();

};

#endif // EXTRACTWIDGET_H
