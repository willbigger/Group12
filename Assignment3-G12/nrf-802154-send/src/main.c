#include <stdio.h>
#include <zephyr/init.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>
#include <inttypes.h>

#include <zephyr/logging/log.h>
	LOG_MODULE_REGISTER(send);

#include "nrf_802154.h"
#include <inttypes.h>

#define PSDU_MAX_SIZE (127u)
#define FCS_LENGTH (2u) /* Length of the Frame Control Sequence */
#define CHECK_PERIOD (500)

#define SW0_NODE	DT_ALIAS(sw0)

static const struct gpio_dt_spec button1 = GPIO_DT_SPEC_GET_OR(SW0_NODE, gpios, {0});

static struct gpio_callback button1_cb_data;

uint8_t pkt[PSDU_MAX_SIZE];

const nrf_802154_transmit_metadata_t metadata = {
	.frame_props = NRF_802154_TRANSMITTED_FRAME_PROPS_DEFAULT_INIT,
	.cca = true
};

void init_button(const struct gpio_dt_spec* button,
                 struct gpio_callback* button_cb_data,
				 gpio_callback_handler_t cb) {
	int ret;

	if (!device_is_ready(button->port)) {
		LOG_DBG("Error: button device %s is not ready\n",
		       button->port->name);
		return;
	}

	ret = gpio_pin_configure_dt(button, GPIO_INPUT);
	if (ret != 0) {
		LOG_DBG("Error %d: failed to configure %s pin %d\n",
		       ret, button->port->name, button->pin);
		return;
	}

	ret = gpio_pin_interrupt_configure_dt(button, GPIO_INT_EDGE_BOTH);
	if (ret != 0) {
		LOG_DBG("Error %d: failed to configure interrupt on %s pin %d\n",
			ret, button->port->name, button->pin);
		return;
	}

	gpio_init_callback(button_cb_data, cb, BIT(button->pin));
	gpio_add_callback(button->port, button_cb_data);
	//LOG_DBG("Set up button at %s pin %d\n", button->port->name, button->pin);
}

static int rf_setup(const struct device *dev)
{
	LOG_INF("RF setup started");
	ARG_UNUSED(dev);

	/* nrf radio driver initialization */
	nrf_802154_init();

	return 0;
}

// callback fn when tx starts
void nrf_802154_tx_started(const uint8_t *p_frame) {
	LOG_INF("tx started");
}

// callback fn when tx fails
void nrf_802154_transmit_failed(uint8_t *frame, nrf_802154_tx_error_t error,
	const nrf_802154_transmit_done_metadata_t *p_metadata)
{
	LOG_INF("tx failed error %u!", error);
	//LOG_DBG("Retransmitting.\n");
	if(!nrf_802154_transmit_raw(pkt, &metadata)) {
		LOG_ERR("driver could not schedule the transmission procedure.");
	}
}

// callback fn for successful tx
void nrf_802154_transmitted_raw(uint8_t *p_frame, 
	const nrf_802154_transmit_done_metadata_t *p_metadata) {
	LOG_INF("frame was transmitted!");
}


void button1_pressed(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
	int val = gpio_pin_get_dt(&button1);
	if (val == 1) {
		LOG_DBG("Button1 pressed.\n");
		if(!nrf_802154_transmit_raw(pkt, &metadata)) {
			LOG_ERR("driver could not schedule the transmission procedure.");
		}
	}
}

int main(int argc, char **argv) {
	nrf_802154_channel_set(20u);
	LOG_DBG("channel: %u", nrf_802154_channel_get());

	// set the pan_id (2 bytes, little-endian)
	uint8_t src_pan_id[] = {0x10, 0x01}; 
	nrf_802154_pan_id_set(src_pan_id);

	// set the extended address (8 bytes, little-endian)
	uint8_t src_extended_addr[] = {0x55, 0xae, 0xca, 0xc4, 0x35, 0x33, 0xfe, 0xf3};
	nrf_802154_extended_address_set(src_extended_addr);

	uint8_t dst_short_addr[] = {0xab, 0xcd};

	pkt[0] = 20u + FCS_LENGTH; /* Length for nrf_transmit (length of pkt + FCS) */
	pkt[1] = 0x21; /* Frame Control Field */
	pkt[2] = 0xc8; /* Frame Control Field */
	pkt[3] = 0x00; /* Sequence number */
	pkt[4] = 0x10; /* Destination PAN ID 0xffff */
	pkt[5] = 0x01; /* Destination PAN ID */
	memcpy(&pkt[6], dst_short_addr, 2); /* Destination extended address */
	memcpy(&pkt[8], src_pan_id, 2); /* Source PAN ID */
	memcpy(&pkt[10], src_extended_addr, 8);/* Source extended address */ 
	pkt[18] = 0x47; /* Payload is "G12"*/
	pkt[19] = 0x31; /* */
	pkt[20] = 0x32; /* */
	
	init_button(&button1, &button1_cb_data, button1_pressed);
	
	return 0;
}

SYS_INIT(rf_setup, POST_KERNEL, CONFIG_PTT_RF_INIT_PRIORITY);
