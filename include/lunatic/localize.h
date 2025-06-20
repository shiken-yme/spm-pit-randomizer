#pragma once
#include <common.h>
#include <spm/msgdrv.h>
#include <spm/npcdrv.h>
#include <spm/pausewin.h>
#include <spm/spmario.h>
#include <spm/search.h>
#include <wii/sc.h>
#include <msl/stdio.h>
#include <msl/string.h>

namespace mod
{
    enum Tribe2Tattle_Types
    {
        /* 0x0 */ TATTLE,
        /* 0x1 */ CARD_NAME,
        /* 0x2 */ CARD_DESC
    };

    char cardNameBuffer[64];

    s32 shadooHealth = 0;

    // Custom Selects, Difficulty Options, and RFC
    const char difficultyOptions[] =
        "<select 1 -1 160 45>\n"
        "Easy\n"
        "Normal\n"
        "Hard";

    const char difficultyText[] =
        "<system>\n"
        "Choose your difficulty.\n"
        "<o>";

    const char musicSignRFText[] =
        "<system>\n"
        "You can choose new music to\n"
        "play in the Pit by reading\n"
        "this sign!\n"
        "<k>";

    const char *vMusicName =
        "Pit of 100 Trials";

    const char *vMusicDesc =
        "Mysterious and entrancing!\n"
        "The classic Pit music.";

    const char *ttMusicName =
        "8-Bit Mix";

    const char *ttMusicDesc =
        "A chiptune jam! Composed by\n"
        "Tater-Tot Tunes.";

    const char *plMusicName =
        "Synth Mix";

    const char *plMusicDesc =
        "Synthy and orchestral!\n"
        "Composed by Plazzap.";

    const char *noMusicName =
        "No Music";

    const char *noMusicDesc =
        "Play your own music in the\n"
        "background, if you'd like!";

    const char *jdMusicName =
        "Piano Mix";

    const char *jdMusicDesc =
        "Jazzy and emotional! Performed\n"
        "live by Jdaster64.";

    const char *moverFeaturesName =
        "Movers";

    const char *moverFeaturesDesc =
        "These guys can help you skip\n"
        "floors, if you have coin!";

    const char *merlunaFeaturesName =
        "Merluna";

    const char *merlunaFeaturesDesc =
        "The esoteric purveyor of\n"
        "Divine Judgement!";

    const char *marioFeaturesName =
        "Mario";

    const char *marioFeaturesDesc =
        "Our hero, Mario! You wouldn't\n"
        "disable him, right?";

    const char *lockPatchesName =
        "Patched Locks";

    const char *lockPatchesDesc =
        "Were you tired of selecting\n"
        "a key every time? Me, too.";

    const char *hpPatchesName =
        "Low HP Sounds";

    const char *hpPatchesDesc =
        "The only sound worse than your\n"
        "Wii crashing on a loading zone.";

    const char *explosionShakeAccessName =
        "Explosions Shake Camera";

    const char *explosionShakeAccessDesc =
        "Toggles camera vibrations when\n"
        "certain enemies explode.";

    const char *selectMusicBox =
        "Select a Tune";

    const char *selectMusicBlueText =
        "Music";

    const char *selectJimboBox =
        "Select your Settings\n"
        "<icon PAD_1 0.67 0 0 0> is Enabled\n"
        "<icon PAD_2 0.67 0 0 0> is Disabled\n";

    const char *selectJimboBlueText =
        "Options";

    const char chestText[] =
        "<system>"
        "Open the chest?\n"
        "(%d percent chance to explode)"
        "<o>";

    // Disorders

    const char disorderIndifferenceItemNotif[] =
        "<system>Your %s has\n"
        "been removed!\n"
        "<k>";

    const char disorderIndifferenceItemNotif2[] =
        "<system>Your %s and\n"
        "%s have been\n"
        "removed!\n"
        "<k>";

    // Shadoo
    const char shadooIntro[] =
        "<housou><wave>Hiiiii, Heroes!</wave><wait 500> Did you have a\n"
        "fun time in this here dungeon?\n"
        "<shake>Hehehehehe!!!\n"
        "</shake><k>\n<p>\n"
        "<dynamic 3>The name is Shadoo!</dynamic><wait 350> I've been\n"
        "trapped here for 2,000 <wave>long,\n"
        "<wait 200>lo<wait 150>o<wait 150>o<wait 150>o<wait 150>o<wait 150>ng years...\n"
        "</wave><k>\n<p>\n"
        "Oh,<wait 350> won't you help me,<wait 200> Heroes?\n"
        "<wait 350>Indeed,<wait 200> allow me to reveal my\n"
        "form to you...\n"
        "<k>";

    const char shadooIntroRamp[] =
        "<housou><wave>Welcome back, Heroes!</wave><wait 500> Looks\n"
        "like you've upped the ante,\n"
        "<wait 200>yes?<wait 400><shake> Hehehehe!!\n"
        "</shake><k>\n<p>\n"
        "<wave>I shall entertain you once\n"
        "more,<wait 200> great Heroes!\n"
        "</wave><wait 500><shake><dynamic 3>Here I cooooome!\n"
        "<k>";

    const char shadooIntroMock[] =
        "<housou><wave>Welcome back, Heroes!</wave><wait 500> Oh my,\n"
        "<wait 200>how interesting...<wait 350> You've\n"
        "returned, <wait 200><wave>but this is no fun!\n"
        "</wave><k>\n<p>\n"
        "You won't be rewarded for\n"
        "defeating me on the same\n"
        "difficulty<wait 100> <shake>twice, </shake><wait 200>you know...\n"
        "</shake><k>\n<p>\n"
        "<wave>Oh,<wait 200> but I shall entertain you\n"
        "nevertheless,<wait 200> great Heroes!\n"
        "<wait 500><shake><dynamic 3>Here I cooooome!\n"
        "<k>";

    const char shadooDoBattle[] =
        "<housou><shake>Let us do battle,<wait 300> accursed\n"
        "Heroes of Light!\n"
        "</shake><k>";

    const char shadooDefeated[] =
        "<housou><wave>Defeated by the prophesied\n"
        "Heroes of Light and the Pixls...\n"
        "<wait 600>How very shamefuuuul...\n"
        "</wave><k>\n<p>\n"
        "<shake>Yet I won't be sealed away by\n"
        "a battle of such low caliber!\n"
        "<wait 500></shake><dynamic 3>Visit me again soon, Heroes!\n"
        "<k>";

    const char tippiDS[] =
        "<fairy>Shadoo...<wait 250> Who are they?\n"
        "<wait 400>Why do they want us to come\n"
        "back here...\n"
        "<k>";

    const char peachDS[] =
        "This Shadoo wants us to come\n"
        "back?<wait 300> How mysterious...<wait 300> Just who\n"
        "are they?\n"
        "<k>";

    const char bowserDS[] =
        "Huh?<wait 300> You want me to come\n"
        "back?<wait 300> Pffft,<wait 100> I'll kick your\n"
        "butt anytime you want!\n"
        "<k>";

    const char luigiDS[] =
        "Come back?<wait 300> Does that mean we'll\n"
        "have to do all 100 floors\n"
        "over again?\n"
        "<k>\n<p>\n"
        "<shake>I don't wanna...\n"
        "</shake><k>";

    const char tippiDSAgain[] =
        "<fairy>Well, we've defeated Shadoo\n"
        "again...<wait 250> What exactly are\n"
        "they?\n"
        "<k>\n<p>\n"
        "It's like there's no end to\n"
        "this...<wait 250> Should we even keep\n"
        "coming back here?\n"
        "<k>";

    const char tippiDS_Sus[] =
        "<fairy>Now that I think about it...\n"
        "<wait 250>Doesn't Shadoo seem weirdly\n"
        "familiar to you?\n"
        "<k>\n<p>\n"
        "Maybe it's the way they talk...\n"
        "<wait 250>Ugh,<wait 100> I just can't remember...\n"
        "<k>";

    // Merluna & blessing/curse text
    const char houraiFirstUse[] =
        "<system>"
        "Hourai Doll has taken effect!\n"
        "HP has been fully restored,\n"
        "but the Blessing has cleared.\n"
        "<k>\n<p>\n"
        "You now have an Affliction:\n"
        "Phoenix's Tail. Attack is\n"
        "halved; HP increased by 10.\n"
        "<k>\n<p>\n"
        "Hourai Doll's effects are no\n"
        "longer active, so be careful!\n"
        "<k>\n<p>\n"
        "These effects will remain\n"
        "until you exit the Pit or\n"
        "the Affliction has cleared.\n"
        "<k>";

    const char houraiWearOff[] =
        "<system>"
        "The temporary HP boost and\n"
        "Attack cut from Phoenix's\n"
        "Tail have now worn off.\n"
        "<k>";

    const char merlunaIntro[] =
        "<shake>Wah-hah-hah!</shake><wait 500> You've found\n"
        "me, the oh-so-wonderful...<wait 250> the\n"
        "dearly accursed... <wait 250><wave>Merluna!</wave>\n"
        "<k>";

    const char merlunaIntro2[] =
        "What am I doing here, you ask\n"
        "me?<wait 250> Well, now, aren't you an\n"
        "<shake>inquisitive</shake> one, indeed...\n"
        "<k>\n<p>\n"
        "If you must know, I am in\n"
        "the business of <wave>distributing\n"
        "the Ancients' unyielding will!\n"
        "</wave>\n<k>\n<p>\n"
        "\"What does she mean, truly?\"<wait 500> \n"
        "Well, if you'd like, I <wave>cooould\n"
        "</wave>show you my great powers...\n"
        "<dkey></dkey><k>\n<p>\n"
        "<wait 1500>...So I will!<wait 500> For a nominal\n"
        "fee, I shall render Divine\n"
        "Judgement upon your person!\n"
        "<k>\n<p>\n"
        "Worry not;<wait 250> quite often, fortune\n"
        "may very well be in your\n"
        "favor...<wait 750> it's all up to chance!\n"
        "<k>\n<p>\n"
        "You have a chance to be\n"
        "<wave>blessed,</wave><wait 500> but you could also\n"
        "be <shake>cursed...\n"
        "</shake><k>\n<p>\n"
        "<wave>But dearest Merluna,<wait 250> how likely\n"
        "is a curse?</wave><wait 400> That depends on\n"
        "your Difficulty setting!\n"
        "</wave><k>\n<p>\n"
        "If you do get cursed,<wait 250> you can\n"
        "always come back to me<wait 100> and\n"
        "I'll make it <wave>allll better...\n"
        "</wave><k>\n<p>\n"
        "You have a %d percent chance\n"
        "to be blessed, and a %d percent\n"
        "chance to be cursed!\n"
        "<k>\n<p>\n"
        "Now, allow me to divine the\n"
        "latest in the Tribe's bidding\n"
        "for you,<wait 500> <wave>o great Hero...\n"
        "</wave><k>";

