#include "bot.hpp"
#include <tgbot/net/TgLongPoll.h>
#include <exception>
#include <string_view>
#include "args_iterator.hpp"

int CountChars(char ch, const std::string &string){
    int count = 0;
    for(char e: string)
        count += e == ch;
    return count;
}

bool IsValidTag(const std::string &tag){
    return tag.size() > 1 && tag[0] == '@' && CountChars('@', tag) == 1;
}

bool IsValidKeytag(const std::string &tag){
    return tag.size() && !CountChars('@', tag);
}

UltimateTagBot::UltimateTagBot(const std::string &tag):
    TgBot::Bot(tag)
{
    getEvents().onNonCommandMessage([this](TgBot::Message::Ptr message){
        OnMessage(message);
    });
    getEvents().onCommand("new_keytag", [this](TgBot::Message::Ptr message){
        OnNewKeytag(message);
    });
    getEvents().onCommand("add_tag", [this](TgBot::Message::Ptr message){
        OnAddTag(message);
    });
    getEvents().onCommand("remove_tag", [this](TgBot::Message::Ptr message){
        OnRemoveTag(message);
    });
    getEvents().onCommand("list_keytags", [this](TgBot::Message::Ptr message){
        OnListKeytags(message);
    });
    getEvents().onCommand("list_tags", [this](TgBot::Message::Ptr message){
        OnListTags(message);
    });
}

int UltimateTagBot::Run(){
    {
        TgBot::TgLongPoll long_poll(*this);

        while(true){
            long_poll.start();
        }

    }

    return EXIT_SUCCESS;
}
void UltimateTagBot::OnNewKeytag(TgBot::Message::Ptr message){
    ArgsIterator it(message->text.c_str());
    it.Advance();
    if(!it)
        return Error(message->chat->id, "Please supply keytag name, /new_keytag [keytag]");

    std::string keytag = it.Current(); it.Advance();

    if(!IsValidKeytag(keytag))
        return Error(message->chat->id, "Conflicted keytag name '" + keytag + "', it should not contain '@'");
    
    if(m_DB.HasKeytag(message->chat->id, keytag))
        return Error(message->chat->id, "Keytag '" + keytag + "' is already created" );
    
    m_DB.CreateKeytag(message->chat->id, keytag);
}
void UltimateTagBot::OnDeleteKeytag(TgBot::Message::Ptr message){
    ArgsIterator it(message->text.c_str());
    it.Advance();
    if(!it)
        return Error(message->chat->id, "Please supply keytag name, /delete_keytag [keytag]");

    std::string keytag = it.Current(); it.Advance();

    if(!IsValidKeytag(keytag))
        return Error(message->chat->id, "Conflicted keytag name '" + keytag + "', it should not contain '@'");
    
    if(!m_DB.HasKeytag(message->chat->id, keytag))
        return Error(message->chat->id, "Keytag '" + keytag + "' does not exists" );
    
    m_DB.DestroyKeytag(message->chat->id, keytag);
}

void UltimateTagBot::OnAddTag(TgBot::Message::Ptr message){
    ArgsIterator it(message->text.c_str());
    it.Advance();
    if(!it)
        return Error(message->chat->id, "Please supply keytag name and tag arguments, /add_tag [keytag] [tag1] [tag2] .. [tagN]");

    std::string keytag = it.Current(); it.Advance();

    if(!IsValidKeytag(keytag))
        return Error(message->chat->id, "Conflicted keytag name '" + keytag + "', it should not contain '@'");

    if(!m_DB.HasKeytag(message->chat->id, keytag))
        return Error(message->chat->id, "Keytag '" + keytag + "' does not exist, use /new_keytag [keytag]");

    TagList tags;
    for(;it; ++it){
        auto tag = it.Current();
        if(IsValidTag(tag))
            if(!m_DB.HasKeytag(message->chat->id, tag))
                tags.push_back(tag);
            else 
                Error(message->chat->id, "Confilicted tag '" + tag + "', tag registered as keytag can't be added to tag list");
        else
            Error(message->chat->id, "Invalid tag '" + tag + "'");
    }

    if(!tags.size())
        return Error(message->chat->id, "No tags to add");

    m_DB.AddTagsFor(message->chat->id, keytag, tags);
}

void UltimateTagBot::OnRemoveTag(TgBot::Message::Ptr message){
    ArgsIterator it(message->text.c_str());
    it.Advance();
    if(!it)
        return Error(message->chat->id, "Please supply keytag name and tag arguments, /remove_tag [keytag] [tag1] [tag2] .. [tagN]");

    std::string keytag = it.Current(); it.Advance();

    if(!IsValidKeytag(keytag))
        return Error(message->chat->id, "Conflicted keytag name '" + keytag + "', it should not contain '@'");

    if(!m_DB.HasKeytag(message->chat->id, keytag))
        return Error(message->chat->id, "Keytag '" + keytag + "' does not exist, use /new_keytag [keytag]");

    TagList tags;
    for(;it; ++it){
        auto tag = it.Current();
        if(IsValidTag(tag))
            if(!m_DB.HasKeytag(message->chat->id, tag))
                tags.push_back(tag);
            else 
                Error(message->chat->id, "Confilicted tag '" + tag + "', tag registered as keytag can't be added to tag list, therefore can't be removed");
        else
            Error(message->chat->id, "Invalid tag '" + tag + "'");
    }

    if(!tags.size())
        return Error(message->chat->id, "No tags to remove");

    m_DB.RemoveTagsFor(message->chat->id, keytag, tags);
}

void UltimateTagBot::OnMessage(TgBot::Message::Ptr message){

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
    ArgsIterator it(message->text.c_str());
    it.Advance();
    if(!it)
        return Error(message->chat->id, "Please supply keytag name, /list_tags [keytag]");

    std::string keytag = it.Current(); it.Advance();

    if(!IsValidKeytag(keytag))
        return Error(message->chat->id, "Conflicted keytag name '" + keytag + "', it should not contain '@'");

    if(!m_DB.HasKeytag(message->chat->id, keytag))
        return Error(message->chat->id, "Keytag '" + keytag + "' does not exist, use /new_keytag [keytag]");

    std::string list = m_DB.GetTagsList(message->chat->id, keytag);

    if(list.size())
        getApi().sendMessage(message->chat->id, list);
    else
        Error(message->chat->id, "No tags added to keytag '" + keytag + "', use /add_tag [keytag] [tag1] [tag2] .. [tagN]");

}

void UltimateTagBot::OnListKeytags(TgBot::Message::Ptr message){
    std::string list = m_DB.GetKeytagsList(message->chat->id);

    if(list.size())
        getApi().sendMessage(message->chat->id, list);
    else
        Error(message->chat->id, "No keytags created, use /new_keytag [keytag] to create one.");
}

void UltimateTagBot::Error(int64_t chat_id, std::string str){
    getApi().sendMessage(chat_id, str);
}