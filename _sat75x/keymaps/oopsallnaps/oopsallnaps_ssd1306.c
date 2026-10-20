
#if 1

#include "qp_internal.h"
#include "qp_comms.h"
#include "qp_oled_panel.h"
#include "qp_sh1106.h"
#include "qp_sh1106_opcodes.h"
#include "qp_surface.h"
#include "qp_surface_internal.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Driver storage
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct sh1106_device_t {
    oled_panel_painter_device_t oled;

    uint8_t framebuffer[SURFACE_REQUIRED_BUFFER_BYTE_SIZE(128, 64, 1)];
} sh1106_device_t;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Quantum Painter API implementations

// Initialisation

bool qp_sh1106_init(painter_device_t device, painter_rotation_t rotation) {
    sh1106_device_t *driver = (sh1106_device_t *)device;

    // Change the surface geometry based on the panel rotation
    if (rotation == QP_ROTATION_90 || rotation == QP_ROTATION_270) {
        driver->oled.surface.base.panel_width  = driver->oled.base.panel_height;
        driver->oled.surface.base.panel_height = driver->oled.base.panel_width;
    } else {
        driver->oled.surface.base.panel_width  = driver->oled.base.panel_width;
        driver->oled.surface.base.panel_height = driver->oled.base.panel_height;
    }

    // Init the internal surface
    if (!qp_init(&driver->oled.surface.base, QP_ROTATION_0)) {
        qp_dprintf("Failed to init internal surface in qp_sh1106_init\n");
        return false;
    }

    // clang-format off
    uint8_t sh1106_init_sequence[] = {
        // Command,                 Delay,  N, Data[N]
        SH1106_SET_MUX_RATIO,           0,  1, 0x3F,
        SH1106_DISPLAY_OFFSET,          0,  1, 0x00,
        SH1106_DISPLAY_START_LINE,      0,  0,
        SH1106_SET_SEGMENT_REMAP_INV,   0,  0,
        SH1106_COM_SCAN_DIR_DEC,        0,  0,
        SH1106_COM_PADS_HW_CFG,         0,  1, 0x12,
        SH1106_SET_CONTRAST,            0,  1, 0x7F,
        SH1106_ALL_ON_RESUME,           0,  0,
        SH1106_NON_INVERTING_DISPLAY,   0,  0,
        SH1106_SET_OSC_DIVFREQ,         0,  1, 0x80,
        SH1106_SET_CHARGE_PUMP,         0,  1, 0x14,
        SH1106_DISPLAY_ON,              0,  0,
    };
    // clang-format on

    // If the display height is anything other than the default 64 pixels, change SH1106_SET_MUX_RATIO data byte to the correct value
    if (driver->oled.base.panel_height != 64) {
        sh1106_init_sequence[3] = driver->oled.base.panel_height - 1;
    }

    // For 128x32 or 96x16 displays, change SH1106_COM_PADS_HW_CFG data byte from alternative (0x12) to sequential (0x02) configuration
    if (driver->oled.base.panel_height <= 32) {
        sh1106_init_sequence[20] = 0x02;
    }

    return qp_comms_bulk_command_sequence(device, sh1106_init_sequence, sizeof(sh1106_init_sequence));
}

#endif
