
#pragma once
#include "functions.h"

sf::Vector2f conv(sf::Vector2f v) {
	return sf::Vector2f(v.x, 800 - v.y);
}

sf::Vector2f conv(float x, float y) {
	return sf::Vector2f(x, 800 - y);
}

float pythag(sf::Vector2f v) {

	return sqrt(pow(v.x, 2) + pow(v.y, 2));
}

sf::Vector2f calcCorner(sf::Vector2f old, sf::Vector2f c, float theta, int type) {

	float z = pythag(c - old);
	theta += 45.0f + 90.0f * (type - 1);

	return sf::Vector2f(c.x + z * sin(theta * (3.1415926f / 180.0f)), c.y + z * cos(theta * (3.1415926f / 180.0f)));
}

void vCout(sf::Vector2f v, std::string t) {
	if(t.size() > 0)
		std::cout << t << ": " << v.x << " " << v.y << std::endl;
	else {
		std::cout << v.x << " " << v.y << std::endl;
	}
}

sf::Vector2f closestPointC(circleUse C, sf::Vector2f point) {

	sf::Vector2f fullD = conv(C.self.getPosition()) - conv(point);
	float theta = atan(fullD.y / fullD.x);

	sf::Vector2f take = sf::Vector2f(C.self.getRadius() * cos(theta), C.self.getRadius() * sin(theta));

	if ((fullD.x < 0 && take.x >= 0) || (fullD.x >= 0 && take.x < 0))take.x *= -1;
	if ((fullD.y < 0 && take.y >= 0) || (fullD.y >= 0 && take.x < 0))take.y *= -1;


	return conv(point) + (fullD - take);
	
}

sf::Vector2f closestPointR(rectUse R, sf::Vector2f point) {

	std::vector<sf::Vector2f> corners;

	corners.push_back(conv(R.p1.rep.getPosition()));
	corners.push_back(conv(R.p2.rep.getPosition()));
	corners.push_back(conv(R.p3.rep.getPosition()));
	corners.push_back(conv(R.p4.rep.getPosition()));

	int closest = 0;

	for (int i = 1; i < corners.size(); i++) {
		if (pythag(corners[i] - point) < pythag(corners[closest] - point)) {
			closest = i;
		}
	}

	//^ Get closest corner
	//Now check which side next to corner is the closest side

	float theta = atan((corners[closest] - point).y / (corners[closest] - point).x) * 3.1415926f / 180.0f;
	sf::Vector2f other;

	if (theta >= 45.0f) {
		//right side
		//get other corner
		int temp = closest - 1;
		if (temp == -1)temp = 3;

		std::cout << temp << std::endl;

		other = corners[temp];
	}
	else {
		//left side

		int temp = closest + 1;
		if (temp == 4)temp = 0;


		other = corners[temp];
	}


	//get line and perp line
	line l(corners[closest], other);
	float perpGrad = l.returnPerpGrad();
	line l1(perpGrad, perpGrad * -1 * point.x + point.y);
	l1.p1 = point;

	//get intersection of two lines
	sf::Vector2f intersect = l1.returnIntersect(l);

	sf::Vector2f r;

	if (l.checkInLine(intersect)) {
		//intersect is the closest
		r = intersect;
	}
	else {
		//corner is closest
		r = corners[closest];
	}

	return r;

}

void rayMarch(float d, sf::Vector2f p, std::vector<circleUse> &c, std::vector<rectUse> &r, Point &t1, Point &t2) {

	sf::Vector2f convP = conv(p);

	//Main function
	
	//find closest point between all of squares / circles
	//draw circle with the distance as the radius
	//move along ray line for the distance: radius
	//keep going until the distance moved is very very small
	//project and display this

	sf::Vector2f closest = sf::Vector2f(10000, 100000); //very high number so anything would be closer

	//Circles

	for (int i = 0; i < c.size(); i++) {
		sf::Vector2f pointOnCircle = conv(closestPointC(c[i], p));
		if (pythag(closest - convP) > pythag(pointOnCircle - convP)) closest = pointOnCircle;
	}

	//Rects

	for (int i = 0; i < r.size(); i++) {
		sf::Vector2f pointOnRect = conv(closestPointR(r[i], p));
		if (pythag(closest - convP) > pythag(pointOnRect - convP)) closest = pointOnRect;
	}

	t1.move(conv(closest));

}