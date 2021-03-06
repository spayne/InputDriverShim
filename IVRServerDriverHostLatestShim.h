using namespace vr;

#include <mutex>
#include <vector>
#include <openvr_driver.h>
#include "dprintf.h"
#include "PoseListenerRegistryImpl.h"

class IVRServerDriverHostLatestShim : public IVRServerDriverHost
{
public:
	IVRServerDriverHost *m_real;
//	std::vector<DeviceDetails> m_devices;

	bool TrackedDeviceAdded(const char *pchDeviceSerialNumber, ETrackedDeviceClass eDeviceClass, ITrackedDeviceServerDriver *pDriver) override
	{
		dprintf("TrackedDeviceAdded %s %d %p\n", pchDeviceSerialNumber, eDeviceClass, pDriver);
		//m_devices.push_back(DeviceDetails(pchDeviceSerialNumber, eDeviceClass, pDriver));
		return m_real->TrackedDeviceAdded(pchDeviceSerialNumber, eDeviceClass, pDriver);
	}

	void TrackedDevicePoseUpdated(uint32_t unWhichDevice, const DriverPose_t & newPose, uint32_t unPoseStructSize) override
	{
	// high traffic
	//	dprintf("TrackedDevicePoseUpdated %d", unWhichDevice);
		m_real->TrackedDevicePoseUpdated(unWhichDevice, newPose, unPoseStructSize);
		g_pose_listener_registry.NotifyPoseListeners(unWhichDevice, newPose);
	}

	void VsyncEvent(double vsyncTimeOffsetSeconds) override
	{
		dprintf("VsyncEvent");
		m_real->VsyncEvent(vsyncTimeOffsetSeconds);
	}

	void VendorSpecificEvent(uint32_t unWhichDevice, vr::EVREventType eventType, const VREvent_Data_t & eventData, double eventTimeOffset) override
	{
		dprintf("VendorSpecificEvent\n");
		m_real->VendorSpecificEvent(unWhichDevice, eventType, eventData, eventTimeOffset);
	}

	bool IsExiting() override
	{
		dprintf("IsExiting\n");
		return m_real->IsExiting();
	}

	bool PollNextEvent(VREvent_t *pEvent, uint32_t uncbVREvent) override
	{
		// high traffic
		//dprintf("PollNextEvent");
		return m_real->PollNextEvent(pEvent, uncbVREvent);
	}

	void GetRawTrackedDevicePoses(float fPredictedSecondsFromNow, TrackedDevicePose_t *pTrackedDevicePoseArray, uint32_t unTrackedDevicePoseArrayCount) override
	{
		dprintf("GetRawTrackedDevicePoses\n");
		m_real->GetRawTrackedDevicePoses(fPredictedSecondsFromNow, pTrackedDevicePoseArray, unTrackedDevicePoseArrayCount);
	}

	void TrackedDeviceDisplayTransformUpdated(uint32_t unWhichDevice, HmdMatrix34_t eyeToHeadLeft, HmdMatrix34_t eyeToHeadRight) override
	{
		dprintf("TrackedDeviceDisplayTransformUpdated\n");
		m_real->TrackedDeviceDisplayTransformUpdated(unWhichDevice, eyeToHeadLeft, eyeToHeadRight);
	}

};

	

