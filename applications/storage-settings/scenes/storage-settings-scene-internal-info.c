#include "../storage-settings.h"
#include <furi-hal-version.h>

static void
    storage_settings_scene_internal_info_dialog_callback(DialogExResult result, void* context) {
    StorageSettings* app = context;

    view_dispatcher_send_custom_event(app->view_dispatcher, result);
}

void storage_settings_scene_internal_info_on_enter(void* context) {
    StorageSettings* app = context;
    uint64_t total_space;
    uint64_t free_space;
    FS_Error error = storage_common_fs_info(app->fs_api, "/int", &total_space, &free_space);
    DialogEx* dialog_ex = app->dialog_ex;

    dialog_ex_set_context(dialog_ex, app);
    dialog_ex_set_result_callback(dialog_ex, storage_settings_scene_internal_info_dialog_callback);

    dialog_ex_set_left_button_text(dialog_ex, "Back");
    if(error != FSE_OK) {
        dialog_ex_set_header(
            dialog_ex, "Internal storage error", 64, 10, AlignCenter, AlignCenter);
        dialog_ex_set_text(
            dialog_ex, storage_error_get_desc(error), 64, 32, AlignCenter, AlignCenter);
    } else {
        string_printf(
            app->text_string,
            "Label: %s\nType: LittleFS\n%lu KB total\n%lu KB free",
            furi_hal_version_get_name_ptr() ? furi_hal_version_get_name_ptr() : "Unknown",
            (uint32_t)(total_space / 1024),
            (uint32_t)(free_space / 1024));
        dialog_ex_set_text(
            dialog_ex, string_get_cstr(app->text_string), 4, 4, AlignLeft, AlignTop);
    }

    view_dispatcher_switch_to_view(app->view_dispatcher, StorageSettingsViewDialogEx);
}

bool storage_settings_scene_internal_info_on_event(void* context, SceneManagerEvent event) {
    StorageSettings* app = context;
    bool consumed = false;

    if(event.type == SceneManagerEventTypeCustom) {
        switch(event.event) {
        case DialogExResultLeft:
            consumed = scene_manager_previous_scene(app->scene_manager);
            break;
        }
    }
    return consumed;
}

void storage_settings_scene_internal_info_on_exit(void* context) {
    StorageSettings* app = context;
    DialogEx* dialog_ex = app->dialog_ex;

    dialog_ex_set_header(dialog_ex, NULL, 0, 0, AlignCenter, AlignCenter);
    dialog_ex_set_text(dialog_ex, NULL, 0, 0, AlignCenter, AlignTop);
    dialog_ex_set_icon(dialog_ex, 0, 0, NULL);
    dialog_ex_set_left_button_text(dialog_ex, NULL);
    dialog_ex_set_right_button_text(dialog_ex, NULL);
    dialog_ex_set_result_callback(dialog_ex, NULL);
    dialog_ex_set_context(dialog_ex, NULL);

    string_clean(app->text_string);
}
