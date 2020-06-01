#ifndef NormalizeNumbers_H
#define NormalizeNumbers_H

inline long normalize_long(long value, long min, long max) {
	if(value < min)
		return min;
	if(value > max)
		return max;
	return value;
}
inline double normalize_double(double value, double min, double max) {
	if(value < min)
		return min;
	if(value > max)
		return max;
	return value;
}

#endif