/* SPDX-License-Identifier: GPL-2.0-or-later */
#pragma once

#define SVAL_DUALUSB

#undef SPLIT_POINTING_ENABLE
#undef POINTING_DEVICE_COMBINED
#undef SPLIT_WATCHDOG_ENABLE
#undef OS_DETECTION_KEYBOARD_RESET

#undef SPLIT_TRANSACTION_IDS_KB
#define SPLIT_TRANSACTION_IDS_KB KEYBOARD_SYNC_A, KEYBOARD_SYNC_B, SVAL_POINTER_SYNC

#if defined(INIT_EE_HANDS_LEFT) && defined(INIT_EE_HANDS_RIGHT)
#    error "dualusb: INIT_EE_HANDS_LEFT and INIT_EE_HANDS_RIGHT are both defined. Each firmware image is exactly one hand; the master/slave role is selected from these macros."
#endif

#if !defined(INIT_EE_HANDS_LEFT) && !defined(INIT_EE_HANDS_RIGHT)
#    error "dualusb: neither INIT_EE_HANDS_LEFT nor INIT_EE_HANDS_RIGHT is defined. Build svalboard/<variant>/left:dualusb and svalboard/<variant>/right:dualusb, not the handless parent target."
#endif

#ifdef SPLIT_POINTING_ENABLE
#    error "dualusb: SPLIT_POINTING_ENABLE is still defined after the #undef above. It registers GET_POINTING_DATA, which puts report_mouse_t on the split wire; its size varies with build flags (MOUSE_SHARED_EP, hires scroll), so the handshake succeeds while pointer data is silently corrupt. This bricked both halves once already."
#endif

#ifdef SPLIT_TRANSACTION_IDS_USER
#    error "dualusb: SPLIT_TRANSACTION_IDS_USER is defined. User ids are appended to the same enum and shift NUM_TOTAL_TRANSACTIONS, which is XORed into the split serial handshake: every transaction fails including matrix sync, with no diagnostic. Put new ids in SPLIT_TRANSACTION_IDS_KB above and update the _Static_assert in keymap.c."
#endif

#ifndef COMMUNITY_MODULES_ENABLE
#    error "dualusb: COMMUNITY_MODULES_ENABLE is not defined. svalboard/core is deliberately kept on BOTH halves; without it EECONFIG_KB_DATA_SIZE drops to 0, which moves the VIA magic address and invalidates stored settings, and svalboard.c/identity.c stop compiling."
#endif

#ifdef OS_DETECTION_KEYBOARD_RESET
#    error "dualusb: OS_DETECTION_KEYBOARD_RESET is still defined after the #undef above. Both halves run their own OS detection over their own USB; a reset triggered on one half only drops that half's split serial while the other keeps scanning, so the pair desynchronises with no diagnostic."
#endif

#ifndef MOUSE_SHARED_EP
#    error "dualusb: MOUSE_SHARED_EP is not defined. The board sets it yes for both halves and it must stay symmetric: an asymmetric setting changes sizeof(report_mouse_t) on one half only and silently corrupts pointer data. Cross-half comparison is impossible at compile time, so both halves assert the same expected state here."
#endif

#if !defined(PRODUCT) || !defined(PRODUCT_ID)
#    error "dualusb: PRODUCT / PRODUCT_ID are not defined by the generated info_config.h. The per-hand overrides below exist because merge_info_jsons applies the least specific file last, so keyboards/svalboard/info.json beats trackball/pmw3389/{left,right}/keyboard.json and BOTH halves ship as \"lightly\" / 0x4044. Keybard never auto-binds and labels each permitted device by its USB product string, so two identically named halves make the device chooser a coin flip - which the whole dual-USB architecture assumed away."
#endif

#undef PRODUCT
#undef PRODUCT_ID

#ifdef INIT_EE_HANDS_LEFT
#    define PRODUCT "Svalboard Trackball PMW3389 Left"
#    define PRODUCT_ID 0x4048
#else
#    define PRODUCT "Svalboard Trackball PMW3389 Right"
#    define PRODUCT_ID 0x4049
#endif
