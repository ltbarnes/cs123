Project: Brush
Student: Logan Barnes (ltbarnes)

Design Considerations:

	SmudgeBrush:
		This class uses the QuadraticBrush super class to create a mask.

Extra Brushes:

	BombBrush:
		This brush takes random samples of paint from within the area designated by the radius and throws them around the screen as if a bomb went off. A crater is created at the center of the blast zone using a quadratic gradient. While the blobs are in the air they will move in the same general direction the mouse moves (if the mouse is moved) otherwise they just spread out in a circular pattern.

	Bubbler:
		This brush is the result of testing out different mask equations and stumbling upon one that creates bubble like features when used on color patterns (doesn't work on black).

Bugs:
	Hasn't crashed randomly yet and valgrind didn't show any memory leaks...so here's to hoping.