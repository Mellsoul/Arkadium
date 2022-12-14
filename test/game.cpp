
#include <iostream>
#include "Framework.h"
#include <cstdlib>
#include <cmath>





struct brick //my own struct
{
	int x, y; //Vector x y;
	int xA, yA; //speed Vector
	int w, h; //Width and Height
	int health=1;
	bool isAlive = true;
	int score = 4;
};
const int blueAmount = 48; //Change amount of bricks you want to spawn
brick brick1[blueAmount]; //blue bricks
//const int redAmount = 2;
//brick brick2[redAmount]; //red bricks*/

struct player//my own struct
{
	float x, y; //Vector x y;
	int w, h; //Width and Height
	bool moveLeft=false;
	bool moveRight=false;
};
player player1;

Sprite* blueBrick;
Sprite* redBrick;
Sprite* smallBall;
Sprite* playerDefault;
Sprite* cursorSprite;
Sprite* backgroundBrick;

struct screenSize
{
	int w, h;
};
screenSize myScreen;

struct ball
{
	float x, y;
	int w, h;
	float xA, yA; //Speed Vector
	bool isAlive=false;
	float initTime=0;
};
const int ballAmount = 3;
ball ball1[ballAmount];

struct cursor
{
	int x, y;
	int xrelative, yrelative;
	int w, h;
};
cursor cursor1;

struct Vector2
{
	Vector2(float xValue, float yValue)
	{
		x = xValue;
		y = yValue;
	}

	float x;
	float y;

	Vector2 Normalize()
	{
		// sqrt length
		float length = sqrt((x * x) + (y * y));
		if (length != 0)
			return Vector2(x / length, y / length);
		else
			return Vector2(x, y);
	}
};


//Time control
int currentTick=0;
//unsigned int previousTick = 0;

//Score
int score = 0;
//Ability
float abilitySpeed = 1;

//Game states;
bool gameWon = false;
bool gameLost = false;
bool gameStart = false;

/* Test Framework realization */
class MyFramework : public Framework {

public:
	virtual void PreInit(int& width, int& height, bool& fullscreen)
	{
		gameWon = false;
		gameLost = false;
		gameStart = false;

		std::cout << "Enter Width" << std::endl;
		std::cin >> width;
		myScreen.w = width;
		std::cout << "Enter Height" << std::endl;
		std::cin >> height;
		myScreen.h = height;
		fullscreen = false;
		std::cout << "Your screen size: " << myScreen.w << "x" << myScreen.h << std::endl;

		for (int i = 0; width < 32 || height < 32; i++) //if resolution is too low
		{
			std::cout << "Don't bully my code" << std::endl;
			std::cout << "Enter Width" << std::endl;
			std::cin >> width;
			myScreen.w = width;
			std::cout << "Enter Height" << std::endl;
			std::cin >> height;
			myScreen.h = height;
			fullscreen = false;
			std::cout << "Your screen size: " << myScreen.w << "x" << myScreen.h << std::endl;

		}

	}

