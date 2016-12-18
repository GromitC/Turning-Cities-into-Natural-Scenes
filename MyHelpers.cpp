#include "MyHelpers.h"

Vec3::Vec3(){
  v[0] = 0;
  v[1] = 0;
  v[2] = 0;
}

Vec3::Vec3(float x, float y, float z){
  v[0] = x;
  v[1] = y;
  v[2] = z;
}

float &Vec3::operator[](int i)
{
    if (i < 3)
      return v[i];
    return v[0];
}

void Vec3::print(){
  cout<<"[";
  int i;
  for (i=0;i<3;i++){ printf("%.2f, ", v[i]);
  }
  cout<<"]"<<endl;;
}

Vec3 Vec3::cross(Vec3 rhs){
  float s1, s2, s3;
  s1 = v[1]*rhs.v[2]-v[2]*rhs.v[1];
  s2 = v[2]*rhs.v[0]-v[0]*rhs.v[2];
  s3 = v[0]*rhs.v[1]-v[1]*rhs.v[0];
  Vec3 result(s1, s2, s3);
  return result;
}

float Vec3::dot(Vec3 rhs){
  return v[0]*rhs.v[0]+v[1]*rhs.v[1]+v[2]*rhs.v[2];
}

float Vec3::mag(){
  return sqrt(this->dot(*this));
}

Vec3 Vec3::normal(){
  Vec3 result;
  float mag = this->mag();
  result.v[0] = v[0]/mag;
  result.v[1] = v[1]/mag;
  result.v[2] = v[2]/mag;
  return result;
}

Vec3 Vec3::operator+(const Vec3& rhs) const{
  Vec3 result(v[0]+rhs.v[0],v[1]+rhs.v[1],v[2]+rhs.v[2]);
  return result;
}

Vec3 Vec3::operator-(const Vec3& rhs) const{
  Vec3 result(v[0]-rhs.v[0],v[1]-rhs.v[1],v[2]-rhs.v[2]);
  return result;
}

Vec3 Vec3::Up() {
  return Vec3(0, 1, 0);
}

Vec4::Vec4(){
  v[0] = 0;
  v[1] = 0;
  v[2] = 0;
  v[3] = 0;
}

Vec4::Vec4(float x, float y, float z, float w){
  v[0] = x;
  v[1] = y;
  v[2] = z;
  v[3] = w;
}

Vec3 Vec4::xyz() {
  return Vec3(v[0], v[1], v[2]);
}

float &Vec4::operator[](int i)
{
    if (i < 4)
      return v[i];
    return v[0];
}


void Vec4::print(){
  cout<<"[";
  int i;
  for (i=0;i<4;i++){ printf("%.2f, ", v[i]);
  }
  cout<<"]"<<endl;
}

Vec4& Vec4::operator*=(const Mat4& rhs){
  //NOTE: v *= A returns Av, so A is left multiplied
  int i,k;
  float temp[4] = {0};
  for (i=0;i<4;i++){
    for (k=0;k<4;k++){
      temp[i] += v[k]*rhs.matrix[i+4*k];
    }
  }
  for(i=0;i<4;i++){
    v[i] = temp[i];
  }
  return *this;
}

Mat4::Mat4(){
  int i;
  for (i=0;i<16;i++){
    matrix[i] = 0;
  }
}

Mat4::Mat4(float m[16]){
  int i;
  for (i=0;i<16;i++){
    matrix[i] = m[i];
  }
}
void Mat4::setVal(int i, float x){
  matrix[i] = x;
}

void Mat4::print(){
  int i,j;
  printf("[");
  for (i=0;i<4;i++){
    printf("[");
    for (j=0;j<4;j++){
      printf("%.2f, ", matrix[i+4*j]);
    }
    if (i!=4){
      printf("]\n");
    }
    else if(i == 4){
      printf("]]\n");
    }
  }
}

Mat4 Mat4::operator*(const Mat4& rhs) const{
  Mat4 prod;  
  int i,j,k;
  for (i=0;i<4;i++){
    for (j=0;j<4;j++){
      for (k=0;k<4;k++){
        prod.matrix[i+4*j] += matrix[i+4*k]*rhs.matrix[4*j+k];
      }
    }
  }
  return prod;
}

