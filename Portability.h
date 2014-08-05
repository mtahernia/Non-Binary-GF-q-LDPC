#ifndef PORTABILITY
#define PORTABILITY

#ifdef VISUAL_C
#include <stdlib.h>
#include <time.h>
#include <float.h>
#else
#include <strings.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#endif

/************************************************************************
 *
 *	VISUAL_C utilities
 *
 ************************************************************************/

#ifdef VISUAL_C

inline char* CharTime(void)
{
	time_t tt = time(NULL);
	return ctime(&tt);
}

inline double atanh(double x)
{
	return 0.5 * log((x-1)/(x+1));
}

inline long min(long l1, long l2)
{
	return (l1 < l2) ? l1 : l2;
}

inline void bzero(void *x, int len)
{
	char *str_x = (char *)x;

	for (int i=0; i<len; i++)
	*str_x++ = 0;
}

inline void bcopy(void *from, void *to, int len)
{
	char *str_to = (char*)to;
	char *str_from = (char*)from;

	if (*str_to > *str_from)
	{
		str_to += len-1;
		str_from += len-1;
		for (int i = 0; i < len; i++)
		*str_to-- = *str_from--;
	}
	else
	{
		for (int i = 0; i < len; i++)
		*str_to++ = *str_from++;
	}
}

inline BOOLEAN isfinite(double &d)
{
	switch(_fpclass(d))
	{
		case _FPCLASS_SNAN:
		case _FPCLASS_QNAN:
		case _FPCLASS_NINF:
		case _FPCLASS_PINF:
		return FALSE;
		break;
		default:
		return TRUE;
	}
}

inline double clip(double &d, double MAXVAL = INF)
{
	switch(_fpclass(d))
	{
		case _FPCLASS_SNAN:
		case _FPCLASS_QNAN:
		d = 0;  // not a number 
		break;
		case _FPCLASS_NINF:
		d = -MAXVAL;
		break;
		case _FPCLASS_PINF:
		d = MAXVAL;
		break;
		default:
		if (d > MAXVAL)
		d = MAXVAL;
		else if (d < -MAXVAL)
		d = -MAXVAL;
//            else if (fabs(d) < EPSILON)
//               d = 0;
		break;
	}
	return d;
}

#endif

/************************************************************************
 *
 *  Unix requirements 
 *
 ************************************************************************/

#ifndef VISUAL_C

inline char* CharTime(void) {
	time_t tt = time(NULL);
	return ctime(&tt);
}

inline double min(double x, double y) {
	return (x < y) ? x : y;
}

inline long min(long x, long y) {
	return (x < y) ? x : y;
}

inline int min(int x, int y) {
	return (x < y) ? x : y;
}

#endif

/************************************************************************
 *
 *  Mixed
 *
 ************************************************************************/

inline unsigned int RandomSeed() {
#ifdef VISUAL_C
	return (unsigned)time( NULL );
#else
	return time(NULL);
#endif
}

inline void my_srand(unsigned int seed) {
#ifdef VISUAL_C
	srand( seed );
#else
	srand48(seed);
#endif
}

inline double my_rand() {
#ifdef VISUAL_C
	return (double)rand() / (double)(RAND_MAX + 1);
#else
	return drand48();
#endif
}

inline double Q(double x) {
#ifndef VISUAL_C
	return 0.5 * erfc(x / sqrt(2.));
#else
	return -x;
#endif
}

/************************************************************************
 *
 *  Linux
 *
 ************************************************************************/

#ifdef LINUX_C

// This function is already defined in my math.h file FIXME: I may need to comment it and adapt the original function
#undef isfinite(x)
inline BOOLEAN isfinite(double d)
{
	if (isnan(d))
	return FALSE;
	else
	{
		switch(isinf(d))
		{
			case -1:
			case 1:
			return FALSE;
			break;
			default:
			return TRUE;
			break;
		}
	}
}

inline double clip(double &d, double MAXVAL = INF)
{
	if (isnan(d))
	d = 0;
	else
	{
		switch(isinf(d))
		{
			case -1:
			d = -MAXVAL;
			break;
			case 1:
			d = MAXVAL;
			break;
			default:
			if (d > MAXVAL)
			d = MAXVAL;
			else if (d < -MAXVAL)
			d = -MAXVAL;
			break;
		}
	}
	return d;
}

#endif

#endif
