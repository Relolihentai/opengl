#version 450 core

struct Ray {
    vec3 origin;
    vec3 direction;
}; 

Ray RayConstructor(vec3 origin, vec3 direction)
{
	Ray ray;
	ray.origin = origin;
	ray.direction = direction;

	return ray;
}

vec3 RayGetPointAt(Ray ray, float t)
{
	return ray.origin + t * ray.direction;
}

struct Camera
{
    vec3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
    vec3 origin;
};

Ray CameraGetRay(Camera camera, vec2 offset)
{
	Ray ray = RayConstructor(camera.origin, 
		camera.lower_left_corner + 
		offset.x * camera.horizontal + 
		offset.y * camera.vertical);

	return ray;
}

struct Sphere 
{
    vec3 center;
    float radius;
};

Sphere SphereConstructor(vec3 center, float radius)
{
    Sphere sphere;
    sphere.center = center;
    sphere.radius = radius;
    return sphere;
}

struct HitRecord
{
    vec3 position;
    vec3 normal;
    float t;
};

bool SphereHit(Sphere sphere, Ray ray, float t_min, float t_max, inout HitRecord hitRecord)
{
	vec3 oc = ray.origin - sphere.center;
	
	float a = dot(ray.direction, ray.direction);
	float b = 2.0 * dot(oc, ray.direction);
	float c = dot(oc, oc) - sphere.radius * sphere.radius;

	float discriminant = b * b - 4 * a * c;
	if(discriminant > 0)
	{
		float temp = (-b - sqrt(discriminant)) / (2.0 * a);
		if(temp < t_max && temp> t_min)
		{
			hitRecord.t = temp;
			hitRecord.position = RayGetPointAt(ray, hitRecord.t);
			hitRecord.normal = (hitRecord.position - sphere.center) / sphere.radius;
			
			return true;
		}

		temp = (-b + sqrt(discriminant)) / (2.0 * a);
		if(temp < t_max && temp> t_min)
		{
			hitRecord.t = temp;
			hitRecord.position = RayGetPointAt(ray, hitRecord.t);
			hitRecord.normal = (hitRecord.position - sphere.center) / sphere.radius;
			
			return true;
		}
	}
	
	return false;
}

struct World
{
	int objectCount;
	Sphere objects[10];
};

uniform World world;

bool WorldHit(Ray ray, float t_min, float t_max, inout HitRecord rec)
{
	HitRecord tempRec;
	float cloestSoFar = t_max;
	bool hitSomething = false;

	for (int i = 0; i < world.objectCount; i++)
	{
		if(SphereHit(world.objects[i], ray, t_min, cloestSoFar, tempRec))
		{
			rec = tempRec;
			cloestSoFar = tempRec.t;

			hitSomething = true;
		}
	}

	return hitSomething;
}

vec3 WorldTrace(Ray ray)
{
    HitRecord record;
	if (WorldHit(ray, 0.001, 100000, record))
    {
        vec3 N = record.normal;
        return 0.5 * vec3(N.x + 1, N.y + 1, N.z + 1);
    }
    vec3 unit_direction = normalize(ray.direction);
    float t = 0.5 * (unit_direction.y + 1.0);
    return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
}

in vec2 screenCoord;
out vec4 FragColor;
uniform Camera camera;
uniform vec2 screenSize;

float rand(vec2 seed)
{
	return fract(sin(dot(seed.xy, vec2(12.9898,78.233))) * 43758.5453123);
}
vec2 rand2(vec2 seed)
{
	return vec2(
		rand(seed) * 2 - 1,
		rand(seed * seed) * 2 - 1
	);
}

void main()
{
	vec3 col = vec3(0.0, 0.0, 0.0);
	int ns = 100;
	for(int i = 0; i < ns; i++)
	{
		Ray ray = CameraGetRay(camera, screenCoord + (rand2(screenCoord * (i + 1)) / screenSize));
		col += WorldTrace(ray);
	}
	col /= ns;

	FragColor.xyz = col;
	FragColor.w = 1.0;
}