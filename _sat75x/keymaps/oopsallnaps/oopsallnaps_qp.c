#include "oopsallnaps_core.h"

#include <qp.h>
#include <printf.h>
#include <stdint.h>

#include <qp_comms.h>
#include "qp_draw.h"
#include "qp_sh1106_opcodes.h"

/**
 * Set the display's contrast with a value from 0-255.
 */
bool set_display_contrast(painter_device_t display, uint8_t val) {
    if (!display) {
        return false;
    }
    
    const uint8_t sequence[] = {
        // Command,      Delay,  N, Data[N]
        SH1106_SET_CONTRAST, 0,  1, val 
    };

    return qp_comms_bulk_command_sequence(display, sequence, sizeof(sequence));
}

bool batch_setpixel(painter_device_t device, const vec2 *sequence, size_t sequence_len, uint8_t offset_x, uint8_t offset_y) {
    painter_driver_t *driver = (painter_driver_t *)device;
    if (!driver || !driver->validate_ok) {
        qp_dprintf("qp_setpixel: fail (validation_ok == false)\n");
        return false;
    }

    if (!qp_comms_start(device)) {
        qp_dprintf("Failed to start comms in qp_setpixel\n");
        return false;
    }

    qp_internal_fill_pixdata(device, 1, 255, 255, 255);
    bool ret = true;
    for (int i = 0; i < sequence_len; i++) {
        bool ok = qp_internal_setpixel_impl(device, sequence[i].x + offset_x, sequence[i].y + offset_y);
        if (!ok) {
            ret = false;
            break;
        }
    }
    qp_comms_stop(device);
    qp_dprintf("qp_setpixel: %s\n", ret ? "ok" : "fail");
    return ret;
}