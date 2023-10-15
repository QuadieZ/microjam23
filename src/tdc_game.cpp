#include "tdc_game.h"

#include "bn_keypad.h"

#include "mj/mj_game_list.h"

#include "bn_regular_bg_items_tdc_dont_press.h"
#include "bn_regular_bg_items_tdc_temple_1.h"
#include "bn_regular_bg_items_tdc_frame.h"

#include "bn_regular_bg_items_tdc_you_lose.h"

MJ_GAME_LIST_ADD(tdc::tdc_game)

namespace tdc
{

tdc_game::tdc_game(int completed_games, const mj::game_data& data) :
    _bg(bn::regular_bg_items::tdc_temple_1.create_bg(-100, 0)),
    _bg2(bn::regular_bg_items::tdc_dont_press.create_bg(0, 20)),
    _frame(bn::regular_bg_items::tdc_frame.create_bg(0, 0))
{
    constexpr int frames_diff = maximum_frames - minimum_frames;
    constexpr int maximum_speed_completed_games = 30;

    completed_games = bn::min(completed_games, maximum_speed_completed_games);

    int frames_reduction = (frames_diff * completed_games) / maximum_speed_completed_games;
    _total_frames = maximum_frames - frames_reduction;
    _total_frames -= data.random.get_int(60);
    _total_frames = bn::clamp(_total_frames, minimum_frames, maximum_frames);
} 

void tdc_game::fade_in([[maybe_unused]] const mj::game_data& data)
{
}

mj::game_result tdc_game::play(const mj::game_data& data)
{
    mj::game_result result;
    result.exit = data.pending_frames == 0;

    if(! _victory && ! _defeat)
    {
        if (data.pending_frames == 1) {
            _victory = true;
        }
        if(bn::keypad::any_pressed())
        {
            _bg2.set_item(bn::regular_bg_items::tdc_you_lose);
            result.remove_title = true;
            _defeat = true;
        }
    }
    else
    {
        if(_show_result_frames)
        {
            --_show_result_frames;
        }
        else
        {
            result.exit = true;
        }
    }

    return result;
}

void tdc_game::fade_out([[maybe_unused]] const mj::game_data& data)
{
}

void tdc_game::credits(mj::credits_type type, bn::ivector<bn::string_view>& output) const
{
    switch(type)
    {

    case mj::credits_type::CODE:
        output.push_back("GValiente");
        break;

    case mj::credits_type::GRAPHICS:
        output.push_back("GValiente");
        break;

    case mj::credits_type::MUSIC:
        break;

    case mj::credits_type::SOUND_EFFECTS:
        break;

    default:
        break;
    }
}

}
