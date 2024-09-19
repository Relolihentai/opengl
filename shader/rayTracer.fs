#version 450 core

#define PI 3.1415926535897932385

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
	int materialType;
	int material;
};

Sphere SphereConstructor(vec3 center, float radius)
{
    Sphere sphere;
    sphere.center = center;
    sphere.radius = radius;
    return sphere;
}

#define MAT_LAMBERTIAN	0
#define MAT_METALLIC	1
#define MAT_DIELECTRIC	2
#define MAT_PBR			3


struct Lambertian
{
	vec3 albedo;
};
struct Metallic
{
	vec3 albedo;
};
struct Dielectric
{
	vec3 albedo;
	//折射率
    float ior;
};
uniform Lambertian lambertMaterials[10];
uniform Metallic matallicMaterials[10];

struct HitRecord
{
    vec3 position;
    vec3 normal;
    float t;
	int materialType;
	int material;
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
			hitRecord.materialType = sphere.materialType;
			hitRecord.material = sphere.material;
			return true;
		}

		temp = (-b + sqrt(discriminant)) / (2.0 * a);
		if(temp < t_max && temp> t_min)
		{
			hitRecord.t = temp;
			hitRecord.position = RayGetPointAt(ray, hitRecord.t);
			hitRecord.normal = (hitRecord.position - sphere.center) / sphere.radius;
			hitRecord.materialType = sphere.materialType;
			hitRecord.material = sphere.material;
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
vec3 random_unit_vector(vec2 seed)
{
	float a = rand(seed * 10) * 2 * PI;
    float z = rand(seed * seed * 10) * 2 - 1;
    float r = sqrt(1 - z * z);
    return vec3(r * cos(a), r * sin(a), z);
}

bool LambertianScatter(in Lambertian lambertian, in Ray incident, in HitRecord hitRecord, out Ray scatter, out vec3 attenuation)
{
	attenuation = lambertian.albedo;
	vec3 scatter_direction = hitRecord.normal + random_unit_vector(vec2(hitRecord.position.x, hitRecord.position.y));
	scatter = RayConstructor(hitRecord.position, scatter_direction);
	return true;
}

vec3 reflect(in vec3 incident, in vec3 normal)
{
	return incident - 2 * dot(normal, incident) * normal;
}

bool MetallicScatter(in Metallic metallic, in Ray incident, in HitRecord hitRecord, out Ray scattered, out vec3 attenuation)
{
	attenuation = metallic.albedo;

	scattered.origin = hitRecord.position;
	scattered.direction = reflect(incident.direction, hitRecord.normal);

	return dot(scattered.direction, hitRecord.normal) > 0.0;
}

bool MaterialScatter(in Ray incident, in HitRecord hitRecord, out Ray scatter, out vec3 attenuation)
{
	if(hitRecord.materialType == MAT_LAMBERTIAN)
		return LambertianScatter(lambertMaterials[hitRecord.material], incident, hitRecord, scatter, attenuation);
	else if (hitRecord.materialType == MAT_METALLIC)
		return MetallicScatter(matallicMaterials[hitRecord.material], incident, hitRecord, scatter, attenuation);
}

vec3 GetEnvironmentColor(World world, Ray ray)
{
	vec3 unit_direction = normalize(ray.direction);
	float t = 0.5 * (unit_direction.y + 1.0);
	return vec3(1.0, 1.0, 1.0) * (1.0 - t) + vec3(0.5, 0.7, 1.0) * t;
}

vec3 WorldTrace(Ray ray, int depth)
{
	HitRecord hitRecord;

	vec3 current_attenuation = vec3(1.0, 1.0, 1.0);
	Ray current_ray = ray;
	
	while(depth>0)
	{
		depth--;
		if(WorldHit(current_ray, 0.001, 100000, hitRecord))
		{
			vec3 attenuation;
			Ray scatter_ray;

			if(!MaterialScatter(current_ray, hitRecord, scatter_ray, attenuation))
				break;

			current_attenuation *= attenuation;
			current_ray = scatter_ray;
		}
		else
		{
			return current_attenuation * GetEnvironmentColor(world, current_ray);
		}
	}

	return vec3(0.0, 0.0, 0.0);
}

in vec2 screenCoord;
out vec4 FragColor;
uniform Camera camera;
uniform vec2 screenSize;

void main()
{
	vec3 col = vec3(0.0, 0.0, 0.0);
	int ns = 100;
	int depth = 50;
	for(int i = 0; i < ns; i++)
	{
		Ray ray = CameraGetRay(camera, screenCoord + (rand2(screenCoord * (i + 1)) / screenSize));
		col += WorldTrace(ray, depth);
	}
	col /= ns;

	FragColor.xyz = col;
	FragColor.w = 1.0;
}