Project: Shapes
Student: Logan Barnes (ltbarnes)


DESIGN

	Shape Class:
		All shapes are subclasses of the Shape class which takes care of setting parameters, updating opengl, updating the NormalRenderer, rendering the shape, and cleaning up arrays. All the subclasses have to do is define vertex locations by overriding the calcVerts() method and filling the m_vertexData array.

	GL_TRIANGLE_SHAPES:
		I decided to render using triangle_strips so a lot of points are added twice to create a break in the strip without making a separate array.


EXTRA

	Torus:
		It attempts to mimic the demo by varying the thickness based on parameter 3.

	Triangle:
		I had a kinda interesting curve going on but I hadn't figured out the normals and now the projects is due so it's back to just a pretty triangle.

	Ripple:
		An animated sine function radiating from the origin.

	Ripple Sphere:
		An extension of the sine plane from Ripple mapped to a sphere. The third parameter changes the frequency of the wave.


BUGS

	I decided to do all the rendering with GL_TRIANGE_STRIPS at the last second so I probably created a few bugs in the process (in addition to all the others I haven't found yet).