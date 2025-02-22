#include "../client.h"

void command_level(Client *c, const Seperator *sep)
{
	const auto arguments = sep->argnum;
	if (!arguments || !sep->IsNumber(1)) {
		c->Message(Chat::White, "Usage: #level [Level]");
		return;
	}

	Mob* t = c;
	if (c->GetTarget()) {
		t = c->GetTarget();
	}

	auto level = static_cast<uint8>(Strings::ToUnsignedInt(sep->arg[1]));
	auto max_level = static_cast<uint8>(RuleI(Character, MaxLevel));

	if (c != t && c->Admin() < RuleI(GM, MinStatusToLevelTarget)) {
		c->Message(Chat::White, "Your status is not high enough to change another person's level.");
		return;
	}

	if (
		level > max_level &&
		c->Admin() < commandLevelAboveCap
	) {
		c->Message(
			Chat::White,
			fmt::format(
				"Level {} is above the Maximum Level of {} and your status is not high enough to go beyond the cap.",
				level,
				max_level
			).c_str()
		);
		return;
	}

	t->SetLevel(level, true);
	if (t->IsClient()) {
		t->CastToClient()->SendLevelAppearance();

		if (RuleB(Bots, Enabled) && RuleB(Bots, BotLevelsWithOwner)) {
			Bot::LevelBotWithClient(t->CastToClient(), level, true);
		}
	}
}
