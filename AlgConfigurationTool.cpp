#include "stdafx.h"
#include "AlgConfigurationTool.h"
#include "tinyxml.h"

AlgConfigurationTool::AlgConfigurationTool()
{

}

AlgConfigurationTool::~AlgConfigurationTool()
{

}

bool AlgConfigurationTool::ReadCameraCorrectionConfig(std::string strPath, std::string& strCameraFile, std::string& strPoseFile, double& dCompensationValue)
{
	TiXmlDocument xmlDocs;
	bool ret = xmlDocs.LoadFile(strPath.c_str(), TIXML_ENCODING_UTF8);//打开配置文件
	if (!ret)
	{
		return false;
	}

	TiXmlElement* pElementRoot = xmlDocs.RootElement();

	if (NULL == pElementRoot)
	{
		return false;
	}

	TiXmlElement* pCorrectionElement = pElementRoot->FirstChildElement("Correction");
	if (pCorrectionElement != NULL)
	{
		TiXmlElement* pCameraFile = pCorrectionElement->FirstChildElement("Camera");
		strCameraFile = pCameraFile->Attribute("Value");

		TiXmlElement* pPoselFile = pCorrectionElement->FirstChildElement("Pose");
		strPoseFile = pPoselFile->Attribute("Value");

		TiXmlElement* pCompensationValue = pCorrectionElement->FirstChildElement("Compensation");
		dCompensationValue = atof(pCompensationValue->Attribute("Value"));
	}

	return true;
}


//相机标定文件
HTuple hv_CameraParameters;
HTuple hv_CameraPose;
double TargetThickness = 0;	//被测物体的厚度
double CompensationValue = 0;

AlgConfigurationTool  AlgConfigTool;
string	CameraConfigPath = "./Cfg/CameraInitConfig.cfg";
