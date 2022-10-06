#include "Utils.h"
#include <windows.h>
#include "DialogHelper.h"
#include "LayerImporter.h"

Layer* Utils::openLayer()
{
	char path[1024] = { 0 };
	if (DialogHelper::selectSingleFile("", "shp", path, 1024))
	{
		return LayerImporter::importShpLayer(path);
	}
	return 0;
}