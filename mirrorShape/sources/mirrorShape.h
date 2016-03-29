//Vendor: Roman Volodin, 2016
//website: http://romanvolodin.com

#ifndef MYDEFORMER_H
#define MYDEFORMER_H

#include <maya/MPxDeformerNode.h>
#include <maya/MDagModifier.h>
#include <maya/MGlobal.h>
#include <maya/MItGeometry.h>
#include <maya/MMatrix.h>
#include <maya/MPoint.h>
#include <maya/MVector.h>
#include <maya/MIntArray.h>
#include <maya/MPointArray.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnIntArrayData.h>
#include <maya/MFnPointArrayData.h>

#define err(stat) CHECK_MSTATUS_AND_RETURN_IT(stat);

class myDeformer : public MPxDeformerNode
{
public:
						myDeformer(); 
	virtual				~myDeformer(); 
	static  void*		creator(); 

    virtual MStatus     deform( MDataBlock& data,
                                MItGeometry& itGeo,
                                const MMatrix& localToWorldMatrix,
                                unsigned int geomIndex ); 

	static  MStatus		initialize();

	static MTypeId	id; //id

	static MObject aOrigMesh;
	static MObject aPointMapping;

private:

	MPointArray symmetryPointMap;

};

#endif