    const char merlunaIntro3[] =
        "Hah-hah,<wait 300> so you've come back\n"
        "for me, <wave>haven't you, Hero?\n"
        "<wait 700></wave><shake>Well, then...\n"
        "</shake><k>\n<p>\n"
        "You have a %d percent chance\n"
        "to be blessed, and a %d percent\n"
        "chance to be cursed!\n"
        "<k>\n<p>\n"
        "You know how the rest goes,<wait 300> so\n"
        "I shall spare you the lecture\n"
        "and hear the Ancients' will...\n"
        "<k>";

    const char merlunaJudgement[] =
        "The Blessing is %s;\n"
        "<wait 300>the Curse is %s.\n"
        "<wait 700><wave>Just 10 coins...<wait 500> wanna risk it?\n"
        "</wave><o>";

    const char merlunaOptions[] =
        "<select 3 -1 360 55>\n"
        "Let's do it!\n"
        "More about Blessing...\n"
        "More about Curse...\n"
        "No, thanks.";

    const char merlunaDecline[] =
        "<wave>Okay, okay, okayyyy... </wave><wait 600><shake>Well,\n"
        "</shake><wait 300>if you change your mind,\n"
        "<wait 300><wave>I'll always be heeeere!\n"
        "</wave><k>";

    const char merlunaBless[] =
        "<wave>Yay!</wave><wait 300> You've just earned the\n"
        "blessing of the Tribe!\n"
        "<wait 500><wave>Enjoy it while you can!\n"
        "</wave><k>";

    const char merlunaCurse[] =
        "<wave>Oh no!</wave><wait 300> Looks like you weren't\n"
        "so lucky...<wait 300> What a shame...\n"
        "<wait 700><shake>Well, good luck now, Hero!\n"
        "</shake><k>";

    const char merlunaScam[] =
        "<shake><dynamic 3>Ugh!!!</shake><wait 500> Have you no respect\n"
        "for the powers of my tribe!?\n"
        "<wait 500>You can't afford me!\n"
        "</dynamic></shake><k>\n<p>\n"
        "<shake>Honestly, </shake><wait 300> some people just\n"
        "have NO respect for us...\n"
        "<wait 700><dynamic 3><shake>DON'T WASTE MY TIME!\n"
        "</dynamic></shake><k>";

    const char merlunaRelieveCurse[] =
        "<shake>Wah-hah-hah,</shake><wait 300> hello, Hero...\n"
        "<wave><wait 800>So, have you been enjoying\n"
        "yourself? </wave><wait 700><shake>Hehehe...\n"
        "</shake><k>\n<p>\n"
        "Well, <wait 300>that's just how it goes!\n"
        "<wait 700>Sometimes,<wait 300> you get lucky,\n"
        "<wait 300>and other times...<wait 800> <wave>You don't!\n"
        "</wave><k>\n<p>\n"
        "I'm just <shake>sure</shake> you've had enough\n"
        "by now, <wait 300>haven't you? <wait 700><wave>But worry\n"
        "not,<wait 200> I can take care of you...\n"
        "</wave><k>\n<p>\n"
        "<wait 1000>For the measly price of... <wait 500><wave>100\n"
        "coins!</wave><wait 500> Surely you can afford\n"
        "that...<wait 700> <wave>Wanna clear the curse?\n"
        "</wave><o>";

    const char merlunaRelieveOptions[] =
        "<select 0 -1 400 40>\n"
        "Yes, please, ma'am!\n"
        "I rather like this curse.";

    const char merlunaRelieveScam[] =
        "<shake><dynamic 3>Unbelievable!</shake><wait 500> You incurred the\n"
        "wrath of my tribe and didn't\n"
        "even prepare an offering!?\n"
        "</dynamic></shake><k>\n<p>\n"
        "<shake>Honestly, </shake><wait 300> some people just\n"
        "have NO respect for us...\n"
        "<wait 700><dynamic 3><shake>DON'T WASTE MY TIME!\n"
        "</dynamic></shake><k>";

    const char merlunaRelieveDecline[] =
        "<wait 1000><wave>...<wait 1400>Well, aren't you strange?\n"
        "</wave><wait 500>Are you what they call,<wait 200> mm,\n"
        "<wait 200>a <shake>\"masochist?\"\n"
        "</shake><k>\n<p>\n"
        "I must say,<wait 200> I don't <wait 100><shake>quite\n"
        "<wait 100></shake>understand your tastes,<wait 200> <wave>but\n"
        "I hope you have fun!\n"
        "</wave><k>";

    const char merlunaRelieveReal[] =
        "<wave>All done!</wave><wait 500> Your curse has been\n"
        "cleared!<wait 200> <shake>Now...</shake><wait 200> <wave>would you like\n"
        "to retry Divine Judgement?\n"
        "</wave><o>";

    const char merlunaRetryOptions[] =
        "<select 0 -1 400 30>\n"
        "I won't lose twice!\n"
        "Maybe not...";

    const char merlunaRetryAccept[] =
        "Hehehe,<wait 200> well,<wait 200> <wave>aren't you a\n"
        "brave little Hero?</wave><wait 800> So be it...\n"
        "<wait 200><shake>Here we go!\n"
        "</shake><k>";

    const char merlunaRetryDecline[] =
        "<wave>Hehehe, I understand... </wave><wait 600><shake>Well,\n"
        "</shake><wait 300>if you change your mind,\n"
        "<wait 300><wave>I'll always be heeeere!\n"
        "</wave><k>";

    const char merlunaSpectre[] =
        "The Blessing known as Spectre\n"
        "<wait 100>is<wait 100> <wave>oh-so very convenient!\n"
        "</wave><wait 200>You'll <shake>certainly</shake> love it...\n"
        "<k>\n<p>\n"
        "Each enemy you defeat will\n"
        "have a 20 percent chance to\n"
        "heal you a certain amount!\n"
        "<k>\n<p>\n"
        "The HP obtained depends on\n"
        "the enemy's HP as well as\n"
        "your progress in the Pit.\n"
        "<k>\n<p>\n"
        "<wave>Doesn't that sound great?\n"
        "<wait 700>It<wait 100> could<wait 100> be<wait 100> yoooours...\n"
        "<wait 100></wave><k>";

    const char merlunaAster[] =
        "The Curse,<wait 200> Tatarian Aster,\n"
        "<wait 200>is simply <shake>terrible...</shake><wait 700> You\n"
        "certainly don't want it!\n"
        "<k>\n<p>\n"
        "Known to some in my tribe as\n"
        "\"Shion,\"<wait 300> it will ensure you have\n"
        "terrible luck...<wait 300> <shake>FOR LIFE!\n"
        "</shake><k>\n<p>\n"
        "In the Pit,<wait 200> you'll have a 30\n"
        "percent chance to be forced\n"
        "to defeat <dynamic 2>every enemy.\n"
        "</dynamic><k>\n<p>\n"
        "Although tedious early on,<wait 200> it\n"
        "can become dangerous in later\n"
        "floors,<wait 200> <wave>so be careful!\n"
        "</wave><k>\n<p>\n"
        "Of course,<wait 200> if you get cursed,\n"
        "<wait 200>I can always make it better\n"
        "for you...<wait 600> <shake>Hehehe...\n"
        "</shake><k>";

    const char merlunaHex[] =
        "The Curse,<wait 200> Vulnerability Hex,\n"
        "<wait 200>will cause you to take more\n"
        "damage from all sources.\n"
        "<k>\n<p>\n"
        "Historically,<wait 200> this curse was used\n"
        "to weaken our enemies and\n"
        "defeat them swiftly...\n"
        "<k>\n<p>\n"
        "It's not so terrible early on,\n"
        "<wait 200><wave>but you'll take increased\n"
        "damage in later floors...\n"
        "</wave><k>\n<p>\n"
        "You'll take 1 extra damage in\n"
        "Floors 1-50,<wait 200> 2 extra through 75,\n"
        "<wait 200>and 3 extra after that!\n"
        "<k>\n<p>\n"
        "Of course,<wait 200> if you get cursed,\n"
        "<wait 200>I can always make it better\n"
        "for you...<wait 600> <shake>Hehehe...\n"
        "</shake><k>";

    const char merlunaHourai[] =
        "The Blessing,<wait 200> Hourai Doll,<wait 200> allows\n"
        "the user to return from the\n"
        "dead,<wait 200> <wave>fully revitalized!\n"
        "<k>\n<p>\n"
        "Weeellll....</wave><wait 400> maybe that's not\n"
        "entirely true...\n"
        "<k>\n<p>\n"
        "If you run out of HP with this\n"
        "Blessing active,<wait 200> you'll revive\n"
        "at full HP,<wait 200> <wave>and then some!\n"
        "</wave><k>\n<p>\n"
        "After Hourai Doll takes effect,\n"
        "<wait 200>the Blessing will end,<wait 200> and you'll\n"
        "be afflicted by Phoenix's Tail.\n"
        "<k>\n<p>\n"
        "Phoenix's Tail increases your max\n"
        "HP by 10,<wait 200> <shake>but halves your attack\n"
        "power!<wait 400></shake> <wave>How dangerous...\n"
        "</wave><k>\n<p>\n"
        "Do know that I cannot remove\n"
        "Phoenix's Tail...<wait 200> <wave>You'll have to\n"
        "put up with it for 20 floors!\n"
        "</wave><k>\n<p>\n"
        "<wave>Doesn't that sound great?\n"
        "<wait 700>It<wait 100> could<wait 100> be<wait 100> yoooours...\n"
        "<wait 100></wave><k>";

