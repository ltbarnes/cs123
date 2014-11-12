Project: Sceneview
Student: Logan Barnes (ltbarnes)


DESIGN

	Shape Class:
		The Shape class was updated to store and render texture data. The function transformAndRender() was also added to render shapes in arbitrary locations based on a transformation matrix.

	Scene Class:
		The Scene class creates copies of CS123ScenePrimitive and CS123SceneLightData structs and stores them in lists for the Sceneview class to call upon. When the parse() function is called the appropriate transformations for each CS123ScenePrimitive is calculated and stored in a list with a corresponding index.


	SceneviewScene Class:

		Shapes:
			One instance of each of the Cone, Cube, Cylinder, Sphere, and Torus shapes are stored as instance variables. When the scene is drawn the render method iterates through each of the CS123ScenePrimitives and renders the correct instance variable shape with the corresponding transformation matrix.

		Tessellation:
			When the scene is initialized the tessellation of the shapes are set based on the number of objects in the scene. The vertices and uv coordinates are then calculated for each shape and bound to the vertex array and buffer.

		Textures Loading:
			The textures are also loaded when the SceneviewScene is initialized by iterating through the CS123ScenePrimitives and getting the material texture filename. A QMap is used to temporarily store texture ids once they are loaded so multiple shapes with the same files can get the appropriate gl id.


ADDITIONAL NOTES:
	
	ShapeScene Class:
		ShapeScene now loads a single texture when initialized to make texture debugging easier (and to make the shapes look cool).

	Torus Texture:
		The texture mapping for the torus differs from the demo so the "seam" is on the inside of the ring. There aren't a lot of torus scenes (if any) so it doesn't affect many scenes but it seems like a more intuitive way to do it.


BUGS

	Don't know of any yet.