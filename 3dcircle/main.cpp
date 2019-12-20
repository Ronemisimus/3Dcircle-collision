#define _USE_MATH_DEFINES
#include <SFML\Graphics.hpp>
#include <SFML\Window.hpp>
#include <time.h>  
#include <math.h>
#include "circle.h"
#include <iostream>

Vector2f size(1366, 768);
using namespace sf;
using namespace std;
int init(RenderWindow &window, vector<circle *> &spheres, int NumOfCircle);
int update(RenderWindow &window, vector<circle *> &sphere);
int draw(RenderWindow &window, vector<circle *> sphere);
int main()
{
	RenderWindow window;
	srand((unsigned)time(NULL));
	int NumOfCircle;
	cout << "enter num of balls" << endl;
	cin >> NumOfCircle;
	vector<circle *> spheres(NumOfCircle);
	init(window, spheres, NumOfCircle);
	while (window.isOpen()){
		update(window, spheres);
		draw(window, spheres);
	}
	system("pause");
	return 0;
}

int init(RenderWindow &window, vector<circle *> &spheres, int NumOfCircle){
	window.create(sf::VideoMode((unsigned)size.x, (unsigned)size.y),"SFML", sf::Style::Fullscreen);
	window.setPosition(Vector2i(0, 0));
	window.setVerticalSyncEnabled(true);
	for (int i = 0; i < NumOfCircle && i < 20; i++)
	{
		circle *tmp;
		if (i == 0)
			tmp = new circle(1500, Color(rand() % 256, rand() % 256, rand() % 256), 60, 100, 100, 0, (float)(rand() % 1000));
		else if (i<6)
			tmp = new circle(Color(rand() % 256, rand() % 256, rand() % 256), 60, (int)(((170 * (i+1)))%1366), 100, 0, (*spheres.at(0)), 900);
		else if (i < 14)
			tmp = new circle(Color(rand() % 256, rand() % 256, rand() % 256), 60, (int)((170 * (i - 5)) % 1366), 320, 0, (*spheres.at(0)), 900);
		else
			tmp = new circle(Color(rand() % 256, rand() % 256, rand() % 256), 60, (int)((170 * (i - 13)) % 1366), 640, 0, (*spheres.at(0)), 900);
		tmp->setSpeed(Vector3f(rand() % 3, rand() % 5, rand() % 5));
		spheres[i]=tmp;
		printf("%d: mass: %f pos: %f %f %f speed: %f %f %f\n",i ,tmp->getMass()
			, tmp->getPos().x, tmp->getPos().y, tmp->getPos().z 
			,tmp->getSpeed().x, tmp->getPos().y, tmp->getPos().z);
	}
	return 0;
}

int update(RenderWindow &window, vector<circle *> &spheres ){
	sf::Event evnt;
	if (window.pollEvent(evnt)){
		if (evnt.type == sf::Event::Closed)
			window.close();
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		window.close();

	for (unsigned i = 0; i < spheres.size(); i++){
		for (unsigned j = i+1; j < spheres.size(); j++){
			float framestillcol;
			bool happend = spheres[i]->getcollisionTime(*(spheres[j]), &framestillcol);
			if (happend && framestillcol <= 1)
			{
				spheres[i]->setPos(spheres[i]->getPos() + framestillcol*spheres[i]->getSpeed());
				spheres[j]->setPos(spheres[j]->getPos() + framestillcol*spheres[j]->getSpeed());
				spheres[i]->handleCollision(*(spheres[j]));
				spheres[i]->setPos(spheres[i]->getPos() + (1 - framestillcol)*spheres[i]->getSpeed());
				spheres[j]->setPos(spheres[j]->getPos() + (1 - framestillcol)*spheres[j]->getSpeed());
			}
		}
	}

	for (unsigned j = 0; j < spheres.size(); j++){
		Vector3f res;
		Vector3f res2;
		res = spheres[j]->getSpeed();
		if (res != Vector3f(0, 0, 0)){
			res2 = Vector3f(res.x, res.y, res.z + 1);
			spheres[j]->rotate(res.y * res2.z - res.z * res2.y
				, res.z* res2.x - res.x * res2.z,
				res.x * res2.y - res.y * res.x);
		}
	}

	for (unsigned i = 0; i < spheres.size(); i++){
		spheres[i]->update();
		spheres[i]->handleWallCollision(size);
	}

	return 0;
}

int draw(RenderWindow &window, vector<circle *> spheres){
	window.clear(sf::Color::Black);
	
	for (auto const& value : spheres) {
		(*value).draw(window);
	}

	window.display();
	return 0;
}