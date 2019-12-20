#define _USE_MATH_DEFINES
#include <SFML\Graphics.hpp>
#include <SFML\Window.hpp>
#include "circle.h"
#include <math.h>
using namespace sf;
#define Res 10000

circle::circle(){

}

circle::circle(int points, sf::Color cl, int radius)
{
	SetNPoints(points);
	pixel = RectangleShape(Vector2f(1, 1));
	this->cl = cl;
	this->radius = radius;
	this->angle = 0;
	init();
}

circle::circle(int points, sf::Color cl, int radius, int offsetX, int offsetY, int offsetZ, float mass){
	SetNPoints(points);
	pixel = RectangleShape(Vector2f(1, 1));
	this->cl = cl;
	this->mass = mass;
	this->radius = radius;
	this->pos = Vector3f((float)offsetX, (float)offsetY, (float)offsetZ);
	angle = 0;
	init();
}

circle::circle(sf::Color cl, int radius, int offsetX, int offsetY, int offsetZ, circle copy, float mass){
	SetNPoints(copy.GetNPoints());
	this->cl = cl;
	this->radius = radius;
	this->mass=mass;
	pixel = RectangleShape(Vector2f(1, 1));
	this->pos = Vector3f((float)offsetX, (float)offsetY, (float)offsetZ);
	points = new Vector3f[NumOfPoints];
	points_to_draw = new Vector2f[NumOfPoints];
	for (int i = 0; i < NumOfPoints; i++){
		Vector3f tmp(copy.GetPoints()[i]);
		points[i] = Vector3f(tmp);
		points_to_draw[i] = Vector2f(tmp.x, tmp.y);
	}
	angle = 0;

}

void circle::init(){
	points = (Vector3f *)malloc(sizeof(Vector3f)*GetNPoints());
	points_to_draw = (Vector2f *)malloc(sizeof(Vector2f)*GetNPoints());
	for (int i = 0; i < GetNPoints(); ){
		float x = (float)(rand() % (2 * Res) - Res) / Res;
		float y = (float)(rand() % (2 * Res) - Res) / Res;
		float z = (float)(rand() % (2 * Res) - Res) / Res;

		float len = sqrt(x*x + y*y + z*z);
		if (len > 0){
			x = x / len;
			y = y / len;
			z = z / len;
			points[i]=Vector3f(x, y, z);
			points_to_draw[i] = Vector2f(x, y);
			i++;
		}
		
	}
}

void circle::setSpeed(Vector3f speed){
	this->speed = speed;
}

Vector3f circle::getSpeed(){
	return speed;
}

int circle::update(){
	setPos( getPos() + getSpeed());

	angle = (float)0.81 * sqrt(speed.x * speed.x + speed.y * speed.y + speed.z * speed.z);
	//pos.z += 0.01f;
	return 0;
}

int circle::draw(RenderWindow &window){
	pixel.setFillColor(cl);
	float scale = pos.z;
	if (scale < -0.0001)
	{
		scale = 1 / fabs(scale);
	}
	else if (scale >= -0.0001)
	{
		pos.z = -1;
		scale = pos.z;
		scale = 1 / fabs(scale);
	}

	for (int i = 0; i < GetNPoints(); i++){
		pixel.setPosition(points_to_draw[i].x*radius*scale + pos.x, points_to_draw[i].y*radius*scale + pos.y);
		window.draw(pixel);
	}
	return 0;
}

void circle::handleWallCollision(Vector2f size){
	if (pos.x + speed.x < radius || pos.x + speed.x > size.x - radius){
		setSpeed(Vector3f(-getSpeed().x, getSpeed().y, getSpeed().z));
	}
	if (pos.y + speed.y < radius || pos.y +speed.y > size.y - radius){
		setSpeed(Vector3f(getSpeed().x, -getSpeed().y, getSpeed().z));
	}
}

