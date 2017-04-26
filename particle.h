// SAMPLE_SOLUTION
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "vec.h"
#include "FL/gl.h"

enum ParticleType
{
	BOX,
	BALL
};

class Particle
{
public:
	Particle(ParticleType type, double mass, int lifetime,
		const Vec3f& sizes, const Vec3f& position, const Vec3f& velocity, const Vec3f& acceleration,
		Vec4f& wp);
	Particle(const Particle& partcle);
	~Particle();


	ParticleType type;
	double mass;
	int lifetime;
	Vec3f sizes;
	Vec3f position;
	Vec3f velocity;
	Vec3f acceleration;
	// GLfloat* mat;
	Vec4f worldPoint;
};

#endif // _PARTICLE_H_