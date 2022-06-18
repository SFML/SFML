/*
 * Copyright (c) 2017 Rob Clark <rclark@redhat.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sub license,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial portions
 * of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#include <errno.h>
#include <inttypes.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <drm-common.h>
#include <gbm.h>

#define WEAK __attribute__((weak))

WEAK uint64_t gbm_bo_get_modifier(struct gbm_bo *bo);
WEAK int gbm_bo_get_plane_count(struct gbm_bo *bo);
WEAK uint32_t gbm_bo_get_stride_for_plane(struct gbm_bo *bo, int plane);
WEAK uint32_t gbm_bo_get_offset(struct gbm_bo *bo, int plane);

static void drm_fb_destroy_callback(struct gbm_bo *bo, void *data)
{
	int drm_fd = gbm_device_get_fd(gbm_bo_get_device(bo));
	struct drm_fb *fb = data;

	if (fb->fb_id)
		drmModeRmFB(drm_fd, fb->fb_id);

	free(fb);
}

struct drm_fb * drm_fb_get_from_bo(struct gbm_bo *bo)
{
	int drm_fd = gbm_device_get_fd(gbm_bo_get_device(bo));
	struct drm_fb *fb = gbm_bo_get_user_data(bo);
	uint32_t width, height, format,
		 strides[4] = {0}, handles[4] = {0},
		 offsets[4] = {0}, flags = 0;
	int ret = -1;

	if (fb)
		return fb;

	fb = calloc(1, sizeof *fb);
	fb->bo = bo;

	width = gbm_bo_get_width(bo);
	height = gbm_bo_get_height(bo);
	format = gbm_bo_get_format(bo);

	if (gbm_bo_get_modifier && gbm_bo_get_plane_count &&
	    gbm_bo_get_stride_for_plane && gbm_bo_get_offset)
	{
		uint64_t modifiers[4] = {0};
		modifiers[0] = gbm_bo_get_modifier(bo);
		const int num_planes = gbm_bo_get_plane_count(bo);
		for (int i = 0; i < num_planes; i++)
		{
			strides[i] = gbm_bo_get_stride_for_plane(bo, i);
			handles[i] = gbm_bo_get_handle(bo).u32;
			offsets[i] = gbm_bo_get_offset(bo, i);
			modifiers[i] = modifiers[0];
		}

		if (modifiers[0])
		{
			flags = DRM_MODE_FB_MODIFIERS;
			// printf("Using modifier %" PRIx64 "\n", modifiers[0]);
		}

		ret = drmModeAddFB2WithModifiers(drm_fd, width, height,
				format, handles, strides, offsets,
				modifiers, &fb->fb_id, flags);
	}

	if (ret)
	{
		// if (flags)
			// fprintf(stderr, "Modifiers failed!\n");

		memcpy(handles, (uint32_t [4]){gbm_bo_get_handle(bo).u32,0,0,0}, 16);
		memcpy(strides, (uint32_t [4]){gbm_bo_get_stride(bo),0,0,0}, 16);
		memset(offsets, 0, 16);
		ret = drmModeAddFB2(drm_fd, width, height, format,
				handles, strides, offsets, &fb->fb_id, 0);
	}

	if (ret)
	{
		printf("failed to create fb: %s\n", strerror(errno));
		free(fb);
		return NULL;
	}

	gbm_bo_set_user_data(bo, fb, drm_fb_destroy_callback);

	return fb;
}

static uint32_t find_crtc_for_encoder(const drmModeRes *resources,
		const drmModeEncoder *encoder)
{

	for (int i = 0; i < resources->count_crtcs; i++)
	{
		/* possible_crtcs is a bitmask as described here:
		 * https://dvdhrm.wordpress.com/2012/09/13/linux-drm-mode-setting-api
		 */
		const uint32_t crtc_mask = 1U << i;
		const uint32_t crtc_id = resources->crtcs[i];
		if (encoder->possible_crtcs & crtc_mask)
		{
			return crtc_id;
		}
	}

	/* no match found */
	return 0;
}

static uint32_t find_crtc_for_connector(const struct drm *drm, const drmModeRes *resources, const drmModeConnector *connector)
{
	for (int i = 0; i < connector->count_encoders; i++)
	{
		const uint32_t encoder_id = connector->encoders[i];
		drmModeEncoder *encoder = drmModeGetEncoder(drm->fd, encoder_id);

		if (encoder)
		{
			const uint32_t crtc_id = find_crtc_for_encoder(resources, encoder);

			drmModeFreeEncoder(encoder);
			if (crtc_id != 0)
			{
				return crtc_id;
			}
		}
	}

	/* no match found */
	return 0;
}

static int get_resources(int fd, drmModeRes **resources)
{
	*resources = drmModeGetResources(fd);
	if (*resources == NULL)
		return -1;
	return 0;
}

