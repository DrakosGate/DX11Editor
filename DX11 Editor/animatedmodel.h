////
////  File Name   :   animatedmodel.h
////  Description :   Class of CAnimatedModel
////  Author      :   Christopher Howlett
////  Mail        :   drakos_gate@yahoo.com
////
//
//
//#pragma once
//
//#ifndef __ANIMATEDMODEL_H__
//#define __ANIMATEDMODEL_H__
//
//// Library Includes
//#include <map>
//
//// Local Includes
//#include "model.h"
//
//// Types
//struct TBoneInfo
//{
//	TBoneInfo()
//	{
//		D3DXMatrixIdentity(&m_matBoneOffset);
//		D3DXMatrixIdentity(&m_matFinalTransformation);
//	}
//	Math::Matrix m_matBoneOffset;
//	Math::Matrix m_matFinalTransformation;
//};
//
//// Constants
//
//// Prototypes
//class Shader;
//
//class CAnimatedModel : public Model
//{
//	//Member functions
//public:
//	CAnimatedModel();
//	virtual ~CAnimatedModel();
//	virtual void Process(float _fDeltaTime);
//	virtual void SendShaderData(Shader* _pShader);
//
//	virtual void LoadAIMesh(ID3D11Device* _pDevice, float _fScale, char* _pcFilename);
//	virtual void LoadAIScene(const aiScene* _pScene, char* _pcFilename);
//	virtual void LoadAIVertexData(unsigned int _iIndex, const aiMesh* _pMesh);
//	virtual void LoadAIBones(unsigned int _iMeshIndex, const aiMesh* _pMesh);
//	virtual void BoneTransform(float _fTimeInSeconds, std::vector<Math::Matrix>& _rMatVector);
//	virtual void ReadNodeHierarchy(float _fAnimationTime, const aiNode* _pNode, const Math::Matrix& _rMatParent);
//	virtual const aiNodeAnim* FindNodeAnim(const aiAnimation* _pAnimation, std::string _sNodeName);
//
//	//Interpolation
//	void CalcInterpolatedScaling(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
//    void CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
//    void CalcInterpolatedPosition(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);    
//	unsigned int FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim);
//    unsigned int FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim);
//    unsigned int FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim);
//    
//	virtual void CreateVertexBuffer(ID3D11Device* _pDevice);
//	void ToDXMatrix(const aiMatrix4x4& _rAIMat, Math::Matrix& _rOutput);
//	void CalculateBoundingBox();
//
//private:
//	CAnimatedModel(const CAnimatedModel& _krInstanceToCopy);
//	const CAnimatedModel& operator =(const CAnimatedModel& _krInstanceToCopy);
//
//	//Member variables
//protected:
//	TAnimatedVertex* m_pAnimatedVertices;
//	Math::Matrix m_matGlobalInverse;
//	std::map<std::string, unsigned int> m_mapBones;
//	int m_iNumBones;
//	std::vector<TBoneInfo> vecTempBones;
//	std::vector<Math::Matrix> vecTransforms;
//	
//	float m_fTimeElapsed;
//
//	//Assimp member variables
//	const aiScene* m_pAIScene;
//	Assimp::Importer m_aImporter;
//
//};
//
//#endif //MODEL_H__