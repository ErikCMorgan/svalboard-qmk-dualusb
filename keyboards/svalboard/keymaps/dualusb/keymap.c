/*
Copyright 2023 Morgan Venable @_claussen

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "../keymap_support.c"
#include "keycodes.h"
#include "quantum_keycodes.h"
#include QMK_KEYBOARD_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "svalboard.h"
#include "transactions.h"
#include "transport.h"
#include "split_util.h"
#include "pointing_device.h"

layer_state_t default_layer_state_set_user(layer_state_t state) {
  sval_set_active_layer(0, false);
  return state;
}

layer_state_t layer_state_set_user(layer_state_t state) {
  sval_set_active_layer(get_highest_layer(state), false);
  return state;
}

enum layer {
    NORMAL,
    NAVNAS,
    FUNC,
    BOARD_CONFIG = MH_AUTO_BUTTONS_LAYER - 1,
    MBO = MH_AUTO_BUTTONS_LAYER,
};

#if __has_include("keymap_all.h")
#include "keymap_all.h"
#else
int sval_macro_size = 0;
uint8_t sval_macros[] = {0};
const uint16_t PROGMEM keymaps[DYNAMIC_KEYMAP_LAYER_COUNT][MATRIX_ROWS][MATRIX_COLS] = {
    /* ===== NORMAL ===== .*/
    [NORMAL] = LAYOUT(
        /*     Center            North               East                South             West                Double*/
        /*R1*/ KC_J              , KC_U              , KC_QUOTE          , KC_M            , KC_H              , KC_NO           ,
        /*R2*/ KC_K              , KC_I              , KC_COLON          , KC_COMMA        , KC_Y              , KC_NO             ,
        /*R3*/ KC_L              , KC_O              , LT(BOARD_CONFIG, KC_NO)  , KC_DOT   , KC_N              , KC_NO           ,
        /*R4*/ KC_SEMICOLON      , KC_P              , KC_BSLS           , KC_SLASH        , KC_RBRC           , KC_NO          ,
        /*L1*/ KC_F              , KC_R              , KC_G              , KC_V            , LSFT(KC_QUOTE)    , KC_NO            ,
        /*L2*/ KC_D              , KC_E              , KC_T              , KC_C            , KC_GRAVE          , KC_NO         ,
        /*L3*/ KC_S              , KC_W              , KC_B              , KC_X            , KC_ESCAPE         , KC_NO           ,
        /*L4*/ KC_A              , KC_Q              , KC_LBRC           , KC_Z            , KC_DELETE         , KC_NO           ,

        /*     Down                 Pad                Up                  Nail            Knuckle             DoubleDown*/
        /*RT*/ MO(NAVNAS)          , KC_SPACE        , KC_NO             , KC_BSPC         , KC_LALT           , MO(FUNC)             ,
        /*LT*/ KC_LSFT       , LT(NAVNAS, KC_ENTER)  , KC_NO             , LGUI_T(KC_TAB)  , KC_LCTL           , KC_CAPS
        ),

    [NAVNAS] = LAYOUT(
        /*     Center            North               East               South             West              Double*/
        /*R1*/ KC_7              , LSFT(KC_7)        , LSFT(KC_6)       , KC_LEFT         , KC_6            , KC_NO           ,
        /*R2*/ KC_8              , LSFT(KC_8)        , KC_NO            , KC_UP           , LSFT(KC_MINUS)  , KC_NO           ,
        /*R3*/ KC_9              , LSFT(KC_9)        , KC_NO            , KC_DOWN         , KC_INSERT       , KC_NO           ,
        /*R4*/ KC_0              , LSFT(KC_0)        , KC_NO            , KC_RIGHT        , LSFT(KC_GRAVE)  , KC_NO           ,
        /*L1*/ KC_4              , LSFT(KC_4)        , KC_5             , KC_END          , LSFT(KC_5)      , KC_NO           ,
        /*L2*/ KC_3              , LSFT(KC_3)        , KC_MINUS         , KC_PGDN       , LSFT(KC_EQUAL)  , KC_NO           ,
        /*L3*/ KC_2              , LSFT(KC_2)        , KC_DOT           , KC_PGUP         , KC_TRNS         , KC_NO           ,
        /*L4*/ KC_1              , LSFT(KC_1)        , KC_EQUAL         , KC_HOME         , KC_TRNS         , KC_NO           ,
        /*Down             Pad             Up              Nail            Knuckle         DoubleDown*/
        /*RT*/ KC_TRNS           , KC_TRNS           , KC_TRNS          , KC_TRNS         , KC_TRNS         , KC_TRNS         ,
        /*LT*/ KC_TRNS           , KC_TRNS           , KC_TRNS          , KC_TRNS         , KC_TRNS         , KC_TRNS
        ),

    /* ===== FUNCTION KEYS ===== */
    [FUNC] = LAYOUT(
        /*Center           North           East            South           West            Double*/
        /*R1*/ KC_F7            , KC_NO             , KC_F16           , KC_F17         , KC_F6           , KC_NO           ,
        /*R2*/ KC_F8            , KC_NO             , KC_NO            , KC_F18         , KC_NO           , KC_NO           ,
        /*R3*/ KC_F9            , KC_NO             , KC_NO            , KC_F19         , KC_NO           , KC_NO           ,
        /*R4*/ KC_F10           , KC_NO             , KC_NO            , KC_F20         , KC_NO           , KC_NO           ,
        /*L1*/ KC_F4            , KC_F24            , KC_F5            , KC_F14         , KC_F15          , KC_NO           ,
        /*L2*/ KC_F3            , KC_F23            , KC_F10           , KC_F13         , KC_NO           , KC_NO           ,
        /*L3*/ KC_F2            , KC_F22            , KC_NO            , KC_F12         , KC_NO           , KC_NO           ,
        /*L4*/ KC_F1            , KC_F21            , KC_NO            , KC_F11         , KC_NO           , KC_NO           ,
        /*Down             Pad             Up              Nail            Knuckle         DoubleDown*/
        /*RT*/ KC_TRNS          , KC_TRNS           , KC_TRNS          , KC_TRNS        , KC_TRNS         , KC_TRNS         ,
        /*LT*/ KC_TRNS          , KC_TRNS           , KC_TRNS          , KC_TRNS        , KC_TRNS         , KC_TRNS
        ),

    [BOARD_CONFIG] = LAYOUT(
        /*         Center              North               East                South               West                (XXX)               */
        /*R1*/     KC_TRNS,            KC_TRNS,            KC_TRNS,            KC_TRNS,            KC_TRNS,            KC_NO,
        /*R2*/     KC_TRNS,            KC_TRNS,            KC_TRNS,            KC_TRNS,            KC_TRNS,            KC_NO,
        /*R3*/     KC_TRNS,            KC_TRNS,            KC_TRNS,            KC_TRNS,            KC_TRNS,            KC_NO,
        /*R4*/     KC_TRNS,            KC_TRNS,            KC_TRNS,            KC_TRNS,            KC_TRNS,            KC_NO,
        /*L1*/     SV_OUTPUT_STATUS,   KC_TRNS,            KC_TRNS,            KC_TRNS,            KC_TRNS,            KC_NO,
        /*L2*/     KC_TRNS,            SV_RIGHT_DPI_INC,   KC_TRNS,            SV_RIGHT_DPI_DEC,   KC_TRNS,            KC_NO,
        /*L3*/     KC_TRNS,            SV_LEFT_DPI_INC,    KC_TRNS,            SV_LEFT_DPI_DEC,    KC_TRNS,            KC_NO,
        /*L4*/     KC_TRNS,            SV_FLYWHEEL,        KC_TRNS,            KC_TRNS,            KC_TRNS,            KC_NO,

        /*        Down                Pad                 Up                  Nail                Knuckle             Double Down         */
        /* RT */  KC_TRNS,            KC_TRNS,            KC_TRNS,            KC_TRNS,            KC_TRNS,            KC_TRNS,
        /* LT */  KC_TRNS,            KC_TRNS,            KC_TRNS,            KC_TRNS,            KC_TRNS,            KC_TRNS
      ),

    /* ===== MBO ===== */
    [MBO] = LAYOUT(
        /*      Center           North               East                South                West                Double*/
        /*R1*/ KC_TRNS           , KC_TRNS           , KC_TRNS           , KC_TRNS           , KC_TRNS           , KC_NO             ,
        /*R2*/ KC_TRNS           , KC_TRNS           , KC_TRNS           , KC_TRNS           , KC_TRNS           , KC_NO             ,
        /*R3*/ KC_TRNS           , KC_TRNS           , KC_TRNS           , KC_TRNS           , KC_TRNS           , KC_NO             ,
        /*R4*/ KC_TRNS           , KC_TRNS           , KC_TRNS           , KC_TRNS           , KC_TRNS           , KC_NO             ,
        /*L1*/ MS_BTN1           , KC_TRNS           , KC_TRNS           , KC_TRNS           , KC_TRNS           , KC_NO             ,
        /*L2*/ MS_BTN3           , KC_TRNS           , KC_TRNS           , KC_TRNS           , KC_TRNS           , KC_NO             ,
        /*L3*/ MS_BTN2           , KC_TRNS           , KC_TRNS           , KC_TRNS           , KC_TRNS           , KC_NO             ,
        /*L4*/ KC_TRNS           , SV_FLYWHEEL       , KC_TRNS           , SV_SNIPER_3       , KC_TRNS           , KC_NO             ,

        /*     Down               Pad                Up                  Nail                Knuckle             DoubleDown */
        /*RT*/ KC_TRNS           , KC_TRNS           , KC_TRNS           , KC_TRNS           , KC_TRNS           , KC_TRNS           ,
        /*LT*/ KC_TRNS           , MS_BTN1           , KC_TRNS           , MS_BTN2           , KC_TRNS           , KC_TRNS
        ),
};
#endif

