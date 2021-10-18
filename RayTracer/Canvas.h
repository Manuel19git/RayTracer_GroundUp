#pragma once
#include <iostream>
#include <fstream>
#include "tuples.h"

class Canvas
{
public:

	//Constructors
	Canvas(float newWidth, float newHeight) : width(newWidth), height(newHeight) 
	{
		//When accessing the matrix -> [col/width][row/height]
		pixel_matrix = new Color * [newWidth];
		for (int i = 0; i < newWidth; ++i)
		{
			pixel_matrix[i] = new Color[newHeight];
		}
	};

	//Methods
	void canvas_to_ppm();

	int width;
	int height;
	Color **pixel_matrix;

};