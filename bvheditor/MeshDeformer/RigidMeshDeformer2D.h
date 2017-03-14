#ifndef __RIGID_MESH_DEFORMER_2D_H__
#define __RIGID_MESH_DEFORMER_2D_H__

#include <map>
#include <set>
#include <limits>
#include <WmlBox2.h>
#include <WmlGMatrix.h>
#include "WmlLinearSystem.h"
#include "WmlLinearSystemExt.h"
#include <OpenToonz\tmeshimage.h>
#include "WmlExtTriangleUtils.h"

class RigidMeshDeformer2D {

public:
	RigidMeshDeformer2D();
	~RigidMeshDeformer2D();
	
	void forceValidation() { ValidateSetup(); }
	

	//---- Interface related ----//
	//! nHandle is vertex ID
	void SetDeformedHandle(unsigned int nHandle, const Wml::Vector2f & vHandle); 
	void UnTransformPoint(Wml::Vector2f & vTransform); 
	void removeHandle(unsigned int nHandle);

	//---- Mesh handling functions ----//
	void initializeFromMesh(TTextureMesh* m_mesh);
	void updateDeformedMesh(TTextureMesh* m_mesh, bool bRigid);

	const Wml::Vector2f * GetTriangleVerts(unsigned int nTriangle) { return m_vTriangles[nTriangle].vScaled; }

	struct Triangle {
		unsigned int nVerts[3];

		// definition of each vertex in triangle-local coordinate system
		Wml::Vector2f vTriCoords[3];

		// un-scaled triangle
		Wml::Vector2f vScaled[3];

		// pre-computed matrices for triangle scaling step
		Wml::GMatrixd mF, mC;
	};

protected:

	struct Vertex {
		Wml::Vector2f vPosition;
	};

	struct Constraint {
		unsigned int nVertex;
		mutable Wml::Vector2f vConstrainedPos;

		Constraint() { nVertex = 0; vConstrainedPos = Wml::Vector2f::ZERO; }
		Constraint(unsigned int nVert, const Wml::Vector2f & vPos) { nVertex = nVert; vConstrainedPos = vPos; }

		bool operator<(const Constraint & c2) const
		{
			return nVertex < c2.nVertex;
		}
	};

	std::vector<Vertex> m_vInitialVerts;
	std::vector<Vertex> m_vDeformedVerts;
	std::vector<Triangle> m_vTriangles;

	std::set<Constraint> m_vConstraints;
	
	bool m_bSetupValid;

	void UpdateConstraint(Constraint & cons);
	
	void InvalidateSetup() { m_bSetupValid = false; }
	void ValidateSetup();

	void ValidateDeformedMesh(bool bRigid); 
	

	Wml::Vector2f GetInitialVert(unsigned int nVert)
	{
		return Wml::Vector2f(m_vInitialVerts[nVert].vPosition.X(), m_vInitialVerts[nVert].vPosition.Y());
	}

	//---- Matrices & Math ----//
	Wml::GMatrixd m_mFirstMatrix;
	std::vector<unsigned int> m_vVertexMap;
	Wml::GMatrixd m_mHXPrime, m_mHYPrime;
	Wml::GMatrixd m_mDX, m_mDY;
	Wml::LinearSystemExtd::LUData m_mLUDecompX, m_mLUDecompY;

	void UpdateScaledTriangle(unsigned int nTriangle);
	void ApplyFittingStep();
	void PrecomputeOrientationMatrix(); // --- not done
	void PrecomputeScalingMatrices(unsigned int nTriangle); // --- not done
	void PrecomputeFittingMatrices(); // --- not done
	

};

#endif // __RIGID_MESH_DEFORMER_2D_H__