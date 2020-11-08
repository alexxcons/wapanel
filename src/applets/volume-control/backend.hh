#pragma once

namespace wapanel::applet {

class backend
{
public:
	virtual auto get_input_volume_in_percent() -> int = 0;
	virtual auto set_input_volume_in_percent() -> void = 0;
	virtual auto mute_input() -> void = 0;
	virtual auto unmute_input() -> void = 0;

	virtual auto get_output_volume_in_percent() -> int = 0;
	virtual auto set_output_volume_in_percent() -> void = 0;
	virtual auto mute_output() -> void = 0;
	virtual auto unmute_output() -> void = 0;

	virtual ~backend() = default;
};


} // namespace wapanel::applet
