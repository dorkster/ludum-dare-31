#pragma once

#define DEGTORAD 0.0174532925 

const int MAX_FRAMES_PER_SEC = 60;
const int TILE_SIZE = 16;

class Point {
public:
	int x, y;
	Point() : x(0), y(0) {}
	Point(int _x, int _y) : x(_x), y(_y) {}
};

class FPoint {
public:
	float x, y;
	FPoint(Point _p) : x((float)_p.x), y((float)_p.y) {}
	FPoint() : x(0), y(0) {}
	FPoint(float _x, float _y) : x(_x), y(_y) {}
};

class Rect {
public:
	int x, y, w, h;
	Rect() : x(0), y(0), w(0), h(0) {}
	Rect(SDL_Rect _r) : x(_r.x), y(_r.y), w(_r.w), h(_r.h) {}
	operator SDL_Rect() {
		SDL_Rect r;
		r.x = x;
		r.y = y;
		r.w = w;
		r.h = h;
		return r;
	}
};

class FRect {
public:
	float x, y, w, h;
    FRect(Rect _r) : x((float)_r.x), y((float)_r.y), w((float)_r.w), h((float)_r.h) {}
	FRect() : x(0), y(0), w(0), h(0) {}
	FRect(SDL_Rect _r) : x(_r.x), y(_r.y), w(_r.w), h(_r.h) {}
	operator SDL_Rect() {
		SDL_Rect r;
		r.x = x;
		r.y = y;
		r.w = w;
		r.h = h;
		return r;
	}
};

class Color {
public:
	Uint8 r, g, b, a;
	Color() : r(0), g(0), b(0), a(255) {}
	Color(Uint8 _r, Uint8 _g, Uint8 _b) : r(_r), g(_g), b(_b), a(255) {}
	Color(Uint8 _r, Uint8 _g, Uint8 _b, Uint8 _a) : r(_r), g(_g), b(_b), a(_a) {}
	operator SDL_Color() {
		SDL_Color c;
		c.r = r;
		c.g = g;
		c.b = b;
		c.a = a;
		return c;
	}
	bool operator ==(const Color &other) {
		return r == other.r && g == other.g && b == other.b && a == other.a;
	}
	bool operator !=(const Color &other) {
		return !((*this) == other);
	}
};

float rotate(float angle, float val);
float calcDistance(FPoint p1, FPoint p2);
FPoint calcVector(FPoint pos, float angle, float speed);
bool isWithin(FPoint center, float radius, FPoint target);
bool isWithin(FRect r, FPoint target);

void logInfo(const char* format, ...);
void logError(const char* format, ...);
