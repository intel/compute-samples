# Subgroup Image Copy

This tutorial builds on the Subgroups concepts demonstrated in the Subgroups Visualization
tutorial.  In this tutorial you will use Subgroup functions to improve the performance of a
simple 8-bit image copy.

This tutorial requires support for the `cl_intel_subgroups` extension.  Because the
application compiles for an OpenCL 2.0 device (using `-cl-std=CL2.0`) by default, some
modifications may be needed to run it on an OpenCL 1.2 device.

## Intel Subgroup Capabilities

Intel Subgroup capabilities are exposed by the Intel [cl_intel_subgroups][cl_intel_subgroups]
extension.  The Intel Subgroups extension builds on the core Subgroup capabilities by adding
generalized data interchange functions (`intel_sub_group_shuffle` and others), and block
reads and writes, for both buffers and images (`intel_sub_group_block_read` and `intel_sub_group_block_write`).

## Copying an Image with Block Reads and Writes

The goal of this tutorial is improve the performance of a simple kernel that copies a
single-channel 8-bit image using Subgroup block reads and writes.  You will do this by
editing `subgroup_imagecopy_kernel.cl`.

By default, the kernel in this file loads an image using the standard OpenCL `read_image`
and `write_image` built-in functions.  Four pixels are copied per work item, so four
calls are needed to read the data, and four calls are needed to write the data.

Change the kernel in this file to copy the image using Subgroup block reads and writes
instead.  Note that the Subgroup block reads and writes process 32-bits of data at a
time.  Since the image is a single-channel 8-bit image, this means that only one call
is required to read the block of data, and only one call is needed to write the data.

By default, the application copies the image 1000 times, and prints how long it took to
execute.  Pay most attention to the Image Copy Execution time, not the Total time,
because the Total time, because the Total time also includes the time spent reading the
input image from a file and writing the output image to a file.  Do you see a performance
improvement using Subgroup block reads and writes?

## Extra Credit

Here are some additional ideas to experiment with:

* Use the flexible block reads and writes from [cl_intel_media_block_io][cl_intel_media_block_io]
  to perform the copy more flexibly.  Hint: It is easier to use the flexible block reads and writes
  if you use the "required Subgroup size" kernel attribute to specify a fixed Subgroup size.
* Modify the kernel and host code to process multiple image rows per work item.  Does this
  improve performance even more?
* Don't just copy the image, instead flip it vertically (easier) or horizontally (harder).
  Hint: To flip the image horizontally, you'll likely want to use Subgroup shuffles to flip the
  data within the Subgroup.

## Usage
    subgroups_imagecopy

----

Intel and the Intel logo are trademarks of Intel Corporation or its subsidiaries in the U.S. and/or other countries.

*Other names and brands may be claimed as the property of others.

OpenCL and the OpenCL logo are trademarks of Apple Inc. used by permission by Khronos.

[cl_intel_media_block_io]: https://www.khronos.org/registry/OpenCL/extensions/intel/cl_intel_media_block_io.txt
[cl_intel_required_subgroup_size]: https://www.khronos.org/registry/OpenCL/extensions/intel/cl_intel_required_subgroup_size.txt
[cl_intel_subgroups]: https://www.khronos.org/registry/OpenCL/extensions/intel/cl_intel_subgroups.txt
