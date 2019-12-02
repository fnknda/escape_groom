#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <stdlib.h>
#include <time.h>

#include <sys/types.h>	// Nothing yet?
#include <sys/socket.h>	// socket() and bind()
#include <netdb.h>	// addrinfo and related stuff
#include <unistd.h>	// gethostname()

#include <arpa/inet.h>	  // inet_ntop and pton
#include <netinet/in.h>	 // Nothing yet?

#include <ncurses.h>
#include <pthread.h>

#include <vector>
#include <math.h>

#include "infos.hpp"

#define INTA 7
#define INTB 2

int t = 0;

char map_message[1024];

constexpr double PI = 3.14159;

double speed = 0.0005d;

int row, col;

std::vector<std::string> map;

struct Player
{
	double fov = PI/3.d;
	double pos_x, pos_y, angle = 0;
	uint8_t keymap = 0;
};

Player p;

void QuitProgram(int n)
{
	std::cout << "Error! Exit code: " << n << std::endl;
	exit(n);
}

void show_message(std::string message)
{
	clear();
	mvprintw(0, 0, message.c_str());
	getch();
}

void Init()
{
	map.clear();
	srand(time(nullptr));
	int map_id = rand() % 10;
	std::ifstream file;
	file.open("map" + itos(map_id) + ".txt");

	file.getline(map_message, 1024);

	int pos_x = 0, pos_y = 0;
	char buff;
	std::string s_buff;
	while(!file.eof())
	{
		file >> std::noskipws >> buff;
		if(buff == '\n')
		{
			pos_y++;
			pos_x = 0;
			map.push_back(s_buff);
			s_buff = "";
		} else if(buff == 'S')
		{
			p.pos_x = pos_x + 0.5d;
			p.pos_y = pos_y + 0.5d;
			p.angle = -PI/2;
			pos_x++;
			s_buff += '.';
		} else if(buff == '#' || buff == '.' || buff == 'A' || buff == 'B' || buff == 'C')
		{
			pos_x++;
			s_buff += buff;
		} else
			break;
	}

	file.close();
	getmaxyx(stdscr,row,col);
	show_message("A essencia do COMPETEC trata-se do trabalho em equipe, ela esta presente em todas as partes do projeto.\nOs tutores das sessoes do competec sao separados em duas funcoes, a primeira seria o(a) mestre de sala, aquele(a) que explica o assunto da aula, enquanto o outro seria o(a) assistente, aquele(a) que tira as duvidas dos alunos e ajuda o mestre da sala em seu dever.\nOs alunos tambem sao incentivados ao trabalho em equipe, atraves da dinamica de dojo de programacao, sendo alocados nas funcoes de piloto(a) e co-piloto(a), um digita o codigo, enquanto o outro ajuda a guiar o caminho para resolver o problema.\nNeste enigma, voces deverao fazer o mesmo, um devera ser os olhos, enquanto o outro sera as pernas, juntos, deverao caminhar rumo a vitoria.\nAperte [Enter] para continuar...");
}

