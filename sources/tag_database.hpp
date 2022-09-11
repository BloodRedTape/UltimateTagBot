#pragma once

#include <set>
#include <vector>
#include <string>
#include <unordered_map>
#include "keytag.hpp"

using TagSet = std::set<std::string>;
using TagMap = std::unordered_map<std::string, TagSet>;
using ChatMap = std::unordered_map<int64_t, TagMap>;

class JsonTagDatabase{
private:
    ChatMap m_Map;
    const char *m_Filename;
public:
    JsonTagDatabase(const char *filename);

    void CreateKeytag(int64_t chat_id, const std::string &keytag);

    bool HasKeytag(int64_t chat_id, const std::string &keytag);

    void DestroyKeytag(int64_t chat_id, const std::string &keytag);

    bool HasTag(int64_t chat_id, const std::string &keytag, const std::string &tag);

    void AddTagsFor(int64_t chat_id, const std::string &keytag, const std::vector<std::string> &tags);

    void RemoveTagsFor(int64_t chat_id, const std::string &keytag, const std::vector<std::string> &tags);

    const TagSet &GetTagsFor(int64_t chat_id, const Keytag &keytag);

    std::string GetTaggingMessage(int64_t chat_id, const std::string &keytag);

    std::string GetKeytagsList(int64_t chat_id);

    std::string GetTagsList(int64_t chat_id, const std::string &keytag);
private:
    void SaveToFile();
};