/* Copyright 2023 @ Keychron (https://www.keychron.com)
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

/*
 * The M1-M5 macro keys sit in a column left of the alphas (row 1-5, col 0).
 * Whichever one currently has a TG(layer) keycode bound to it lights up
 * white while that layer is active - same mechanism q11.c uses to light
 * Caps Lock. We look the keycode up from the keymap itself (instead of
 * keeping a hand-maintained LED->layer table) so this keeps working with
 * zero extra code if M1-M4 ever get bound to a layer too.
 */
static const keypos_t mx_keys[] = {
    {.row = 1, .col = 0}, // M1
    {.row = 2, .col = 0}, // M2
    {.row = 3, .col = 0}, // M3
    {.row = 4, .col = 0}, // M4
    {.row = 5, .col = 0}, // M5
};

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    // Mx keys are only bound on the base layer (transparent elsewhere), and
    // the Mac/Win base layer is picked once by the dip switch, so that's the
    // only layer we need to read the keycode from.
    uint8_t base_layer = get_highest_layer(default_layer_state);

    for (uint8_t i = 0; i < ARRAY_SIZE(mx_keys); i++) {
        uint16_t keycode = keymap_key_to_keycode(base_layer, mx_keys[i]);
        if (IS_QK_TOGGLE_LAYER(keycode)) {
            uint8_t layer = QK_TOGGLE_LAYER_GET_LAYER(keycode);
            if (layer_state_cmp(layer_state, layer)) {
                uint8_t led = g_led_config.matrix_co[mx_keys[i].row][mx_keys[i].col];
                if (led >= led_min && led < led_max && led != NO_LED) {
                    rgb_matrix_set_color(led, RGB_WHITE);
                }
            }
        }
    }

    return true;
}

/*
 * This enum defines the list of all layout layers on the keyboard.
 * The order of layers here determines their index (e.g., MAC_BASE is layer 0, MAC_FN is layer 1, etc.).
 * To temporarily disable Dvorak, we commented out the MAC_DVORAK and WIN_DVORAK lines.
 * If you want to use Dvorak in the future, simply uncomment them here, in the keymap arrays below, and the layer blocks below.
 */
enum layers{
    MAC_BASE,
    MAC_FN,
    MAC_NVG,
    MAC_NVG_SCRLL,
    // MAC_DVORAK, // Uncomment this line to re-enable Dvorak for Mac
    WIN_BASE,
    WIN_FN,
    WIN_NVG,
    WIN_NVG_SCRLL,
    // WIN_DVORAK, // Uncomment this line to re-enable Dvorak for Windows
};

#ifdef DIP_SWITCH_ENABLE
/*
 * This function is automatically called by QMK when the hardware switch on the keyboard (e.g., Mac/Win switch) is toggled.
 *
 * - 'index == 0' refers to the primary Mac/Win slide switch.
 * - 'active' is true when switched to the 'Mac' side, and false when switched to the 'Win' side.
 * - We dynamically change the base active layer using the layer names (MAC_BASE or WIN_BASE)
 *   from our 'layers' enum above. This keeps our layout working even if we add or remove layers later.
 * - Returning 'false' tells QMK to skip the default layer-switching behavior defined in keyboards/keychron/q11/q11.c.
 */
bool dip_switch_update_user(uint8_t index, bool active) {
    if (index == 0) {
        default_layer_set(1UL << (active ? MAC_BASE : WIN_BASE));
    }
    return false;
}
#endif

