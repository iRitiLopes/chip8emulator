#include <SDL3/SDL.h>
#include <iostream>
#include <stdio.h>
#include <bitset>
#include <vector>

#pragma once

#define TEXTURE_SIZE 250

class Platform{

	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Texture* texture;
	SDL_Surface* surface;
	SDL_FRect dst_rect;

	int WIDTH;
	int HEIGHT;

public:
	Platform(const char* title, int width, int height, int textureWidth, int textureHeight) {
		WIDTH = width;
		HEIGHT = height;

		if (SDL_Init(SDL_INIT_VIDEO) < 0) {
			std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
			exit(1);
		}
		
		Uint32 window_flags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN;

		SDL_CreateWindowAndRenderer(title, width, height, window_flags, &window, &renderer);

		texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, textureWidth, textureHeight);
		SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);

		if (!window) {
			std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
			SDL_Quit();
			exit(1);
		}

		if (renderer == nullptr) {
			SDL_Log("Error: SDL_CreateRenderer(): %s\n", SDL_GetError());
			exit(-1);
		}

		SDL_SetRenderScale(renderer, 1, 1);

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		//SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		//SDL_RenderPoint(renderer, width /2, height /2);

		//SDL_RenderPresent(renderer);

		SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
		SDL_ShowWindow(window);

		
	}

	~Platform()
	{
		quit();
	}

	void quit() {
		SDL_DestroyTexture(texture);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
	}

	void processInput(uint8_t* keys, bool* close) {
		SDL_Event event;

		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_EVENT_KEY_DOWN:
				switch (event.key.key) {
					case SDLK_ESCAPE:
						std::cout << "Escape key pressed!" << std::endl;
						quit();
						*close = true;
						break;
					case SDLK_X:
						keys[0] = 1;
						break;
					case SDLK_1:
						keys[1] = 1;
						break;
					case SDLK_2:
						keys[2] = 1;
						break;
					case SDLK_3:
						keys[3] = 1;
						break;
					case SDLK_Q:
						keys[4] = 1;
						break;
					case SDLK_W:
						keys[5] = 1;
						break;
					case SDLK_E:
						keys[6] = 1;
						break;
					case SDLK_A:
						keys[7] = 1;
						break;
					case SDLK_S:
						keys[8] = 1;
						break;
					case SDLK_D:
						keys[9] = 1;
						break;
					case SDLK_Z:
						keys[0xA] = 1;
						break;
					case SDLK_C:
						keys[0xB] = 1;
						break;
					case SDLK_4:
						keys[0xC] = 1;
						break;
					case SDLK_R:
						keys[0xD] = 1;
						break;
					case SDLK_F:
						keys[0xE] = 1;
						break;
					case SDLK_V:
						keys[0xF] = 1;
						break;
				}
				break;
			case SDL_EVENT_KEY_UP:
				switch (event.key.key) {
					case SDLK_X:
						keys[0] = 0;
						break;
					case SDLK_1:
						keys[1] = 0;
						break;
					case SDLK_2:
						keys[2] = 0;
						break;
					case SDLK_3:
						keys[3] = 0;
						break;
					case SDLK_Q:
						keys[4] = 0;
						break;
					case SDLK_W:
						keys[5] = 0;
						break;
					case SDLK_E:
						keys[6] = 0;
						break;
					case SDLK_A:
						keys[7] = 0;
						break;
					case SDLK_S:
						keys[8] = 0;
						break;
					case SDLK_D:
						keys[9] = 0;
						break;
					case SDLK_Z:
						keys[0xA] = 0;
						break;
					case SDLK_C:
						keys[0xb] = 0;
						break;
					case SDLK_4:
						keys[0xC] = 0;
						break;
					case SDLK_R:
						keys[0xD] = 0;
						break;
					case SDLK_F:
						keys[0xE] = 0;
						break;
					case SDLK_V:
						keys[0xF] = 0;
						break;
				}
				break;
			default:
				break;
			}
		}
	}


	void update(uint32_t const* buffer, int pitch) {
		//bool running = true;
		SDL_Event event;

		auto x = SDL_UpdateTexture(texture, nullptr, buffer, pitch);
		SDL_RenderClear(renderer);
		SDL_RenderTexture(renderer, texture, nullptr, nullptr);
		SDL_RenderPresent(renderer);

		return;

		//while (running) {
			while (SDL_PollEvent(&event)) {
				if (event.type == SDL_EVENT_QUIT) { // Close button clicked
					//running = false;
				}

				if (event.type == SDL_EVENT_KEY_DOWN) {
					if (event.key.key == SDLK_W) {
						std::cout << "W key pressed!" << std::endl;
					}

					auto time = SDL_GetTicks();
					auto sin = SDL_sin(time);

					auto x = (this->WIDTH + sin * 20) / 1.25;
					auto y = (this->HEIGHT + sin * 20) / 1.25;

					SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
					SDL_RenderClear(renderer);
					drawPixel(x, y);
					drawPixel();


					dst_rect.x = ((float)(this->WIDTH - TEXTURE_SIZE)) / 2.0f;
					dst_rect.y = ((float)(this->HEIGHT - TEXTURE_SIZE)) / 2.0f;
					dst_rect.w = dst_rect.h = (float)TEXTURE_SIZE;
					SDL_RenderTexture(renderer, texture, NULL, &dst_rect);
					SDL_RenderPresent(renderer);
				}
			}

			// Delay to limit CPU usage
			SDL_Delay(16);
		//}
	}

	void drawPixel(float x, float y) {
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderPoint(renderer, x, y);
	}

	void drawPixel2() {
		if (SDL_LockTextureToSurface(texture, NULL, &surface)) {
			SDL_Rect r[3];

			r[0].w = 20;
			r[0].h = 20;
			r[0].x = 0;
			r[0].y = 0;

			r[1].w = 20;
			r[1].h = 20;
			r[1].x = 0;
			r[1].y = 1 + 20;

			r[2].w = 20;
			r[2].h = 20;
			r[2].x = 0;
			r[2].y = 2 + 40;

			SDL_FillSurfaceRects(surface, r, 3, SDL_MapRGB(SDL_GetPixelFormatDetails(surface->format), NULL, 0, 255, 0));
			SDL_UnlockTexture(texture);
		}
	}

	void drawPixel() {
		
		if (SDL_LockTextureToSurface(texture, NULL, &surface)) {
			uint8_t a[5] = { 0xE0, 0x90, 0x90, 0x90, 0xE0 };

			std::vector<SDL_Rect> rs;
			SDL_Rect r[40];
			SDL_FillSurfaceRect(surface, NULL, SDL_MapRGB(SDL_GetPixelFormatDetails(surface->format), NULL, 0, 0, 0));  /* make the whole surface black */
			for (int i = 0; i < sizeof(a); i++) {
				std::bitset<8> repr = a[i];
				//std::cout << repr << std::endl;

				auto repr_str = repr.to_string();

				//std::cout << repr_str << std::endl;
				for (int j = 0; j < repr_str.length(); j++) {
					std::cout << repr_str[j];
					
					
					if (repr_str[j] == '1') {
						SDL_Rect ra;
						ra.w = 20;
						ra.h = 20;
						ra.x = j + j * 20;
						ra.y = i + i * 20;
						rs.push_back(ra);

						//std::cout << ra.x << std::endl;

						/*std::cout << "x: " << r[(i * 8 + j)].x << " , y: " << r[(i * 8 + j)].y << std::endl;
						std::cout << "j: " << j << ", i: " << i << std::endl;
						std::cout << "r[(i*8 + j) = " << (i * 8 + j) << "]: " << r << std::endl;*/
					}
				}
				
				std::cout << std::endl;
			}

			SDL_FillSurfaceRects(surface, &rs[0], rs.size(), SDL_MapRGB(SDL_GetPixelFormatDetails(surface->format), NULL, 0, 255, 0));  /* make a strip of the surface green */
			SDL_UnlockTexture(texture);  /* upload the changes (and frees the temporary surface)! */

			for (int a = 0; a < rs.size(); a++) {
				std::cout << "r[" << a << "].x = " << rs[a].x << std::endl;
				std::cout << "r[" << a << "].y = " << rs[a].y << std::endl;
			}
		}
	}
	
};

