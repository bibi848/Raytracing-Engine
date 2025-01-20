#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"
#include "material.h"
#include <fstream>

class Camera {
public:
    double aspect_ratio = 1.0;
    int image_width = 100;
    int samples_per_pixel = 10;
    int max_depth = 10;

    double vfov = 90;
    Vec3 lookfrom = Vec3(0, 0, 0);
    Vec3 lookat = Vec3(0, 0, -1);
    Vec3 vup = Vec3(0, 1, 0);

	void render(const Hittable& world) {
        initialise();
        
        std::ofstream image_file("C:/Users/oscar/Documents/C++/Raytracing-Engine/Images/ppm/output_image11.ppm");

        image_file << "P3\n" << image_width << ' ' << image_height << "\n255\n";

        for (int j = 0; j < image_height; j++) {
            std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
            for (int i = 0; i < image_width; i++) {
                colour pixel_colour(0, 0, 0);
                for (int sample = 0; sample < samples_per_pixel; sample++) {
                    Ray r = get_ray(i, j);
                    pixel_colour += ray_colour(r, max_depth, world);
                }
                write_colour(image_file, pixel_samples_scale * pixel_colour);
            }
        }
        std::clog << "\rDone.                 \n";
        image_file.close();
		
	}
private:
    int    image_height;        // Rendered image height
    double pixel_samples_scale; // Colour scale factor for a sum of pixel samples
    Vec3   center;              // Camera center
    Vec3   pixel00_loc;         // Location of pixel 0, 0
    Vec3   pixel_delta_u;       // Offset to pixel to the right
    Vec3   pixel_delta_v;       // Offset to pixel below
    Vec3   u, v, w;

    void initialise() {
        image_height = int(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height;

        pixel_samples_scale = 1.0 / samples_per_pixel;

        center = lookfrom;

        // Determine viewport dimensions.
        auto focal_length = (lookfrom - lookat).length();
        auto theta = deg2rad(vfov);
        auto h = std::tan(theta / 2);
        auto viewport_height = 2 * h * focal_length;
        auto viewport_width = viewport_height * (double(image_width) / image_height);

        // Calculate the u,v,w vectors for the camera coordinate frame.
        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(vup, w));
        v = cross(w, u);

        // Calculate the vectors across the horizontal and down the vertical viewport edges.
        auto viewport_u = viewport_width * u;
        auto viewport_v = viewport_height * -v;

        // Calculate the horizontal and vertical delta vectors from pixel to pixel.
        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        // Calculate the location of the upper left pixel.
        auto viewport_upper_left = center - (focal_length * w) - viewport_u / 2 - viewport_v / 2;
        pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
    }

    Ray get_ray(int i, int j) const {

        auto offset = sample_square();
        auto pixel_sample = pixel00_loc + ((i + offset.x()) * pixel_delta_u) + ((j + offset.y()) * pixel_delta_v);
        auto ray_origin = center;
        auto ray_direction = pixel_sample - ray_origin;

        return Ray(ray_origin, ray_direction);
    }

    Vec3 sample_square() const {
        return Vec3(random_double() - 0.5, random_double() - 0.5, 0);
    }


    colour ray_colour(const Ray& r, int depth, const Hittable& world) {

        if (depth <= 0) {
            return colour(0, 0, 0);
        }

        hit_record rec;

        if (world.hit(r, interval(0.001, infinity), rec)) {
            Ray scattered;
            colour attenuation;
            if (rec.mat->scatter(r, rec, attenuation, scattered)) {
                return attenuation * ray_colour(scattered, depth - 1, world);
            }
            return colour(0, 0, 0);
        }

        Vec3 unit_direction = unit_vector(r.direction());
        auto a = 0.5 * (unit_direction.y() + 1.0);
        return (1.0 - a) * colour(1.0, 1.0, 1.0) + a * colour(0.5, 0.7, 1.0);
    }
};


#endif
