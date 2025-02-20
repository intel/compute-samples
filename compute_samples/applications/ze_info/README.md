# ze_info
This sample demonstrates how to query Level Zero platform about its properties.

## Description
Level Zero platform consists of:
* `drivers` representing collections of physical devices.
* `devices` representing physical devices.

Device discovery is done by calling `zeInitDrivers` and `zeDeviceGet` as shown in the example below:

```c++
std::vector<ze_driver_handle_t> get_drivers() {
  ze_init_driver_type_desc_t desc = {ZE_STRUCTURE_TYPE_INIT_DRIVER_TYPE_DESC};
  desc.pNext = nullptr;
  desc.flags = UINT32_MAX; // all driver types requested

  uint32_t count = 0;
  zeInitDrivers(&count, nullptr, &desc);

  std::vector<ze_driver_handle_t> drivers(count);
  zeInitDrivers(&count, drivers.data(), &desc);

  return drivers;
}

std::vector<ze_device_handle_t> get_driver_devices(ze_driver_handle_t driver) {
  auto result = ZE_RESULT_SUCCESS;

  uint32_t count = 0;
  result = zeDeviceGet(driver, &count, nullptr);

  std::vector<ze_device_handle_t> devices(count);
  result = zeDeviceGet(driver, &count, devices.data());

  return devices;
}
```

Each `driver` and `device` has its own properties that can be queried. To do so a dedicated output parameter must be passed to an appropriate Level Zero function. For example, below code snippet shows how to query `device` properties:

```c++
ze_device_properties_t get_device_properties(ze_device_handle_t device) {
  ze_device_properties_t properties = {ZE_STRUCTURE_TYPE_DEVICE_PROPERTIES};
  const auto result = zeDeviceGetProperties(device, &properties);
  return properties;
}
```

## Usage

By default `ze_info` simply prints platform information to the console:

    ze_info

It is also possible to save all properties in JSON format:

    ze_info --output=info.json

Additional options can be found in `help` message:

    ze_info --help

## References
1. https://spec.oneapi.com/versions/latest/oneL0/core/PROG.html
1. https://spec.oneapi.com/versions/latest/oneL0/core/api.html#driver-functions
1. https://spec.oneapi.com/versions/latest/oneL0/core/api.html#device-functions
