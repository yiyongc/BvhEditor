#include "Skeleton.h"

Skeleton::Skeleton() {
	m_selectedVertex = -1;
}

Skeleton::~Skeleton() {}

int Skeleton::getSelectedVertex() {
	return m_selectedVertex;
}

void Skeleton::setSelectedVertex(int vertexID) {
	m_selectedVertex = vertexID;
}

void Skeleton::addVertex(SkeletonVertex* v) {
	int parent = getSelectedVertex();
	if (parent != -1)
		v->setParent(parent);
	joints.push_back(v);
}

int Skeleton::getVertexCount() {
	return joints.size();
}

SkeletonVertex* Skeleton::getVertex(int vertexID) {
	for (int i = 0; i < joints.size(); i++) {
		if (joints.at(i)->getID() == vertexID)
			return joints.at(i);
	}
	return NULL;
}

bool Skeleton::removeVertex(int id) {
	for (int i = 0; i < joints.size(); i++) {
		if (joints.at(i)->getID() == id) {
			for (int j = 0; j < joints.size(); j++) {
				if (joints.at(j)->getParent() == id)
					joints.at(j)->setParent(joints.at(i)->getParent());
			}
			joints.erase(joints.begin() + i);

			return true;
		}
	}

	return false;
}

std::vector<SkeletonVertex*> Skeleton::getVertices() {
	return joints;
}