void sval_pointer_sync_handler(uint8_t in_len, const void *in_data, uint8_t out_len, void *out_data);

void keyboard_post_init_user(void) {
  transaction_register_rpc(SVAL_POINTER_SYNC, sval_pointer_sync_handler);
#if __has_include("keymap_all.h")
  if (fresh_install) {
    sval_init_defaults();
  }
#endif
}

bool is_keyboard_master_impl(void);
void last_pointing_device_activity_trigger(void);

bool is_keyboard_master_impl(void) {
#ifdef INIT_EE_HANDS_LEFT
    return true;
#else
    return false;
#endif
}

_Static_assert(KEYBOARD_SYNC_A == 8, "KEYBOARD_SYNC_A != 8: split transaction enum drifted; NUM_TOTAL_TRANSACTIONS is XORed into the serial handshake, so a shift on one half kills every transaction including matrix sync with no diagnostic");
_Static_assert(KEYBOARD_SYNC_B == 9, "KEYBOARD_SYNC_B != 9: split transaction enum drifted; NUM_TOTAL_TRANSACTIONS is XORed into the serial handshake, so a shift on one half kills every transaction including matrix sync with no diagnostic");
_Static_assert(SVAL_POINTER_SYNC == 10, "SVAL_POINTER_SYNC != 10: it must be APPENDED last in SPLIT_TRANSACTION_IDS_KB, after KEYBOARD_SYNC_B, or the Scan Lab RPC ids shift");
_Static_assert(NUM_TOTAL_TRANSACTIONS == 11, "NUM_TOTAL_TRANSACTIONS != 11: a split feature flag changed. This value is XORed into the split serial handshake; if the two halves disagree, every transaction fails including matrix sync, with no diagnostic. Re-measure with svalboard-port/measure_enum.sh before changing this number");

