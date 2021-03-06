#include "PoseListenerRegistryImpl.h"

PoseListenerRegistryImpl g_pose_listener_registry;

void PoseListenerRegistryImpl::RegisterPoseUpdateListener(uint32_t which_device, PoseCallback cb, void *user_data)
{
	pose_listeners_mutex.lock();
	pose_listeners.push_back(PoseListener(which_device, cb, user_data));
	pose_listeners_mutex.unlock();
}

void PoseListenerRegistryImpl::DeregisterPoseUpdateListener(uint32_t which_device, PoseCallback cb, void *user_data)
{
	pose_listeners_mutex.lock();
	for (int i = 0; i < pose_listeners.size(); i++)
	{
		if (which_device == pose_listeners[i].which_device &&
			cb == pose_listeners[i].callback &&
			user_data == pose_listeners[i].user_data)
		{
			pose_listeners.erase(pose_listeners.begin() + i);
			break;
		}
	}
	pose_listeners_mutex.unlock();
}

void PoseListenerRegistryImpl::NotifyPoseListeners(uint32_t unWhichDevice, const vr::DriverPose_t & newPose)
{
	pose_listeners_mutex.lock();
	for (int i = 0; i < pose_listeners.size(); i++)
	{
		if (pose_listeners[i].which_device == unWhichDevice)
		{
			pose_listeners[i].callback(unWhichDevice, newPose, pose_listeners[i].user_data);
		}
	}
	pose_listeners_mutex.unlock();
}