	virtual bool Init() {

		showCursor(false);

		blueBrick = createSprite("data/01-Breakout-Tiles.png");
		redBrick = createSprite("data/07-Breakout-Tiles.png");
		smallBall = createSprite("data/58-Breakout-Tiles.png");
		playerDefault = createSprite("data/50-Breakout-Tiles.png");
		cursorSprite = createSprite("data/59-Breakout-Tiles.png");
		backgroundBrick = createSprite("data/29-Breakout-Tiles.png");


		for (int i = 0; i < blueAmount; i++) //Adjusting size of brick textures
		{
			getSpriteSize(blueBrick, brick1[i].w, brick1[i].h);
			std::cout << "Size of " << i << " blueBrick " << brick1[i].w << "x" << brick1[i].h << std::endl;
		}
		for (int i = 0; i < blueAmount; i++)
		{
			setSpriteSize(blueBrick, brick1[i].w * (myScreen.w / 32)/100, brick1[i].h * (myScreen.h / 32)/100);
			brick1[i].w = brick1[i].w * (myScreen.w / 32) / 100;
			brick1[i].h = brick1[i].h * (myScreen.h / 32) / 100;

			brick1[i].x = myScreen.w / 8 + myScreen.w / 8 * (i % 6);
			brick1[i].y = myScreen.h / 24 * (i / 6);

			std::cout << "New size of " << i << " blueBrick " << brick1[i].w << "x" << brick1[i].h  << std::endl;

			//drawSprite(blueBrick, myScreen.w / 8 + myScreen.w / 8 * (i % 6), myScreen.h / 24 * (i / 6));

		}
		



		getSpriteSize(playerDefault, player1.w, player1.h); //Adjusting size of player textures
		setSpriteSize(playerDefault, player1.w * (myScreen.w / 32) / 100, player1.h * (myScreen.h / 32) / 100);
		player1.w = player1.w * (myScreen.w / 32) / 100;
		player1.h = player1.h * (myScreen.h / 32) / 100;
		std::cout << "Size of player: " << player1.w << "x" << player1.h << std::endl;

		player1.x = myScreen.w / 2 - player1.w / 2;//starting position middle of bottom edge
		player1.y = myScreen.h - player1.h;
		std::cout << "Player starting position: " << player1.x << "x" << player1.y<< std::endl;


		for (int i = 0; i < ballAmount; i++)
		{
			getSpriteSize(smallBall, ball1[i].w, ball1[i].h); //Adjusting size of ball textures
		}
		for (int i = 0; i < ballAmount; i++) {
			setSpriteSize(smallBall, ball1[i].w * (myScreen.w / 32) / 100, ball1[i].h * (myScreen.h / 32) / 100);
			ball1[i].w = ball1[i].w * (myScreen.w / 32) / 100;
			ball1[i].h = ball1[i].h * (myScreen.w / 32) / 100; //Ball only scales with width of screen
			std::cout << "New Size of ball: " << ball1[i].w << "x" << ball1[i].h << std::endl;
			ball1[0].xA = 0;
			ball1[0].yA = 0;
			ball1[1].xA = 0;
			ball1[1].yA = 0;
			ball1[2].xA = 0;
			ball1[2].yA = 0;
		}
		for (int i = 0; i < ballAmount; i++)
		{
			ball1[i].x = myScreen.w / 2 -(player1.w/2) +(player1.w/2) * i; //starting balls positions
			ball1[i].y = myScreen.h - 3 * player1.h;
		}
			
		getSpriteSize(cursorSprite, cursor1.w, cursor1.h); //Adjusting size of cursor texture
		setSpriteSize(cursorSprite, cursor1.w * (myScreen.w / 32) / 100, cursor1.h * (myScreen.h / 32) / 100);
		cursor1.w = cursor1.w * (myScreen.w / 32) / 100;
		cursor1.h = cursor1.h * (myScreen.w / 32) / 100; //Cursor only scales with width of screen
		std::cout << "New Size of cursor: " << cursor1.w << "x" << cursor1.h << std::endl;

		return true;
	}

	virtual void Close() {

	}

