# OpenGL

These are some notes I decided to take while working on the [Hello
Triangle example](https://learnopengl.com/Getting-started/Hello-Triangle).

- Since OpenGL 3.0, there have been 2 modes of operation or contexts:
  _compatible_ and _core_, the former being the oldest.
- API calls that aim to indicate which version of the specification you
  want to use in your application must be made before the OpenGL window
  is actually created.
- A vertex array object (VAO) can be used to store vertex attribute
  configuration, so they can be easily recalled by binding the VAO.
- Use of VAOs is required by OpenGL Core.
- When drawing geometry that is made up of more than a single triangle,
  one can draw triangle strips or use element buffer objects, which
  consist of indices to the vertex data.
- The last element buffer object that gets bound while a VAO is bound,
  is stored as the VAO's element buffer object. Binding to a VAO then
  also automatically binds that EBO.

