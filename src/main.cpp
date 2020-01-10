#include <assert.h>
#include <cstdlib>
#include "Convert.h"

bool eq(double a, double b)
{
	const double e = 1e-5;
	return fabs(a - b) < e;
}

int main()
{
	double wlon;
	double wlat;
	double glon, glat,blon,blat;
	
	GeoConvert convert(true);

	wlon = 121.926087;
	wlat = 30.899813;
	assert(convert.WGS84ToGCJ02(wlon, wlat, glon, glat));
	assert(convert.GCJ02ToWGS84(glon, glat, wlon, wlat));
	assert(eq(121.926087, wlon));
	assert(eq(30.899813, wlat));

	wlon = 121.926087;
	wlat = 30.899813;
	assert(convert.WGS84ToBD09(wlon, wlat, blon, blat));
	assert(convert.BD09ToWGS84(blon, blat, wlon, wlat));
	assert(eq(121.926087, wlon));
	assert(eq(30.899813, wlat));
	
	return 0;
}