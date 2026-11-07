/// Include the image write library and enable it
#include <cmath>
#include <ostream>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <vector>
#include <algorithm>

/// Uncomment these lines if you want to debug using couts
#include <iostream>
#include "Vector3.hpp"
#include "Triangle.hpp"
#include "file_util.h"
#include <random>

using namespace std;

/// Width of the final image
const int w = 640;

/// Height of the final image
const int h = 640;

/// The 1d array for storing the pixel we have generated
unsigned char pixels[w * h * 3];

Vector3 background_color = Vector3{0, 0, 200};
vector<Triangle> triangles = {};

float trace_ray_distance(Vector3 orgin, Vector3 direction)
{
  float closest_t = INFINITY;

  for (const Triangle &triangle : triangles)
  {
    Vector3 edge_one = triangle.v_1 - triangle.v_2;
    Vector3 edge_two = triangle.v_3 - triangle.v_2;
    Vector3 p = edge_two.cross(edge_one);
    Vector3 abc = p.normalize();
    float d = -abc.dot(triangle.v_1);

    float t = -(abc.dot(orgin) + d) / abc.dot(direction);

    if (t > closest_t || t <= 0)
      continue;

    Vector3 collision_point = direction * t + orgin;

    Vector3 minus_one = collision_point - triangle.v_1;
    Vector3 minus_two = collision_point - triangle.v_2;
    Vector3 minus_three = collision_point - triangle.v_3;

    float triangle_area = p.length() / 2;

    float area_one = minus_two.cross(minus_three).length() / 2;
    float area_two = minus_three.cross(minus_one).length() / 2;
    float area_three = minus_one.cross(minus_two).length() / 2;

    float u = area_one / triangle_area;
    float v = area_two / triangle_area;
    float a = area_three / triangle_area;

    float sum = u + v + a;

    float diff = abs(1 - sum);

    if (diff > 0.00001 || isnan(sum))
    {
      // I'm not in the triangle
    }
    else
    {
      // I'm in the triangle
      closest_t = t;
    }
  }
  return closest_t;
}

Vector3 trace_ray(Vector3 orgin, Vector3 direction, int bounces = 2)
{
  Vector3 color = Vector3{background_color.x, background_color.y, background_color.z};
  if (bounces <= 0)
    return background_color;

  float closest_t = 1000000;

  for (const Triangle &triangle : triangles)
  {
    Vector3 edge_one = triangle.v_1 - triangle.v_2;
    Vector3 edge_two = triangle.v_3 - triangle.v_2;
    Vector3 p = edge_two.cross(edge_one);
    Vector3 abc = p.normalize();
    float d = -abc.dot(triangle.v_1);

    float t = -(abc.dot(orgin) + d) / abc.dot(direction);

    if (t > closest_t || t <= 0)
      continue;

    Vector3 collision_point = direction * t + orgin;

    Vector3 minus_one = collision_point - triangle.v_1;
    Vector3 minus_two = collision_point - triangle.v_2;
    Vector3 minus_three = collision_point - triangle.v_3;

    float triangle_area = p.length() / 2;

    float area_one = minus_two.cross(minus_three).length() / 2;
    float area_two = minus_three.cross(minus_one).length() / 2;
    float area_three = minus_one.cross(minus_two).length() / 2;

    float u = area_one / triangle_area;
    float v = area_two / triangle_area;
    float a = area_three / triangle_area;

    float sum = u + v + a;

    float diff = abs(1 - sum);

    if (diff > 0.00001 || isnan(sum))
    {
      // I'm not in the triangle
    }
    else
    {
      // I'm in the triangle
      closest_t = t;

      Vector3 base_color = Vector3{100, 0, 0};
      Vector3 light_direction = Vector3{.5, 1, .1}.normalize();

      Vector3 ambient = base_color * .1;
      Vector3 normal = p.normalize();

      float strength = max(0.f, light_direction.dot(normal));

      Vector3 diffuse = base_color * strength;
      if (trace_ray_distance(collision_point, light_direction) != INFINITY){
        diffuse = Vector3{0, 0, 0};
      }

      Vector3 reflective = Vector3{0, 0, 0};

      if (true)
      {
        Vector3 reflective_direction = direction - normal * 2 * normal.dot(direction);
        reflective = trace_ray(collision_point, reflective_direction, bounces - 1) * .5;
        diffuse *= .5;
        ambient *= .5;
      }

      Vector3 _color = ambient + diffuse + reflective;
      color = _color;
    }
  }
  return color;
}

