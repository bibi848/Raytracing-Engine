#include "FunctionGroup.h"
#include <cmath>

bool SphereIntersect(float rayOrigin[3], float rayDirection[3], float SpherePosition[3], float radius, float& t0, float& t1)
{
	// Vector from ray origin to sphere center
	float oc[3] = { rayOrigin[0] - SpherePosition[0], rayOrigin[1] - SpherePosition[1], rayOrigin[2] - SpherePosition[2] };

	// Coefficients a,b,c
	float a = rayDirection[0] * rayDirection[0] + rayDirection[1] * rayDirection[1] + rayDirection[2] * rayDirection[2];
	float b = 2 * (oc[0] * rayDirection[0] + oc[1] * rayDirection[1] + oc[2] * rayDirection[2]);
	float c = oc[0] * oc[0] + oc[1] * oc[1] + oc[2] * oc[2] - radius * radius;

	float discriminant = b * b - 4 * a * c;

	if (discriminant < 0) {
		return false; // No intersection
	}

	// Calculate the two possible values of t (intersection points)
	discriminant = sqrt(discriminant);
	t0 = (-b - discriminant) / (2.0f * a);
	t1 = (-b + discriminant) / (2.0f * a);

	// Return true if there's an intersection
	return true;
}
