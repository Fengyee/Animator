#include "particle.h"
#include "FL/gl.h"

Particle::Particle(ParticleType type, double mass, int lifetime,
	const Vec3f& sizes, const Vec3f& position, const Vec3f& velocity, const Vec3f& acceleration,
	Vec4f& wp)
{
	this->type = type;
	this->mass = mass;
	this->lifetime = lifetime;
	this->sizes = sizes;
	this->position = position;
	this->velocity = velocity;
	this->acceleration = acceleration;
	this->worldPoint = Vec4f(wp[0], wp[1], wp[2], wp[3]);
}

Particle::Particle(const Particle& partcle)
{
	this->type = partcle.type;
	this->mass = partcle.mass;
	this->lifetime = partcle.lifetime;
	this->sizes = partcle.sizes;
	this->position = partcle.position;
	this->velocity = partcle.velocity;
	this->acceleration = partcle.acceleration;
	this->worldPoint = Vec4f(partcle.worldPoint[0], partcle.worldPoint[1], partcle.worldPoint[2], partcle.worldPoint[3]);
}

Particle::~Particle()
{
}