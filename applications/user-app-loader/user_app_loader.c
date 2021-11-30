#include <furi.h>
#include <gui/gui.h>
#include <input/input.h>
#include <furi-hal.h>

typedef enum {
    EventTypeTick,
    EventTypeKey,
} EventType;

typedef struct {
    EventType type;
    InputEvent input;
} BlinkEvent;

static void render(Canvas* canvas, void* ctx) {
    canvas_clear(canvas);
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str(canvas, 2, 10, "User app example");
    canvas_draw_str(canvas, 2, 20, "Hold back to exit");
    size_t flash_start = (size_t)furi_hal_flash_get_free_start_address();
    size_t flash_end = (size_t)furi_hal_flash_get_free_end_address();
    char str[32];
    sprintf(str, "start: 0x%x", flash_start);
    canvas_draw_str(canvas, 2, 30, str);
    sprintf(str, "end: 0x%x", flash_end);
    canvas_draw_str(canvas, 2, 40, str);
}

static void handle_input(InputEvent* input_event, void* ctx) {
    furi_assert(ctx);
    osMessageQueueId_t event_queue = ctx;

    BlinkEvent event = {.type = EventTypeKey, .input = *input_event};
    osMessageQueuePut(event_queue, &event, 0, 0);
}

int32_t user_app_example(void* p) {
    osMessageQueueId_t event_queue = osMessageQueueNew(8, sizeof(BlinkEvent), NULL);

    ViewPort* view_port = view_port_alloc();
    furi_check(view_port);
    view_port_draw_callback_set(view_port, render, NULL);
    view_port_input_callback_set(view_port, handle_input, event_queue);

    Gui* gui = furi_record_open("gui");
    gui_add_view_port(gui, view_port, GuiLayerFullscreen);

    BlinkEvent event;

    while(1) {
        furi_check(osMessageQueueGet(event_queue, &event, NULL, osWaitForever) == osOK);
        if(event.type == EventTypeKey) {
            if((event.input.type == InputTypeLong) && (event.input.key == InputKeyBack)) {
                break;
            }
        }
    }

    view_port_enabled_set(view_port, false);
    gui_remove_view_port(gui, view_port);
    view_port_free(view_port);
    osMessageQueueDelete(event_queue);

    return 0;
}

int32_t __attribute__((__section__(".userapp_wrap"))) __attribute__((weak)) userapp_wrap(void* p) {
    return user_app_example(p);
}

int32_t user_app_loader(void* p) {
    return userapp_wrap(p);
}
