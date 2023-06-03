# QtOpengl

## for render polygon currectly, we need to know:

void QOpenGLFunctions::glDrawArrays(GLenum mode, GLint first, GLsizei count)

GL_enum

<img src="./README/GLenum.png" style="zoom:100%;" />

## current problem:
- I can't render the obj model with facets which has more than 3 vertex.
- I can't select the model rendered in screen because I don't know how.
