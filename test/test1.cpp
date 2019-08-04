#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>

#include <iostream>
#include <sstream>
#include <string>
#include <wchar.h>
#include <sstream>

#include <lscl/lscl.hpp>

using namespace std;

TEST_CASE( "Testing availability of testing", "[global]" )
{
	std::stringstream ss;
	LSCL::Nodebuilder::Builder builder(ss);
	
	REQUIRE( 2 == 2 );
}

TEST_CASE( "Testing UTF-8 to uint32 translation", "[global]" )
{
	std::wstring ans;
	ans = LSCL::char_ptr_to_wstring("123");
	REQUIRE(ans == L"123");
	ans = LSCL::char_ptr_to_wstring("");
	REQUIRE(ans == L"");
	ans = LSCL::char_ptr_to_wstring("абырВАЛГ");
	REQUIRE(ans == L"абырВАЛГ");
	ans = LSCL::char_ptr_to_wstring("😍😘😗😙😚🤗😻👫👬👭💑👩‍❤‍👩👨‍❤‍👨💏👩‍❤‍💋‍👩👨‍❤‍💋‍👨💋🌹🌷🌼💐💍🍸☕🏩💒🎈🎀🎁💌❤💛💙💚💜💔❣💕💞💓💗💖💘💝💟😀😬😁😂😃😄😅😆😇😉😊🙂🙃☺😋😌😍😘😗😙😚😜😝😛🤑🤓😎🤗😏😶😐😑😒🙄🤔😳😞😟😠😡😔😕🙁☹😣😖😫😩😤😮😱😨😰😯😦😧😢😥😪😓😭😵😲🤐😷🤒🤕😴💤💩😈👿👹👺💀👻👽🤖😺😸😹😻😼😽🙀😿😾🙌👏👋👍👎👊✊✌👌✋💪🙏☝👆👇👈👉🖕🤘🖖✍💅👄👅👂👃👁👀👤🗣👶👦👧👨👩👱👴👵👲👳👮👷💂🕵🎅👼👸👰🚶🏃💃👯👫👬👭🙇💁🙅🙆🙋🙎🙍💇💆💑👩‍❤‍👩👨‍❤‍👨💏👩‍❤‍💋‍👩👨‍❤‍💋‍👨👪👨‍👩‍👧👨‍👩‍👧‍👦👨‍👩‍👦‍👦👨‍👩‍👧‍👧👩‍👩‍👦👩‍👩‍👧👩‍👩‍👧‍👦👩‍👩‍👦‍👦👩‍👩‍👧‍👧👨‍👨‍👦👨‍👨‍👧👨‍👨‍👧‍👦👨‍👨‍👦‍👦👨‍👨‍👧‍👧👚👕👖👔👗👙👘💄💋👣👠👡👢👞👟👒🎩⛑🎓👑🎒👝👛👜💼👓🕶💍🌂👦🏻👧🏻👨🏻👩🏻👴🏻👵🏻👶🏻👱🏻👮🏻👲🏻👳🏻👷🏻👸🏻💂🏻🎅🏻👼🏻💆🏻💇🏻👰🏻🙍🏻🙎🏻🙅🏻🙆🏻💁🏻🙋🏻🙇🏻🙌🏻🙏🏻🚶🏻🏃🏻💃🏻💪🏻👈🏻👉🏻☝🏻👆🏻🖕🏻👇🏻✌🏻🖖🏻🤘🏻🖐🏻✊🏻✋🏻👊🏻👌🏻👍🏻👎🏻👋🏻👏🏻👐🏻✍🏻💅🏻👂🏻👃🏻🚣🏻🛀🏻🏄🏻🏇🏻🏊🏻🚴🏻🚵🏻🐶🐱🐭🐹🐰🐻🐼🐨🐯🦁🐮🐷🐽🐸🐙🐵🙈🙉🙊🐒🐔🐧🐦🐤🐣🐥🐺🐗🐴🦄🐝🐛🐌🐞🐜🕷🦂🦀🐍🐢🐠🐟🐡🐬🐳🐋🐊🐆🐅🐃🐂🐄🐪🐫🐘🐐🐏🐑🐎🐖🐀🐁🐓🦃🕊🐕🐩🐈🐇🐿🐾🐉🐲🌵🎄🌲🌳🌴🌱🌿☘🍀🎍🎋🍃🍂🍁🌾🌺🌻🌹🌷🌼🌸💐🍄🌰🎃🐚🕸🌎🌍🌏🌕🌖🌗🌘🌑🌒🌓🌔🌚🌝🌛🌜🌞🌙⭐🌟💫✨☄☀🌤⛅🌥🌦☁🌧⛈🌩⚡🔥💥❄🌨⛄☃🌬💨🌪🌫☂☔💧💦🌊🍏🍎🍐🍊🍋🍌🍉🍇🍓🍈🍒🍑🍍🍅🍆🌶🌽🍠🍯🍞🧀🍗🍖🍤🍳🍔🍟🌭🍕🍝🌮🌯🍜🍲🍥🍣🍱🍛🍙🍚🍘🍢🍡🍧🍨🍦🍰🎂🍮🍬🍭🍫🍿🍩🍪🍺🍻🍷🍸🍹🍾🍶🍵☕🍼🍴🍽⚽🏀🏈⚾🎾🏐🏉🎱⛳🏌🏓🏸🏒🏑🏏🎿⛷🏂⛸🏹🎣🚣🏊🏄🛀⛹🏋🚴🚵🏇🕴🏆🎽🏅🎖🎗🏵🎫🎟🎭🎨🎪🎤🎧🎼🎹🎷🎺🎸🎻🎬🎮👾🎯🎲🎰🎳🚗🚕🚙🚌🚎🏎🚓🚑🚒🚐🚚🚛🚜🏍🚲🚨🚔🚍🚘🚖🚡🚠🚟🚃🚋🚝🚄🚅🚈🚞🚂🚆🚇🚊🚉🚁🛩✈🛫🛬⛵🛥🚤⛴🛳🚀🛰💺⚓🚧⛽🚏🚦🚥🏁🚢🎡🎢🎠🏗🌁🗼🏭⛲🎑⛰🏔🗻🌋🗾🏕⛺🏞🛣🛤🌅🌄🏜🏖🏝🌇🌆🏙🌃🌉🌌🌠🎇🎆🌈🏘🏰🏯🏟🗽🏠🏡🏚🏢🏬🏣🏤🏥🏦🏨🏪🏫🏩💒🏛⛪🕌🕍🕋⛩⌚📱📲💻⌨🖥🖨🖱🖲🕹🗜💽💾💿📀📼📷📸📹🎥📽🎞📞☎📟📠📺📻🎙🎚🎛⏱⏲⏰🕰⏳⌛📡🔋🔌💡🔦🕯🗑🛢💸💵💴💶💷💰💳💎⚖🔧🔨⚒🛠⛏🔩⚙⛓🔫💣🔪🗡⚔🛡🚬☠⚰⚱🏺🔮📿💈⚗🔭🔬🕳💊💉🌡🏷🔖🚽🚿🛁🔑🗝🛋🛌🛏🚪🛎🖼🗺⛱🗿🛍🎈🎏🎀🎁🎊🎉🎎🎐🎌🏮✉📩📨📧💌📮📪📫📬📭📦📯📥📤📜📃📑📊📈📉📄📅📆🗓📇🗃🗳🗄📋🗒📁📂🗂🗞📰📓📕📗📘📙📔📒📚📖🔗📎🖇✂📐📏📌📍🚩🏳🏴🔐🔒🔓🔏🖊🖋✒📝✏🖍🖌🔍🔎❤💛💙💚💜💔❣💕💞💓💗💖💘💝💟☮✝☪🕉☸✡🔯🕎☯☦🛐⛎♈♉♊♋♌♍♎♏♐♑♒♓🆔⚛🈳🈹☢☣📴📳🈶🈚🈸🈺🈷✴🆚🉑💮🉐㊙㊗🈴🈵🈲🅰🅱🆎🆑🅾🆘⛔📛🚫❌⭕💢♨🚷🚯🚳🚱🔞📵❗❕❓❔‼⁉💯🔅🔆🔱⚜〽⚠🚸🔰♻🈯💹❇✳❎✅💠🌀➿🌐Ⓜ🏧🈂🛂🛃🛄🛅♿🚭🚾🅿🚰🚹🚺🚼🚻🚮🎦📶🈁🆖🆗🆙🆒🆕🆓0⃣1⃣2⃣3⃣4⃣5⃣6⃣7⃣8⃣9⃣🔟🔢▶⏸⏯⏹⏺⏭⏮⏩⏪🔀🔁🔂◀🔼🔽⏫⏬➡⬅⬆⬇↗↘↙↖↕↔🔄↪↩⤴⤵#⃣*⃣ℹ🔤🔡🔠🔣🎵🎶〰➰✔🔃➕➖➗✖💲💱©®™🔚🔙🔛🔝🔜☑🔘⚪⚫🔴🔵🔸🔹🔶🔷🔺▪▫⬛⬜🔻◼◻◾◽🔲🔳🔈🔉🔊🔇📣📢🔔🔕🃏🀄♠♣♥♦🎴👁‍🗨💭🗯💬🕐🕑🕒🕓🕔🕕🕖🕗🕘🕙🕚🕛🕜🕝🕞🕟🕠🕡🕢🕣🕤🕥🕦🕧🇦🇺🇦🇹🇧🇾🇧🇪🇧🇷🇨🇦🇨🇱🇨🇳🇨🇴🇩🇰🇫🇮🇫🇷🇩🇪🇭🇰🇮🇳🇮🇩🇮🇪🇮🇱🇮🇹🇯🇵🇰🇿🇲🇴🇲🇾🇲🇽🇳🇱🇳🇿🇳🇴🇵🇭🇵🇱🇵🇹🇵🇷🇷🇺🇸🇦🇸🇬🇿🇦🇰🇷🇪🇸🇸🇪🇨🇭🇹🇷🇺🇦🇦🇪🇬🇧🇺🇸🇻🇳");
	REQUIRE(ans == L"😍😘😗😙😚🤗😻👫👬👭💑👩‍❤‍👩👨‍❤‍👨💏👩‍❤‍💋‍👩👨‍❤‍💋‍👨💋🌹🌷🌼💐💍🍸☕🏩💒🎈🎀🎁💌❤💛💙💚💜💔❣💕💞💓💗💖💘💝💟😀😬😁😂😃😄😅😆😇😉😊🙂🙃☺😋😌😍😘😗😙😚😜😝😛🤑🤓😎🤗😏😶😐😑😒🙄🤔😳😞😟😠😡😔😕🙁☹😣😖😫😩😤😮😱😨😰😯😦😧😢😥😪😓😭😵😲🤐😷🤒🤕😴💤💩😈👿👹👺💀👻👽🤖😺😸😹😻😼😽🙀😿😾🙌👏👋👍👎👊✊✌👌✋💪🙏☝👆👇👈👉🖕🤘🖖✍💅👄👅👂👃👁👀👤🗣👶👦👧👨👩👱👴👵👲👳👮👷💂🕵🎅👼👸👰🚶🏃💃👯👫👬👭🙇💁🙅🙆🙋🙎🙍💇💆💑👩‍❤‍👩👨‍❤‍👨💏👩‍❤‍💋‍👩👨‍❤‍💋‍👨👪👨‍👩‍👧👨‍👩‍👧‍👦👨‍👩‍👦‍👦👨‍👩‍👧‍👧👩‍👩‍👦👩‍👩‍👧👩‍👩‍👧‍👦👩‍👩‍👦‍👦👩‍👩‍👧‍👧👨‍👨‍👦👨‍👨‍👧👨‍👨‍👧‍👦👨‍👨‍👦‍👦👨‍👨‍👧‍👧👚👕👖👔👗👙👘💄💋👣👠👡👢👞👟👒🎩⛑🎓👑🎒👝👛👜💼👓🕶💍🌂👦🏻👧🏻👨🏻👩🏻👴🏻👵🏻👶🏻👱🏻👮🏻👲🏻👳🏻👷🏻👸🏻💂🏻🎅🏻👼🏻💆🏻💇🏻👰🏻🙍🏻🙎🏻🙅🏻🙆🏻💁🏻🙋🏻🙇🏻🙌🏻🙏🏻🚶🏻🏃🏻💃🏻💪🏻👈🏻👉🏻☝🏻👆🏻🖕🏻👇🏻✌🏻🖖🏻🤘🏻🖐🏻✊🏻✋🏻👊🏻👌🏻👍🏻👎🏻👋🏻👏🏻👐🏻✍🏻💅🏻👂🏻👃🏻🚣🏻🛀🏻🏄🏻🏇🏻🏊🏻🚴🏻🚵🏻🐶🐱🐭🐹🐰🐻🐼🐨🐯🦁🐮🐷🐽🐸🐙🐵🙈🙉🙊🐒🐔🐧🐦🐤🐣🐥🐺🐗🐴🦄🐝🐛🐌🐞🐜🕷🦂🦀🐍🐢🐠🐟🐡🐬🐳🐋🐊🐆🐅🐃🐂🐄🐪🐫🐘🐐🐏🐑🐎🐖🐀🐁🐓🦃🕊🐕🐩🐈🐇🐿🐾🐉🐲🌵🎄🌲🌳🌴🌱🌿☘🍀🎍🎋🍃🍂🍁🌾🌺🌻🌹🌷🌼🌸💐🍄🌰🎃🐚🕸🌎🌍🌏🌕🌖🌗🌘🌑🌒🌓🌔🌚🌝🌛🌜🌞🌙⭐🌟💫✨☄☀🌤⛅🌥🌦☁🌧⛈🌩⚡🔥💥❄🌨⛄☃🌬💨🌪🌫☂☔💧💦🌊🍏🍎🍐🍊🍋🍌🍉🍇🍓🍈🍒🍑🍍🍅🍆🌶🌽🍠🍯🍞🧀🍗🍖🍤🍳🍔🍟🌭🍕🍝🌮🌯🍜🍲🍥🍣🍱🍛🍙🍚🍘🍢🍡🍧🍨🍦🍰🎂🍮🍬🍭🍫🍿🍩🍪🍺🍻🍷🍸🍹🍾🍶🍵☕🍼🍴🍽⚽🏀🏈⚾🎾🏐🏉🎱⛳🏌🏓🏸🏒🏑🏏🎿⛷🏂⛸🏹🎣🚣🏊🏄🛀⛹🏋🚴🚵🏇🕴🏆🎽🏅🎖🎗🏵🎫🎟🎭🎨🎪🎤🎧🎼🎹🎷🎺🎸🎻🎬🎮👾🎯🎲🎰🎳🚗🚕🚙🚌🚎🏎🚓🚑🚒🚐🚚🚛🚜🏍🚲🚨🚔🚍🚘🚖🚡🚠🚟🚃🚋🚝🚄🚅🚈🚞🚂🚆🚇🚊🚉🚁🛩✈🛫🛬⛵🛥🚤⛴🛳🚀🛰💺⚓🚧⛽🚏🚦🚥🏁🚢🎡🎢🎠🏗🌁🗼🏭⛲🎑⛰🏔🗻🌋🗾🏕⛺🏞🛣🛤🌅🌄🏜🏖🏝🌇🌆🏙🌃🌉🌌🌠🎇🎆🌈🏘🏰🏯🏟🗽🏠🏡🏚🏢🏬🏣🏤🏥🏦🏨🏪🏫🏩💒🏛⛪🕌🕍🕋⛩⌚📱📲💻⌨🖥🖨🖱🖲🕹🗜💽💾💿📀📼📷📸📹🎥📽🎞📞☎📟📠📺📻🎙🎚🎛⏱⏲⏰🕰⏳⌛📡🔋🔌💡🔦🕯🗑🛢💸💵💴💶💷💰💳💎⚖🔧🔨⚒🛠⛏🔩⚙⛓🔫💣🔪🗡⚔🛡🚬☠⚰⚱🏺🔮📿💈⚗🔭🔬🕳💊💉🌡🏷🔖🚽🚿🛁🔑🗝🛋🛌🛏🚪🛎🖼🗺⛱🗿🛍🎈🎏🎀🎁🎊🎉🎎🎐🎌🏮✉📩📨📧💌📮📪📫📬📭📦📯📥📤📜📃📑📊📈📉📄📅📆🗓📇🗃🗳🗄📋🗒📁📂🗂🗞📰📓📕📗📘📙📔📒📚📖🔗📎🖇✂📐📏📌📍🚩🏳🏴🔐🔒🔓🔏🖊🖋✒📝✏🖍🖌🔍🔎❤💛💙💚💜💔❣💕💞💓💗💖💘💝💟☮✝☪🕉☸✡🔯🕎☯☦🛐⛎♈♉♊♋♌♍♎♏♐♑♒♓🆔⚛🈳🈹☢☣📴📳🈶🈚🈸🈺🈷✴🆚🉑💮🉐㊙㊗🈴🈵🈲🅰🅱🆎🆑🅾🆘⛔📛🚫❌⭕💢♨🚷🚯🚳🚱🔞📵❗❕❓❔‼⁉💯🔅🔆🔱⚜〽⚠🚸🔰♻🈯💹❇✳❎✅💠🌀➿🌐Ⓜ🏧🈂🛂🛃🛄🛅♿🚭🚾🅿🚰🚹🚺🚼🚻🚮🎦📶🈁🆖🆗🆙🆒🆕🆓0⃣1⃣2⃣3⃣4⃣5⃣6⃣7⃣8⃣9⃣🔟🔢▶⏸⏯⏹⏺⏭⏮⏩⏪🔀🔁🔂◀🔼🔽⏫⏬➡⬅⬆⬇↗↘↙↖↕↔🔄↪↩⤴⤵#⃣*⃣ℹ🔤🔡🔠🔣🎵🎶〰➰✔🔃➕➖➗✖💲💱©®™🔚🔙🔛🔝🔜☑🔘⚪⚫🔴🔵🔸🔹🔶🔷🔺▪▫⬛⬜🔻◼◻◾◽🔲🔳🔈🔉🔊🔇📣📢🔔🔕🃏🀄♠♣♥♦🎴👁‍🗨💭🗯💬🕐🕑🕒🕓🕔🕕🕖🕗🕘🕙🕚🕛🕜🕝🕞🕟🕠🕡🕢🕣🕤🕥🕦🕧🇦🇺🇦🇹🇧🇾🇧🇪🇧🇷🇨🇦🇨🇱🇨🇳🇨🇴🇩🇰🇫🇮🇫🇷🇩🇪🇭🇰🇮🇳🇮🇩🇮🇪🇮🇱🇮🇹🇯🇵🇰🇿🇲🇴🇲🇾🇲🇽🇳🇱🇳🇿🇳🇴🇵🇭🇵🇱🇵🇹🇵🇷🇷🇺🇸🇦🇸🇬🇿🇦🇰🇷🇪🇸🇸🇪🇨🇭🇹🇷🇺🇦🇦🇪🇬🇧🇺🇸🇻🇳");
	std::vector<uint32_t> ans2;
	ans2 = LSCL::utf8_vector_to_uint32_vector(std::vector<uint8_t>{0b11011111, 0b10111111});
	REQUIRE(ans2.size() == 1);
	REQUIRE(ans2[0] == 2047);
}

