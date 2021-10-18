#include "Canvas.h"
#ifdef _WIN32
#include <windows.h>
#define SYSERROR()  GetLastError()
#else
#include <errno.h>
#define SYSERROR()  errno
#endif
//Clamp value between range [0,255]
int clamp(int value)
{
	if (value > 255) return 255;
	else if (value < 0) return 0;
	else return value;
}

void Canvas::canvas_to_ppm()
{
	string ppm;
	string ppm_header;
	ppm_header = "P3\n";
	ppm_header += to_string(width) + " " + to_string(height) + "\n";
	ppm_header += "255\n";

	string ppm_matrix = "";
	string line = "";
	for (int row = 0; row < height; ++row)
	{
		for (int col = 0; col < width; ++col)
		{
			//Makes sure that a line doesnt exceed 70 characters
			int counter = 70;
			if (line.size() > 70)
			{
				while (line[counter] != ' ') counter--;
				ppm_matrix += line + "\n";
				line = "";
			}
			//Change value from range[0,1] to range[0,255] with Round Up
			line += to_string(clamp((int)round(pixel_matrix[col][row].r * 255))) + " ";
			line += to_string(clamp((int)round(pixel_matrix[col][row].g * 255))) + " ";
			line += to_string(clamp((int)round(pixel_matrix[col][row].b * 255))) + " ";
		}
		ppm_matrix += line + "\n";
		line = "";
	}

	ppm = ppm_header + ppm_matrix;

	ofstream outfile("canvas.ppm");

	if (outfile.is_open())
	{
		outfile << ppm;
		outfile.flush();
		outfile.close();
		std::cout << "wrote the file successfully!" << std::endl;
	}
	else
	{
		std::cerr << "Failed to open file : " << SYSERROR() << std::endl;
	}

}