_Static_assert((uint16_t)SV_FLYWHEEL <= (uint16_t)QK_USER_31, "SV_FLYWHEEL past QK_USER_31: the QK_USER keycode range holds 32 slots and only those are addressable by the configurator. Overflowing compiles fine and silently makes the keycode unassignable in Keybard/VIA. SV_FLYWHEEL is slot 30 of 32; retire a keycode before adding another");

#define SVAL_F_LEFT_SCROLL    (1UL << 0)
#define SVAL_F_RIGHT_SCROLL   (1UL << 1)
#define SVAL_F_AXIS_LOCK      (1UL << 2)
#define SVAL_F_NATURAL_SCROLL (1UL << 3)
#define SVAL_F_LEFT_AUTO      (1UL << 4)
#define SVAL_F_RIGHT_AUTO     (1UL << 5)
#define SVAL_F_AUTO_MOUSE     (1UL << 6)
#define SVAL_F_IS_MAC         (1UL << 7)
#define SVAL_F_SCROLL_HOLD    (1UL << 8)
#define SVAL_F_SCROLL_TOGGLE  (1UL << 9)
#define SVAL_F_SNIPER_TG2     (1UL << 10)
#define SVAL_F_SNIPER_TG3     (1UL << 11)
#define SVAL_F_SNIPER_TG5     (1UL << 12)
#define SVAL_F_BOOST_TG2      (1UL << 13)
#define SVAL_F_BOOST_TG3      (1UL << 14)
#define SVAL_F_BOOST_TG5      (1UL << 15)
#define SVAL_F_FLYWHEEL       (1UL << 16)
#define SVAL_F_MOUSE_MODE     (1UL << 17)