    const char merlunaParamita[] =
        "The Blessing,<wait 200> Paramita,<wait 200> is\n"
        "incredibly powerful,<wait 200> but\n"
        "<wave>oh-so-very costly!\n"
        "</wave><k>\n<p>\n"
        "At will,<wait 200> you can multiply your\n"
        "attack power...<wait 400> but it'll take\n"
        "<shake>20 percent</shake> of your current HP!\n"
        "<k>\n<p>\n"
        "Paramita will multiply attack\n"
        "by two in Floors 1-50,<wait 200> by three\n"
        "in 51-75,<wait 200> and by four in 75-99!\n"
        "<k>\n<p>\n"
        "This buff will last for twenty\n"
        "seconds,<wait 200> with an equally long\n"
        "cooldown after it ends.\n"
        "<k>\n<p>\n"
        "<wave>Doesn't that sound great?\n"
        "<wait 700>It<wait 100> could<wait 100> be<wait 100> yoooours...\n"
        "<wait 100></wave><k>";

    const char merlunaMigraine[] =
        "The Curse,<wait 200> Migraine,<wait 200> is <wave>soooooo\n"
        "very annoying!</wave><wait 400> You'll be\n"
        "frustrated in no time flat!\n"
        "<k>\n<p>\n"
        "When you are hit by an enemy,\n"
        "<wait 200>you will have a 75 percent\n"
        "chance to be <shake>debuffed...\n"
        "</shake><k>\n<p>\n"
        "<wave>You might forget how to walk,\n"
        "or you might forget how to use\n"
        "your abilities...\n"
        "<k>\n<p>\n"
        "...but sometimes,<wait 200> you might\n"
        "become as slow as a snail,<wait 200> or\n"
        "as heavy as a boulder!\n"
        "</wave><k>\n<p>\n"
        "The amount of time the debuff\n"
        "lasts is <shake>random!</shake><wait 300> It can last\n"
        "up to one to ten seconds...\n"
        "<k>\n<p>\n"
        "Of course,<wait 200> if you get cursed,\n"
        "<wait 200>I can always make it better\n"
        "for you...<wait 600> <shake>Hehehe...\n"
        "</shake><k>";

    // Mover
    const char moverIntro[] =
        "Hey.<wait 250> Wassup?<wait 250> I'm a Mover.\n"
        "I know a few things about\n"
        "getting through this dungeon.\n"
        "<k>\n<p>\n"
        "For a few coins, I can move\n"
        "you where you wanna go.\n"
        "<wait 250>Where do you wanna go?\n"
        "<o>";

    const char moverChoicesLv1[] =
        "<select 2 -1 400 40>\n"
        "20 Coins: Down 2 Floors\n"
        "60 Coins: Down 5 Floors\n"
        "Nah, I'm fine.";

    const char moverChoicesLv2[] =
        "<select 2 -1 400 40>\n"
        "25 Coins: Down 2 Floors\n"
        "75 Coins: Down 5 Floors\n"
        "Nah, I'm fine.";

    const char moverChoicesLv3[] =
        "<select 2 -1 400 40>\n"
        "30 Coins: Down 2 Floors\n"
        "90 Coins: Down 5 Floors\n"
        "Nah, I'm fine.";

    const char moverChoicesLv4[] =
        "<select 2 -1 400 40>\n"
        "40 Coins: Down 2 Floors\n"
        "120 Coins: Down 5 Floors\n"
        "Nah, I'm fine.";

    const char moverNo[] =
        "Whatever, man. Have it your\n"
        "way. It's not like I do this\n"
        "FOR A LIVING or anything!\n"
        "<k>";

    const char moverScam[] =
        "<wave>Maaan,</wave> you don't have the cash\n"
        "money! How about you talk\n"
        "to me when you can PAY UP?\n"
        "<k>";

    const char moverYes[] =
        "<wave>Alriiiight!</wave> Appreciate the\n"
        "business, man. Let's getcha\n"
        "outta here. <wave>Close your eyes!</wave>\n"
        "<k>";

    // Quickstart
    const char quickstartOptions[] =
        "<select 0 -1 360 40>\n"
        "New Save\n"
        "Start from 2-1\n"
        "Start from 6-1\n"
        "Start from Postgame";

    const char quickstartText[] =
        "<system>"
        "Do you want to quickstart\n"
        "the Pit Randomizer or\n"
        "create a new save file?\n"
        "<o>";

    // Whacka

    const char whackaHi[] =
        "Whaaack-hoo! <wait 200>I'm Whacka!\n"
        "<wait 250>Didn't expect to see someone\n"
        "else down here, <wait 200>whack-a-do!\n"
        "<k>\n<p>\n"
        "It's soooo quiet in this room,\n"
        "<wait 200><wave>whack-a-hooo!</wave><wait 300> And this\n"
        "is MY spot, whack-hoo!\n"
        "<k>\n<p>\n"
        "It suuuuure looked dangerous\n"
        "in those other rooms!<wait 200> You\n"
        "should be careful, whack-roo!\n"
        "<k>";

    const char whackaHiBlueLong1[] =
        "Whack-bluuuuue!<wait 250> Check it out!\n"
        "<wait 150>I'm blue now,<wait 100> whack-a-yoooo!\n"
        "<wait 150>Never felt better,<wait 100> whack-roo!\n"
        "<k>";

    const char whackaHiBlueLong2[] =
        "Whack-hoo?<wait 250> Why am I blue?\n"
        "<wait 500>Well.......<wait 1200> <wave>That's a secret,\n"
        "<wait 300>whack-a-bluuuuuuuue!\n"
        "</wave><k>\n<p>\n"
        "I bet you want to be blue,\n"
        "<wait 100>too,<wait 100> don't you?<wait 300> I can help,\n"
        "<wait 200>whack-a-yooo!\n"
        "<k>\n<p>\n"
        "But you have to do me a big\n"
        "favor,<wait 100> yep,<wait 100> whack-a-hoo!\n"
        "<k>\n<wait 500><p>\n"
        "Give me my bump back!<wait 250> Yeah,\n"
        "<wait 100>the one you whacked right\n"
        "out of me,<wait 200> whack-a-do!\n"
        "<k>\n<p>\n"
        "If you give it back to me,\n"
        "<wait 200>you can be blue!<wait 250> <wave>That's a\n"
        "promise,<wait 150> whack-bluuuue!\n"
        "</wave><k>";

    const char whackaHmph[] =
        "You don't wanna give me my\n"
        "bump back?<wait 250> Hmph, fine!<wait 250> <wave>No\n"
        "blue for you,<wait 100> whackety-hoo!\n"
        "</wave><k>";

    const char whackaThanks[] =
        "<wave>Yay!</wave><wait 250> Thanks for giving me\n"
        "my bump back,<wait 100> whackety-woo!\n"
        "<wait 250><wave>I'm so happy,<wait 100> whack-a-rooo!\n"
        "</wave><k>\n<p>\n"
        "As promised,<wait 100> I'll help make\n"
        "you blue,<wait 100> but you'll have to\n"
        "wait,<wait 100> whackety-hoo!\n"
        "<k>\n<p>\n"
        "Give me about 15 seconds to\n"
        "make your present!<wait 250> Be right\n"
        "back,<wait 100> whackety-yack-a-dooo!\n"
        "<k>";

    const char whackaGiveBump1[] =
        "Hoooo...<wait 250> I'm back!<wait 200> Did you\n"
        "miss me?<wait 200> I hope that wasn't\n"
        "too loud,<wait 100> whack-hoooo...\n"
        "<k>\n<p>\n"
        "Here you go!<wait 250> It's your present,\n"
        "<wait 100>especially from me to you,\n"
        "<wait 100><wave>whack-a-bluuuuue!\n"
        "</wave><k>";

    const char whackaGiveBump2[] =
        "You can use this to become\n"
        "blue anytime!<wait 250> It can suck\n"
        "all the blue back out,<wait 100> too!\n"
        "<k>\n<p>\n"
        "Think of me every time you\n"
        "go blue,<wait 100> whack-a-bluuuue!\n"
        "<wait 250>Stay safe,<wait 100> friend!\n"
        "<k>";

    const char whackaHiBlueShort[] =
        "Give me my bump back!<wait 250> Yeah,\n"
        "<wait 100>the one you whacked right\n"
        "out of me,<wait 200> whack-a-do!\n"
        "<k>\n<p>\n"
        "If you give it back to me,\n"
        "<wait 200>you can be blue!<wait 250> <wave>That's a\n"
        "promise,<wait 150> whack-bluuuue!\n"
        "</wave><k>";

    const char whackaDontHaveBump[] =
        "...You don't have my bump?\n"
        "<wait 200>What did you do with it?\n"
        "<wait 500>You didn't eat it,<wait 100> did you?\n"
        "<k>\n<p>\n"
        "Well,<wait 100> then you don't get to\n"
        "be blue like me!<wait 250> <wave>What a shame,\n"
        "whack-a-bluuuuue!"
        "</wave><k>";

    const char tippiBlueWhacka[] =
        "<fairy>...<wait 500>Weren't you already blue\n"
        "before?<wait 250> How did you become\n"
        "even more blue...?\n"
        "<k>";

    const char peachBlueWhacka[] =
        "I thought you were already\n"
        "blue,<wait 100> though?<wait 250> How'd you get\n"
        "to be more blue?\n"
        "<k>";

    const char bowserBlueWhacka[] =
        "What's your problem,<wait 100> you\n"
        "weird mole?<wait 250> Why are you so\n"
        "darn blue?\n"
        "<k>";

    const char luigiBlueWhacka[] =
        "Weren't you already blue,\n"
        "little Whacka?<wait 250> I hope I'm\n"
        "not misremembering...\n"
        "<k>";

    const char whackaHiBluePost[] =
        "Whack-a-bluuuue! It sure is\n"
        "nice being blue, whack-hoo!\n"
        "<k>\n<p>\n"
        "Do you like being blue?\n"
        "I sure hope so, yahooo!\n"
        "<k>";

    const char whackaOww[] =
        "<wave>Whack-OWWWWWWWWWWW!\n"
        "</wave><wait 250>That really hurt!<wait 200> I'm going\n"
        "somewhere else now...\n"
        "<k>";

    const char whackaCantHurt[] =
        "<wave>Whack-BLUUUUUUUE!\n"
        "</wave><wait 250>You can't hurt me anymore,\n"
        "<wait 200>because I'm blue!<wait 250> Whack-hoo!\n"
        "<k>";

