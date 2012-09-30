#include "mrParticleGeoShader.h"
#include <stdlib.h>
#include <Partio.h>
#include "pystring.h"
//#include "miaux.h"
#include "utils.h"
#include <sstream>


PartioContainer::PartioContainer()
{
	data = NULL;
}

PartioContainer::PartioContainer(std::string fileName)
{
	this->data = NULL;

	this->cacheFileName = fileName;
	this->data = Partio::read(fileName.c_str());
	if( this->data == NULL)
	{
		mi_error("data == NULL, unable to read file %d", fileName.c_str());
		return;
	}

	mi_info("Number of particles %d", data->numParticles());
	mi_info("Particle has %d attributes:", data->numAttributes());
	for(int i=0;i<data->numAttributes();i++)
	{
		Partio::ParticleAttribute attr;
		data->attributeInfo(i,attr);
		mi_info("attribute %d name: %s", i, attr.name.c_str());
	}
}

PartioContainer::~PartioContainer()
{}

bool PartioContainer::good()
{
	return (this->data != NULL);
}


bool PartioContainer::assertAttribute(char *attrName, Partio::ParticleAttribute& attr)
{
	if(!this->data->attributeInfo(attrName, attr))
	{
		mi_error("Attribute %s not found in particle file", attrName);
		return false;
	}

	std::string an(attrName);
	if( an == "position")
	{
		if( ((attr.type != Partio::FLOAT) &&  (attr.type != Partio::VECTOR)) || (attr.count != 3))
		{
			mi_error("Attribute data type is wrong.");
			return false;
		}
	}
	if( an == "velocity")
	{
		if( ((attr.type != Partio::FLOAT) &&  (attr.type != Partio::VECTOR)) || (attr.count != 3))
		{
			mi_error("Attribute data type is wrong.");
			return false;
		}
	}
	if( an == "id")
	{
		if( (attr.type != Partio::INT) || (attr.count != 1))
		{
			mi_error("Attribute data type is wrong.");
			return false;
		}
	}
	if( an == "rgbPP")
	{}
	if( an == "radiusPP")
	{
		if( (attr.type != Partio::FLOAT) || (attr.count != 1))
		{
			mi_error("Attribute data type is wrong.");
			return false;
		}
	}

	return true;
}


void readPartio( char *fileName )
{
	Partio::ParticlesDataMutable* data=Partio::read(fileName);
	std::cout << "Number of particles " << data->numParticles() << std::endl;	

	for(int i=0;i<data->numAttributes();i++)
	{
		Partio::ParticleAttribute attr;
		data->attributeInfo(i,attr);
		std::cout<<"attribute["<<i<<"] is "<<attr.name<<std::endl;
	}

	Partio::ParticleAttribute posAttr;
	if(!data->attributeInfo("position",posAttr)
	   || (posAttr.type != Partio::FLOAT && posAttr.type != Partio::VECTOR)
	   || posAttr.count != 3){
	   std::cerr<<"Failed to get proper position attribute"<<std::endl;
	   return;
	}
	
	float *pos;
	for(int i=0;i<data->numParticles();i++)
	{
		pos = (float *)data->data<float>(posAttr,i);
		mi_info("Position %f %f %f", pos[0], pos[1], pos[2]);
	}
}

