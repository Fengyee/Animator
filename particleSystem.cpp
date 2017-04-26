#pragma warning(disable : 4786)

#include "particleSystem.h"
#include "particle.h"
#include "modelerdraw.h"
#include <FL/gl.h>
#include <stdio.h>
#include <stdlib.h>
#include <list>
#include <random>
#include <chrono>
#include "mat.h"
#include "modelerapp.h"
#include "modelerui.h"

extern Mat4f getModelViewMatrix();
extern Vec4f WorldPoint;

/***************
 * Constructors
 ***************/

ParticleSystem::ParticleSystem(ParticleType type, int life, const Vec3f& size, float bakefps, int speed)
	: simulate(false), dirty(false), particleType(type), particleLife(life), particleNumber(speed)
{
	particleSize = size;
	bake_fps = bakefps;
	if (bakefps <= 0) bake_fps = 30.0;
}



/*************
 * Destructor
 *************/

ParticleSystem::~ParticleSystem() 
{
	clearBaked();

}


/******************
 * Simulation fxns
 ******************/

/** Start the simulation */
void ParticleSystem::startSimulation(float t)
{
    
	// TODO
	if (t < bake_start_time)
	{
		clearBaked();
	}

	bake_start_time = t;

	// These values are used by the UI ...
	// -ve bake_end_time indicates that simulation
	// is still progressing, and allows the
	// indicator window above the time slider
	// to correctly show the "baked" region
	// in grey.
	bake_end_time = -1;
	simulate = true;
	dirty = true;

}

/** Stop the simulation */
void ParticleSystem::stopSimulation(float t)
{
    
	// TODO
	bake_end_time = t;
	// These values are used by the UI
	simulate = false;
	dirty = true;

}

/** Reset the simulation */
void ParticleSystem::resetSimulation(float t)
{
    
	// TODO
	bake_start_time = t;
	bake_end_time = t;
	// These values are used by the UI
	simulate = false;
	dirty = true;

}

/** Compute forces and update particles **/
void ParticleSystem::computeForcesAndUpdateParticles(int index)
{
	list<Particle>* particles = bakedParticles[index];
	for (auto it = particles->begin(); it != particles->end(); it++)
	{
		// Gravity
		Vec3f gravity(0, -9.8 * it->mass, 0);
		// air drag force, k = 0.1
		Vec3f drag = prod(it->velocity, it->velocity) * (-0.1);
		// update acceleration 
		it->acceleration = (gravity + drag) / it->mass;
	}
}


/** Render particles */
void ParticleSystem::drawParticles(float t)
{
	if (t < bake_start_time || bake_start_time <= bake_end_time && t > bake_end_time) return;
	bakeParticles(t);

	list<Particle>* particles = bakedParticles[((int)(t - bake_start_time)* bake_fps)];
	if (particles == NULL) return;

	// render particles
	glPushMatrix();
	for (auto it = particles->begin(); it != particles->end(); it++)
	{
		glPushMatrix();
		glTranslatef(it->worldPoint[0], it->worldPoint[1], it->worldPoint[2]);
		glTranslatef(it->position[0], it->position[1], it->position[2]);
		glScalef(it->sizes[0], it->sizes[1], it->sizes[2]);
		switch (it->type)
		{
		case BOX:
			drawBox(1, 1, 1);
			break;
		case BALL:
			drawSphere(0.5);
		default:
			break;
		}
		glPopMatrix();
	}
	glPopMatrix();
}

/** Adds the current configuration of particles to
  * your data structure for storing baked particles **/
void ParticleSystem::bakeParticles(float t) 
{
	int frameNum = (t - bake_start_time) * bake_fps;
	printf("frameNum is %d, %f, %f, %f\n", frameNum, t, bake_start_time, bake_fps);
	if (frameNum < bakedParticles.size() &&
		bakedParticles[frameNum] != NULL)
	{
		return;
	}

	while (bakedParticles.size() <= frameNum)
	{
		bakedParticles.push_back(NULL);
		// printf("size is %d and frameNum is %d\n", bakedParticles.size(), frameNum);	
	}

	int lastBaked = frameNum - 1;
	while (lastBaked >= 0 && bakedParticles[lastBaked] == NULL)
	{
		lastBaked -= 1;
	}

	unsigned seed = chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine* re = new default_random_engine(seed);

	if (lastBaked < 0)
	{
		list<Particle>* newParticles = new list<Particle>;
		for (int i = 0; i < particleNumber; i++)
		{
			ParticleType t = BOX;
			Vec3f size(particleSize[0], particleSize[1], particleSize[2]);
			uniform_real_distribution<double> urd(-0.5, 0.5);
			Vec3f v(urd(*re), urd(*re) + 0.5, urd(*re) + 3);
			newParticles->push_back(Particle(t, 1, particleLife, particleSize, Vec3f(0, 0, 0), v, Vec3f(0, 0, 0), WorldPoint));
		}
		bakedParticles[0] = newParticles;
		lastBaked = 0;
		if (frameNum == 0) return;
	}

	list<Particle>* particles = bakedParticles[lastBaked];
	list<Particle>* tmp = new list<Particle>;
	computeForcesAndUpdateParticles(lastBaked);
	for (auto it = particles->begin(); it != particles->end(); it++)
	{
		if (it->lifetime < 0) continue;
		Particle next(*it);
		next.lifetime -= (frameNum - lastBaked);
		double deltaT = (frameNum - lastBaked) / bake_fps;
		next.position += deltaT * next.velocity / 2;
		next.velocity += deltaT * next.acceleration;
		next.position += deltaT * next.velocity / 2;
		tmp->push_back(next);
	}
	for (int i = 0; i < particleNumber * (frameNum - lastBaked); i++)
	{
		ParticleType t = BOX;
		Vec3f size(particleSize[0], particleSize[1], particleSize[2]);
		uniform_real_distribution<double> urd(-0.5, 0.5);
		Vec3f v(urd(*re), urd(*re) + 0.5, urd(*re) + 3);
		tmp->push_back(Particle(t, 1, particleLife, particleSize, Vec3f(0, 0, 0), v, Vec3f(0, 0, 0), WorldPoint));
	}

	bakedParticles[frameNum] = tmp;
}

/** Clears out your data structure of baked particles */
void ParticleSystem::clearBaked()
{
	for (auto it = bakedParticles.begin(); it != bakedParticles.end(); it++)
	{
		if (*it != NULL)
		{
			// Clear the baked particle list
			(*it)->clear();
			delete *it;
		}
	}
	bakedParticles.clear();
	this->resetSimulation(0.0);
}
