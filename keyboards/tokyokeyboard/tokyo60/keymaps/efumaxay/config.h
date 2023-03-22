#pragma once

#define ENABLE_COMPILE_KEYCODE

#define COMBO_COUNT 1
#define COMBO_SHOULD_TRIGGER

#define TAPPING_TERM 200 // Explicitly
#define TAPPING_TERM_PER_KEY
#define PERMISSIVE_HOLD_PER_KEY
#define HOLD_ON_OTHER_KEY_PRESS_PER_KEY

/**
 * 以下を事前にIMEへ設定
 *
 * - IMEを無効化 直接入力
 * - IMEを無効化 入力文字なし
 * - IMEを無効化 変換前入力中
 * - IMEを無効化 変換中
 * - IMEを無効化 サジェスト表示中
 * - IMEを無効化 サジェスト選択中
 */
#define IME_OFF KC_F13

/**
 * 以下を事前にIMEへ設定
 *
 * - IMEを有効化 直接入力
 * - IMEを有効化 入力文字なし
 */
#define IME_ON KC_F14

/**
 * 以下を事前にIMEへ設定
 *
 * - 半角英数に変換 変換前入力中
 * - 半角英数に変換 変換中
 */
#define IME_EISU_HENKAN KC_F23

/**
 * 事前に PowerToys Run の "アクティブ化のショートカット" に設定
 */
#define POWER_TOYS_RUN_ACTIVATE RALT(KC_SPC)

/**
 * 事前に VSCode の `workbench.action.terminal.toggleTerminal` に設定
 */
#define VSCODE_TOGGLE_TERMINAL KC_F19
