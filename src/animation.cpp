#include "animation.h"
#include "engines.h"

Animation::Animation()
    : filename("")
    , index(0)
    , frame_index(0)
    , loops(0)
    , times_played(0)
{}

Animation::~Animation() {
}

void Animation::load(const std::string& _filename, int _index, unsigned frame_count, unsigned duration, int _loops, int frame_w, int frame_h, int offset_x, int offset_y)
{
    filename = _filename;
    image.load(render_engine, filename);
    index = _index;
    loops = _loops;
    frame_size.x = frame_w;
    frame_size.y = frame_h;
    frame_offset.x = offset_x;
    frame_offset.y = offset_y;
	
    //
    // Convert frame count and duration into engine frames
    //
    frames.clear();
    duration = (unsigned)(floor(((duration*MAX_FRAMES_PER_SEC) / 1000.f) + 0.5f));
	if (frame_count > 0 && duration % frame_count == 0) {
		const unsigned short divided = duration/frame_count;
		// if we can evenly space frames among the duration, do it
		for (unsigned i=0; i<frame_count; ++i) {
			for (unsigned j=0; j<divided; ++j) {
				frames.push_back(i);
			}
		}
	}
	else {
		// we can't evenly space frames, so we try using Bresenham's line algorithm to lay them out
		int x0 = 0;
		int y0 = 0;
		int x1 = duration-1;
		int y1 = frame_count-1;

		int dx = x1-x0;
		int dy = y1-y0;

		int D= 2*dy - dx;

		frames.push_back(y0);

		int x = x0+1;
		int y = y0;

		while (x<=x1) {
			if (D > 0) {
				y++;
				frames.push_back(y);
				D = D + ((2*dy)-(2*dx));
			}
			else {
				frames.push_back(y);
				D = D + (2*dy);
			}
			x++;
		}
	}

    // go to the first frame
    if (!frames.empty())
        image.setClip(frames[frame_index]*frame_size.x, index*frame_size.y, frame_size.x, frame_size.y);
}

void Animation::logic() {
    if (!frames.empty())
        image.setClip(frames[frame_index]*frame_size.x, index*frame_size.y, frame_size.x, frame_size.y);

    if (loops == 0 || times_played < loops) {
        if (frame_index < frames.size()-1)
            frame_index++;
        else {
            if (loops == 0 || times_played+1 < loops)
                frame_index = 0;
            times_played++;
        }
    }
}

void Animation::render() {
    image.render();
}

void Animation::setPos(float x, float y) {
    image.setPos(x-frame_offset.x, y-frame_offset.y);
}

void Animation::setAngle(float angle) {
    image.setAngle(angle);
}

int Animation::getTimesPlayed() {
    return times_played;
}

bool Animation::isLastFrame() {
    return frame_index == frames.size()-1;
}

bool Animation::isFinished() {
    return loops != 0 && isLastFrame() && times_played == loops;
}
