#include <iostream>
#include <tgbot/Api.h>
#include <tgbot/Bot.h>
#include <tgbot/net/TgLongPoll.h>
#include <tgbot/TgException.h>

using namespace TgBot;

int main(int argc, char **argv){
    if(argc != 2)
        return EXIT_FAILURE;

    Bot bot(argv[1]);

    printf("TOKEN: %s\n", bot.getToken().c_str());

    bot.getEvents().onCommand("start", [&bot](TgBot::Message::Ptr message) {
        bot.getApi().sendMessage(message->chat->id, "Hi!");
    });
    bot.getEvents().onAnyMessage([&bot](TgBot::Message::Ptr message) {
        printf("User wrote %s\n", message->text.c_str());
        if (StringTools::startsWith(message->text, "/start")) {
            return;
        }
        bot.getApi().sendMessage(message->chat->id, "Your message is: " + message->text);

        if(message->text == "@all"){
            bot.getApi().sendMessage(message->chat->id, "@bloodredtape");
        }
    });

    try {
        printf("Bot username: %s\n", bot.getApi().getMe()->username.c_str());
        TgBot::TgLongPoll longPoll(bot);
        while (true) {
            printf("Long poll started\n");
            longPoll.start();
        }
    } catch (TgBot::TgException& e) {
        printf("error: %s\n", e.what());
    }

    return puts("Hello UTB");
}