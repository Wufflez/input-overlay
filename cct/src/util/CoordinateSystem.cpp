#include "CoordinateSystem.hpp"

bool CoordinateSystem::handle_events(SDL_Event * e)
{
	bool was_handled = false;

	if (e->type == SDL_MOUSEBUTTONDOWN)
	{
		if (e->button.button == SDL_BUTTON_RIGHT)
		{
			m_drag_offset = { (e->button.x - m_origin.x) / m_scale_f * m_scale_f,
				(e->button.y - m_origin.y) / m_scale_f * m_scale_f };
			was_handled = true;
		}
	}
	else if (e->type == SDL_MOUSEMOTION)
	{
		if (e->motion.state & SDL_BUTTON_RMASK)
		{
			/* Align movement to grid*/
			m_origin.x = (SDL_min(e->button.x - m_drag_offset.x, m_origin_anchor.x))
				/ m_scale_f * m_scale_f;
			m_origin.y = (SDL_min(e->button.y - m_drag_offset.y, m_origin_anchor.y))
				/ m_scale_f * m_scale_f;
			was_handled = true;
		}
	}
	else if (e->type == SDL_MOUSEWHEEL)
	{
		if (e->wheel.y > 0) /* UP */
		{
			m_scale_f = SDL_min(m_scale_f++, 7);
		}
		else
		{
			m_scale_f = SDL_max(m_scale_f--, 1);
		}
		was_handled = true;
	}

	return was_handled;
}

void CoordinateSystem::draw_foreground(void)
{
	/* Fill space on above and to the left of the axes */
	/*SDL_Rect temp = { get_origin_left(), m_dimensions.y, m_dimensions.w - m_origin_anchor.x, m_origin_anchor.y };

	m_helper->util_fill_rect(&temp, m_helper->palette()->dark_gray());
	temp = { m_dimensions.x, m_dimensions.y, m_origin_anchor.x, m_dimensions.h - m_origin_anchor.y };
	m_helper->util_fill_rect(&temp, m_helper->palette()->dark_gray());*/

	/* Draw Scale*/
	int step = 10 * m_scale_f;
	int start;

	/* I think this makes sense */
	printf("o_l: %i\n", get_origin_left());
	start = get_origin_left() + ((m_origin.x - get_origin_left()) % (10 * m_scale_f)) + step;

	/* X Axis*/
	for (int x = start; x < get_right(); x += step)
	{
		bool flag = (x - m_origin.x) % 100 == 0 && (x - m_origin.x) != 0;
		if (flag)
		{
			std::string tag = std::to_string((x - m_origin.x) / m_scale_f);
			SDL_Rect dim = m_helper->util_text_dim(&tag);
			m_helper->util_text(&tag, x + dim.h / 2, get_origin_top() - dim.w - 6, m_helper->palette()->white(), 90);
			m_helper->util_draw_line(x, get_origin_top() - 4, x, get_origin_top() + 4, m_helper->palette()->white());
			m_helper->util_draw_line(x, get_origin_top() + 4, x, get_bottom(), m_helper->palette()->gray());
		}
		else
		{
			m_helper->util_draw_line(x, get_origin_top() - 2, x, get_origin_top() + 2, m_helper->palette()->white());
		}
	}

	start = get_origin_top() + ((m_origin.y - get_origin_top()) % (10 * m_scale_f)) + step;

	for (int y = start; y < get_bottom(); y += step)
	{
		bool flag = (y - m_origin.y) % 100 == 0 && (y - m_origin.x) != 0;

		if (flag)
		{
			std::string tag = std::to_string((y - m_origin.y) / m_scale_f);
			SDL_Rect dim = m_helper->util_text_dim(&tag);

			m_helper->util_text(&tag, get_origin_left() - dim.w - 5, y - dim.h / 2, m_helper->palette()->white());
			m_helper->util_draw_line(get_origin_left() - 4, y, get_origin_left() + 4, y, m_helper->palette()->white());
			m_helper->util_draw_line(get_origin_left() + 4, y, get_right(), y, m_helper->palette()->gray());
		}
		else
		{
			m_helper->util_draw_line(get_origin_left() - 2, y, get_origin_left() + 2, y, m_helper->palette()->white());
		}
	}

	/* Draw origin cross (0/0) */
	m_helper->util_draw_line(0, get_origin_top(), get_right(), get_origin_top(), m_helper->palette()->white());
	m_helper->util_draw_line(get_origin_left(), 0, get_origin_left(), get_bottom(), m_helper->palette()->white());

	/* Axe titles */
	std::string t = "X in pixels";
	SDL_Rect dim = m_helper->util_text_dim(&t);

	m_helper->util_text(&t, get_origin_left() - dim.w - 5, get_origin_top() - dim.h, m_helper->palette()->white());
	t = "Y in pixels";
	dim = m_helper->util_text_dim(&t);
	m_helper->util_text(&t, get_origin_left() - dim.h - 5, get_origin_top() + 5, m_helper->palette()->white(), 90);

	t = "Scale: " + std::to_string(m_scale_f);
	dim = m_helper->util_text_dim(&t);
	m_helper->util_text(&t, get_right() - dim.w - 5, 5, m_helper->palette()->white());
}

void CoordinateSystem::draw_background(void)
{
	
}
