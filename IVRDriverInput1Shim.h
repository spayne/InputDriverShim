#pragma once
// InputDriver1Shim.cpp : compatible with 
//


using namespace vr;

//copy from an 1.15 version of openvr_driver.h
namespace vr115
{
	static const char * const IVRDriverInput_Version = "IVRDriverInput_001";

	class IVRDriverInput
	{
	public:

		/** Creates a boolean input component for the device */
		virtual EVRInputError CreateBooleanComponent(PropertyContainerHandle_t ulContainer, const char *pchName, VRInputComponentHandle_t *pHandle) = 0;

		/** Updates a boolean component */
		virtual EVRInputError UpdateBooleanComponent(VRInputComponentHandle_t ulComponent, bool bNewValue, double fTimeOffset) = 0;

		/** Creates a scalar input component for the device */
		virtual EVRInputError CreateScalarComponent(PropertyContainerHandle_t ulContainer, const char *pchName, VRInputComponentHandle_t *pHandle, EVRScalarType eType, EVRScalarUnits eUnits) = 0;

		/** Updates a boolean component */
		virtual EVRInputError UpdateScalarComponent(VRInputComponentHandle_t ulComponent, float fNewValue, double fTimeOffset) = 0;

		/** Creates a haptic component for the device */
		virtual EVRInputError CreateHapticComponent(PropertyContainerHandle_t ulContainer, const char *pchName, VRInputComponentHandle_t *pHandle) = 0;

		/** Creates a skeleton component. */
		virtual EVRInputError CreateSkeletonComponent(PropertyContainerHandle_t ulContainer, const char *pchName, const char *pchSkeletonPath, const char *pchBasePosePath, const VRBoneTransform_t *pGripLimitTransforms, uint32_t unGripLimitTransformCount, VRInputComponentHandle_t *pHandle) = 0;

		/** Updates a skeleton component. */
		virtual EVRInputError UpdateSkeletonComponent(VRInputComponentHandle_t ulComponent, const VRBoneTransform_t *pTransforms, uint32_t unTransformCount) = 0;

	};
}


	using namespace vr;

	class IVRDriverInputShim001 : public vr115::IVRDriverInput
	{
	public:
		vr115::IVRDriverInput * m_real;

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

		EVRInputError UpdateSkeletonComponent(VRInputComponentHandle_t ulComponent, const VRBoneTransform_t *pTransforms, uint32_t unTransformCount) override
		{
			EVRInputError e = m_real->UpdateSkeletonComponent(ulComponent, pTransforms, unTransformCount);
			return e;
		}
	};