    const char tippiHitWhacka[] =
        "<fairy>I don't think being extra\n"
        "blue necessarily makes one\n"
        "invincible...\n"
        "<k>";

    const char peachHitWhacka[] =
        "Are you sure?<wait 200> I can't imagine\n"
        "being more blue should make\n"
        "you more powerful...\n"
        "<k>";

    const char bowserHitWhacka[] =
        "Being blue doesn't make you\n"
        "invulnerable, you little freak!\n"
        "<wait 250>What's your deal?\n"
        "<k>";

    const char luigiHitWhacka[] =
        "Huh?<wait 250> So being all blue and\n"
        "stuff makes you stronger?\n"
        "<wait 250>I'm not so sure...\n"
        "<k>";

    const char whackaCantHurt2[] =
        "Really?<wait 250> Well, I sure do <wave>feel\n"
        "</wave>a whole lot stronger now,\n"
        "<wait 200>wicky-whackety-whack-a-do!\n"
        "<k>";

    const char *mystBumpDescPtr =
        "msg_blue_bump_desc";

    const char *mystBumpNamePtr =
        "msg_blue_bump_name";

    const char *mystBumpDesc =
        "A weird bump with an odd ability.\n"
        "Given to you by a \"blue\" Whacka.";

    const char *mystBumpName =
        "Mysterious Whacka Bump";

    const char bumpUseMsg[] =
        "<system>"
        "Would you like to be blue?\n"
        "<o>";

    const char bumpUnuseMsg[] =
        "<system>"
        "Would you like to go back to\n"
        "the way things were, back when\n"
        "when we first met each other?\n"
        "<o>";

    const char bumpYoureBlueNow[] =
        "* You're blue now.\n"
        "<k>";

    const char bumpYoureNotBlueNow[] =
        "<system>"
        "* You're no longer blue.\n"
        "<k>";

    // Jimbo
    const char jimboIntro[] =
        "Sup.<wait 300> I'm Jimbo.<wait 200> Nice to meet\n"
        "ya.<wait 300> I sorta guard the entrance\n"
        "to this here Pit.\n"
        "<k>\n<p>\n"
        "Yeah, I'm kind of a big deal.\n"
        "<wait 300>Warnin' the people of the\n"
        "perils that lie in wait...\n"
        "<k>\n<p>\n"
        "Yep, that's just what I do.\n"
        "<wait 300>No one even asks me to do\n"
        "this.<wait 300> I'm just <wave>that cool, yo.\n"
        "</wave><k>\n<p>\n"
        "Anyway, you don't look like\n"
        "just any ordinary citizen, so\n"
        "I can let ya go down there.\n"
        "<k>\n<p>\n"
        "Matter of fact, I'll even help\n"
        "ya out if ya want.<wait 300> Yep, I can\n"
        "do that.\n"
        "<k>\n<p>\n"
        "Well, hey, I won't go IN there\n"
        "with ya...<wait 300> though I would totally\n"
        "survive, yo...\n"
        "<k><wait 1000>\n<p>\n"
        "...<wait 1000>Uh, anyway, I can help ya\n"
        "with a few things. Preferences.\n"
        "Options, if ya will.<wait 300> Yo.\n"
        "<k>\n<p>\n"
        "Y'know what they called me\n"
        "back at Shy High?<wait 300> \"The Options\n"
        "Menu.\"<wait 300> Yeah. That's right, yo.\n"
        "<k>\n<p>\n"
        "I'm not lyin'.<wait 300> Ya went to a\n"
        "different school, so ya wouldn't\n"
        "know.<wait 300> C'mon, seriously, yo.\n"
        "<k>\n<p>\n"
        "I can totally change some parts\n"
        "of your experience in the Pit.\n"
        "<wait 300>Don't ask how.<wait 300> <wave>I just can.\n"
        "</wave><k>\n<p>\n"
        "So, with allat out of the way,\n"
        "what can I do ya for?\n"
        "<o>";

    const char jimboIntro2[] =
        "Sup.<wait 300> What can I do ya for?\n"
        "<o>";

    const char jimboWarn[] =
        "Whoa.<wait 300> Hoooold up.<wait 300> Ya haven't\n"
        "beaten the Pit yet, so I\n"
        "gotta warn ya...\n"
        "<k>\n<p>\n"
        "Openin' this particular menu\n"
        "might spoil the experience for\n"
        "ya.<wait 300> Sure ya wanna proceed?\n"
        "<o>";

    const char jimboAnythingElse[] =
        "Need anything else?\n"
        "<o>";

    const char jimboBye[] =
        "Bet. Catch ya later, yo.\n"
        "<k>";

    const char jimboSettings[] =
        "<select 0 -1 280 40>\n"
        "Features\n"
        "Game Patches\n"
        "Accessibility\n"
        "I'm all good, yo.";

    // Pit Boodin
    const char boodinIntro[] =
        "Yo!<wait 400> Name's Boodin,<wait 200> owner an'\n"
        "operator of Fondest Hopes,\n"
        "<wait 200>your #1 Flopside card shop.\n"
        "<k>\n<p>\n"
        "You got luck!<wait 300> You caught\n"
        "me right in the middle of\n"
        "restockin' my shop.\n"
        "<k>\n<p>\n"
        "Go ahead and take a look\n"
        "at what I got on hand.\n"
        "<wait 200>I'll hook you up.\n"
        "<k>";

    const char boodinItemSelected[] =
        "The %s card?\n"
        "<wait 200>That basically goes for\n"
        "%d coins,<wait 200> buuut...\n"
        "<k>\n<p>\n"
        "I'll cut you a deal.\n"
        "<wait 300><dynamic 3>%d coins.</dynamic><wait 300> That's half price!\n"
        "<wait 200>How 'bout it?\n"
        "<o>";

    const char boodinSelect[] =
        "<select 0 -1 160 40>\n"
        "Sure\n"
        "Nah";

    const char boodinDecline[] =
        "Change your mind?<wait 300> That's\n"
        "chill.<wait 300> Thanks for stoppin'\n"
        "by!<wait 300> Stay safe out there.\n"
        "<k>";

    const char boodinSatisfied[] =
        "Thanks for the buyin'!\n"
        "<wait 300>Stay safe out there.\n"
        "<k>";

    const char boodinWantMore[] =
        "Want some more cards?\n"
        "<o>";

    const char boodinNoSpace[] =
        "Yo, tough luck on that.\n"
        "<wait 200>You can't hold any more\n"
        "Catch Card SPs.\n"
        "<k>\n<p>\n"
        "Maybe you'll catch me at\n"
        "another time. Thanks for\n"
        "stoppin' by!\n"
        "<k>";

    const char boodinClassism[] =
        "Yo, tough luck on that.\n"
        "<wait 200>You can't afford that card.\n"
        "<k>\n<p>\n"
        "Maybe you'll catch me at\n"
        "another time. Thanks for\n"
        "stoppin' by!\n"
        "<k>";

    // Misc
    const char yesNoSelect_NoByDefault[] =
        "<select 1 -1 160 40>\n"
        "Yes\n"
        "No";

    const char yesNoSelect_YesByDefault[] =
        "<select 0 -1 160 40>\n"
        "Yes\n"
        "No";

    const char *mac_kanban_004 =
        "<kanban>\n"
        "<center>Flopside's #1 Useless Pipe</center>\n"
        "<col c00000ff><center>IT'S USELESS! Try it out!</center>\n"
        "</col><center>-Flopside City Hall-</center>\n"
        "<k>";

    const char *D100_entrance_03 =
        "<system>\n"
        "I can't promise this will\n"
        "work correctly, but hey,\n"
        "whatever floats your boat.\n"
        "<o>";

    const char hardDifficultyFirst[] =
        "<system>\n"
        "On Hard Difficulty, shells\n"
        "will no longer deal damage\n"
        "to enemies in the Pit.\n"
        "<k>\n<p>\n"
        "However, shells that are\n"
        "stomped or kicked will be \n"
        "vulnerable to shell damage.\n"
        "<k>\n<p>\n"
        "<wave>G<wait 300>o<wait 300>o<wait 300>o<wait 300>o<wait 300>o<wait 300>o<wait 300>d<wait 600> l<wait 300>u<wait 300>c<wait 300>k<wait 300>!\n"
        "</wave><k>";

    // Tattles and Catch Cards :(

    const char tattle_phantom_mario[] =
        "That's Phantom Mario.<wait 150> This\n"
        "fake Mario lives in the Pit\n"
        "of 100 Trials...\n"
        "<k>\n<p>\n"
        "Max HP is %d. Attack is %d.\n"
        "<wait 150>He has high attack power\n"
        "and jumps and hammers well...\n"
        "<k>\n<p>\n"
        "If you don't plan your stomp,\n"
        "he might hit you with his\n"
        "hammer.\n"
        "<k>\n<p>\n"
        "You share weaknesses, so\n"
        "if you can exploit that, you'll\n"
        "win...\n"
        "<k>\n<p>\n"
        "After all, he's not the real\n"
        "Mario...<wait 150> He's just a bad copy...\n"
        "<k>";

    const char desc_phantom_mario[] =
        "Max HP: ???\n"
        "Attack: %d\n"
        "<k>\n<p>\n"
        "It's Mario's dark-power\n"
        "doppelganger!\n"
        "<k>\n<p>\n"
        "Supposedly, one of the final\n"
        "foes in the Pit of 100 Trials.";

    const char tattle_phantom_peach[] =
        "That's Phantom Peach, a phony\n"
        "Peach that dwells in the\n"
        "Pit of 100 Trials...\n"
        "<k>\n<p>\n"
        "Max HP is %d. Attack is %d.\n"
        "<wait 150>This Peach uses her parasol\n"
        "just like the original.\n"
        "<k>\n<p>\n"
        "And if she uses it in midair,\n"
        "she might mess up the timing\n"
        "of your attack...\n"
        "<k>\n<p>\n"
        "But that might also give you\n"
        "an opportunity to attack...\n"
        "<k>\n<p>\n"
        "After all, she's not the real\n"
        "Peach...<wait 150> She's just a bad\n"
        "copy...\n"
        "<k>";

