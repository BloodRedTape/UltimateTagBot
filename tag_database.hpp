#pragma once

#include <set>
#include <vector>
#include <string>
#include <unordered_map>

using TagSet = std::set<std::string>;
using TagMap = std::unordered_map<std::string, TagSet>;
using ChatMap = std::unordered_map<int32_t, TagMap>;

class TagDatabase{
private:
    ChatMap m_Map;
public:
    void CreateKeytag(int32_t chat_id, const std::string &keytag);

    bool HasKeytag(int32_t chat_id, const std::string &keytag);

    void DestroyKeytag(int32_t chat_id, const std::string &keytag);

    bool HasTag(int32_t chat_id, const std::string &keytag, const std::string &tag);

    void AddTagsFor(int32_t chat_id, const std::string &keytag, const std::vector<std::string> &tags);

    void RemoveTagsFor(int32_t chat_id, const std::string &keytag, const std::vector<std::string> &tags);

    std::string GetTaggingMessage(int32_t chat_id, const std::string &keytag);

    std::string GetKeytagsList(int32_t chat_id);
};