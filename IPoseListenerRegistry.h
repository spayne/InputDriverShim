#pragma once

#include <openvr_driver.h>

class IPoseListenerRegistry
{
public:
	typedef void(*PoseCallback)(uint32_t which_device, const vr::DriverPose_t & newPose, void *user_data);
	virtual void RegisterPoseUpdateListener(uint32_t which_device, PoseCallback cb, void *user_data) = 0;
	virtual void DeregisterPoseUpdateListener(uint32_t which_device, PoseCallback cb, void *user_data) = 0;
};

static const char *IPoseListenerRegistry_Version = "IPoseListenerRegistry_001";