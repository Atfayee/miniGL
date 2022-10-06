#include "LayerImporter.h"
#include "MyGeoDefine.h"
#include "Graphic.h"

#include "..\\3rdParty\\gdal\\include\\ogrsf_frmts.h"
#include "..\\3rdParty\\gdal\\include\\ogr_feature.h"
#include "..\\3rdParty\\gdal\\include\\gdal.h"
#include "..\\3rdParty\\gdal\\include\\ogr_api.h"
//#include <ogrsf_frmts.h>
//#include <ogr_feature.h>
//#include <gdal.h>
//#include <ogr_api.h>
//#include <assert.h>

#pragma comment(lib,"../3rdParty/gdal/lib/gdal_i.lib")

Layer* LayerImporter::importShpLayer(const char* path, const char* labelFieldName)
{
	Color colorStyle[5] = { BLACK,RED,GREEN,BLUE,YELLOW };
	GDALAllRegister();
	OGRRegisterAll();
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "NO");
	CPLSetConfigOption("SHAPE_ENCODING", "");

	OGRSFDriver* poDriver = OGRSFDriverRegistrar::GetRegistrar()->GetDriverByName("ESRI Shapefile");
	OGRDataSource* poDS = poDriver->Open(path);
	if (poDS == NULL) return NULL;

	OGRLayer* poLayer = poDS->GetLayer(0);
	if (poLayer == NULL) return NULL;

	OGREnvelope ev;
	poLayer->GetExtent(&ev);
	Layer* pLayer = new Layer((GeomType)poLayer->GetGeomType(),"Map");
	pLayer->setEnvelop(ev.MinX, ev.MinY, ev.MaxX, ev.MaxY);

	int labelFieldIndex = -1;
	if (strcmp(labelFieldName, "") != 0)
		labelFieldIndex = poLayer->GetLayerDefn()->GetFieldIndex(labelFieldName);
	OGRFeature* pFeature;
	while ((pFeature = poLayer->GetNextFeature()))
	{
		OGRGeometry* pOGRGeometry = pFeature->GetGeometryRef();

		switch (pOGRGeometry->getGeometryType())
		{
		case wkbPoint:
		{
			OGRPoint* pOGRPoint = (OGRPoint*)pOGRGeometry;
			PointGeometry* pGeometry = new PointGeometry(pOGRPoint->getX(), pOGRPoint->getY());
			if (labelFieldIndex >= 0)
				pGeometry->label = pFeature->GetFieldAsString(labelFieldIndex);
			pLayer->addGeometry(pGeometry, false);
		}
		break;
		case wkbLineString:
		{
			OGRLineString* pOGRLineString = (OGRLineString*)pOGRGeometry;
			PolylineGeometry* pGeometry = new PolylineGeometry();
			for (int i = 0, size = pOGRLineString->getNumPoints(); i < size; ++i)
			{
				pGeometry->addPoint(pOGRLineString->getX(i), pOGRLineString->getY(i));
			}
			if (labelFieldIndex >= 0)
				pGeometry->label = pFeature->GetFieldAsString(labelFieldIndex);
			pGeometry->color = RED;
			pLayer->addGeometry(pGeometry, false);
		}
		break;
		case wkbPolygon:
		{
			OGRPolygon* pOGRPolygon = (OGRPolygon*)pOGRGeometry;
			//PolylineGeometry* pGeometry = new PolylineGeometry();
			PolygonGeometry* pGeometry = new PolygonGeometry();
			OGRLinearRing* ring = pOGRPolygon->getExteriorRing();
			for (int i = 0, size = ring->getNumPoints(); i < size; ++i)
			{
				//assert(ring->getX(i) < 120 && ring->getY(i) < 32);
				pGeometry->addPoint(ring->getX(i), ring->getY(i));
			}
			if (labelFieldIndex >= 0)
				pGeometry->label = pFeature->GetFieldAsString(labelFieldIndex);
			pLayer->addGeometry(pGeometry, false);
		}
		break;
		}
		OGRFeature::DestroyFeature(pFeature);

		//static int ii = 0;
		//++ii;
		//if( ii > 0 ) break;
	}

	OGRDataSource::DestroyDataSource(poDS);
	OGRCleanupAll();

	return pLayer;
}