#pragma once
#include <appletapi.h>
#include <gtk/gtk.h>
#include <string>

/*
	Default config

	[[panel.applet]]
		name = "app-finder"
		icon = ""

		user_manager= ""
		settings = ""
		file_manager = ""

		[panel.applet.logout]
			shutdown = ""
			restart = ""
			logout = ""
			suspend = ""
			hibernate = ""
			lock = ""
*/

namespace wapanel::applet {

namespace ui_comps {

	class logout_box {
	private:
		GtkBox *m_root;

	public:
		logout_box();
		~logout_box();

		auto get_widget() -> GtkWidget *;

		struct config {
			std::string shutdown_cmd;
			std::string restart_cmd;
			std::string logout_cmd;
			std::string suspend_cmd;
			std::string hibernate_cmd;
			std::string lock_cmd;
		} cmp_config;
	};

	class action_bar {
	private:
		GtkBox *m_root;
		GtkButton *m_user_account_setting;

		GtkBox *m_fast_actions_container;
		GtkButton *m_system_settings;
		GtkButton *m_file_manager;

		GtkToggleButton *m_logout;
		GtkPopover *m_logout_popover;

		logout_box m_logout_content;

	public:
		action_bar();
		~action_bar();

		auto get_widget() -> GtkWidget *;

		struct config {
			std::string user_manager_cmd;
			std::string settings_cmd;
			std::string file_manager_cmd;
		} cmp_config;
	};

	class list_area {
	private:
		GtkNotebook *m_list_container;

	public:
		list_area();
		~list_area();

		auto get_widget() -> GtkWidget *;
	};

};

class app_finder {
private:
	int m_id;

	GtkToggleButton *m_app_finder_btn;
	GtkPopover *m_finder_popover;
	GtkBox *m_finder_aligner;

	ui_comps::action_bar *m_sidebar;
	ui_comps::list_area *m_list_area;

public:
	app_finder(wap_t_applet_config applet_config, int id);
	~app_finder();

	auto get_widget() -> GtkWidget *;
};

} // namespace wapanel::applet
