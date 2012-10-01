#ifndef PARTIO_CONTAINER_H
#define PARTIO_CONTAINER_H

#include <Partio.h>


class PartioContainer{
public:
	PartioContainer();
	PartioContainer(std::string fileName);
	~PartioContainer();
	bool good();
	bool assertAttribute(char *attrName, Partio::ParticleAttribute& attr);
	void getAttributes(std::vector<std::string>& attributeNames);

	void getPosition(int index, float* pos);

	std::string cacheFileName;
	Partio::ParticlesDataMutable* data;
private:

	Partio::ParticleAttribute positionAttr;
};

#endif
