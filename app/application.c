// Stripped down firmware to report accelerometer only
#include <application.h>

#define ACCELEROMETER_UPDATE_INTERVAL 100

// Accelerometer instance
bc_lis2dh12_t lis2dh12;

// This function dispatches accelerometer events
void lis2dh12_event_handler(bc_lis2dh12_t *self, bc_lis2dh12_event_t event,
                            void *event_param)
{
    // Update event?
    if (event == BC_LIS2DH12_EVENT_UPDATE)
    {
        bc_lis2dh12_result_g_t result;

        // Successfully read accelerometer vectors?
        if (bc_lis2dh12_get_result_g(self, &result))
        {
            bc_log_info("APP: Acceleration = [%.2f,%.2f,%.2f]",
                        result.x_axis, result.y_axis, result.z_axis);
            bc_radio_pub_acceleration(&result.x_axis, &result.y_axis,
                                      &result.z_axis);
       }
    }
    // Error event?
    else if (event == BC_LIS2DH12_EVENT_ERROR)
    {
        bc_log_error("APP: Accelerometer error");
    }
}

void application_init(void)
{
    // Initialize log
    bc_log_init(BC_LOG_LEVEL_INFO, BC_LOG_TIMESTAMP_ABS);
    bc_log_info("APP: Reset");

    // Initialize accelerometer
    bc_lis2dh12_init(&lis2dh12, BC_I2C_I2C0, 0x19);
    bc_lis2dh12_set_event_handler(&lis2dh12, lis2dh12_event_handler, NULL);
    bc_lis2dh12_set_update_interval(&lis2dh12, ACCELEROMETER_UPDATE_INTERVAL);

    // Initialize radio
    bc_radio_init(BC_RADIO_MODE_NODE_SLEEPING);

    // Send radio pairing request
    bc_radio_pairing_request("accelerometer", VERSION);
}
