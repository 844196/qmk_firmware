/**
 * This program forked from keymap.c:
 * Copyright 2021 @ Keychron (https://www.keychron.com)
 * Release under the GNU General Public License (GPL) version 2.
 */

/* Copyright 2021 @ Keychron (https://www.keychron.com)
 * Copyright 2023 Masaya Takeda <844196@gmail.com>
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

// clang-format off

enum layers {
    BASE,
    GAME,
    FN,
    DANGER
};

enum {
    TD_RALT_LAUNCHER,
    TD_B_TOGGLE_TERMINAL,
    TD_RCTL_ESC,
    TD_MEDIA_CONTROL
};

uint16_t CURRENT_BASE_LAYER = BASE;

void earpods(tap_dance_state_t *state, void *user_data) {
    switch (state->count) {
        case 1:
            tap_code(KC_MPLY); // このボタンおせば おんがく かなでる
            break;
        case 2:
            tap_code(KC_MNXT);
            break;
        default:
            tap_code(KC_MPRV);
            reset_tap_dance(state);
            break;
    }
}

tap_dance_action_t tap_dance_actions[] = {
    [TD_RALT_LAUNCHER] = ACTION_TAP_DANCE_DOUBLE(KC_RALT, POWER_TOYS_RUN_ACTIVATE),
    [TD_B_TOGGLE_TERMINAL] = ACTION_TAP_DANCE_DOUBLE(KC_B, VSCODE_TOGGLE_TERMINAL),
    [TD_RCTL_ESC] = ACTION_TAP_DANCE_DOUBLE(KC_RCTL, KC_ESC),
    [TD_MEDIA_CONTROL] = ACTION_TAP_DANCE_FN(earpods)
};

const key_override_t override_c_h = ko_make_basic(MOD_BIT(KC_RCTL), KC_H, KC_BSPC);
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
    &esc_with_ime_off,
    NULL // Null terminate the array of overrides!
};

const uint16_t PROGMEM combo_bracket[] = {KC_LBRC, KC_RBRC, COMBO_END};
combo_t key_combos[COMBO_COUNT] = {
    COMBO(combo_bracket, MO(DANGER))
};

#define KC_EISU LALT_T(IME_OFF)
#define KC_KANA RGUI_T(IME_ON)
#define KC_SPFN LT(FN, KC_SPC)
#define TD_RALT TD(TD_RALT_LAUNCHER)
#define TD_B TD(TD_B_TOGGLE_TERMINAL)
#define VS_TERM VSCODE_TOGGLE_TERMINAL
#define TD_RCTL TD(TD_RCTL_ESC)
#define TD_MCTL TD(TD_MEDIA_CONTROL)
#define KC_Z_FN LT(FN, KC_Z)

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case KC_EISU:
            if (record->tap.count && record->event.pressed) {
                tap_code16(IME_EISU_HENKAN);
            }
            return true;
        default:
            return true;
    }
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [BASE] = LAYOUT_ansi_69(
        KC_GRV,  KC_1,     KC_2,     KC_3,    KC_4,    KC_5,    KC_6,     KC_7,    KC_8,    KC_9,    KC_0,     KC_MINS,  KC_EQL,   KC_BSPC,          KC_MUTE,
        KC_TAB,  KC_Q,     KC_W,     KC_E,    KC_R,    KC_T,    KC_Y,     KC_U,    KC_I,    KC_O,    KC_P,     KC_LBRC,  KC_RBRC,  KC_BSLS,          TD_MCTL,
        TD_RCTL, KC_A,     KC_S,     KC_D,    KC_F,    KC_G,              KC_H,    KC_J,    KC_K,    KC_L,     KC_SCLN,  KC_QUOT,  KC_ENT,           VS_TERM,
        KC_LSFT,           KC_Z_FN,  KC_X,    KC_C,    KC_V,    KC_B,     KC_B,    KC_N,    KC_M,    KC_COMM,  KC_DOT,   KC_SLSH,  MO(FN),  KC_UP,
        MO(FN),  KC_LALT,  KC_LCTL,           KC_SPC,           KC_EISU,  KC_KANA,          KC_SPFN,           TD_RALT,            KC_LEFT, KC_DOWN, KC_RGHT),

    [GAME] = LAYOUT_ansi_69(
        _______, _______,  _______,  _______, _______, _______, _______,  _______, _______, _______, _______,  _______,  _______,  _______,          _______,
        _______, _______,  _______,  _______, _______, _______, _______,  _______, _______, _______, _______,  _______,  _______,  _______,          _______,
        KC_RCTL, _______,  _______,  _______, _______, _______,           _______, _______, _______, _______,  _______,  _______,  _______,          _______,
        _______,           KC_Z,     _______, _______, _______, _______,  _______, _______, _______, _______,  _______,  _______,  _______, _______,
        _______, _______,  _______,           _______,          _______,  _______,          KC_SPC,            _______,            _______, _______, _______),

    [FN] = LAYOUT_ansi_69(
        KC_ESC,  KC_F1,    KC_F2,    KC_F3,   KC_F4,   KC_F5,   KC_F6,    KC_F7,   KC_F8,   KC_F9,   KC_F10,   KC_F11,   KC_F12,   KC_DEL,           _______,
        _______, _______,  _______,  _______, _______, _______, _______,  _______, _______, _______, _______,  _______,  _______,  _______,          _______,
        _______, _______,  _______,  _______, _______, _______,           KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT,  _______,  _______,  _______,          QK_MAKE,
        _______,           _______,  _______, _______, _______, _______,  _______, _______, _______, _______,  _______,  _______,  _______, _______,
        _______, _______,  _______,           _______,          _______,  _______,          _______,           _______,            _______, _______, _______),

    [DANGER] = LAYOUT_ansi_69(
        QK_BOOT, _______,  _______,  _______, _______, _______, _______,  _______, _______, _______, _______,  _______,  _______,  _______,          _______,
        _______, _______,  _______,  _______, _______, _______, _______,  _______, _______, _______, _______,  _______,  _______,  _______,          _______,
        _______, _______,  _______,  _______, _______, _______,           _______, _______, _______, _______,  _______,  _______,  _______,          _______,
        _______,           _______,  _______, _______, _______, _______,  _______, _______, _______, _______,  _______,  _______,  _______, _______,
        _______, _______,  _______,           _______,          _______,  _______,          _______,           _______,            _______, _______, _______)
};

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case KC_SPFN:
            return TAPPING_TERM + 150;
        case TD_RALT:
            return TAPPING_TERM + 25;
        default:
            return TAPPING_TERM;
    }
}

bool get_permissive_hold(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case KC_SPFN:
        case KC_Z_FN:
            return true; // ローリングプレス
        default:
            return false;
    }
}

bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case KC_EISU:
        case KC_KANA:
            return true; // TAPPING_TERM 以内であっても、別のキーを押した場合はすぐに Alt-〇 (もしくはWin-〇) として扱う
        default:
            return false;
    }
}

bool combo_should_trigger(uint16_t combo_index, combo_t *combo, uint16_t keycode, keyrecord_t *record) {
    return CURRENT_BASE_LAYER == BASE;
}

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
    [BASE] = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
    [GAME] = {ENCODER_CCW_CW(_______, _______)},
    [FN]   = {ENCODER_CCW_CW(KC_MPRV, KC_MNXT)},
    [DANGER] = {ENCODER_CCW_CW(_______, _______)}
};
#endif // ENCODER_MAP_ENABLE

#if defined(RGB_MATRIX_ENABLE)
void keyboard_post_init_user(void) {
    rgb_matrix_set_flags(LED_FLAG_NONE);
    rgb_matrix_set_color_all(RGB_OFF);
    rgb_matrix_enable_noeeprom();
}

layer_state_t default_layer_state_set_user(layer_state_t state) {
    CURRENT_BASE_LAYER = get_highest_layer(state);
    switch (CURRENT_BASE_LAYER) {
        case GAME:
            rgb_matrix_set_color(W_LED_INDEX, RGB_RED);
            rgb_matrix_set_color(A_LED_INDEX, RGB_RED);
            rgb_matrix_set_color(S_LED_INDEX, RGB_RED);
            rgb_matrix_set_color(D_LED_INDEX, RGB_RED);
            break;
        default:
            rgb_matrix_set_color(W_LED_INDEX, RGB_OFF);
            rgb_matrix_set_color(A_LED_INDEX, RGB_OFF);
            rgb_matrix_set_color(S_LED_INDEX, RGB_OFF);
            rgb_matrix_set_color(D_LED_INDEX, RGB_OFF);
            break;
    }
    return state;
}

layer_state_t layer_state_set_user(layer_state_t state) {
    switch (get_highest_layer(state)) {
        case FN:
            rgb_matrix_set_color(H_LED_INDEX, RGB_GREEN);
            rgb_matrix_set_color(J_LED_INDEX, RGB_GREEN);
            rgb_matrix_set_color(K_LED_INDEX, RGB_GREEN);
            rgb_matrix_set_color(L_LED_INDEX, RGB_GREEN);
            break;
        case DANGER:
            rgb_matrix_set_color(GRV_LED_INDEX, RGB_RED);
            rgb_matrix_set_color(ONE_LED_INDEX, RGB_RED);
            break;
        default:
            rgb_matrix_set_color(GRV_LED_INDEX, RGB_OFF);
            rgb_matrix_set_color(ONE_LED_INDEX, RGB_OFF);
            rgb_matrix_set_color(H_LED_INDEX, RGB_OFF);
            rgb_matrix_set_color(J_LED_INDEX, RGB_OFF);
            rgb_matrix_set_color(K_LED_INDEX, RGB_OFF);
            rgb_matrix_set_color(L_LED_INDEX, RGB_OFF);
            break;
    }
    return state;
}
#endif // RGB_MATRIX_ENABLE
