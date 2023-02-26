/*
 * Copyright (c) 2015-2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/types.h>
#include <stddef.h>
#include <sys/printk.h>
#include <sys/util.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <stdio.h>

static void scan_cb(const bt_addr_le_t *addr, int8_t rssi, uint8_t adv_type,
		    struct net_buf_simple *buf)
{
	char src_addr[18];

	// Convert address to typical MAC address format.
	bt_addr_le_to_str(addr, src_addr, 18);

	// if ((rssi > -30) || (strcmp(src_addr, target) == 0)){
	// 	printk("[BLE ADV] src: %s (rssi: %i)\n", src_addr, rssi);
	// }
	// char target[] = "C4:8C:60:56:F8:2A";
	// if (strcmp(src_addr, target) == 0){
	// 	printk("[BLE ADV] src: %s (rssi: %i)\n", src_addr, rssi);
	// }
	//printk("[BLE ADV] src: %s (rssi: %i) type: %d\t", src_addr, rssi, adv_type);
	//printk("Size of buffer: %i\t", buf->len);
	int indexes_left = 0;
	int pos_index = 0;
	int num_elem = 0;
	for (int i=0; i < buf->len; i++){
		if (i == 0 || indexes_left == 0){
			indexes_left = buf->data[i];
			pos_index = 0;
			num_elem++;
		} else {
			if (pos_index == 1){
				printk("%x ", buf->data[i]);
			}
			indexes_left--;
			pos_index++;
		}
	}
	printk("%d\n", num_elem);
}

void main(void)
{
	struct bt_le_scan_param scan_param = {
		.type       = BT_HCI_LE_SCAN_ACTIVE,
		.options    = BT_LE_SCAN_OPT_NONE,
		.interval   = 0x0010,
		.window     = 0x0010,
	};
	int err;

	printk("Starting Scanner\n");

	// Initialize the Bluetooth Subsystem
	err = bt_enable(NULL);
	if (err) {
		printk("Bluetooth init failed (err %d)\n", err);
		return;
	}

	printk("Bluetooth initialized\n");

	err = bt_le_scan_start(&scan_param, scan_cb);
	if (err) {
		printk("Starting scanning failed (err %d)\n", err);
		return;
	}
}