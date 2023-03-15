#include <zephyr/types.h>
#include <stddef.h>
#include <errno.h>
#include <sys/printk.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/conn.h>
#include <bluetooth/uuid.h>
#include <bluetooth/gatt.h>
#include <sys/byteorder.h>
#include <zephyr.h>
#include <device.h>
#include <devicetree.h>
#include <drivers/gpio.h>


#define LED0_NODE DT_ALIAS(led0)
#define LED1_NODE DT_ALIAS(led1)
#define LED2_NODE DT_ALIAS(led2)
#define LED3_NODE DT_ALIAS(led3)

#if DT_NODE_HAS_STATUS(LED0_NODE, okay)
#define LED0	DT_GPIO_LABEL(LED0_NODE, gpios)
#define PIN0	DT_GPIO_PIN(LED0_NODE, gpios)
#define FLAGS0	DT_GPIO_FLAGS(LED0_NODE, gpios)
#else
/* A build error here means your board isn't set up to blink an LED. */
#error "Unsupported board: led0 devicetree alias is not defined"
#define LED0	""
#define PIN0	0
#define FLAGS0	0
#endif

#if DT_NODE_HAS_STATUS(LED1_NODE, okay)
#define LED1	DT_GPIO_LABEL(LED1_NODE, gpios)
#define PIN1	DT_GPIO_PIN(LED1_NODE, gpios)
#define FLAGS1	DT_GPIO_FLAGS(LED1_NODE, gpios)
#else
/* A build error here means your board isn't set up to blink an LED. */
#error "Unsupported board: led0 devicetree alias is not defined"
#define LED1	""
#define PIN1	0
#define FLAGS1	0
#endif

#if DT_NODE_HAS_STATUS(LED2_NODE, okay)
#define LED2	DT_GPIO_LABEL(LED2_NODE, gpios)
#define PIN2	DT_GPIO_PIN(LED2_NODE, gpios)
#define FLAGS2	DT_GPIO_FLAGS(LED2_NODE, gpios)
#else
/* A build error here means your board isn't set up to blink an LED. */
#error "Unsupported board: led0 devicetree alias is not defined"
#define LED2	""
#define PIN2	0
#define FLAGS2	0
#endif

#if DT_NODE_HAS_STATUS(LED3_NODE, okay)
#define LED3	DT_GPIO_LABEL(LED3_NODE, gpios)
#define PIN3	DT_GPIO_PIN(LED3_NODE, gpios)
#define FLAGS3	DT_GPIO_FLAGS(LED3_NODE, gpios)
#else
/* A build error here means your board isn't set up to blink an LED. */
#error "Unsupported board: led0 devicetree alias is not defined"
#define LED3	""
#define PIN3	0
#define FLAGS3	0
#endif

//central
#define LAB2_SERVICE_UUID BT_UUID_128_ENCODE(0xBDFC9792, 0x8234, 0x405E, 0xAE02, 0x35EF3274B299)
#define LAB2_SERVICE_CHARACTERISTIC_UUID1 0x0001
#define LAB2_SERVICE_CHARACTERISTIC_UUID2 0x0002
#define LAB2_SERVICE_CHARACTERISTIC_UUID3 0x0003
#define LAB2_SERVICE_CHARACTERISTIC_UUID4 0x0004

static void start_scan(void);

static struct bt_conn *default_conn;

static struct bt_uuid* search_service_uuid = BT_UUID_DECLARE_128(LAB2_SERVICE_UUID);
static struct bt_uuid* search_characteristic_uuid_1 = BT_UUID_DECLARE_16(LAB2_SERVICE_CHARACTERISTIC_UUID1);
static struct bt_uuid* search_characteristic_uuid_2 = BT_UUID_DECLARE_16(LAB2_SERVICE_CHARACTERISTIC_UUID2);
static struct bt_uuid* search_characteristic_uuid_3 = BT_UUID_DECLARE_16(LAB2_SERVICE_CHARACTERISTIC_UUID3);
static struct bt_uuid* search_characteristic_uuid_4 = BT_UUID_DECLARE_16(LAB2_SERVICE_CHARACTERISTIC_UUID4);
static struct bt_gatt_discover_params discover_params;
static struct bt_gatt_notify_params notify_params1;
static struct bt_gatt_notify_params notify_params2;
static struct bt_gatt_notify_params notify_params3;
static struct bt_gatt_notify_params notify_params4;

// Callback after receiving a notification.
static uint8_t notif_func_1(struct bt_conn *conn, uint8_t err,
			       struct bt_gatt_read_params *params,
			       const void *data, uint16_t length)
{
	const struct device *dev;
	bool led_is_on = true;
	int ret;
	printk("Enering notify function 1");
	if (err) {printk("notify failed (err %d)\n", err);}
	dev = device_get_binding(LED0);
	if (dev == NULL) {return;}
	ret = gpio_pin_configure(dev, PIN0, GPIO_OUTPUT_ACTIVE | FLAGS0);
	if (ret < 0) {return;}
	gpio_pin_set(dev, PIN0, (int)led_is_on);
	return BT_GATT_ITER_STOP;
}

