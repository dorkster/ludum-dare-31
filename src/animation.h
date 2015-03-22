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


#pragma once

#include "common.h"
#include "utils.h"
#include "render_engine.h"

class Animation {
public:
    Animation();
    ~Animation();
    void setTo(Animation& other);
    void load(const std::string& _filename, int _index, unsigned frames, unsigned duration, int _loops, int frame_w, int frame_h, int offset_x, int offset_y);
    void logic();
    void render();
    void setPos(float x, float y);
    void setAngle(float angle);
    int getTimesPlayed();
    bool isLastFrame();
    bool isFinished();

private:
    Image image;
    std::vector<unsigned> frames;
    std::string filename;
    int index;
    unsigned frame_index;
    int loops;
    int times_played;
    Point frame_size;
    Point frame_offset;
};