void *update(void *null)
{
	getmaxyx(stdscr,row,col);
	double late_pos_x, late_pos_y, angle_gap = p.fov/col;
	while(!(p.keymap & QUIT))
	{
		if(p.keymap & RSET)
			Init();
		else
		{
			late_pos_x = p.pos_x;
			late_pos_y = p.pos_y;

			if(p.keymap & LK_L)
			p.angle -= speed;
			if(p.keymap & LK_R)
			p.angle += speed;

			if(p.keymap & MV_F)
			{
				p.pos_x += speed * cos(p.angle);
				p.pos_y += speed * sin(p.angle);
			}
			if(p.keymap & MV_B)
			{
				p.pos_x -= speed * cos(p.angle);
				p.pos_y -= speed * sin(p.angle);
			}
			if(p.keymap & MV_L)
			{
				p.pos_x += speed * sin(p.angle);
				p.pos_y -= speed * cos(p.angle);
			}
			if(p.keymap & MV_R)
			{
				p.pos_x -= speed * sin(p.angle);
				p.pos_y += speed * cos(p.angle);
			}

			if(map[p.pos_y][p.pos_x] == '#')
			{
				if(map[p.pos_y][late_pos_x] == '#')
					p.pos_y = late_pos_y;
				if(map[late_pos_y][p.pos_x] == '#')
					p.pos_x = late_pos_x;
			} else if(map[late_pos_y][p.pos_x] == 'A')
			{
				show_message("Numero 1: " + itos(INTA) + itos(t));
				p.pos_x = late_pos_x;
				p.pos_y = late_pos_y;
				t++;
			} else if(map[p.pos_y][p.pos_x] == 'B')
			{
				show_message("Numero 2: " + itos(INTB) + itos(t));
				p.pos_x = late_pos_x;
				p.pos_y = late_pos_y;
				t++;
			} else if(map[p.pos_y][p.pos_x] == 'C')
			{
				show_message(map_message);
				p.pos_x = late_pos_x;
				p.pos_y = late_pos_y;
			}
		}

		erase();

		double current_drawing_angle = p.angle - (p.fov/2);
		int def_y = row/2;
		for(int x = 0; x < col; x++)
		{
			double crr_x = p.pos_x, crr_y = p.pos_y, distance = 0;
			while(distance < 7 && map[crr_y][crr_x] != '#' && map[crr_y][crr_x] != 'A' && map[crr_y][crr_x] != 'B' && map[crr_y][crr_x] != 'C')
			{
				distance += 0.1d;
				crr_x += 0.1d * cos(current_drawing_angle);
				crr_y += 0.1d * sin(current_drawing_angle);
			}
			if(distance == 0) distance = 0.1d;

			int color;
			if(distance < 0.7) color = 1;
			else if(distance < 1.5) color = 2;
			else if(distance < 3) color = 3;
			else if(distance < 7) color = 4;
			else color = 5;
			if(map[crr_y][crr_x] != '#') color += 5;
			attron(COLOR_PAIR(color));
			for(int y = 0; y < (row/distance); y++)
			{
				mvaddch(def_y + y, x, ' ');
				mvaddch(def_y - y, x, ' ');
			}
			attroff(COLOR_PAIR(color));

			current_drawing_angle += angle_gap;
		}

		for(int y = 0; y < map.size(); y++)
			for(int x = 0; x < map[0].length(); x++)
			{
				if(map[y][x] != '#' && map[y][x] != '.')
				{
					attron(COLOR_PAIR(11));
					mvaddch(y, x, map[y][x]);
					attroff(COLOR_PAIR(11));
				} else
					mvaddch(y, x, map[y][x]);
			}

		mvaddch(p.pos_y, p.pos_x, '@');
		refresh();
	}
	pthread_exit(nullptr);
}

int main(int argc, char *argv[])
{
	Init();

	std::string my_port = "42000";

	if(argc == 2)
	{
		my_port = argv[1];
	}

	int status, sock, c_sock;
	socklen_t addr_size;
	struct sockaddr_storage c_addr;
	struct addrinfo hint, *res;

	memset(&hint, 0, sizeof(hint));
	hint.ai_family = AF_UNSPEC;
	hint.ai_socktype = SOCK_STREAM;
	hint.ai_flags = AI_PASSIVE;

	if((status = getaddrinfo(NULL, my_port.c_str(), &hint, &res)) != 0) QuitProgram(1);
	if((sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == -1) QuitProgram(2);
	if((status = bind(sock, res->ai_addr, res->ai_addrlen)) == -1) QuitProgram(3);
	if((status = listen(sock, BACKLOG)) == -1) QuitProgram(4);

	char ipstr[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, res->ai_addr, ipstr, INET_ADDRSTRLEN);
	std::cout << "Listening for connections on address: " << ipstr << ":" << my_port << std::endl;

	addr_size = sizeof(c_addr);
	c_sock = accept(sock, (struct sockaddr *)&c_addr, &addr_size);

	initscr();
	noecho();
	start_color();
	curs_set(0);

	init_color(21, 750, 750, 750);
	init_color(22, 500, 500, 500);
	init_color(23, 250, 250, 250);

	init_color(24, 1000, 1000, 500);
	init_color(25, 750, 750, 375);
	init_color(26, 500, 500, 250);
	init_color(27, 250, 250, 125);

	init_pair(1, COLOR_WHITE, COLOR_WHITE);
	init_pair(2, 21, 21);
	init_pair(3, 22, 22);
	init_pair(4, 23, 23);
	init_pair(5, COLOR_BLACK, COLOR_BLACK);

	init_pair(6, 24, 24);
	init_pair(7, 25, 25);
	init_pair(8, 26, 26);
	init_pair(9, 27, 27);
	init_pair(10, COLOR_BLACK, COLOR_BLACK);

	init_pair(11, COLOR_BLACK, 24);

	pthread_t update_thread;
	pid_t pid = pthread_create(&update_thread, nullptr, update, nullptr);

	while(!(p.keymap & QUIT))
	{
		recv(c_sock, &p.keymap, 1, 0);
	}

	pthread_join(update_thread, nullptr);

	endwin();

	shutdown(sock, 2);
	freeaddrinfo(res);

	pthread_exit(nullptr);
}
