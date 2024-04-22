# OpenGL

These are some notes I decided to take while working on the
[Hello Triangle example](https://learnopengl.com/Getting-started/Hello-Triangle).

- Since OpenGL 3.0, there have been 2 modes of operation or contexts:
  _compatible_ and _core_, the former being the oldest.
- API calls that aim to indicate which version of the specification you want to
  use in your application must be made before the OpenGL window is actually
  created.
- A vertex array object (VAO) can be used to store vertex attribute
  configuration, so they can be easily recalled by binding the VAO.
- Use of VAOs is required by OpenGL Core.
- When drawing geometry that is made up of more than a single triangle, one can
  draw triangle strips or use element buffer objects, which consist of indices
  to the vertex data.
- The last element buffer object that gets bound while a VAO is bound, is stored
  as the VAO's element buffer object. Binding to a VAO then also automatically
  binds that EBO.

## Following it up on 2024

It is April 2024 and, for one reason or another, I've felt like playing with
OpenGL again. I'm now reading some documentation and intend on creating a simple
example application to study the use of _ortographic projections_, so I can
experiment with the relationship between vertex coordinates and actual screen
pixels. My first goal is to try and precisely position some elements on the UI,
as if I were creating a GUI application. I then intend on working on another
example of something that resembles a 2D game scene. I would like to experiment
with effects such as screen shake and 2D rotation using OpenGL.

### Texture filtering

While going through the documentation, I bumped into a section on textures and,
more specifically, texture filtering. The gist of what I was able to gather:

- Texture filtering deals with deciding how texture data is obtained from an
  image (assuming 2D textures) based on texture coordinates. Since texture
  coordinates are continuous and texture pixels are discrete, there has to be
  some kind of mapping between the two. The way the actual pixel is selected or
  approximated depends on the filtering method used.
- `GL_NEAREST` is the default method and it simply selects the pixel whose
  center is closest to the texture coordinate.
- `GL_LINEAR`, also known as *bilinear filtering*, interpolates the colors of
  neighboring pixels to create a final color.
- `GL_NEAREST` results in a more blocky appearance, while `GL_LINEAR` leads to a
  more blurred version.

### Orthographic projection

To do

### TODO

- Reorganize dependencies so they can be reused by the experiments; it might be
  important to allow for different versions of the same dependency (e.g. glad)
  to coexist.