#define SVAL_R_MOUSE_MODE (1U << 0)
#define SVAL_R_MOTION     (1U << 1)

#define SVAL_SYNC_STALE_MS 100

typedef struct {
    int16_t  x;
    int16_t  y;
    int16_t  h;
    int16_t  v;
    uint32_t flags;
    uint16_t automouse_threshold;
    uint8_t  left_dpi_index;
    uint8_t  right_dpi_index;
    uint8_t  automouse_decay;
    uint8_t  buttons;
    uint8_t  sniper_hold_2;
    uint8_t  sniper_hold_3;
    uint8_t  sniper_hold_5;
    uint8_t  boost_hold_2;
    uint8_t  boost_hold_3;
    uint8_t  boost_hold_5;
} sval_sync_t;

_Static_assert(sizeof(sval_sync_t) == 24, "sizeof(sval_sync_t) != 24: the master memcpys this struct onto the split wire and the slave casts the buffer back, so a layout change on one half only is silently misinterpreted field by field");
_Static_assert(sizeof(sval_sync_t) <= RPC_M2S_BUFFER_SIZE, "sval_sync_t exceeds RPC_M2S_BUFFER_SIZE: transaction_rpc_exec would truncate the master-to-slave payload");
_Static_assert(RPC_S2M_BUFFER_SIZE == 32 && RPC_M2S_BUFFER_SIZE == 32, "RPC buffer sizes are not both 32: transaction_rpc_exec truncates any payload longer than the buffer, and each half sizes its copy of split_shared_memory_t from these macros independently, so a change on one half only silently shifts every field that follows them");

_Static_assert(offsetof(split_shared_memory_t, rpc_m2s_buffer) == 36, "offsetof(split_shared_memory_t, rpc_m2s_buffer) != 36: transaction_register_rpc() bakes this offset into the transaction descriptor, and NUM_TOTAL_TRANSACTIONS does NOT change when the offset shifts, so the serial handshake still succeeds and RPC payloads land on top of a neighbouring field. RGBLIGHT_LAYERS, RGBLIGHT_MAX_LAYERS, MATRIX_ROWS, MATRIX_COLS, the RPC buffer sizes and every SPLIT_*_ENABLE flag all move it. Re-measure with the response-file probe described in svalboard-port/ROADMAP.md before changing this number");
_Static_assert(offsetof(split_shared_memory_t, rpc_s2m_buffer) == 68, "offsetof(split_shared_memory_t, rpc_s2m_buffer) != 68: same silent failure mode as rpc_m2s_buffer, on the slave-to-master reply path that carries want_mouse_mode");
_Static_assert(sizeof(split_shared_memory_t) == 104, "sizeof(split_shared_memory_t) != 104: the halves must agree on the entire shared-memory layout, not only on NUM_TOTAL_TRANSACTIONS. A size change means a field was added, removed or resized, and the serial handshake cannot detect it");

