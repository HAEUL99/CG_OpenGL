#include "Camera.h"

void Camera::set(Vec3f _eye, Vec3f _lookat, Vec3f vup, float _fov) {
    eye = _eye; lookat = _lookat; fov = _fov;
    Vec3f w = eye - lookat;
    float windowSize = sqrtf(w * w) * tanf(fov / 2);
    Vec3f rightcross = cross(vup, w);
    Vec3f upcross = cross(w,right);
    Vec3f rightnorm =  cross(vup, w) * (1/ sqrtf( cross(vup, w) *  cross(vup, w)));
    Vec3f upnorm = cross(w,right) * (1/ sqrtf( cross(w,right) * cross(w,right)));
    right.x = rightnorm.x * windowSize;
    right.y = rightnorm.y * windowSize;
    right.z = rightnorm.z * windowSize;
    up.x = upnorm.x * windowSize;
    up.y = upnorm.y * windowSize;
    up.z = upnorm.z * windowSize;
}

Ray Camera::getRay(int X, int Y) {
    //vec3 dir = lookat + right * (2 * (X + 0.5f) / windowWidth - 1) + up * (2 * (Y + 0.5f) / windowHeight - 1) - eye;
    Vec3f dir = lookat + right * (2 * (X + 0.5f) / WINDOW_WIDTH - 1) + up * (2 * (Y + 0.5f) / WINDOW_HEIGHT - 1) - eye;
    return Ray(eye, dir);
}
