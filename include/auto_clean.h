#pragma once

typedef struct MediaAutoCleanCtx {
    void (*func)(void*);
    void* data;
    bool is_cleanup;
} MediaAutoCleanCtx;

static void MediaCleanup(MediaAutoCleanCtx* ctx) {
    if (ctx->is_cleanup && ctx->func && ctx->data) {
        ctx->func(ctx->data);
    }
}

#define MEDIA_AUTO_CLEAN_RESOURCE1(func, data) \
    MediaAutoCleanCtx ctx_##data __attribute__((cleanup(MediaCleanup))) = {func, &data, true}

#define MEDIA_AUTO_CLEAN_RESOURCE2(_func, _data) \
    MediaAutoCleanCtx ctx_##data __attribute__((cleanup(MediaCleanup))) = { \
        .func = _func, \
        .data = &_data, \
        .is_cleanup = true \
    }

void cleanup_int(void *ptr);