_Static_assert(sizeof(report_mouse_t) == 12, "sizeof(report_mouse_t) != 12: this pins MOUSE_SHARED_EP (report_id byte), MOUSE_EXTENDED_REPORT (boot_x/boot_y plus int16 x/y) and WHEEL_EXTENDED_REPORT (int16 h/v) ON for both halves. An asymmetric setting changes what a mouse report is on one half only and silently corrupts pointer data; MOUSE_EXTENDED_REPORT and WHEEL_EXTENDED_REPORT also set the MOUSE_REPORT_XY/HV bounds that sval_clamp16 hard-codes");
_Static_assert(MOUSE_REPORT_XY_MIN == INT16_MIN + 1 && MOUSE_REPORT_XY_MAX == INT16_MAX, "MOUSE_REPORT_XY range is not [INT16_MIN+1, INT16_MAX]: sval_clamp16 hard-codes those bounds, so a narrower report type would let the accumulators emit values the HID report cannot carry");
_Static_assert(MOUSE_REPORT_HV_MIN == INT16_MIN + 1 && MOUSE_REPORT_HV_MAX == INT16_MAX, "MOUSE_REPORT_HV range is not [INT16_MIN+1, INT16_MAX]: sval_clamp16 hard-codes those bounds, so a narrower report type would let the accumulators emit values the HID report cannot carry");

static int16_t sval_clamp16(int32_t v) {
    if (v > MOUSE_REPORT_XY_MAX) return MOUSE_REPORT_XY_MAX;
    if (v < MOUSE_REPORT_XY_MIN) return MOUSE_REPORT_XY_MIN;
    return (int16_t)v;
}

pointing_device_status_t pointing_device_get_status(void) {
    return POINTING_DEVICE_STATUS_SUCCESS;
}

#ifdef INIT_EE_HANDS_LEFT

static sval_sync_t sync_out;

report_mouse_t pointing_device_task_user(report_mouse_t reportMouse) {
    sync_out.x = sval_clamp16((int32_t)sync_out.x + reportMouse.x);
    sync_out.y = sval_clamp16((int32_t)sync_out.y + reportMouse.y);
    sync_out.h = sval_clamp16((int32_t)sync_out.h + reportMouse.h);
    sync_out.v = sval_clamp16((int32_t)sync_out.v + reportMouse.v);

    sync_out.flags = 0;
    if (global_saved_values.left_scroll) sync_out.flags |= SVAL_F_LEFT_SCROLL;
    if (global_saved_values.right_scroll) sync_out.flags |= SVAL_F_RIGHT_SCROLL;
    if (global_saved_values.axis_scroll_lock) sync_out.flags |= SVAL_F_AXIS_LOCK;
    if (global_saved_values.natural_scroll) sync_out.flags |= SVAL_F_NATURAL_SCROLL;
    if (global_saved_values.left_automouse) sync_out.flags |= SVAL_F_LEFT_AUTO;
    if (global_saved_values.right_automouse) sync_out.flags |= SVAL_F_RIGHT_AUTO;
    if (global_saved_values.auto_mouse) sync_out.flags |= SVAL_F_AUTO_MOUSE;
    if (is_mac) sync_out.flags |= SVAL_F_IS_MAC;
    if (scroll_hold) sync_out.flags |= SVAL_F_SCROLL_HOLD;
    if (scroll_toggle) sync_out.flags |= SVAL_F_SCROLL_TOGGLE;
    if (sniper_toggle_2) sync_out.flags |= SVAL_F_SNIPER_TG2;
    if (sniper_toggle_3) sync_out.flags |= SVAL_F_SNIPER_TG3;
    if (sniper_toggle_5) sync_out.flags |= SVAL_F_SNIPER_TG5;
    if (boost_toggle_2) sync_out.flags |= SVAL_F_BOOST_TG2;
    if (boost_toggle_3) sync_out.flags |= SVAL_F_BOOST_TG3;
    if (boost_toggle_5) sync_out.flags |= SVAL_F_BOOST_TG5;
    if (enable_flywheel) sync_out.flags |= SVAL_F_FLYWHEEL;
    if (mouse_mode_enabled) sync_out.flags |= SVAL_F_MOUSE_MODE;

    sync_out.automouse_threshold = global_saved_values.automouse_threshold;
    sync_out.automouse_decay     = global_saved_values.automouse_decay;
    sync_out.left_dpi_index      = global_saved_values.left_dpi_index;
    sync_out.right_dpi_index     = global_saved_values.right_dpi_index;
    sync_out.sniper_hold_2       = sniper_hold_2;
    sync_out.sniper_hold_3       = sniper_hold_3;
    sync_out.sniper_hold_5       = sniper_hold_5;
    sync_out.boost_hold_2        = boost_hold_2;
    sync_out.boost_hold_3        = boost_hold_3;
    sync_out.boost_hold_5        = boost_hold_5;

    if (reportMouse.x || reportMouse.y) last_pointing_device_activity_trigger();

    if (!is_transport_connected()) {
        report_mouse_t absent = {0};
        sync_out.x = sync_out.y = sync_out.h = sync_out.v = 0;
        return sval_pointer_pipeline(reportMouse, absent);
    }

    memset(&reportMouse, 0, sizeof(reportMouse));
    return reportMouse;
}

