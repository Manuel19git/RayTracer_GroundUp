#include "Camera.h"

//Returns a ray that starts that points from camera to pixel
myRay Camera::ray_for_pixel(int px, int py)
{
	//the offset from the edge of the canvas to the pixels center
	float xoffset = ((float)px + 0.5) * pixel_size;
	float yoffset = ((float)py + 0.5) * pixel_size;

	//the untransformed coordinates of the pixel in world space
	float world_x = half_width - xoffset;
	float world_y = half_height - yoffset;

	//using the camera matrix, transform the canvas point and the origin, 
	//and then compute the rays direction vector
	myPoint pixel, origin;
	MyMatrix inv = inverse(transform);
	pixel = inv * myPoint(world_x, world_y, -1);
	origin = inv * myPoint(0, 0, 0);
	myVector direction = pixel - origin;
	direction = direction.normalize();

	//Release memory
	inv.remove();

	return myRay(origin, direction);

}

//Renders a world with current camera and writes it to a ppm file
Canvas Camera::render(World world)
{
	//Define canvas
	Canvas canvas(hsize, vsize);

	//For each of the pixels of the camera shade
	for (int py = 0; py < vsize; ++py)
	{
		for (int px = 0; px < hsize; ++px)
		{
			myRay ray = ray_for_pixel(px, py);
			Color color = color_at(world, ray);
			canvas.pixel_matrix[px][py] = color;
		}
	}

	return canvas;
}