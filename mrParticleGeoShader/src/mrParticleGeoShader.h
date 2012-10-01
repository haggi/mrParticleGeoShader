#ifndef MR_PARTICLE_GEO_H
#define MR_PARTICLE_GEO_H

#include <Partio.h>

#include "shader.h"
#include "geoshader.h"

#define VERSION "0.01"

struct mrParticleGeoShader_paras{
	int			geometryType; // "PolySprites:NativeParticles" 
	miScalar	sizeMultiplier; 
	miScalar	minPixelSize;
	miScalar	maxPixelSize;
	miScalar	density;
	int			frameOffset;
	miScalar	seqScale;
	int			minSizeBehaviour;
	miScalar	size;
	miScalar	sizeVariation;

	int			i_particleFiles;
	int			n_particleFiles;
	miTag		particleFiles[1];

	miScalar dummy; // for per frame triggering in maya
};

#endif