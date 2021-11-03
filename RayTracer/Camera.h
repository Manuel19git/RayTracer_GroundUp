#pragma once
#include "Canvas.h"
#include "World.h"

class Camera
{
public:

	Camera(int horizontal_pixels, int vertical_pixels, float field_of_view)
	{
		hsize = horizontal_pixels;
		vsize = vertical_pixels;
		fov = field_of_view;

		//Computation of pixel size
		float half_view = tan(fov / 2); //Half view of canvas
		float aspect = (float)hsize / (float)vsize;

		if (aspect >= 1)
		{
			half_width = half_view;
			half_height = half_view / aspect;
		}
		else
		{
			half_width = half_view * aspect;
			half_height = half_view;
		}
		
		pixel_size = (half_width * 2) / hsize;

		transform = I_Matrix();
	}

	//Returns a ray that starts that points from camera to pixel
	myRay ray_for_pixel(int px, int py);

	//Renders a world with current camera and writes it to a canvas
	Canvas render(World world);



	//Horizontal size of canvas in pixels
	int hsize;
	//Vertical size of canvas in pixels
	int vsize;
	//Pixel size in a canvas one unit away
	float pixel_size;

	float half_width;
	float half_height;

	//Angle of vision in radians
	float fov;
	//Matrix that defines how the world is viewed from the camera
	MyMatrix transform = MyMatrix(4);
};
