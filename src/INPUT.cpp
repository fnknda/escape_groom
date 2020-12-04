#include <iostream>
#include <cstring>

#include <sys/socket.h>
#include <arpa/inet.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "infos.hpp"

uint8_t keymap = 0, last_keymap;

void QuitProgram(int n)
{
	std::cout << "Error! Exit code: " << n << std::endl;
	exit(n);
}

SDL_Window *g_window = nullptr;
SDL_Renderer *g_renderer = nullptr;

SDL_Texture
*Key_Q_Black = nullptr,
*Key_W_Black = nullptr,
*Key_E_Black = nullptr,
*Key_R_Black = nullptr,
*Key_A_Black = nullptr,
*Key_S_Black = nullptr,
*Key_D_Black = nullptr,
*Key_Q_Green = nullptr,
*Key_W_Green = nullptr,
*Key_E_Green = nullptr,
*Key_R_Green = nullptr,
*Key_A_Green = nullptr,
*Key_S_Green = nullptr,
*Key_D_Green = nullptr,
*Key_Q = nullptr,
*Key_W = nullptr,
*Key_E = nullptr,
*Key_R = nullptr,
*Key_A = nullptr,
*Key_S = nullptr,
*Key_D = nullptr;


int size = 75;
int offset_x = (SCREEN_WIDTH + 50 + (4 * size)) / 4;
int offset_y = (SCREEN_HEIGHT + 30 + (2 * size)) / 4;
SDL_Rect
*Key_Q_Pos = new SDL_Rect{offset_x + 10, offset_y + 10, size, size},
*Key_W_Pos = new SDL_Rect{offset_x + 20+size, offset_y + 10, size, size},
*Key_E_Pos = new SDL_Rect{offset_x + 30+(2*size), offset_y + 10, size, size},
*Key_R_Pos = new SDL_Rect{offset_x + 40+(3*size), offset_y + 10, size, size},
*Key_A_Pos = new SDL_Rect{offset_x + 10, offset_y + 20+size, size, size},
*Key_S_Pos = new SDL_Rect{offset_x + 20+size, offset_y + 20+size, size, size},
*Key_D_Pos = new SDL_Rect{offset_x + 30+(2*size), offset_y + 20+size, size, size};


SDL_Texture* loadTexture(std::string path)
{
	SDL_Texture* newTexture = nullptr;

	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if(loadedSurface)
	{
		newTexture = SDL_CreateTextureFromSurface( g_renderer, loadedSurface );
		SDL_FreeSurface( loadedSurface );
	}

	return newTexture;
}

bool Init()
{
	bool success = false;
	if(SDL_Init(SDL_INIT_VIDEO) >= 0)
	{
		g_window = SDL_CreateWindow("Escape Room - INPUT", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS | SDL_WINDOW_FULLSCREEN);
		if(g_window)
		{
			g_renderer = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);
			if(g_renderer)
			{
				SDL_SetRenderDrawColor(g_renderer, 0x00, 0x00, 0x00, 0xFF);
				if(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)
				{
					success = true;
				}
			}
		}
	}

	Key_Q_Black = loadTexture("white_q.png");
	Key_W_Black = loadTexture("white_w.png");
	Key_E_Black = loadTexture("white_e.png");
	Key_R_Black = loadTexture("white_r.png");
	Key_A_Black = loadTexture("white_a.png");
	Key_S_Black = loadTexture("white_s.png");
	Key_D_Black = loadTexture("white_d.png");
	Key_Q_Green = loadTexture("green_q.png");
	Key_W_Green = loadTexture("green_w.png");
	Key_E_Green = loadTexture("green_e.png");
	Key_R_Green = loadTexture("green_r.png");
	Key_A_Green = loadTexture("green_a.png");
	Key_S_Green = loadTexture("green_s.png");
	Key_D_Green = loadTexture("green_d.png");
	Key_Q = Key_Q_Black;
	Key_W = Key_W_Black;
	Key_E = Key_E_Black;
	Key_R = Key_R_Black;
	Key_A = Key_A_Black;
	Key_S = Key_S_Black;
	Key_D = Key_D_Black;

	return success;
}

void Close()
{
	SDL_DestroyRenderer(g_renderer);
	g_renderer = nullptr;
	SDL_DestroyWindow(g_window);
	g_window = nullptr;

	IMG_Quit();
	SDL_Quit();
}

