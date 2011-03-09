#include "Boid.h"

Boid::Boid() {
	pos.set(300.0f, 300.0f, 0.0f);
	acc.set(0.4,0.4, 0.0f);
	vel.set(ofRandom(0.1f, 0.4f), ofRandom(0.1f, 0.4f), 0.3f);
	vel = 0.0f;

    //max_vel.set(20.0f, 20.0f, 0.0f);
	max_vel = 30.0f;
	mass = 1.0f;

	// steering forces.
	max_force = 2.0f;
	force.set(0.0f, 0.0f, 0.0f);

	// arrive threshold
	arrive_dist = 140.0f;

	img.loadImage("alpha.png");

}

void Boid::update() {
	force.limit(max_force);
	vel += force;

	vel.limit(max_vel);
	pos += vel;
	//force = 0;
}


void Boid::draw() {
	ofEnableSmoothing();
	ofEnableAlphaBlending();
	//ofSetColor(255, 0, 0, 100);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glPushMatrix();
	glTranslatef(pos.x, pos.y, pos.z);
		img.draw(0, 0, 200, 200);
	glPopMatrix();
	
	//ofSetLineWidth(3);

	float arrow_len = 30;
	ofxVec3f arrow_end = pos +(vel.normalized() * arrow_len);
	//ofFill();
	//ofEnableSmoothing();
	ofLine(pos.x, pos.y, arrow_end.x, arrow_end.y);
	ofCircle(arrow_end.x, arrow_end.y, 3);

	ofCircle(pos.x, pos.y, 10);

	//ofSetLineWidth(20);
	//ofCircle(pos.x, pos.y, 10);

	//ofSetColor(0x00);
	//ofSetLineWidth(3);
	//ofCircle(pos.x, pos.y, 12);


	glDisable(GL_BLEND);
	ofDisableAlphaBlending();
	ofDisableSmoothing();

}

void Boid::bounce() {
	if (pos.x > ofGetWidth()) {
		pos.x = ofGetWidth();
		vel.x *= -1;
	}
	else if (pos.x < 0) {
		pos.x = 0;
		vel.x *= -1;
	}
	if (pos.y > ofGetHeight()) {
		pos.y = ofGetHeight();
		vel.y *= -1;
	}
	else if (pos.y < 0) {
		pos.y = 0;
		vel.y *= -1;
	}
	if (pos.z < -ofGetHeight()) {
		pos.z = -ofGetHeight();
		vel.z *= -1;
	}
	else if (pos.z > 200) {
		pos.z = 200;
		vel.z *= -1;
	}
}

// steering behaviors

void Boid::seek(ofxVec3f oTarget) {
	// This is how it works.
	// 1. We calculate the change in position which would bring
	// use directly to the target. This is "desired_vel". Though we don't
	// want to jump directly to this target, but slowly move to it.
	ofxVec3f desired_vel = oTarget - pos;

	// 2. Therefore we want to move to this position at the maximum
	// allowed speed. We do this by normalizing the vector and then
	// multiplying it with the maximum allowed speed. This is exactly
	// what ofxVec3f.scale() does.
	desired_vel.scale(max_vel);

	// 3. Now we have the maximum desired velocity at the maximum speed.
	// Though we need to adjust this speed as we want to go into that direction
	// at the best we are allowed, which is the remaining velocity.
	desired_vel -= vel;

	addForce(desired_vel);

	//cout << "Desired vel: " << desired_vel.x << ", " << desired_vel.y << endl;
}

/**
 * Fleeing is the exactly opposite of seek. We calculate the exactly same force
 * as we would for seek(), though instead of adding a force into the target direction
 * we subtract this force. See seek() for a description of how this calculation works.
 */
void Boid::flee(ofxVec3f oTarget) {
	ofxVec3f desired_vel = oTarget - pos;
	desired_vel.scale(max_vel);
	desired_vel -= vel;
	subtractForce(desired_vel);
}


/**
 * Arrive makes the boid slow down when it moves towards its targets. The closer
 * it gets the slower it will move. The boid only slows down when it is withing a
 * certain distance of the target.
 */
void Boid::arrive(ofxVec3f oTarget) {
	ofxVec3f desired_vel = oTarget - pos;
	float dist = desired_vel.length();
	desired_vel.normalize();

	//ofSetColor(0xFF0000);
	//ofCircle(oTarget.x, oTarget.y, 4);

	// When we are within the arrive distance, we switch to slow-down mode.
	if (dist <= arrive_dist) {
		desired_vel *= ((max_vel * dist / arrive_dist));
		ofSetColor(0x00);
	}
	// else we continue our journey they same we did when we were seeking
	else {
		desired_vel *= max_vel;
	}
	// We add a little margin here;
	// w/o the boid will oversh0ot its target.
	if (dist > 10) {
		desired_vel -= vel;
		addForce(desired_vel);
	}
	// When were are within the margin, stop moving.
	else{
		force = 0;
		vel = 0;
	}
}

/**
 * Pursueing is somewhat the same as seeking, though we seek for a
 * moving target. But we also add something extra. We predict the
 * future position of the target and use that as a target.
 *
 * We take the velocity and position of the Boid we pursue and add
 * a little extra to this position using the velocity and a "look
 * ahead time". This look ahead time indicates how far we look in the
 * future ;-). A good solution to find out what the look ahead time
 * should be is using the distance between 'this' and the Boid we
 * pursue.
 *
 */
void Boid::pursue(Boid* pBoid) {
	float dist = (pBoid->getPosition() - getPosition()).length();
	float ahead_time = dist / max_vel;
	ofxVec3f predicted_target = pBoid->getPosition() + pBoid->getVelocity()*ahead_time;
	seek(predicted_target);
}



void Boid::addForce(ofxVec3f oForce) {
	force += oForce;
}
void Boid::subtractForce(ofxVec3f oForce) {
	force -= oForce;
}

ofxVec3f Boid::getPosition() {
	return pos;
}

ofxVec3f Boid::getVelocity() {
	return vel;
}

void Boid::setPosition(ofxVec3f oPos) {
	pos = oPos;
}

void Boid::setMaxVelocity(float fMax) {
	max_vel = fMax;
}
float Boid::getMaxVelocity() {
	return max_vel;
}

void Boid::setVelocity(ofxVec3f oVel) {
	vel = oVel;
}