    const char desc_phantom_peach[] =
        "Max HP: ???\n"
        "Attack: %d\n"
        "<k>\n<p>\n"
        "It's a dark duplicate of Peach!\n"
        "\n"
        "<k>\n<p>\n"
        "Supposedly, one of the final\n"
        "foes in the Pit of 100 Trials.";

    const char tattle_phantom_bowser[] =
        "That's Phantom Bowser. <wait 150>This\n"
        "fake Bowser rules the Pit\n"
        "of 100 Trials...\n"
        "<k>\n<p>\n"
        "Max HP is %d. Attack is %d.\n"
        "Defense is 4. <wait 150>He has spikes,\n"
        "so you can't stomp on him...\n"
        "<k>\n<p>\n"
        "He has high Defense, so you\n"
        "might need to use Boomer and\n"
        "Cudge...\n"
        "<k>\n<p>\n"
        "Watch out for his ferocious\n"
        "fire attack...\n"
        "<k>\n<p>\n"
        "But after all, he's not the\n"
        "real Bowser...<wait 150> He's just a bad\n"
        "copy...\n"
        "<k>";

    const char desc_phantom_bowser[] =
        "Max HP: ???\n"
        "Attack: %d\n"
        "<k>\n<p>\n"
        "It's a dark duplicate of\n"
        "Bowser!\n"
        "<k>\n<p>\n"
        "Supposedly, one of the final\n"
        "foes in the Pit of 100 Trials.";

    const char tattle_phantom_luigi[] =
        "That's Dark Luigi. <wait 150>He lurks in\n"
        "the Pit of 100 Trials...\n"
        "<k>\n<p>\n"
        "Max HP is %d. Attack is %d.\n"
        "<wait 150>He has the same incredible\n"
        "jumping ability as Luigi.\n"
        "<k>\n<p>\n"
        "Watch out for that super\n"
        "jump of his when you go to\n"
        "stomp him...\n"
        "<k>\n<p>\n"
        "But his jump could be an\n"
        "interesting opportunity for an\n"
        "attack as well...\n"
        "<k>\n<p>\n"
        "After all, he's not the real\n"
        "Luigi...<wait 150> He's just a bad\n"
        "copy...\n"
        "<k>";

    const char desc_phantom_luigi[] =
        "Max HP: ???\n"
        "Attack: %d\n"
        "<k>\n<p>\n"
        "It's a dark duplicate of Luigi!\n"
        "\n"
        "<k>\n<p>\n"
        "Supposedly, one of the final\n"
        "foes in the Pit of 100 Trials.";

    const char tattle_hyper_goomba[] =
        "<fairy><keyyon>It's a Hyper Goomba...\n"
        "<wait 250>Max HP is %d. Attack is %d.\n"
        "<k>\n<p>\n"
        "This is placeholder text...\n"
        "<wait 250>The developer doesn't want\n"
        "to write much else...\n"
        "<k>";

    const char desc_hyper_goomba[] =
        "Max HP: %d\n"
        "Attack: %d\n"
        "<k>\n<p>\n"
        "Placeholder alert!\n"
        "The dev is very lazy.";

    const char tattle_spiked_gloomba[] =
        "<fairy><keyyon>It's a Spiked Gloomba...\n"
        "<wait 250>Max HP is %d. Attack is %d.\n"
        "<k>\n<p>\n"
        "This is placeholder text...\n"
        "<wait 250>The developer doesn't want\n"
        "to write much else...\n"
        "<k>";

    const char desc_spiked_gloomba[] =
        "Max HP: %d\n"
        "Attack: %d\n"
        "<k>\n<p>\n"
        "Placeholder alert!\n"
        "The dev is very lazy.";

    const char tattle_spiked_hyper_goomba[] =
        "<fairy><keyyon>It's a Spiked Hyper Goomba...\n"
        "<wait 250>Max HP is %d. Attack is %d.\n"
        "<k>\n<p>\n"
        "This is placeholder text...\n"
        "<wait 250>The developer doesn't want\n"
        "to write much else...\n"
        "<k>";

    const char desc_spiked_hyper_goomba[] =
        "Max HP: %d\n"
        "Attack: %d\n"
        "<k>\n<p>\n"
        "Placeholder alert!\n"
        "The dev is very lazy.";

    const char tattle_hyper_paragoomba[] =
        "<fairy><keyyon>It's a Hyper Paragoomba...\n"
        "<wait 250>Max HP is %d. Attack is %d.\n"
        "<k>\n<p>\n"
        "This is placeholder text...\n"
        "<wait 250>The developer doesn't want\n"
        "to write much else...\n"
        "<k>";

    const char desc_hyper_paragoomba[] =
        "Max HP: %d\n"
        "Attack: %d\n"
        "<k>\n<p>\n"
        "Placeholder alert!\n"
        "The dev is very lazy.";

    const char tattle_kamikaze_goomba[] =
        "<fairy><keyyon>It's a Kamikaze Goomba...\n"
        "<wait 250>Max HP is %d. Attack is %d.\n"
        "<k>\n<p>\n"
        "This is placeholder text...\n"
        "<wait 250>The developer doesn't want\n"
        "to write much else...\n"
        "<k>";

    const char desc_kamikaze_goomba[] =
        "Max HP: %d\n"
        "Attack: %d\n"
        "<k>\n<p>\n"
        "Placeholder alert!\n"
        "The dev is very lazy.";

    const char tattle_dark_koopa[] =
        "<fairy><keyyon>It's a Dark Koopa...\n"
        "<wait 250>Max HP is %d. Attack is %d.\n"
        "<k>\n<p>\n"
        "This is placeholder text...\n"
        "<wait 250>The developer doesn't want\n"
        "to write much else...\n"
        "<k>";

    const char desc_dark_koopa[] =
        "Max HP: %d\n"
        "Attack: %d\n"
        "<k>\n<p>\n"
        "Placeholder alert!\n"
        "The dev is very lazy.";

    const char tattle_dark_koopatrol[] =
        "<fairy><keyyon>It's a Dark Koopatrol...\n"
        "<wait 250>Max HP is %d. Attack is %d.\n"
        "<k>\n<p>\n"
        "This is placeholder text...\n"
        "<wait 250>The developer doesn't want\n"
        "to write much else...\n"
        "<k>";

    const char desc_dark_koopatrol[] =
        "Max HP: %d\n"
        "Attack: %d\n"
        "<k>\n<p>\n"
        "Placeholder alert!\n"
        "The dev is very lazy.";

    const char tattle_dark_paratroopa[] =
        "<fairy><keyyon>It's a Dark Paratroopa...\n"
        "<wait 250>Max HP is %d. Attack is %d.\n"
        "<k>\n<p>\n"
        "This is placeholder text...\n"
        "<wait 250>The developer doesn't want\n"
        "to write much else...\n"
        "<k>";

    const char desc_dark_paratroopa[] =
        "Max HP: %d\n"
        "Attack: %d\n"
        "<k>\n<p>\n"
        "Placeholder alert!\n"
        "The dev is very lazy.";

    const char tattle_red_spike_top[] =
        "<fairy><keyyon>It's a Red Spike Top...\n"
        "<wait 250>Max HP is %d. Attack is %d.\n"
        "<k>\n<p>\n"
        "This is placeholder text...\n"
        "<wait 250>The developer doesn't want\n"
        "to write much else...\n"
        "<k>";

    const char desc_red_spike_top[] =
        "Max HP: %d\n"
        "Attack: %d\n"
        "<k>\n<p>\n"
        "Placeholder alert!\n"
        "The dev is very lazy.";

    const char tattle_sky_blue_spiny[] =
        "<fairy><keyyon>It's a Sky-Blue Spiny...\n"
        "<wait 250>Max HP is %d. Attack is %d.\n"
        "<k>\n<p>\n"
        "This is placeholder text...\n"
        "<wait 250>The developer doesn't want\n"
        "to write much else...\n"
        "<k>";

    const char desc_sky_blue_spiny[] =
        "Max HP: %d\n"
        "Attack: %d\n"
        "<k>\n<p>\n"
        "Placeholder alert!\n"
        "The dev is very lazy.";

    const char tattle_dark_bones[] =
        "<fairy><keyyon>It's a Dark Bones...\n"
        "<wait 250>Max HP is %d. Attack is %d.\n"
        "<k>\n<p>\n"
        "This is placeholder text...\n"
        "<wait 250>The developer doesn't want\n"
        "to write much else...\n"
        "<k>";

    const char desc_dark_bones[] =
        "Max HP: %d\n"
        "Attack: %d\n"
        "<k>\n<p>\n"
        "Placeholder alert!\n"
        "The dev is very lazy.";

    const char tattle_shady_hammer_bro[] =
        "<fairy><keyyon>It's a Shady Hammer Bro...\n"
        "<wait 250>Max HP is %d. Attack is %d.\n"
        "<k>\n<p>\n"
        "This is placeholder text...\n"
        "<wait 250>The developer doesn't want\n"
        "to write much else...\n"
        "<k>";

    const char desc_shady_hammer_bro[] =
        "Max HP: %d\n"
        "Attack: %d\n"
        "<k>\n<p>\n"
        "Placeholder alert!\n"
        "The dev is very lazy.";

    const char tattle_shady_boomerang_bro[] =
        "<fairy><keyyon>It's a Shady Boomerang Bro...\n"
        "<wait 250>Max HP is %d. Attack is %d.\n"
        "<k>\n<p>\n"
        "This is placeholder text...\n"
        "<wait 250>The developer doesn't want\n"
        "to write much else...\n"
        "<k>";

    const char desc_shady_boomerang_bro[] =
        "Max HP: %d\n"
        "Attack: %d\n"
        "<k>\n<p>\n"
        "Placeholder alert!\n"
        "The dev is very lazy.";

    const char tattle_ice_bro[] =
        "<fairy><keyyon>It's an Ice Bro...\n"
        "<wait 250>Max HP is %d. Attack is %d.\n"
        "<k>\n<p>\n"
        "This is placeholder text...\n"
        "<wait 250>The developer doesn't want\n"
        "to write much else...\n"
        "<k>";

    const char desc_ice_bro[] =
        "Max HP: %d\n"
        "Attack: %d\n"
        "<k>\n<p>\n"
        "Placeholder alert!\n"
        "The dev is very lazy.";

