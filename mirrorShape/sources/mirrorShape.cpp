//Vendor: Roman Volodin, 2016
//website: http://romanvolodin.com

#include "mirrorShape.h"
#include "mirrorShapeCmd.h"

MTypeId     myDeformer::id( 0x00000234 ); //it's not registered ID, use your own ID
MObject     myDeformer::aOrigMesh; 
MObject		myDeformer::aPointMapping;

myDeformer::myDeformer(){}//constructor
 
myDeformer::~myDeformer(){} // destructor

void* myDeformer::creator() //create an instance
{ 
    return new myDeformer();
}


//Main Deformation method
MStatus myDeformer::deform(		MDataBlock& data, //dataHandle which contains all our decalred attributes
								MItGeometry& itGeo, //our geometry that is deformed
								const MMatrix& localToWorldMatrix, //world matrix of our deformed geometry
								unsigned int geomIndex )
{
    MStatus status;

	//get envelope
	float env = data.inputValue( envelope ).asFloat();

	//get symmetry map
	MObject oPointMapData = data.inputValue(aPointMapping).data(); 
	if (env == 0.0f || oPointMapData.isNull()) 
    {
        return MS::kSuccess;
    }

	MFnPointArrayData pointArrayData(oPointMapData, &status); err(status);
	symmetryPointMap = pointArrayData.array();

	
	//iterate our geometry vertices
	for(itGeo.reset(); !itGeo.isDone(); itGeo.next())
	{
		MPoint pt = itGeo.position(); 
		int index = itGeo.index();

		MPoint symPt = symmetryPointMap[index];

		MVector offset(symPt.x - pt.x, symPt.y - pt.y, symPt.z - pt.z); //symmetrical offset vector

		pt = pt + offset * env;
		itGeo.setPosition(pt);
	}
    
    return MS::kSuccess;
}

//initialize the attributes of our node
MStatus myDeformer::initialize()
{
    MStatus status;

    MFnTypedAttribute tAttr;

	aOrigMesh = tAttr.create( "origMesh", "origMesh", MFnData::kMesh);
    addAttribute(aOrigMesh);
    attributeAffects(aOrigMesh, outputGeom);

	aPointMapping = tAttr.create( "pointMapping", "pointMapping", MFnData::kPointArray ); //type = pointArray (vertex positions with symmetrical values)
    tAttr.setHidden( true );
    tAttr.setConnectable( false );
    addAttribute( aPointMapping );
    attributeAffects( aPointMapping, outputGeom );//attribute affects outputgeom attribute

    return MS::kSuccess;

}