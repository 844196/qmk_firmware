#include QMK_KEYBOARD_H

enum layers {
    BASE = 0,
    ARROWS,
    FN,
    DANGER
};

enum tap_dances {
    TD_LCTL_LAUNCHER,
    TD_RCTL_ESC,
    TD_RSFT_ESC,
    TD_FN_MCTL
};

typedef enum {
    TD_NONE,
    TD_UNKNOWN,
    TD_SINGLE_TAP,
    TD_SINGLE_HOLD,
    TD_DOUBLE_TAP,
    TD_DOUBLE_HOLD,
    TD_DOUBLE_SINGLE_TAP, // Send two single taps
    TD_TRIPLE_TAP,
    TD_TRIPLE_HOLD
} td_state_t;

typedef struct {
    bool is_press_action;
    td_state_t state;
} td_tap_t;

td_state_t cur_dance(tap_dance_state_t *state) {
    if (state->count == 1) {
        if (state->interrupted || !state->pressed) return TD_SINGLE_TAP;
        // Key has not been interrupted, but the key is still held. Means you want to send a 'HOLD'.
        else return TD_SINGLE_HOLD;
    } else if (state->count == 2) {
        // TD_DOUBLE_SINGLE_TAP is to distinguish between typing "pepper", and actually wanting a double tap
        // action when hitting 'pp'. Suggested use case for this return value is when you want to send two
        // keystrokes of the key, and not the 'double tap' action/macro.
        if (state->interrupted) return TD_DOUBLE_SINGLE_TAP;
        else if (state->pressed) return TD_DOUBLE_HOLD;
        else return TD_DOUBLE_TAP;
    }

    // Assumes no one is trying to type the same letter three times (at least not quickly).
    // If your tap dance key is 'KC_W', and you want to type "www." quickly - then you will need to add
    // an exception here to return a 'TD_TRIPLE_SINGLE_TAP', and define that enum just like 'TD_DOUBLE_SINGLE_TAP'
    if (state->count == 3) {
        if (state->interrupted || !state->pressed) return TD_TRIPLE_TAP;
        else return TD_TRIPLE_HOLD;
    } else return TD_UNKNOWN;
}

// Combos
const uint16_t PROGMEM combo_brackets[] = {KC_LBRC, KC_RBRC, COMBO_END};

combo_t key_combos[COMBO_COUNT] = {
    COMBO(combo_brackets, MO(DANGER))
};

// Key overrides
const key_override_t override_c_h = ko_make_basic(MOD_BIT(KC_RCTL), KC_H, KC_BSPC);
const key_override_t override_c_grv = ko_make_basic(MOD_BIT(KC_RCTL), KC_GRV, VSCODE_TOGGLE_TERMINAL);

bool esc_ime_off(bool key_down, void *context) {
    tap_code(IME_OFF);
    return true;
}
const key_override_t esc_with_ime_off = {
    .layers = ~0,
    .trigger_mods = MOD_BIT(KC_RCTL),
    .trigger = KC_LBRC,
    .suppressed_mods = MOD_BIT(KC_RCTL),
    .negative_mod_mask = 0,
    .replacement = KC_ESC,
    .options = ko_options_default,
    .custom_action = esc_ime_off,
    .context = NULL,
    .enabled = NULL
};

const key_override_t **key_overrides = (const key_override_t *[]){
    &override_c_h,
    &override_c_grv,
    &esc_with_ime_off,
    NULL
};

// Tap dances
static td_tap_t fn_state = {
    .is_press_action = true,
    .state = TD_NONE
};

void fn_finished(tap_dance_state_t *state, void *user_data) {
    fn_state.state = cur_dance(state);
    switch (fn_state.state) {
        case TD_SINGLE_TAP:
            if (state->interrupted) {
                layer_on(FN);
            } else {
                register_code(KC_MPLY);
            }
            break;
        case TD_SINGLE_HOLD:
            layer_on(FN);
            break;
        case TD_DOUBLE_TAP:
            register_code(KC_MNXT);
            break;
        case TD_TRIPLE_TAP:
            register_code(KC_MPRV);
            break;
        default:
            break;
    }
}

