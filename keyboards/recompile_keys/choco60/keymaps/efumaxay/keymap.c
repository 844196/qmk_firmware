/* Copyright 2019 Naoto Takai
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include QMK_KEYBOARD_H
#include "efumaxay.h"
#include "cur_dance.h"
#include "td_state.h"
#include "td_tap.h"

typedef struct {
    bool is_repeat;
    int pressed_time;
} repeat_state_t;


/**
 * Enums
 */
enum layers {
    _BASE,
    _FN,
    _JKL,
    _DANGER
};

enum combos {
    FA_LGUI,
    FS_LALT,
    FD_LCTL,
    JK_LCTL,
    JL_LALT,
    JS_LGUI,
    DANGER_ENTRY
};

enum tap_dances {
    TD_LALT_LAUNCHER,
    TD_RCTL_ESC,
    TD_FN_MEDIA_CONTROL,
    TD_RSFT_ESC_FN
};


/**
 * Aliases
 */
#define MY_FN   TD(TD_FN_MEDIA_CONTROL)
#define MY_Z    LT(_FN, KC_Z)
#define MY_CALT MT(MOD_LALT, KC_LNG1)
#define MY_SPFN LT(_FN, KC_SPC)
#define MY_CTL  TD(TD_RCTL_ESC)
#define MY_LCTL MT(MOD_LCTL, KC_LNG2)
#define MY_RCTL MT(MOD_LCTL, KC_LNG1)
#define MY_RALT TD(TD_LALT_LAUNCHER)
#define MY_RSFT TD(TD_RSFT_ESC_FN)
#define MY_SPC  LT(_FN, KC_LNG1)


/**
 * Tap-Hold Configurations
 */
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case MY_SPFN:
            return TAPPING_TERM + 150;
        case MY_CTL:
            return TAPPING_TERM + 50;
        case MY_FN:
            return TAPPING_TERM + 100;
        default:
            return TAPPING_TERM;
    }
}

bool get_permissive_hold(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case MY_SPC:
        case MY_Z:
            return true; // ローリングプレス
        default:
            return false;
    }
}

bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case MY_LCTL:
        case MY_RCTL:
            return true;
        default:
            return false;
    }
}


/**
 * Keymaps
 */
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT(
        KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,   KC_0,    KC_MINS, KC_EQL,  KC_BSLS, KC_GRV,
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,   KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,
        MY_CTL,  KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,   KC_SCLN, KC_QUOT, XXXXXXX,
        KC_LSFT, MY_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT, KC_SLSH, MY_RSFT, MY_FN,
                 KC_LALT, MY_LCTL, KC_SPC,           KC_ENT,           MY_SPC,  MY_RCTL, MY_RALT
    ),

    [_FN] = LAYOUT(
        KC_ESC,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  _______, KC_DEL,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, KC_VOLD, KC_VOLU, KC_MUTE, _______, _______, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
                 _______, _______, _______,          _______,          _______, _______, _______
    ),

    [_JKL] = LAYOUT(
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, KC_LSFT, KC_LSFT, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
                 _______, _______, _______,          _______,          _______, _______, _______
    ),

    [_DANGER] = LAYOUT(
        QK_BOOT, QK_MAKE, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, QK_BOOT,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
                 _______, _______, _______,          _______,          _______, _______, _______
    )
};


/**
 * Combos
 */
const uint16_t PROGMEM fa_lgui[] = {KC_F, KC_A, COMBO_END};
const uint16_t PROGMEM fs_lalt[] = {KC_F, KC_S, COMBO_END};
const uint16_t PROGMEM fd_lctl[] = {KC_F, KC_D, COMBO_END};
const uint16_t PROGMEM jk_lctl[] = {KC_J, KC_K, COMBO_END};
const uint16_t PROGMEM jl_lalt[] = {KC_J, KC_L, COMBO_END};
const uint16_t PROGMEM js_lgui[] = {KC_J, KC_SCLN, COMBO_END};

const uint16_t PROGMEM danger_entry[] = {KC_LBRC, KC_RBRC, COMBO_END};

combo_t key_combos[] = {
    [FA_LGUI] = COMBO(fa_lgui, KC_LGUI),
    [FS_LALT] = COMBO(fs_lalt, KC_LALT),
    [FD_LCTL] = COMBO(fd_lctl, KC_LCTL),
    [JK_LCTL] = COMBO_ACTION(jk_lctl),
    [JL_LALT] = COMBO_ACTION(jl_lalt),
    [JS_LGUI] = COMBO(js_lgui, KC_LGUI),

    [DANGER_ENTRY] = COMBO(danger_entry, MO(_DANGER))
};

void process_combo_event(uint16_t combo_index, bool pressed) {
    switch (combo_index) {
        case JK_LCTL:
            if (pressed) {
                register_code(KC_LCTL);
                layer_on(_JKL);
            } else {
                unregister_code(KC_LCTL);
                layer_off(_JKL);
            }
            break;
        case JL_LALT:
            if (pressed) {
                register_code(KC_LALT);
                layer_on(_JKL);
            } else {
                unregister_code(KC_LALT);
                layer_off(_JKL);
            }
            break;
        default:
            if (!pressed) {
                layer_off(_JKL);
            }
            break;
    }
}