int main(int argc, char const *argv[])
{
	const char *ip = "127.0.0.1";
	int port = 42000;
	if(argc > 1)
	{
		ip = argv[1];
		if(argc > 2)
		{
			port = atoi(argv[2]);
		}
	}

	int sock;

	sockaddr_in hint;
	hint.sin_addr.s_addr = inet_addr(ip);
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);

	if((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) QuitProgram(3);
	if(connect(sock, reinterpret_cast<sockaddr *>(&hint), sizeof(hint)) == -1) QuitProgram(4);

	Init();

	SDL_RenderClear(g_renderer);
	SDL_RenderCopy(g_renderer, Key_Q, nullptr, Key_Q_Pos);
	SDL_RenderCopy(g_renderer, Key_W, nullptr, Key_W_Pos);
	SDL_RenderCopy(g_renderer, Key_E, nullptr, Key_E_Pos);
	SDL_RenderCopy(g_renderer, Key_R, nullptr, Key_R_Pos);
	SDL_RenderCopy(g_renderer, Key_A, nullptr, Key_A_Pos);
	SDL_RenderCopy(g_renderer, Key_S, nullptr, Key_S_Pos);
	SDL_RenderCopy(g_renderer, Key_D, nullptr, Key_D_Pos);
	SDL_RenderPresent(g_renderer);

	while(!(keymap & QUIT))
	{
		SDL_Event e;
		while(SDL_PollEvent(&e) != 0)
		{
			if(e.type == SDL_QUIT)
				keymap |= QUIT;
			else if(e.type == SDL_KEYDOWN)
			{
				switch(e.key.keysym.sym)
				{
					case SDLK_w:
						keymap |= MV_F;
						Key_W = Key_W_Green;
						break;
					case SDLK_s:
						keymap |= MV_B;
						Key_S = Key_S_Green;
						break;
					case SDLK_a:
						keymap |= MV_L;
						Key_A = Key_A_Green;
						break;
					case SDLK_d:
						keymap |= MV_R;
						Key_D = Key_D_Green;
						break;
					case SDLK_q:
						keymap |= LK_L;
						Key_Q = Key_Q_Green;
						break;
					case SDLK_e:
						keymap |= LK_R;
						Key_E = Key_E_Green;
						break;
					case SDLK_r:
						keymap |= RSET;
						Key_R = Key_R_Green;
				}
			} else if(e.type == SDL_KEYUP)
			{
				switch(e.key.keysym.sym)
				{
					case SDLK_w:
						keymap &= ~MV_F;
						Key_W = Key_W_Black;
						break;
					case SDLK_s:
						keymap &= ~MV_B;
						Key_S = Key_S_Black;
						break;
					case SDLK_a:
						keymap &= ~MV_L;
						Key_A = Key_A_Black;
						break;
					case SDLK_d:
						keymap &= ~MV_R;
						Key_D = Key_D_Black;
						break;
					case SDLK_q:
						keymap &= ~LK_L;
						Key_Q = Key_Q_Black;
						break;
					case SDLK_e:
						keymap &= ~LK_R;
						Key_E = Key_E_Black;
						break;
					case SDLK_r:
						keymap &= ~RSET;
						Key_R = Key_R_Black;
				}
			}
		}

		if(last_keymap != keymap)
		{
			send(sock, &keymap, 1, 0);
			last_keymap = keymap;

			SDL_RenderClear(g_renderer);

			SDL_RenderCopy(g_renderer, Key_Q, nullptr, Key_Q_Pos);
			SDL_RenderCopy(g_renderer, Key_W, nullptr, Key_W_Pos);
			SDL_RenderCopy(g_renderer, Key_E, nullptr, Key_E_Pos);
			SDL_RenderCopy(g_renderer, Key_R, nullptr, Key_R_Pos);
			SDL_RenderCopy(g_renderer, Key_A, nullptr, Key_A_Pos);
			SDL_RenderCopy(g_renderer, Key_S, nullptr, Key_S_Pos);
			SDL_RenderCopy(g_renderer, Key_D, nullptr, Key_D_Pos);

			SDL_RenderPresent(g_renderer);
		}
	}

	Close();

	return 0;
}