    const char tattle_shady_magikoopa[] =
        "<fairy><keyyon>It's a Shady Magikoopa...\n"
        "<wait 250>Max HP is %d. Attack is %d.\n"
        "<k>\n<p>\n"
        "This is placeholder text...\n"
        "<wait 250>The developer doesn't want\n"
        "to write much else...\n"
        "<k>";

    const char desc_shady_magikoopa[] =
        "Max HP: %d\n"
        "Attack: %d\n"
        "<k>\n<p>\n"
        "Placeholder alert!\n"
        "The dev is very lazy.";

    const char tattle_shady_striker[] =
        "<fairy><keyyon>It's a Shady Striker...\n"
        "<wait 250>Max HP is %d. Attack is %d.\n"
        "<k>\n<p>\n"
        "This is placeholder text...\n"
        "<wait 250>The developer doesn't want\n"
        "to write much else...\n"
        "<k>";

    const char desc_shady_striker[] =
        "Max HP: %d\n"
        "Attack: %d\n"
        "<k>\n<p>\n"
        "Placeholder alert!\n"
        "The dev is very lazy.";

    const char tattle_bomb_boo[] =
        "<fairy><keyyon>It's a Bomb Boo...\n"
        "<wait 250>Max HP is %d. Attack is %d.\n"
        "<k>\n<p>\n"
        "This is placeholder text...\n"
        "<wait 250>The developer doesn't want\n"
        "to write much else...\n"
        "<k>";

    const char desc_bomb_boo[] =
        "Max HP: %d\n"
        "Attack: %d\n"
        "<k>\n<p>\n"
        "Placeholder alert!\n"
        "The dev is very lazy.";

    const char tattle_white_clubba[] =
        "<fairy><keyyon>It's a White Clubba...\n"
        "<wait 250>Max HP is %d. Attack is %d.\n"
        "<k>\n<p>\n"
        "This is placeholder text...\n"
        "<wait 250>The developer doesn't want\n"
        "to write much else...\n"
        "<k>";

    const char desc_white_clubba[] =
        "Max HP: %d\n"
        "Attack: %d\n"
        "<k>\n<p>\n"
        "Placeholder alert!\n"
        "The dev is very lazy.";

    const char tattle_green_fuzzy[] =
        "<fairy><keyyon>It's a Green Fuzzy...\n"
        "<wait 250>Max HP is %d. Attack is %d.\n"
        "<k>\n<p>\n"
        "This is placeholder text...\n"
        "<wait 250>The developer doesn't want\n"
        "to write much else...\n"
        "<k>";

    const char desc_green_fuzzy[] =
        "Max HP: %d\n"
        "Attack: %d\n"
        "<k>\n<p>\n"
        "Placeholder alert!\n"
        "The dev is very lazy.";

    const char tattle_hyper_cleft[] =
        "<fairy><keyyon>It's a Hyper Cleft...\n"
        "<wait 250>Max HP is %d. Attack is %d.\n"
        "<k>\n<p>\n"
        "This is placeholder text...\n"
        "<wait 250>The developer doesn't want\n"
        "to write much else...\n"
        "<k>";

    const char desc_hyper_cleft[] =
        "Max HP: %d\n"
        "Attack: %d\n"
        "<k>\n<p>\n"
        "Placeholder alert!\n"
        "The dev is very lazy.";

    const char tattle_dark_puff[] =
        "<fairy><keyyon>It's a Dark Puff...\n"
        "<wait 250>Max HP is %d. Attack is %d.\n"
        "<k>\n<p>\n"
        "This is placeholder text...\n"
        "<wait 250>The developer doesn't want\n"
        "to write much else...\n"
        "<k>";

    const char desc_dark_puff[] =
        "Max HP: %d\n"
        "Attack: %d\n"
        "<k>\n<p>\n"
        "Placeholder alert!\n"
        "The dev is very lazy.";

    const char tattle_tileoid_pu[] =
        "<fairy><keyyon>It's a Tileoid PU...\n"
        "<wait 250>Max HP is %d. Attack is %d.\n"
        "<k>\n<p>\n"
        "This is placeholder text...\n"
        "<wait 250>The developer doesn't want\n"
        "to write much else...\n"
        "<k>";

    const char desc_tileoid_pu[] =
        "Max HP: %d\n"
        "Attack: %d\n"
        "<k>\n<p>\n"
        "Placeholder alert!\n"
        "The dev is very lazy.";

    const char tattle_bawbus[] =
        "<fairy><keyyon>It's a Bawbus...\n"
        "<wait 250>Max HP is %d. Attack is %d.\n"
        "<k>\n<p>\n"
        "This is placeholder text...\n"
        "<wait 250>The developer doesn't want\n"
        "to write much else...\n"
        "<k>";

    const char desc_bawbus[] =
        "Max HP: %d\n"
        "Attack: %d\n"
        "<k>\n<p>\n"
        "Placeholder alert!\n"
        "The dev is very lazy.";

    const char tattle_ninjeremiah[] =
        "<fairy><keyyon>It's a Ninjeremiah...\n"
        "<wait 250>Max HP is %d. Attack is %d.\n"
        "<k>\n<p>\n"
        "This is placeholder text...\n"
        "<wait 250>The developer doesn't want\n"
        "to write much else...\n"
        "<k>";

    const char desc_ninjeremiah[] =
        "Max HP: %d\n"
        "Attack: %d\n"
        "<k>\n<p>\n"
        "Placeholder alert!\n"
        "The dev is very lazy.";

    const char tattle_skellobyte[] =
        "<fairy><keyyon>It's a Skellobyte...\n"
        "<wait 250>Max HP is %d. Attack is %d.\n"
        "<k>\n<p>\n"
        "This is placeholder text...\n"
        "<wait 250>The developer doesn't want\n"
        "to write much else...\n"
        "<k>";

    const char desc_skellobyte[] =
        "Max HP: %d\n"
        "Attack: %d\n"
        "<k>\n<p>\n"
        "Placeholder alert!\n"
        "The dev is very lazy.";

    const char tattle_spiky_skellobyte[] =
        "<fairy><keyyon>It's a Spiky Skellobyte...\n"
        "<wait 250>Max HP is %d. Attack is %d.\n"
        "<k>\n<p>\n"
        "This is placeholder text...\n"
        "<wait 250>The developer doesn't want\n"
        "to write much else...\n"
        "<k>";

    const char desc_spiky_skellobyte[] =
        "Max HP: %d\n"
        "Attack: %d\n"
        "<k>\n<p>\n"
        "Placeholder alert!\n"
        "The dev is very lazy.";

    const char tattle_ash_cherbil[] =
        "<fairy><keyyon>It's an Ash Cherbil...\n"
        "<wait 250>Max HP is %d. Attack is %d.\n"
        "<k>\n<p>\n"
        "This is placeholder text...\n"
        "<wait 250>The developer doesn't want\n"
        "to write much else...\n"
        "<k>";

    const char desc_ash_cherbil[] =
        "Max HP: %d\n"
        "Attack: %d\n"
        "<k>\n<p>\n"
        "Placeholder alert!\n"
        "The dev is very lazy.";

    const char tattle_shady_koopa[] =
        "<fairy><keyyon>It's a Shady Koopa...\n"
        "<wait 250>Max HP is %d. Attack is %d.\n"
        "<k>\n<p>\n"
        "This is placeholder text...\n"
        "<wait 250>The developer doesn't want\n"
        "to write much else...\n"
        "<k>";

    const char desc_shady_koopa[] =
        "Max HP: %d\n"
        "Attack: %d\n"
        "<k>\n<p>\n"
        "Placeholder alert!\n"
        "The dev is very lazy.";

    const char tattle_flip_shady_koopa[] =
        "<fairy><keyyon>It's a Shady Koopa...\n"
        "<wait 250>Max HP is %d. Attack is %d.\n"
        "<k>\n<p>\n"
        "This is placeholder text...\n"
        "<wait 250>The developer doesn't want\n"
        "to write much else...\n"
        "<k>";

    const char desc_flip_shady_koopa[] =
        "Max HP: %d\n"
        "Attack: %d\n"
        "<k>\n<p>\n"
        "Placeholder alert!\n"
        "The dev is very lazy.";

    const char tattle_flip_buzzy_beetle[] =
        "<fairy><keyyon>It's a Buzzy Beetle...\n"
        "<wait 250>Max HP is %d. Attack is %d.\n"
        "<k>\n<p>\n"
        "This is placeholder text...\n"
        "<wait 250>The developer doesn't want\n"
        "to write much else...\n"
        "<k>";

    const char desc_flip_buzzy_beetle[] =
        "Max HP: %d\n"
        "Attack: %d\n"
        "<k>\n<p>\n"
        "Placeholder alert!\n"
        "The dev is very lazy.";

    const char tattle_flip_spike_top[] =
        "<fairy><keyyon>It's a Spike Top...\n"
        "<wait 250>Max HP is %d. Attack is %d.\n"
        "<k>\n<p>\n"
        "This is placeholder text...\n"
        "<wait 250>The developer doesn't want\n"
        "to write much else...\n"
        "<k>";

    const char desc_flip_spike_top[] =
        "Max HP: %d\n"
        "Attack: %d\n"
        "<k>\n<p>\n"
        "Placeholder alert!\n"
        "The dev is very lazy.";

    const char tattle_green_magikoopa[] =
        "<fairy><keyyon>It's a Green Magikoopa...\n"
        "<wait 250>Max HP is %d. Attack is %d.\n"
        "<k>\n<p>\n"
        "This is placeholder text...\n"
        "<wait 250>The developer doesn't want\n"
        "to write much else...\n"
        "<k>";

    const char desc_green_magikoopa[] =
        "Max HP: %d\n"
        "Attack: %d\n"
        "<k>\n<p>\n"
        "Placeholder alert!\n"
        "The dev is very lazy.";

    const char tattle_white_magikoopa[] =
        "<fairy><keyyon>It's a White Magikoopa...\n"
        "<wait 250>Max HP is %d. Attack is %d.\n"
        "<k>\n<p>\n"
        "This is placeholder text...\n"
        "<wait 250>The developer doesn't want\n"
        "to write much else...\n"
        "<k>";

    const char desc_white_magikoopa[] =
        "Max HP: %d\n"
        "Attack: %d\n"
        "<k>\n<p>\n"
        "Placeholder alert!\n"
        "The dev is very lazy.";

