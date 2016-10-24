#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "bird.h"
#include <cmath>

static const sf::Vector2f BIRD_SIZE = { 40 , 28 };
static const sf::Vector2f BIRD_POSITION = { 80, 320 };
static const int COLLISION_SHAPE_RADIUS = 10;

static const float JUMP_HEIGHT = 70;
static const float G = 12;
static const float IMPULSE = 200;

static const float UP_ROT_ANGALE = -50;
static const float DOWN_ROT_ANGLE = 90;
static const float DOWN_ROT_SPEED = 0.9;

static const float FLAPPING_SPEED = 15;

bool initializeBird(Bird &bird)
{
	bird.shape.setSize(BIRD_SIZE);
	if (!bird.shapeTexture.loadFromFile("resources/MainHero.png"))
		return false;
	bird.shape.setTexture(&bird.shapeTexture);
	bird.shape.setTextureRect(sf::IntRect(40, 0, 40, 28));
	bird.shape.setRotation(0);
	bird.shape.setOrigin(bird.shape.getGlobalBounds().width / 2.0f, bird.shape.getGlobalBounds().height / 2.0f);
	bird.shape.setPosition(BIRD_POSITION);
	bird.jumping = NOT_STARTED;
	bird.jumpingVector = { 0, 0, 0 }; // {speed, time, past height}
	bird.animTime = 0;

	bird.collisionShape.setRadius(COLLISION_SHAPE_RADIUS);
	bird.collisionShape.setOrigin(bird.collisionShape.getGlobalBounds().width / 2.0f, bird.collisionShape.getGlobalBounds().height / 2.0f);
	bird.collisionShape.setPosition(BIRD_POSITION);
	bird.collisionShape.setFillColor(sf::Color::White);

	return true;
}

void animateBird(Bird &bird, const float &elapsedTime)
{
	bird.animTime += FLAPPING_SPEED * elapsedTime;
	if ((int)bird.animTime > 2)
	{
		bird.animTime = 0;
	}
	bird.shape.setTextureRect(sf::IntRect((int)bird.animTime * 40, 0, 40, 28));
}

void startJump(Bird &bird, Interface &gui)
{
	gui.wingSound.play();
	if (bird.jumping != STARTED)
	{
		gui.ost.play();
		bird.jumping = STARTED;
	}
	bird.jumpingVector[0] = sqrt((2.0f * JUMP_HEIGHT) / G);
	bird.jumpingVector[1] = 0;
	bird.jumpingVector[2] = 0;
}

void birdJump(const float &elapsedTime, Bird &bird)
{
	float time = bird.jumpingVector[1] += elapsedTime;
	float speed = bird.jumpingVector[0];
	float pastHeight = bird.jumpingVector[2];
	float height;
	float movement;

	bird.jumpingVector[2] = height = speed * time - 0.5f * G * pow(time, 2.0f);
	movement = pastHeight - height;
	if (bird.shape.getPosition().y < -BIRD_SIZE.y)
	{
		bird.shape.setPosition(BIRD_POSITION.x, -BIRD_SIZE.y);
	}

	if (movement < 0)
	{
		bird.shape.setRotation(UP_ROT_ANGALE);
	}
	else if (bird.shape.getRotation() != DOWN_ROT_ANGLE)
	{
		bird.shape.rotate(DOWN_ROT_SPEED);
		if (bird.shape.getRotation() < 360 + UP_ROT_ANGALE && bird.shape.getRotation() > DOWN_ROT_ANGLE)
			bird.shape.setRotation(DOWN_ROT_ANGLE);
	}

	bird.shape.move(0, IMPULSE * movement);
	bird.collisionShape.setPosition(bird.shape.getPosition());
}