TEST_CASE( "Testing uint32 to UTF-8 translation", "[global]" )
{
	std::string ans;
	ans = LSCL::wchar_ptr_to_string(L"123");
	REQUIRE(ans == "123");
	ans = LSCL::wchar_ptr_to_string(L"");
	REQUIRE(ans == "");
	ans = LSCL::wchar_ptr_to_string(L"абырВАЛГ");
	REQUIRE(ans == "абырВАЛГ");
	ans = LSCL::wchar_ptr_to_string(L"😍😘😗😙😚🤗😻👫👬👭💑👩‍❤‍👩👨‍❤‍👨💏👩‍❤‍💋‍👩👨‍❤‍💋‍👨💋🌹🌷🌼💐💍🍸☕🏩💒🎈🎀🎁💌❤💛💙💚💜💔❣💕💞💓💗💖💘💝💟😀😬😁😂😃😄😅😆😇😉😊🙂🙃☺😋😌😍😘😗😙😚😜😝😛🤑🤓😎🤗😏😶😐😑😒🙄🤔😳😞😟😠😡😔😕🙁☹😣😖😫😩😤😮😱😨😰😯😦😧😢😥😪😓😭😵😲🤐😷🤒🤕😴💤💩😈👿👹👺💀👻👽🤖😺😸😹😻😼😽🙀😿😾🙌👏👋👍👎👊✊✌👌✋💪🙏☝👆👇👈👉🖕🤘🖖✍💅👄👅👂👃👁👀👤🗣👶👦👧👨👩👱👴👵👲👳👮👷💂🕵🎅👼👸👰🚶🏃💃👯👫👬👭🙇💁🙅🙆🙋🙎🙍💇💆💑👩‍❤‍👩👨‍❤‍👨💏👩‍❤‍💋‍👩👨‍❤‍💋‍👨👪👨‍👩‍👧👨‍👩‍👧‍👦👨‍👩‍👦‍👦👨‍👩‍👧‍👧👩‍👩‍👦👩‍👩‍👧👩‍👩‍👧‍👦👩‍👩‍👦‍👦👩‍👩‍👧‍👧👨‍👨‍👦👨‍👨‍👧👨‍👨‍👧‍👦👨‍👨‍👦‍👦👨‍👨‍👧‍👧👚👕👖👔👗👙👘💄💋👣👠👡👢👞👟👒🎩⛑🎓👑🎒👝👛👜💼👓🕶💍🌂👦🏻👧🏻👨🏻👩🏻👴🏻👵🏻👶🏻👱🏻👮🏻👲🏻👳🏻👷🏻👸🏻💂🏻🎅🏻👼🏻💆🏻💇🏻👰🏻🙍🏻🙎🏻🙅🏻🙆🏻💁🏻🙋🏻🙇🏻🙌🏻🙏🏻🚶🏻🏃🏻💃🏻💪🏻👈🏻👉🏻☝🏻👆🏻🖕🏻👇🏻✌🏻🖖🏻🤘🏻🖐🏻✊🏻✋🏻👊🏻👌🏻👍🏻👎🏻👋🏻👏🏻👐🏻✍🏻💅🏻👂🏻👃🏻🚣🏻🛀🏻🏄🏻🏇🏻🏊🏻🚴🏻🚵🏻🐶🐱🐭🐹🐰🐻🐼🐨🐯🦁🐮🐷🐽🐸🐙🐵🙈🙉🙊🐒🐔🐧🐦🐤🐣🐥🐺🐗🐴🦄🐝🐛🐌🐞🐜🕷🦂🦀🐍🐢🐠🐟🐡🐬🐳🐋🐊🐆🐅🐃🐂🐄🐪🐫🐘🐐🐏🐑🐎🐖🐀🐁🐓🦃🕊🐕🐩🐈🐇🐿🐾🐉🐲🌵🎄🌲🌳🌴🌱🌿☘🍀🎍🎋🍃🍂🍁🌾🌺🌻🌹🌷🌼🌸💐🍄🌰🎃🐚🕸🌎🌍🌏🌕🌖🌗🌘🌑🌒🌓🌔🌚🌝🌛🌜🌞🌙⭐🌟💫✨☄☀🌤⛅🌥🌦☁🌧⛈🌩⚡🔥💥❄🌨⛄☃🌬💨🌪🌫☂☔💧💦🌊🍏🍎🍐🍊🍋🍌🍉🍇🍓🍈🍒🍑🍍🍅🍆🌶🌽🍠🍯🍞🧀🍗🍖🍤🍳🍔🍟🌭🍕🍝🌮🌯🍜🍲🍥🍣🍱🍛🍙🍚🍘🍢🍡🍧🍨🍦🍰🎂🍮🍬🍭🍫🍿🍩🍪🍺🍻🍷🍸🍹🍾🍶🍵☕🍼🍴🍽⚽🏀🏈⚾🎾🏐🏉🎱⛳🏌🏓🏸🏒🏑🏏🎿⛷🏂⛸🏹🎣🚣🏊🏄🛀⛹🏋🚴🚵🏇🕴🏆🎽🏅🎖🎗🏵🎫🎟🎭🎨🎪🎤🎧🎼🎹🎷🎺🎸🎻🎬🎮👾🎯🎲🎰🎳🚗🚕🚙🚌🚎🏎🚓🚑🚒🚐🚚🚛🚜🏍🚲🚨🚔🚍🚘🚖🚡🚠🚟🚃🚋🚝🚄🚅🚈🚞🚂🚆🚇🚊🚉🚁🛩✈🛫🛬⛵🛥🚤⛴🛳🚀🛰💺⚓🚧⛽🚏🚦🚥🏁🚢🎡🎢🎠🏗🌁🗼🏭⛲🎑⛰🏔🗻🌋🗾🏕⛺🏞🛣🛤🌅🌄🏜🏖🏝🌇🌆🏙🌃🌉🌌🌠🎇🎆🌈🏘🏰🏯🏟🗽🏠🏡🏚🏢🏬🏣🏤🏥🏦🏨🏪🏫🏩💒🏛⛪🕌🕍🕋⛩⌚📱📲💻⌨🖥🖨🖱🖲🕹🗜💽💾💿📀📼📷📸📹🎥📽🎞📞☎📟📠📺📻🎙🎚🎛⏱⏲⏰🕰⏳⌛📡🔋🔌💡🔦🕯🗑🛢💸💵💴💶💷💰💳💎⚖🔧🔨⚒🛠⛏🔩⚙⛓🔫💣🔪🗡⚔🛡🚬☠⚰⚱🏺🔮📿💈⚗🔭🔬🕳💊💉🌡🏷🔖🚽🚿🛁🔑🗝🛋🛌🛏🚪🛎🖼🗺⛱🗿🛍🎈🎏🎀🎁🎊🎉🎎🎐🎌🏮✉📩📨📧💌📮📪📫📬📭📦📯📥📤📜📃📑📊📈📉📄📅📆🗓📇🗃🗳🗄📋🗒📁📂🗂🗞📰📓📕📗📘📙📔📒📚📖🔗📎🖇✂📐📏📌📍🚩🏳🏴🔐🔒🔓🔏🖊🖋✒📝✏🖍🖌🔍🔎❤💛💙💚💜💔❣💕💞💓💗💖💘💝💟☮✝☪🕉☸✡🔯🕎☯☦🛐⛎♈♉♊♋♌♍♎♏♐♑♒♓🆔⚛🈳🈹☢☣📴📳🈶🈚🈸🈺🈷✴🆚🉑💮🉐㊙㊗🈴🈵🈲🅰🅱🆎🆑🅾🆘⛔📛🚫❌⭕💢♨🚷🚯🚳🚱🔞📵❗❕❓❔‼⁉💯🔅🔆🔱⚜〽⚠🚸🔰♻🈯💹❇✳❎✅💠🌀➿🌐Ⓜ🏧🈂🛂🛃🛄🛅♿🚭🚾🅿🚰🚹🚺🚼🚻🚮🎦📶🈁🆖🆗🆙🆒🆕🆓0⃣1⃣2⃣3⃣4⃣5⃣6⃣7⃣8⃣9⃣🔟🔢▶⏸⏯⏹⏺⏭⏮⏩⏪🔀🔁🔂◀🔼🔽⏫⏬➡⬅⬆⬇↗↘↙↖↕↔🔄↪↩⤴⤵#⃣*⃣ℹ🔤🔡🔠🔣🎵🎶〰➰✔🔃➕➖➗✖💲💱©®™🔚🔙🔛🔝🔜☑🔘⚪⚫🔴🔵🔸🔹🔶🔷🔺▪▫⬛⬜🔻◼◻◾◽🔲🔳🔈🔉🔊🔇📣📢🔔🔕🃏🀄♠♣♥♦🎴👁‍🗨💭🗯💬🕐🕑🕒🕓🕔🕕🕖🕗🕘🕙🕚🕛🕜🕝🕞🕟🕠🕡🕢🕣🕤🕥🕦🕧🇦🇺🇦🇹🇧🇾🇧🇪🇧🇷🇨🇦🇨🇱🇨🇳🇨🇴🇩🇰🇫🇮🇫🇷🇩🇪🇭🇰🇮🇳🇮🇩🇮🇪🇮🇱🇮🇹🇯🇵🇰🇿🇲🇴🇲🇾🇲🇽🇳🇱🇳🇿🇳🇴🇵🇭🇵🇱🇵🇹🇵🇷🇷🇺🇸🇦🇸🇬🇿🇦🇰🇷🇪🇸🇸🇪🇨🇭🇹🇷🇺🇦🇦🇪🇬🇧🇺🇸🇻🇳");
	REQUIRE(ans == "😍😘😗😙😚🤗😻👫👬👭💑👩‍❤‍👩👨‍❤‍👨💏👩‍❤‍💋‍👩👨‍❤‍💋‍👨💋🌹🌷🌼💐💍🍸☕🏩💒🎈🎀🎁💌❤💛💙💚💜💔❣💕💞💓💗💖💘💝💟😀😬😁😂😃😄😅😆😇😉😊🙂🙃☺😋😌😍😘😗😙😚😜😝😛🤑🤓😎🤗😏😶😐😑😒🙄🤔😳😞😟😠😡😔😕🙁☹😣😖😫😩😤😮😱😨😰😯😦😧😢😥😪😓😭😵😲🤐😷🤒🤕😴💤💩😈👿👹👺💀👻👽🤖😺😸😹😻😼😽🙀😿😾🙌👏👋👍👎👊✊✌👌✋💪🙏☝👆👇👈👉🖕🤘🖖✍💅👄👅👂👃👁👀👤🗣👶👦👧👨👩👱👴👵👲👳👮👷💂🕵🎅👼👸👰🚶🏃💃👯👫👬👭🙇💁🙅🙆🙋🙎🙍💇💆💑👩‍❤‍👩👨‍❤‍👨💏👩‍❤‍💋‍👩👨‍❤‍💋‍👨👪👨‍👩‍👧👨‍👩‍👧‍👦👨‍👩‍👦‍👦👨‍👩‍👧‍👧👩‍👩‍👦👩‍👩‍👧👩‍👩‍👧‍👦👩‍👩‍👦‍👦👩‍👩‍👧‍👧👨‍👨‍👦👨‍👨‍👧👨‍👨‍👧‍👦👨‍👨‍👦‍👦👨‍👨‍👧‍👧👚👕👖👔👗👙👘💄💋👣👠👡👢👞👟👒🎩⛑🎓👑🎒👝👛👜💼👓🕶💍🌂👦🏻👧🏻👨🏻👩🏻👴🏻👵🏻👶🏻👱🏻👮🏻👲🏻👳🏻👷🏻👸🏻💂🏻🎅🏻👼🏻💆🏻💇🏻👰🏻🙍🏻🙎🏻🙅🏻🙆🏻💁🏻🙋🏻🙇🏻🙌🏻🙏🏻🚶🏻🏃🏻💃🏻💪🏻👈🏻👉🏻☝🏻👆🏻🖕🏻👇🏻✌🏻🖖🏻🤘🏻🖐🏻✊🏻✋🏻👊🏻👌🏻👍🏻👎🏻👋🏻👏🏻👐🏻✍🏻💅🏻👂🏻👃🏻🚣🏻🛀🏻🏄🏻🏇🏻🏊🏻🚴🏻🚵🏻🐶🐱🐭🐹🐰🐻🐼🐨🐯🦁🐮🐷🐽🐸🐙🐵🙈🙉🙊🐒🐔🐧🐦🐤🐣🐥🐺🐗🐴🦄🐝🐛🐌🐞🐜🕷🦂🦀🐍🐢🐠🐟🐡🐬🐳🐋🐊🐆🐅🐃🐂🐄🐪🐫🐘🐐🐏🐑🐎🐖🐀🐁🐓🦃🕊🐕🐩🐈🐇🐿🐾🐉🐲🌵🎄🌲🌳🌴🌱🌿☘🍀🎍🎋🍃🍂🍁🌾🌺🌻🌹🌷🌼🌸💐🍄🌰🎃🐚🕸🌎🌍🌏🌕🌖🌗🌘🌑🌒🌓🌔🌚🌝🌛🌜🌞🌙⭐🌟💫✨☄☀🌤⛅🌥🌦☁🌧⛈🌩⚡🔥💥❄🌨⛄☃🌬💨🌪🌫☂☔💧💦🌊🍏🍎🍐🍊🍋🍌🍉🍇🍓🍈🍒🍑🍍🍅🍆🌶🌽🍠🍯🍞🧀🍗🍖🍤🍳🍔🍟🌭🍕🍝🌮🌯🍜🍲🍥🍣🍱🍛🍙🍚🍘🍢🍡🍧🍨🍦🍰🎂🍮🍬🍭🍫🍿🍩🍪🍺🍻🍷🍸🍹🍾🍶🍵☕🍼🍴🍽⚽🏀🏈⚾🎾🏐🏉🎱⛳🏌🏓🏸🏒🏑🏏🎿⛷🏂⛸🏹🎣🚣🏊🏄🛀⛹🏋🚴🚵🏇🕴🏆🎽🏅🎖🎗🏵🎫🎟🎭🎨🎪🎤🎧🎼🎹🎷🎺🎸🎻🎬🎮👾🎯🎲🎰🎳🚗🚕🚙🚌🚎🏎🚓🚑🚒🚐🚚🚛🚜🏍🚲🚨🚔🚍🚘🚖🚡🚠🚟🚃🚋🚝🚄🚅🚈🚞🚂🚆🚇🚊🚉🚁🛩✈🛫🛬⛵🛥🚤⛴🛳🚀🛰💺⚓🚧⛽🚏🚦🚥🏁🚢🎡🎢🎠🏗🌁🗼🏭⛲🎑⛰🏔🗻🌋🗾🏕⛺🏞🛣🛤🌅🌄🏜🏖🏝🌇🌆🏙🌃🌉🌌🌠🎇🎆🌈🏘🏰🏯🏟🗽🏠🏡🏚🏢🏬🏣🏤🏥🏦🏨🏪🏫🏩💒🏛⛪🕌🕍🕋⛩⌚📱📲💻⌨🖥🖨🖱🖲🕹🗜💽💾💿📀📼📷📸📹🎥📽🎞📞☎📟📠📺📻🎙🎚🎛⏱⏲⏰🕰⏳⌛📡🔋🔌💡🔦🕯🗑🛢💸💵💴💶💷💰💳💎⚖🔧🔨⚒🛠⛏🔩⚙⛓🔫💣🔪🗡⚔🛡🚬☠⚰⚱🏺🔮📿💈⚗🔭🔬🕳💊💉🌡🏷🔖🚽🚿🛁🔑🗝🛋🛌🛏🚪🛎🖼🗺⛱🗿🛍🎈🎏🎀🎁🎊🎉🎎🎐🎌🏮✉📩📨📧💌📮📪📫📬📭📦📯📥📤📜📃📑📊📈📉📄📅📆🗓📇🗃🗳🗄📋🗒📁📂🗂🗞📰📓📕📗📘📙📔📒📚📖🔗📎🖇✂📐📏📌📍🚩🏳🏴🔐🔒🔓🔏🖊🖋✒📝✏🖍🖌🔍🔎❤💛💙💚💜💔❣💕💞💓💗💖💘💝💟☮✝☪🕉☸✡🔯🕎☯☦🛐⛎♈♉♊♋♌♍♎♏♐♑♒♓🆔⚛🈳🈹☢☣📴📳🈶🈚🈸🈺🈷✴🆚🉑💮🉐㊙㊗🈴🈵🈲🅰🅱🆎🆑🅾🆘⛔📛🚫❌⭕💢♨🚷🚯🚳🚱🔞📵❗❕❓❔‼⁉💯🔅🔆🔱⚜〽⚠🚸🔰♻🈯💹❇✳❎✅💠🌀➿🌐Ⓜ🏧🈂🛂🛃🛄🛅♿🚭🚾🅿🚰🚹🚺🚼🚻🚮🎦📶🈁🆖🆗🆙🆒🆕🆓0⃣1⃣2⃣3⃣4⃣5⃣6⃣7⃣8⃣9⃣🔟🔢▶⏸⏯⏹⏺⏭⏮⏩⏪🔀🔁🔂◀🔼🔽⏫⏬➡⬅⬆⬇↗↘↙↖↕↔🔄↪↩⤴⤵#⃣*⃣ℹ🔤🔡🔠🔣🎵🎶〰➰✔🔃➕➖➗✖💲💱©®™🔚🔙🔛🔝🔜☑🔘⚪⚫🔴🔵🔸🔹🔶🔷🔺▪▫⬛⬜🔻◼◻◾◽🔲🔳🔈🔉🔊🔇📣📢🔔🔕🃏🀄♠♣♥♦🎴👁‍🗨💭🗯💬🕐🕑🕒🕓🕔🕕🕖🕗🕘🕙🕚🕛🕜🕝🕞🕟🕠🕡🕢🕣🕤🕥🕦🕧🇦🇺🇦🇹🇧🇾🇧🇪🇧🇷🇨🇦🇨🇱🇨🇳🇨🇴🇩🇰🇫🇮🇫🇷🇩🇪🇭🇰🇮🇳🇮🇩🇮🇪🇮🇱🇮🇹🇯🇵🇰🇿🇲🇴🇲🇾🇲🇽🇳🇱🇳🇿🇳🇴🇵🇭🇵🇱🇵🇹🇵🇷🇷🇺🇸🇦🇸🇬🇿🇦🇰🇷🇪🇸🇸🇪🇨🇭🇹🇷🇺🇦🇦🇪🇬🇧🇺🇸🇻🇳");
	std::vector<uint8_t> ans2;
	std::vector<uint32_t> symba{50000, 60000, 70000, 80000, 900000, 4294967295, 0, 4294000000, 0};
	ans2 = LSCL::uint32_vector_to_utf8_vector(symba);
	std::vector<uint32_t> kimba = LSCL::utf8_vector_to_uint32_vector(ans2);
	REQUIRE(symba == kimba);
}

