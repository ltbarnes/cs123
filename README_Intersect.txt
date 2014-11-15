Project: Intersect
Student: Logan Barnes (ltbarnes)


DESIGN

	RayShapes:
		A new class RayShape was created to accommodate intersection mathematics without dragging along all the unnecessary OpenGL functions. The base class for all RayShapes is an axis-aligned bounding box (class AABB) of unit length centered at the origin. All shapes check for intersection with this box before attempting to find an intersection point on the main shape (except cube obviously).
		

	RayScene Class:
		The SceneviewScene, ShapeScene, and RayScene classes are sub classes of Scene which maintains the lists of scene objects and lights. This allows one scene to retrieve data from other scene regardless of the scene type. This is used to transfer data from either the SceneviewScene or the ShapeScene in the 3D canvas to the RayScene before rendering an image to the 2D canvas.


	KDTree:
		The KDTree is a collection of KDNodes each with two KDNode children. The children are created by designating a "splitting plane" within each parent and constructing the sub volumes from the AABBs created by the split. If a node only has one shape in it or exceeds a specified max depth the node is designated a leaf node and both children are null.


	Super Sampling:
		When super sampling is selected samples are taken from each corner of the pixel in addition to the middle point then averaged together.


	Multithreading:
		Rows of of the 2D canvas are calculated in a RayTask object which extends QObject. Normally one RayTask operates on the main thread until it finishes then a new RayTask is created and the process continues. When multithreading is selected multiple RayTasks are created and run using QtConcurrent::map and monitored with a QFutureWatcher object. As the threads finish running, signals are sent to the canvas to repaint the image and to the window to reset the buttons.


	Shapes and Orbit Cam:
		The base class Scene for OpenGLScene was modified so both ShapesScene and SceneviewScene contain the same basic data structures. The Orbit cam was modified to include a getScaleMatrix() method so ray tracing should work on the shapes scene and when using the orbit cam.


BUGS:
	
	I'm sure there are plenty out there with these threads poking around but I haven't found any big ones yet.
	