# test_cl_command_queue_families

## Overview

Tests for [cl_intel_command_queue_families](https://github.com/KhronosGroup/OpenCL-Docs/blob/master/extensions/cl_intel_command_queue_families.asciidoc) OpenCL extension.

## Tests

1. Basic
    - [x] Device queries
        - [x] `CL_DEVICE_QUEUE_FAMILY_PROPERTIES_INTEL`
            - [x] `properties` field is the same as the one returned by `CL_DEVICE_QUEUE_ON_HOST_PROPERTIES`.
            - [x] `capabilities` contains valid values.
            - [x] `count` is greater than 1.
            - [x] `name` is not empty.
            - [x] `CL_QUEUE_DEFAULT_CAPABILITIES_INTEL` is reported by at least one family.
    - [x] Command queue queries
        - [x] `CL_QUEUE_FAMILY_INTEL` is the same as the one passed to `clCreateCommandQueueWithProperties`.
        - [x] `CL_QUEUE_INDEX_INTEL` is the same as the one passed to `clCreateCommandQueueWithProperties`.
        - [x] `CL_QUEUE_FAMILY_INTEL` is one of families with `CL_QUEUE_DEFAULT_CAPABILITIES_INTEL` when a default command queue is used.
        - [x] `CL_QUEUE_INDEX_INTEL` is between `0` and `count` when a default command queue is used.
    - [ ] Capabilities
        - [x] `CL_QUEUE_CAPABILITY_TRANSFER_BUFFER_INTEL`
            - [x] Call `clEnqueueReadBuffer`.
            - [x] Call `clEnqueueWriteBuffer`.
            - [x] Call `clEnqueueCopyBuffer`.
        - [x] `CL_QUEUE_CAPABILITY_TRANSFER_BUFFER_RECT_INTEL`
            - [x] Call `clEnqueueReadBufferRect`.
            - [x] Call `clEnqueueWriteBufferRect`.
            - [x] Call `clEnqueueCopyBufferRect`.
        - [x] `CL_QUEUE_CAPABILITY_MAP_BUFFER_INTEL`
            - [x] Call `clEnqueueMapBuffer`.
            - [x] Call `clEnqueueUnmapMemObject`.
        - [x] `CL_QUEUE_CAPABILITY_FILL_BUFFER_INTEL`
            - [x] Call `clEnqueueFillBuffer`.
        - [x] `CL_QUEUE_CAPABILITY_TRANSFER_IMAGE_INTEL`
            - [x] Call `clEnqueueReadImage`.
            - [x] Call `clEnqueueWriteImage`.
            - [x] Call `clEnqueueCopyImage`.
        - [x] `CL_QUEUE_CAPABILITY_MAP_IMAGE_INTEL`
            - [x] Call `clEnqueueMapImage `.
            - [x] Call `clEnqueueUnmapMemObject`.
        - [x] `CL_QUEUE_CAPABILITY_FILL_IMAGE_INTEL`
            - [x] Call `clEnqueueFillImage `.
        - [x] `CL_QUEUE_CAPABILITY_TRANSFER_BUFFER_IMAGE_INTEL`
            - [x] Call `clEnqueueCopyBufferToImage `.
        - [x] `CL_QUEUE_CAPABILITY_TRANSFER_IMAGE_BUFFER_INTEL`
            - [x] Call `clEnqueueCopyImageToBuffer `.
        - [x] `CL_QUEUE_CAPABILITY_MARKER_INTEL`
            - [x] Call `clEnqueueMarkerWithWaitList`.
        - [x] `CL_QUEUE_CAPABILITY_BARRIER_INTEL`
            - [x] Call `clEnqueueBarrierWithWaitList`.
        - [ ] `CL_QUEUE_CAPABILITY_KERNEL_INTEL`
            - [x] Call `clEnqueueNDRangeKernel`.
            - [x] Call `clEnqueueTask`.
            - [ ] Call `clEnqueueNativeKernel`.
        - [x] `CL_QUEUE_CAPABILITY_CREATE_SINGLE_QUEUE_EVENTS_INTEL`
            - [x] Get profiling info from an event.
            - [x] Pass an event to `clWaitForEvents`.
        - [x] `CL_QUEUE_CAPABILITY_CREATE_CROSS_QUEUE_EVENTS_INTEL`
            - [x] Get profiling info from an event.
            - [x] Pass an event to `clWaitForEvents`.
            - [x] `CL_QUEUE_CAPABILITY_CREATE_SINGLE_QUEUE_EVENTS_INTEL` is also supported.
        - [x] `CL_QUEUE_CAPABILITY_SINGLE_QUEUE_EVENT_WAIT_LIST_INTEL`
            - [x] Pass an event to a wait list in the same command queue.
        - [x] `CL_QUEUE_CAPABILITY_CROSS_QUEUE_EVENT_WAIT_LIST_INTEL`
            - [x] Pass an event to a wait list in a different command queue.
            - [x] `CL_QUEUE_CAPABILITY_SINGLE_QUEUE_EVENT_WAIT_LIST_INTEL` is also supported.
        - [x] `CL_QUEUE_DEFAULT_CAPABILITIES_INTEL`
            - [x] All above scenarios.
1. Advanced
    - [x] Concurrent execution
        - [x] Enqueue multiple commands into multiple command queues from all available command queue families.
    - [x] Object sharing
        - [x] Buffer based producer/consumer pattern between command queue families.
    - [ ] Sub-devices
        - [ ] Run all above scenarios on sub-devices.

## Selecting tests

* To list all of available test cases please use `--gtest_list_tests` flag.
* To run only selected test case please use `--gtest_filter` flag.

More details can be found in [Google Test documentation]([https://github.com/google/googletest/blob/master/googletest/docs/advanced.md#selecting-tests).
