#ifndef SIM_GAME_STATE_ENUM
#define SIM_GAME_STATE_ENUM

enum GameState : uint {GS_ignore_input, GS_main_menu, GS_browse_herald, GS_court, 
    GS_select, GS_conversation_choice, GS_entry, GS_conversation_typing, GS_conversation_displayed, 
    GS_describe, GS_retinue_menu, GS_skirmish, GS_queued_conversation, GS_queued_skirmish,
    GS_hunt_menu, GS_tracking, GS_mirror, GS_maps, GS_provinces_menu, GS_province_category_menu,
    GS_province_items};

#endif
