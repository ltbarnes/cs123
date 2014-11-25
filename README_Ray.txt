Project: Ray
Student: Logan Barnes (ltbarnes)


DESIGN

#################################### From Intersect ###################################
	
	RayShapes:
		A new class RayShape was created to accommodate intersection mathematics without dragging along all the unnecessary OpenGL functions. The base class for all RayShapes is an axis-aligned bounding box (class AABB) of unit length centered at the origin. All shapes check for intersection with this box before attempting to find an intersection point on the main shape (except cube obviously).
		

	RayScene Class:
		The SceneviewScene, ShapeScene, and RayScene classes are sub classes of Scene which maintains the lists of scene objects and lights. This allows one scene to retrieve data from other scene regardless of the scene type. This is used to transfer data from either the SceneviewScene or the ShapeScene to the RayScene before rendering an image to the 2D canvas.


#################################### Additions for Ray ###################################

	Raycursion:
		The rayTrace method in the RayTaskBlock class (part of RayScene) was modified to allow for reflection calculations. After the initial ray is calculated based on the pixel location and camera, the raycursion() method is called to recursively calculate color intensities for reflected rays.

			- RECURSION_DEPTH is defined at the top of RayScene.cpp. (1 = no reflection)


	Textures:
		QImages are stored in a QHash with their corresponding filenames as keys. This prevents the same images from being stored multiple times for different objects. To sample the file appropriately each shape has a method getUV(point, normal) which returns the proper uv sampling coordinates for the given point in object space.

		The ambient term is multiplied by the texture value when texture mapping is active so the texture doesn't disappear in areas where there is no diffuse lighting.


EXTRA

	Shapes and Orbit Cam:
		The base class Scene for OpenGLScene was modified so both ShapesScene and SceneviewScene contain the same basic data structures. The Orbit cam was modified to include a getScaleMatrix() method so ray tracing should work on the shapes scene and when using the orbit cam.


	KDTree:
		The KDTree is a collection of KDNodes each with two KDNode children. The children are created by designating a "splitting plane" within each parent and constructing the sub volumes from the AABBs created by the split. If a node only has one shape in it or exceeds a specified max depth the node is designated a leaf node and both children are null.


	Multithreading:
		Rows of of the 2D canvas are calculated in a RayTask object which extends QObject. Normally one RayTask operates on the main thread until it finishes then a new RayTask is created and the process continues. When multithreading is selected multiple RayTasks are created and run using QtConcurrent::map and monitored with a QFutureWatcher object. As the threads finish running, signals are sent to the canvas to repaint the image and to the window to reset the buttons.


	Super Sampling:
		When super sampling is selected samples are taken from 4 points within the pixel and the variance between those colors is calculated. If the variance is above a certain threshold more samples are taken based on randomly taken based on the number of samples requested.

		The pixels with a variance above the threshold can be visualized by setting the sample number to 3 or higher and selecting the "Visualize Adaptive Samples" checkbox.

	
	Spotlight:
		A super basic spotlight implementation is used to render scenes like spotlight_on_cheese.


BUGS:
	
	I'm sure there are plenty out there with these threads poking around but I haven't found any big ones yet.
	