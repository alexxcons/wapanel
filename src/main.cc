#include <gio/gio.h>
#include <gtk/gtk.h>
#include <stdio.h>
#include <vector>

#include "applets.hh"
#include "config.hh"
#include "log.hh"
#include "panel.hh"
#include "utils.hh"

// Basic info
std::vector<wapanel::panel *> panels;
GtkApplication *app;
GFileMonitor *config_monitor;
// Basic info

#ifndef VERSION
#	define VERSION "0.0.0~ERROR"
#endif

#define __inter_verstr(verstr)	#verstr
#define VERSION_STRING(version) __inter_verstr(version)

auto static app_activate(GtkApplication *_app) -> void;
auto static config_changed(GFileMonitor *monitor, GFile *file, GFile *other_file, GFileMonitorEvent event_type,
						   gpointer user_data) -> void {
	if (event_type != G_FILE_MONITOR_EVENT_CHANGES_DONE_HINT) { return; }

	log_info("====== Config file update ======");

	// Exit if error is found.
	if (wapanel::conf::read_config()) {
		wapanel::spawn_gtk_error("Found some error while trying to read config file. Exiting.");
		exit(1);
	}

	wapanel::applets::remove_existing_instances();

	// Check if any panels is removed or added to the config.
	if (wapanel::conf::config.panels_conf.size() != panels.size()) {
		// Remove existing panels.
		for (auto &&panel : panels) {
			gtk_application_remove_window(app, panel->get_gtk_window());
			delete panel;
		}

		panels.clear();

		// Init all again panels.
		app_activate(app);
		return;
	}

	// Reconfigure panels.
	for (auto &&panel : panels) {
		panel->configure();
	}
}

auto static app_startup(GtkApplication *_app) -> void {
	// Exit if error is found.
	if (wapanel::conf::read_config()) {
		wapanel::spawn_gtk_error("Found some error while trying to read config file. Exiting.");
		exit(1);
	}

	// Monitor for changes in config.
	config_monitor = g_file_monitor_file(g_file_new_for_path(wapanel::conf::config.used_config_file.c_str()),
										 G_FILE_MONITOR_NONE, NULL, NULL);
	g_signal_connect(config_monitor, "changed", G_CALLBACK(config_changed), &wapanel::conf::config);

	// Searches various paths for applets.
	wapanel::applets::search_for_applets();
}

auto static app_activate(GtkApplication *_app) -> void {
	for (size_t i = 0; i < wapanel::conf::config.panels_conf.size(); i++) {
		wapanel::panel *panel = new wapanel::panel(i);

		gtk_application_add_window(_app, panel->get_gtk_window());

		log_info("Displaying panel %i", i);

		panels.push_back(panel);
	}
}

auto static app_shutdown(GtkApplication *_app) -> void { wapanel::applets::remove_applets(); }

auto main(int argc, char **argv) -> int {
	int status;

#ifdef VERSION
	log_info("Started wapanel version %s", VERSION_STRING(VERSION));
#endif

	// Create GTK Application.
	app = gtk_application_new("com.firstbober.wapanel", G_APPLICATION_FLAGS_NONE);

	g_signal_connect(app, "startup", G_CALLBACK(app_startup), NULL);
	g_signal_connect(app, "activate", G_CALLBACK(app_activate), NULL);
	g_signal_connect(app, "shutdown", G_CALLBACK(app_shutdown), NULL);

	status = g_application_run(G_APPLICATION(app), argc, argv);
	g_object_unref(app);

	// Remove panels on exit.
	for (auto &&panel : panels) {
		delete panel;
	}

	return status;
}