void makeit(miTag *result)
{
	// declare the map as a 3-dimensional map
	mi_api_map_decl_dim ( 3 );

	// the 'mi_api_map_field_decl' function takes four arguments:
	//
	// miParam_type type: basic type of the field (miTYPE_SCALAR or miTYPE_INTEGER)
	// char *name : field name
	// int dimension : dimension of the field, 0 for single values, > 0 for arrays
	// miBoolean global : miTRUE if it's a global field, miFALSE otherwise

	// add the "extension" field as a single float
	miParameter *extension_field = mi_api_map_field_decl (
	miTYPE_SCALAR , mi_mem_strdup("extension") , 0 , miFALSE );

	// add the "color" field as an array of 3 scalars
	miParameter *color_field = mi_api_map_field_decl (
	miTYPE_SCALAR , mi_mem_strdup("color") , 3 , miFALSE );

	// append the color to the extension for the declaration
	// (this also frees the 'color_field' miParameter)
	miParameter *fields_list = mi_api_map_field_append ( extension_field , color_field );

	// create a declaration called "particles" with the given fields list
	// (this also frees the 'fields_list' miParameter)
	miMap_decl *decl = mi_api_map_decl_begin ( mi_mem_strdup("particles") , fields_list );

	// ends the declaration
	mi_api_map_decl_end ();

//	Then you begin the object definition, by calling 'mi_api_object_begin' and possibly setting the object flags as needed, then you begin the definition of the particle object as a set of spheres:
	miObject *obj = mi_api_object_begin(mi_mem_strdup("TestParticleObject"));
	obj->visible = miTRUE;

	// begin the definition of the particle object as spheres
	mi_api_map_obj_type ( mi_mem_strdup("spheres") );

	// the 'mi_api_map_obj_field' function takes 2 arguments:
	//
	// char *field_name : name of the field to map ("radius" in this case)
	// char *mapped_name : name of the mapped field ("extension" in this case)

	// maps the "radius" field to the "extension" field of this map
	mi_api_map_obj_field ( mi_mem_strdup("radius") , mi_mem_strdup("extension") );	

	// begins the definition of the map, taking "particles" as the declaration name
	mi_api_map_begin ( mi_mem_strdup("particles") );
	int num_elements = 1000;

	for ( int i = 0 ; i < num_elements ; ++i ) 
	{
		//
		// compute the position in x, y and z
		//
		miScalar x = srnd() * 10.0f;
		miScalar y = srnd() * 10.0f;
		miScalar z = srnd() * 10.0f;

		// define the position of this element
		mi_api_map_value ( miTYPE_SCALAR , &x );
		mi_api_map_value ( miTYPE_SCALAR , &y );
		mi_api_map_value ( miTYPE_SCALAR , &z );
		mi_api_map_field_end ();

		//
		// compute the radius
		miScalar radius = srnd() * 3.0f;
		// define the radius of this element
		mi_api_map_value ( miTYPE_SCALAR , &radius );
		mi_api_map_field_end ();

		//
		// compute the color in r, g and b
		//
		miScalar r = srnd();
		miScalar g = srnd();
		miScalar b = srnd();

		// define the color of this element
		mi_api_map_value ( miTYPE_SCALAR , &r );
		mi_api_map_value ( miTYPE_SCALAR , &g );
		mi_api_map_value ( miTYPE_SCALAR , &b );
		mi_api_map_field_end();

		// end the definition of this element
		mi_api_map_element_end ();
	}

	// terminates the map definition and stores it in the DB
	miTag map_tag = mi_api_map_end ( 0 );

	miTag particleObjTag = mi_api_object_end();
	miBoolean geoAddResult = mi_geoshader_add_result(result, particleObjTag);

}

miObject *beginObject()
{
    miObject *obj = mi_api_object_begin(NULL);
	obj->visible = miTRUE;
	obj->shadow = 3;
	obj->finalgather = 3;
	obj->globillum = 3;
	obj->reflection = 3;
	obj->refraction = 3;
	obj->caustic = 3;
	obj->face = 'a';

    mi_api_basis_list_clear();
    mi_api_object_group_begin(0.0);

	return obj;
}

miTag finishObject()
{
    miBoolean done = mi_api_object_group_end();
	miTag resultTag = mi_api_object_end();
    return resultTag;
}


void createNativeParticles(miState *state, mrParticleGeoShader_paras *paras, PartioContainer& pc)
{
	mi_info("Creating native particles for cache file: %s", pc.cacheFileName.c_str());
}

