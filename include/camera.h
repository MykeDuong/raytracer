#pragma once

#include <cmath>
#include <fstream>
#include <optional>

#include "color.h"
#include "hittable.h"
#include "material.h"
#include "util.h"
#include "vect3.h"

class Camera {
public:
  double aspect_ratio = 16.0 / 9.0;
  int image_width = 1920;
  int samples_per_pixel = 10;
  int max_ray_depth = 10;
  double vertical_fov = 90;
  Point3 look_from = {0, 0, 0};
  Point3 look_at = {0, 0, -1};
  Vect3 v_up = {0, 1, 0};

  double defocus_angle = 0;
  double focus_distance = 10.0;

  void render(const Hittable &world) {
    this->initialize();

    std::ofstream out_file{"image.ppm"};
    out_file << "P3\n"
             << this->image_width << " " << this->image_height << "\n255\n";
    for (int j = 0; j < this->image_height; j++) {
      std::clog << "[LOG] Scanlines remaining: " << (this->image_height - j)
                << std::endl;
      for (int i = 0; i < this->image_width; i++) {
        Color pixel_color{0, 0, 0};
        for (int sample = 0; sample < this->samples_per_pixel; sample++) {
          Ray r = this->get_ray(i, j);
          pixel_color += this->ray_color(r, this->max_ray_depth, world);
        }
        pixel_color *= this->pixel_sample_scales;
        pixel_color.write_color(out_file);
      }
    }

    std::clog << "[LOG] Done" << std::endl;
  };

private:
  Point3 center{0, 0, 0};
  double pixel_sample_scales;
  int image_height;
  Point3 pixel00_location;
  Vect3 pixel_delta_u;
  Vect3 pixel_delta_v;
  double viewport_height;
  Vect3 up, right, forward;
  Vect3 defocus_disk_horizontal_radius;
  Vect3 defocus_disk_vertical_radius;

  void initialize() {
    // Calculate height, assert it is at least 1
    this->image_height = (int)(this->image_width / this->aspect_ratio);
    this->image_height = (this->image_height < 1) ? 1 : this->image_height;

    this->center = this->look_from;

    const double theta = Utility::degrees_to_radians(this->vertical_fov);
    const double h = std::tan(theta / 2) * this->focus_distance;
    this->viewport_height = 2 * h;
    const double viewport_width =
        this->viewport_height *
        ((double)(this->image_width) / this->image_height);

    this->forward = unit_vector(this->look_from - this->look_at);
    this->right = unit_vector(cross(v_up, this->forward));
    this->up = unit_vector(cross(this->forward, this->right));

    // Vectors across the horizontal and down the vertical viewport edge
    const Vect3 viewport_u = viewport_width * this->right;
    const Vect3 viewport_v = this->viewport_height * -this->up;

    // Horizontal & vertical delta vectors from pixel to pixel
    this->pixel_delta_u = viewport_u / this->image_width;
    this->pixel_delta_v = viewport_v / this->image_height;

    // Location of upper-left pixel
    const Point3 viewport_upper_left = this->center -
                                       (this->focus_distance * this->forward) -
                                       viewport_u / 2 - viewport_v / 2;
    this->pixel00_location =
        viewport_upper_left + 0.5 * (this->pixel_delta_u + this->pixel_delta_v);

    const double defocus_radius =
        this->focus_distance *
        std::tan(Utility::degrees_to_radians(this->defocus_angle / 2));
    this->defocus_disk_horizontal_radius = this->right * defocus_radius;
    this->defocus_disk_vertical_radius = this->up * defocus_radius;

    // Pixel sample scale
    this->pixel_sample_scales = 1.0 / this->samples_per_pixel;
  };

  Ray get_ray(int i, int j) const {
    const Vect3 offset = this->sample_square();
    const Point3 pixel_sample = this->pixel00_location +
                                ((i + offset.x()) * this->pixel_delta_u) +
                                ((j + offset.y()) * this->pixel_delta_v);

    const Point3 ray_origin =
        (this->defocus_angle <= 0) ? this->center : this->defocus_disk_sample();
    const Vect3 ray_direction = pixel_sample - ray_origin;
    const double ray_time = Utility::random_double();
    return { ray_origin, ray_direction, ray_time };
  };

  Vect3 sample_square() const {
    return Vect3(
        Utility::random_double() - 0.5, 
        Utility::random_double() - 0.5,
        0);
  };

  Point3 defocus_disk_sample() const {
    const Point3 p = random_in_unit_disk();
    return this->center + (p[0] * this->defocus_disk_horizontal_radius) +
           (p[1] * this->defocus_disk_vertical_radius);
  };

  Color ray_color(const Ray &ray, int ray_depth, const Hittable &world) {
    if (ray_depth <= 0) {
      return {0, 0, 0};
    }
    std::optional<HitRecord> record =
        world.hit(ray, {0.001, Constant::infinity});
    if (record.has_value()) {
      std::optional<ScatterRecord> scatter_result =
          record->material->scatter(ray, record.value());
      if (scatter_result.has_value()) {
        const Ray scattered = scatter_result.value().scattered;
        const Color attenuation = scatter_result.value().attenuation;
        return attenuation * this->ray_color(scattered, ray_depth - 1, world);
      }
      return {0, 0, 0};
    }
    const Vect3 unit_direction = unit_vector(ray.direction());
    const double a = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - a) * Color(1.0, 1.0, 1.0) + a * Color(0.5, 0.7, 1.0);
  };
};
