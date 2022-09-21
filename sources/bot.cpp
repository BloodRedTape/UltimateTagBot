#include "bot.hpp"
#include <tgbot/net/TgLongPoll.h>
#include <exception>
#include <string_view>
#include "args_iterator.hpp"
#include "utils.hpp"
#include "tag.hpp"

bool IsValidTag(const std::string &tag){
    return tag.size() > 1 && tag[0] == '@' && CountChars('@', tag) == 1;
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
    getEvents().onCommand("delete_keytag", [this](TgBot::Message::Ptr message){
        OnDeleteKeytag(message);
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

    Keytag keytag = it.Current(); it.Advance();

    if(!keytag)
        return Error(message->chat->id, "Conflicted keytag name '" + keytag + "', it should not contain '@' and '#'");
    
    if(m_DB.HasKeytag(message->chat->id, keytag))
        return Error(message->chat->id, "Keytag '" + keytag + "' is already created" );
    
    m_DB.CreateKeytag(message->chat->id, keytag);

    Error(message->chat->id, "created new keytag '" + keytag + "'");
}
void UltimateTagBot::OnDeleteKeytag(TgBot::Message::Ptr message){
    ArgsIterator it(message->text.c_str());
    it.Advance();
    if(!it)
        return Error(message->chat->id, "Please supply keytag name, /delete_keytag [keytag]");

    Keytag keytag = it.Current(); it.Advance();

    if(!keytag)
        return Error(message->chat->id, "Conflicted keytag name '" + keytag + "', it should not contain '@' and '#'");
    
    if(!m_DB.HasKeytag(message->chat->id, keytag))
        return Error(message->chat->id, "Keytag '" + keytag + "' does not exists" );
    
    m_DB.DestroyKeytag(message->chat->id, keytag);

    Error(message->chat->id, "deleted keytag '" + keytag + "'");
}

void UltimateTagBot::OnAddTag(TgBot::Message::Ptr message){
    ArgsIterator it(message->text.c_str());
    it.Advance();
    if(!it)
        return Error(message->chat->id, "Please supply keytag name and tag arguments, /add_tag [keytag] [tag1] [tag2] .. [tagN]");

    Keytag keytag = it.Current(); it.Advance();

    if(!keytag)
        return Error(message->chat->id, "Conflicted keytag name '" + keytag + "', it should not contain '@'");

    if(!m_DB.HasKeytag(message->chat->id, keytag))
        return Error(message->chat->id, "Keytag '" + keytag + "' does not exist, use /new_keytag [keytag]");

    TagList tags;
    for(;it; ++it){
        Tag tag = it.Current();
        if(tag)
            if(!m_DB.HasKeytag(message->chat->id, tag))
                tags.push_back(tag);
            else 
                Error(message->chat->id, "Confilicted tag '" + tag + "', tag registered as keytag can't be added to tag list");
        else
            Error(message->chat->id, "Invalid tag '" + it.Current() + "'");
    }

    if(!tags.size())
        return Error(message->chat->id, "No tags to add");

    m_DB.AddTagsFor(message->chat->id, keytag, tags);
    std::string added_tags;
    for(const auto &tag: tags)
        added_tags += tag + ' ';
    Error(message->chat->id, "added " + added_tags + "to keytag '" + keytag + "'");
}

void UltimateTagBot::OnRemoveTag(TgBot::Message::Ptr message){
    ArgsIterator it(message->text.c_str());
    it.Advance();
    if(!it)
        return Error(message->chat->id, "Please supply keytag name and tag arguments, /remove_tag [keytag] [tag1] [tag2] .. [tagN]");

    Keytag keytag = it.Current(); it.Advance();

    if(!keytag)
        return Error(message->chat->id, "Conflicted keytag name '" + keytag + "', it should not contain '@' and '#'");

    if(!m_DB.HasKeytag(message->chat->id, keytag))
        return Error(message->chat->id, "Keytag '" + keytag + "' does not exist, use /new_keytag [keytag]");

    TagList tags;
    for(;it; ++it){
        Tag tag = it.Current();
        if(tag)
            if(!m_DB.HasKeytag(message->chat->id, tag))
                tags.push_back(tag);
            else 
                Error(message->chat->id, "Confilicted tag '" + tag + "', tag registered as keytag can't be added to tag list, therefore can't be removed");
        else
            Error(message->chat->id, "Invalid tag '" + it.Current() + "'");
    }

    if(!tags.size())
        return Error(message->chat->id, "No tags to remove");

    m_DB.RemoveTagsFor(message->chat->id, keytag, tags);

    std::string removed_tags;
    for(const auto &tag: tags)
        removed_tags += tag + ' ';
    Error(message->chat->id, "removed " + removed_tags + "from keytag '" + keytag + "'");
}

template <typename T, typename E>
void PushBackUnique(T &collection, E element){
    bool found = false;
    for(const E &e: collection){
        if(e == element){
            found = true;
            break;
        }
    }
    if(!found)
        collection.push_back(std::move(element));
}

void UltimateTagBot::OnMessage(TgBot::Message::Ptr message){
    std::vector<Keytag> keytags;

    for(ArgsIterator it(message->text.c_str()); it; ++it){
        std::string arg = it.Current();

        if(arg[0] != '@')
            continue;

        std::string keytag_text;
        for(char ch: arg.substr(1, arg.size() - 1)){
            if(!Keytag::IsKeytagChar(ch))
                break;

            keytag_text.push_back(ch);
        }

        Keytag keytag = keytag_text; 

        if(!keytag)
            continue;

        PushBackUnique(keytags, std::move(keytag));
    }    

    for(const auto &keytag: keytags){
        if(!m_DB.HasKeytag(message->chat->id, keytag))
            continue;

        const auto &tag_set = m_DB.GetTagsFor(message->chat->id, keytag);

        int tags = 0;
        std::string reply;
        for(const auto &tag: tag_set){
            reply += '@' + tag + ' ';
            tags++;

            if(tags % MaxTagsInMessage == 0){
                getApi().sendMessage(message->chat->id, reply);
                reply = {};
            }
        } 
        if(reply.size())
            getApi().sendMessage(message->chat->id, reply);
    }
}

void UltimateTagBot::OnListTags(TgBot::Message::Ptr message){
    ArgsIterator it(message->text.c_str());
    it.Advance();
    if(!it)
        return Error(message->chat->id, "Please supply keytag name, /list_tags [keytag]");

    Keytag keytag = it.Current(); it.Advance();

    if(!keytag)
        return Error(message->chat->id, "Conflicted keytag name '" + keytag + "', it should not contain '@' and '#'");

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
