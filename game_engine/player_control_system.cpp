#include "player_control_system.hpp"
#include "engine.hpp"
#include <functional>

extern aech::engine_t engine;

namespace aech
{
	void player_control_system_t::init()
	{
		auto fn = std::bind(&player_control_system_t::input_listener, this, std::placeholders::_1);
		engine.add_event_listener(events::window::input, fn);
	}

	void player_control_system_t::update(float dt)
	{
		//for (auto& entity : mEntities)
	//{
	//	auto& transform = gCoordinator.Get<Transform>(entity);


	//	if (mButtons.test(static_cast<std::size_t>(InputButtons::W)))
	//	{
	//		transform.position.z += (dt * 10.0f);
	//	}

	//	else if (mButtons.test(static_cast<std::size_t>(InputButtons::S)))
	//	{
	//		transform.position.z -= (dt * 10.0f);
	//	}


	//	if (mButtons.test(static_cast<std::size_t>(InputButtons::Q)))
	//	{
	//		transform.position.y += (dt * 10.0f);
	//	}

	//	else if (mButtons.test(static_cast<std::size_t>(InputButtons::E)))
	//	{
	//		transform.position.y -= (dt * 10.0f);
	//	}


	//	if (mButtons.test(static_cast<std::size_t>(InputButtons::A)))
	//	{
	//		transform.position.x += (dt * 10.0f);
	//	}

	//	else if (mButtons.test(static_cast<std::size_t>(InputButtons::D)))
	//	{
	//		transform.position.x -= (dt * 10.0f);
	//	}
	//}
	}

	void player_control_system_t::input_listener(event_t& event)
	{
		m_buttons = event.get_param<std::bitset<8>>(events::window::params::input);
	}
}