TEST_CASE( "Stream", "[nodebuilder]" )
{
	std::string ts(
		"В процессе курса на многочисленных примерах дается представление "
		"о многообразии строительных конструкций как в отношении материалов, "
		"так и конструктивных схем. Анализируются основные нагрузки и воздействия "
		"на строительные конструкции. Основной упор курса делается на "
		"расчет таких конструктивных элементов, как балка, колонна, плита, стена, "
		"арка и нить; причем выполненных из разных материалов – металла (стали), "
		"дерева, камня (кирпича), железобетона. Далее дается представление "
		"о конструировании соединений элементов друг с другом. "
		"После чего показаны принципы формирования строительных конструкций в целом, "
		"в том числе большепролетных, пространственных и высотных. После анализа "
		"работы наземных конструкций дается представление о расчете фундаментов "
		"и грунтов оснований. Также ведется оценка ограждающих функций "
		"конструкций (тепло и звукоизоляция). "
	);
	std::stringstream ss(ts);
	LSCL::Nodebuilder::Stream lscl_stream_1(ss);
	for (auto e: ts)
	{
		char c = lscl_stream_1.peek_next_char();
		lscl_stream_1.eat_next_char();
		REQUIRE(c == e);
	}
	REQUIRE(lscl_stream_1.peek_next_char() == '\0');
	ss.str(ts);
	LSCL::Nodebuilder::Stream lscl_stream_2(ss);
	for (auto e: ts)
	{
		char c = lscl_stream_2.pop_next_char();
		REQUIRE(c == e);
	}
	REQUIRE(lscl_stream_2.pop_next_char() == '\0');
}

