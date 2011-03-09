#include "Boids.h"

Boids::Boids(int iNumBoids) {
	num_boids = iNumBoids;
	for (int i = 0; i < num_boids; ++i) {
		boids.push_back(new Boid());
		boids.back()->setPosition(
			ofxVec3f(
				ofRandom(0,ofGetWidth())
				,ofRandom(0, ofGetHeight())
				,300
			)
		);
	}
}

void Boids::update() {
	for(int i = 0; i < boids.size(); ++i) {
		boids[i]->update();
	}
}

void Boids::bounce() {
	for(int i = 0; i < boids.size(); ++i) {
		boids[i]->bounce();
	}
}

void Boids::draw() {
	for(int i = 0; i < boids.size(); ++i) {
		boids[i]->draw();
	}
}

void Boids::seekParent(ofxVec3f oMainTarget) {
	boids[0]->seek(oMainTarget);
	for(int i = 1; i < boids.size(); ++i) {
		boids[i]->seek(boids[i-1]->getPosition());
	}
}

/**
 * This method implements flocking behavior a la Graig Reynolds.
 * Flocking has three main aspecs:
 *
 * - cohesion: make sure you stay a "group"
 * - separation: when the group becomes to crowdy separate
 * - alignment: fly into the same direction.
 *
 */
void Boids::flock() {
	float too_close_dist = 90.0f;
	float in_sight_dist = 150.0f;
	int insight_count = 0;
	ofxVec3f average_vel, average_pos;
	for (int i = 0; i < boids.size(); ++i) {
		average_vel.set(0,0,0);
		average_pos.set(0,0,0);
		insight_count = 0;

		//for(int j = i + 1; j < boids.size(); ++j) {
		for(int j = 0; j < boids.size(); ++j) {
			if (j == i) continue;
			float dist = (boids[i]->getPosition() - boids[j]->getPosition()).length();

			// separate
			if (dist <= in_sight_dist ) {
				average_vel += boids[j]->getVelocity();
				average_pos += boids[j]->getPosition();
				insight_count++;
				if (dist <= too_close_dist) {
					boids[i]->flee(boids[j]->getPosition());
					/*
					ofxVec3f heading = boids[i]->getVelocity().normalize();
					ofxVec3f diff = boids[j]->getPosition()-boids[i]->getPosition();
					float dot_prod = diff.dot(heading);
					if (dot_prod > 0) {
						boids[i]->flee(boids[j]->getPosition());
					}
					*/
				}
			}
		}
		if (insight_count > 0) {
			average_pos /= insight_count;
			average_vel /= insight_count;
			boids[i]->seek(average_pos);
			boids[i]->addForce(average_vel - boids[i]->getVelocity());
		}
	}
}

vector<Boid*> Boids::getBoids() {
	return boids;
}

void Boids::setMaxVelocity(float fMax) {
	for (int i = 0; i < boids.size(); ++i) {
		boids[i]->setMaxVelocity(fMax);
	}
}

void Boids::arrive(ofxVec3f oTarget) {
	for (int i = 0; i < boids.size(); ++i) {
		boids[i]->arrive(oTarget);
	}
}
void Boids::flee(ofxVec3f oTarget) {
	for (int i = 0; i < boids.size(); ++i) {
		boids[i]->flee(oTarget);
	}
}

void Boids::explode() {
	float max_vel = boids[0]->getMaxVelocity();
	for (int i = 0; i < boids.size(); ++i) {
		boids[i]->setVelocity(ofxVec3f(
			ofRandom(-max_vel,max_vel)
			,ofRandom(-max_vel,max_vel)
			,ofRandom(0,-max_vel)
		));
	}
}


