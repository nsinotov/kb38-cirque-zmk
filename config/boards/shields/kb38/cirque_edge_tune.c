/*
 * Cirque Pinnacle edge sensitivity tuning
 *
 * Lowers WideZMin thresholds so the outer ring of a circular trackpad
 * registers touches reliably. Without this, the ASIC's default thresholds
 * are too high for the weaker capacitive signal at the sensor edges,
 * causing jittery/slow cursor movement in the outer ~1/3 of the pad.
 *
 * Values from Cirque application note AN-000032 / reference code for
 * curved-overlay circular trackpads. Lower = more sensitive at edges.
 */

#include <zephyr/device.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/init.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(cirque_edge_tune, LOG_LEVEL_INF);

#define PINNACLE_I2C_ADDR      0x2A

#define ERA_VALUE              0x1B
#define ERA_ADDR_HI            0x1C
#define ERA_ADDR_LO            0x1D
#define ERA_CTRL               0x1E
#define ERA_CTRL_WRITE         0x02

#define CAL_CONFIG1            0x07
#define CAL_CONFIG1_CALIBRATE  BIT(5)
#define STATUS1                0x02
#define STATUS1_CC             BIT(3)

#define X_AXIS_WIDE_Z_MIN     0x0149
#define Y_AXIS_WIDE_Z_MIN     0x0168

#define WIDE_Z_MIN_X           2
#define WIDE_Z_MIN_Y           1

static int pinnacle_era_write(const struct device *i2c, uint16_t addr, uint8_t value) {
    int ret;

    ret = i2c_reg_write_byte(i2c, PINNACLE_I2C_ADDR, ERA_VALUE, value);
    if (ret) return ret;

    ret = i2c_reg_write_byte(i2c, PINNACLE_I2C_ADDR, ERA_ADDR_HI, (addr >> 8) & 0xFF);
    if (ret) return ret;

    ret = i2c_reg_write_byte(i2c, PINNACLE_I2C_ADDR, ERA_ADDR_LO, addr & 0xFF);
    if (ret) return ret;

    ret = i2c_reg_write_byte(i2c, PINNACLE_I2C_ADDR, ERA_CTRL, ERA_CTRL_WRITE);
    if (ret) return ret;

    uint8_t ctrl;
    do {
        k_usleep(50);
        ret = i2c_reg_read_byte(i2c, PINNACLE_I2C_ADDR, ERA_CTRL, &ctrl);
        if (ret) return ret;
    } while (ctrl != 0x00);

    return 0;
}

static int pinnacle_force_recalibrate(const struct device *i2c) {
    uint8_t cal;
    int ret;

    ret = i2c_reg_read_byte(i2c, PINNACLE_I2C_ADDR, CAL_CONFIG1, &cal);
    if (ret) return ret;

    cal |= CAL_CONFIG1_CALIBRATE;
    ret = i2c_reg_write_byte(i2c, PINNACLE_I2C_ADDR, CAL_CONFIG1, cal);
    if (ret) return ret;

    k_msleep(100);

    uint8_t status;
    ret = i2c_reg_read_byte(i2c, PINNACLE_I2C_ADDR, STATUS1, &status);
    if (ret) return ret;

    status &= ~STATUS1_CC;
    return i2c_reg_write_byte(i2c, PINNACLE_I2C_ADDR, STATUS1, status);
}

static int cirque_edge_tune_init(void) {
    const struct device *i2c = DEVICE_DT_GET(DT_NODELABEL(i2c1));

    if (!device_is_ready(i2c)) {
        LOG_ERR("I2C device not ready");
        return -ENODEV;
    }

    int ret;

    ret = pinnacle_era_write(i2c, X_AXIS_WIDE_Z_MIN, WIDE_Z_MIN_X);
    if (ret) {
        LOG_ERR("Failed to set X WideZMin: %d", ret);
        return ret;
    }

    ret = pinnacle_era_write(i2c, Y_AXIS_WIDE_Z_MIN, WIDE_Z_MIN_Y);
    if (ret) {
        LOG_ERR("Failed to set Y WideZMin: %d", ret);
        return ret;
    }

    ret = pinnacle_force_recalibrate(i2c);
    if (ret) {
        LOG_ERR("Failed to recalibrate: %d", ret);
        return ret;
    }

    LOG_INF("Edge sensitivity tuned (X=%d, Y=%d)", WIDE_Z_MIN_X, WIDE_Z_MIN_Y);
    return 0;
}

SYS_INIT(cirque_edge_tune_init, APPLICATION, 99);