bool checkScreenSpace(miState *state, mrParticleGeoShader_paras *paras, miVector pos, miVector& bottomLeft, miVector& topRight)
{
	miScalar minPixelSize = *mi_eval_scalar(&paras->minPixelSize);
	miScalar maxPixelSize = *mi_eval_scalar(&paras->maxPixelSize);
	int minSizeBehaviour = *mi_eval_integer(&paras->minSizeBehaviour); // 0 = delete, 1 = resize
	if(maxPixelSize <= 0.0f )
		maxPixelSize = 100000000.0f;

	miVector screenBottomLeft = bottomLeft;
	miVector screenTopRight = topRight;
	mi_point_from_camera(state, &screenBottomLeft, &screenBottomLeft);
	mi_point_from_camera(state, &screenTopRight, &screenTopRight);
	mi_point_to_raster(state, &screenBottomLeft, &screenBottomLeft);
	mi_point_to_raster(state, &screenTopRight, &screenTopRight);
	miScalar screenDist = mi_vector_dist(&screenBottomLeft, &screenTopRight);
	miScalar dist = mi_vector_dist(&bottomLeft, &topRight);
	
	//mi_info("screenDist %f  minPixelSize %f", screenDist, minPixelSize);
	// nonsense no check will be done
	if(minPixelSize > maxPixelSize)
		return true;

	float minClip = 0.001f;

	if( screenDist < minPixelSize )
	{
		if( minSizeBehaviour == 0)
			return false;
		//mi_info("screenDist %f < minPixelSize %f", screenDist, minPixelSize);
		float resizeFactor = minPixelSize/screenDist;
		float resizeDist = dist * resizeFactor/2.0f;
		topRight = pos;
		bottomLeft = pos;
		topRight.x += resizeDist;
		topRight.y += resizeDist;
		bottomLeft.x -= resizeDist;
		bottomLeft.y -= resizeDist;

		return true;
	}
	if( screenDist > maxPixelSize )
	{
		float resizeFactor = maxPixelSize/screenDist;
		float resizeDist = dist * resizeFactor/2.0f;
		topRight = pos;
		bottomLeft = pos;
		topRight.x += resizeDist;
		topRight.y += resizeDist;
		bottomLeft.x -= resizeDist;
		bottomLeft.y -= resizeDist;

		return true;

	}
	return true;
}

void fixInstRotation(miState *state, miVector& bottomLeft, miVector& topRight)
{
	
}

