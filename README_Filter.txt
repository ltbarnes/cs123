Project: Shapes
Student: Logan Barnes (ltbarnes)

First off sorry about the lack of comments in the filter section. Time got away from me a bit :(

DESIGN

	Filter Class:
		All filters are subclasses of the Filter class. 

	GrayScale:
		Grayscale filtering is a function of Filter since Edge Detect uses it as well.

	Edge Detect, Blur, and Scale:
		Since all these classes contain edge case filtering the edges of the image are extended by mirroring the original image. This prevents the filters from accessing points off the edge of the image and preserves the basic color patterns. The filters are also separated to perform two 1D operations instead of one 2D operation.

	Rotate:
		This class calculates each pixel in the rotated image by reversing the transformation for each pixel in the destination canvas and finding the weighted average of the four nearest pixels from the source.



EXTRA (DON'T USE LARGE IMAGES FOR THESE!!! or do but it'll take forever)
	
	The following filters don't take the marquee selection into account.
	
	DFT:
		So I implemented a super speedy (not) DFT algorithm that transforms the image into it's frequency domain then graphs the magnitudes on a logarithmic scale with the larger frequencies in the middle.

	Inverse DFT:
		The Inverse DFT transforms a frequency image back into the spacial spectrum. This only works if used after the DFT filter since it uses the same frequency data to recreate the original. The pixels on the screen don't have any affect on the inverse transformation (that's what I was working toward but obviously ran out of time).

	Sharpen:
		This uses the DFT to get the frequency domain for the image, removes any values with a magnitude greater than 0.05, and uses the Inverse DFT to return the image to the spacial domain.