#define MAX_DRM_DEVICES 64

static int find_drm_device(drmModeRes **resources)
{
	drmDevicePtr devices[MAX_DRM_DEVICES] = { NULL };
	int num_devices, fd = -1;

	num_devices = drmGetDevices2(0, devices, MAX_DRM_DEVICES);
	if (num_devices < 0)
	{
		printf("drmGetDevices2 failed: %s\n", strerror(-num_devices));
		return -1;
	}

	for (int i = 0; i < num_devices; i++)
	{
		drmDevicePtr device = devices[i];
		int ret;

		if (!(device->available_nodes & (1 << DRM_NODE_PRIMARY)))
			continue;
		/* OK, it's a primary device. If we can get the
		 * drmModeResources, it means it's also a
		 * KMS-capable device.
		 */
		fd = open(device->nodes[DRM_NODE_PRIMARY], O_RDWR);
		if (fd < 0)
			continue;
		ret = get_resources(fd, resources);
		if (!ret)
			break;
		close(fd);
		fd = -1;
	}
	drmFreeDevices(devices, num_devices);

	if (fd < 0)
		printf("no drm device found!\n");
	return fd;
}

int init_drm(struct drm *drm, const char *device, const char *mode_str,
		unsigned int vrefresh)
{
	drmModeRes *resources;
	drmModeConnector *connector = NULL;
	drmModeEncoder *encoder = NULL;
	int i, ret, area;

	if (device)
	{
		drm->fd = open(device, O_RDWR);
		ret = get_resources(drm->fd, &resources);
		if (ret < 0 && errno == EOPNOTSUPP)
			printf("%s does not look like a modeset device\n", device);
	}
	else
	{
		drm->fd = find_drm_device(&resources);
	}

	if (drm->fd < 0)
	{
		printf("could not open drm device\n");
		return -1;
	}

	if (!resources)
	{
		printf("drmModeGetResources failed: %s\n", strerror(errno));
		return -1;
	}

	/* find a connected connector: */
	for (i = 0; i < resources->count_connectors; i++)
	{
		connector = drmModeGetConnector(drm->fd, resources->connectors[i]);
		if (connector->connection == DRM_MODE_CONNECTED)
		{
			/* it's connected, let's use this! */
			break;
		}
		drmModeFreeConnector(connector);
		connector = NULL;
	}

	if (!connector)
	{
		/* we could be fancy and listen for hotplug events and wait for
		 * a connector..
		 */
		printf("no connected connector!\n");
		return -1;
	}

	/* find user requested mode: */
	if (mode_str && *mode_str)
	{
		for (i = 0; i < connector->count_modes; i++)
		{
			drmModeModeInfo *current_mode = &connector->modes[i];

			if (strcmp(current_mode->name, mode_str) == 0)
			{
				if (vrefresh == 0 || current_mode->vrefresh == vrefresh)
				{
					drm->mode = current_mode;
					break;
				}
			}
		}
		if (!drm->mode)
			printf("requested mode not found, using default mode!\n");
	}

	/* find preferred mode or the highest resolution mode: */
	if (!drm->mode)
	{
		for (i = 0, area = 0; i < connector->count_modes; i++)
		{
			drmModeModeInfo *current_mode = &connector->modes[i];

			if (current_mode->type & DRM_MODE_TYPE_PREFERRED)
			{
				drm->mode = current_mode;
				break;
			}

			int current_area = current_mode->hdisplay * current_mode->vdisplay;
			if (current_area > area)
			{
				drm->mode = current_mode;
				area = current_area;
			}
		}
	}

	if (!drm->mode)
	{
		printf("could not find mode!\n");
		return -1;
	}

	/* find encoder: */
	for (i = 0; i < resources->count_encoders; i++)
	{
		encoder = drmModeGetEncoder(drm->fd, resources->encoders[i]);
		if (encoder->encoder_id == connector->encoder_id)
			break;
		drmModeFreeEncoder(encoder);
		encoder = NULL;
	}

	if (encoder)
	{
		drm->crtc_id = encoder->crtc_id;
	}
	else
	{
		uint32_t crtc_id = find_crtc_for_connector(drm, resources, connector);
		if (crtc_id == 0)
		{
			printf("no crtc found!\n");
			return -1;
		}

		drm->crtc_id = crtc_id;
	}

	drmModeFreeResources(resources);

	drm->connector_id = connector->connector_id;

    drm->saved_connector = connector;
    drm->saved_encoder = encoder;

    // get original display mode so we can restore display mode after program exits
    drm->original_crtc = drmModeGetCrtc(drm->fd, drm->crtc_id);

	if(getenv("SFML_DRM_DEBUG"))
	{
		printf("DRM Mode used: %s@%d\n", drm->mode->name, drm->mode->vrefresh);
	}

	return 0;
}
