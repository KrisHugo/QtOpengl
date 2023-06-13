# QtOpengl

## for render polygon correctly, we need to know:
```c++
//In QT
//直接使用vertex buffer渲染的方法
void QOpenGLFunctions::glDrawArrays(
         GLenum mode, 
         GLint first, 
         GLsizei count);
//使用indices buffer作为vertex buffer中顶点缓存渲染顺序的渲染方法
void QOpenGLFunctions::glDrawElements(
         GLenum  mode,
         GLsizei count,
         GLenum  type,
   const GLvoid  *indices);
```
> 本项目中使用的是glDrawArrays而不是DrawElements, 是因为暂时没有解决vertex的attributes的实际indices按顺序读取的过程中实际是不同的, 这导致我无法正确使用indices来读取vertex. 因此我只能暂时在cpu中手动将原本的indices遍历获取渲染模型的正确vertex buffer, 但实际上这个过程中会重复保存很多的顶点缓存, 导致数据量极大, 但我暂时没有什么好的解决办法, 查了诸多opengl的资料也暂时没有找到. 但这个点是提升渲染效率的极重要的一点.

GL_enum : see in "./Readme/GL_enum.jpg"

[test](https://github.com/KrisHugo/QtOpengl/blob/master/Readme/GLenum.png)

<!-- <img src="./README/GLenum.png" style="zoom:100%;" /> -->

## 针对被渲染物体的整体控制思路
- 每个顶点在加入vertex shader中渲染时, 需要计算顶点的实际渲染坐标, 可以通过输入四个参数进行控制: projection, view, viewPos, model.
- 其中projection和view以及viewPos(或者可以直接说camera.position)控制3D物体渲染到摄像头上的2D投影坐标, model是每个顶点所归属的3d模型的中心位置, 四个参数与顶点vertex的属性运算之后, 就可以确定实际渲染的点在3d空间中的位置，以及正确的渲染在摄像机的投影位置.

## To-do
- Find a Covex Hull Solutions for 3D models.

## Done
- optimize the structure of a obj model load on memory
  - save facets with dictionary to categorize the facets by its vPoints count.
- 针对一个模型的一部分或者obj进行单独操作应该如何处理? 可能必须在facet属性中记录所属的obj或group, 同时由于模型之间是相连的, 实际要操作的不是facet的indices而是indices指向的vertex属性, 所以只要修改vertex的数据, 整个部分就可以直接被移动, 旋转, 缩放, 而不需要顾虑与其他部分的链接问题.

```c++
class facets{
  facets_uid;
  // v v/t/n v/t v//n
  vector<float> vpointsIndex;//we haven't know the size of a facets until we loaded it.
  vector<float> tpointsIndex;//there might be null while we are loading, so we might check if it exists.
  vector<float> npointsIndex;//there might be null while we are loading, so we might check if it exists.
}
class ObjModel{
  vector<vec3> vpoints;
  vector<vec2> tpoints;
  vector<vec3> npoints;
  vector<string> obj;
  unordered_map<string, vector<uid>> facetsInObj; //for modifing a whole part of a objModel.
  unordered_map<int, vector<uid>> facetsInSize; //for rendering the whole objModel on Screen by opengl functions correctly.
  //... a obj model might be more complex, such as: smooth, mtl, etc. and I will and be willing to fix all of them to build this project after I finish my current emergent works.
  // texture for rendering correct and comfortable for viewers.
}

```

- render polygon : using indics buffer and while we loaded the multi-vertex facet, we transform the orginal indices to the indices that opengl can draw the facet as plenty triangles combined.
  - for example : a rectangle can be drawed as two right triangle.

> 由于我们存储的时候直接通过facetsInSize进行存储, 而facets实际可以绑定为indices buffer, opengl就可以直接根据indices buffer依次获取顶点索引, 然后根据索引从vertex buffer中找到对应顶点进行渲染.
> 1. facet为三角面, 直接进行GL_Triangle模式渲染
> 2. facet为多角面, opengl为我们提供了GL_Triangle_Stripe方法进行连续三角面渲染, 但实际上如果为了这为数不多的多角面单独去判断并单独设置缓存是不划算的.
> 3. 因此, 但facet为多角面时, 与其让opengl自动渲染, 不如我们提前根据一个方式将每个多角面转换成可以通过GL_Triangle的形式直接渲染的多个三角面
> 而转换的逻辑实际上与GL_Triangle_Stripe的方法很类似, 即从第四个顶点开始, 每个顶点前新增两个顶点来构成一个新的三角形, 而新增的两个顶点分别为: 这个facet的第一个顶点以及当前要渲染成新三角面的顶点在这个facet的顶点序列中的前一个顶点.
> 
> for example: 0,1,2,3 -> 0,1,2,0,2,3 / 0,1,2,3,4 -> 0,1,2,0,2,3,0,3,4 ...

## Bugs
- file "./resources/Iphone_second_version_finished.obj" can't be rendered correctly and I dont know the reason.

## Fixed:
- readline: while I was trying to read each line of a obj file, I realize at the end of a line, there might be "\r\n" or "\n" which we need to remove it, but I just simply remove the last byte of each line rather than set a reg to remove, and it might cause some problem.
- vertex: I can't render the obj model with facets which has more than 3 vertex.
- render: I can't select the model rendered in screen because I don't know how.

## Reference:
- [基础理论专栏目录](https://zhuanlan.zhihu.com/p/143212327)
  - [凸包问题](https://zhuanlan.zhihu.com/p/158043191)
