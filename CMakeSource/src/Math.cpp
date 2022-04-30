/*
 * Math.cpp
 *
 *  Created on: Apr 29, 2022
 *      Author: filebin
 */

#include "pch.hpp"

NSP_BETTERCPP_BEGIN

ulong Math::hash(ulong x) {
    x += (x << 20);
    x ^= (x >> 12);
    x += (x << 6);
    x ^= (x >> 22);
    x += (x << 30);
    return x;
}

ulong Math::hash(ulong x, ulong y)  {
    return hash(x ^ hash(y));
}

ulong Math::hash(ulong x, ulong y, ulong z) {
    return hash(x ^ hash(y) ^ hash(z));
}

ulong Math::hash(ulong x, ulong y, ulong z, ulong w) {
    return hash(x ^ hash(y) ^ hash(z) ^ hash(w));
}

double Math::asinDeg(double x) {return toDegrees(::asin(x)); } // @suppress("Invalid arguments") // @suppress("Function cannot be resolved")
double Math::acosDeg(double x) { return toDegrees(::acos(x)); } // @suppress("Invalid arguments") // @suppress("Function cannot be resolved")
double Math::atanDeg(double x) { return toDegrees(::atan(x)); } // @suppress("Invalid arguments") // @suppress("Function cannot be resolved")
double Math::atanDeg(double y, double x) { return toDegrees(::atan2(y, x)); } // @suppress("Invalid arguments") // @suppress("Function cannot be resolved")

double Math::sinDeg(double x) { return ::sin(toRadians(x)); } // @suppress("Function cannot be resolved")
double Math::cosDeg(double x) { return ::cos(toRadians(x)); } // @suppress("Function cannot be resolved")
double Math::tanDeg(double x) { return ::tan(toRadians(x)); } // @suppress("Function cannot be resolved")

double Math::asin(double x) { return ::asin(x); } // @suppress("Function cannot be resolved")
double Math::acos(double x) { return ::acos(x); } // @suppress("Function cannot be resolved")
double Math::atan(double x) { return ::atan(x); } // @suppress("Function cannot be resolved")
double Math::atan(double y, double x) { return ::atan2(y, x); } // @suppress("Function cannot be resolved")

double Math::sin(double x) { return ::sin(x); } // @suppress("Function cannot be resolved")
double Math::cos(double x) { return ::cos(x); } // @suppress("Function cannot be resolved")
double Math::tan(double x) { return ::tan(x); } // @suppress("Function cannot be resolved")

double Math::toRadians(double x) { return x / 180. * PI; }
double Math::toDegrees(double x) { return x * 180. / PI; }

double random(double min, double max) { return ((double) rand() / 0x7fff) * (max - min) + min; } // @suppress("Function cannot be resolved")

double Math::qrsqrt(double x) {
	double xhalf = 0.5 * x;
	int64_t i = *(int64_t*) &x;				   //evil bit hack
	i = 0x5fe6ec85e7de30da - (i >> 1); //what a fuck?
	x = *(double*) &i;

	x *= (1.5 - xhalf * x * x); //1st iteration
	x *= (1.5 - xhalf * x * x); //2nd iteration
	//x *= (1.5 - xhalf * x * x); //3rd iteration
	//x *= (1.5 - xhalf * x * x); //4th iteration

	return x;
}
float Math::qrsqrt(float x) {
	float xhalf = 0.5 * x;
	int32_t i = *(int32_t*) &x;	   //evil bit hack
	i = 0x5f3759df - (i >> 1); //what a fuck?
	x = *(float*) &i;

	x *= (1.5f - xhalf * x * x); //1st iteration
	x *= (1.5f - xhalf * x * x); //2nd iteration
	//x *= (1.5 - xhalf * x * x); //3rd iteration
	//x *= (1.5 - xhalf * x * x); //4th iteration

	return x;
}

NSP_BETTERCPP_END