void sval_pointer_sync_handler(uint8_t in_len, const void *in_data, uint8_t out_len, void *out_data) {
    (void)in_len;
    (void)in_data;
    (void)out_len;
    (void)out_data;
}

void housekeeping_task_user(void) {
    if (!is_transport_connected()) return;
#ifdef MOUSEKEY_ENABLE
    sync_out.buttons = mousekey_get_report().buttons;
#endif
    sval_sync_t push  = sync_out;
    uint8_t     reply = 0;
    sync_out.x = sync_out.y = sync_out.h = sync_out.v = 0;
    if (transaction_rpc_exec(SVAL_POINTER_SYNC, sizeof(push), &push,
                             sizeof(reply), &reply)) {
        if (reply & SVAL_R_MOUSE_MODE) mouse_mode(true);
        if (reply) last_pointing_device_activity_trigger();
    }
}

#else /* slave */

static sval_sync_t sync_in;
static uint32_t    last_sync_ms;
static uint8_t     reply_flags;
static bool        master_mouse_mode;

void sval_pointer_sync_handler(uint8_t in_len, const void *in_data, uint8_t out_len, void *out_data) {
    if (in_len == sizeof(sval_sync_t)) {
        const sval_sync_t *n = (const sval_sync_t *)in_data;

        sync_in.x = sval_clamp16((int32_t)sync_in.x + n->x);
        sync_in.y = sval_clamp16((int32_t)sync_in.y + n->y);
        sync_in.h = sval_clamp16((int32_t)sync_in.h + n->h);
        sync_in.v = sval_clamp16((int32_t)sync_in.v + n->v);

        sync_in.flags               = n->flags;
        sync_in.automouse_threshold = n->automouse_threshold;
        sync_in.automouse_decay     = n->automouse_decay;
        sync_in.left_dpi_index      = n->left_dpi_index;
        sync_in.right_dpi_index     = n->right_dpi_index;
        sync_in.buttons             = n->buttons;
        sync_in.sniper_hold_2       = n->sniper_hold_2;
        sync_in.sniper_hold_3       = n->sniper_hold_3;
        sync_in.sniper_hold_5       = n->sniper_hold_5;
        sync_in.boost_hold_2        = n->boost_hold_2;
        sync_in.boost_hold_3        = n->boost_hold_3;
        sync_in.boost_hold_5        = n->boost_hold_5;

        last_sync_ms = timer_read32();
    }
    if (out_data && out_len >= sizeof(uint8_t)) {
        *(uint8_t *)out_data = reply_flags;
        reply_flags          = 0;
    }
}

