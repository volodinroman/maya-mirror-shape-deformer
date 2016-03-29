//Vendor: Roman Volodin, 2016
//website: http://romanvolodin.com


#ifndef MIRRORBS_H
#define MIRRORBS_H

#include <iostream>
#include <cmath>

#include <maya/MPxCommand.h>
#include <maya/MDGModifier.h>
#include <maya/MGlobal.h>
#include <maya/MSyntax.h>
#include <maya/MSelectionList.h>
#include <maya/MDagPath.h>
#include <maya/MObject.h>
#include <maya/MCommandResult.h>
#include <maya/MPlug.h>
#include <maya/MArgDatabase.h>
#include <maya/MIntArray.h>
#include <maya/MPointArray.h>

#include <maya/MItDependencyGraph.h>
#include <maya/MItMeshVertex.h>

#include <maya/MFnMesh.h>
#include <maya/MFnDagNode.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MFnIntArrayData.h>
#include <maya/MFnPointArrayData.h>

#define err(stat) CHECK_MSTATUS_AND_RETURN_IT(stat);

class MirrorBlendShape : public MPxCommand 
{
public:
	virtual MStatus	doIt ( const MArgList& );
	virtual MStatus undoIt();
 	virtual MStatus redoIt();
	virtual bool isUndoable() const { return true; }

	static void *creator() { return new MirrorBlendShape; }
	static MSyntax newSyntax();
	
	MStatus getShapeNode( MDagPath& path );
	MStatus getDeformerNodeFromTargetMesh( MObject& oDeformer );
	bool checkInRange(float number, float min, float max);

private:
	
	MDGModifier dgMod; //for modifying dag nodes

	//Selected DagPaths
	MDagPath dpBaseMesh;
	MDagPath dpTargetMesh;

	//Selected nodes
	MObject oBaseMesh;
	MObject oTargetMesh;

	MString deformerName; //final deformer name
	MIntArray symmetryIndexMap; //collect symmetrical indices
	MPointArray symmetryPointMap; //collect symmetrical MPoints

	MString symPlane; //symmetry plane
	
};

#endif