Mat4& Mat4::operator*=(const Mat4& rhs) {
  float prod[16] = {0};  
  int i,j,k;
  for (i=0;i<4;i++){
    for (j=0;j<4;j++){
      for (k=0;k<4;k++){
        prod[i+4*j] += matrix[i+4*k]*rhs.matrix[4*j+k];
      }
    }
  }
  for (i=0;i<15;i++){
    matrix[i] = prod[i];
  }
  return *this;
}

Vec4 Mat4::operator*(const Vec4& rhs) const{
  int i,k;
  float temp[4] = {0};
  for (i=0;i<4;i++){
    for (k=0;k<4;k++){
      temp[i] += rhs.v[k]*matrix[i+4*k];
    }
  }
  return Vec4(temp[0], temp[1], temp[2], temp[3]);
}

Mat4 Mat4::Identity(){
  Mat4 id;
  id.matrix[0] = 1;
  id.matrix[5] = 1;
  id.matrix[10] = 1;
  id.matrix[15] = 1;
  return id;
}

Mat4 Translate(Vec3 trans){
  Mat4 t;
  t.matrix[0] = 1.0;
  t.matrix[5] = 1.0;
  t.matrix[10] = 1.0;
  t.matrix[15] = 1.0;
  t.matrix[12] = trans.v[0];
  t.matrix[13] = trans.v[1];
  t.matrix[14] = trans.v[2];
  return t;
}

Mat4 Scale(Vec3 scale){
  Mat4 s;
  s.matrix[0] = scale.v[0];
  s.matrix[5] = scale.v[1];
  s.matrix[10] = scale.v[2];
  s.matrix[15] = 1.0;
  return s;
}

Mat4 Rotate(Vec4 rot) {
  return Rotate(rot.v[3], rot.xyz());
}

Mat4 Rotate(float theta, Vec3 rot){
  float x,y,z;
  Vec3 rotNorm = rot.normal();
  x = rotNorm.v[0];
  y = rotNorm.v[1];
  z = rotNorm.v[2];
  Mat4 r;

  float c = cos(theta*PI/180.0); //convert degree to radians
  float s = sin(theta*PI/180.0); 
  r.matrix[0] = c+x*x*(1-c);
  r.matrix[1] = y*x*(1-c)+z*s;
  r.matrix[2] = z*x*(1-c)-y*s;

  r.matrix[4] = x*y*(1-c)-z*s;
  r.matrix[5] = y*y*(1-c)+c;
  r.matrix[6] = y*z*(1-c)+x*s;

  r.matrix[8] = x*z*(1-c)+y*s;
  r.matrix[9] = y*z*(1-c)-x*s;
  r.matrix[10] = z*z*(1-c)+c;

  r.matrix[15] = 1.0;
  return r;
}


Mat4 LookAt(Vec3 eye, Vec3 center, Vec3 up){
  float m[16] = {0};
  Vec3 f, Up, s, u;
  f = (center-eye).normal();
  Up = up.normal();
  s = f.cross(Up);
  u = s.normal().cross(f);

  m[0] = s.v[0];
  m[4] = s.v[1];
  m[8] = s.v[2];
  m[1] = u.v[0];
  m[5] = u.v[1];
  m[9] = u.v[2];
  m[2] = -1*f.v[0];
  m[6] = -1*f.v[1];
  m[10] =-1*f.v[2];
  m[15] = 1;

  Mat4 M =  Mat4(m);
  Vec3 zero; //hacky, should just implement scalar multiplicaton
  Mat4 T = Translate(zero-eye);
  return M*T;
  return M;
}


Mat4 Perspective(float fovy, float aspect, float zNear, float zFar){
  float m[16] = {0};
  float f = 1/tan(fovy * PI / 360);
  m[0] = f/aspect;
  m[5] = f;
  m[10] = (zFar+zNear)/(zNear-zFar);
  m[11] = -1;
  m[14] = 2*zFar*zNear/(zNear-zFar);
  return Mat4(m);
}