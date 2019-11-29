#include <iostream>
#include <cstring>

#include <sys/types.h>	  // Nothing yet?
#include <sys/socket.h>	 // Nothing yet?
#include <netdb.h>	  // addrinfo and related stuff

#include <arpa/inet.h>	  // inet_ntop and pton
#include <netinet/in.h>	 // Nothing yet?

#include <SDL2/SDL.h>

#include "infos.hpp"

uint8_t keymap = 0, last_keymap;

void QuitProgram(int n)
{
	std::cout << "Error! Exit code: " << n << std::endl;
	exit(n);
}

SDL_Window *g_window = nullptr;
SDL_Surface *g_surface = nullptr;

bool Init()
{
	bool success = false;
	if(SDL_Init(SDL_INIT_VIDEO) >= 0)
	{
		g_window = SDL_CreateWindow("Escape Room - INPUT", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if(g_window)
		{
			g_surface = SDL_GetWindowSurface(g_window);
			SDL_FillRect( g_surface, NULL, SDL_MapRGB( g_surface->format, 0x00, 0x00, 0x00 ) );
			SDL_UpdateWindowSurface(g_window);
			success = true;
		}
	}
	return success;
}

void Close()
{
	SDL_DestroyWindow(g_window);
	SDL_Quit();
}

int main(int argc, char const *argv[])
{
	const char *ip = "localhost";
	const char *port = "42000";
	if(argc > 1)
	{
		ip = argv[1];
		if(argc > 2)
		{
			port = argv[2];
		}
	}

	int status, sock;
	struct addrinfo hint, *res;

	memset(&hint, 0, sizeof(hint));
	hint.ai_family = AF_UNSPEC;
	hint.ai_socktype = SOCK_STREAM;

	if((status = getaddrinfo(ip, port, &hint, &res)) != 0) QuitProgram(2);

	if((sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == -1) QuitProgram(3);
	if((status = connect(sock, res->ai_addr, res->ai_addrlen)) == -1) QuitProgram(4);

	char ipstr[INET6_ADDRSTRLEN];
	struct sockaddr_storage *tempaddr;
	getpeername(sock, (struct sockaddr*)tempaddr, (socklen_t *)sizeof(*tempaddr));
	inet_ntop(AF_INET, tempaddr, ipstr, INET6_ADDRSTRLEN);

	Init();

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
						break;
					case SDLK_s:
						keymap |= MV_B;
						break;
					case SDLK_a:
						keymap |= MV_L;
						break;
					case SDLK_d:
						keymap |= MV_R;
						break;
					case SDLK_q:
						keymap |= LK_L;
						break;
					case SDLK_e:
						keymap |= LK_R;
						break;
					case SDLK_r:
						keymap |= RSET;
				}
			} else if(e.type == SDL_KEYUP)
			{
				switch(e.key.keysym.sym)
				{
					case SDLK_w:
						keymap &= ~MV_F;
						break;
					case SDLK_s:
						keymap &= ~MV_B;
						break;
					case SDLK_a:
						keymap &= ~MV_L;
						break;
					case SDLK_d:
						keymap &= ~MV_R;
						break;
					case SDLK_q:
						keymap &= ~LK_L;
						break;
					case SDLK_e:
						keymap &= ~LK_R;
						break;
					case SDLK_r:
						keymap &= ~RSET;
				}
			}
		}

		if(last_keymap != keymap)
		{
			send(sock, &keymap, 1, 0);
			last_keymap = keymap;
		}
	}

	Close();

	shutdown(sock, 2);
	freeaddrinfo(res);
	return 0;
}