miTag createMeshParticles(miState *state, mrParticleGeoShader_paras *paras, PartioContainer& pc)
{
	mi_info("Creating mesh particles for cache file: %s", pc.cacheFileName.c_str());
	if( ! pc.good())
	{
		mi_error("Invalid PartioContainer.");
		return miNULLTAG;
	}

	Partio::ParticleAttribute posAttr;
	if(!pc.assertAttribute("position", posAttr))
		return miNULLTAG;

	Partio::ParticleAttribute idAttr;
	bool hasId = true;
	if(!pc.assertAttribute("id", idAttr))
		hasId = false;

	Partio::ParticleAttribute radiusPPAttr;
	bool hasRadiusPP = true;
	if(!pc.assertAttribute("radiusPP", radiusPPAttr))
		hasRadiusPP = false;

	Partio::ParticleAttribute velocityAttr;
	bool hasVelocity = true;
	if(!pc.assertAttribute("velocity", velocityAttr))
		hasVelocity = false;

	if( pc.data->numParticles() == 0 )
	{
		mi_error("No particles in container.");
		return miNULLTAG;
	}
	
	miObject *obj = beginObject();

	float *fpos;
	srand(123345);
	int numParticles = pc.data->numParticles();
	float sizeMultiplier = *mi_eval_scalar(&paras->sizeMultiplier);
	float density = *mi_eval_scalar(&paras->density);
	float size = *mi_eval_scalar(&paras->size);
	float sizeVariation = *mi_eval_scalar(&paras->sizeVariation);

	// particle number can vary because of density value
	int numWrittenParticles = 0;

	// define vectors
	for(int vtxId = 0; vtxId < numParticles; vtxId++)
	{
		miVector pos;
		fpos = (float *)pc.data->data<float>(posAttr, vtxId);

		int id = vtxId;
		if( hasId)
			id = *pc.data->data<int>(idAttr, vtxId);

		float radiusPP = 1.0f;
		if( hasRadiusPP )
			radiusPP = *pc.data->data<float>(radiusPPAttr, vtxId);

		miVector vel = {0.0, 0.0, 0.0};
		if(hasVelocity)
		{
			float *v;
			v = (float *)pc.data->data<float>(velocityAttr, vtxId);
			vel.x = v[0];
			vel.y = v[1];
			vel.z = v[2];
			// velocity ist distance/sekunde, eine velocity von 24 legt also eine Distanz von 1 Einheit pro frame bei 24fps zurück
			// zusätzlich muss man noch den shutter angle beachten, bei 140° sind das -.2 -> 0.2 also 0.4 * 1 und damit grob .4 Einheiten
			float factor = 1.0f/24.0f * 0.4f;
			mi_vector_mul(&vel, factor);
		}

		pos.x = fpos[0];
		pos.y = fpos[1];
		pos.z = fpos[2];

		miVector camPos = pos;

		// ich transformiere das particle in camera space und gehe dann einfach size/2 nach rechts und oben
		miMatrix matrix;
		mi_matrix_ident(matrix);
		miInstance *inst = (miInstance *)mi_db_access(state->instance);
		mi_matrix_copy(matrix, inst->tf.global_to_local);
		mi_db_unpin(state->instance);
		
		mi_point_from_world(state, &camPos, &pos);
		mi_point_from_world(state, &camPos, &camPos);
		mi_point_to_camera(state, &camPos, &camPos);

		float psize = radiusPP * sizeMultiplier;
		int pId = vtxId;
		double rndVal = rnd(id * state->camera->frame + 5);
		if( rndVal > density)
			continue;
		
		float srndVal = (rndVal - 0.5f) * 2.0f;
		psize *= size + (size * srndVal * sizeVariation * 0.5f);
		psize = fabs(psize);
		if(psize == 0.0f)
			continue;

		miVector upRight, bottomLeft;
		upRight = camPos;
		upRight.x += psize/2.0f;
		upRight.y += psize/2.0f;
		bottomLeft = camPos;
		bottomLeft.x -= psize/2.0f;
		bottomLeft.y -= psize/2.0f;

		// checkScreenSpace in Screenspace und testet auf minPixelSize. 
		if(!checkScreenSpace(state, paras, camPos, bottomLeft, upRight) )
			continue;
		numWrittenParticles++;

		miVector v0, v1, v2, v3;
		v0 = bottomLeft;
		v2 = upRight;
		v1 = bottomLeft;
		v1.y = upRight.y;
		v3 = upRight;
		v3.y = bottomLeft.y;

		mi_point_from_camera(state, &v0, &v0);
		mi_point_from_camera(state, &v1, &v1);
		mi_point_from_camera(state, &v2, &v2);
		mi_point_from_camera(state, &v3, &v3);

		mi_point_to_world(state, &v0, &v0);
		mi_point_to_world(state, &v1, &v1);
		mi_point_to_world(state, &v2, &v2);
		mi_point_to_world(state, &v3, &v3);

		miVector v01, v02, v03;
		mi_vector_sub(&v01, &v0, &v1);
		mi_vector_sub(&v02, &v0, &v2);
		mi_vector_sub(&v03, &v0, &v3);
		mi_vector_transform(&v01, &v01, matrix);
		mi_vector_transform(&v02, &v02, matrix);
		mi_vector_transform(&v03, &v03, matrix);
		mi_vector_add(&v1, &v0, &v01);
		mi_vector_add(&v2, &v0, &v02);
		mi_vector_add(&v3, &v0, &v03);

		// add geometry vectors 
		// e.g. -0.5 -0.5 0.5
		add_vector(v0.x, v0.y, v0.z);
		add_vector(v1.x, v1.y, v1.z);
		add_vector(v2.x, v2.y, v2.z);
		add_vector(v3.x, v3.y, v3.z);

		// single motion vector per particle
		add_vector(vel.x, vel.y, vel.z);

	}

	// uv coordinates
	miVector uvw;
	uvw.x = uvw.y = uvw.z = 0.0f;
	uvw.z = 123.0f;
	mi_api_vector_xyz_add( &uvw );
	uvw.x = 1.0;
	mi_api_vector_xyz_add( &uvw );
	uvw.y = 1.0;
	mi_api_vector_xyz_add( &uvw );
	uvw.x = 0.0;
	mi_api_vector_xyz_add( &uvw );


	// define vertices
	// depending on the attributes we have x vectors per vertex:
	// 0: pos1
	// 1: pos2
	// 2: pos3
	// 3: pos4
	// 4: vel
	// tex0 = numWrittenParticles * 5 - 4

	// num done particles für rnd density
	int texIndex = numWrittenParticles * 5;
	for(int vtxId = 0; vtxId < numWrittenParticles; vtxId++)
	{
		int vertexIndex = vtxId * 5;
		int mvIndex = vtxId * 5 + 4;
		// add vertex definitions
		// e.g. v 0 n 8 t 32 m 46
		mi_api_vertex_add(vertexIndex);
		mi_api_vertex_tex_add( texIndex, -1, -1);
		mi_api_vertex_motion_add(mvIndex);

		mi_api_vertex_add(vertexIndex + 1);
		mi_api_vertex_tex_add( texIndex + 1, -1, -1);
		mi_api_vertex_motion_add(mvIndex);

		mi_api_vertex_add(vertexIndex + 2);
		mi_api_vertex_tex_add( texIndex + 2, -1, -1);
		mi_api_vertex_motion_add(mvIndex);

		mi_api_vertex_add(vertexIndex + 3);
		mi_api_vertex_tex_add( texIndex + 3, -1, -1);
		mi_api_vertex_motion_add(mvIndex);
	}


	// add poly for every particle

	for( int pId = 0; pId < numWrittenParticles; pId++)
	{
		int vtxId = pId * 4;
		mi_api_poly_begin_tag(1, miNULLTAG);

		mi_api_poly_index_add(vtxId);
		mi_api_poly_index_add(vtxId + 1);
		mi_api_poly_index_add(vtxId + 2);
		mi_api_poly_index_add(vtxId + 3);

		mi_api_poly_end();

	}
	miTag objTag = finishObject();
	return objTag;
   
}

