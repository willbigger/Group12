/*
 * Copyright (c) 2021 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */
#include <stdio.h>
#include <zephyr/init.h>
#include <zephyr/kernel.h>
#include <inttypes.h>

#include <zephyr/logging/log.h>
	LOG_MODULE_REGISTER(recv);

#include "nrf_802154.h"

#define PSDU_MAX_SIZE (127u)

//in milliseconds
#define CHECK_PERIOD (500)
#define LISTEN_PERIOD (5)

void five_s_timer_handler(struct k_timer *timer);

K_TIMER_DEFINE(five_s_timer, five_s_timer_handler, NULL);

void five_s_timer_handler(struct k_timer *timer)
{
    LOG_INF("15 seconds");
	nrf_802154_stat_totals_t stats;
	nrf_802154_stat_totals_get(&stats);

	LOG_INF("[Receiver Stats] Listen: %llu us", stats.total_listening_time);
	LOG_INF("[Receiver Stats] Receive: %llu us", stats.total_receive_time);
	LOG_INF("[Receiver Stats] Transmit: %llu us", stats.total_transmit_time);
}

static void pkt_hexdump(uint8_t *pkt, uint8_t length) {
  int i;
  printk("Packet: ");
	for (i=0; i<length; i++) {
		printk("%02x ", *pkt);
		pkt++;
	}
	printk("\n");
}

static int rf_setup(const struct device *dev)
{
	LOG_INF("RF setup started");
	ARG_UNUSED(dev);

	/* nrf radio driver initialization */
	nrf_802154_init();
	return 0;
}

void nrf_802154_received_raw(uint8_t *data, int8_t power, uint8_t lqi) {
	LOG_INF("Received Raw");
	pkt_hexdump(data+1, *data - 2U); /* print packet from byte [1, len-2] */
	nrf_802154_buffer_free_raw(data);
	if(nrf_802154_sleep()) {
		LOG_DBG("radio sleeping");
	} else {
		LOG_ERR("driver could not enter the receive state");
	}
}

// void nrf_802154_receive_failed(nrf_802154_rx_error_t error, uint32_t id) {
// 	LOG_INF("rx failed error %u!", error);
// }

int main(void) {
	nrf_802154_channel_set(20u);
	nrf_802154_auto_ack_set(true);
	LOG_DBG("channel: %u", nrf_802154_channel_get());
	
	// set the pan_id (2 bytes, little-endian)
	uint8_t pan_id[] = {0x10, 0x01};
	nrf_802154_pan_id_set(pan_id);

	// set the extended address (8 bytes, little-endian)
	uint8_t short_addr[] = {0xab, 0xcd};
	nrf_802154_short_address_set(short_addr);

	// Start timer
	k_timer_start(&five_s_timer, K_SECONDS(15), K_SECONDS(15));

	while(1){
		//LOG_DBG(timer);
		if(nrf_802154_receive()) {
			LOG_DBG("radio entered rx state");
		} else {
			LOG_ERR("driver could not enter the receive state");
		}
		k_sleep(K_MSEC(LISTEN_PERIOD)); 
		if(nrf_802154_sleep()) {
			LOG_DBG("radio sleeping");
		} else {
			LOG_ERR("driver could not enter the receive state");
		}
		k_sleep(K_MSEC(CHECK_PERIOD)); 
	}

	// if(nrf_802154_receive()) {
	// 	LOG_DBG("radio entered rx state");
	// } else {
	// 	LOG_ERR("driver could not enter the receive state");
	// }

	return 0;
}

SYS_INIT(rf_setup, POST_KERNEL, CONFIG_PTT_RF_INIT_PRIORITY);