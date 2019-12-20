#pragma once
#include <SFML\Graphics.hpp>
using namespace sf;

class circle
{
private:
	int NumOfPoints;
	sf::Vector3f *points;
	sf::Vector2f *points_to_draw;
	sf::RectangleShape pixel;
	sf::Color cl;
	int radius;
	Vector3f pos;
	Vector3f speed;
	float mass;
	float angle;
public:
	circle();
	circle(int points,sf::Color cl, int radius);
	circle(int points, sf::Color cl, int radius, int offsetX, int offsetY, int offsetZ, float mass);
	circle(sf::Color cl, int radius, int offsetX, int offsetY, int offsetZ, circle copy, float mass);
	~circle();
	void init();
	int update();
	int draw(RenderWindow &window);
	void SetNPoints(int NPoints);
	int GetNPoints();
	sf::Vector3f *GetPoints();
	void SetPoints(sf::Vector3f *points);
	sf::Vector2f *GetPoints_to_draw();
	void SetPoints_to_draw(sf::Vector2f *points_to_draw);
	void rotate(float U, float V, float W);
	void setSpeed(Vector3f speed);
	void setPos(Vector3f Pos);
	Vector3f getPos();
	Vector3f getSpeed();
	bool getcollisionTime(circle check, float *res);
	void handleCollision(circle &sphere2);
	void handleWallCollision(Vector2f size);
	static float DotProduct(Vector3f, Vector3f);
	double getMass();
	void setRadius(float radius);
};

