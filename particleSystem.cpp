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
#include <math.h>

extern Mat4f WorldMatrix;
extern Mat4f getModelViewMatrix();

/***************
 * Constructors
 ***************/

ParticleSystem::ParticleSystem(ParticleType type, int life, const Vec3f& size, float bakefps, int speed)
	: simulate(false), dirty(false), particleType(type), particleLife(life), particleNumber(speed)
{
	particleSize = size;
	bake_fps = bakefps;
	isMirror = 0;
	isFire = 0;
	if (bakefps <= 0) bake_fps = 30.0;
}

ParticleSystem::ParticleSystem(ParticleType type, int life, const Vec3f& size, float bakefps, int speed, int iF)
	: simulate(false), dirty(false), particleType(type), particleLife(life), particleNumber(speed), isFire(iF)
{
	particleSize = size;
	bake_fps = bakefps;
	isMirror = 0;
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
	if (isFire == 0)
	{
		list<Particle>* particles = bakedParticles[index];
		Vec3f g(1, 1, 1);
		if (isMirror == 1)
		{
			g = Vec3f(0, 9.8, 0);
		}
		else {
			g = Vec3f(0, -9.8, 0);
		}
		float dragCoef = -0.1;
		for (auto it = particles->begin(); it != particles->end(); it++)
		{
			// Gravity
			Vec3f drag = prod(it->velocity, it->velocity) * dragCoef;
			// update acceleration 
			it->acceleration = drag / it->mass + g;
		}
	}
}


/** Render particles */
void ParticleSystem::drawParticles(float t, int isM, GLint texName)
{
	if (t < bake_start_time || bake_start_time <= bake_end_time && t > bake_end_time) return;
	isMirror = isM;
	bakeParticles(t);
	
	/*
	Mat4f mvm = getModelViewMatrix();
	Vec3f forward(mvm[2][0], mvm[2][1], mvm[2][2]);
	Vec3f right(mvm[0][0], mvm[0][1], mvm[0][2]);
	Vec3f up(mvm[1][0], mvm[1][1], mvm[1][2]);
	*/
	glPushMatrix();
		GLfloat modelview[16];
		glGetFloatv(GL_MODELVIEW_MATRIX, modelview);
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++) {
				if (i == j)
					modelview[i * 4 + j] = 1.0;
				else
					modelview[i * 4 + j] = 0.0;
			}
		}
		/*
		glLoadMatrixf(modelview);
		glTranslatef(1, 1, 1);
		setDiffuseColor(0.2, 0.2, 0.7);
		drawBox(0.5, 0.5, 0.5);
		*/
	glPopMatrix();
	

	int frameNum = (t - bake_start_time) * bake_fps;
	// printf("frame number is %d\n", frameNum);
	list<Particle>* particles = bakedParticles[frameNum];
	if (particles == NULL) return;

	// render particles
	for (auto it = particles->begin(); it != particles->end(); it++)
	{
		glPushMatrix();
		setDiffuseColor(1.0f, 1.0f, 0.5f);
		if (isFire == 1)
		{
			it->sizes[0] = 0.1 * it->lifetime / 20;
			it->sizes[1] = 0.1 * it->lifetime / 20;
			it->sizes[2] = 0.1 * it->lifetime / 20;
			glLoadMatrixf(modelview);
			setDiffuseColor(1.0f, 0.0f, 0.0f);
		}
		glTranslatef(it->position[0], it->position[1], it->position[2]);
		
		glScalef(it->sizes[0], it->sizes[1], it->sizes[2]);
		
		
		switch (it->type)
		{
		case BOX:
			if (isFire == 0)
				drawBox(1, 1, 1);
			else
			{
				glEnable(GL_TEXTURE_2D);
				glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
				glBindTexture(GL_TEXTURE_2D, texName);
				drawTextureBox(1, 1, 1);
				glFlush();
				glDisable(GL_TEXTURE_2D);
			}
			break;
		case BALL:
			drawSphere(0.5);
			break;
		default:
			break;
		}
		glPopMatrix();
	}
}

/** Adds the current configuration of particles to
  * your data structure for storing baked particles **/
void ParticleSystem::bakeParticles(float t) 
{
	int frameNum = (t - bake_start_time) * bake_fps;
	// printf("frameNum is %d, %f, %f, %f\n", frameNum, t, bake_start_time, bake_fps);
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

	if (lastBaked < 0)
	{
		list<Particle>* newParticles = new list<Particle>;
		for (int i = 0; i < particleNumber; i++)
		{
			for (int j = 0; j < 10; ++j)
			{
				newParticles->push_back(spawnParticle(Vec4f(0.1 * j, -0.9, 0, 1)));
			}
		}
		bakedParticles[0] = newParticles;
		lastBaked = 0;
		if (frameNum == 0) return;
	}

	list<Particle>* particles = bakedParticles[lastBaked];
	list<Particle>* tmp = new list<Particle>;
	// printf("compute forces and update particles at: %d (%d)\n", lastBaked, frameNum);
	computeForcesAndUpdateParticles(lastBaked);
	for (auto it = particles->begin(); it != particles->end(); it++)
	{
		if (it->lifetime < 0) continue;
		Particle next(*it);
		next.lifetime -= (frameNum - lastBaked);
		double deltaT = (frameNum - lastBaked) / bake_fps;
		// printf("deltaT is %f\n", deltaT);
		next.position += deltaT * next.velocity / 2;
		next.velocity += deltaT * next.acceleration;
		next.position += deltaT * next.velocity / 2;
		tmp->push_back(next);
	}
	for (int i = 0; i < particleNumber; i++)
	{
		for (int j = 0; j < 10; ++j)
		{
			tmp->push_back(spawnParticle(Vec4f(0.1 * j, -0.9, 0, 1)));
		}
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

Particle ParticleSystem::spawnParticle(Vec4f position)
{
	unsigned seed = chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine* re = new default_random_engine(seed);
	ParticleType t = BOX;
	Vec3f size(particleSize[0], particleSize[1], particleSize[2]);
	uniform_real_distribution<double> urd(-0.5, 0.5);
	// Vec4f pos(0.5, 0.9, 0, 1);
	if (isFire == 0)
	{
		position = WorldMatrix * position;
		// Vec3f v2(urd(*re), urd(*re), urd(*re) + 1);
		Vec3f v2(urd(*re) * 5, urd(*re), urd(*re));
		return Particle(t, 1, particleLife, particleSize, Vec3f(position[0], position[1], position[2]), v2, Vec3f(0, 0, 0));
	}
	else
	{
		// position = WorldMatrix * Vec4f(2, -1.2, 2, 1);
		uniform_real_distribution<double> urd_angle(0, 6.283);
		double speed = (urd(*re) + 0.5);
		Vec3f v2(speed * cos(urd_angle(*re)), urd(*re) + 0.5, speed * sin(urd_angle(*re)));
		int lifetime_fire = (int)(particleLife * speed);
		Vec3f g(1, 1, 1);
		if (isMirror == 1)
		{
			g = Vec3f(0, 9.8, 0);
		}
		else {
			g = Vec3f(0, -9.8, 0);
		}
		return Particle(t, 0.01, lifetime_fire, particleSize * 2, Vec3f(3, -1.5, 4) + Vec3f(urd(*re), (urd(*re)-0.5)/2, urd(*re)), v2, g);
	}
}