static void sval_apply_policy(const sval_sync_t *in) {
    static uint8_t  applied_scale[7]   = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    static uint8_t  applied_right_dpi  = 0xFF;
    uint8_t         scale[7];

    uint8_t left_dpi_index  = (in->left_dpi_index  < DPI_CHOICES_LENGTH) ? in->left_dpi_index  : 0;
    uint8_t right_dpi_index = (in->right_dpi_index < DPI_CHOICES_LENGTH) ? in->right_dpi_index : 0;

    global_saved_values.left_scroll         = !!(in->flags & SVAL_F_LEFT_SCROLL);
    global_saved_values.right_scroll        = !!(in->flags & SVAL_F_RIGHT_SCROLL);
    global_saved_values.axis_scroll_lock    = !!(in->flags & SVAL_F_AXIS_LOCK);
    global_saved_values.natural_scroll      = !!(in->flags & SVAL_F_NATURAL_SCROLL);
    global_saved_values.left_automouse      = !!(in->flags & SVAL_F_LEFT_AUTO);
    global_saved_values.right_automouse     = !!(in->flags & SVAL_F_RIGHT_AUTO);
    global_saved_values.auto_mouse          = !!(in->flags & SVAL_F_AUTO_MOUSE);
    global_saved_values.automouse_threshold = in->automouse_threshold;
    global_saved_values.automouse_decay     = in->automouse_decay;
    global_saved_values.left_dpi_index      = left_dpi_index;
    global_saved_values.right_dpi_index     = right_dpi_index;

    is_mac = !!(in->flags & SVAL_F_IS_MAC);

    scroll_hold   = !!(in->flags & SVAL_F_SCROLL_HOLD);
    scroll_toggle = !!(in->flags & SVAL_F_SCROLL_TOGGLE);

    if (right_dpi_index != applied_right_dpi) {
        applied_right_dpi = right_dpi_index;
        set_right_dpi(right_dpi_index);
    }

    uint8_t scroll_div = is_mac ? MAC_DIVISOR : SCROLL_DIVISOR;
    if (l_x.div != scroll_div || l_y.div != scroll_div || r_x.div != scroll_div || r_y.div != scroll_div) {
        set_div_axis(&l_x, scroll_div);
        set_div_axis(&l_y, scroll_div);
        set_div_axis(&r_x, scroll_div);
        set_div_axis(&r_y, scroll_div);
    }

    scale[0] = in->sniper_hold_2;
    scale[1] = in->sniper_hold_3;
    scale[2] = in->sniper_hold_5;
    scale[3] = in->boost_hold_2;
    scale[4] = in->boost_hold_3;
    scale[5] = in->boost_hold_5;
    scale[6] = (uint8_t)((in->flags >> 10) & 0x3F);

    if (memcmp(scale, applied_scale, sizeof(scale)) != 0) {
        memcpy(applied_scale, scale, sizeof(scale));
        sniper_hold_2   = scale[0];
        sniper_hold_3   = scale[1];
        sniper_hold_5   = scale[2];
        boost_hold_2    = scale[3];
        boost_hold_3    = scale[4];
        boost_hold_5    = scale[5];
        sniper_toggle_2 = !!(scale[6] & 0x01);
        sniper_toggle_3 = !!(scale[6] & 0x02);
        sniper_toggle_5 = !!(scale[6] & 0x04);
        boost_toggle_2  = !!(scale[6] & 0x08);
        boost_toggle_3  = !!(scale[6] & 0x10);
        boost_toggle_5  = !!(scale[6] & 0x20);
        update_sniper_divisor();
        update_boost_multiplier();
    }
}

report_mouse_t pointing_device_task_user(report_mouse_t reportMouse) {
    sval_sync_t    in;
    bool           stale;
    report_mouse_t master     = {0};
    bool           own_motion = (reportMouse.x != 0 || reportMouse.y != 0);

    chSysLock();
    memcpy(&in, &sync_in, sizeof(in));
    sync_in.x = sync_in.y = sync_in.h = sync_in.v = 0;
    stale = (last_sync_ms == 0) || (timer_elapsed32(last_sync_ms) > SVAL_SYNC_STALE_MS);
    if (stale) memset(&sync_in, 0, sizeof(sync_in));
    chSysUnlock();

    if (stale) {
        memset(&in, 0, sizeof(in));
    } else {
        sval_apply_policy(&in);
    }

    enable_flywheel = !!(in.flags & SVAL_F_FLYWHEEL);

    bool now_mouse_mode = !!(in.flags & SVAL_F_MOUSE_MODE);
    if (master_mouse_mode && !now_mouse_mode) sval_automouse_reset();
    master_mouse_mode = now_mouse_mode;

    master.x = in.x;
    master.y = in.y;
    master.h = in.h;
    master.v = in.v;

    reportMouse = sval_pointer_pipeline(master, reportMouse);
    reportMouse.buttons = in.buttons;

    uint8_t reply = own_motion ? SVAL_R_MOTION : 0;
    if (sval_automouse_request) {
        sval_automouse_request = false;
        reply |= SVAL_R_MOUSE_MODE;
    }
    if (reply) {
        chSysLock();
        reply_flags |= reply;
        chSysUnlock();
    }

    return reportMouse;
}

void housekeeping_task_user(void) {}

#endif
