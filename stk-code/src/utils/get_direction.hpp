#include <iostream>
#include <cmath>


float degreesToRadians_custom(float degrees) {
    return degrees * (M_PI / 180.0);
}


float radiansToDegrees_custom(float radians) {
    return radians * (180.0 / M_PI);
}


float dotProduct_custom(float x1, float z1, float x2, float z2) {
    return x1 * x2 + z1 * z2;
}


float magnitude_custom(float x, float z) {
    return std::sqrt(x * x + z * z);
}

// Function to determine the direction of the aim
int aimDirection(float kartX, float kartZ, float frontX, float frontZ, float aimX, float aimZ, float deg_low, float deg_high) {

    float d_kX = frontX - kartX;
    float d_kZ = frontZ - kartZ;
    float d_aX = aimX - kartX;
    float d_aZ = aimZ - kartZ;

    float dot_product = dotProduct_custom(d_kX, d_kZ, d_aX, d_aZ);
    float mag_d_k = magnitude_custom(d_kX, d_kZ);
    float mag_d_a = magnitude_custom(d_aX, d_aZ);

    float cos_theta = dot_product / (mag_d_k * mag_d_a);

    float theta_rad = std::acos(cos_theta);

    float theta_deg = radiansToDegrees_custom(theta_rad);

    if (theta_deg <= deg_low) {
        return 0; // Aiming correctly
    } else if (theta_deg <= deg_high) {
        if (d_kX * d_aZ - d_kZ * d_aX > 0) {
            //std::cout << "Slightly to the right" << std::endl;
            return 1; // Slightly to the right
        } else {
            //std::cout << "Slightly to the left" << std::endl;
            return 2; // Slightly to the left
        }
    } else {
        if (d_kX * d_aZ - d_kZ * d_aX > 0) {
            //std::cout << "Too much to the right" << std::endl;
            return 3; // Too much to the right
        } else {
            //std::cout << "Too much to the left" << std::endl;
            return 4; // Too much to the left
        }
    }
}