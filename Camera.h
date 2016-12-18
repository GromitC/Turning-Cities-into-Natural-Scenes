#include "MyHelpers.h"
#include <cmath>

class Camera {

public:
	Camera(int dist, Vec3 at);
	~Camera();

	Vec3 Position();
	Vec3 At();
	Vec3 Up();
	Mat4 View();
	void Update(float x, float y);
	void DeltaDistance(float d);

private:

	Vec3 _pos;
	Vec3 _at;
	Vec3 _up;

	float _pitch = 0;
	float _distance = 0;
	float _yaw = 0;
};
