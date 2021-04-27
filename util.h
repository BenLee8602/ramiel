#ifndef UTIL_H_
#define UTIL_H_


int range(int num, int min, int max) {
	if (num < min) return min;
	if (num > max) return max;
	return num;
}


#endif