static uint8_t notif_func_2(struct bt_conn *conn, uint8_t err,
			       struct bt_gatt_read_params *params,
			       const void *data, uint16_t length)
{
	const struct device *dev;
	bool led_is_on = true;
	int ret;
	printk("Enering notify function 2");
	if (err) {printk("notify failed (err %d)\n", err);}
	dev = device_get_binding(LED1);
	if (dev == NULL) {return;}
	ret = gpio_pin_configure(dev, PIN1, GPIO_OUTPUT_ACTIVE | FLAGS1);
	if (ret < 0) {return;}
	gpio_pin_set(dev, PIN1, (int)led_is_on);
	return BT_GATT_ITER_STOP;
}

static uint8_t notif_func_3(struct bt_conn *conn, uint8_t err,
			       struct bt_gatt_read_params *params,
			       const void *data, uint16_t length)
{
	const struct device *dev;
	bool led_is_on = true;
	int ret;
	printk("Enering notify function 3");
	if (err) {printk("notify failed (err %d)\n", err);}
	dev = device_get_binding(LED2);
	if (dev == NULL) {return;}
	ret = gpio_pin_configure(dev, PIN2, GPIO_OUTPUT_ACTIVE | FLAGS2);
	if (ret < 0) {return;}
	gpio_pin_set(dev, PIN2, (int)led_is_on);
	return BT_GATT_ITER_STOP;
}

static uint8_t notif_func_4(struct bt_conn *conn, uint8_t err,
			       struct bt_gatt_read_params *params,
			       const void *data, uint16_t length)
{
	const struct device *dev;
	bool led_is_on = true;
	int ret;
	printk("Enering notify function 4");
	if (err) {printk("notify failed (err %d)\n", err);}
	dev = device_get_binding(LED3);
	if (dev == NULL) {return;}
	ret = gpio_pin_configure(dev, PIN3, GPIO_OUTPUT_ACTIVE | FLAGS3);
	if (ret < 0) {return;}
	gpio_pin_set(dev, PIN3, (int)led_is_on);
	return BT_GATT_ITER_STOP;
}

static uint8_t discover_func(struct bt_conn *conn,
			     const struct bt_gatt_attr *attr,
			     struct bt_gatt_discover_params *params)
{
	int err;

	if (!attr) {
		printk("Discover complete\n");
		(void)memset(params, 0, sizeof(*params));
		return BT_GATT_ITER_STOP;
	}

	// printk("[ATTRIBUTE] handle %u\n", attr->handle);

	if (bt_uuid_cmp(discover_params.uuid, BT_UUID_DECLARE_128(LAB2_SERVICE_UUID)) == 0) {
		// printk("Found service\n");
		discover_params.uuid = search_characteristic_uuid_1;
		discover_params.start_handle = attr->handle + 1;
		discover_params.type = BT_GATT_DISCOVER_CHARACTERISTIC;
		err = bt_gatt_discover(conn, &discover_params);
		if (err) {
			printk("Discover failed (err %d)\n", err);
		}
	}
	else if ((bt_uuid_cmp(discover_params.uuid, BT_UUID_DECLARE_16(LAB2_SERVICE_CHARACTERISTIC_UUID1)) == 0)) {
		// printk("Found characteristic\n");
		printk("Subscription 1\n");
		notify_params1.func = notif_func_1;
		notify_params1.attr = attr;
		//err = bt_gatt_read(conn, &read_params);
		err = bt_gatt_notify_cb(conn, &notify_params1);
		discover_params.uuid++;
		if (err) {
			printk("Read failed (err %d)\n", err);
		}
	} else if ((bt_uuid_cmp(discover_params.uuid, BT_UUID_DECLARE_16(LAB2_SERVICE_CHARACTERISTIC_UUID2)) == 0)){
		// printk("Found characteristic\n");
		printk("Subscription 2\n");
		notify_params2.func = notif_func_2;
		notify_params2.attr = attr;
		//err = bt_gatt_read(conn, &read_params);
		err = bt_gatt_notify_cb(conn, &notify_params2);
		discover_params.uuid++;
		if (err) {
			printk("Read failed (err %d)\n", err);
		}
	} else if ((bt_uuid_cmp(discover_params.uuid, BT_UUID_DECLARE_16(LAB2_SERVICE_CHARACTERISTIC_UUID3)) == 0)){
		// printk("Found characteristic\n");
		printk("Subscription 3\n");
		notify_params3.func = notif_func_3;
		notify_params3.attr = attr;
		//err = bt_gatt_read(conn, &read_params);
		err = bt_gatt_notify_cb(conn, &notify_params3);
		discover_params.uuid++;
		if (err) {
			printk("Read failed (err %d)\n", err);
		}
	} else if ((bt_uuid_cmp(discover_params.uuid, BT_UUID_DECLARE_16(LAB2_SERVICE_CHARACTERISTIC_UUID4)) == 0)){
		// printk("Found characteristic\n");
		printk("Subscription 4\n");
		notify_params4.func = notif_func_4;
		notify_params4.attr = attr;
		//err = bt_gatt_read(conn, &read_params);
		err = bt_gatt_notify_cb(conn, &notify_params4);
		if (err) {
			printk("Read failed (err %d)\n", err);
		}
	}

