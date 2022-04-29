/*
 * Math.hpp
 *
 *  Created on: Apr 29, 2022
 *      Author: filebin
 */

#pragma once
#include "../stdafx.hpp"
#include "../Core.hpp"

NSP_BETTERCPP_BEGIN

better_static_class(Math) {
public:
	constexpr static double PI = 3.14159265359;

	template<typename T> static T abs(T x) { return x < 0 ? -x : x; }
	template<typename T> static T max(T a, T b) { return a > b ? a : b;}
	template<typename T> static T min(T a, T b) { return a < b ? a : b; }
	template<typename T> static T clamp(T v, T a, T b) { return min(max(v, a), b); }
	template<typename T> static T clamp01(T v) { return clamp<T>(v, 0, 1); }
	template<typename T> static T lerp(T a, T b, T t) { return a * (1 - t) + b * t; }

	static ulong hash(ulong x);
	static ulong hash(ulong x, ulong y);
	static ulong hash(ulong x, ulong y, ulong z);
	static ulong hash(ulong x, ulong y, ulong z, ulong w);

	static double asinDeg(double x);
	static double acosDeg(double x);
	static double atanDeg(double x);
	static double atanDeg(double y, double x);

	static double sinDeg(double x);
	static double cosDeg(double x);
	static double tanDeg(double x);

	static double asin(double x);
	static double acos(double x);
	static double atan(double x);
	static double atan(double y, double x);

	static double sin(double x);
	static double cos(double x);
	static double tan(double x);

	static double toRadians(double x);
	static double toDegrees(double x);

	static double random(double min = 0., double max = 1.);

	static double qrsqrt(double x);
	static float qrsqrt(float x);
};

NSP_BETTERCPP_END
