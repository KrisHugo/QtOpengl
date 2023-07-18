#ifndef BINARYMASK
#define BINARYMASK

#include <QtMath>
quint64 maskGenerate(int lMaskStartPos)
{
    return ((quint64)1 << (lMaskStartPos + 1)) - 1;
}

/*
 * 根据需要的水印信息部分，以及目标位置生成可以直接嵌入数据中的水印
 */
quint64 initialWaterMark(quint64 value, int position, int destination, int length){
    quint64 mask = ((quint64)1 << (position + 1)) - ((quint64)1 << (position - length + 1));
    return (value & mask) << (destination - position);
}

#endif
