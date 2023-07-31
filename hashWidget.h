#ifndef HASHWIDGET_H
#define HASHWIDGET_H
#include <QFile>
#include <QDialog>
#include <QCryptographicHash>
#include <QDebug>
#include <QMessageBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>
#include "HashType.h"

class HashDialog : public QDialog
{
    Q_OBJECT
public:
    explicit HashDialog(QWidget *parent = nullptr);
    HashDialog(QWidget *parent, QString &_fileStr, HashType hashType);

signals:
    void errorOccurred(QString& errorMessage);
    void hashStringGenerated(QString& hashString);
    void HashStringInsert();

protected:
    QString fileStr;
    HashType type;

    QLabel *filePathLabel;
    QLineEdit *filePathLineEdit;
    QPushButton *calculateButton;
    QPushButton *insertButton;
    QTextEdit *outputTextEdit;
protected slots:
    void HashGenerate();
    void HashInsert();
};


#endif // HASHWIDGET_H