void fn_reset(tap_dance_state_t *state, void *user_data) {
    switch (fn_state.state) {
        case TD_SINGLE_TAP:
            if (state->interrupted) {
                layer_off(FN);
            } else {
                unregister_code(KC_MPLY);
            }
            break;
        case TD_SINGLE_HOLD:
            layer_off(FN);
            break;
        case TD_DOUBLE_TAP:
            unregister_code(KC_MNXT);
            break;
        case TD_TRIPLE_TAP:
            unregister_code(KC_MPRV);
            break;
        default:
            break;
    }
    fn_state.state = TD_NONE;
}

tap_dance_action_t tap_dance_actions[] = {
    [TD_LCTL_LAUNCHER] = ACTION_TAP_DANCE_DOUBLE(KC_LCTL, POWER_TOYS_RUN_ACTIVATE),
    [TD_RCTL_ESC] = ACTION_TAP_DANCE_DOUBLE(KC_RCTL, KC_ESC),
    [TD_RSFT_ESC] = ACTION_TAP_DANCE_DOUBLE(KC_RSFT, KC_ESC),
    [TD_FN_MCTL] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, fn_finished, fn_reset)
};

// Aliases
#define MY_EISU LCTL_T(IME_OFF)
#define MY_KANA RGUI_T(IME_ON)
#define MY_LCTL TD(TD_RCTL_ESC)
#define MY_RCTL TD(TD_LCTL_LAUNCHER)
#define MY_RSFT TD(TD_RSFT_ESC)
#define MY_Z    LT(ARROWS, KC_Z)
#define MY_FN   TD(TD_FN_MCTL)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [BASE] = LAYOUT_60_hhkb(
        KC_ESC,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,  KC_EQL,   KC_BSLS,  KC_GRV,
        KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,  KC_RBRC,            KC_BSPC,
        MY_LCTL,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,                      KC_ENT,
        KC_LSFT,            MY_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,            MY_RSFT,  MY_FN,
                            KC_LALT,  MY_EISU,                                KC_SPC,                                 MY_KANA,  MY_RCTL
    ),

    [ARROWS] = LAYOUT_60_hhkb(
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,
        _______,  _______,  _______,  _______,  _______,  _______,  KC_LEFT,  KC_DOWN,  KC_UP,    KC_RGHT,  _______,  _______,                      _______,
        _______,            _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,  _______,
                            _______,  _______,                                _______,                                _______,  _______
    ),

    [FN] = LAYOUT_60_hhkb(
        _______,  KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,   KC_INS,   KC_DEL,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  KC_UP,    _______,            _______,
        _______,  KC_VOLD,  KC_VOLU,  KC_MUTE,  _______,  _______,  _______,  _______,  _______,  _______,  KC_LEFT,  KC_RGHT,                      _______,
        _______,            _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  KC_DOWN,            _______,  _______,
                            _______,  _______,                                _______,                                _______,  _______
    ),

    [DANGER] = LAYOUT_60_hhkb(
        QK_BOOT,  QK_MAKE,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,                      _______,
        _______,            _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,  _______,
                            _______,  _______,                                _______,                                _______,  _______
    ),
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (keycode == MY_EISU && record->tap.count && record->event.pressed) {
        tap_code16(IME_EISU_HENKAN);
    }
    return true;
}

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case MY_RCTL:
            return TAPPING_TERM + 25;
        case MY_FN:
            return TAPPING_TERM + 100;
        default:
            return TAPPING_TERM;
    }
}

bool get_permissive_hold(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case MY_Z:
            return true;
        default:
            return false;
    }
}

bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case MY_EISU:
        case MY_KANA:
            return true;
        default:
            return false;
    }
}
