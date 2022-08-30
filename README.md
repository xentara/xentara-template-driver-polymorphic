# Polymorphic I/O Driver Template for Xentara

This repository contains skeleton code for a Xentara driver with the following features:

Connection oriented | Batch processing | Polymorphic I/O Points
:-----------------: | :--------------: | :--------------------:
NO                  | NO               | YES

This driver template requires the Xentara development environment, as well as a Xentara licence. You can get a Xentara
licence in the [Xentara Online Shop](https://www.xentara.io/product/xentara-for-industrial-automation/).

The documentation for Xentara can be found at https://docs.xentara.io/xentara

This driver template uses the Xentara Utility Library, as well as the Xentara Plugin Framework. Docs can be found here:

https://docs.xentara.io/xentara-utils/  
https://docs.xentara.io/xentara-plugin/

## Xentara I/O Component Template

*(See [I/O Components](https://docs.xentara.io/xentara/xentara_io_components.html) in the [Xentara documentation](https://docs.xentara.io/xentara/))*

[src/TemplateIoComponent.hpp](src/TemplateIoComponent.hpp)  
[src/TemplateIoComponent.cpp](src/TemplateIoComponent.cpp)

The I/O component template provides template code for I/O components that can never be disconnected or reconnected to the computer.
Such components are either entirely stateless, or require a single initialization and deinitialization call. This generally includes
three types of components:

- I/O components that are physically located inside the computer (PCIe expansion boards, SPI or I2C components etc.),
- I/O components that are permanently attached to the computer, and cannot be removed or reattached without shutting down, or
- virtual I/O components, that do not represent physical devices at all (simulators, A/I models, computational units etc.).

## Xentara I/O Point Templates

*(See [I/O Points](https://docs.xentara.io/xentara/xentara_io_points.html) in the [Xentara documentation](https://docs.xentara.io/xentara/))*

### Input Template

[src/TemplateInput.hpp](src/TemplateInput.hpp)  
[src/TemplateInput.cpp](src/TemplateInput.cpp)  

The input template provides template code for a read-only I/O point whose value must be read using an individual command for each input.

The template code has the following features:

- The data type of the value is configurable in the [model.json](https://docs.xentara.io/xentara/xentara_model_file.html) file.
- The input publishes a [Xentara task](https://docs.xentara.io/xentara/xentara_element_members.html#xentara_tasks) called *read*,
  which acquires the current value from the I/O component using a read command.
- The input publishes [Xentara events](https://docs.xentara.io/xentara/xentara_element_members.html#xentara_events) to signal changes
  to the value and [quality](https://docs.xentara.io/xentara/xentara_io_points.html#xentara_io_points_quality).

### Output Template

[src/TemplateOutput.hpp](src/TemplateOutput.hpp)  
[src/TemplateOutput.cpp](src/TemplateOutput.cpp)

The output template provides template code for a read/write I/O point whose value must be read and written using individual commands for each output.

The template code has the following features:

- The data type of the value is configurable in the [model.json](https://docs.xentara.io/xentara/xentara_model_file.html) file.
- The input and output values are handled entirely separately. A written output value is not reflected in the input value until
  it has been read back from the I/O component using a read command. This is necessary because the I/O component might reject or
  modify the written value.
- The value of the output is not sent to the I/O component directly when it is written, but placed in a queue to be written at
  a specificly scheduled time using a [Xentara task](https://docs.xentara.io/xentara/xentara_element_members.html#xentara_tasks).
  This allows the user to fine-tune when and how often write commands are sent, and to coordinate reads and writes.
- The output publishes a [Xentara task](https://docs.xentara.io/xentara/xentara_element_members.html#xentara_tasks) called *read*,
  which acquires the current value from the I/O component using a read command.
- The output publishes [Xentara events](https://docs.xentara.io/xentara/xentara_element_members.html#xentara_events) to signal changes
  to the value and [quality](https://docs.xentara.io/xentara/xentara_io_points.html#xentara_io_points_quality).
- The output publishes a [Xentara task](https://docs.xentara.io/xentara/xentara_element_members.html#xentara_tasks) called *write*,
  which checks if an output value is pending, and writes it to the I/O component using a write command, if necessary.
- The output publishes [Xentara events](https://docs.xentara.io/xentara/xentara_element_members.html#xentara_events) to signal if
  a new value was written, or if a write error occurred. 
