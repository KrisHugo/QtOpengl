
#include "hashWidget.h"

HashDialog::HashDialog(QWidget *parent)  : QDialog(parent)
{

}

HashDialog::HashDialog(QWidget *parent, QString &_fileStr, HashType _type) : QDialog(parent)
{
    fileStr = _fileStr;
    type = _type;

    //input widget

    // 创建窗口的控件
    filePathLabel = new QLabel("File Path:");
    filePathLineEdit = new QLineEdit(fileStr);
    calculateButton = new QPushButton("Calculate");
    insertButton = new QPushButton("嵌入凸包模型");
    outputTextEdit = new QTextEdit;

    // 设置布局
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(filePathLabel);
    layout->addWidget(filePathLineEdit);
    layout->addWidget(calculateButton);
    layout->addWidget(outputTextEdit);
    layout->addWidget(insertButton);
    setLayout(layout);

    connect(calculateButton, SIGNAL(clicked()), this, SLOT(HashGenerate()));
    connect(insertButton, SIGNAL(clicked()), this, SLOT(HashInsert()));
    setWindowTitle("哈希生成");
}

void HashDialog::HashGenerate()
{
    // 打开文件
    QFile file(fileStr);
    if (!file.open(QIODevice::ReadOnly))
    {
        QString errorMessage = "Failed to open file: " + fileStr;
        emit errorOccurred(errorMessage);
        return;
    }
    if(type == HashType::SHA256){
        // 创建QCryptographicHash对象并设置哈希算法为SHA256
        QCryptographicHash hash(QCryptographicHash::Sha256);

        // 逐行读取文件内容并添加到哈希对象中
        while (!file.atEnd())
        {
            QByteArray line = file.readLine().trimmed();  // 去除行首尾的空白字符
            if (!line.startsWith("#"))  // 跳过以"#"开头的注释行
            {
                hash.addData(line);
            }
        }

        // 获取计算后的哈希值
        QByteArray hashResult = hash.result();

        // 将字节数组转换为十六进制字符串
        QString hashString = hashResult.toHex();

        outputTextEdit->setText(hashString);
        emit hashStringGenerated(hashString);
    }
}

void HashDialog::HashInsert(){
    emit HashStringInsert();
}
