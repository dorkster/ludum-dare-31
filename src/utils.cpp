/*
Copyright © 2014-2015 Justin Jacobs

This file is part of Noman's Dungeon.

Noman's Dungeon is free software: you can redistribute it and/or modify it under the terms
of the GNU General Public License as published by the Free Software Foundation,
either version 3 of the License, or (at your option) any later version.

Noman's Dungeon is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
Noman's Dungeon.  If not, see http://www.gnu.org/licenses/
*/


#include <stdarg.h>
#include "common.h"
#include "utils.h"

float rotate(float angle, float val) {
    angle += val;

    while (angle > 360.0)
        angle -= 360;
    while (angle < 0.0)
        angle += 360;

    return angle;
}

float calcDistance(FPoint p1, FPoint p2) {
	return sqrt((p2.x-p1.x) * (p2.x-p1.x) + (p2.y-p1.y) * (p2.y-p1.y));
}

FPoint calcVector(FPoint pos, float angle, float speed) {
    FPoint fp;
    fp.x = pos.x + (sin(angle*DEGTORAD)*speed);
    fp.y = pos.y + (-cos(angle*DEGTORAD)*speed);
    return fp;
}

bool isWithin(FPoint center, float radius, FPoint target) {
	return (calcDistance(center, target) < radius);
}

bool isWithin(FRect r, FPoint target) {
	return target.x >= r.x && target.y >= r.y && target.x < r.x+r.w && target.y < r.y+r.h;
}

void logInfo(const char* format, ...) {
	va_list args;

	va_start(args, format);

	SDL_LogMessageV(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, format, args);

	va_end(args);
}

void logError(const char* format, ...) {
	va_list args;

	va_start(args, format);

	SDL_LogMessageV(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, format, args);

	va_end(args);
}

