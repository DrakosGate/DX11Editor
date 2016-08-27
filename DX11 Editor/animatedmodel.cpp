//
//// Library Includes
//#include <D3D11.h>
//
//// Local Includes
//#include "defines.h"
//#include "boundingbox.h"
//#include "shader.h"
//
//// This Include
//#include "animatedmodel.h"
//
//// Static Variables
//
//// Static Function Prototypes
//
//// Implementation
//
///**
//*
//* CAnimatedModel class constructor
//* (Task ID: n/a)
//*
//* @author Christopher Howlett
//*
//*/
//CAnimatedModel::CAnimatedModel()
//	: m_pAnimatedVertices( 0 )
//	, m_iNumBones( 0 )
//	, m_fTimeElapsed( 0.0f )
//{
//
//}
//
///**
//*
//* CAnimatedModel class destructor
//* (Task ID: n/a)
//*
//* @author Christopher Howlett
//*
//*/
//CAnimatedModel::~CAnimatedModel()
//{
//	if( m_pAnimatedVertices )
//	{
//		delete[] m_pAnimatedVertices;
//		m_pAnimatedVertices = 0;
//	}
//}
////=============================================================
////			ASSIMP FILE LOADING
////=============================================================
///**
//*
//* CAnimatedModel class Update
//* (Task ID: n/a)
//*
//* @author Christopher Howlett
//* @param dt Time elapsed
//*
//*/
//void
//CAnimatedModel::Process( float _fDeltaTime )
//{
//	m_fTimeElapsed += _fDeltaTime;
//	vecTransforms.clear();
//	BoneTransform( m_fTimeElapsed, vecTransforms );
//
//	if( m_bIsBillboarded == false )
//	{
//		//Calculate world matrix
//		D3DXQuaternionRotationYawPitchRoll( &m_quatRot, m_vecRotation.y, m_vecRotation.x, m_vecRotation.z );
//		D3DXMatrixTransformation( &m_matWorld,
//			NULL,
//			NULL,
//			NULL, //&m_vecScale,
//			NULL,
//			&m_quatRot,
//			&m_vecPosition );
//	}
//}
//void
//CAnimatedModel::SendShaderData( Shader* _pShader )
//{
//	//for(unsigned int iBone = 0; iBone < vecTransforms.size(); ++iBone)
//	//{
//	//	_pShader->GetEffect()->GetVariableByName("gBones")->GetElement(iBone)->AsMatrix()->SetMatrix(vecTransforms[iBone]);
//	//}
//}
///**
//*
//* CAnimatedModel class LoadFromFile
//* (Task ID: n/a)
//*
//* @author Christopher Howlett
//*
//*/
//void
//CAnimatedModel::LoadAIMesh( ID3D11Device* _pDevice, float _fScale, char* _pcFilename )
//{
//	m_fModelScale = _fScale;
//	m_pAIScene = m_aImporter.ReadFile( _pcFilename, aiProcess_GenSmoothNormals | aiProcess_FlipUVs );
//	if( m_pAIScene )
//	{
//		ToDXMatrix( m_pAIScene->mRootNode->mTransformation, m_matGlobalInverse );
//		D3DXMatrixInverse( &m_matGlobalInverse, NULL, &m_matGlobalInverse );
//		LoadAIScene( m_pAIScene, _pcFilename );
//	}
//	else
//	{
//		MessageBox( NULL, L"Could not load assimp scene from file", L"ERROR", MB_OK );
//	}
//
//	CreateVertexBuffer( _pDevice );
//	CreateIndexBuffer( _pDevice );
//
//	CalculateBoundingBox();
//}
//void
//CAnimatedModel::LoadAIScene( const aiScene* _pScene, char* _pcFilename )
//{
//	int iNumMeshes = _pScene->mNumMeshes;
//
//	for( int iMesh = 0; iMesh < iNumMeshes; ++iMesh )
//	{
//		const aiMesh* pMesh = _pScene->mMeshes[ iMesh ];
//		LoadAIVertexData( iMesh, pMesh );
//		LoadAIBones( iMesh, pMesh );
//	}
//}
//void
//CAnimatedModel::LoadAIVertexData( unsigned int _iIndex, const aiMesh* _pMesh )
//{
//	m_iVertexCount = _pMesh->mNumVertices;
//	m_pAnimatedVertices = new TAnimatedVertex[ m_iVertexCount ];
//
//	for( int iVertex = 0; iVertex < m_iVertexCount; ++iVertex )
//	{
//		const aiVector3D* pPosition = &( _pMesh->mVertices[ iVertex ] );
//		const aiVector3D* pNormal = &( _pMesh->mNormals[ iVertex ] );
//		const aiVector3D* pTexCoord = &( _pMesh->mTextureCoords[ 0 ][ iVertex ] );
//
//		m_pAnimatedVertices[ iVertex ].pos = Math::Vector3( -pPosition->x, pPosition->y, -pPosition->z );
//		m_pAnimatedVertices[ iVertex ].normal = Math::Vector3( pNormal->x, pNormal->y, pNormal->z );
//		m_pAnimatedVertices[ iVertex ].texC = Math::Vector2( pTexCoord->x, pTexCoord->y );
//
//		m_pAnimatedVertices[ iVertex ].vecIDs = Math::Vector4( 0.0f, 0.0f, 0.0f, 0.0f );
//		m_pAnimatedVertices[ iVertex ].vecWeights = Math::Vector4( 0.0f, 0.0f, 0.0f, 0.0f );
//	}
//
//	m_iIndexCount = _pMesh->mNumFaces * 3;
//	m_pIndices = new unsigned int[ m_iIndexCount ];
//	int iCurrentFace = 0;
//	for( unsigned int iFace = 0; iFace < _pMesh->mNumFaces; ++iFace )
//	{
//		const aiFace& rFace = _pMesh->mFaces[ iFace ];
//		m_pIndices[ iCurrentFace++ ] = rFace.mIndices[ 0 ];
//		m_pIndices[ iCurrentFace++ ] = rFace.mIndices[ 1 ];
//		m_pIndices[ iCurrentFace++ ] = rFace.mIndices[ 2 ];
//	}
//}
//void
//CAnimatedModel::LoadAIBones( unsigned int _iMeshIndex, const aiMesh* _pMesh )
//{
//	m_iNumBones = 0;
//	for( unsigned int iBone = 0; iBone < _pMesh->mNumBones; ++iBone )
//	{
//		unsigned int iBoneIndex = 0;
//		std::string sBoneName = _pMesh->mBones[ iBone ]->mName.data;
//
//		if( m_mapBones.find( sBoneName ) == m_mapBones.end() )
//		{
//			iBoneIndex = m_iNumBones;
//			++m_iNumBones;
//			TBoneInfo newBone;
//			vecTempBones.push_back( newBone );
//		}
//		else
//		{
//			iBoneIndex = m_mapBones[ sBoneName ];
//		}
//
//		m_mapBones[ sBoneName ] = iBoneIndex;
//		ToDXMatrix( _pMesh->mBones[ iBone ]->mOffsetMatrix, vecTempBones[ iBoneIndex ].m_matBoneOffset );
//
//		for( unsigned int iWeight = 0; iWeight < _pMesh->mBones[ iBone ]->mNumWeights; ++iWeight )
//		{
//			unsigned int iVertex = _pMesh->mBones[ iBone ]->mWeights[ iWeight ].mVertexId;
//			float fWeight = _pMesh->mBones[ iBone ]->mWeights[ iWeight ].mWeight;
//			if( m_pAnimatedVertices[ iVertex ].vecWeights.x < fWeight )
//			{
//				m_pAnimatedVertices[ iVertex ].vecWeights.w = m_pAnimatedVertices[ iVertex ].vecWeights.z;
//				m_pAnimatedVertices[ iVertex ].vecIDs.w = m_pAnimatedVertices[ iVertex ].vecIDs.z;
//
//				m_pAnimatedVertices[ iVertex ].vecWeights.z = m_pAnimatedVertices[ iVertex ].vecWeights.y;
//				m_pAnimatedVertices[ iVertex ].vecIDs.z = m_pAnimatedVertices[ iVertex ].vecIDs.y;
//
//				m_pAnimatedVertices[ iVertex ].vecWeights.y = m_pAnimatedVertices[ iVertex ].vecWeights.x;
//				m_pAnimatedVertices[ iVertex ].vecIDs.y = m_pAnimatedVertices[ iVertex ].vecIDs.x;
//
//				m_pAnimatedVertices[ iVertex ].vecWeights.x = fWeight;
//				m_pAnimatedVertices[ iVertex ].vecIDs.x = static_cast<float>( iBoneIndex );
//			}
//			else if( m_pAnimatedVertices[ iVertex ].vecWeights.y < fWeight )
//			{
//				m_pAnimatedVertices[ iVertex ].vecWeights.w = m_pAnimatedVertices[ iVertex ].vecWeights.z;
//				m_pAnimatedVertices[ iVertex ].vecIDs.w = m_pAnimatedVertices[ iVertex ].vecIDs.z;
//
//				m_pAnimatedVertices[ iVertex ].vecWeights.z = m_pAnimatedVertices[ iVertex ].vecWeights.y;
//				m_pAnimatedVertices[ iVertex ].vecIDs.z = m_pAnimatedVertices[ iVertex ].vecIDs.y;
//
//				m_pAnimatedVertices[ iVertex ].vecWeights.y = fWeight;
//				m_pAnimatedVertices[ iVertex ].vecIDs.y = static_cast<float>( iBoneIndex );
//			}
//			else if( m_pAnimatedVertices[ iVertex ].vecWeights.z < fWeight )
//			{
//				m_pAnimatedVertices[ iVertex ].vecWeights.w = m_pAnimatedVertices[ iVertex ].vecWeights.z;
//				m_pAnimatedVertices[ iVertex ].vecIDs.w = m_pAnimatedVertices[ iVertex ].vecIDs.z;
//
//				m_pAnimatedVertices[ iVertex ].vecWeights.z = fWeight;
//				m_pAnimatedVertices[ iVertex ].vecIDs.z = static_cast<float>( iBoneIndex );
//			}
//			else if( m_pAnimatedVertices[ iVertex ].vecWeights.w < fWeight )
//			{
//				m_pAnimatedVertices[ iVertex ].vecWeights.w = fWeight;
//				m_pAnimatedVertices[ iVertex ].vecIDs.w = static_cast<float>( iBoneIndex );
//			}
//			else
//			{
//
//			}
//		}
//	}
//}
//void
//CAnimatedModel::BoneTransform( float _fTimeInSeconds, std::vector<Math::Matrix>& _rMatVector )
//{
//	Math::Matrix matIdentity;
//	D3DXMatrixIdentity( &matIdentity );
//
//	double fTicksPerSecond = m_pAIScene->mAnimations[ 0 ]->mTicksPerSecond;
//	double fTimeInTicks = _fTimeInSeconds * fTicksPerSecond;
//	float fAnimationTime = static_cast<float>( fmod( fTimeInTicks, m_pAIScene->mAnimations[ 0 ]->mDuration ) );
//
//	//Recursive node hierarchy matrix calculation
//	ReadNodeHierarchy( fAnimationTime, m_pAIScene->mRootNode, matIdentity );
//
//	_rMatVector.clear();
//	for( int iBone = 0; iBone < m_iNumBones; ++iBone )
//	{
//		//D3DXMatrixTranspose(&vecTempBones[iBone].m_matFinalTransformation, &vecTempBones[iBone].m_matFinalTransformation);
//		_rMatVector.push_back( vecTempBones[ iBone ].m_matFinalTransformation );
//	}
//}
//void
//CAnimatedModel::ReadNodeHierarchy( float _fAnimationTime, const aiNode* _pNode, const Math::Matrix& _rMatParent )
//{
//	std::string sNodeName = _pNode->mName.data;
//	const aiAnimation* pAnim = m_pAIScene->mAnimations[ 0 ];
//	Math::Matrix matTransform;
//	ToDXMatrix( _pNode->mTransformation, matTransform );
//	const aiNodeAnim* pNodeAnim = FindNodeAnim( pAnim, sNodeName );
//
//	if( pNodeAnim )
//	{
//		aiVector3D vecScaling;
//		CalcInterpolatedScaling( vecScaling, _fAnimationTime, pNodeAnim );
//
//		aiQuaternion quatRot;
//		CalcInterpolatedRotation( quatRot, _fAnimationTime, pNodeAnim );
//
//		aiVector3D vecTranslation;
//		CalcInterpolatedPosition( vecTranslation, _fAnimationTime, pNodeAnim );
//
//		D3DXMatrixTransformation( &matTransform,
//			NULL,
//			NULL,
//			&Math::Vector3( vecScaling.x, vecScaling.y, vecScaling.z ),
//			NULL,
//			&Math::Quaternion( quatRot.x, quatRot.y, quatRot.z, quatRot.w ),
//			&Math::Vector3( vecTranslation.x, vecTranslation.y, vecTranslation.z ) );
//		D3DXMatrixTranspose( &matTransform, &matTransform );
//	}
//
//	Math::Matrix matGlobal = _rMatParent * matTransform;
//	if( m_mapBones.find( sNodeName ) != m_mapBones.end() )
//	{
//		unsigned int iBoneIndex = m_mapBones[ sNodeName ];
//		vecTempBones[ iBoneIndex ].m_matFinalTransformation = m_matGlobalInverse * matGlobal * vecTempBones[ iBoneIndex ].m_matBoneOffset;
//	}
//
//	for( unsigned int iChild = 0; iChild < _pNode->mNumChildren; ++iChild )
//	{
//		ReadNodeHierarchy( _fAnimationTime, _pNode->mChildren[ iChild ], matGlobal );
//	}
//}
//const aiNodeAnim*
//CAnimatedModel::FindNodeAnim( const aiAnimation* _pAnimation, std::string _sNodeName )
//{
//	for( unsigned int iChannel = 0; iChannel < _pAnimation->mNumChannels; ++iChannel )
//	{
//		const aiNodeAnim* pNodeAnim = _pAnimation->mChannels[ iChannel ];
//
//		if( std::string( pNodeAnim->mNodeName.data ) == _sNodeName )
//		{
//			return pNodeAnim;
//		}
//	}
//
//	return NULL;
//}
//void
//CAnimatedModel::CalcInterpolatedPosition( aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim )
//{
//	if( pNodeAnim->mNumPositionKeys == 1 )
//	{
//		Out = pNodeAnim->mPositionKeys[ 0 ].mValue;
//		return;
//	}
//
//	unsigned int PositionIndex = FindPosition( AnimationTime, pNodeAnim );
//	unsigned int NextPositionIndex = ( PositionIndex + 1 );
//	float DeltaTime = (float)( pNodeAnim->mPositionKeys[ NextPositionIndex ].mTime - pNodeAnim->mPositionKeys[ PositionIndex ].mTime );
//	float Factor = ( AnimationTime - (float)pNodeAnim->mPositionKeys[ PositionIndex ].mTime ) / DeltaTime;
//	const aiVector3D& Start = pNodeAnim->mPositionKeys[ PositionIndex ].mValue;
//	const aiVector3D& End = pNodeAnim->mPositionKeys[ NextPositionIndex ].mValue;
//	aiVector3D Delta = End - Start;
//	Out = Start + Factor * Delta;
//}
//
//
//void
//CAnimatedModel::CalcInterpolatedRotation( aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim )
//{
//	// we need at least two values to interpolate...
//	if( pNodeAnim->mNumRotationKeys == 1 )
//	{
//		Out = pNodeAnim->mRotationKeys[ 0 ].mValue;
//		return;
//	}
//
//	unsigned int RotationIndex = FindRotation( AnimationTime, pNodeAnim );
//	unsigned int NextRotationIndex = ( RotationIndex + 1 );
//	float DeltaTime = (float)( pNodeAnim->mRotationKeys[ NextRotationIndex ].mTime - pNodeAnim->mRotationKeys[ RotationIndex ].mTime );
//	float Factor = ( AnimationTime - (float)pNodeAnim->mRotationKeys[ RotationIndex ].mTime ) / DeltaTime;
//	const aiQuaternion& StartRotationQ = pNodeAnim->mRotationKeys[ RotationIndex ].mValue;
//	const aiQuaternion& EndRotationQ = pNodeAnim->mRotationKeys[ NextRotationIndex ].mValue;
//	aiQuaternion::Interpolate( Out, StartRotationQ, EndRotationQ, Factor );
//	Out = Out.Normalize();
//}
//void
//CAnimatedModel::CalcInterpolatedScaling( aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim )
//{
//	if( pNodeAnim->mNumScalingKeys == 1 )
//	{
//		Out = pNodeAnim->mScalingKeys[ 0 ].mValue;
//		return;
//	}
//
//	unsigned int ScalingIndex = FindScaling( AnimationTime, pNodeAnim );
//	unsigned int NextScalingIndex = ( ScalingIndex + 1 );
//	float DeltaTime = (float)( pNodeAnim->mScalingKeys[ NextScalingIndex ].mTime - pNodeAnim->mScalingKeys[ ScalingIndex ].mTime );
//	float Factor = ( AnimationTime - (float)pNodeAnim->mScalingKeys[ ScalingIndex ].mTime ) / DeltaTime;
//	const aiVector3D& Start = pNodeAnim->mScalingKeys[ ScalingIndex ].mValue;
//	const aiVector3D& End = pNodeAnim->mScalingKeys[ NextScalingIndex ].mValue;
//	aiVector3D Delta = End - Start;
//	Out = Start + Factor * Delta;
//}
//unsigned int
//CAnimatedModel::FindPosition( float AnimationTime, const aiNodeAnim* pNodeAnim )
//{
//	for( unsigned int i = 0; i < pNodeAnim->mNumPositionKeys - 1; i++ )
//	{
//		if( AnimationTime < (float)pNodeAnim->mPositionKeys[ i + 1 ].mTime )
//		{
//			return i;
//		}
//	}
//	return 0;
//}
//
//
//unsigned int
//CAnimatedModel::FindRotation( float AnimationTime, const aiNodeAnim* pNodeAnim )
//{
//	for( unsigned int i = 0; i < pNodeAnim->mNumRotationKeys - 1; i++ )
//	{
//		if( AnimationTime < (float)pNodeAnim->mRotationKeys[ i + 1 ].mTime )
//		{
//			return i;
//		}
//	}
//	return 0;
//}
//
//
//unsigned int
//CAnimatedModel::FindScaling( float AnimationTime, const aiNodeAnim* pNodeAnim )
//{
//	for( unsigned int i = 0; i < pNodeAnim->mNumScalingKeys - 1; i++ )
//	{
//		if( AnimationTime < (float)pNodeAnim->mScalingKeys[ i + 1 ].mTime )
//		{
//			return i;
//		}
//	}
//	return 0;
//}
///**
//*
//* CAnimatedModel class CreateModelBuffers
//* (Task ID: n/a)
//*
//* @author Christopher Howlett
//*
//*/
//void
//CAnimatedModel::CreateVertexBuffer( ID3D11Device* _pDevice )
//{
//	D3D11_BUFFER_DESC tVertexBufferDesc;
//	tVertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
//	tVertexBufferDesc.ByteWidth = sizeof( TAnimatedVertex )* m_iVertexCount;
//	tVertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
//	tVertexBufferDesc.CPUAccessFlags = 0;
//	tVertexBufferDesc.MiscFlags = 0;
//	tVertexBufferDesc.StructureByteStride = 0;
//
//	D3D11_SUBRESOURCE_DATA tVertexData;
//	tVertexData.pSysMem = m_pAnimatedVertices;
//	tVertexData.SysMemPitch = 0;
//	tVertexData.SysMemSlicePitch = 0;
//
//	_pDevice->CreateBuffer( &tVertexBufferDesc, &tVertexData, &m_pVertexBuffer );
//}
//void
//CAnimatedModel::ToDXMatrix( const aiMatrix4x4& _rAIMat, Math::Matrix& _rOutput )
//{
//	_rOutput._11 = _rAIMat.a1;
//	_rOutput._12 = _rAIMat.a2;
//	_rOutput._13 = _rAIMat.a3;
//	_rOutput._14 = _rAIMat.a4;
//
//	_rOutput._21 = _rAIMat.b1;
//	_rOutput._22 = _rAIMat.b2;
//	_rOutput._23 = _rAIMat.b3;
//	_rOutput._24 = _rAIMat.b4;
//
//	_rOutput._31 = _rAIMat.c1;
//	_rOutput._32 = _rAIMat.c2;
//	_rOutput._33 = _rAIMat.c3;
//	_rOutput._34 = _rAIMat.c4;
//
//	_rOutput._41 = _rAIMat.d1;
//	_rOutput._42 = _rAIMat.d2;
//	_rOutput._43 = _rAIMat.d3;
//	_rOutput._44 = _rAIMat.d4;
//}
///**
//*
//* CAnimatedModel class CalculateBoundingBox
//* (Task ID: n/a)
//*
//* @author Christopher Howlett
//*
//*/
//void
//CAnimatedModel::CalculateBoundingBox()
//{
//	if( m_pAnimatedVertices )
//	{
//		m_pBoundingBox = new CBoundingBox();
//		m_pBoundingBox->Initialise( m_pAnimatedVertices, m_iVertexCount );
//		m_fRadius = m_pBoundingBox->GetRadius();
//	}
//}