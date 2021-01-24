# InputDriverShim
a method to insert code between the openvr system and a real driver


This shim allows calls between the driver and a real IVRContext to be intercepted
Normal communication
   IVRDriverContext -> a real driver
Communication path using the IVRDriverContextShim
   IVRDRiverContext <-> IVRDriverContextShim <-> a real driver
