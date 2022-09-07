#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <tgbot/Bot.h>

using TagList = std::vector<std::string>;

class UltimateTagBot: TgBot::Bot{
private:
    std::unordered_map<std::string, TagList> m_TagMap;
public:
    UltimateTagBot(const std::string &tag);

    int Run();

    void OnAddTag(TgBot::Message::Ptr message);

    void OnRemoveTag(TgBot::Message::Ptr message);

    void OnMessage(TgBot::Message::Ptr message);
};