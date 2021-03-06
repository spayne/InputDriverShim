// InputDriverShim.cpp : Defines the exported functions for the DLL application.
//

using namespace vr;

class IVRDriverInputLatestShim : public IVRDriverInput
{
public:
	IVRDriverInput *m_real;

	EVRInputError CreateBooleanComponent(PropertyContainerHandle_t ulContainer, const char *pchName, VRInputComponentHandle_t *pHandle) override
	{
		EVRInputError e = m_real->CreateBooleanComponent(ulContainer, pchName, pHandle);
		dprintf("CreateBooleanComponent %d %s\n", *pHandle, pchName);
		return e;
	}

	EVRInputError UpdateBooleanComponent(VRInputComponentHandle_t ulComponent, bool bNewValue, double fTimeOffset) override
	{
		//dprintf("UpdateBooleanComponent %d %d\n", ulComponent, bNewValue);
		EVRInputError e = m_real->UpdateBooleanComponent(ulComponent, bNewValue, fTimeOffset);
		return e;
	}

	EVRInputError CreateScalarComponent(PropertyContainerHandle_t ulContainer, const char *pchName, VRInputComponentHandle_t *pHandle, EVRScalarType eType, EVRScalarUnits eUnits) override
	{
		EVRInputError e = m_real->CreateScalarComponent(ulContainer, pchName, pHandle, eType, eUnits);
		dprintf("CreateScalarComponent %d %s %d %d\n", *pHandle, pchName, eType, eUnits);
		return e;
	}

	EVRInputError UpdateScalarComponent(VRInputComponentHandle_t ulComponent, float fNewValue, double fTimeOffset) override
	{
		//dprintf("UpdateScalarComponent %d %f\n", ulComponent, fNewValue);
		EVRInputError e = m_real->UpdateScalarComponent(ulComponent, fNewValue, fTimeOffset);
		return e;
	}

	EVRInputError CreateHapticComponent(PropertyContainerHandle_t ulContainer, const char *pchName, VRInputComponentHandle_t *pHandle) override
	{
		
		EVRInputError e = m_real->CreateHapticComponent(ulContainer, pchName, pHandle);
		dprintf("CreateHapticComponent %d %s\n", *pHandle, pchName);
		return e;
	}

	EVRInputError CreateSkeletonComponent(PropertyContainerHandle_t ulContainer, const char *pchName, const char *pchSkeletonPath, const char *pchBasePosePath, const VRBoneTransform_t *pGripLimitTransforms, uint32_t unGripLimitTransformCount, VRInputComponentHandle_t *pHandle) override
	{
		EVRInputError e = m_real->CreateSkeletonComponent(ulContainer, pchName, pchSkeletonPath, pchBasePosePath, pGripLimitTransforms, unGripLimitTransformCount, pHandle);
		dprintf("CreateSkeletonComponent %d %s %s %s %d\n", pHandle, pchName, pchSkeletonPath, pchBasePosePath, unGripLimitTransformCount);
		return e;
	}

	EVRInputError UpdateSkeletonComponent(VRInputComponentHandle_t ulComponent, EVRSkeletalMotionRange eMotionRange, const VRBoneTransform_t *pTransforms, uint32_t unTransformCount) override
	{
		EVRInputError e = m_real->UpdateSkeletonComponent(ulComponent, eMotionRange, pTransforms, unTransformCount);
		return e;
	}

	virtual void extra1()
	{
		dprintf("extra1\n");
	}
	virtual void extra2()
	{
		dprintf("extra2\n");
	}
	virtual void extra3()
	{
		dprintf("extra3\n");
	}
	virtual void extra4()
	{
		dprintf("extra4\n");
	}
};

