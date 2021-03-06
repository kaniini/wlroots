#ifndef BACKEND_DRM_RENDERER_H
#define BACKEND_DRM_RENDERER_H

#include <EGL/egl.h>
#include <gbm.h>
#include <stdbool.h>
#include <stdint.h>
#include <wlr/backend.h>
#include <wlr/render/wlr_renderer.h>

struct wlr_drm_backend;
struct wlr_drm_plane;
struct wlr_buffer;

struct wlr_drm_renderer {
	struct wlr_drm_backend *backend;
	struct gbm_device *gbm;
	struct wlr_egl egl;

	struct wlr_renderer *wlr_rend;
	struct wlr_gbm_allocator *allocator;
};

struct wlr_drm_surface {
	struct wlr_drm_renderer *renderer;

	uint32_t width;
	uint32_t height;

	struct wlr_swapchain *swapchain;
	struct wlr_buffer *back_buffer;
};

struct wlr_drm_fb {
	struct wlr_buffer *wlr_buf;

	struct wlr_buffer *mgpu_wlr_buf;

	struct gbm_bo *bo;
	uint32_t id;
};

bool init_drm_renderer(struct wlr_drm_backend *drm,
	struct wlr_drm_renderer *renderer, wlr_renderer_create_func_t create_render);
void finish_drm_renderer(struct wlr_drm_renderer *renderer);

bool drm_surface_make_current(struct wlr_drm_surface *surf, int *buffer_age);
void drm_surface_unset_current(struct wlr_drm_surface *surf);

void drm_fb_clear(struct wlr_drm_fb *fb);
bool drm_fb_lock_surface(struct wlr_drm_fb *fb, struct wlr_drm_backend *drm,
		struct wlr_drm_surface *surf, struct wlr_drm_surface *mgpu);
bool drm_fb_import(struct wlr_drm_fb *fb, struct wlr_drm_backend *drm,
		struct wlr_buffer *buf, struct wlr_drm_surface *mgpu,
		struct wlr_drm_format_set *set);

void drm_fb_move(struct wlr_drm_fb *new, struct wlr_drm_fb *old);

bool drm_surface_render_black_frame(struct wlr_drm_surface *surf);

bool drm_plane_init_surface(struct wlr_drm_plane *plane,
		struct wlr_drm_backend *drm, int32_t width, uint32_t height,
		uint32_t format, bool force_linear, bool with_modifiers);
void drm_plane_finish_surface(struct wlr_drm_plane *plane);

#endif