void circle::handleCollision(circle &sphere2)
{
	Vector3f radVect(sphere2.pos - pos);
	Vector3f help = radVect;
	help.z = radVect.z + 1;
	Vector3f tanVect(radVect.y*help.z - radVect.z*help.y,
		radVect.z*help.x - radVect.x*help.z,
		radVect.x*help.y - radVect.y*help.x);
	Vector3f tantanVect(radVect.y*tanVect.z - radVect.z*tanVect.y,
						radVect.z*tanVect.x - radVect.x*tanVect.z,
						radVect.x*tanVect.y - radVect.y*tanVect.x);
	float len = sqrt(radVect.x*radVect.x + radVect.y*radVect.y + radVect.z*radVect.z);
	radVect = Vector3f(radVect.x / len, radVect.y / len, radVect.z / len);

	len = sqrt(tanVect.x*tanVect.x + tanVect.y*tanVect.y + tanVect.z*tanVect.z);
	tanVect = Vector3f(tanVect.x / len, tanVect.y / len, tanVect.z / len);

	len = sqrt(tantanVect.x*tantanVect.x + tantanVect.y*tantanVect.y + tantanVect.z*tantanVect.z);
	tantanVect = Vector3f(tantanVect.x / len, tantanVect.y / len, tantanVect.z / len);

	Vector3f v1n = radVect * DotProduct(radVect, getSpeed());//x1
	Vector3f v1t = tanVect * DotProduct(tanVect, getSpeed());//y1
	Vector3f v1tt = tantanVect * DotProduct(tantanVect, getSpeed());//z1
	Vector3f v2n = radVect * DotProduct(radVect, sphere2.getSpeed());//x2
	Vector3f v2t = tanVect * DotProduct(tanVect, sphere2.getSpeed());//y2
	Vector3f v2tt = tantanVect * DotProduct(tantanVect, sphere2.getSpeed());//z2
	//s1.vel = vec{v1x}{(m1-m2)/(m1+m2)} + vec{v2x}{(2*m2)/(m1+m2)} + vec{v1y}
	/*float con1 = ((getMass() - sphere2.getMass()) / (getMass() + sphere2.getMass()));
	float con2 = ((2 * getMass()) / (getMass() + sphere2.getMass()));
	Vector3f v1nPrime = (con1 * v1n) + (v2n * con2) + v1t;
	//s2.vel = vec{ v1x }{(2 * m1) / (m1 + m2)} +vec{ v2x }{(m2 - m1) / (m1 + m2)} +vec{ v2y }
	con1 = ((2 * getMass()) / (getMass() + sphere2.getMass()));
	con2 = ((sphere2.getMass() - getMass()) / (getMass() + sphere2.getMass()));
	Vector3f v2nPrime = con1 * v1n + con2 * v2n + v2t;*/
	
	//n
	Vector3f u1n = ((float)getMass()*v1n + (float)sphere2.getMass()*(2.0f*v2n - v1n)) / (float)(getMass() + sphere2.getMass());
	Vector3f u2n = v1n - v2n + u1n;

	//t
	Vector3f u1t = ((float)getMass()*v1t + (float)sphere2.getMass()*(2.0f*v2t - v1t)) / (float)(getMass() + sphere2.getMass());
	Vector3f u2t = - (v1t - v2t + u1t);

	//tt
	Vector3f u1tt = ((float)getMass()*v1tt + (float)sphere2.getMass()*(2.0f*v2tt - v1tt)) / (float)(getMass() + sphere2.getMass());
	Vector3f u2tt = v1tt - v2tt + u1tt;

	Vector3f v1nprime = u1n + u1t + u1tt;
	Vector3f v2nprime = u2n + u2t + u2tt;

	v1nprime.z = 0;
	v2nprime.z = 0;

	setSpeed(v1nprime);
	sphere2.setSpeed(v2nprime);
}

Vector3f circle::getPos(){
	return pos;
}

