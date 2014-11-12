Project: Intersect
Student: Logan Barnes (ltbarnes)


DESIGN

	RayShapes:
		A new class RayShape was created to accommodate intersection mathematics without dragging along all the unnecessary OpenGL functions. The base class for all RayShapes is an axis-aligned bounding box (class AABB) of unit length centered at the origin. All shapes check for intersection with this box before attempting to find an intersection point on the main shape (except cube obviously).
		

	RayScene Class:
		The SceneviewScene, ShapeScene, and RayScene classes are sub classes of Scene which maintains the lists of scene objects and lights. This allows one scene to retrieve data from other scene regardless of the scene type. This is used to transfer data from either the SceneviewScene or the ShapeScene in the 3D canvas to the RayScene before rendering an image to the 2D canvas.


	KDTree:
		Work in progress. Hopefully it will be functional for Ray. Likely to see some remnants floating around the project.


	Super Sampling:
		When super sampling is selected samples are taken from each corner of the pixel in addition to the middle point then averaged together.


BUGS

	KDTree fails pretty hard when it's used on scenes with more than one shape.