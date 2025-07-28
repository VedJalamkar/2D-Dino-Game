#include <iostream>
#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Network.hpp"
#include "SFML/System.hpp"
#include "SFML/Audio.hpp"

using namespace std;
using namespace sf;

// Renders Window
RenderWindow GameScreen(VideoMode({ 800, 600 }), "Dino Runner", Style::Default);

// Declearing Textures and Sprites
Texture Dino;
Sprite DinoSprite;
Texture Cactus;
Sprite CactusSprite;
Texture BackGround;
Sprite BackGroundSprite;
Texture Clouds;
Sprite CloudsSprite;

// Declearing Positions
float DinoX = 10.0f;
float DinoY = 400.0f;
float CactusX = 400.0f;
float CactusY = 414.0f;
float CloudsX = 400.0f;
float CloudsY = 10.0f;

// Required Variables for Game Mechanics
bool isJumping = false;
bool isGameOver = false;
float Gravity = 0.5f;
float VelocityDinoY = 0.0f;
float Ground = 400.0f;
const int MaxJumps = 15;
int JumpCount = 0;

// Game Score
int Score = 0;
Font GameOverFont;
Text ScoreDisplay;


void GameAssets() {
	// BackGround
	BackGround.loadFromFile("Background.png");
	BackGroundSprite.setTexture(BackGround);
	BackGroundSprite.setScale((float)800 / BackGround.getSize().x,
							  (float)600 / BackGround.getSize().y);
	BackGroundSprite.setPosition(0, 0);

	Clouds.loadFromFile("Clouds.png");
	CloudsSprite.setTexture(Clouds);
	CloudsSprite.setScale(0.5f, 0.5f);
	CloudsSprite.setPosition(CloudsX, CloudsY);

	// Load Texture and Sprites
	Dino.loadFromFile("Dino.png");
	DinoSprite.setTexture(Dino);
	DinoSprite.setScale(0.075f, 0.075f);
	DinoSprite.setPosition(DinoX, DinoY); // Just for Testing

	Cactus.loadFromFile("Cactus.png");
	CactusSprite.setTexture(Cactus);
	CactusSprite.setScale(0.4f, 0.4f);
	CactusSprite.setPosition(CactusX, CactusY); // Just for Testing

	// Game Over Display
	GameOverFont.loadFromFile("arial.ttf");
	ScoreDisplay.setFont(GameOverFont);
	ScoreDisplay.setCharacterSize(30.0f);
	ScoreDisplay.setFillColor(Color::Black);
	ScoreDisplay.setPosition(350.0f, 250.0f);
}

void DinoMovement() {
	//Dino Movements of Jumping
	if (Keyboard::isKeyPressed(Keyboard::Space) && !isJumping) {
		isJumping = true;
		VelocityDinoY = -10.0f;
		JumpCount++;

	}
	if (isJumping) {
		DinoSprite.move(0, VelocityDinoY);
		VelocityDinoY += Gravity;

		if (DinoSprite.getPosition().y > Ground) {
			DinoSprite.setPosition(DinoSprite.getPosition().x, Ground);
			isJumping = false;
			VelocityDinoY = 0.0f;
			JumpCount = 0;
		}
	}
}

void ObstacleCactus() {
	//Cactus Movement
	CactusSprite.move(-5.0f, 0);
	if (CactusSprite.getPosition().x < -50) {
		CactusSprite.setPosition(800, CactusY);
		Score++;
	}

	CloudsSprite.move(-3.0f, 0);
	if (CloudsSprite.getPosition().x < -50) {
		CloudsSprite.setPosition(800, CloudsY);
	}
}

void CollosionLogic() {

	// HitBoxes Adjustments
	FloatRect DinoHitBox = FloatRect(
		DinoSprite.getPosition().x + 15,
		DinoSprite.getPosition().y + 15       ,
		DinoSprite.getGlobalBounds().width - 35,
		DinoSprite.getGlobalBounds().height - 35
	);

	FloatRect CactusHitBox = FloatRect(
		CactusSprite.getPosition().x + 25,
		CactusSprite.getPosition().y,
		CactusSprite.getGlobalBounds().width - 40,
		CactusSprite.getGlobalBounds().height - 5
	);

	if (DinoHitBox.intersects(CactusHitBox)) {
		isGameOver = true;
	}

}


int main() {

	GameScreen.setFramerateLimit(60); // Sets Framelimit to 60
	GameAssets();

	//GameLoop
	while (GameScreen.isOpen()) {
		GameScreen.clear(Color:: White);
		Event UserResponse;
		while (GameScreen.pollEvent(UserResponse)) {
			if (UserResponse.type == Event::Closed) {
				GameScreen.close();
			}
		} 
		if (!isGameOver) {
			GameScreen.draw(BackGroundSprite);
			GameScreen.draw(CloudsSprite);
			GameScreen.draw(DinoSprite);
			GameScreen.draw(CactusSprite);
			DinoMovement();
			ObstacleCactus();
			CollosionLogic();
			GameScreen.display();
		}
		else {
			ScoreDisplay.setString("Score: " + to_string(Score));
			GameScreen.draw(ScoreDisplay);
			GameScreen.display();
			sleep(seconds(1));
			break;
		}
	}
	return 0;
}