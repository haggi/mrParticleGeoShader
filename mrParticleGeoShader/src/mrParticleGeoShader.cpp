#include "mrParticleGeoShader.h"
#include <stdlib.h>

float rnd()
{
	float rm = (float)RAND_MAX;
	float r  = (float)rand();
	return( r/rm );
}

float srnd()
{
	float rm = (float)RAND_MAX;
	float r  = (float)rand();
	return( ((r/rm) - 0.5f) * 2.0f );
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


extern "C" DLLEXPORT int mrParticleGeoShader_version()
{
	return (1);
}

extern "C" DLLEXPORT miBoolean mrParticleGeoShader(
	miTag             *result,
	miState                 *state,
	mrParticleGeoShader_paras   *paras)
{
	makeit(result);
	return miTRUE;
}


