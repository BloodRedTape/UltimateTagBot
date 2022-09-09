#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <tgbot/Bot.h>
#include "tag_database.hpp"

using TagList = std::vector<std::string>;

class UltimateTagBot: TgBot::Bot{
private:
    JsonTagDatabase m_DB = "db.json";
public:
    UltimateTagBot(const std::string &tag);

    int Run();
private:
    void OnNewKeytag(TgBot::Message::Ptr message);

    void OnDeleteKeytag(TgBot::Message::Ptr message);

    void OnAddTag(TgBot::Message::Ptr message);

    void OnRemoveTag(TgBot::Message::Ptr message);

    void OnMessage(TgBot::Message::Ptr message);

    void OnListTags(TgBot::Message::Ptr message);

    void OnListKeytags(TgBot::Message::Ptr message);

    void Error(int32_t char_id, std::string str);
};