TEST_CASE( "Scalar node properties", "[nodes]" )
{
	LSCL::Node_internal *root = new LSCL::Node_internal(LSCL::NODETYPE_NONE);
	REQUIRE(root->type == LSCL::NODETYPE_NONE);
	REQUIRE(root->is(LSCL::NODETYPE_NONE));
	REQUIRE(root->parent == nullptr);
	LSCL::Node_internal scalar(LSCL::NODETYPE_SCALAR, root);
	REQUIRE(scalar.parent == root);
	REQUIRE(scalar.type == LSCL::NODETYPE_SCALAR);
	REQUIRE(scalar.is(LSCL::NODETYPE_SCALAR));
	LSCL::Node_internal *scalar_ptr = &scalar;
	REQUIRE(scalar_ptr->type == LSCL::NODETYPE_SCALAR);
	REQUIRE(scalar_ptr->is(LSCL::NODETYPE_SCALAR));
}

TEST_CASE( "[] access properties", "[nodes]" )
{
	/*
	LSCL::Node_internal n1(nullptr, "value_1");
	LSCL::Node_internal list(LSCL::NODETYPE_LIST);
	for (int i = 0; i < 10; ++i)
	{
		list.insert("num_" + std::to_string(i));
	}
	list.insert(n1, 3);
	
	REQUIRE(n1.size() == 0);
	*/
}
