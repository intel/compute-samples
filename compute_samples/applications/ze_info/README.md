# ze_info
This sample demonstrates how to query Level Zero platform about its properties.

## Description
Level Zero platform consists of:
* `drivers` representing collections of physical devices.
* `devices` representing physical devices.

Device discovery is done by calling `zeDriverGet` and `zeDeviceGet` as shown in the example below:

```c
std::vector<ze_driver_handle_t> get_drivers() {
  uint32_t count = 0;
  zeDriverGet(&count, nullptr);

  std::vector<ze_driver_handle_t> drivers(count);
  zeDriverGet(&count, drivers.data());

  return drivers;
}

std::vector<ze_device_handle_t>
get_driver_devices(const ze_driver_handle_t driver) {
  uint32_t count = 0;
  zeDeviceGet(driver, &count, nullptr);

  std::vector<ze_device_handle_t> devices(count);
  zeDeviceGet(driver, &count, devices.data());

  return devices;
}
```

Each `driver` and `device` has its own properties that can be queried. To do so a dedicated output parameter must be passed to an appropriate Level Zero function. For example, below code snippet shows how to query `device` properties:

```c
ze_device_properties_t get_device_properties(const ze_device_handle_t device) {
  ze_device_properties_t properties = {ZE_DEVICE_PROPERTIES_VERSION_CURRENT};
  zeDeviceGetProperties(device, &properties);
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
