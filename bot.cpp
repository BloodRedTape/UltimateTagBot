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
    getEvents().onCommand("list_tags", [this](TgBot::Message::Ptr message){
        OnListTags(message);
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
    ArgsIterator it(message->text.c_str());
    it.Advance();
    if(!it)
        return Error(message->chat->id, "Invalid command arguments");

    std::string keytag = it.Current();
    TagList tags;
    for(it.Advance();it; ++it)
        tags.push_back(it.Current());

    if(!tags.size())
        return Error(message->chat->id, "Zero tags supplied");

    m_DB.AddTagsFor(message->chat->id, keytag, tags);
}

void UltimateTagBot::OnRemoveTag(TgBot::Message::Ptr message){

}

void UltimateTagBot::OnMessage(TgBot::Message::Ptr message){
    puts("message");

    for(ArgsIterator it(message->text.c_str()); it; ++it){
        std::string arg = it.Current();

        if(arg[0] != '@')
            continue;

        std::string keytag(arg.c_str() + 1, arg.size() - 1);

        if(!m_DB.HasKeytag(message->chat->id, keytag))
            continue;

        std::string reply = m_DB.GetTaggingMessage(message->chat->id, keytag);
        
        if(reply.size())
            getApi().sendMessage(message->chat->id, reply);
    }    
}

void UltimateTagBot::OnListTags(TgBot::Message::Ptr message){
    std::string reply = "Registered tags in this chat";
#if 0
    for(const auto &[keytag, tags]: m_TagMap){
        reply += '\n';
        reply += keytag + ":";

        for(const auto &tag: tags){
            reply += " " + tag;
        }
    }
#endif
    getApi().sendMessage(message->chat->id, reply);
}

void UltimateTagBot::Error(int32_t chat_id, std::string str){
    getApi().sendMessage(chat_id, str);
}