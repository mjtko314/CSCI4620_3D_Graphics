/// Include the image write library and enable it
#include <ostream>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <vector>

/// Uncomment these lines if you want to debug using couts
#include <iostream>
#include "Vector3.hpp"
#include "Triangle.hpp"
#include "file_util.h"

using namespace std;

int main()
{
  /// Example of how to debug with couts.
  /// You must uncomment the include and namespace lines above
  // cout << "Hello, world" << endl;

  Vector3 temp = Vector3{0, 0, 0};
  temp += Vector3{1, 1, 1};

  cout << temp << endl;

  /// Width of the final image
  const int w = 640;

  /// Height of the final image
  const int h = 640;

  /// The 1d array for storing the pixel we have generated
  unsigned char pixels[w * h * 3];

  ///Parse Obj File
  string file = "untitled.obj";
  vector<string> lines = readLines(file);

  vector<vector<string>> vectors = {};
  vector<vector<string>> uv = {};
  vector<vector<string>> normals = {};
  vector<vector<string>> faces = {};

  int index = 0;
  vector<vector<string>> splitLines;

  //split each line
  for (const string& line : lines) {
    index = 0;
    vector<string> splitLine = split(line, ' ');
    splitLines.push_back(splitLine);
  }

  //sort obj file
  for (vector<string>& line : splitLines){
    if(line.front() == "v")
      vectors.push_back(line);
    else if (line.front() == "vt")
      uv.push_back(line);
    else if (line.front() == "vn")
      normals.push_back(line);
    else if (line.front() == "f")
      faces.push_back(line);
    else
      continue;
  }

  //setup faces
  vector<vector<string>> processedFaces = {};
  for (const vector<string> line : faces){
    for(int i = 1; i < line.size(); i++ ){
      processedFaces.push_back(split(line[i], '/'));
    
  }

  //Create Triangles
  vector<Triangle> triangles = {};
  
  for (vector<string> face : processedFaces)
  {
    vector<Vector3> triangle = {};
    vector<string> targetVector = vectors[stoi(face[0]) - 1];
    triangle.push_back(Vector3(stof(targetVector[0]), stof(targetVector[1]), stof(targetVector[2])));
  }


  Vector3 background_color = Vector3{120, 81, 169};

  /// Loop over each line doing down
  for (int y = 0; y < h; y++)
  {
    /// While going down, loop over every pixel going right
    for (int x = 0; x < w; x++)
    {

      /// Figure out our index in the 1d array of pixels
      int index = y * w * 3 + x * 3;

      /// The value of the red channel (0-255)
      unsigned char r = background_color.x;

      /// The value of the green channel (0-255)
      unsigned char g = background_color.y;

      /// The value of the blue channel (0-255)
      unsigned char b = background_color.z;

      // How do I ray trace?
      Vector3 camera_orgin = Vector3{0, 0, 1};
      Vector3 camera_lookat = Vector3{0, 0, 0};
      Vector3 camera_up = Vector3{0, 1, 0};

      Vector3 target{
          x / (float)w * 2 - 1, -(y / (float)h * 2 - 1), 0};

      Vector3 ray = (target - camera_orgin).normalize();

      float closest_t = 1000000;

      /// loop over each triangle
      for (const Triangle &triangle : triangles)
      {
        Vector3 edge_one = triangle.one - triangle.two;
        Vector3 edge_two = triangle.three - triangle.two;
        Vector3 p = edge_two.cross(edge_one);
        Vector3 abc = p.normalize();
        float d = -abc.dot(triangle.one);

        float t = -(abc.dot(camera_orgin) + d) / abc.dot(ray);

        if(t > closest_t)
          continue;

        Vector3 collision_point = ray * t + camera_orgin;

        Vector3 minus_one = collision_point - triangle.one;
        Vector3 minus_two = collision_point - triangle.two;
        Vector3 minus_three = collision_point - triangle.three;

        float triangle_area = p.length() / 2;

        float area_one = minus_two.cross(minus_three).length() / 2;
        float area_two = minus_three.cross(minus_one).length() / 2;
        float area_three = minus_one.cross(minus_two).length() / 2;

        float u = area_one / triangle_area;
        float v = area_two / triangle_area;
        float a = area_three / triangle_area;

        float sum = u + v + a;

        float diff = abs(1 - sum);

        if (diff > 0.00001)
        {
          // I'm not in the triangle
        }
        else
        {
          // I'm in the triangle
          closest_t = t;

          r = 0 * u + 0 * v + 255 * a;
          g = 0 * u + 255 * v + 0 * a;
          b = 255 * u + 0 * v + 0 * a;
        }

        /// Assign the r, g, and b values to the pixel at index
        pixels[index] = r;
        pixels[index + 1] = g;
        pixels[index + 2] = b;
      }
    }
  }

  /// Write the final image as a png
  stbi_write_png("image.png", w, h, 3, pixels, w * 3);
  return 0;
}
