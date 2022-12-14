#include "tag_database.hpp"
#include <nlohmann/json.hpp>
#include <iostream>
#include <fstream>
#include "utils.hpp"

using json = nlohmann::json;

JsonTagDatabase::JsonTagDatabase(const char *filename):
    m_Filename(filename)
{
    m_Map = json::parse(ReadEntireFile(filename));
}

void JsonTagDatabase::CreateKeytag(int64_t chat_id, const std::string &keytag){
    m_Map[chat_id][keytag];

    SaveToFile();
}

bool JsonTagDatabase::HasKeytag(int64_t chat_id, const std::string &keytag){
    TagMap &tag_map = m_Map[chat_id];

    return tag_map.find(keytag) != tag_map.end();
}

void JsonTagDatabase::DestroyKeytag(int64_t chat_id, const std::string &keytag){
    m_Map[chat_id].erase(keytag);

    SaveToFile();
}

bool JsonTagDatabase::HasTag(int64_t chat_id, const std::string &keytag, const std::string &tag){
    if(!HasKeytag(chat_id, keytag))
        return false;
    TagSet &tag_set = m_Map[chat_id][keytag];

    return tag_set.find(tag) != tag_set.end(); 
}

void JsonTagDatabase::AddTagsFor(int64_t chat_id, const std::string &keytag, const std::vector<std::string> &tags){
    assert(HasKeytag(chat_id, keytag));
    TagSet &tag_set = m_Map[chat_id][keytag];

    for(const auto &tag: tags)
        tag_set.insert(tag);

    SaveToFile();
}

void JsonTagDatabase::RemoveTagsFor(int64_t chat_id, const std::string &keytag, const std::vector<std::string> &tags){
    assert(HasKeytag(chat_id, keytag));
    TagSet &tag_set = m_Map[chat_id][keytag];

    for(const auto &tag: tags)
        tag_set.erase(tag);
    
    SaveToFile();
}
const TagSet &JsonTagDatabase::GetTagsFor(int64_t chat_id, const Keytag &keytag){
    return m_Map[chat_id][keytag];
}

std::string JsonTagDatabase::GetKeytagsList(int64_t chat_id){
    std::string res;
    for(const auto &[keytag, _]: m_Map[chat_id])
        res += keytag + '\n';

    if(res.size())
        res.back() = '\0';
    return res;
}

std::string JsonTagDatabase::GetTagsList(int64_t chat_id, const std::string &keytag){
    if(!HasKeytag(chat_id, keytag))
        return {};

    std::string res;    
    for(const auto &tag: m_Map[chat_id][keytag])
        res += tag + '\n';
    
    if(res.size())
        res.back() = '\0';

    return res;
}

void JsonTagDatabase::SaveToFile(){
    WriteEntireFile(m_Filename, json(m_Map).dump());
}