    const char tattle_red_magikoopa[] =
        "<fairy><keyyon>It's a Red Magikoopa...\n"
        "<wait 250>Max HP is %d. Attack is %d.\n"
        "<k>\n<p>\n"
        "This is placeholder text...\n"
        "<wait 250>The developer doesn't want\n"
        "to write much else...\n"
        "<k>";

    const char desc_red_magikoopa[] =
        "Max HP: %d\n"
        "Attack: %d\n"
        "<k>\n<p>\n"
        "Placeholder alert!\n"
        "The dev is very lazy.";

    const char tattle_spinia[] =
        "<fairy><keyyon>It's a Spinia...\n"
        "<wait 250>Max HP is %d. Attack is %d.\n"
        "<k>\n<p>\n"
        "This is placeholder text...\n"
        "<wait 250>The developer doesn't want\n"
        "to write much else...\n"
        "<k>";

    const char desc_spinia[] =
        "Max HP: %d\n"
        "Attack: %d\n"
        "<k>\n<p>\n"
        "Placeholder alert!\n"
        "The dev is very lazy.";

    const char tattle_spunia[] =
        "<fairy><keyyon>It's a Spunia...\n"
        "<wait 250>Max HP is %d. Attack is %d.\n"
        "<k>\n<p>\n"
        "This is placeholder text...\n"
        "<wait 250>The developer doesn't want\n"
        "to write much else...\n"
        "<k>";

    const char desc_spunia[] =
        "Max HP: %d\n"
        "Attack: %d\n"
        "<k>\n<p>\n"
        "Placeholder alert!\n"
        "The dev is very lazy.";

    const char tattle_bleepboxer[] =
        "<fairy><keyyon>It's a Bleepboxer...\n"
        "<wait 250>Max HP is %d. Attack is %d.\n"
        "<k>\n<p>\n"
        "This is placeholder text...\n"
        "<wait 250>The developer doesn't want\n"
        "to write much else...\n"
        "<k>";

    const char desc_bleepboxer[] =
        "Max HP: %d\n"
        "Attack: %d\n"
        "<k>\n<p>\n"
        "Placeholder alert!\n"
        "The dev is very lazy.";

    const char tattle_kilo_muth[] =
        "<fairy><keyyon>It's a Kilo Muth...\n"
        "<wait 250>Max HP is %d. Attack is %d.\n"
        "<k>\n<p>\n"
        "This is placeholder text...\n"
        "<wait 250>The developer doesn't want\n"
        "to write much else...\n"
        "<k>";

    const char desc_kilo_muth[] =
        "Max HP: %d\n"
        "Attack: %d\n"
        "<k>\n<p>\n"
        "Placeholder alert!\n"
        "The dev is very lazy.";

    const char tattle_yellow_shy_guy[] =
        "<fairy><keyyon>It's a Yellow Shy Guy...\n"
        "<wait 250>Max HP is %d. Attack is %d.\n"
        "<k>\n<p>\n"
        "This is placeholder text...\n"
        "<wait 250>The developer doesn't want\n"
        "to write much else...\n"
        "<k>";

    const char desc_yellow_shy_guy[] =
        "Max HP: %d\n"
        "Attack: %d\n"
        "<k>\n<p>\n"
        "Placeholder alert!\n"
        "The dev is very lazy.";

    const char tattle_green_shy_guy[] =
        "<fairy><keyyon>It's a Green Shy Guy...\n"
        "<wait 250>Max HP is %d. Attack is %d.\n"
        "<k>\n<p>\n"
        "This is placeholder text...\n"
        "<wait 250>The developer doesn't want\n"
        "to write much else...\n"
        "<k>";

    const char desc_green_shy_guy[] =
        "Max HP: %d\n"
        "Attack: %d\n"
        "<k>\n<p>\n"
        "Placeholder alert!\n"
        "The dev is very lazy.";

    const char tattle_blue_shy_guy[] =
        "<fairy><keyyon>It's a Blue Shy Guy...\n"
        "<wait 250>Max HP is %d. Attack is %d.\n"
        "<k>\n<p>\n"
        "This is placeholder text...\n"
        "<wait 250>The developer doesn't want\n"
        "to write much else...\n"
        "<k>";

    const char desc_blue_shy_guy[] =
        "Max HP: %d\n"
        "Attack: %d\n"
        "<k>\n<p>\n"
        "Placeholder alert!\n"
        "The dev is very lazy.";

    const char tattle_shy_guy[] =
        "<fairy><keyyon>It's a Shy Guy...\n"
        "<wait 250>Max HP is %d. Attack is %d.\n"
        "<k>\n<p>\n"
        "This is placeholder text...\n"
        "<wait 250>The developer doesn't want\n"
        "to write much else...\n"
        "<k>";

    const char desc_shy_guy[] =
        "Max HP: %d\n"
        "Attack: %d\n"
        "<k>\n<p>\n"
        "Placeholder alert!\n"
        "The dev is very lazy.";

    const char tattle_gold_chomp[] =
        "<fairy><keyyon>It's a Gold Chomp...\n"
        "<wait 250>Max HP is %d. Attack is %d.\n"
        "Defense is 5.\n"
        "<k>\n<p>\n"
        "This is placeholder text...\n"
        "<wait 250>The developer doesn't want\n"
        "to write much else...\n"
        "<k>";

    const char desc_gold_chomp[] =
        "Max HP: %d   Attack: %d\n"
        "Defense: 5\n"
        "<k>\n<p>\n"
        "Placeholder alert!\n"
        "The dev is very lazy.";

    const char tattle_dark_lakitu[] =
        "<fairy><keyyon>It's a Dark Lakitu...\n"
        "<wait 250>Max HP is %d. Attack is %d.\n"
        "<k>\n<p>\n"
        "This is placeholder text...\n"
        "<wait 250>The developer doesn't want\n"
        "to write much else...\n"
        "<k>";

    const char desc_dark_lakitu[] =
        "Max HP: %d\n"
        "Attack: %d\n"
        "<k>\n<p>\n"
        "Placeholder alert!\n"
        "The dev is very lazy.";

    const char tattle_merluna[] =
        "<fairy><keyyon>That's Merluna...<wait 250> She's a\n"
        "mysterious sage that claims to\n"
        "render \"Divine Judgement\"...\n"
        "<k>\n<p>\n"
        "That is, she can bless you or\n"
        "she can curse you...<wait 250> You never\n"
        "know which one it'll be...\n"
        "<k>\n<p>\n"
        "Apparently, she's recorded as\n"
        "having been alive 2000 years\n"
        "ago...\n"
        "<k>\n<p>\n"
        "Has she really been alive for\n"
        "so long...?\n"
        "<k>";

    const char tattle_whacka[] =
        "<fairy><keyyon>That's a Whacka...<wait 250> It's rare to\n"
        "see one...<wait 250> They're a delicate\n"
        "species...\n"
        "<k>\n<p>\n"
        "I'd advise not stomping on\n"
        "it, unless you want to be\n"
        "responsible for its tears...\n"
        "<k>";

    const char tattle_mover[] =
        "<fairy><keyyon>That's a Mover...<wait 250> These guys\n"
        "dwell in the Pit and can help\n"
        "you skip a few floors...\n"
        "<k>\n<p>\n"
        "They can get a little pricey,\n"
        "though...<wait 250> It's up to you if you\n"
        "want to use them...\n"
        "<k>";

    const char tattle_jimbo[] =
        "<fairy><keyyon>That's Jimbo...<wait 250> He's an Anti Guy\n"
        "that guards civilians from\n"
        "the Pit of 100 Trials...\n"
        "<k>\n<p>\n"
        "Well, that's what he claims...\n"
        "<wait 250>Do people even come down\n"
        "here, anyway?\n"
        "<k>\n<p>\n"
        "He's definitely more powerful\n"
        "than he looks, though...<wait 250> It won't\n"
        "hurt to give him a chance...\n"
        "<k>";

