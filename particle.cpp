#include "particle.h"
#include "FL/gl.h"

Particle::Particle(ParticleType type, double mass, int lifetime,
	const Vec3f& sizes, const Vec3f& position, const Vec3f& velocity, const Vec3f& acceleration)
{
	this->type = type;
	this->mass = mass;
	this->lifetime = lifetime;
	this->sizes = sizes;
	this->position = position;
	this->velocity = velocity;
	this->acceleration = acceleration;
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
}

Particle::~Particle()
{
}