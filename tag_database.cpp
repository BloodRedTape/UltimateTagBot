#include "tag_database.hpp"

void TagDatabase::CreateKeytag(int32_t chat_id, const std::string &keytag){
    m_Map[chat_id][keytag];
}

bool TagDatabase::HasKeytag(int32_t chat_id, const std::string &keytag){
    TagMap &tag_map = m_Map[chat_id];

    return tag_map.find(keytag) != tag_map.end();
}

void TagDatabase::DestroyKeytag(int32_t chat_id, const std::string &keytag){
    m_Map[chat_id].erase(keytag);
}

bool TagDatabase::HasTag(int32_t chat_id, const std::string &keytag, const std::string &tag){
    if(!HasKeytag(chat_id, keytag))
        return false;
    TagSet &tag_set = m_Map[chat_id][keytag];

    return tag_set.find(tag) != tag_set.end(); 
}

void TagDatabase::AddTagsFor(int32_t chat_id, const std::string &keytag, const std::vector<std::string> &tags){
    TagSet &tag_set = m_Map[chat_id][keytag];

    for(const auto &tag: tags)
        tag_set.insert(tag);
}

void TagDatabase::RemoveTagsFor(int32_t chat_id, const std::string &keytag, const std::vector<std::string> &tags){
    TagSet &tag_set = m_Map[chat_id][keytag];

    for(const auto &tag: tags)
        tag_set.erase(tag);
}

std::string TagDatabase::GetTaggingMessage(int32_t chat_id, const std::string &keytag){
    if(!HasKeytag(chat_id, keytag)) 
        return {};
    
    TagSet &set = m_Map[chat_id][keytag];

    std::string message;

    for(const auto &tag: set)
        message += tag + ' ';

    return message;
}

std::string TagDatabase::GetKeytagsList(int32_t chat_id){
    std::string result;

    for(const auto &keytag: m_Map[chat_id]){

    }
    return {};
}