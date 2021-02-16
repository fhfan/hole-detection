
#pragma once
#include <string>
#include "HalconCpp.h"

using namespace HalconCpp;
using namespace std;



class AlgConfigurationTool
{
public:
	AlgConfigurationTool();
	~AlgConfigurationTool();

	bool ReadCameraCorrectionConfig(std::string strPath, std::string& strCameraFile, std::string& strPoseFile, double& dCompensationValue);
};


//相机标定文件
extern HTuple hv_CameraParameters;
extern HTuple hv_CameraPose;
extern double TargetThickness;
extern  double CompensationValue;

//相机初始配置文件路径
extern string   CameraConfigPath;
extern AlgConfigurationTool  AlgConfigTool;
