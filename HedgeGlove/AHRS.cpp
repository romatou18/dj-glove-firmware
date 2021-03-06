/*
    AHRS.h
    Author: Seb Madgwick

    Algorithm code adapted from:
    http://www.x-io.co.uk/open-source-imu-and-ahrs-algorithms/
*/

//------------------------------------------------------------------------------
// Includes

#include "AHRS.h"

//------------------------------------------------------------------------------
// Definitions

#define SAMPLE_FREQ 100.0f  // sample frequency in Hz

//------------------------------------------------------------------------------
// Methods

AHRS::AHRS() {
    twoKp = (2.0f * 0.5f);                      // 2 * proportional gain (Kp)
    q0 = 1.0f, q1 = 0.0f, q2 = 0.0f, q3 = 0.0f; // initial quaternion describing Earth relative algined sensor
}

void AHRS::update(float gx, float gy, float gz, float ax, float ay, float az) {
    float recipNorm;
    float halfvx, halfvy, halfvz;
    float halfex, halfey, halfez;
    float qa, qb, qc;

    // Compute feedback only if accelerometer measurement valid (avoids NaN in accelerometer normalisation)
    if(!((ax == 0.0f) && (ay == 0.0f) && (az == 0.0f))) {

        // Normalise accelerometer measurement
        recipNorm = invSqrt(ax * ax + ay * ay + az * az);
        ax *= recipNorm;
        ay *= recipNorm;
        az *= recipNorm;

        // Estimated direction of gravity and vector perpendicular to magnetic flux
        halfvx = q1 * q3 - q0 * q2;
        halfvy = q0 * q1 + q2 * q3;
        halfvz = q0 * q0 - 0.5f + q3 * q3;

        // Error is sum of cross product between estimated and measured direction of gravity
        halfex = (ay * halfvz - az * halfvy);
        halfey = (az * halfvx - ax * halfvz);
        halfez = (ax * halfvy - ay * halfvx);

        // Apply proportional feedback
        gx += twoKp * halfex;
        gy += twoKp * halfey;
        gz += twoKp * halfez;
    }

    // Integrate rate of change of quaternion
    gx *= (0.5f * (1.0f / SAMPLE_FREQ));    // pre-multiply common factors
    gy *= (0.5f * (1.0f / SAMPLE_FREQ));
    gz *= (0.5f * (1.0f / SAMPLE_FREQ));
    qa = q0;
    qb = q1;
    qc = q2;
    q0 += (-qb * gx - qc * gy - q3 * gz);
    q1 += (qa * gx + qc * gz - q3 * gy);
    q2 += (qa * gy - qb * gz + q3 * gx);
    q3 += (qa * gz + qb * gy - qc * gx);

    // Normalise quaternion
    recipNorm = invSqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
    q0 *= recipNorm;
    q1 *= recipNorm;
    q2 *= recipNorm;
    q3 *= recipNorm;
}

float AHRS::invSqrt(float x) {  // fast inverse square-root, see: http://en.wikipedia.org/wiki/Fast_inverse_square_root
    float halfx = 0.5f * x;
    float y = x;
    long i = *(long*)&y;
    i = 0x5f3759df - (i >> 1);
    y = *(float*)&i;
    y = y * (1.5f - (halfx * y * y));
    return y;
}

//------------------------------------------------------------------------------
// End of file
