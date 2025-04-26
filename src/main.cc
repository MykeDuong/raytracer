#include <cmath>
#include <iostream>
#include <memory>

#include "camera.h"
#include "hittable_list.h"
#include "sphere.h"
#include "util.h"
#include "version.h"

int main() {
  std::clog << "# Raytracer Version " 
            << RAYTRACER_VERSION_MAJOR << "." << RAYTRACER_VERSION_MINOR 
            << std::endl;

  // World
  HittableList world;

  // Material
  const auto material_ground = std::make_shared<Lambertian>(Color(0.5, 0.5, 0.5));
  world.add(std::make_shared<Sphere>(Point3(0.0, -1000.0, -0.0), 1000.0, material_ground));

  for (int a = -11; a < 11; a++) {
    for (int b = -11; b < 11; b++) {
      const double choose_material = Utility::random_double();
      const Point3 center { a + 0.9 * Utility::random_double(), 0.2, b + 0.9 * Utility::random_double() };
      
      if ((center - Point3 {4, 0.2, 0}).length() > 0.9) {
        std::shared_ptr<Material> sphere_material;
        
        if (choose_material < 0.8) {
          // Lambertian
          const Color albedo = Color::random() * Color::random();
          sphere_material = std::make_shared<Lambertian>(albedo);
        } else if (choose_material < 0.95) {
          // Metal
          const Color albedo = Color::random(0.5, 1);
          const double fuzz = Utility::random_double(0, 0.5);
          sphere_material = std::make_shared<Metal>(albedo, fuzz);
        } else {
          sphere_material = std::make_shared<Dielectric>(1.5);
        }
        world.add(std::make_shared<Sphere>(center, 0.2, sphere_material));
      }
    }
  }

  auto material1 = std::make_shared<Dielectric>(1.5);
  world.add(std::make_shared<Sphere>(Point3(0, 1, 0), 1.0, material1));

  auto material2 = std::make_shared<Lambertian>(Color{0.4, 0.2, 0.1});
  world.add(std::make_shared<Sphere>(Point3(-4, 1, 0), 1.0, material2));

  auto material3 = std::make_shared<Metal>(Color{0.7, 0.6, 0.5}, 0.0);
  world.add(std::make_shared<Sphere>(Point3(4, 1, 0), 1.0, material3));

  // Camera
  Camera camera;
  camera.samples_per_pixel = 500;
  camera.max_ray_depth = 50;
  camera.look_from    = { 13, 2, 3 };
  camera.look_at      = {  0, 0, 0 };
  camera.v_up         = {  0, 1, 0 };
  camera.vertical_fov = 20;

  camera.defocus_angle = 0.6;
  camera.focus_distance = 10.0;

  // Render
  camera.render(world);

  return 0;
}