#define KC_TASK LGUI(KC_TAB)
#define KC_FLXP LGUI(KC_E)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    [MAC_BASE] = LAYOUT_91_ansi(
        KC_MUTE,        KC_ESC,   KC_BRID,  KC_BRIU,  KC_MCTL,  KC_LPAD,  RM_VALD,   RM_VALU,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,  KC_VOLD,    KC_VOLU,  KC_INS,   KC_DEL,   KC_MUTE,
        _______,        KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,      KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,    KC_EQL,   KC_BSPC,            KC_PGUP,
        _______,        KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,      KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,    KC_RBRC,  KC_BSLS,            KC_PGDN,
        _______,        KC_CAPS,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,      KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,              KC_ENT,             KC_HOME,
        /* TG(MAC_DVORAK) - Uncomment the TG key code to toggle Dvorak layer */ _______, KC_LSFT,            KC_Z,     KC_X,     KC_C,     KC_V,      KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,              KC_RSFT,  KC_UP,
        TG(MAC_NVG),    KC_LCTL,  MO(MAC_FN),  KC_LOPT,  KC_LCMD,         KC_SPC,                        KC_SPC,             KC_RCMD,  MO(MAC_FN), KC_RCTL,  KC_LEFT,  KC_DOWN,  KC_RGHT
        ),

    [MAC_FN] = LAYOUT_91_ansi(
        RM_TOGG,  _______,  KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,     KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,     KC_F12,   _______,  _______,  RM_TOGG,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,   _______,  _______,  _______,  _______,  _______,  _______,    _______,  _______,            _______,
        _______,  RM_TOGG,  RM_NEXT,  RM_VALU,  RM_HUEU,  RM_SATU,  RM_SPDU,   _______,  _______,  _______,  _______,  _______,  _______,    _______,  _______,            _______,
        _______,  _______,  RM_PREV,  RM_VALD,  RM_HUED,  RM_SATD,  RM_SPDD,   _______,  _______,  _______,  _______,  _______,  _______,              _______,            _______,
        _______,  _______,            _______,  _______,  _______,  _______,   _______,  NK_TOGG,  _______,  _______,  _______,  _______,              _______,  _______,
        _______,  _______,  _______,  _______,  _______,            _______,                       _______,            _______,  _______,    _______,  KC_HOME,  _______,  KC_END
        ),

    [MAC_NVG] = LAYOUT_91_ansi(
        _______,  _______,          _______,  _______,  _______,  _______,  _______,   _______,  _______,  _______,  _______,  _______,  _______,    _______,  _______,       _______,  _______,
        _______,  _______,          _______,  _______,  _______,  _______,  _______,   _______,  _______,  _______,  _______,  _______,  _______,    _______,  _______,                 _______,
        _______,  _______,  MS_ACL0,  MS_ACL1,  MS_ACL2,  _______,  _______,   _______,              _______,             _______,                 _______,         _______,  _______, _______, _______,          _______,
        _______,  _______,          _______,  _______,  _______,  _______,  _______,   MS_LEFT,  MS_DOWN,    MS_UP,  MS_RGHT,  _______,  _______,              _______,                 _______,
        _______, MO(MAC_NVG_SCRLL), _______,  _______,  _______,  _______,   _______,  _______,  _______,  _______,  _______,  _______,              MO(MAC_NVG_SCRLL),  _______,
        _______,  _______,          _______,  _______,  _______,            _______,                       MS_BTN1,            _______,  _______,    _______,  _______,       _______,  _______
        ),

    /*
     * MAC_DVORAK Layer:
     * To re-enable this layer, remove the block comment start indicator at the beginning
     * and the block comment end indicator at the end of this block.
     */
    /*
    [MAC_DVORAK] = LAYOUT_91_ansi(
        KC_MUTE,  KC_ESC,   KC_BRID,  KC_BRIU,  KC_MCTL,  KC_LPAD,  RM_VALD,   RM_VALU,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,  KC_VOLD,    KC_VOLU,  KC_INS,   KC_DEL,   KC_MUTE,
        _______,  KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,      KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,    KC_EQL,   KC_BSPC,            KC_PGUP,
        _______,  KC_TAB,   KC_QUOT,  KC_COMM,  KC_DOT,   KC_P,     KC_Y,      KC_F,     KC_G,     KC_C,     KC_R,     KC_L,     KC_SLSH,    KC_EQL,   KC_BSLS,            KC_PGDN,
        _______,  KC_CAPS,  KC_A,     KC_O,     KC_E,     KC_U,     KC_I,      KC_D,     KC_H,     KC_T,     KC_N,     KC_S,     KC_MINS,              KC_ENT,             KC_HOME,
        _______,  KC_LSFT,            KC_SCLN,  KC_Q,     KC_J,     KC_K,      KC_X,     KC_B,     KC_M,     KC_W,     KC_V,     KC_Z,                KC_RSFT,  KC_UP,
        _______,  KC_LCTL,  MO(MAC_FN),  KC_LOPT,  KC_LCMD,         KC_SPC,                        KC_SPC,             KC_RCMD,  MO(MAC_FN), KC_RCTL,  KC_LEFT,  KC_DOWN,  KC_RGHT
    ),
    */

    [MAC_NVG_SCRLL] = LAYOUT_91_ansi(
        _______,  _______,  _______,  _______,  _______,  _______,  _______,   _______,              _______,             _______,                _______,              _______,  _______, _______, _______, _______, _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,   _______,              _______,             _______,                _______,              _______,  _______, _______, _______,          _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,   _______,              _______,             _______,                _______,              _______,  _______, _______, _______,          _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,   QK_MOUSE_WHEEL_LEFT,  QK_MOUSE_WHEEL_DOWN, QK_MOUSE_WHEEL_UP,      QK_MOUSE_WHEEL_RIGHT, _______,           _______, _______,          _______,
        _______,  _______,            _______,  _______,  _______,  _______,   _______,              _______,             _______,                _______,              _______,           _______, _______, _______,
        _______,  _______,  _______,  _______,  _______,            MS_BTN3,                         MS_BTN2,                                                           _______,  _______, _______, _______, _______, _______
        ),


    [WIN_BASE] = LAYOUT_91_ansi(
        KC_MUTE,        KC_ESC,   KC_BRID,  KC_BRIU,  KC_TASK,  KC_FLXP,  RM_VALD,   RM_VALU,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,  KC_VOLD,    KC_VOLU,  KC_INS,   KC_DEL,   KC_MUTE,
        _______,        KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,      KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,    KC_EQL,   KC_BSPC,            KC_PGUP,
        _______,        KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,      KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,    KC_RBRC,  KC_BSLS,            KC_PGDN,
        _______,        KC_CAPS,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,      KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,              KC_ENT,             KC_HOME,
        /* TG(WIN_DVORAK) - Uncomment the TG key code to toggle Dvorak layer */ _______, KC_LSFT,            KC_Z,     KC_X,     KC_C,     KC_V,      KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,              KC_RSFT,  KC_UP,
        TG(WIN_NVG),    KC_LCTL,  MO(WIN_FN),  KC_LWIN,  KC_LALT,         KC_SPC,                        KC_SPC,             KC_RALT,  MO(WIN_FN), KC_RCTL,  KC_LEFT,  KC_DOWN,  KC_RGHT
        ),

    [WIN_FN] = LAYOUT_91_ansi(
        RM_TOGG,  _______,  KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,     KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,     KC_F12,   KC_INS,   KC_DEL,   KC_MUTE,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,   _______,  _______,  _______,  _______,  _______,  _______,    _______,  _______,            _______,
        _______,  RM_TOGG,  RM_NEXT,  RM_VALU,  RM_HUEU,  RM_SATU,  RM_SPDU,   _______,  _______,  _______,  _______,  _______,  _______,    _______,  _______,            _______,
        _______,  _______,  RM_PREV,  RM_VALD,  RM_HUED,  RM_SATD,  RM_SPDD,   _______,  _______,  _______,  _______,  _______,  _______,              _______,            _______,
        _______,  _______,            _______,  _______,  _______,  _______,   _______,  NK_TOGG,  _______,  _______,  _______,  _______,              _______,  _______,
        _______,  _______,  _______,  _______,  _______,            _______,                       _______,            _______,  _______,    _______,  KC_HOME,  _______,  KC_END
        ),

    [WIN_NVG] = LAYOUT_91_ansi(
        _______,  _______,          _______,  _______,  _______,  _______,  _______,   _______,  _______,  _______,  _______,  _______,  _______,    _______,  _______,       _______,  _______,
        _______,  _______,          _______,  _______,  _______,  _______,  _______,   _______,  _______,  _______,  _______,  _______,  _______,    _______,  _______,                 _______,
        _______,  _______,  MS_ACL0,  MS_ACL1,  MS_ACL2,  _______,  _______,   _______,              _______,             _______,                 _______,          _______,  _______, _______, _______,          _______,
        _______,  _______,          _______,  _______,  _______,  _______,  _______,   MS_LEFT,  MS_DOWN,    MS_UP,  MS_RGHT,  _______,  _______,              _______,                 _______,
        _______, MO(WIN_NVG_SCRLL), _______,  _______,  _______,  _______,   _______,  _______,  _______,  _______,  _______,  _______,              MO(WIN_NVG_SCRLL),  _______,
        _______,  _______,          _______,  _______,  _______,            MS_BTN3,                       MS_BTN1,            _______,  _______,    _______,  _______,       _______,  _______
        ),

    [WIN_NVG_SCRLL] = LAYOUT_91_ansi(
        _______,  _______,  _______,  _______,  _______,  _______,  _______,   _______,              _______,             _______,                 _______,          _______,  _______, _______, _______, _______, _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,   _______,              _______,             _______,                 _______,          _______,  _______, _______, _______,          _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,   _______,              _______,             _______,                 _______,          _______,  _______, _______, _______,          _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,   QK_MOUSE_WHEEL_LEFT,  QK_MOUSE_WHEEL_DOWN, QK_MOUSE_WHEEL_UP,  QK_MOUSE_WHEEL_RIGHT,  _______,           _______, _______,          _______,
        _______,  _______,            _______,  _______,  _______,  _______,   _______,              _______,             _______,                _______,           _______,           _______, _______, _______,
        _______,  _______,  _______,  _______,  _______,            MS_BTN3,                         MS_BTN2,                                                        _______,  _______, _______, _______, _______, _______
        ),

    /*
     * WIN_DVORAK Layer:
     * To re-enable this layer, remove the block comment start indicator at the beginning
     * and the block comment end indicator at the end of this block.
     */
    /*
    [WIN_DVORAK] = LAYOUT_91_ansi(
        KC_MUTE,      KC_ESC,   KC_BRID,  KC_BRIU,  KC_TASK,  KC_FLXP,  RM_VALD,   RM_VALU,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,  KC_VOLD,    KC_VOLU,  KC_INS,   KC_DEL,   KC_MUTE,
        _______,      KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,      KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,    KC_EQL,   KC_BSPC,            KC_PGUP,
        _______,      KC_TAB,   KC_QUOT,  KC_COMM,  KC_DOT,   KC_P,     KC_Y,      KC_F,     KC_G,     KC_C,     KC_R,     KC_L,     KC_SLSH,    KC_EQL,   KC_BSLS,            KC_PGDN,
        _______,      KC_CAPS,  KC_A,     KC_O,     KC_E,     KC_U,     KC_I,      KC_D,     KC_H,     KC_T,     KC_N,     KC_S,     KC_MINS,              KC_ENT,             KC_HOME,
        _______,      KC_LSFT,            KC_SCLN,  KC_Q,     KC_J,     KC_K,      KC_X,     KC_B,     KC_M,     KC_W,     KC_V,     KC_Z,                KC_RSFT,  KC_UP,
        TG(WIN_NVG),  KC_LCTL,  MO(WIN_FN),  KC_LALT,  KC_LWIN,         KC_SPC,                        KC_SPC,             KC_RALT,  MO(WIN_FN), KC_RCTL,  KC_LEFT,  KC_DOWN,  KC_RGHT
        ),
    */
};

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [MAC_BASE] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [MAC_FN]   = { ENCODER_CCW_CW(RM_VALD, RM_VALU), ENCODER_CCW_CW(RM_VALD, RM_VALU) },
    [MAC_NVG] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [MAC_NVG_SCRLL] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [WIN_BASE] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [WIN_FN]   = { ENCODER_CCW_CW(RM_VALD, RM_VALU), ENCODER_CCW_CW(RM_VALD, RM_VALU) },
    [WIN_NVG] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [WIN_NVG_SCRLL] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
};
#endif // ENCODER_MAP_ENABLE
