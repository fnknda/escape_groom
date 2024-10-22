#include <sstream>

constexpr int BACKLOG = 3;

constexpr int SCREEN_WIDTH = 1366;
constexpr int SCREEN_HEIGHT = 768;

constexpr uint8_t MV_F = 0b00000001;
constexpr uint8_t MV_B = 0b00000010;
constexpr uint8_t MV_L = 0b00000100;
constexpr uint8_t MV_R = 0b00001000;
constexpr uint8_t LK_L = 0b00010000;
constexpr uint8_t LK_R = 0b00100000;
constexpr uint8_t RSET = 0b01000000;
constexpr uint8_t QUIT = 0b10000000;

std::string dtos(double x) {
	std::stringstream s;  // Allocates memory on stack
	s << x;
	return s.str();// returns a s.str() as a string by value
}

std::string itos(int x) {
	std::stringstream s;  // Allocates memory on stack
	s << x;
	return s.str();// returns a s.str() as a string by value
}
