#ifndef SKELETONVERTEX_H
#define SKELETONVERTEX_H

class SkeletonVertex {

public:
	SkeletonVertex();
	SkeletonVertex(double x, double y);
	~SkeletonVertex();

	int getParent();
	void setParent(int parent);

	int getID();
	void setID(int id);

	double getXPos();
	double getYPos();
	void setXPos(double x);
	void setYPos(double y);

private:
	double xPos;
	double yPos;
	int parent_id;
	int vertex_id;


};


#endif // !SKELETONVERTEX_H
