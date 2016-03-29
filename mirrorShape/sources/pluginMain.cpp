//Vendor: Roman Volodin, 2016
//website: http://romanvolodin.com

//SYNOPSIS
//This is a deformer node that mirrors a shape geometry so as we could get a mirrored blendShape for one click 
//myDeformerCmd creates a Symmetry Map that is used by defomer for relocating a passed in mesh vertices positions

//USAGE
//just select two meshes (forst one - is a base mesh with simmetrical sides, the second one is a target mehs) and type "mirrorShape;"" in Script Editor


#include "mirrorShape.h"
#include "mirrorShapeCmd.h"

#include <maya/MFnPlugin.h>

MStatus initializePlugin( MObject obj )
{
    MStatus status;

    MFnPlugin fnPlugin( obj, "Roman Volodin", "1.0", "Any");

    //register deformer node
    status = fnPlugin.registerNode( "mirrorShape",  myDeformer::id,  myDeformer::creator,  myDeformer::initialize,  MPxNode::kDeformerNode );
    CHECK_MSTATUS_AND_RETURN_IT( status );

	//register maya command
	status = fnPlugin.registerCommand("mirrorShape", MirrorBlendShape::creator, MirrorBlendShape::newSyntax);
	CHECK_MSTATUS_AND_RETURN_IT( status );

    return MS::kSuccess;
}


MStatus uninitializePlugin( MObject obj )
{
    MStatus status;

    MFnPlugin fnPlugin( obj );

    status = fnPlugin.deregisterNode( myDeformer::id );
    CHECK_MSTATUS_AND_RETURN_IT( status );

	status = fnPlugin.deregisterCommand("mirrorShape");
	CHECK_MSTATUS_AND_RETURN_IT(status);


    return MS::kSuccess;

}