	virtual bool Tick() {

		currentTick = getTickCount();


		drawTestBackground();

		for (int i = 0; i*128<myScreen.h; i++)
		{
			for (int j = 0; j*128<myScreen.w; j++)
			{
				drawSprite(backgroundBrick, j * 128, i * 128);

			}

		}


		for (int i = 0; i < blueAmount; i++)
		{
			if (brick1[i].isAlive)
			{
				drawSprite(blueBrick, brick1[i].x, brick1[i].y); //spawning 6 bricks per row
			}
		}

		for (int i = 0; i < ballAmount; i++)
		{
			if (ball1[i].isAlive)
			{
				//collision borders
				if (ball1[i].y < 0)
				{
					ball1[i].yA *= (-1);
				}
				if (ball1[i].x < 0 || ball1[i].x>myScreen.w - ball1[i].w)
				{
					ball1[i].xA *= (-1);
				}
				if (ball1[i].y > myScreen.h)
				{
					ball1[i].isAlive = false;

				}

				//collision player
				if (ball1[i].x > player1.x && ball1[i].x < player1.x + player1.w) 
				{
					if (ball1[i].y + ball1[i].h > player1.y)
					{
						ball1[i].y = player1.y - ball1[i].h;
						ball1[i].yA *= (-1);
					}
				}

				
				//collision bricks
				for (int j = 0; j < blueAmount; j++)
				{
					if (brick1[j].isAlive)
					{
						if (ball1[i].x + ball1[i].w > brick1[j].x && ball1[i].x < brick1[j].x + brick1[j].w)
						{
							//std::cout << "Left" << ball1[i].x << "," << brick1[j].x << "Right:" << ball1[i].x + ball1[i].w << "," << brick1[j].x + brick1[j].w << std::endl;
							if (ball1[i].y + ball1[i].h > brick1[j].y && ball1[i].y < brick1[j].y + brick1[j].h)
							{
								brick1[j].isAlive = false;
								score += brick1[i].score;
								std::cout << "Current score: " << score << std::endl;
								if (ball1[i].y + ball1[i].h > brick1[j].y + 5 && ball1[i].y < brick1[j].y + brick1[j].h - 5)
								{
									ball1[i].xA *= (-1);
								}
								else
								{

									ball1[i].yA *= (-1);
								}
							}
							
						}

					}

				}
				/*double deltaTime = getTickCount() - previousTick;
				deltaTime /= 10;
				previousTick = getTickCount();
				std::cout << deltaTime << std::endl;*/



				//Speed Vector
				ball1[i].x += ball1[i].xA; //*deltaTime;
				ball1[i].y += ball1[i].yA; //*deltaTime;
				

			}

				drawSprite(smallBall, ball1[i].x, ball1[i].y); //draw ball

				
		}
		//std::cout << getTickCount() << std::endl;

		if (brick1[0].x >= 0 && brick1[5].x <= myScreen.w) {

			for (int i = 0; i < blueAmount; i++)
			{
				brick1[i].x += brick1[i].xA;
				if (currentTick % 1000 >= 500)
				{
					brick1[i].xA = 2;
				}
				else
				{
					brick1[i].xA = -2;
				}
			}
		}

	


		if (player1.moveLeft && player1.x>0)
		{
			player1.x -= 4 *abilitySpeed;
		}
		if (player1.moveRight && player1.x<myScreen.w-player1.w)
		{
			player1.x += 4 * abilitySpeed;
		}

		drawSprite(playerDefault, player1.x, player1.y); //player
		drawSprite(cursorSprite, cursor1.x, cursor1.y);

		//Win/lose condition;
		int deadBalls = 0;
		for (int i = 0; i < ballAmount; i++)
		{
			if (ball1[i].isAlive == false)
			{
				deadBalls++;
			}
			if (deadBalls == ballAmount && gameLost == false /*&& gameStart == true*/ )
			{
				std::cout << "Start your game or Press UP to reset after lose" << std::endl;
				std::cout << "Launch balls by pressing left mouse button" << std::endl;
				gameLost = true;
				gameStart = false;

			}
		}
		int deadBricks = 0;

		for (int i = 0; i < blueAmount; i++)
		{
			if (brick1[i].isAlive == false)
			{
				deadBricks++;
			}

			if (deadBricks == blueAmount && gameWon == false/*&& gameStart == true*/ )
			{
				std::cout << "You Win, let balls fall and Press UP to restart" << std::endl;
				gameWon = true;
				gameStart = false;
			}
		}


		return false;
	}

	virtual void onMouseMove(int x, int y, int xrelative, int yrelative) {	
		cursor1.x = x;
		cursor1.y = y;
		cursor1.xrelative = xrelative;
		cursor1.yrelative = yrelative;
		
		
	}