// 
//	possible file name patterns:
//		maya nParticle cache: nodeNameNumber.ext	
//
std::string paddedFrameNumber( int frameNumber, int padding)
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


std::string getCorrectFileName(miState *state, mrParticleGeoShader_paras *paras, std::string fileName)
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

extern "C" DLLEXPORT int mrParticleGeoShader_version()
{
	return (1);
}

extern "C" DLLEXPORT miBoolean mrParticleGeoShader(
	miTag             *result,
	miState                 *state,
	mrParticleGeoShader_paras   *paras)
{
	
	int			geometryType = *mi_eval_integer(&paras->geometryType);
	miScalar	minPixelSize = *mi_eval_scalar(&paras->minPixelSize);
	miScalar	maxPixelSize = *mi_eval_scalar(&paras->maxPixelSize);
	int i_m = *mi_eval_integer(&paras->i_particleFiles);
	int n_m = *mi_eval_integer(&paras->n_particleFiles);
	miTag *particleFiles = mi_eval_tag(paras->particleFiles) + i_m;

	int mag0 = 1179603508;
	int mag = 1179603512;
	char *c0 = (char *)&mag0;
	char *c = (char *)&mag;
	std::cout << "Val0: " << c0[0] << " " << c0[1] << " " << c0[2] << " " << c0[3] << "\n";
	std::cout << "Val1: " << c[0] << " " << c[1] << " " << c[2]  << " " << c[3] << "\n";

	for(int i = 0; i < n_m; i++)
	{
		if (particleFiles[i])
		{
			std::string fileName = tag_to_string(particleFiles[i]);
			std::string correctedFileName = getCorrectFileName(state, paras, fileName);
			mi_info("reading cacheFile %s", correctedFileName.c_str());
			PartioContainer pc(correctedFileName);
			if( geometryType == 0)
			{
				miTag particleTag = createMeshParticles(state, paras, pc);
				if( particleTag != miNULLTAG)
					miBoolean done = mi_geoshader_add_result( result, particleTag);
			}else
				createNativeParticles(state, paras, pc);
		}
	}
	return miTRUE;
}


