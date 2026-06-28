#pragma once
#include <stdint.h>
#include <functional>

enum class TouchEvent : uint8_t {
    None,
    Tap,
    LongPress,
    SwipeUp,
    SwipeDown,
    SwipeLeft,
    SwipeRight,
};

struct TouchPoint {
    int16_t x = 0, y = 0;
    bool pressed = false;
};

using TouchCallback = std::function<void(TouchEvent, int16_t x, int16_t y)>;

class TouchManager {
public:
    static TouchManager &instance();

    bool begin();
    void update();

    void onTouch(TouchCallback cb) { _cb = cb; }
    TouchPoint lastPoint() const { return _last; }

private:
    TouchManager() = default;
    void _read();
    void _classify(int16_t dx, int16_t dy, uint32_t ms);

    TouchCallback _cb;
    TouchPoint    _last;
    int16_t       _startX = 0, _startY = 0;
    int16_t       _endX   = 0, _endY   = 0;
    uint32_t      _startMs = 0;
    bool          _tracking = false;
};
