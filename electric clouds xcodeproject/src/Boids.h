#ifndef BOIDS_H
#define BOIDS_H

#include <vector>
#include "Boid.h"
#include "ofxVec3f.h"


/**
 * Aggregator for our boids.
 */
class Boids {
	public:
		Boids(int iNumBoids);
		void update();
		void draw();

		void bounce();
		void seekParent(ofxVec3f oMainTarget);
		void flock();
		vector<Boid*> getBoids();
		void setMaxVelocity(float fMaxVel);
		void arrive(ofxVec3f oTarget);
		void flee(ofxVec3f oTarget);
		void explode(); // apply random velocity (impuls);

	private:
		int num_boids;
		vector<Boid*> boids;

};

#endif
