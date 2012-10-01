#ifndef AE_UTILS_H
#define AE_UTILS_H

#include <shader.h>
#include <sstream>

inline std::string stringify(double x)
{
  std::ostringstream out;
  if (!(out << x))
	  return std::string("");
  return out.str();
}

inline std::string stringify(int x)
{
  std::ostringstream out;
  if (!(out << x))
	  return std::string("");
  return out.str();
}

inline void add_vector(miVector& v)
{
	mi_api_vector_xyz_add(&v);
}

inline void add_vector(miScalar x, miScalar y, miScalar z )
{
	miVector v;
	v.x = x;
	v.y = y;
	v.z = z;
	mi_api_vector_xyz_add(&v);
}

inline float rnd(int seed)
{
	srand(seed);
	float rm = (float)RAND_MAX;
	float r  = (float)rand();
	return( r/rm );
}

inline float rnd()
{
	float rm = (float)RAND_MAX;
	float r  = (float)rand();
	return( r/rm );
}

inline float srnd()
{
	float rm = (float)RAND_MAX;
	float r  = (float)rand();
	return( ((r/rm) - 0.5f) * 2.0f );
}


inline std::string tag_to_string(miTag tag)
{
    std::string result;
    if (tag != 0) 
	{
		result = (char*)mi_db_access(tag);
		mi_db_unpin(tag);
    }
    return result;
}

inline void addColor(miColor& a, miColor& b)
{
	a.r += b.r;
	a.g += b.g;
	a.b += b.b;
}

inline void mulColor(miColor& a, miScalar s)
{
	a.r *= s;
	a.g *= s;
	a.b *= s;
}

inline void mulColor(miColor& a, miColor b)
{
	a.r *= b.r;
	a.g *= b.g;
	a.b *= b.b;
}

inline void mulColor(miColor& result, miColor& a, miColor b, miScalar value)
{
	result.r = a.r * (1.0f - value) +  b.r * value;
	result.g = a.g * (1.0f - value) +  b.g * value;
	result.b = a.b * (1.0f - value) +  b.b * value;
}

inline void divColor(miColor& a, miScalar s)
{
	if( s == 0.0f)
		return;
	a.r /= s;
	a.g /= s;
	a.b /= s;
}

inline miScalar colorSum(miColor& a)
{
	return (a.r + a.g + a.b);
}

inline miScalar colorVal(miColor& a)
{
	return colorSum(a)/3.0f;
}

inline bool colorIsBlack(miColor& a)
{
	return ((a.r + a.g + a.b) == 0.0f);
}

inline float radians( miScalar degree)
{
	return degree/180.0f * M_PI;
}

inline float degree( miScalar radians)
{
	return radians/M_PI * 180.0f;
}

inline float linstep( miScalar min, miScalar max, miScalar value)
{
	if( value <= min)
		return 0.0f;
	if( value >= max)
		return 1.0f;

	miScalar newVal = value - min;
	miScalar dist = max - min;
	if( dist == 0.0f)
		return 1.0f;
	return newVal/dist;
}

inline std::string paddedFrameNumber( int frameNumber, int padding)
{
	std::string numberString = stringify(frameNumber);
	size_t numberStringLen = numberString.length();
	
	if( numberStringLen <= padding)
		return numberString;
	
	int zeroLen = padding - numberStringLen;
	std::string zeroString = "";
	for( int i = 0; i < zeroLen; i++)
		zeroString += "0";

	return zeroString + numberString;
}


inline std::string getCorrectFileName(miState *state, mrParticleGeoShader_paras *paras, std::string fileName)
{
	int frameNumber = state->camera->frame;
	int frameOffset = *mi_eval_integer(&paras->frameOffset);
	miScalar seqScale = *mi_eval_scalar(&paras->seqScale);
	frameNumber += frameOffset;
	frameNumber /= seqScale;
	int numDigits = pystring::count(fileName, "#");
	std::string paddedFrameString = paddedFrameNumber(frameNumber, numDigits);
	std::string fnReplace = pystring::replace(fileName, "#", "<>");	
	fnReplace = pystring::replace(fnReplace, "<>", paddedFrameString);	
	return fnReplace;
}
#endif