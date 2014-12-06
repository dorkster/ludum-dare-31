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
