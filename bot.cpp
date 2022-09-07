#include "bot.hpp"
#include <tgbot/net/TgLongPoll.h>
#include <exception>
#include <string_view>
#include "args_iterator.hpp"

UltimateTagBot::UltimateTagBot(const std::string &tag):
    TgBot::Bot(tag)
{
    getEvents().onAnyMessage([this](TgBot::Message::Ptr message){
        OnMessage(message);
    });
    getEvents().onCommand("add_tag", [this](TgBot::Message::Ptr message){
        OnAddTag(message);
    });
    getEvents().onCommand("remove_tag", [this](TgBot::Message::Ptr message){
        OnRemoveTag(message);
    });

}

int UltimateTagBot::Run(){
    try{
        TgBot::TgLongPoll long_poll(*this);

        while(true){
            long_poll.start();
        }

    }catch(const std::exception &e){
        printf("%s\n", e.what());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

void UltimateTagBot::OnAddTag(TgBot::Message::Ptr message){

}

void UltimateTagBot::OnRemoveTag(TgBot::Message::Ptr message){

}

void UltimateTagBot::OnMessage(TgBot::Message::Ptr message){

    std::string reply;

    for(ArgsIterator it(message->text.c_str()); it; ++it){
        reply += it.Current() + ", ";
    }

    getApi().sendMessage(message->chat->id, reply);
}