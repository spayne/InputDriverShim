// InputDriverShim.cpp : Defines the exported functions for the DLL application.
//

#include <windows.h>
#include <tchar.h>

#if defined(_WIN32)
#define strtok_r strtok_s
#define HMD_DLL_EXPORT extern "C" __declspec( dllexport )
#define HMD_DLL_IMPORT extern "C" __declspec( dllimport )
#elif defined(__GNUC__) || defined(COMPILER_GCC) || defined(__APPLE__)
#define HMD_DLL_EXPORT extern "C" __attribute__((visibility("default")))
#define HMD_DLL_IMPORT extern "C" 
#else
#error "Unsupported Platform."
#endif

// do this first to get the latest copy and all of it's
// includes
#include <openvr_driver.h>
#include "dprintf.h"
#include "string.h"

#include "IVRDriverInput1Shim.h"
#include "IVRDriverInputLatestShim.h"
#include "IVRServerDriverHostLatestShim.h"
#include "PoseListenerRegistryImpl.h"

using namespace vr;

// use interitance to derive a special type of IVRDriverContext - the Shim
// this shim allows calls between the driver and a real IVRContext to be intercepted
// Normal communication
//   IVRDriverContext -> a real driver
// Communication path using the IVRDriverContextShim
//   IVRDRiverContext <-> IVRDriverContextShim <-> a real driver
//
class IVRDriverContextShim : public IVRDriverContext
{
public:
	IVRDriverInputLatestShim m_input_shim;
	IVRDriverInputShim001 m_input_shim_001;
	IVRDriverContext *real_driver_context;
	IVRServerDriverHostLatestShim m_server_driver_host_shim;

	// Responisibility: return a shimmed IVRDriver_Input when the real driver asks for one.
	// Note:
	//   the driver is going to ask for a specific IVRDriverInput version. 
	//   the version requested may or may not be the one that is compiled with this shim - so there is a
	//   possibility of failure.
	virtual void *GetGenericInterface(const char *pchInterfaceVersion, EVRInitError *peError = nullptr)
	{
		dprintf("IVRDriverContextShim::GetGenericInterface for %s\n", pchInterfaceVersion);
		void *real = real_driver_context->GetGenericInterface(pchInterfaceVersion, peError);

		// intercept ServerDriverHost interfaces
		if (strcmp(IVRServerDriverHost_Version, pchInterfaceVersion) == 0)
		{
			dprintf("Info. returning shim for %s\n", pchInterfaceVersion);
			m_server_driver_host_shim.m_real = (IVRServerDriverHost *)real;
			return &m_server_driver_host_shim;
		}

		// intercept input drivers
		if (strcmp(IVRDriverInput_Version, pchInterfaceVersion) == 0)
		{
			dprintf("Info. returning shim for %s\n", pchInterfaceVersion);
			m_input_shim.m_real = (IVRDriverInput *)real;
			return &m_input_shim;
		}
		// is it an older version?
		if (strcmp(vr115::IVRDriverInput_Version, pchInterfaceVersion) == 0)
		{
			dprintf("Info. returning shim for older version %s\n", pchInterfaceVersion);
			m_input_shim_001.m_real = (vr115::IVRDriverInput *) real;
			return &m_input_shim_001;
		}
		else
		{
			dprintf("Info. Not shimming %s. returning vrserver interface\n", pchInterfaceVersion);
			return real;
		}
	}

	/** Returns the property container handle for this driver */
	virtual DriverHandle_t GetDriverHandle()
	{
		return real_driver_context->GetDriverHandle();
	}
};

class IServerTrackedDeviceProvider4Shim : public IServerTrackedDeviceProvider
{
public:
	IServerTrackedDeviceProvider *real_device_provider;
	
	IVRDriverContextShim m_driver_context_shim;

	EVRInitError Init(IVRDriverContext *pDriverContext) override
	{
		dprintf("IServerTrackedDeviceProvider4Shim::Init\n");
		m_driver_context_shim.real_driver_context = pDriverContext;

		EVRInitError err = real_device_provider->Init(&m_driver_context_shim);
		dprintf(" returning %d from real device provider->Init\n", err);
		return err;
	}

	void Cleanup() override
	{
		return real_device_provider->Cleanup();
	}

	const char * const *GetInterfaceVersions() override
	{
		return real_device_provider->GetInterfaceVersions();
	}

	void RunFrame() override
	{
		return real_device_provider->RunFrame();
	}

	bool ShouldBlockStandbyMode() override
	{
		return real_device_provider->ShouldBlockStandbyMode();
	}

	void EnterStandby() override
	{
		return real_device_provider->EnterStandby();
	}

	void LeaveStandby() override
	{
		return real_device_provider->LeaveStandby();
	}
};


static HINSTANCE hGetProcIDDLL;

typedef void *(__stdcall *realfactory_t)(const char *pInterfaceName, int *pReturnCode);
static realfactory_t factory_fn;
static IServerTrackedDeviceProvider4Shim m_device_provider_shim;



HMD_DLL_EXPORT void *HmdDriverFactory(const char *pInterfaceName, int *pReturnCode)
{
	dprintf("HmdDriverFactory %s\n", pInterfaceName);

	if (hGetProcIDDLL == nullptr)
	{
		dprintf("about to load library\n");
		hGetProcIDDLL =LoadLibrary(_T("D:\\Program Files (x86)\\Steam\\steamapps\\common\\SteamVR\\drivers\\lighthouse\\bin\\win64\\real_driver_lighthouse.dll"));
		dprintf("got %d\n", hGetProcIDDLL);
		

		if (!hGetProcIDDLL)
		{
			dprintf("LastError %d\n", GetLastError());
			exit(1);
		}

		if (hGetProcIDDLL)
		{
			factory_fn = (realfactory_t)GetProcAddress(hGetProcIDDLL, "HmdDriverFactory");
			dprintf("got factoryfn %d\n", factory_fn);
		}
	}

	dprintf("caller requested %s\n", pInterfaceName);
	void *real = factory_fn(pInterfaceName, pReturnCode);

	if (strcmp(pInterfaceName, "IServerTrackedDeviceProvider_004") == 0)
	{
		m_device_provider_shim.real_device_provider = (IServerTrackedDeviceProvider*) real;
		dprintf("returning %p\n", &m_device_provider_shim);
		return &m_device_provider_shim;
	}
	else if (strcmp(pInterfaceName, IPoseListenerRegistry_Version) == 0)
	{
		dprintf("returning %s:%p\n", pInterfaceName, &g_pose_listener_registry);
		return &g_pose_listener_registry;
	}
	else
	{
		dprintf("returning %p\n", real);
		return real;
	}
}

