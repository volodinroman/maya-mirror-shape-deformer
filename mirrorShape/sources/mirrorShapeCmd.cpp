//Vendor: Roman Volodin, 2016
//website: http://romanvolodin.com

#include "mirrorShape.h"
#include "mirrorShapeCmd.h"

//create mel command attributes
MSyntax MirrorBlendShape::newSyntax()
{
	MSyntax syntax;

	syntax.addFlag( "-n", "-name", MSyntax::kString ); //deformer name
	syntax.addFlag( "-sym", "-symmetryPlane", MSyntax::kString); //symmetry plane
    syntax.setObjectType( MSyntax::kSelectionList, 2, 2 ); 
    syntax.useSelectionAsDefault( true ); 
    syntax.enableEdit( false ); 
    syntax.enableQuery( false );

	return syntax;
}


//get shape node of the path. If path is already the shape - dont do anything
MStatus MirrorBlendShape::getShapeNode( MDagPath& path )
{
    MStatus status;
 
    if ( path.apiType() == MFn::kMesh ) 
    {
        return MS::kSuccess;
    }
 
    unsigned int numShapes;
    status = path.numberOfShapesDirectlyBelow( numShapes ); err(status); 

 
    for ( unsigned int i = 0; i < numShapes; ++i )
    {
        status = path.extendToShapeDirectlyBelow( i ); err(status); 

         if ( !path.hasFn( MFn::kMesh ) ) 
        {
            path.pop();
            continue;
        }
 
        MFnDagNode fnNode( path, &status ); err(status); 

 
        if ( !fnNode.isIntermediateObject() ) 
        {
            return MS::kSuccess; 
        }
        path.pop();
    }
 
    return MS::kFailure; //if we didnt find any shape node under the passed in arrgument
}

//get deformer MObject from a given targetMesh MObject
MStatus MirrorBlendShape::getDeformerNodeFromTargetMesh(MObject& oDeformer)
{
    MStatus status;
    MObject oTargetMesh = dpTargetMesh.node(); //get DAG node of the target mesh
 
    //Iterator that comes though all nodes related to the Target Mesh Node 
    MItDependencyGraph itGraph( oTargetMesh, 
								MFn::kInvalid, 
								MItDependencyGraph::kUpstream,
								MItDependencyGraph::kDepthFirst, 
								MItDependencyGraph::kNodeLevel, 
								&status );
    err( status );
 
    while ( !itGraph.isDone() )
    {
        oDeformer = itGraph.currentItem(); //Current Node MObject
        MFnDependencyNode fnNode( oDeformer, &status ); err( status );
        
        if ( fnNode.typeId() == myDeformer::id ) //mirrorShape Deformer Node Id - is a unique ID for the deformer
        {
            return MS::kSuccess;
        }
        itGraph.next();
    }
    return MS::kFailure;
}

//check number in range
bool MirrorBlendShape::checkInRange(float number, float min, float max)
{
	if(number >= min && number <= max)
	{
		return true;
	}
	else{
		return false;
	}
}

