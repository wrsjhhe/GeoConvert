#include <cstdlib>
#include "Convert.h"
constexpr double PI = 3.14159265358979324;
constexpr double PIX = PI * 3000 / 180;
const double EE = 0.00669342162296594323;
const double A = 6378245.0;


GeoConvert::GeoConvert(bool test): m_test(test)
{

}

GeoConvert::GeoConvert():m_test(false)
{

}

bool GeoConvert::outOfChina(double lon, double lat)
{
	if (lon < 72.004 || lon > 137.8347)
		return true;
	if (lat < 0.8293 || lat > 55.8271)
		return true;
	return false;
}

double GeoConvert::transformLat(double x, double y)
{
	double ret = -100.0 + 2.0 * x + 3.0 * y + 0.2 * y * y + 0.1 * x * y + 0.2 * sqrt(fabs(x));
	ret += (20.0 * sin(6.0 * x * PI) + 20.0 * sin(2.0 * x * PI)) * 2.0 / 3.0;
	ret += (20.0 * sin(y * PI) + 40.0 * sin(y / 3.0 * PI)) * 2.0 / 3.0;
	ret += (160.0 * sin(y / 12.0 * PI) + 320 * sin(y * PI / 30.0)) * 2.0 / 3.0;
	return ret;
}

double GeoConvert::transformLon(double x, double y)
{
	double ret = 300.0 + x + 2.0 * y + 0.1 * x * x + 0.1 * x * y + 0.1 * sqrt(fabs(x));
	ret += (20.0 * sin(6.0 * x * PI) + 20.0 * sin(2.0 * x * PI)) * 2.0 / 3.0;
	ret += (20.0 * sin(x * PI) + 40.0 * sin(x / 3.0 * PI)) * 2.0 / 3.0;
	ret += (150.0 * sin(x / 12.0 * PI) + 300.0 * sin(x / 30.0 * PI)) * 2.0 / 3.0;
	return ret;
}

bool GeoConvert::WGS84ToGCJ02(double wLon, double wLat, double& gLon, double& gLat)
{
	if (m_test && outOfChina(wLon, wLat))
	{
		gLat = wLat;
		gLon = wLon;
		return false;
	}

	double dLat = transformLat(wLon - 105.0, wLat - 35.0);
	double dLon = transformLon(wLon - 105.0, wLat - 35.0);
	double radLat = wLat / 180.0 * PI;
	double magic = sin(radLat);
	magic = 1 - EE * magic * magic;
	double sqrtMagic = sqrt(magic);
	dLat = (dLat * 180.0) / ((A * (1 - EE)) / (magic * sqrtMagic) * PI);
	dLon = (dLon * 180.0) / (A / sqrtMagic * cos(radLat) * PI);
	gLat = wLat + dLat;
	gLon = wLon + dLon;

	return true;
}

bool GeoConvert::GCJ02ToWGS84(double gLon, double gLat, double& wLon, double& wLat)
{
	if (m_test && outOfChina(gLon, gLat))
	{
		wLon = gLon;
		wLat = gLat;
		return false;
	}

	double dLat = transformLat(wLon - 105.0, wLat - 35.0);
	double dLon = transformLon(wLon - 105.0, wLat - 35.0);
	double radLat = wLat / 180.0 * PI;
	double magic = sin(radLat);
	magic = 1 - EE * magic * magic;
	double sqrtMagic = sqrt(magic);
	dLat = (dLat * 180.0) / ((A * (1 - EE)) / (magic * sqrtMagic) * PI);
	dLon = (dLon * 180.0) / (A / sqrtMagic * cos(radLat) * PI);
	wLat = gLat - dLat;
	wLon = gLon - dLon;
	return true;
}

bool GeoConvert::BD09ToGCJ02(double bLon, double bLat, double& gLon, double& gLat)
{
	if (m_test && outOfChina(bLon, bLat))
	{
		gLon = bLon;
		gLat = bLat;
		return false;
	}
	double x = bLon - 0.0065, y = bLat - 0.006;
	double z = sqrt(x * x + y * y) - 0.00002 * sin(y * PIX);
	double theta = atan2(y, x) - 0.000003 * cos(x * PIX);
	gLon = z * cos(theta);
	gLat = z * sin(theta);

	return true;
}

bool GeoConvert::GCJ02ToBD09(double gLon, double gLat, double& bLon, double& bLat)
{
	if (m_test && outOfChina(gLon, gLat))
	{
		bLon = gLon;
		bLat = gLat;
		return false;
	}
	double z = sqrt(gLon * gLon + gLat * gLat) + 0.00002 * sin(gLon * PIX);
	double theta = atan2(gLat, gLon) + 0.000003 * cos(gLon * PIX);
	bLon = z * cos(theta) + 0.0065;
	bLat = z * sin(theta) + 0.006;

	return true;
}

bool GeoConvert::WGS84ToBD09(double wLon, double wLat, double& bLon, double& bLat)
{
	if (m_test && outOfChina(wLon, wLat))
	{
		bLon = wLon;
		bLat = wLat;
		return false;
	}
	double glon, glat;
	return WGS84ToGCJ02(wLon, wLat, glon, glat) && GCJ02ToBD09(glon, glat, bLon, bLat);
}

bool GeoConvert::BD09ToWGS84(double& bLon, double& bLat, double& wLon, double& wLat)
{
	if (m_test && outOfChina(bLon, bLat))
	{
		wLon = bLon;
		wLat = bLat;
		return false;
	}

	double glon, glat;
	return BD09ToGCJ02(bLon, bLat, glon, glat) && GCJ02ToWGS84(glon, glat, wLon, wLat);
}