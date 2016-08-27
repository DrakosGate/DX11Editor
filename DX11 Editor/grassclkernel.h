////
////  File Name   :   level.h
////  Description :   Class of Level
////  Author      :   Christopher Howlett
////  Mail        :   drakos_gate@yahoo.com
////
//
//#pragma once
//
//#ifndef __GRASSCLKERNEL_H__
//#define __GRASSCLKERNEL_H__
//
//// Library Includes
//#include <vector>
//
//// Local Includes
//#include "openclkernel.h"
//#include "mathlibrary.h"
//
//// Types
//
//// Constants
//
//// Prototypes
//class Grass;
//class OpenCLContext;
//class AIHiveMind;
//class RenderEntity;
//
//class CGrassCLKernel : public OpenCLKernel
//{
////Member functions
//public:
//	CGrassCLKernel();
//	virtual ~CGrassCLKernel();
//	 
//	virtual void CreateBuffers(OpenCLContext* _pOpenCL, Grass* _pGrass, std::vector<RenderEntity*>* _pCollisionObjects);
//	virtual void SendDataToGPU(OpenCLContext* _pOpenCL, Grass* _pGrass, std::vector<RenderEntity*>* _pCollisionObjects, float _fDeltaTime);
//	void RetrieveOpenCLResults(OpenCLContext* _pOpenCL, Grass* _pGrass);
//
////Member variables
//protected:	
//	//Memory 
//	cl_mem m_clInPos;
//	cl_mem m_clInDir;
//	cl_mem m_clInObjects;
//	cl_mem m_clOutDir;
//
//	Math::Vector4* m_pPositions;
//	Math::Vector4* m_pDirections;
//	Math::Vector4* m_pObjectData;
//	Math::Vector4* m_pOutDirections;
//
//	int m_iNumObstacles;
//};
//
//#endif//__GRASSCLKERNEL_H__