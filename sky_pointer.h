/******************************************************************
Author:     David Vazquez Garcia    <davidvazquez.gijon@gmail.com>
            Juan Menendez Blanco    <juanmb@gmail.com>
Version:    2.0
Date:       2016/May/26

This code is part of the SkyPointer project:
    https://github.com/juanmb/skypointer

*******************************************************************************/
#ifndef _SkyPointer_h_
#define _SkyPointer_h_

#include <inttypes.h>
#include "AccelStepper.h"

// Speed
#ifdef DEBUG
#define GOTO_SPEED 100
#define MOVE_SPEED 100
#define HOME_FAST_SPEED 20
#define HOME_SLOW_SPEED 20
#define ACCEL 300
#else
#define GOTO_SPEED 6000
#define MOVE_SPEED 200
#define HOME_FAST_SPEED 400
#define HOME_SLOW_SPEED 20
#define ACCEL 4000
#endif

// Naming the axes (ports)
#define X 0
#define Y 1
#define Z 2
// Motor parameters
#define STEPS 200
#define MICROSTEPS 16
#define USTEPS_REV STEPS*MICROSTEPS
// Directions for rotations
#define FW 0            // Forward
#define BW 1            // Backward
// Maximum absolute rotation
#define SEMIRANGE 0.7 // Turns allowed in each direction
#define SEMIRANGE_USTEPS SEMIRANGE*USTEPS_REV

// CNC Shield pins
#define XSTEP 2
#define YSTEP 3
#define ZSTEP 4
#define XDIR 5
#define YDIR 6
#define ZDIR 7
#define ENABLE 8
#define XSTOP 9
#define YSTOP 10
#define ZSTOP 11
#define SPINDLE_ENABLE 12
#define SPINDLE_DIR 13

// Pin for the photo diode -- Map to ZSTEP
#define PHOTO_PIN ZSTEP
// Pin for the laser -- Map to ZDIR
#define LASER_PIN ZDIR
// Default laser timeout
#define LASER_TIMEOUT  5000 // 5 seconds
// Baud Rate
#define BAUDRATE 115200

// Modulus operator
#define MOD(a, b) ((((a) % (b)) + (b)) % (b))

typedef enum state {
    ST_IDLE,
    ST_HOMING_FW,
    ST_HOMING_BW,
    ST_HOMING_SLOW,
} state;


class SkyPointer {
public:
    SkyPointer(void);
    void init(void);            // Initialize the hardware
    int16_t calcAzDelta(uint16_t);
    int16_t calcAltDelta(uint16_t);
    void run(void);             // Update the position of the motors
    void laser(uint8_t);        // Turn ON/OFF the laser
    uint8_t isLaserOn(void);    // Check the laser state
    void setLaserTimeout(uint32_t);
    void home();                // Find home position
    void move(int16_t az, int16_t alt, uint16_t speed);
    void goTo(uint16_t az, uint16_t alt, uint16_t speed);
    void getPos(uint16_t *az, uint16_t *alt);
    void stop();                // Stop both motors
    void releaseMotors();       // Disable the motor drivers
    void setTimeOn(uint32_t);   // Store elapsed ON time for laser
    uint32_t getTimeOn(void);   // Get elapsed ON time for laser

private:
    uint32_t laserOnTime;  // Elapsed ON time for laser
    uint32_t laserTimeout; // Laser timeout
    AccelStepper azMotor;
    AccelStepper altMotor;
    int16_t absPos;        // Absolute rotation
    state _state;
};

#endif
