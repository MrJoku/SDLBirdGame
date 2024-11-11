#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_ttf.h>

using namespace std;

int main(int argc, char *argv[])
{
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();

	SDL_Window *window = SDL_CreateWindow("Flappy bord", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, 0);

	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

	SDL_Texture *texture = IMG_LoadTexture(renderer, "bird.png");
	
	
	//text
	TTF_Font* font;
	font = TTF_OpenFont("mononoki.ttf", 24);
	SDL_Color textColor = {255, 0, 0, 255};
	SDL_Surface *text = text = TTF_RenderText_Solid(font, "Hello world!", textColor);
	SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, text);
	SDL_FreeSurface(text);
	SDL_Rect textRect = {0, 0, text->w, text->h};	

	int rectSpeed = 60;
	int fallingSpeed = 1;
	int pipeSpeed = 2;
	int x = 300;
	int y = 200;
	int score = 0;
	
	SDL_Rect rect1 = {x, y, 30, 30};
	SDL_Rect rect2 = {0, 440, 640, 40};

	int pipe1X = 600;
	int pipe1Y = 0;
	SDL_Rect pipe1 = {pipe1X, pipe1Y, 100, 200};

	int pipe2X = 600;
	int pipe2Y = 380;
	SDL_Rect pipe2 = {pipe2X, pipe2Y, 100, 200};

	SDL_Rect pipeMove = {0, 0, 10, 640};

	SDL_Rect pointColl = {pipe1X, 0, 100, 480};
	bool canScore = true;
	bool close = false;
	SDL_Event event;
	while (!close)
	{
		cout << SDL_GetTicks() << "\n";
		rect1.y += 1;
		pipe1.x -= pipeSpeed;
		pipe2.x -= pipeSpeed;
		pointColl.x -= pipeSpeed;
		// INPUT
		SDL_PollEvent(&event);
		switch (event.type)
		{
			case SDL_QUIT:
				close = true;
				break;

			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
					case SDLK_q : return 1 ; break;
					case SDLK_SPACE : rect1.y -= rectSpeed ; break;
				}
		}

		// RENDERING & COLLISION
		SDL_SetRenderDrawColor( renderer, 85, 244, 244, 255);
		SDL_RenderClear(renderer);

		// BIRD RECT
		//SDL_SetRenderDrawColor( renderer, 255, 0, 0, 255);
		//SDL_RenderFillRect(renderer, &rect1);
		//SDL_RenderCopy(renderer, texture, NULL, &rect1);
		SDL_RenderCopyEx(renderer, texture, NULL, &rect1, 8 + rect1.y, NULL, SDL_FLIP_NONE);


		SDL_SetRenderDrawColor( renderer, 107, 57, 34, 255);
		SDL_RenderFillRect(renderer, &rect2);

		SDL_SetRenderDrawColor( renderer, 0, 255, 0, 255);
		SDL_RenderFillRect(renderer, &pipe1);
		SDL_RenderFillRect(renderer, &pipe2);
		//SDL_RenderFillRect(renderer, &pipeMove);
		//SDL_RenderFillRect(renderer, &pointColl);

		SDL_bool floorColl = SDL_HasIntersection(&rect1, &rect2);
		SDL_bool pipeColl1 = SDL_HasIntersection(&rect1, &pipe1);
		SDL_bool pipeColl2 = SDL_HasIntersection(&rect1, &pipe2);
		SDL_bool scoreColl = SDL_HasIntersection(&rect1, &pointColl);

		SDL_bool pipe1Move = SDL_HasIntersection(&pipe1, &pipeMove);
		if (floorColl || pipeColl1 || pipeColl2)
		{
			//SDL_SetRenderDrawColor(renderer, 242, 242, 242, 255);
			cout << "\nScore " << score << "\n";
			return 1;
		}
		/*else
			SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);*/

		if (pipe1Move)
		{
			pipe1.x = 660;
			pipe2.x = 660;
			pipe1.y = -100 + rand() % 101;
			pipe2.y = 280 + rand() % 101;
			pointColl.x = 660;
			//cout << pipe1.y << "\n" << pipe2.y << "\n";
			canScore = true;
		}

		if (scoreColl && canScore)
		{
			score++;
			canScore = false;
			//char scoreString = static_cast<char>(score);
			SDL_Surface *text = text = TTF_RenderText_Solid(font, /**scoreString*/ "Hello World!", textColor);
			SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, text);
			SDL_FreeSurface(text);
		}

		SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
		SDL_RenderPresent(renderer);
	}
}
