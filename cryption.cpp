#ifndef CRYPTION
#define CRYPTION

#include <QDebug>
#include <QFile>
#include <QString>
#include <QCryptographicHash>

QString generateSHA256(const QString& input)
{
    // 将输入转换为字节数组
    QByteArray data = input.toUtf8();

    // 创建QCryptographicHash对象并设置哈希算法为SHA256
    QCryptographicHash hash(QCryptographicHash::Sha256);

    // 添加要计算哈希值的数据
    hash.addData(data);

    // 获取计算后的哈希值
    QByteArray hashResult = hash.result();

    // 将字节数组转换为十六进制字符串
    QString hashString = hashResult.toHex();

    return hashString;
}

QString generateSHA256FromFile(const QString& fileName)
{
    // 打开文件
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Failed to open file: " << fileName;
        return QString();
    }

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

    return hashString;
}






#endif
