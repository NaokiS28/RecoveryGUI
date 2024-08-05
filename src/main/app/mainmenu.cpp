/*
 * 573in1 - Copyright (C) 2022-2024 spicyjpeg
 *
 * 573in1 is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * 573in1 is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * 573in1. If not, see <https://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include "common/util/hash.hpp"
#include "common/util/templates.hpp"
#include "main/app/app.hpp"
#include "main/app/mainmenu.hpp"
#include "main/uibase.hpp"

struct MenuEntry {
public:
	util::Hash name, prompt;
	void       (MainMenuScreen::*target)(ui::Context &ctx);
};

static const MenuEntry _MENU_ENTRIES[]{
	{
		.name   = "MainMenuScreen.entry1.name"_h,
		.prompt = "MainMenuScreen.entry1.prompt"_h,
		.target = &MainMenuScreen::action1
	}, {
		.name   = "MainMenuScreen.entry2.name"_h,
		.prompt = "MainMenuScreen.entry2.prompt"_h,
		.target = &MainMenuScreen::action2
	}, {
		.name   = "MainMenuScreen.entry3.name"_h,
		.prompt = "MainMenuScreen.entry3.prompt"_h,
		.target = &MainMenuScreen::action3
	}
};

const char *MainMenuScreen::_getItemName(ui::Context &ctx, int index) const {
	return STRH(_MENU_ENTRIES[index].name);
}

void MainMenuScreen::action1(ui::Context &ctx) {
	APP->_messageScreen.previousScreens[MESSAGE_SUCCESS] = this;
	APP->_messageScreen.setMessage(
		MESSAGE_SUCCESS, STR("MainMenuScreen.entryMessage"), 1
	);

	ctx.show(APP->_messageScreen, false, true);
}

void MainMenuScreen::action2(ui::Context &ctx) {
	APP->_messageScreen.previousScreens[MESSAGE_SUCCESS] = this;
	APP->_messageScreen.setMessage(
		MESSAGE_SUCCESS, STR("MainMenuScreen.entryMessage"), 2
	);

	ctx.show(APP->_messageScreen, false, true);
}

void MainMenuScreen::action3(ui::Context &ctx) {
	APP->_messageScreen.previousScreens[MESSAGE_SUCCESS] = this;
	APP->_messageScreen.setMessage(
		MESSAGE_SUCCESS, STR("MainMenuScreen.entryMessage"), 3
	);

	ctx.show(APP->_messageScreen, false, true);
}

void MainMenuScreen::show(ui::Context &ctx, bool goBack) {
	_title      = STR("MainMenuScreen.title");
	_prompt     = STRH(_MENU_ENTRIES[0].prompt);
	_itemPrompt = STR("MainMenuScreen.itemPrompt");

	_listLength = util::countOf(_MENU_ENTRIES);

	ListScreen::show(ctx, goBack);
}

void MainMenuScreen::update(ui::Context &ctx) {
	auto &action = _MENU_ENTRIES[_activeItem];
	_prompt      = STRH(action.prompt);

	ListScreen::update(ctx);

	if (ctx.buttons.pressed(ui::BTN_START))
		(this->*action.target)(ctx);
}