    const char *msgSearchTribeToTattle(spm::npcdrv::NPCEntry *npc, s32 tribeId, Tribe2Tattle_Types type)
    {
        spm::npcdrv::NPCTribe *tribe = spm::npcdrv::npcGetTribe(tribeId);
        char tattleBuffer[512];
        char cardDescBuffer[512];
        if (npc != nullptr || npc != 0)
        {
            if (msl::string::strstr(npc->name, "rebear") != nullptr)
            {
                msl::stdio::sprintf(tattleBuffer, tattle_merluna);
            }
            if (msl::string::strstr(npc->name, "dan_card") != nullptr)
            {
                msl::stdio::sprintf(tattleBuffer, spm::msgdrv::msgSearch("mac_19_card"));
            }
            if (msl::string::strstr(npc->name, "mover") != nullptr)
            {
                msl::stdio::sprintf(tattleBuffer, tattle_mover);
            }
            if (msl::string::strstr(npc->name, "dan_koburon") != nullptr)
            {
                msl::stdio::sprintf(tattleBuffer, tattle_whacka);
            }
            if (msl::string::strstr(npc->name, "jimbo") != nullptr)
            {
                msl::stdio::sprintf(tattleBuffer, tattle_jimbo);
            }
        }
        switch (tribeId)
        {
        case 2:
            msl::stdio::sprintf(tattleBuffer, tattle_hyper_goomba, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardDescBuffer, desc_hyper_goomba, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardNameBuffer, "Hyper Goomba");
            break;
        case 5:
            msl::stdio::sprintf(tattleBuffer, tattle_spiked_gloomba, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardDescBuffer, desc_spiked_gloomba, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardNameBuffer, "Spiked Gloomba");
            break;
        case 6:
            msl::stdio::sprintf(tattleBuffer, tattle_spiked_hyper_goomba, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardDescBuffer, desc_spiked_hyper_goomba, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardNameBuffer, "Spiked Hyper Goomba");
            break;
        case 8:
            msl::stdio::sprintf(tattleBuffer, tattle_hyper_paragoomba, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardDescBuffer, desc_hyper_paragoomba, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardNameBuffer, "Hyper Paragoomba");
            break;
        case 10:
            msl::stdio::sprintf(tattleBuffer, tattle_kamikaze_goomba, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardDescBuffer, desc_kamikaze_goomba, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardNameBuffer, "Kamikaze Goomba");
            break;
        case 16:
            msl::stdio::sprintf(tattleBuffer, tattle_dark_koopa, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardDescBuffer, desc_dark_koopa, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardNameBuffer, "Dark Koopa");
            break;
        case 19:
            msl::stdio::sprintf(tattleBuffer, tattle_dark_koopatrol, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardDescBuffer, desc_dark_koopatrol, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardNameBuffer, "Dark Koopatrol");
            break;
        case 24:
            msl::stdio::sprintf(tattleBuffer, tattle_dark_paratroopa, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardDescBuffer, desc_dark_paratroopa, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardNameBuffer, "Dark Paratroopa");
            break;
        case 29:
            msl::stdio::sprintf(tattleBuffer, tattle_red_spike_top, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardDescBuffer, desc_red_spike_top, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardNameBuffer, "Red Spike Top");
            break;
        case 38:
            msl::stdio::sprintf(tattleBuffer, tattle_sky_blue_spiny, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardDescBuffer, desc_sky_blue_spiny, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardNameBuffer, "Sky-Blue Spiny");
            break;
        case 43:
        case 44:
            msl::stdio::sprintf(tattleBuffer, tattle_dark_bones, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardDescBuffer, desc_dark_bones, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardNameBuffer, "Dark Bones");
            break;
        case 49:
        case 50:
            msl::stdio::sprintf(tattleBuffer, tattle_shady_hammer_bro, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardDescBuffer, desc_shady_hammer_bro, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardNameBuffer, "Shady Hammer Bro");
            break;
        case 55:
        case 56:
            msl::stdio::sprintf(tattleBuffer, tattle_shady_boomerang_bro, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardDescBuffer, desc_shady_boomerang_bro, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardNameBuffer, "Shady Boomerang Bro");
            break;
        case 61:
        case 62:
            msl::stdio::sprintf(tattleBuffer, tattle_ice_bro, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardDescBuffer, desc_ice_bro, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardNameBuffer, "Ice Bro");
            break;
        case 66:
        case 67:
        case 68:
            msl::stdio::sprintf(tattleBuffer, tattle_shady_magikoopa, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardDescBuffer, desc_shady_magikoopa, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardNameBuffer, "Shady Magikoopa");
            break;
        case 75:
        case 76:
            msl::stdio::sprintf(tattleBuffer, tattle_shady_striker, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardDescBuffer, desc_shady_striker, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardNameBuffer, "Shady Striker");
            break;
        case 86:
            msl::stdio::sprintf(tattleBuffer, tattle_bomb_boo, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardDescBuffer, desc_bomb_boo, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardNameBuffer, "Bomb Boo");
            break;
        case 95:
            msl::stdio::sprintf(tattleBuffer, tattle_white_clubba, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardDescBuffer, desc_white_clubba, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardNameBuffer, "White Clubba");
            break;
        case 98:
            msl::stdio::sprintf(tattleBuffer, tattle_green_fuzzy, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardDescBuffer, desc_green_fuzzy, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardNameBuffer, "Green Fuzzy");
            break;
        case 101:
            msl::stdio::sprintf(tattleBuffer, tattle_hyper_cleft, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardDescBuffer, desc_hyper_cleft, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardNameBuffer, "Hyper Cleft");
            break;
        case 111:
        case 112:
            msl::stdio::sprintf(tattleBuffer, tattle_dark_puff, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardDescBuffer, desc_dark_puff, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardNameBuffer, "Dark Puff");
            break;
            case 124:
            msl::stdio::sprintf(tattleBuffer, tattle_gold_chomp, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardDescBuffer, desc_gold_chomp, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardNameBuffer, "Gold Chomp");
            break;
        case 167:
            msl::stdio::sprintf(tattleBuffer, tattle_tileoid_pu, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardDescBuffer, desc_tileoid_pu, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardNameBuffer, "Tileoid PU");
            break;
        case 171:
            msl::stdio::sprintf(tattleBuffer, tattle_bawbus, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardDescBuffer, desc_bawbus, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardNameBuffer, "Bawbus");
            break;
        case 220:
        case 221:
        case 222:
            msl::stdio::sprintf(tattleBuffer, tattle_ninjeremiah, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardDescBuffer, desc_ninjeremiah, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardNameBuffer, "Ninjeremiah");
            break;
        case 225:
            msl::stdio::sprintf(tattleBuffer, tattle_skellobyte, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardDescBuffer, desc_skellobyte, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardNameBuffer, "Skellobyte");
            break;
        case 228:
            msl::stdio::sprintf(tattleBuffer, tattle_spiky_skellobyte, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardDescBuffer, desc_spiky_skellobyte, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardNameBuffer, "Spiky Skellobyte");
            break;
        case 330:
            msl::stdio::sprintf(tattleBuffer, tattle_phantom_mario, shadooHealth, tribe->attackStrength);
            msl::stdio::sprintf(cardDescBuffer, desc_phantom_mario, tribe->attackStrength);
            msl::stdio::sprintf(cardNameBuffer, "Phantom Mario");
            break;
        case 332:
            msl::stdio::sprintf(tattleBuffer, tattle_phantom_peach, shadooHealth, tribe->attackStrength);
            msl::stdio::sprintf(cardDescBuffer, desc_phantom_peach, tribe->attackStrength);
            msl::stdio::sprintf(cardNameBuffer, "Phantom Peach");
            break;
        case 333:
            msl::stdio::sprintf(tattleBuffer, tattle_phantom_bowser, shadooHealth, tribe->attackStrength);
            msl::stdio::sprintf(cardDescBuffer, desc_phantom_bowser, tribe->attackStrength);
            msl::stdio::sprintf(cardNameBuffer, "Phantom Bowser");
            break;
        case 331:
            msl::stdio::sprintf(tattleBuffer, tattle_phantom_luigi, shadooHealth, tribe->attackStrength);
            msl::stdio::sprintf(cardDescBuffer, desc_phantom_luigi, tribe->attackStrength);
            msl::stdio::sprintf(cardNameBuffer, "Phantom Luigi");
            break;
        case 446:
            msl::stdio::sprintf(tattleBuffer, tattle_ash_cherbil, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardDescBuffer, desc_ash_cherbil, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardNameBuffer, "Ash Cherbil");
            break;
        case 465:
            msl::stdio::sprintf(tattleBuffer, tattle_shady_koopa, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardDescBuffer, desc_shady_koopa, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardNameBuffer, "Shady Koopa");
            break;
        case 466:
            msl::stdio::sprintf(tattleBuffer, tattle_flip_shady_koopa, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardDescBuffer, desc_flip_shady_koopa, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardNameBuffer, "Shady Koopa");
            break;
        case 470:
            msl::stdio::sprintf(tattleBuffer, tattle_flip_buzzy_beetle, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardDescBuffer, desc_flip_buzzy_beetle, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardNameBuffer, "Buzzy Beetle");
            break;
        case 471:
            msl::stdio::sprintf(tattleBuffer, tattle_flip_spike_top, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardDescBuffer, desc_flip_spike_top, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardNameBuffer, "Spike Top");
            break;
        case 473:
        case 474:
        case 475:
            msl::stdio::sprintf(tattleBuffer, tattle_green_magikoopa, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardDescBuffer, desc_green_magikoopa, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardNameBuffer, "Green Magikoopa");
            break;
        case 476:
        case 477:
        case 478:
            msl::stdio::sprintf(tattleBuffer, tattle_white_magikoopa, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardDescBuffer, desc_white_magikoopa, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardNameBuffer, "White Magikoopa");
            break;
        case 479:
        case 480:
        case 481:
            msl::stdio::sprintf(tattleBuffer, tattle_red_magikoopa, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardDescBuffer, desc_red_magikoopa, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardNameBuffer, "Red Magikoopa");
            break;
        case 495:
            msl::stdio::sprintf(tattleBuffer, tattle_spinia, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardDescBuffer, desc_spinia, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardNameBuffer, "Spinia");
            break;
        case 496:
            msl::stdio::sprintf(tattleBuffer, tattle_spunia, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardDescBuffer, desc_spunia, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardNameBuffer, "Spunia");
            break;
        case 504:
        case 505:
            msl::stdio::sprintf(tattleBuffer, tattle_bleepboxer, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardDescBuffer, desc_bleepboxer, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardNameBuffer, "Bleepboxer");
            break;
        case 506:
            msl::stdio::sprintf(tattleBuffer, tattle_kilo_muth, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardDescBuffer, desc_kilo_muth, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardNameBuffer, "Kilo Muth");
            break;
        case 529:
            msl::stdio::sprintf(tattleBuffer, tattle_yellow_shy_guy, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardDescBuffer, desc_yellow_shy_guy, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardNameBuffer, "Shy Guy");
            break;
        case 530:
            msl::stdio::sprintf(tattleBuffer, tattle_green_shy_guy, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardDescBuffer, desc_green_shy_guy, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardNameBuffer, "Shy Guy");
            break;
        case 531:
            msl::stdio::sprintf(tattleBuffer, tattle_blue_shy_guy, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardDescBuffer, desc_blue_shy_guy, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardNameBuffer, "Shy Guy");
            break;
        case 532:
            msl::stdio::sprintf(tattleBuffer, tattle_shy_guy, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardDescBuffer, desc_shy_guy, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardNameBuffer, "Shy Guy");
            break;
        case 534:
            msl::stdio::sprintf(tattleBuffer, tattle_dark_lakitu, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardDescBuffer, desc_dark_lakitu, tribe->maxHp, tribe->attackStrength);
            msl::stdio::sprintf(cardNameBuffer, "Dark Lakitu");
            break;
        }
        if (type == TATTLE)
        {
            msl::stdio::sprintf(spm::search::search_wp->msgBuf, "<gsearch>\n%s", tattleBuffer);
            const char *tattleMsg = spm::search::search_wp->msgBuf;
            return tattleMsg;
        }
        else if (type == CARD_NAME)
        {
            const char *nameMsg = cardNameBuffer;
            return nameMsg;
        }
        else
        {
            msl::stdio::sprintf(spm::pausewin::pausewinCardDescBuf, cardDescBuffer);
            const char *descMsg = spm::pausewin::pausewinCardDescBuf;
            return descMsg;
        }
    }
}