int main()
{
  /// Example of how to debug with couts.
  /// You must uncomment the include and namespace lines above
  // cout << "Hello, world" << endl;

  /// Parse Obj File
  string file = "CornellBox.obj";
  vector<string> lines = readLines(file);

  vector<vector<float>> vectors = {};
  vector<vector<float>> uv = {};
  vector<vector<float>> normals = {};
  vector<vector<string>> faces = {};

  int index = 0;
  vector<vector<string>> splitLines;

  // split each line
  for (const string &line : lines)
  {
    index = 0;
    vector<string> splitLine = split(line, ' ');
    splitLines.push_back(splitLine);
  }

  // sort obj file
  for (vector<string> &line : splitLines)
  {
    vector<float> temp = {};
    if (line.front() == "v")
    {
      for (string x : line)
      {
        if (x != "v")
          temp.push_back(stof(x));
      }
      vectors.push_back(temp);
    }
    else if (line.front() == "vt")
    {
      for (string x : line)
      {
        if (x != "vt")
          temp.push_back(stof(x));
      }
      uv.push_back(temp);
    }
    else if (line.front() == "vn")
    {
      for (string x : line)
      {
        if (x != "vn")
          temp.push_back(stof(x));
      }
      normals.push_back(temp);
    }
    else if (line.front() == "f")
      faces.push_back(line);
    else
      continue;
  }

  // setup faces

  for (const vector<string> line : faces)
  {
    vector<vector<string>> processedFaces = {};
    for (int i = 1; i < line.size(); i++)
    {
      processedFaces.push_back(split(line[i], '/'));
    }

    // Create Triangles

    for (int i = 1; i < (int)processedFaces.size() - 1; i++)
    {
      Triangle triangle;
      triangle.v_1 = Vector3(vectors[stoi(processedFaces[0][0]) - 1][0],
                             vectors[stoi(processedFaces[0][0]) - 1][1],
                             vectors[stoi(processedFaces[0][0]) - 1][2]);

      triangle.v_2 = Vector3(vectors[stoi(processedFaces[i][0]) - 1][0],
                             vectors[stoi(processedFaces[i][0]) - 1][1],
                             vectors[stoi(processedFaces[i][0]) - 1][2]);

      triangle.v_3 = Vector3(vectors[stoi(processedFaces[i + 1][0]) - 1][0],
                             vectors[stoi(processedFaces[i + 1][0]) - 1][1],
                             vectors[stoi(processedFaces[i + 1][0]) - 1][2]);

      triangle.vt_1 = Vector3(uv[stoi(processedFaces[0][1]) - 1][0],
                              uv[stoi(processedFaces[0][1]) - 1][1],
                              0);

      triangle.vt_2 = Vector3(uv[stoi(processedFaces[i][1]) - 1][0],
                              uv[stoi(processedFaces[i][1]) - 1][1],
                              0);

      triangle.vt_3 = Vector3(uv[stoi(processedFaces[i + 1][1]) - 1][0],
                              uv[stoi(processedFaces[i + 1][1]) - 1][1],
                              0);

      triangle.vn_1 = Vector3(normals[stoi(processedFaces[0][2]) - 1][0],
                              normals[stoi(processedFaces[0][2]) - 1][1],
                              normals[stoi(processedFaces[0][2]) - 1][2]);

      triangle.vn_2 = Vector3(normals[stoi(processedFaces[i][2]) - 1][0],
                              normals[stoi(processedFaces[i][2]) - 1][1],
                              normals[stoi(processedFaces[i][2]) - 1][2]);

      triangle.vn_3 = Vector3(normals[stoi(processedFaces[i + 1][2]) - 1][0],
                              normals[stoi(processedFaces[i + 1][2]) - 1][1],
                              normals[stoi(processedFaces[i + 1][2]) - 1][2]);

      triangles.push_back(triangle);
    }
  }

  Vector3 camera_orgin = Vector3{0, 0, 1};
  Vector3 camera_lookat = Vector3{0, 0, 0};
  Vector3 camera_up = Vector3{0, 1, 0};

  ///Generate Offset
  int samples = 4;
  
  std::random_device rd;
  std::mt19937 gen(rd());

  std::uniform_real_distribution<double> distrib(-0.5, 0.5);
  float x_offset = distrib(gen);
  float y_offset = distrib(gen);
  std::cout << "Random decimal: " << x_offset << "\n";

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
      
      Vector3 target{
          (x + x_offset) / (float)w * 2 - 1, -((y+y_offset) / (float)h * 2 - 1), 0};

      Vector3 ray = (target - camera_orgin).normalize();

      Vector3 color = trace_ray(camera_orgin, ray);

      r = color.x;
      g = color.y;
      b = color.z;

      /// Assign the r, g, and b values to the pixel at index
      pixels[index] = r;
      pixels[index + 1] = g;
      pixels[index + 2] = b;
    }
  }

  /// Write the final image as a png
  stbi_write_png("image.png", w, h, 3, pixels, w * 3);
  return 0;
}