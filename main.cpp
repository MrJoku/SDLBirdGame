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
	
	
	// score text
	TTF_Font* font;
	font = TTF_OpenFont("mononoki.ttf", 24);
	SDL_Color textColor = {0, 0, 0, 255};
	
	SDL_Surface *text = TTF_RenderText_Solid(font, "0", textColor);
	SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, text);
	SDL_FreeSurface(text);
	SDL_Rect textRect = {0, 0, text->w, text->h};

	// fps text
	SDL_Surface *fpsText = TTF_RenderText_Solid(font, "0", textColor);
	SDL_Texture *fpsTextTexture = SDL_CreateTextureFromSurface(renderer, fpsText);
	SDL_FreeSurface(fpsText);
	SDL_Rect fpsTextRect = {100, 0, fpsText->w, fpsText->h};

	int rectSpeed = 60;
	float fallingSpeed = 0.2;
	float pipeSpeed = 0.3;
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

	SDL_Rect pipeMove = {-100, 0, 10, 640};

	SDL_Rect pointColl = {pipe1X, 0, 100, 480};
	bool canScore = true;
	bool close = false;
	SDL_Event event;


	Uint64 NOW = SDL_GetPerformanceCounter();
	Uint64 LAST = 0;

	double deltaTime = 0;

	while (!close)
	{
		Uint64 start = SDL_GetPerformanceCounter();
		// very cool deltatime
		LAST = NOW;
		NOW = SDL_GetPerformanceCounter();
		deltaTime = (double)((NOW - LAST) * 1000 / (double)SDL_GetPerformanceFrequency());


		rect1.y += fallingSpeed * deltaTime;
		pipe1.x -= pipeSpeed * deltaTime;
		pipe2.x -= pipeSpeed * deltaTime;
		pointColl.x -= pipeSpeed * deltaTime;
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
		SDL_RenderCopyEx(renderer, texture, NULL, &rect1, 8 + rect1.y, NULL, SDL_FLIP_NONE);


		SDL_SetRenderDrawColor( renderer, 107, 57, 34, 255);
		SDL_RenderFillRect(renderer, &rect2);

		SDL_SetRenderDrawColor( renderer, 0, 255, 0, 255);
		SDL_RenderFillRect(renderer, &pipe1);
		SDL_RenderFillRect(renderer, &pipe2);

		SDL_bool floorColl = SDL_HasIntersection(&rect1, &rect2);
		SDL_bool pipeColl1 = SDL_HasIntersection(&rect1, &pipe1);
		SDL_bool pipeColl2 = SDL_HasIntersection(&rect1, &pipe2);
		SDL_bool scoreColl = SDL_HasIntersection(&rect1, &pointColl);

		SDL_bool pipe1Move = SDL_HasIntersection(&pipe1, &pipeMove);
		if (floorColl || pipeColl1 || pipeColl2)
		{
			cout << "\nScore " << score << "\n";
			return 1;
		}

		if (pipe1Move)
		{
			pipe1.x = 660;
			pipe2.x = 660;
			pipe1.y = -100 + rand() % 101;
			pipe2.y = 280 + rand() % 101;
			pointColl.x = 660;
			canScore = true;
		}

		if (scoreColl && canScore)
		{
			score++;
			text = TTF_RenderText_Solid(font, to_string(score).c_str(), textColor);
			textTexture = SDL_CreateTextureFromSurface(renderer, text);
			SDL_FreeSurface(text);
			textRect = {0, 0, text->w, text->h};
			canScore = false;
		}
		
		float elapsed;

		cout << "Current FPS: " << to_string(1.0f / elapsed) << endl;
		SDL_Surface *fpsText = TTF_RenderText_Solid(font, to_string(static_cast<int>(round(1.0f/elapsed))).c_str(), textColor);
		SDL_Texture *fpsTextTexture = SDL_CreateTextureFromSurface(renderer, fpsText);
		SDL_FreeSurface(fpsText);
		SDL_Rect fpsTextRect = {100, 0, fpsText->w, fpsText->h};

		SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
		SDL_RenderCopy(renderer, fpsTextTexture, NULL, &fpsTextRect);

		SDL_RenderPresent(renderer);
		Uint64 end = SDL_GetPerformanceCounter();

		elapsed = (end - start) / (float)SDL_GetPerformanceFrequency();

		
	}
}
