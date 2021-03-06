#pragma once

#include "IPoseListenerRegistry.h"

#include <vector>
#include <mutex>

struct PoseListener
{
	PoseListener(uint32_t which_device_in, IPoseListenerRegistry::PoseCallback callback_in, void *user_data_in)
		: which_device(which_device_in),
		callback(callback_in),
		user_data(user_data_in)
	{}

	uint32_t which_device;
	IPoseListenerRegistry::PoseCallback callback;
	void *user_data;
};

class PoseListenerRegistryImpl : IPoseListenerRegistry
{
public:
	void RegisterPoseUpdateListener(uint32_t which_device, PoseCallback cb, void *user_data) override;
	void DeregisterPoseUpdateListener(uint32_t which_device, PoseCallback cb, void *user_data) override;
	void NotifyPoseListeners(uint32_t unWhichDevice, const vr::DriverPose_t & newPose);

	std::vector<PoseListener> pose_listeners;
	std::mutex pose_listeners_mutex;
};

extern PoseListenerRegistryImpl g_pose_listener_registry;