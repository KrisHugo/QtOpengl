# QtOpengl

## for render polygon currectly, we need to know:

void QOpenGLFunctions::glDrawArrays(GLenum mode, GLint first, GLsizei count)

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
  unordered_map<string, uid> facetsInObj; //for modifing a whole part of a objModel.
  unordered_map<int, uid> facetsInSize; //for rendering the whole objModel on Screen by opengl functions correctly.
  //... a obj model might be more complex, such as: smooth, mtl, etc. and I will and be willing to fix all of them to build this project after I finish my current emergent works.
  // texture for rendering correct and comfortable for viewers.
}

```
- render polygon : using indics buffer and while we loaded the multi-vertex facet, we transform the orginal indices to the indices that opengl can draw the facet as plenty triangles combined.
  - for example: a rectangle can be drawed as two right triangle.
## To fix: ReadFile
while I was trying to read each line of a obj file, I realize at the end of a line, there might be "\r\n" or "\n" which we need to remove it, but I just simply remove the last byte of each line rather than set a reg to remove, and it might cause some problem.

## current problem:
- I can't render the obj model with facets which has more than 3 vertex.
- I can't select the model rendered in screen because I don't know how.
