#ifndef COLOUR_H
#define COLOUR_H

#include "interval.h"
#include "vec3.h"

using colour = Vec3;

inline double linear_to_gamma(double linear_component)
{
	if (linear_component > 0) {
		return std::sqrt(linear_component);
	}

	return 0;
}

void write_colour(std::ostream& out, const colour& pixel_colour) {
	auto r = pixel_colour.x();
	auto g = pixel_colour.y();
	auto b = pixel_colour.z();

	// Replace NaN components with zero
	if (r != r) r = 0.0;
	if (g != g) g = 0.0;
	if (b != b) b = 0.0;

	r = linear_to_gamma(r);
	g = linear_to_gamma(g);
	b = linear_to_gamma(b);

	static const interval intensity(0.000, 0.999);
	int rbyte = int(256 * intensity.clamp(r));
	int gbyte = int(256 * intensity.clamp(g));
	int bbyte = int(256 * intensity.clamp(b));
   
	out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}

#endif
