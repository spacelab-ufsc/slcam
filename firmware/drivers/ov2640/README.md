# OV2640

C driver for the OV2640 sensor and the FIFO on ArduCAM Mini modules, based on
[ArduCAM/Arduino](https://github.com/ArduCAM/Arduino), particularly
`ArduCAM/ArduCAM.cpp` and `ArduCAM/ov2640_regs.h`.

## Integration

Before calling `ov2640_init()`, initialize the board's buses and pass an
`ov2640_bus_t` to `ov2640_configure()`. The structure is copied; the `ctx` context
must remain valid. Status callbacks use `int32_t` and return zero on success or
a negative error on failure. `delay_ms` does not return a status.

- `sensor_write`: writes a register address and value over SCCB/I²C to the
  7-bit device address `OV2640_SCCB_ADDRESS` (0x30).
- `sensor_read`: writes the register address, ends the transaction, and reads
  one byte from the same device, as in ArduCAM's SCCB implementation.
- `spi_select`: asserts (`true`) or deasserts (`false`) the ArduChip chip select.
- `spi_transfer`: simultaneously transmits and receives `len` bytes without
  changing CS; discards received data when `rx == NULL`. Use mode 0, MSB first,
  and a clock rate supported by the module.
- `delay_ms`: waits at least the requested interval, including the 100 ms delay
  after resetting the sensor.

For a sensor without an ArduChip, both SPI callbacks may be `NULL`; in that case,
FIFO operations return `OV2640_ERROR_TRANSPORT`. The registers accessed through
`ov2640_read_reg/write_reg` belong to the SCCB sensor; those accessed through
`ov2640_read_controller_reg/write_controller_reg` belong to the SPI ArduChip.

The firmware's current I²C driver (`drivers/i2c/i2c.c`) is still incomplete.
Therefore, the application must provide transport callbacks before using this
driver on hardware. The driver does not hardcode pin assignments or bus ports.
It supports one camera; the application must serialize calls and maintain
exclusive access to the bus throughout each burst read.

## Initialization and image settings

`ov2640_init()` checks SPI communication when available and the sensor ID
(0x2641 or 0x2642), resets the sensor, and configures JPEG at 320×240.
`ov2640_set_format()` repeats the reset and full configuration: JPEG at 320×240
or RGB565 at 320×240. Reapply image settings after changing the format.
`ov2640_set_resolution()` uses the nine JPEG tables from 160×120 to 1600×1200;
the additional resolutions are intended for JPEG mode. The 1280×1024 table was
corrected to configure a height of 1024 instead of 960.

There are five lighting modes, five levels each of saturation, brightness, and
contrast (-2 to +2), and eight effects. Invalid options are rejected before any
write. Lists passed to `ov2640_write_regs(reglist, count)` must end with
`{0xff, 0xff}`. `count` specifies the number of accessible entries, including the
terminator; the function does not access indices beyond this limit. A missing
terminator returns an error, but earlier writes may already have occurred.
The first failure stops processing the list.

## Capture

1. Call `ov2640_flush_fifo()` and `ov2640_start_capture()`.
2. Poll `ov2640_capture_done()` until it returns 1; enforce a timeout in the
   application. Zero means capture is pending, and negative values are errors.
3. Call `ov2640_read_fifo_length()` to obtain the size in bytes.
4. Call `ov2640_set_fifo_burst()`, read chunks with `ov2640_read_fifo()`, and
   finish with `ov2640_end_fifo_burst()` to release CS.

The application controls the total number of bytes read according to the FIFO
size. `ov2640_read_fifo()` returns zero on success; it keeps CS asserted between
chunks and requests its release if a transfer fails. If releasing CS fails,
retry `ov2640_end_fifo_burst()` before performing other operations.

`ov2640_clear_fifo_flag()` is equivalent to clearing the FIFO. The original
misspelled name `ov2640_read_fifo_lentgh()` has been preserved as an alias.
`ov2640_set_mode()` selects MCU2LCD (0); use
`ov2640_set_controller_mode()` to explicitly select MCU2LCD, CAM2LCD, or LCD2MCU
on shields that support these modes.

Operations return zero on success and propagate negative transport errors,
except for the size and capture queries described above. Local error codes are
defined in `ov2640.h`. A configuration failure may leave registers partially
updated; fix the communication issue and reinitialize the driver.

## Validation

Run `make -C tests/ov2640` to build and run the host tests with simulated
transports. They check identification, formats, dimensions, image settings,
argument rejection, error propagation, and the chip select lifecycle.
Electrical and image validation must be performed on the board.
