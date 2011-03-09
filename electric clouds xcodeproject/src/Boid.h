#ifndef BOID_H
#define BOID_H
#include "ofxVec3f.h"
#include "ofMain.h"
class Boid {
	public:
		Boid();
		void update();
		void draw();
		void bounce();

		// steering behaviors
		void addForce(ofxVec3f oForce);
		void subtractForce(ofxVec3f oForce);
		void seek(ofxVec3f oTarget); // go to a point
		void flee(ofxVec3f oTarget); // flee from this target
		void arrive(ofxVec3f oTarget); // arrive at this point.
		void pursue(Boid* pBoid);

		//void evade(Boid* pBoid);

		// getter|setters
		ofxVec3f getPosition();
		ofxVec3f getVelocity();

		void setPosition(ofxVec3f oPos);
		void setMaxVelocity(float fMax);
		float getMaxVelocity();
		void setVelocity(ofxVec3f oVel);

	private:
		ofImage img;
		float max_force;
		float max_vel;
		float mass;
		float arrive_dist;
		ofxVec3f force;
		ofxVec3f pos;
		ofxVec3f acc;
		ofxVec3f vel;

};

#endif
