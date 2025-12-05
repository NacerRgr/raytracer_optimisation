#include <iostream>
#include <chrono>
#include "SceneLoader.hpp"

int main(int argc, char *argv[])
{
  std::cout << std::endl;
  std::cout << "*********************************" << std::endl;
  std::cout << "*** Kevin's Awesome Raytracer ***" << std::endl;
  std::cout << "*********************************" << std::endl;
  std::cout << std::endl;

  if (argc < 2)
  {
    std::cerr << "[ERROR] Please a path your scene file (.json)" << std::endl;
    std::cout << std::endl;
    exit(0);
  }

  std::string path = argv[1];
  auto [scene, camera, image] = SceneLoader::Load(path);

  std::string outpath = "image.png";
  if (argc > 2)
  {
    outpath = argv[2];
  }

  //  enable/disable BSP Tree based on compile-time flag
#ifdef USE_BSP
  scene->enableBSP(true); // BSP tree enabled
#else
  scene->enableBSP(false); // BSP tree disabled
#endif

  std::cout << "Rendering " << image->width << "x" << image->height << " pixels..." << std::endl;

  auto begin = std::chrono::high_resolution_clock::now();
  camera->render(*image, *scene);
  auto end = std::chrono::high_resolution_clock::now();
  auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);

  std::cout << "Done." << std::endl;
  std::printf("Total time: %.3f seconds.\n", elapsed.count() * 1e-9);

  std::cout << "Writing file: " << outpath << std::endl;
  image->writeFile(outpath);

  delete scene;
  delete camera;
  delete image;
}