	virtual void onMouseButtonClick(FRMouseButton button, bool isReleased) {
		FRMouseButton::LEFT;
		FRMouseButton::RIGHT;
		
		if (button == FRMouseButton::LEFT && isReleased==true) //launch balls
		{
			if (ball1[0].isAlive == false)
			{
				Vector2 direction0 = Vector2(cursor1.x - ball1[0].x, cursor1.y - ball1[0].y);
				direction0 = direction0.Normalize();
				ball1[0].isAlive = true;
				ball1[0].xA = direction0.x*2 ;
				ball1[0].yA = direction0.y*2 ;
				ball1[0].initTime = getTickCount();
				std::cout << "Direction of ball 0 x:" << direction0.x <<std::endl;
				std::cout << "Direction of ball 0 y:" << direction0.y <<std::endl;

			}

			if (ball1[0].isAlive == true && ball1[1].isAlive == false && currentTick - ball1[0].initTime >100 /*At least 1s break in between launching the balls*/)
			{
				Vector2 direction0 = Vector2(cursor1.x - ball1[1].x, cursor1.y - ball1[1].y);
				direction0 = direction0.Normalize();
				ball1[1].isAlive = true;
				ball1[1].xA = direction0.x*2;
				ball1[1].yA = direction0.y*2;
				ball1[1].initTime = getTickCount();


			}

			if (ball1[1].isAlive == true && ball1[2].isAlive == false && currentTick - ball1[1].initTime > 100)
			{
				Vector2 direction0 = Vector2(cursor1.x - ball1[2].x, cursor1.y - ball1[2].y);
				direction0 = direction0.Normalize();
				ball1[2].isAlive = true;
				ball1[2].xA = direction0.x*2;
				ball1[2].yA = direction0.y*2;
				ball1[2].initTime = getTickCount();

			}
		}
		if (button == FRMouseButton::RIGHT && isReleased == true)
		{
			if (score >= 20)
			{
				score -= 20;
				if (currentTick % 2 == 0)
				{
					abilitySpeed +=0.4;
					if (abilitySpeed > 2)
					{
						abilitySpeed = 2;
					}
					std::cout << "Current ability speed: " << abilitySpeed;

				}
				else if (currentTick % 2 == 1)
				{
					abilitySpeed -= 0.4;
					if (abilitySpeed < 0.5)
					{
						abilitySpeed =0.5;
					}
					std::cout << "Current ability speed: " << abilitySpeed;
				}
			}
			
		}
	}

	virtual void onKeyPressed(FRKey k) {
		
		FRKey::LEFT;
		FRKey::RIGHT;
		FRKey::UP;
		if (k == FRKey::LEFT)
		{
			player1.moveLeft = true;
		}
		if (k==FRKey::RIGHT)
		{
			player1.moveRight = true;
		}
		if (gameStart == false)
		{
			if (k == FRKey::UP) //press UP to restart the game
			{
				gameStart = true;
				gameLost = false;
				gameWon = false;
				score = 0;
				for (int i = 0; i < blueAmount; i++)
				{
					brick1[i].isAlive = true;
				}
				for (int i = 0; i < ballAmount; i++)
				{
					ball1[i].x = myScreen.w / 2 - (player1.w / 2) + (player1.w / 2) * i; //starting balls positions
					ball1[i].y = myScreen.h - 3 * player1.h;
					ball1[i].xA = 0;
					ball1[i].yA = 0;
			
				}
				player1.x = myScreen.w / 2 - player1.w / 2; //starting position middle of bottom edge
				player1.y = myScreen.h - player1.h;
			}
		}

	}

	virtual void onKeyReleased(FRKey k) {
		FRKey::LEFT;
		FRKey::RIGHT;
		if (k == FRKey::LEFT)
		{
			player1.moveLeft = false;
		}
		if (k == FRKey::RIGHT)
		{
			player1.moveRight = false;
		}

	}

	virtual const char* GetTitle() override
	{
		return "Arcanoid";
	}
};

int main(int argc, char* argv[])
{
	
	return run(new MyFramework);

}
