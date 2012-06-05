#ifndef __CONFIG_H
#define __CONFIG_H

#define FREERDP_VERSION "1.0"
#define FREERDP_VERSION_FULL "1.0.1"
#define FREERDP_VERSION_MAJOR 1
#define FREERDP_VERSION_MINOR 0
#define FREERDP_VERSION_REVISION 1

#define FREERDP_DATA_PATH "./bin/share/freerdp"
#define FREERDP_PLUGIN_PATH "./bin/lib/freerdp"
#define FREERDP_KEYMAP_PATH "./bin/share/freerdp/keymaps"

/* Include files */
#define HAVE_FCNTL_H
#define HAVE_UNISTD_H
#define HAVE_LIMITS_H
#define HAVE_STDINT_H
#define HAVE_STDBOOL_H
#define HAVE_INTTYPES_H

#define HAVE_TM_GMTOFF

/* Options */
/* #undef WITH_PROFILER */
/* #undef WITH_SSE2 */
/* #undef WITH_NEON */

/* Debug */
/* #undef WITH_DEBUG_CERTIFICATE */
/* #undef WITH_DEBUG_CHANNELS */
/* #undef WITH_DEBUG_CLIPRDR */
/* #undef WITH_DEBUG_DVC */
/* #undef WITH_DEBUG_GDI */
/* #undef WITH_DEBUG_KBD */
/* #undef WITH_DEBUG_LICENSE */
/* #undef WITH_DEBUG_NEGO */
/* #undef WITH_DEBUG_NLA */
/* #undef WITH_DEBUG_NTLM */
/* #undef WITH_DEBUG_ORDERS */
/* #undef WITH_DEBUG_RAIL */
/* #undef WITH_DEBUG_RDP */
/* #undef WITH_DEBUG_REDIR */
/* #undef WITH_DEBUG_RFX */
/* #undef WITH_DEBUG_SCARD */
/* #undef WITH_DEBUG_SVC */
/* #undef WITH_DEBUG_TRANSPORT */
/* #undef WITH_DEBUG_WND */
/* #undef WITH_DEBUG_X11 */
/* #undef WITH_DEBUG_X11_CLIPRDR */
/* #undef WITH_DEBUG_X11_LOCAL_MOVESIZE */
/* #undef WITH_DEBUG_XV */

/* Mobile OS options */
/* #undef WITH_IOS */
/* #undef WITH_IOS_SIM */
#define WITH_ANDROID

#endif
