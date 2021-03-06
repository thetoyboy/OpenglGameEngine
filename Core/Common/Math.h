//////////////////////////////////////////////////////////////
//                      Math                                //
//this is Math library that contains nearly all operation   //
//applied on Vertices and Matrices                          //
//                                                          //
//////////////////////////////////////////////////////////////
#ifndef PFE_MATH_H_
#define PFE_MATH_H_

#include "BasePrimitiveTypes.h"
#include <math.h>
#include <stdio.h>

#ifndef M_PI
    #define M_PI		3.14159265358979323846
#endif
#ifndef M_PI_2
    #define M_PI_2		1.57079632679489661923
#endif
#ifndef M_PI_4
    #define M_PI_4		0.78539816339744830962
#endif

#define MAX(a,b) (a>b)?a:b
#define MIN(a,b) (a<b)?a:b


typedef struct {
	_float x, y, z, w;
} Vertex4d, Point4d, Normal4d;
typedef struct {
    _float x,y,z ;
} Vertex3d,Point3d,Normal3d ;
typedef struct {
    _float u,v ;
} Vertex2d,Point2d,Normal2d ;

inline _float frand_a_b(_float a, _float b) {
	return (rand() / (_float)RAND_MAX) * (b - a) + a;
};


inline _float Magnitude3d(Vertex3d Ver){
	_float tmp = Ver.x*Ver.x + Ver.y*Ver.y + Ver.z*Ver.z;
    return (_float)sqrt(tmp);
};
inline Vertex3d getVertex3d(Vertex3d point1,Vertex3d point2){
	Vertex3d Result;
	Result.x = point2.x - point1.x;
	Result.y = point2.y - point1.y;
	Result.z = point2.z - point1.z;
	return Result;
}
inline _float Distance3d(Vertex3d point1,Vertex3d point2){
    return Magnitude3d(getVertex3d(point1,point2));
}
inline Vertex3d ScaleVertex3d(Vertex3d Ver,_float s){
	Vertex3d Result;
	Result.x = Ver.x*s;
	Result.y = Ver.y*s;
	Result.z = Ver.z*s;
	return Result;
};
inline _float DotProduct3d(Vertex3d Ver1 ,Vertex3d Ver2){
    return Ver1.x*Ver2.x+Ver1.y*Ver2.y+Ver1.z*Ver2.z;
};
inline Vertex3d CrossProduct3d(Vertex3d Left,Vertex3d Right){

	return { Left.y * Right.z - Right.y * Left.z,
		Left.z * Right.x - Right.z * Left.x,
		Left.x * Right.y - Right.x * Left.y };
};
inline Vertex3d Normalize3d(Vertex3d Ver){
    _float d=Magnitude3d(Ver);
	if (d == 0.0f) {
		return Ver;
	}
    return ScaleVertex3d(Ver,1.0f/d);
};
inline Vertex3d AddVertex3d(Vertex3d Ver1 ,Vertex3d Ver2){
	Vertex3d Result;
	Result.x = Ver1.x + Ver2.x;
	Result.y = Ver1.y + Ver2.y;
	Result.z = Ver1.z + Ver2.z;
	return Result;
};
inline Vertex3d SubsVertex3d(Vertex3d Ver1,Vertex3d Ver2){
	Vertex3d Result;
	Result.x = Ver1.x - Ver2.x;
	Result.y = Ver1.y - Ver2.y;
	Result.z = Ver1.z - Ver2.z;
	return Result;
};
inline Vertex3d Rotate3d(Vertex3d Ver,Vertex3d Pers,_float  ang ){
    Vertex3d Result ;
	Pers = Normalize3d(Pers);
	_float MC = 1.0f - (_float)cos(ang), S = (_float)sin(ang), C = (_float)cos(ang);
	_float	DP = (Ver.x*Pers.x + Ver.y*Pers.y + Ver.z*Pers.z);
	Result.x = MC*(Pers.x*DP) + S*(Ver.z*Pers.y - Ver.y*Pers.z) + C*Ver.x;
	Result.y = MC*(Pers.y*DP) + S*(Ver.x*Pers.z - Ver.z*Pers.x) + C*Ver.y;
	Result.z = MC*(Pers.z*DP) + S*(Ver.y*Pers.x - Ver.x*Pers.y) + C*Ver.z;
	return Result;
};
inline void Multi4x4Mtx(_float* leftMtx, _float* rightMtx,_float* resultMtx){
        resultMtx[0] =leftMtx[0]*rightMtx[0]+leftMtx[4]*rightMtx[1]+leftMtx[8]*rightMtx[2] +leftMtx[12]*rightMtx[3];
        resultMtx[1] =leftMtx[1]*rightMtx[0]+ leftMtx[5]*rightMtx[1]+leftMtx[9]*rightMtx[2] +leftMtx[13]*rightMtx[3];
        resultMtx[2] =leftMtx[2]*rightMtx[0]+ leftMtx[6]*rightMtx[1]+leftMtx[10]*rightMtx[2]+leftMtx[14]*rightMtx[3];
        resultMtx[3] =leftMtx[3]*rightMtx[0]+ leftMtx[7]*rightMtx[1]+leftMtx[11]*rightMtx[2]+leftMtx[15]*rightMtx[3];
        resultMtx[4] =leftMtx[0]*rightMtx[4]+ leftMtx[4]*rightMtx[5]+leftMtx[8]*rightMtx[6] +leftMtx[12]*rightMtx[7];
        resultMtx[5] =leftMtx[1]*rightMtx[4]+ leftMtx[5]*rightMtx[5]+leftMtx[9]*rightMtx[6] +leftMtx[13]*rightMtx[7];
        resultMtx[6] =leftMtx[2]*rightMtx[4]+ leftMtx[6]*rightMtx[5]+leftMtx[10]*rightMtx[6]+leftMtx[14]*rightMtx[7];
        resultMtx[7] =leftMtx[3]*rightMtx[4]+ leftMtx[7]*rightMtx[5]+leftMtx[11]*rightMtx[6]+leftMtx[15]*rightMtx[7];
        resultMtx[8] =leftMtx[0]*rightMtx[8]+ leftMtx[4]*rightMtx[9]+leftMtx[8]*rightMtx[10]+leftMtx[12]*rightMtx[11];
        resultMtx[9] =leftMtx[1]*rightMtx[8]+ leftMtx[5]*rightMtx[9]+leftMtx[9]*rightMtx[10]+leftMtx[13]*rightMtx[11];
        resultMtx[10]=leftMtx[2]*rightMtx[8]+ leftMtx[6]*rightMtx[9]+leftMtx[10]*rightMtx[10]+leftMtx[14]*rightMtx[11];
        resultMtx[11]=leftMtx[3]*rightMtx[8]+ leftMtx[7]*rightMtx[9]+leftMtx[11]*rightMtx[10]+leftMtx[15]*rightMtx[11];
        resultMtx[12]=leftMtx[0]*rightMtx[12]+leftMtx[4]*rightMtx[13]+leftMtx[8]*rightMtx[14]+leftMtx[12]*rightMtx[15];
        resultMtx[13]=leftMtx[1]*rightMtx[12]+leftMtx[5]*rightMtx[13]+leftMtx[9]*rightMtx[14]+leftMtx[13]*rightMtx[15];
        resultMtx[14]=leftMtx[2]*rightMtx[12]+leftMtx[6]*rightMtx[13]+leftMtx[10]*rightMtx[14]+leftMtx[14]*rightMtx[15];
        resultMtx[15]=leftMtx[3]*rightMtx[12]+leftMtx[7]*rightMtx[13]+leftMtx[11]*rightMtx[14]+leftMtx[15]*rightMtx[15];
};
inline Vertex3d Mtx_x_Vertex(_float* M4x4,Vertex3d ver){
    Vertex3d rslt;
    rslt.x=M4x4[0]*ver.x+M4x4[4]*ver.y+M4x4[8]*ver.z+M4x4[12];
    rslt.y=M4x4[1]*ver.x+M4x4[5]*ver.y+M4x4[9]*ver.z+M4x4[13];
    rslt.z=M4x4[2]*ver.x+M4x4[6]*ver.y+M4x4[10]*ver.z+M4x4[14];
    return rslt ;
};
inline bool OppositeDirection(Vertex3d ver1,Vertex3d ver2){
    ver1=Normalize3d(ver1);
    ver2=Normalize3d(ver2);
    Vertex3d ver=SubsVertex3d(ver1,ver2);
    if(ver.x==0.0f && ver.y==0.0f && ver.z==0.0f )
        return true ;
    else
        return false;
}
inline _float DegreetoRadius(_float Degree)
{
	return (_float)(Degree* M_PI / 180.0f);
}
inline _float RadiustoDegree(_float Radius)
{
	return(_float)(Radius*180.0 / M_PI);
}
inline void FillViewMatrix(Vertex3d Position, Vertex3d Direction, Vertex3d Up,_float* Dest)
{
	Vertex3d	Forward = Direction; 
	Vertex3d    Side = Normalize3d(CrossProduct3d(Forward, Up));
	Vertex3d	Upward = Normalize3d(CrossProduct3d(Side, Direction));
	Dest[0] = Side.x;		Dest[4] = Side.y;			Dest[8]  = Side.z;			Dest[12] = -DotProduct3d(Side, Position);
	Dest[1] = Upward.x;		Dest[5] = Upward.y;			Dest[9]  = Upward.z;		Dest[13] = -DotProduct3d(Upward, Position);
	Dest[2] = -Forward.x;	Dest[6] = -Forward.y;		Dest[10] = -Forward.z;		Dest[14] = DotProduct3d(Forward, Position);
	Dest[3] = 0.0f;			Dest[7] = 0.0f;				Dest[11] = 0.0f;			Dest[15] = 1.0f;
}
inline void FillPersPectiveMatrix(_float FOV, _float Aspect , _float Near, _float Far , _float* Dest)
{
	_float tanHalfFovy = (_float)tan(FOV / 2.0f);

	Dest[1] = Dest[2] = Dest[3] = 0.0f;
	Dest[4] = Dest[6] = Dest[7] = 0.0f;
	Dest[8] = Dest[9] = 0.0f;
	Dest[12] = Dest[13] = Dest[15] = 0.0f;

	Dest[0] = 1.0f / (Aspect * tanHalfFovy);
	Dest[5] = 1.0f / (tanHalfFovy);
	Dest[11] = -1;

	Dest[10] = -(Far + Near) / (Far - Near);
	Dest[14] = -(2.0f * Far * Near) / (Far - Near);

}
inline void FillOrthographicsMatrix(_float left, _float top, _float right, _float bottom, _float Near, _float Far, _float* Dest)
{


	Dest[0] =  2.0f / (right - left);
	Dest[5] =  2.0f / (top - bottom);
	Dest[10]= -2.0f / (Far - Near);
	Dest[12] = -(right + left) / (right - left);
	Dest[13] = -(top + bottom) / (top - bottom);
	Dest[14] = -(Far+ Near) / (Far - Near);

	Dest[1] = Dest[2] =  Dest[3] =  Dest[4] =  Dest[6] =0.0f ;
	Dest[7] = Dest[8] = Dest[9] = Dest[11] = 0.0f;
	Dest[15] = 1.0f;
}


