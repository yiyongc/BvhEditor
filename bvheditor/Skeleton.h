#ifndef SKELETON_H
#define SKELETON_H

#include "SkeletonVertex.h"
#include <vector>

class Skeleton {

public:
	Skeleton();
	~Skeleton();

	void addVertex(SkeletonVertex* v);
	int getVertexCount();
	bool removeVertex(int id);
	int getSelectedVertex();
	void setSelectedVertex(int vertexID);
	std::vector<SkeletonVertex*> getVertices();
	SkeletonVertex* getVertex(int vertexID);

private:
	std::vector<SkeletonVertex*> joints;
	int m_selectedVertex;
};


#endif // !SKELETON_H