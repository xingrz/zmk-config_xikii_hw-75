/*
 * Copyright (c) 2022-2023 XiNGRZ
 * SPDX-License-Identifier: MIT
 */

#include "handler.h"
#include "usb_comm.pb.h"

#include <pb_encode.h>

#ifdef VER_ZEPHYR
static const char *zephyr_version = VER_ZEPHYR;
#else
static const char *zephyr_version = "unknown";
#endif

#ifdef VER_ZMK
static const char *zmk_version = VER_ZMK;
#else
static const char *zmk_version = "unknown";
#endif

#ifdef VER_APP
static const char *app_version = VER_APP;
#else
static const char *app_version = "unknown";
#endif

static bool write_string(pb_ostream_t *stream, const pb_field_t *field, void *const *arg)
{
	char *str = *arg;
	if (!pb_encode_tag_for_field(stream, field)) {
		return false;
	}
	return pb_encode_string(stream, (uint8_t *)str, strlen(str));
}

bool handle_version(const usb_comm_MessageH2D *h2d, usb_comm_MessageD2H *d2h, const void *bytes,
		    uint32_t bytes_len)
{
	usb_comm_Version *res = &d2h->payload.version;
	res->zephyr_version.funcs.encode = write_string;
	res->zephyr_version.arg = (void *)zephyr_version;
	res->zmk_version.funcs.encode = write_string;
	res->zmk_version.arg = (void *)zmk_version;
	res->app_version.funcs.encode = write_string;
	res->app_version.arg = (void *)app_version;

	res->has_features = true;
	res->features.has_rgb = res->features.rgb = true;
	res->features.has_eink = res->features.eink = true;
	res->features.has_knob = res->features.knob = true;

	return true;
}
