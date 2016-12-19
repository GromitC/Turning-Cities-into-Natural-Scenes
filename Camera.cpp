#include "Camera.h"

Camera::Camera(int dist, Vec3 at) {
	_distance = dist;
	_at = at;
	_up = Vec3::Up();

	_pos = Vec3(at.v[0] - dist, at.v[1], at.v[2]);
}

Camera::~Camera() {

}

Vec3 Camera::Position() {
	return _pos;
}

Vec3 Camera::At() {
	return _at;
}

Vec3 Camera::Up() {
	return _up;
}

Mat4 Camera::View() {
	Mat4 ret = LookAt(_pos, _at, _up);
    ret = ret * Rotate(_pitch, Vec3(0, 0, 1));
    ret = ret * Rotate(_yaw, Vec3(0, 1, 0));
    return ret;
}

void Camera::Update(float x, float y) {
	_yaw += x;
    _pitch += y;
}

void Camera::DeltaDistance(float d) {
	_pos.v[0] += d;
}
