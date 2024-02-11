/* MIT License

Copyright (c) 2019 Mattia Dal Ben

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include QMK_KEYBOARD_H
#include <stdio.h>

// Each layer gets a name for readability, which is then used in the keymap matrix below.
// The underscores don't mean anything - you can have a layer called STUFF or any other name.
// Layer names don't all need to be of the same length, obviously, and you can also skip them
// entirely and just use numbers.
enum layers {
    _BL,
    _GM,
    _GM13,
    _GM14,
    _FN
};

enum custom_keycodes {
    KC_DBL0 = SAFE_RANGE,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/* Keymap _BL: (Base Layer) Default Layer
 * ,-----------------------.
 * | GM  |CT+GM|ST+GM|-/FN |
 * |-----|-----|-----|-----|
 * |  7  |  8  |  9  |     |
 * |-----|-----|-----|  +  |
 * |  4  |  5  |  6  |     |
 * |-----|-----|-----|-----|
 * |  1  |  2  |  3  |     |
 * |-----|-----|-----| En  |
 * |  0  | 00  |  .  |     |
 * `-----------------------'
 */
  [_BL] = LAYOUT(
    OSL(_GM), OSL(_GM13), OSL(_GM14), LT(_FN, KC_PMNS),
    KC_P7,    KC_P8,    KC_P9,
    KC_P4,    KC_P5,    KC_P6,    KC_PPLS,
    KC_P1,    KC_P2,    KC_P3,
    KC_P0,    KC_DBL0,  KC_PDOT,  KC_PENT
  ),

/* Keymap _GM: Game layer
 * ,-------------------.
 * | BL |----|----|    |
 * |----|----|----|----|
 * |F13 |F14 |F15 |    |
 * |----|----|----|    |
 * |F16 |F17 |F18 |    |
 * |----|----|----|----|
 * |F19 |F20 |F21 |    |
 * |----|----|----|    |
 * |F22 |F23 |F24 |    |
 * `-------------------'
 */
   [_GM] = LAYOUT(
     KC_TRNS, XXXXXXX, XXXXXXX,  XXXXXXX,
     KC_F13,  KC_F14,  KC_F15,
     KC_F16,  KC_F17,  KC_F18,   XXXXXXX,
     KC_F19,  KC_F20,  KC_F21,
     KC_F22,  KC_F23,  KC_F24,   XXXXXXX
   ),
   [_GM13] = LAYOUT(
     KC_TRNS, XXXXXXX, XXXXXXX,  XXXXXXX,
     KC_F13,  KC_F14,  KC_F15,
     KC_F16,  KC_F17,  KC_F18,   XXXXXXX,
     KC_F19,  KC_F20,  KC_F21,
     KC_F22,  KC_F23,  KC_F24,   XXXXXXX
   ),
   [_GM14] = LAYOUT(
     KC_TRNS, XXXXXXX, XXXXXXX,  XXXXXXX,
     KC_F13,  KC_F14,  KC_F15,
     KC_F16,  KC_F17,  KC_F18,   XXXXXXX,
     KC_F19,  KC_F20,  KC_F21,
     KC_F22,  KC_F23,  KC_F24,   XXXXXXX
   ),

/* Keymap _FN: RGB Function Layer
 * ,-------------------.
 * |RMOD|RGBP|RTOG| FN |
 * |----|----|----|----|
 * |HUD |HUI |    |    |
 * |----|----|----|    |
 * |SAD |SAI |    |    |
 * |----|----|----|----|
 * |VAD |VAS |    |    |
 * |----|----|----|    |
 * |RST |    |    |    |
 * `-------------------'
 */
   [_FN] = LAYOUT(
     RGB_MOD,  RGB_M_P,  RGB_TOG,   _______,
     RGB_HUD,  RGB_HUI,  XXXXXXX,
     RGB_SAD,  RGB_SAI,  XXXXXXX,   XXXXXXX,
     RGB_VAD,  RGB_VAI,  XXXXXXX,
     QK_BOOT,  XXXXXXX,  XXXXXXX,   XXXXXXX
   ),
};

void process_GM(uint16_t keycode, uint16_t mod, keyrecord_t *record) {
    if (record->event.pressed) {
        register_code(mod);
        register_code(keycode);
    } else {
        unregister_code(keycode);
        unregister_code(mod);
    }
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (get_highest_layer(layer_state)) {
      case _GM13:
          process_GM(keycode, KC_F13, record);
          return false;
      case _GM14:
          process_GM(keycode, KC_F14, record);
          return false;
      default:
          switch (keycode) {
              case KC_DBL0:
                  if (record->event.pressed) {
                      SEND_STRING("00");
                  } else {
                      // when keycode KC_DBL0 is released
                  }
              break;
          }
  }
  return true;
};

#ifdef OLED_ENABLE

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    return OLED_ROTATION_270;  // flips the display 270 degrees
}

bool oled_task_user(void) {
  // Host Keyboard Layer Status
  oled_write_P(PSTR("Layer"), false);
  switch (get_highest_layer(layer_state)) {
    case _BL:
      oled_write_ln_P(PSTR(" BAS"), false);
      break;
    case _GM:
      oled_write_ln_P(PSTR(" GAM"), false);
      break;
    case _FN:
      oled_write_ln_P(PSTR(" RGB"), false);
      break;
    default:
      // Or use the write_ln shortcut over adding '\n' to the end of your string
      oled_write_ln_P(PSTR(" UND"), false);
  }

  // Host Keyboard LED Status
  led_t led_state = host_keyboard_led_state();
  oled_write_P(PSTR("-----"), false);
  oled_write_P(PSTR("Stats"), false);
  oled_write_P(led_state.num_lock ? PSTR("num:*") : PSTR("num:."), false);
  oled_write_P(led_state.caps_lock ? PSTR("cap:*") : PSTR("cap:."), false);
  oled_write_P(led_state.scroll_lock ? PSTR("scr:*") : PSTR("scr:."), false);

  // Host Keyboard RGB backlight status

    switch (get_highest_layer(layer_state)) {
        case _FN: {
            oled_write_P(PSTR("-----"), false);
            oled_write_P(PSTR("Light"), false);
            static char led_buf[30];
            snprintf(led_buf, sizeof(led_buf) - 1, "RGB:%cM: %2d\nh: %2ds: %2dv: %2d\n",
                rgblight_is_enabled() ? '*' : '.', (uint8_t)rgblight_get_mode(),
                (uint8_t)(rgblight_get_hue() / RGBLIGHT_HUE_STEP),
                (uint8_t)(rgblight_get_sat() / RGBLIGHT_SAT_STEP),
                (uint8_t)(rgblight_get_val() / RGBLIGHT_VAL_STEP));
            oled_write(led_buf, false);
            break;
        }
        default:
      oled_write_ln_P(PSTR(""), false);
      oled_write_ln_P(PSTR(""), false);
      oled_write_ln_P(PSTR(""), false);
      oled_write_ln_P(PSTR(""), false);
      oled_write_ln_P(PSTR(""), false);
      oled_write_ln_P(PSTR(""), false);
      oled_write_ln_P(PSTR(""), false);
      oled_write_ln_P(PSTR(""), false);
            break;
    }

    return false;
}
#endif