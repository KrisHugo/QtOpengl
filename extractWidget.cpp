#include "extractWidget.h"

#include <QVBoxLayout>
ExtractHashDialog::ExtractHashDialog(QWidget *parent) : QDialog(parent)
{

}

ExtractHashDialog::ExtractHashDialog(QWidget *parent, HashType _type, ObjData *_objData) : QDialog(parent)
{
    //    fileStr = _fileStr;
    type = _type;

    //input widget

    // 创建窗口的控件
    //    filePathLabel = new QLabel("File Path:");
    //    filePathLineEdit = new QLineEdit(fileStr);
    //    calculateButton = new QPushButton("Calculate");
    extractButton = new QPushButton("提取水印信息");
    outputTextEdit = new QTextEdit;

    // 设置布局
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(extractButton);
    layout->addWidget(outputTextEdit);
    setLayout(layout);
    objData = _objData;
    //    connect(calculateButton, SIGNAL(clicked()), this, SLOT(HashGenerate()));
    connect(extractButton, SIGNAL(clicked()), this, SLOT(HashExtract()));
    setWindowTitle("哈希提取");
}

void ExtractHashDialog::HashExtract(){
    std::string watermark;
    objData->ExtractWatermark(objData->chData.vertices, objData->chData.nVertices, watermark);
    outputTextEdit->setText(QString::fromStdString(watermark));
}
