# QtOpengl

## for render polygon currectly, we need to know:

void QOpenGLFunctions::glDrawArrays(GLenum mode, GLint first, GLsizei count)

void QOpenGLFucntions::glDrawIndices()

GL_enum : see in "./Readme/GL_enum.jpg"


<!-- <img src="./README/GLenum.png" style="zoom:100%;" /> -->

## To-do
- optimize the structure of a obj model load on memory
  - save facets with dictionary to categorize the facets by its vPoints count.


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

## To fix: ReadFile
while I was trying to read each line of a obj file, I realize at the end of a line, there might be "\r\n" or "\n" which we need to remove it, but I just simply remove the last byte of each line rather than set a reg to remove, and it might cause some problem.

## current problem:
- I can't render the obj model with facets which has more than 3 vertex.
- I can't select the model rendered in screen because I don't know how.
