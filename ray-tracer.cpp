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

using namespace std;


/// Width of the final image
  const int w = 640;

  /// Height of the final image
  const int h = 640;

  /// The 1d array for storing the pixel we have generated
  unsigned char pixels[w * h * 3];

 //Cornell Box
 // Left Wall
  Triangle triangle01 = Triangle{Vector3{-1, 1, -2}, Vector3{-1, 1, -1}, Vector3{-1, -1, -2}};
  Triangle triangle02 = Triangle{Vector3{-1, 1, -1}, Vector3{-1, -1, -1}, Vector3{-1, -1, -2}};

  // Right Wall
  Triangle triangle03 = Triangle{Vector3{1, 1, -1}, Vector3{1, 1, -2}, Vector3{1, -1, -2}};
  Triangle triangle04 = Triangle{Vector3{1, -1, -1}, Vector3{1, 1, -1}, Vector3{1, -1, -2}};

  // Top Wall
  Triangle triangle05 = Triangle{Vector3{-1, 1, -1}, Vector3{-1, 1, -2}, Vector3{1, 1, -2}};
  Triangle triangle06 = Triangle{Vector3{1, 1, -1}, Vector3{-1, 1, -1}, Vector3{1, 1, -2}};

  // Bottom Wall
  Triangle triangle07 = Triangle{Vector3{-1, -1, -2}, Vector3{-1, -1, -1}, Vector3{1, -1, -2}};
  Triangle triangle08 = Triangle{Vector3{-1, -1, -1}, Vector3{1, -1, -1}, Vector3{1, -1, -2}};

  // Back Wall
  Triangle triangle09 = Triangle{Vector3{-1, 1, -2}, Vector3{-1, -1, -2}, Vector3{1, -1, -2}};
  Triangle triangle10 = Triangle{Vector3{1, 1, -2}, Vector3{-1, 1, -2}, Vector3{1, -1, -2}};

  vector<Triangle> triangles = {
      triangle01, triangle02,
      triangle03, triangle04,
      triangle05, triangle06,
      triangle07, triangle08,
      triangle09, triangle10};

  Vector3 background_color = Vector3{0, 0, 0};


  float trace_ray_distance(Vector3 orgin, Vector3 direction )
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

        if(t > closest_t || t <= 0)
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

  Vector3 trace_ray(Vector3 orgin, Vector3 direction, int bounces = 2 )
  {
    Vector3 color = Vector3{background_color.x, background_color.y, background_color.z};
    if(bounces <= 0)
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

        if(t > closest_t || t <= 0)
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

          Vector3 base_color = Vector3{255, 255, 255};
          Vector3 light_direction = Vector3{.5, 1, .1}.normalize();

          Vector3 ambient = base_color * .1;
          Vector3 normal = p.normalize();

          float strength = max(0.0f, light_direction.dot(normal));

          Vector3 diffuse = base_color * strength;


          Vector3 refelctive = Vector3{0,0,0};

          if(true){
            Vector3 refelctive_distance = direction - normal * 2 * normal.dot(direction);
            diffuse *= .5;
            ambient *= .5;
          }

          Vector3 _color = ambient + diffuse + refelctive;
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

  Vector3 temp = Vector3{0, 0, 0};
  temp += Vector3{1, 1, 1};

  cout << temp << endl;

  // ///Parse Obj File
  // string file = "untitled.obj";
  // vector<string> lines = readLines(file);

  // vector<vector<string>> vectors = {};
  // vector<vector<string>> uv = {};
  // vector<vector<string>> normals = {};
  // vector<vector<string>> faces = {};

  // int index = 0;
  // vector<vector<string>> splitLines;

  // //split each line
  // for (const string& line : lines) {
  //   index = 0;
  //   vector<string> splitLine = split(line, ' ');
  //   splitLines.push_back(splitLine);
  // }

  // //sort obj file
  // for (vector<string>& line : splitLines){
  //   if(line.front() == "v")
  //     vectors.push_back(line);
  //   else if (line.front() == "vt")
  //     uv.push_back(line);
  //   else if (line.front() == "vn")
  //     normals.push_back(line);
  //   else if (line.front() == "f")
  //     faces.push_back(line);
  //   else
  //     continue;
  // }

  // //setup faces
  // vector<vector<string>> processedFaces = {};

  // for (const vector<string> line : faces){
  //   for(int i = 1; i < line.size(); i++ ){
  //     processedFaces.push_back(split(line[i], '/'));  
  // }

  // cout << processedFaces[2][0] << endl;

  // //Create Triangles
  // vector<Triangle> triangles = {};
  
  // for (vector<string> face : processedFaces)
  // {
  //   Triangle triangle;
  //   triangle.v_1 = Vector3(stof(vectors[stoi(processedFaces[0][0]) - 1][0]),
  //                          stof(vectors[stoi(processedFaces[0][0]) - 1][1]),
  //                          stof(vectors[stoi(processedFaces[0][0]) - 1][2]));

  //   triangle.v_2 = Vector3(stof(vectors[stoi(processedFaces[1][0]) - 1][0]),
  //                          stof(vectors[stoi(processedFaces[1][0]) - 1][1]),
  //                          stof(vectors[stoi(processedFaces[1][0]) - 1][2]));

  //   triangle.v_3 = Vector3(stof(vectors[stoi(processedFaces[2][0]) - 1][0]),
  //                          stof(vectors[stoi(processedFaces[2][0]) - 1][1]),
  //                          stof(vectors[stoi(processedFaces[2][0]) - 1][2]));

  //   triangle.vt_1 = Vector3(stof(uv[stoi(processedFaces[0][1]) - 1][0]),
  //                           stof(uv[stoi(processedFaces[0][1]) - 1][1]),
  //                           stof(uv[stoi(processedFaces[0][1]) - 1][2]));

  //   triangle.vt_2 = Vector3(stof(uv[stoi(processedFaces[1][1]) - 1][0]),
  //                           stof(uv[stoi(processedFaces[1][1]) - 1][1]),
  //                           stof(uv[stoi(processedFaces[1][1]) - 1][2]));

  //   triangle.vt_3 = Vector3(stof(uv[stoi(processedFaces[2][1]) - 1][0]),
  //                           stof(uv[stoi(processedFaces[2][1]) - 1][1]),
  //                           stof(uv[stoi(processedFaces[2][1]) - 1][2]));

  //   triangle.vn_1 = Vector3(stof(normals[stoi(processedFaces[0][2]) - 1][0]),
  //                           stof(normals[stoi(processedFaces[0][2]) - 1][1]),
  //                           stof(normals[stoi(processedFaces[0][2]) - 1][2]));

  //   triangle.vn_2 = Vector3(stof(normals[stoi(processedFaces[1][2]) - 1][0]),
  //                           stof(normals[stoi(processedFaces[1][2]) - 1][1]),
  //                           stof(normals[stoi(processedFaces[1][2]) - 1][2]));

  //   triangle.vn_2 = Vector3(stof(normals[stoi(processedFaces[2][2]) - 1][0]),
  //                           stof(normals[stoi(processedFaces[2][2]) - 1][1]),
  //                           stof(normals[stoi(processedFaces[2][2]) - 1][2]));
    
  //   triangles.push_back(triangle);
  // }

  Vector3 camera_orgin = Vector3{0, 0, 1};
  Vector3 camera_lookat = Vector3{0, 0, 0};
  Vector3 camera_up = Vector3{0, 1, 0};

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
          x / (float)w * 2 - 1, -(y / (float)h * 2 - 1), 0};

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
