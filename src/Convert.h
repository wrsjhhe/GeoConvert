#pragma once

class GeoConvert
{
public:
	GeoConvert(bool test);
	GeoConvert();
public:
	bool WGS84ToGCJ02(double wLon, double wLat, double& gLon, double& gLat);
	bool GCJ02ToWGS84(double gLon, double gLat, double& wLon, double& wLat);
	bool BD09ToGCJ02(double bLon, double bLat, double& gLon, double& gLat);
	bool GCJ02ToBD09(double gLon, double gLat, double& bLon, double& bLat);
	bool WGS84ToBD09(double wLon, double wLat, double& bLon, double& bLat);
	bool BD09ToWGS84(double& bLon, double& bLat, double& wLon, double& wLat);

private:
	bool outOfChina(double lon, double lat);
	double transformLat(double x, double y);
	double transformLon(double x, double y);

private:
	bool m_test;
};