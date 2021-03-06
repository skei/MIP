#pragma once

#include "../clap.h"

static CLAP_CONSTEXPR const char CLAP_EXT_EVENT_FILTER[] = "clap.event-filter";

#ifdef __cplusplus
extern "C" {
#endif

// This extension lets the host know which event types the plugin is interested
// in.
// The host will cache the set of accepted events before activating the plugin.
// The set of accepted events can't change while the plugin is active.
typedef struct clap_plugin_event_filter {
   // Returns true if the plugin is interested in the given event type.
   // [main-thread]
   bool (*accepts)(const clap_plugin *plugin, clap_event_type event_type);
} clap_plugin_event_filter;


typedef struct clap_host_event_filter {
   // Informs the host that the set of accepted event type changed.
   // This requires the plugin to be deactivated.
   // [main-thread]
   void (*changed)(const clap_host *host);
} clap_host_event_filter;

#ifdef __cplusplus
}
#endif