/**
 * Key Overrides
 */
const key_override_t override_c_h = ko_make_basic(MOD_BIT(KC_RCTL), KC_H, KC_BSPC);
const key_override_t override_c_m = ko_make_basic(MOD_BIT(KC_RCTL), KC_M, KC_ENT);

const key_override_t override_print_screen = ko_make_basic(MOD_MASK_CS, KC_S, LSFT(LGUI(KC_S)));

bool custom_action_ime_off(bool key_down, void *context) {
    tap_code(KC_LNG2);
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
    .custom_action = custom_action_ime_off,
    .context = NULL,
    .enabled = NULL
};

const key_override_t **key_overrides = (const key_override_t *[]){
    &override_c_h,
    &override_c_m,
    &override_print_screen,
    &esc_with_ime_off,
    NULL
};


/**
 * Tap dances
 */
static td_tap_t rsft_state = {
    .is_press_action = true,
    .state = TD_NONE
};

void rsft_finished(tap_dance_state_t *state, void *user_data) {
    rsft_state.state = cur_dance(state);
    switch (rsft_state.state) {
        case TD_SINGLE_TAP:
            if (state->interrupted) {
                layer_on(_FN);
            } else {
                register_code(VSCODE_TOGGLE_TERMINAL);
            }
            break;
        case TD_SINGLE_HOLD:
            layer_on(_FN);
            break;
        case TD_DOUBLE_TAP:
            register_code(KC_ESC);
            break;
        default:
            break;
    }
}

void rsft_reset(tap_dance_state_t *state, void *user_data) {
    switch (rsft_state.state) {
        case TD_SINGLE_TAP:
            if (state->interrupted) {
                layer_off(_FN);
            } else {
                unregister_code(VSCODE_TOGGLE_TERMINAL);
            }
            break;
        case TD_SINGLE_HOLD:
            layer_off(_FN);
            break;
        case TD_DOUBLE_TAP:
            unregister_code(KC_ESC);
            break;
        default:
            break;
    }
    rsft_state.state = TD_NONE;
}

static td_tap_t earpods_state = {
    .is_press_action = true,
    .state = TD_NONE
};

static repeat_state_t ffd_repeat_state = {
    .is_repeat = false,
    .pressed_time = 0
};
static repeat_state_t rwd_repeat_state = {
    .is_repeat = false,
    .pressed_time = 0
};

void earpods_finished(tap_dance_state_t *state, void *user_data) {
    earpods_state.state = cur_dance(state);
    switch (earpods_state.state) {
        case TD_SINGLE_TAP:
            if (state->interrupted) {
                layer_on(_FN);
            } else {
                register_code(KC_MPLY);
            }
            break;
        case TD_SINGLE_HOLD:
            layer_on(_FN);
            break;
        case TD_DOUBLE_TAP:
            register_code(KC_MNXT);
            break;
        case TD_DOUBLE_HOLD:
            ffd_repeat_state.is_repeat = true;
            break;
        case TD_TRIPLE_TAP:
            register_code(KC_MPRV);
            break;
        case TD_TRIPLE_HOLD:
            rwd_repeat_state.is_repeat = true;
            break;
        default:
            break;
    }
}

void earpods_reset(tap_dance_state_t *state, void *user_data) {
    switch (earpods_state.state) {
        case TD_SINGLE_TAP:
            if (state->interrupted) {
                layer_off(_FN);
            } else {
                unregister_code(KC_MPLY);
            }
            break;
        case TD_SINGLE_HOLD:
            layer_off(_FN);
            break;
        case TD_DOUBLE_TAP:
            unregister_code(KC_MNXT);
            break;
        case TD_DOUBLE_HOLD:
            ffd_repeat_state.is_repeat = false;
            ffd_repeat_state.pressed_time = 0;
            break;
        case TD_TRIPLE_TAP:
            unregister_code(KC_MPRV);
            break;
        case TD_TRIPLE_HOLD:
            rwd_repeat_state.is_repeat = false;
            rwd_repeat_state.pressed_time = 0;
            break;
        default:
            break;
    }
    earpods_state.state = TD_NONE;
}

void matrix_scan_user(void) {
    if (ffd_repeat_state.is_repeat) {
        if (ffd_repeat_state.pressed_time % 500 == 0) {
            ffd_repeat_state.pressed_time = 0;
            tap_code(KC_MFFD);
        }
        ffd_repeat_state.pressed_time++;
    }
    if (rwd_repeat_state.is_repeat) {
        if (rwd_repeat_state.pressed_time % 500 == 0) {
            rwd_repeat_state.pressed_time = 0;
            tap_code(KC_MRWD);
        }
        rwd_repeat_state.pressed_time++;
    }
}

tap_dance_action_t tap_dance_actions[] = {
    [TD_LALT_LAUNCHER] = ACTION_TAP_DANCE_DOUBLE(KC_LALT, LALT(KC_SPC)),
    [TD_RCTL_ESC] = ACTION_TAP_DANCE_DOUBLE(KC_RCTL, KC_ESC),
    [TD_RSFT_ESC_FN] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, rsft_finished, rsft_reset),
    [TD_FN_MEDIA_CONTROL] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, earpods_finished, earpods_reset)
};