inline Vertex4d getQuaternionFromMatrix(const _float Matrix[16]) {
	Vertex4d rslt = { 0.0f,0.0f,0.0f,0.0f };

	float trace = Matrix[0] + Matrix[5] + Matrix[10]; // I removed + 1.0f; see discussion with Ethan
	if (trace > 0) {// I changed M_EPSILON to 0
		float s = 0.5f / sqrtf(trace + 1.0f);
		rslt.w = 0.25f / s;
		rslt.x = (Matrix[6] - Matrix[9]) * s;
		rslt.y = (Matrix[8] - Matrix[2]) * s;
		rslt.z = (Matrix[1] - Matrix[4]) * s;
	}
	else {
		if (Matrix[0] > Matrix[5] && Matrix[0] > Matrix[10]) {
			float s = 2.0f * sqrtf(1.0f + Matrix[0] - Matrix[5] - Matrix[10]);
			rslt.w = (Matrix[6] - Matrix[9]) / s;
			rslt.x = 0.25f * s;
			rslt.y = (Matrix[4] + Matrix[1]) / s;
			rslt.z = (Matrix[8] + Matrix[2]) / s;
		}
		else if (Matrix[5] > Matrix[10]) {
			float s = 2.0f * sqrtf(1.0f + Matrix[5] - Matrix[0] - Matrix[10]);
			rslt.w = (Matrix[8] - Matrix[2]) / s;
			rslt.x = (Matrix[4] + Matrix[1]) / s;
			rslt.y = 0.25f * s;
			rslt.z = (Matrix[9] + Matrix[6]) / s;
		}
		else {
			float s = 2.0f * sqrtf(1.0f + Matrix[10] - Matrix[0] - Matrix[5]);
			rslt.w = (Matrix[1] - Matrix[4]) / s;
			rslt.x = (Matrix[8] + Matrix[2]) / s;
			rslt.y = (Matrix[9] + Matrix[6]) / s;
			rslt.z = 0.25f * s;
		}
	}
	return rslt;
}

inline void getQuaternionToMatrix(Vertex4d q, _float matrix[16] ) {
	matrix[0] = 1 - 2 * q.y*q.y - 2 * q.z*q.z;	matrix[4] = 2 * q.x*q.y - 2 * q.z*q.w;		matrix[8]  = 2 * q.x*q.z + 2 * q.y*q.w;
	matrix[1] = 2 * q.x*q.y + 2 * q.z*q.w;		matrix[5] = 1 - 2 * q.x*q.x - 2 * q.z*q.z;	matrix[9]  = 2 * q.y*q.z - 2 * q.x*q.w;
	matrix[2] = 2 * q.x*q.z - 2 * q.y*q.w;		matrix[6] = 2 * q.y*q.z + 2 * q.x*q.w;		matrix[10] = 1 - 2 * q.x*q.x - 2 * q.y*q.y;
}
#endif // PFE_MATH_H_

