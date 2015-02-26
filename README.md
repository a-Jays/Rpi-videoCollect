# Rpi-videoCollect

`frames.cpp` collects frames in a buffer and writes to disk in a separate thread. Main thread is the same as frame collection thread.

`gps_logger.cpp` runs in a separate thread by itself.

Frames are written only when a GPS lock is acquired. The acquision keeps running though (in the hope of avoiding the few initial frames when the camera is adjusting). There's an also an additional delay (of 5 seconds) before it even starts the camera loop. Set Wiring's pin 11 (hardware pin 26) `high` to stop the code and shutdown. Remember that `high` is 3.3V here for RPi.

Be wary of CPU overloads. Two threads will be running all the time; a third one is launched periodically (to write).

TODO:
Check CPU loads.

*Requires `wiringPi` installed. Compile with `-lwiringPi`.*

*Requires C++11. Compile with `-std=c++0x`.*

Compile only `main.cpp` into an executable. Consider enabling `O3` (or equivalent) -level optimisations for performance.
Run with `sudo` level permissions (required by Wiring).
