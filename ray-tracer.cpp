/// Include the image write library and enable it
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <list>

/// Uncomment these lines if you want to debug using couts
#include <iostream>
using namespace std;

class Vector3
{
public:
  float x, y, z;

  Vector3 &operator+=(const Vector3 &rhs)
  {
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    return *this;
  }

  Vector3 &operator-=(const Vector3 &rhs)
  {
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;
    return *this;
  }

  Vector3 &operator*=(const float rhs)
  {
    x *= rhs;
    y *= rhs;
    z *= rhs;
    return *this;
  }

  float dot(const Vector3 &rhs)
  {
    return x * rhs.x + y * rhs.y + z * rhs.z;
  }

  Vector3 cross(const Vector3 &rhs)
  {
    return Vector3{
        y * rhs.z - z * rhs.y,
        z * rhs.x - x * rhs.z,
        x * rhs.y - y * rhs.x};
  }

  float length()
  {
    return sqrt(x * x + y * y + z * z);
  }

  Vector3 normalize()
  {
    float l = length();
    return Vector3{x / l, y / l, z / l};
  }
};

inline Vector3 &operator+(Vector3 lhs, const Vector3 &rhs)
{
  return lhs += rhs;
}

inline Vector3 &operator-(Vector3 lhs, const Vector3 &rhs)
{
  return lhs -= rhs;
}

inline Vector3 &operator*(Vector3 lhs, const float rhs)
{
  return lhs *= rhs;
}

/// Uncomment these lines if you happen to add a Vector3 and want to debug :)
inline std::ostream &operator<<(std::ostream &os, const Vector3 &v)
{
  return os << '(' << v.x << ", " << v.y << ", " << v.z << ')';
}

class Triangle
{
  public:
  Vector3 one, two, three;
  int r,g,b;
};

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
  
  Triangle triangle = Triangle{Vector3{0.5, 1, 0}, Vector3{-1, -1, 0}, Vector3{1, -1, 0}};
  Triangle triangle2 = Triangle{Vector3{1, 1, 0}, Vector3{-1, -1, 0}, Vector3{1, -1, 0}};
  list<Triangle> triangles = {triangle, triangle2};

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


      Vector3 target
      {
        x/(float)w*2-1, -(y/(float)h*2-1) , 0
      };

      Vector3 ray = (target - camera_orgin).normalize();

      for (Triangle triangle : triangles){
        Vector3 edge_one = triangle.one - triangle.two;
        Vector3 edge_two = triangle.three - triangle.two;
        Vector3 p = edge_two.cross(edge_one);
        Vector3 abc = p.normalize();
        float d = -abc.dot(triangle.one);

        float t = -(abc.dot(camera_orgin) + d)/abc.dot(ray);

        Vector3 collision_point = ray*t+camera_orgin;

        Vector3 minus_one = collision_point - triangle.one;
        Vector3 minus_two = collision_point - triangle.two;
        Vector3 minus_three = collision_point - triangle.three;

        float triangle_area = p.length()/2;

        float area_one = minus_two.cross(minus_three).length()/2;
        float area_two = minus_three.cross(minus_one).length()/2;
        float area_three = minus_one.cross(minus_two).length()/2;

        float u = area_one/triangle_area;
        float v = area_two/triangle_area;
        float a = area_three/triangle_area;

        float sum = u + v + a;

        float diff = abs(1-sum);

        if(diff > 0.00001)
        {
          //I'm not in the triangle
        }
        else
        {
          //I'm in the triangle
          r = 0 * u + 0 * v + 0 * a;
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