MStatus MirrorBlendShape::doIt ( const MArgList &argList )
{ 
	MStatus stat;

	//get args
	MArgDatabase argData(syntax(), argList, &stat); err(stat);

	MSelectionList selection;
	stat = argData.getObjects(selection); err(stat);

	//get DP of selection
	stat = selection.getDagPath(0, dpBaseMesh); err(stat);
	stat = selection.getDagPath(1, dpTargetMesh); err(stat);

	//Get shapes DP of selection
	stat = getShapeNode(dpBaseMesh); err(stat);
	stat = getShapeNode(dpTargetMesh); err(stat);

	//get "Name" attribute
	if ( argData.isFlagSet( "-n" )){ //if we set some name
        deformerName = argData.flagArgumentString( "-n", 0, &stat ); err(stat); //get this name from the argument
    }
    else{
        deformerName = "mirrorShapeDeformer#"; //or set the default name
    }

	//get "symetry plane" attribute
	int xAxis, yAxis, zAxis;
	if(argData.isFlagSet("-sym")){
		symPlane = argData.flagArgumentString("-sym",0,&stat); err(stat);
	}
	else{
		symPlane = "YZ";
	}

	if(symPlane == "YZ" || symPlane == "ZY" || symPlane == "yz" || symPlane == "zy"){
		xAxis = -1; yAxis = 1; zAxis = 1;
	}
	else if(symPlane == "XY" || symPlane == "YX" || symPlane == "xy" || symPlane == "yx"){
		xAxis =  1; yAxis = 1; zAxis =-1;
	}
	else if(symPlane == "XZ" || symPlane == "ZX" || symPlane == "xz" || symPlane == "zx"){
		xAxis = 1; yAxis = -1; zAxis = 1;
	}
	else{
		xAxis = -1; yAxis = 1; zAxis = 1; //YZ by default
	}

	//Create deformer
	char buffer[512];

	sprintf( buffer, "deformer -type mirrorShape -n \"%s\" %s", deformerName.asChar(), dpTargetMesh.partialPathName().asChar() );
    stat = dgMod.commandToExecute( buffer ); err(stat); //execute mel command (MDGModifier method) written in the buffer
	
	MFnMesh fnTargetMesh(dpTargetMesh);

	//create progress bar
	MFnMesh fnBaseMesh(dpBaseMesh);
	MPointArray basePoints;
	fnBaseMesh.getPoints(basePoints, MSpace::kWorld);
	int baseVtxLen = basePoints.length();
	int progress = 0;
	MGlobal::executePythonCommand(MString() + "maya.cmds.progressWindow(title='Symmetry Calculation', progress = 0, min = 0, max = " + (baseVtxLen - 1) + ", isInterruptable = False)");

	//create symmetry map
	//    sym MPoint  sym MPoint
	//  [  [MPoint] ,  [MPoint]  .... ]
	//    cur Index   cur Index

	MItMeshVertex iter(dpBaseMesh);
	while(!iter.isDone())
	{
		MPoint pos = iter.position(MSpace::kWorld, &stat); err(stat); //get current position
		int symID = iter.index(); //current index is a symmetry index
		MPoint symPoint = pos; //symmetry position

		

		//iterate the mesh again to find a symmetrical point with an opposite X value (or Y or Z)
		MItMeshVertex mapIter(dpBaseMesh);
		while(!mapIter.isDone())
		{
			MPoint symPos = mapIter.position(MSpace::kWorld, &stat); err(stat); //get current position
			if( checkInRange(symPos.x, xAxis * pos.x - 0.01, xAxis * pos.x + 0.01) &&
				checkInRange(symPos.y, yAxis * pos.y - 0.01, yAxis * pos.y + 0.01) &&
				checkInRange(symPos.z, zAxis * pos.z - 0.01, zAxis * pos.z + 0.01))
			{
				symID = mapIter.index();
			}
			mapIter.next();
		}

		//update progress bar
		progress++;
		MGlobal::executePythonCommand(MString() +"cmds.progressWindow(edit = True, progress = " + progress + ")");

		MPoint symPt;
		fnTargetMesh.getPoint(symID, symPt); //get target's symmetrical point position
		symPoint = MPoint(symPt.x * xAxis, symPt.y * yAxis, symPt.z * zAxis);
		
		symmetryIndexMap.append(symID); //append symmetrical index / or current index
		symmetryPointMap.append(symPoint);

		iter.next();
	}

	MGlobal::executePythonCommand("maya.cmds.progressWindow(endProgress = 1)");

	return redoIt();
}


MStatus MirrorBlendShape::redoIt()
{	
	MStatus stat;

	//Main 
	stat = dgMod.doIt(); err(stat); //create deformer for the target mesh

	//get MObject of the created deformer
	MObject oDeformer;
	stat = getDeformerNodeFromTargetMesh(oDeformer); err(stat);

	//Connect plugs baseMesh.outMesh -> deformer.inputGeometry
	MFnDagNode fnBaseMesh(dpBaseMesh, &stat); err(stat);

	MPlug plugWorldMesh = fnBaseMesh.findPlug("worldMesh", false, &stat); err(stat); //find plug WorldMesh from base mesh - that contains world space transform data

	stat = plugWorldMesh.selectAncestorLogicalIndex(0, plugWorldMesh.attribute()); err(stat); //get plug's child with index(0)

	MPlug plugDeformer(oDeformer, myDeformer::aOrigMesh); //get deformer's plug to connect to

	//set mpoint mapping to the deformer attribute
	MFnPointArrayData fnPointData;
	MObject oPointData = fnPointData.create(symmetryPointMap,&stat); err(stat);
	MPlug plugPointMapping(oDeformer, myDeformer::aPointMapping);
	plugPointMapping.setMObject(oPointData);

	MDGModifier tempDGMod;
	stat = tempDGMod.connect(plugWorldMesh, plugDeformer); err(stat);
	stat = tempDGMod.doIt();

	setResult(deformerName);

	return MS::kSuccess;

}


MStatus MirrorBlendShape::undoIt()
{
	MStatus stat;

	stat = dgMod.undoIt(); err(stat); //undo deformer creation

	return MS::kSuccess;
}