	return BT_GATT_ITER_CONTINUE;
}

static void connected(struct bt_conn *conn, uint8_t conn_err)
{
	char addr[BT_ADDR_LE_STR_LEN];
	int err;

	bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

	if (conn_err) {
		printk("Failed to connect to %s (%u)\n", addr, conn_err);

		bt_conn_unref(default_conn);
		default_conn = NULL;

		start_scan();
		return;
	}

	printk("Connected: %s\n", addr);

	if (conn == default_conn) {
		discover_params.uuid = search_service_uuid;
		discover_params.func = discover_func;
		discover_params.start_handle = BT_ATT_FIRST_ATTTRIBUTE_HANDLE;
		discover_params.end_handle = BT_ATT_LAST_ATTTRIBUTE_HANDLE;
		discover_params.type = BT_GATT_DISCOVER_PRIMARY;

		err = bt_gatt_discover(default_conn, &discover_params);
		if (err) {
			printk("Discover failed(err %d)\n", err);
			return;
		}
	}
}

static void disconnected(struct bt_conn *conn, uint8_t reason)
{
	char addr[BT_ADDR_LE_STR_LEN];

	bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

	printk("Disconnected: %s (reason 0x%02x)\n", addr, reason);

	if (default_conn != conn) {
		return;
	}

	bt_conn_unref(default_conn);
	default_conn = NULL;

	start_scan();
}

BT_CONN_CB_DEFINE(conn_callbacks) = {
	.connected = connected,
	.disconnected = disconnected,
};

// Called for each advertising data element in the advertising data.
static bool ad_found(struct bt_data *data, void *user_data)
{
	bt_addr_le_t *addr = user_data;

	printk("[AD]: %u data_len %u\n", data->type, data->data_len);

	switch (data->type) {
	case BT_DATA_UUID128_ALL:
		if (data->data_len != 16) {
			printk("AD malformed\n");
			return true;
		}

		struct bt_le_conn_param *param;
		struct bt_uuid uuid;
		int err;

		bt_uuid_create(&uuid, data->data, 16);
		if (bt_uuid_cmp(&uuid, BT_UUID_DECLARE_128(LAB2_SERVICE_UUID)) == 0) {
			printk("Found matching advertisement\n");

			err = bt_le_scan_stop();
			if (err) {
				printk("Stop LE scan failed (err %d)\n", err);
				return false;
			}

			param = BT_LE_CONN_PARAM_DEFAULT;
			err = bt_conn_le_create(addr, BT_CONN_LE_CREATE_CONN, param, &default_conn);
			if (err) {
				printk("Create conn failed (err %d)\n", err);
				start_scan();
			}
		}

		return false;
		
	}

	return true;
}

static void device_found(const bt_addr_le_t *addr, int8_t rssi, uint8_t type,
			 struct net_buf_simple *ad)
{
	char dev[BT_ADDR_LE_STR_LEN];

	bt_addr_le_to_str(addr, dev, sizeof(dev));
	printk("[DEVICE]: %s, AD evt type %u, AD data len %u, RSSI %i\n",
	       dev, type, ad->len, rssi);

	// We're only interested in connectable devices.
	if (type == BT_GAP_ADV_TYPE_ADV_IND ||
	    type == BT_GAP_ADV_TYPE_ADV_DIRECT_IND) {
		// Helper function to parse the advertising data (AD) elements
		// from the advertisement. This will call `ad_found()` for
		// each element.
		bt_data_parse(ad, ad_found, (void*) addr);
	}
}

static void start_scan(void)
{
	int err;

	struct bt_le_scan_param scan_param = {
		.type       = BT_LE_SCAN_TYPE_PASSIVE,
		.options    = BT_LE_SCAN_OPT_NONE,
		.interval   = BT_GAP_SCAN_FAST_INTERVAL,
		.window     = BT_GAP_SCAN_FAST_WINDOW,
	};

	err = bt_le_scan_start(&scan_param, device_found);
	if (err) {
		printk("Scanning failed to start (err %d)\n", err);
		return;
	}

	printk("Scanning successfully started\n");
}

static void bt_ready(int err)
{
	if (err) {
		printk("Bluetooth init failed (err %d)\n", err);
		return;
	}

	printk("Bluetooth initialized\n");

	start_scan();
}

void main(void)
{
	int err;

	err = bt_enable(bt_ready);

	if (err) {
		printk("Bluetooth init failed (err %d)\n", err);
		return;
	}
}