void circle::rotate(float u, float v, float w){
	float L = (u*u + v * v + w * w);
	angle = (float)angle * M_PI / 180.0; //converting to radian value
	float u2 = u * u;
	float v2 = v * v;
	float w2 = w * w;
	float rotationMatrix[4][4];
	float inputMatrix[4][1];
	float outputMatrix[4][1];
	for (int i = 0; i < GetNPoints();i++){
		float x = points[i].x;
		float y = points[i].y;
		float z = points[i].z;

		inputMatrix[0][0] = x;
		inputMatrix[1][0] = y;
		inputMatrix[2][0] = z;
		inputMatrix[3][0] = 1.0;
		for (int ind = 0; ind < 4; ind++){
			outputMatrix[ind][0] = 0.0;
		}

		rotationMatrix[0][0] = (u2 + (v2 + w2) * cos(angle)) / L;
		rotationMatrix[0][1] = (u * v * (1 - cos(angle)) - w * sqrt(L) * sin(angle)) / L;
		rotationMatrix[0][2] = (u * w * (1 - cos(angle)) + v * sqrt(L) * sin(angle)) / L;
		rotationMatrix[0][3] = 0.0;

		rotationMatrix[1][0] = (u * v * (1 - cos(angle)) + w * sqrt(L) * sin(angle)) / L;
		rotationMatrix[1][1] = (v2 + (u2 + w2) * cos(angle)) / L;
		rotationMatrix[1][2] = (v * w * (1 - cos(angle)) - u * sqrt(L) * sin(angle)) / L;
		rotationMatrix[1][3] = 0.0;

		rotationMatrix[2][0] = (u * w * (1 - cos(angle)) - v * sqrt(L) * sin(angle)) / L;
		rotationMatrix[2][1] = (v * w * (1 - cos(angle)) + u * sqrt(L) * sin(angle)) / L;
		rotationMatrix[2][2] = (w2 + (u2 + v2) * cos(angle)) / L;
		rotationMatrix[2][3] = 0.0;

		rotationMatrix[3][0] = 0.0;
		rotationMatrix[3][1] = 0.0;
		rotationMatrix[3][2] = 0.0;
		rotationMatrix[3][3] = 1.0;

		for (int m= 0; m < 4; m++){
			for (int j = 0; j < 1; j++){
				outputMatrix[m][j] = 0;
				for (int k = 0; k < 4; k++){
					outputMatrix[m][j] += rotationMatrix[m][k] * inputMatrix[k][j];
				}
			}
		}

		x = outputMatrix[0][0];
		y = outputMatrix[1][0];
		z = outputMatrix[2][0];
		points[i] = Vector3f(x, y, z);
		points_to_draw[i] = Vector2f(outputMatrix[0][0], outputMatrix[1][0]);
		
	}
	
}

int circle::GetNPoints(){
	return NumOfPoints;
}

sf::Vector3f *circle::GetPoints(){
	return points;
}

sf::Vector2f *circle::GetPoints_to_draw(){
	return points_to_draw;
}

void circle::SetNPoints(int Npoints){
	NumOfPoints = Npoints;
}

void circle::SetPoints(sf::Vector3f *points){
	this->points = points;
}

void circle::SetPoints_to_draw(sf::Vector2f *points_to_draw){
	this->points_to_draw = points_to_draw;
}

void circle::setPos(Vector3f Pos){
	this->pos.x = Pos.x;
	this->pos.y = Pos.y;
	this->pos.z = Pos.z;
}

float circle::DotProduct(Vector3f a, Vector3f b){
	return (a.x*b.x + a.y*b.y + a.z*b.z);
}

bool circle::getcollisionTime(circle check, float *res){
	Vector3f s = pos - check.pos;
	Vector3f v = speed - check.speed; // relative velocity between spheres
	float r = (float)(radius + check.radius);

	float c = DotProduct(s,s) - r*r; // if negative, they overlap
	if (c < 0.0) // if true, they already overlap
	{
		*res = 0;
		return false;
	}

	float a = DotProduct(v,v);

	float b = DotProduct(v, s);
	if (b >= 0.0)
		return false; // does not move towards each other

	float d = b*b - a*c;
	if (d < 0.0)
		return false; // no real roots ... no collision

	*res = (-b - sqrt(d)) / a;
	return true;
}

double circle::getMass(){
	return mass;
}

void circle::setRadius(float radius){
	this->radius = radius;
}

circle::~circle()
{
}
