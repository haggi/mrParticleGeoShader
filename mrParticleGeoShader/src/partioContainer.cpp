#include "partioContainer.h"
#include "shader.h"

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
	if(data->numParticles() == 0)
	{
		mi_warning("No particles in cache file.");
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

void PartioContainer::getAttributes(std::vector<std::string>& attributeNames)
{
	if( this->good())
		return;
	
	attributeNames.clear();

	for(int i=0;i<data->numAttributes();i++)
	{
		Partio::ParticleAttribute attr;
		data->attributeInfo(i,attr);
		attributeNames.push_back(attr.name.c_str());
	}
}
