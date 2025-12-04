#include <iostream>
#include <cmath>
#include "Camera.hpp"
#include "../raymath/Ray.hpp"

// ============================================================================
// EVALUATION 3: Threading Support
// ============================================================================
#ifdef USE_THREADING
#include <thread>
#include <vector>
#endif

struct RenderSegment
{
public:
  int rowMin;
  int rowMax;
  Image *image;
  double height;
  double intervalX;
  double intervalY;
  int reflections;
  Scene *scene;
};

Camera::Camera() : position(Vector3())
{
}

Camera::Camera(Vector3 pos) : position(pos)
{
}

Camera::~Camera()
{
}

Vector3 Camera::getPosition()
{
  return position;
}

void Camera::setPosition(Vector3 &pos)
{
  position = pos;
}

/**
 * Render a segment (set of rows) of the image
 */
void renderSegment(RenderSegment *segment)
{

  for (int y = segment->rowMin; y < segment->rowMax; ++y)
  {
    double yCoord = (segment->height / 2.0) - (y * segment->intervalY);

    for (int x = 0; x < segment->image->width; ++x)
    {
      double xCoord = -0.5 + (x * segment->intervalX);

      Vector3 coord(xCoord, yCoord, 0);
      Vector3 origin(0, 0, -1);
      Ray ray(origin, coord - origin);

      Color pixel = segment->scene->raycast(ray, ray, 0, segment->reflections);
      segment->image->setPixel(x, y, pixel);
    }
  }
}

void Camera::render(Image &image, Scene &scene)
{

  double ratio = (double)image.width / (double)image.height;
  double height = 1.0 / ratio;

  double intervalX = 1.0 / (double)image.width;
  double intervalY = height / (double)image.height;

  scene.prepare();

// ============================================================================
// EVALUATION 3: Multithreading Implementation
// ============================================================================
#ifdef USE_THREADING

  // Get the number of hardware threads available
  unsigned int nthreads = std::thread::hardware_concurrency();

  // Fallback to 4 threads if hardware_concurrency returns 0
  if (nthreads == 0)
  {
    nthreads = 4;
  }

  std::cout << "Rendering with " << nthreads << " threads..." << std::endl;

  std::vector<std::thread> threads;
  std::vector<RenderSegment *> segments;

  // Calculate rows per thread
  int rowsPerThread = image.height / nthreads;
  int remainingRows = image.height % nthreads;

  // Divide the image into segments
  int currentRow = 0;
  for (unsigned int i = 0; i < nthreads; ++i)
  {
    RenderSegment *seg = new RenderSegment();
    seg->height = height;
    seg->image = &image;
    seg->scene = &scene;
    seg->intervalX = intervalX;
    seg->intervalY = intervalY;
    seg->reflections = Reflections;
    seg->rowMin = currentRow;

    // Distribute remaining rows to first threads
    int extraRow = (i < remainingRows) ? 1 : 0;
    seg->rowMax = currentRow + rowsPerThread + extraRow;

    currentRow = seg->rowMax;

    segments.push_back(seg);

    // Create and launch thread
    threads.push_back(std::thread(renderSegment, seg));
  }

  // Wait for all threads to complete
  for (auto &thread : threads)
  {
    thread.join();
  }

  // Clean up segments
  for (auto *seg : segments)
  {
    delete seg;
  }

  std::cout << "Rendering complete!" << std::endl;

#else

  // ============================================================================
  // Single-threaded rendering (original code)
  // ============================================================================
  std::cout << "Rendering with single thread..." << std::endl;

  RenderSegment *seg = new RenderSegment();
  seg->height = height;
  seg->image = &image;
  seg->scene = &scene;
  seg->intervalX = intervalX;
  seg->intervalY = intervalY;
  seg->reflections = Reflections;
  seg->rowMin = 0;
  seg->rowMax = image.height;
  renderSegment(seg);

  delete seg;

  std::cout << "Rendering complete!" << std::endl;

#endif
}

std::ostream &operator<<(std::ostream &_stream, Camera &cam)
{
  Vector3 pos = cam.getPosition();
  return _stream << "Camera(" << pos << ")";
}